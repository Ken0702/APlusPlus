#!/usr/bin/env python
# script to check which of the jobs were already successfully carried out
# this file was copied from $APP_SCHANNEL/check_submissions.py
#
# FOR USER CONFIG OPTIONS, SEE BOTTOM OR LAUNCH WITH THE -h FLAG
#
""" Check which of the jobs were already successfully carried out.

Takes a set of users and their submission files containing the systematics.
Then checks how many samples were submitted/completed for each user via rucio.
"""

# ------- imports --------------------------------------------------------------------- #
from __future__ import print_function

import collections
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import SafeConfigParser as ConfigParser
import datetime
import json
import os
import cPickle as pickle
import re
import subprocess
import sys

import pandatools.Client
import rucio.client

# ------- global variables ------------------------------------------------------------ #
rucio_client = rucio.client.Client()
# earliest_date = datetime.datetime.utcnow() - datetime.timedelta(days=28)  # days < 30
earliest_date = datetime.datetime(2019, 1, 15)
debug = 0

# regex
rexScope = re.compile(r'^(user\.\w+)\.')
# rexDid = re.compile(r'^(user\.\w+)\.(mc|data).*\.(v[0-9a-z]+)(?:\.[0-9])?_out\.root$')
# rexDid = re.compile(r'^(user\.\w+)\.(\w+)\.([0-9a-zA-Z]+)_(\w+)_(v\w+)_(a|d|e)\.(v\d+)' +
#                     r'(_(?:app|mem)_output\.root|\.log)(?:\.[0-9]+)?$')
# v29 update:
rexDid = re.compile(r'^(user\.\w+)\.(\w+)\.([a-zA-Z]+)_(\w+)_(v\w+)\.(v\w+)' +
                    r'(_(?:app|mem)_output\.root|\.log)(?:\.[0-9]+)?$')

rexTask = re.compile(r'^(user\.\w+)\.(\w+)\.([0-9a-zA-Z]+)_(\w+)_(v\w+)_(a|d|e)\.(v\d+)$')
rexMachine = re.compile(r'^/rdsk/dat([ms])([1-8])/.*$')
rexCutEnd = re.compile(r'(^.*?)(?:\.[0-9]+)?$')
rexGridOutputDir = re.compile(r'^(user\.\w+)\.(\w+)\.([0-9]*[a-zA-Z]+[0-9]*)_(\w+)_(v\w+)\.(v\w+)' +
                              r'(_(?:app|mem)_output\.root|\.log)(?:\.[0-9]+)?$')
rexGridOutputFile = re.compile(r'^(user\.\w+)\.([0-9]+)\.(\w+)\.([0-9]+)\._([0-9]+)' +
                               r'\.((?:app|mem)_output\.root)$')
rexGridOutputFileData = re.compile(r'^(user\.\w+)\.AllYear\.grp(1[5-8])_(\w+)\.([0-9]+)' +
                                   r'\._([0-9]+)\.((?:app|mem)_output\.root)$')


# ------- functions ------------------------------------------------------------------- #
def _mkdir(target):
    """Create directory without error if it exists (like mkdir -p)."""
    try:
        os.makedirs(target)
    except OSError as e:
        if e.errno != 17:
            raise


def read_lines_from_file(file_):
    """Read file and return list of non-empty lines, ignoring lines starting with #."""
    lines = []
    with open(file_) as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            lines.append(line.strip())
    return lines


def find_DIDsample_in_DSIDs_list(search_string, DSIDs_file):
    """Find the DIDsample name expected by A++ by searching a DSIDs list."""
    with open(DSIDs_file) as f:
        for line in f:
            print(line)
            print(search_string)
            if line.startswith('#') or line.isspace():
                if line.isspace(): 
                    print("SKIPLINE")
                continue
            if line.find(search_string) != -1:
                print("KEEP")
                print(search_string)
		#if "data" in search_string: 
                #    quit()
                return line.split(',')[0]
    # if not found:
    print(search_string)
    print("NONE")
    return None


# ------- classes --------------------------------------------------------------------- #
# --- begin class campaign ------------------------------------------------------------ #
class Campaign(object):
    """Top level object to store all tasks, samples, systematics."""
    config_defaults = {
        # general
        'campaign': 'schannel_App_MemTk_test',
        'ntup_version': 'v29lj_MC16a',
        'grid_version': 'v11',
        'verbose': 0,
        'submission_date': '2019-11-01',
        # download
        'download_dir': 'None',
        'hadd_target_dir': 'None',
        'DSIDs_file': 'None',
        }

    # --- class campaign -------------------------------------------------------------- #
    def __init__(self, config_file='config.txt'):
        print('Creating campaign from configuration file {}'.format(config_file))
        # general
        self.cparser = ConfigParser(defaults=Campaign.config_defaults)
        self.cparser.read(config_file)
        self.name = self.cparser.get('main', 'campaign')
        self.ntup_version = self.cparser.get('main', 'ntup_version')
        self.grid_version = self.cparser.get('main', 'grid_version')
        self.version_filter = r'.*{}\.{}/?$'.format(self.ntup_version, self.grid_version)
        self.verbose = self.cparser.getint('main', 'verbose')
        submission_date = self.cparser.get('main', 'submission_date')
        try:
            self.earliest_date = datetime.datetime.strptime(submission_date, '%Y-%m-%d')
        except ValueError as e:
            print('Caught ValueError: {}; using default of 2019-01-15'.format(e))
            self.earliest_date = earliest_date
        self.file_extension = '_mem_output.root'
        # users, systematics, samples configuration
        self.users = {}
        self.systematic_names = []
        self.syst_long2short = {}
        self.syst_short2long = {}
        self.sample_names = []
        self.sample_config = {}
        self.sample_split = {}
        self.dids = {}
        self.expected_partial_dids = []
        # status
        self.n_dids_distributed = 0  # dids distributed via users
        self.n_dids_done = 0
        self.n_dids_missing = 0
        self.status = None
        # download
        self.download_dir = self.cparser.get('main', 'download_dir')
        if self.download_dir == 'None':
            self.download_dir = None
        self.hadd_target_dir = self.cparser.get('main', 'hadd_target_dir')
        if self.hadd_target_dir == 'None':
            self.hadd_target_dir = None
        self.DSIDs_file = self.cparser.get('main', 'DSIDs_file')
        if self.DSIDs_file == 'None':
            self.DSIDs_file = None

        # initialize users, samples, systematics long/short
        self.initialize_users()
        self.initialize_samples()
        self.initialize_systematics()
        sys.stdout.flush()  # avoid buffering when piping

    # --- class campaign -------------------------------------------------------------- #
    def initialize_users(self):
        names = []
        full_names = []
        if self.cparser.has_option('users', 'names'):
            names = json.loads(self.cparser.get('users', 'names'))
        if self.cparser.has_option('users', 'full_names'):
            full_names = json.loads(self.cparser.get('users', 'full_names'))
        if len(names) == len(full_names):
            for name, full_name in zip(names, full_names):
                self.add_user_checked_by_name(name, full_name)
        if len(self.users) > 0:
            print('  Found users ' + ', '.join(sorted(self.users.keys())))
        else:
            print('  Did not find users in config file')

    # --- class campaign -------------------------------------------------------------- #
    def initialize_samples(self):
        """Search and add samples to configuration.

Samples are taken from keys of a sample_config_file if available, otherwise
from the all_samples_file if available."""
        all_samples = []
        sample_file = ""
        if self.cparser.has_option('main', 'sample_config_file'):
            sample_file = self.cparser.get('main', 'sample_config_file')
            with open(sample_file) as f:
                self.sample_config = json.load(f)
            all_samples = self.sample_config.keys()
            # find sample splits
            for samplename in all_samples:
                nsubjobs = self.sample_config[samplename].get('nsubjobs')
                subjobid = self.sample_config[samplename].get('subjobid')
                if nsubjobs is None or subjobid is None:
                    continue
                self.sample_split[samplename] = (nsubjobs, subjobid)
        elif self.cparser.has_option('main', 'all_samples_file'):
            sample_file = self.cparser.get('main', 'all_samples_file')
            all_samples = read_lines_from_file(sample_file)
        for samplename in all_samples:
            self.add_sample_name(samplename)
        if len(all_samples) > 0:
            print('  Found {} samples in {}'.format(len(self.sample_names),
                                                    sample_file))

    # --- class campaign -------------------------------------------------------------- #
    def initialize_systematics(self):
        if self.cparser.has_option('main', 'all_systs_short_file'):
            all_systs_short_file = self.cparser.get('main', 'all_systs_short_file')
            lines = read_lines_from_file(all_systs_short_file)
            for line in lines:
                syst, syst_short = line.strip().split()[0:2]
                self.add_systematic_name(syst)
                self.syst_long2short[syst] = syst_short
                self.syst_short2long[syst_short] = syst
            print('  Found {} systematics in {}'.format(len(self.systematic_names),
                                                        all_systs_short_file))

    # --- class campaign -------------------------------------------------------------- #
    def add_sample_name(self, name):
        if name not in self.sample_names:
            self.sample_names.append(name)

    # --- class campaign -------------------------------------------------------------- #
    def add_systematic_name(self, name):
        # find systematic in list
        if name not in self.systematic_names:
            self.systematic_names.append(name)

    # --- class campaign -------------------------------------------------------------- #
    def add_user_checked_by_name(self, name, full_name=None):
        """Add user if not already added, return User object."""
        user = None
        if name in self.users.keys():
            user = self.users[name]
            if full_name is not None and user.full_name is None:
                user.full_name = full_name
        else:
            user = User(name, full_name, self)
            user.verbose = self.verbose
            self.users[name] = user
        return user

    # --- class campaign -------------------------------------------------------------- #
    def add_user_systematics_samples(self, username, systfile, samplefile):
        """Add systematics and samples to a user.

The user with name USERNAME is created and added to self if non-existent,
gets assigned all systematics found in SYSTFILE, and all samples found in
SAMPLEFILE are attached to each systematic."""
        user = self.add_user_checked_by_name(username)
        systematics = read_lines_from_file(systfile)
        samples = read_lines_from_file(samplefile)
        print('  Adding {} systematics with {} samples for user {}'.format(
            len(systematics), len(samples), username))
        first_systematic = True
        for systematicname in systematics:
            if systematicname not in self.systematic_names:
                print('    Systematic {} unknown in this campaign; skip'.format(
                    systematicname))
                continue
            systematic = user.systematics.get(systematicname)
            if systematic is None:
                systematic = Systematic(systematicname, self.syst_long2short[systematicname])
                user.add_systematic(systematic)
            # self.add_systematic_name(systematicname)
            for samplename in samples:
                sample = systematic.samples.get(samplename)
                if sample is None:
                    sample = Sample(samplename)
                    systematic.add_sample(sample)
                # self.add_sample_name(samplename)
                if first_systematic and samplename not in self.sample_names:
                    print('    Sample {} unknown in this campaign. Track manually.'.format(
                        samplename))
            first_systematic = False

    # --- class campaign -------------------------------------------------------------- #
    def update_task_status(self, check_has_dids=True):
        """Update the users task status dictionary via panda.

Per default, CHECK_HAS_DIDS=True requires the user to have attached DIDs to avoid
checking irrelevant users. The user query is filtered using this campaign's
version string."""
        print('Checking panda to update task status since {}..'.format(self.earliest_date.strftime('%Y-%m-%d')))
        for username in sorted(self.users.keys()):
            user = self.users[username]
            if check_has_dids and len(user.dids) == 0:
                print('  Skip user {} who has no DIDs'.format(username))
                continue
            user.update_task_status(self.version_filter)

    # --- class campaign -------------------------------------------------------------- #
    def read_task_status(self, filename):
        """Read the task status for all self.users from FILENAME."""
        print('Reading user task status dictionaries from {}'.format(filename))
        if not os.path.exists(filename):
            print('  File {} does not exist, skip.'.format(filename))
            return
        with open(filename, 'rb') as f:
            global_task_dict = pickle.load(f)
        for username in sorted(self.users.keys()):
            user = self.users[username]
            user_task_dict = global_task_dict.get(user.name, {})
            user.task_dict.update(user_task_dict)
            if self.verbose > 0:
                print('  Read in {:3d} tasks for user {}'.format(len(user_task_dict), username))
        print('  Read task status for {} users'.format(len(self.users)))

    # --- class campaign -------------------------------------------------------------- #
    def dump_task_status(self, filename):
        """Save the task status of all self.users to FILENAME.

The dumped object is a dict of the form {<username>: <user.task_dict>}."""
        print('Dumping user task status dictionaries to {}'.format(filename))
        global_task_dict = {}
        for user in self.users.values():
            global_task_dict[user.name] = user.task_dict
        with open(filename, 'wb') as f:
            pickle.dump(global_task_dict, f)

    # --- class campaign -------------------------------------------------------------- #
    def build_expected_datasets(self, recurse=True):
        """Create lists/dicts of expected datasets from configuration.

A list of expected partial (without scope) DIDs is created from the syst/sample
configuration. If RECURSE is not turned off, a format string is given to each
user to create the expected full DID names in user.build_expected_datasets."""
        print('Building lists of expected datasets')
        # build list of dids without scope
        self.expected_partial_dids = []
        for syst_name in self.systematic_names:
            for sample_name in self.sample_names:
                # hardcode nominal and data/qcd names here
                # if not nominal and data or qcd, then skip
                if (syst_name.lower().find('nominal') < 0 and
                    (sample_name.lower().find('data') >= 0 or
                     sample_name.lower().find('qcd') >= 0)):
                    continue
                partial_did = '{}.{}_{}_{}.{}{}'.format(
                    self.name, sample_name, self.syst_long2short[syst_name],
                    self.ntup_version, self.grid_version, self.file_extension)
                self.expected_partial_dids.append(partial_did)
        print('  Total expected datasets: {}'.format(len(self.expected_partial_dids)))
        # build user dids for user-attached systs+samples
        format_string = 'user.{}.{}.{}_{}_{}.{}{}'.format(
            '{USER}', self.name, '{SAMPLE}', '{SYSTEMATIC}',
            self.ntup_version, self.grid_version, self.file_extension)
        if recurse:
            for username in sorted(self.users.keys()):
                user = self.users[username]
                user.build_expected_datasets(format_string)
        sys.stdout.flush()  # avoid buffering when piping

    # --- class campaign -------------------------------------------------------------- #
    def update_did_production_status(self):
        """Set the DIDs' status with rucio for each user."""
        print('Updating status of dataset production progress')
        sys.stdout.flush()  # avoid buffering when piping
        for username in sorted(self.users.keys()):
            user = self.users[username]
            user.update_did_rucio_status()
            self.dids.update(user.dids)
        sys.stdout.flush()  # avoid buffering when piping

    # --- class campaign -------------------------------------------------------------- #
    def update_did_task_status(self):
        """Update the task status of all user-attached DIDs from panda."""
        print('Updating the task status for all users')
        for username in sorted(self.users.keys()):
            user = self.users[username]
            user.update_did_task_status()
        sys.stdout.flush()  # avoid buffering when piping

    # --- class campaign -------------------------------------------------------------- #
    def update_download_status(self, require_panda_done=True):
        """Check completeness of local files for each user's DIDs with rucio."""
        print('Checking downloaded datasets in {}'.format(
            self.download_dir if self.download_dir is not None else 'user-specific paths'))
        sys.stdout.flush()  # avoid buffering when piping
        for username in sorted(self.users.keys()):
            user = self.users[username]
            if user.download_dir is None:
                user.download_dir = self.download_dir
            user.update_download_status(require_panda_done)
            self.dids.update(user.dids)

    # --- class campaign -------------------------------------------------------------- #
    def update_statistics(self, recurse=True):
        """Check DIDs and optionally users for 'done' or 'incomplete' statuses."""
        print('  Updating statistics')
        n_total = len(self.dids)
        did_progress = collections.Counter()
        for didname in sorted(self.dids.keys()):
            did = self.dids[didname]
            did_progress[did.rucio_status] += 1
        self.n_dids_distributed = n_total
        self.n_dids_done = did_progress['done']
        self.n_dids_missing = did_progress['missing']
        self.status = 'done' if n_total == did_progress['done'] else 'incomplete'
        if recurse:
            user_progress = collections.Counter()
            for username in sorted(self.users.keys()):
                user = self.users[username]
                user.update_statistics(recurse)
                user_progress[user.status] += 1
            self.n_users_done = user_progress['done']
            self.n_users_incomplete = user_progress['incomplete']

    # --- class campaign -------------------------------------------------------------- #
    def check_expected_produced(self):
        # what to compare?
        # - full list of dids (self.expected_partial_dids)
        # - dids associated to users
        # - show remaining dids
        print('  Comparing expected and produced datasets')
        expected_partial_dids = list(self.expected_partial_dids)
        for i in range(len(expected_partial_dids) - 1, -1, -1):
            partial_did = expected_partial_dids[i]
            for user in self.users.values():
                for userdid in user.dids.keys():
                    if userdid.find(partial_did) >= 0:
                        expected_partial_dids.pop(i)
        print('    There are {} missing datasets'.format(len(expected_partial_dids)))
        if self.verbose > 1:
            for partial_did in sorted(expected_partial_dids):
                print('    Expected DID not distributed:', partial_did)
        sys.stdout.flush()  # avoid buffering when piping

    # --- class campaign -------------------------------------------------------------- #
    def download_datasets(self, require_panda_done=True, only_print_to_list=None):
        """Download datasets for each user."""
        if only_print_to_list is None:
            print('Downloading datasets to {}'.format(
                self.download_dir if self.download_dir is not None else 'user-specific paths'))
        else:
            print('Printing list of datasets to be downloaded to {}'.format(
                only_print_to_list))
        for username in sorted(self.users.keys()):
            user = self.users[username]
            if user.download_dir is None:
                user.download_dir = self.download_dir
            user.download_datasets(require_panda_done, only_print_to_list)

    # --- class campaign -------------------------------------------------------------- #
    def convert_datasets(self, require_downloaded=True):
        """Convert datasets from grid format to A++ format for each user."""
        print('Converting datasets to {}'.format(
            self.hadd_target_dir if self.hadd_target_dir is not None else 'user-specific paths'))
        for username in sorted(self.users.keys()):
            user = self.users[username]
            if user.hadd_target_dir is None:
                user.hadd_target_dir = self.hadd_target_dir
            user.convert_datasets(require_downloaded)

    # --- class campaign -------------------------------------------------------------- #
    def print_summary(self, recurse=True):
        self.update_statistics(recurse)
        print('Summary for campaign {}, {}'.format(self.name, self.version_filter))
        print(' < this part on campaign-wide dids is not correct in case the sample config changed >')
        print('  datasets:')
        print('    {:4d}/{:4d}/{:4d} DIDs done/distributed/expected'.format(
            self.n_dids_done, self.n_dids_distributed, len(self.expected_partial_dids)))
        # print('    {:4d}/{:4d} DIDs done'.format(self.n_dids_done, self.n_dids_distributed))
        print('         {:4d}      DIDs not distributed to any user'.format(
            len(self.expected_partial_dids)-self.n_dids_distributed))
        print('    {:4d}           DIDs missing on grid'.format(self.n_dids_missing))
        # print('    {:4d}/{:4d} DIDs missing on grid'.format(self.n_dids_missing, self.n_dids_distributed))
        # print('    {:4d}/{:4d} DIDs downloaded'.format(self.n_dids_downloaded, self.n_dids_distributed))
        if recurse:
            print('\n\nSummary for users using rucio:')
            for username in sorted(self.users.keys()):
                user = self.users[username]
                if len(user.dids) > 0:
                    user.print_summary()
            print('\nSummary using panda:')
            for username in sorted(self.users.keys()):
                user = self.users[username]
                if len(user.dids) > 0:
                    user.print_summary_panda()
            print('\nSummary of downloaded samples:')
            for username in sorted(self.users.keys()):
                user = self.users[username]
                if len(user.dids) > 0:
                    user.print_summary_download()
            print('\nSummary of converted samples:')
            for username in sorted(self.users.keys()):
                user = self.users[username]
                if len(user.dids) > 0:
                    user.print_summary_convert()
        # print('  systematics:')
        # print('    {:3d}/{:3d} done'.format())
        # print('  {} systematics, {} samples'.format(len(self.systs), len(self.samples)))
        # print('  {} datasets expected'.format(len(self.expected_datasets_scopeless)))
        # print('  {} datasets produced'.format(len(self.produced_datasets)))
# --- end class campaign -------------------------------------------------------------- #


# --- begin class user ---------------------------------------------------------------- #
class User(object):
    """User object"""
    def __init__(self, name, full_name=None, campaign=None):
        # general
        self.name = name
        self.full_name = full_name
        self.campaign = campaign
        self.verbose = 0 if campaign is None else campaign.verbose
        # systematics, samples, did configuration
        self.systematics = {}
        self.dids = {}
        # rucio status
        self.n_dids_distributed = 0
        self.n_dids_done = 0
        self.n_dids_missing = 0
        self.status = None
        # panda/jedi task status
        self.jedi_tasks = {}  # dict with JediTask objects; do I need that?
        self.systematics_task_status_counter = None
        self.task_dict = {}
        self.task_status_counter = None
        # download
        self.download_dir = None
        self.download_status_counter = None
        self.systematics_download_status_counter = None
        # convert
        self.hadd_target_dir = None
        self.convert_status_counter = None
        self.systematics_convert_status_counter = None

    # --- class user ------------------------------------------------------------------ #
    def add_systematic(self, systematic):
        """Add a systematic object to this user's systematic dictionary."""
        self.systematics[systematic.name] = systematic

    # --- class user ------------------------------------------------------------------ #
    def get_systematic(self, systematic_name):
        """Get a systematic object by name from this user's dictionary."""
        return self.systematics[systematic_name]

    # --- class user ------------------------------------------------------------------ #
    def update_task_status(self, filter_=''):
        """Update this user's task status dictionary via panda.

The 'filter_' option is a regex filter applied to the tasks' taskName."""
        print('  Checking task status for user {}'.format(self.name))
        sys.stdout.flush()  # avoid buffering when piping
        if self.full_name is None:
            print('  No full name known, cannot check for tasks')
            return
        state, jediTask_dict = pandatools.Client.getJobIDsJediTasksInTimeRange(
            self.campaign.earliest_date, dn=self.full_name)
        if state != 0 or jediTask_dict is None:
            print('  pandatools.Client.getJobIDsJediTasksInTimeRange failed with error',
                  state)
            return
        filtered_dict = {v['jediTaskID']: v for k, v in jediTask_dict.items()
                         if re.findall(filter_, v['taskName'])}
        # also create JediTask objects
        for jediTaskID, task_dict in filtered_dict.items():
            jedi_task = JediTask(task_dict)
            self.jedi_tasks[jediTaskID] = jedi_task
        print('    Found {} tasks'.format(len(filtered_dict)))
        self.task_dict.update(filtered_dict)
        sys.stdout.flush()  # avoid buffering when piping

    # --- class user ------------------------------------------------------------------ #
    def build_expected_datasets(self, format_string):
        """Create dictionary of DIDs from the syst/sample configuration.

The attached systematics and their samples are used with the mandatory
FORMAT_STRING to create the DID name. A DID object is then created and
attached to the dids dictionary."""
        for syst in self.systematics.values():
            for sample in syst.samples.values():
                if not syst.is_nominal_only() and sample.nominal_only:
                    continue
                didname = format_string.format(
                    USER=self.name, SAMPLE=sample.name, SYSTEMATIC=syst.short_name)
                if didname in self.dids:
                    print('Warning: DID {} already exists for user {}'.format(
                        didname, self.name))
                    continue
                did = DidData(didname, self.campaign)
                did.verbose = self.verbose
                self.dids[didname] = did
                syst.dids[didname] = did
                sample.dids[didname] = did
        print('  Expected datasets for user {}: {}'.format(self.name, len(self.dids)))

    # --- class user ------------------------------------------------------------------ #
    def update_did_rucio_status(self):
        """Set the DIDs' status depending on whether rucio finds the DID."""
        print('  Check production progress with rucio for all dids of user {}'.format(self.name))
        for didname in sorted(self.dids.keys()):
            did = self.dids[didname]
            n_dids = len(list(rucio_client.list_dids(
                scope='user.' + self.name, filters={'name': did.did})))
            if n_dids == 0:
                did.rucio_status = 'missing'
                if self.verbose > 0:
                    print('      DID not done:', did.did)
            if n_dids == 1:
                did.rucio_status = 'done'
            if n_dids > 1:
                print('Warning: found {} results for DID {}'.format(
                    n_dids, did.did))
                did.rucio_status = 'duplicate'

    # --- class user ------------------------------------------------------------------ #
    def update_download_status(self, require_panda_done=True):
        """Check completeness of local files for each DID with rucio."""
        print('  Check download progress for user {}'.format(self.name))
        sys.stdout.flush()  # avoid buffering when piping
        for didname in sorted(self.dids.keys()):
            did = self.dids[didname]
            if did.download_dir is None:
                did.download_dir = self.download_dir
            did.update_download_status(require_panda_done)

    # --- class user ------------------------------------------------------------------ #
    def update_did_task_status(self):
        """Update the jedi task status of all DIDs for this user."""
        n_tasks_updated = 0
        n_tasks_unexpected = 0
        n_tasks_broken = 0
        for jediTaskID in sorted(self.task_dict.keys()):
            jediTask = self.task_dict[jediTaskID]
            out_did_names = [ds.strip('/') for ds in jediTask['outDS'].split(',')]
            found_did = False
            for out_did_name in out_did_names:
                if out_did_name in self.dids.keys():
                    if jediTask['status'] == 'broken':
                        n_tasks_broken += 1
                        if self.verbose > 2:
                            print('      Task {}, {} broken'.format(jediTaskID, jediTask['taskName']))
                        continue  # don't count a broken task as found
                    found_did = True
                    did = self.dids[out_did_name]
                    did.taskID = jediTaskID
                    did.task_status = jediTask['status']
                    did.task_dict = jediTask
                    n_tasks_updated += 1
            if not found_did:
                n_tasks_unexpected += 1
                if self.verbose > 2:
                    print('      Task {}, {} not expected'.format(jediTaskID, jediTask['taskName']))
        print('  Updated {} DIDs with a task status for user {}'.format(n_tasks_updated, self.name))
        if n_tasks_unexpected > 0:
            print('  Warning: found {} unexpected tasks with panda for user {}'.format(
                n_tasks_unexpected, self.name))
        if n_tasks_broken > 0:
            print('  Warning: found {} broken tasks with panda for user {}'.format(
                n_tasks_broken, self.name))

    # --- class user ----------------------------------------------------------------- #
    def update_statistics(self, recurse=True):
        """Check DIDs and optionally systematics for 'done' or 'incomplete' statuses."""
        n_total = len(self.dids)
        did_progress = collections.Counter()
        task_progress = collections.Counter()
        download_progress = collections.Counter()
        convert_progress = collections.Counter()
        for did in self.dids.values():
            did_progress[did.rucio_status] += 1
            task_progress[did.task_status] += 1
            download_progress[did.download_status] += 1
            convert_progress[did.convert_status] += 1
        self.task_status_counter = task_progress
        self.download_status_counter = download_progress
        self.convert_status_counter = convert_progress
        self.n_dids_distributed = n_total
        self.n_dids_done = did_progress['done']
        self.n_dids_missing = did_progress['missing']
        self.status = 'done' if n_total == did_progress['done'] else 'incomplete'
        if recurse:
            systematic_progress = collections.Counter()
            self.systematics_task_status_counter = collections.Counter()
            self.systematics_download_status_counter = collections.Counter()
            self.systematics_convert_status_counter = collections.Counter()
            for systematic in self.systematics.values():
                systematic.update_statistics(recurse)
                systematic_progress[systematic.status] += 1
                self.systematics_task_status_counter[systematic.task_status] += 1
                self.systematics_download_status_counter[systematic.download_status] += 1
                self.systematics_convert_status_counter[systematic.convert_status] += 1
            self.n_systematics_done = systematic_progress['done']
            self.n_systematics_incomplete = systematic_progress['incomplete']

    # --- class user ------------------------------------------------------------------ #
    def download_datasets(self, require_panda_done=True, only_print_to_list=None):
        """Download datasets for this user."""
        if only_print_to_list is None:
            print('  Downloading datasets for user {} to {}'.format(self.name, self.download_dir))
            if self.download_dir is None:
                print('  Cannot download for user {}, download_dir is not set.'.format(self.name))
                return
            for didname in sorted(self.dids.keys()):
                did = self.dids[didname]
                if did.download_dir is None:
                    did.download_dir = self.download_dir
                did.download(require_panda_done)
        else:
            print('  Printing list of datasets to be downloaded for user {} to {}'.format(
                self.name, only_print_to_list))
            with open(only_print_to_list, 'a') as f:
                for line in sorted(self.dids.keys()):
                    f.write(line + '\n')

    # --- class user ------------------------------------------------------------------ #
    def convert_datasets(self, require_downloaded=True):
        """Convert datasets from grid format to A++ format for this user."""
        if self.hadd_target_dir is None:
            print('  Cannot convert for user {}, hadd_target_dir is not set.'.format(self.name))
            return
        print('  Converting datasets for user {} to {}'.format(self.name, self.hadd_target_dir))
        for didname in sorted(self.dids.keys()):
            did = self.dids[didname]
            if did.hadd_target_dir is None:
                did.hadd_target_dir = self.hadd_target_dir
            did.hadd(require_downloaded)

    # --- class user ------------------------------------------------------------------ #
    def print_summary(self):
        """Print summary of DID production status (rucio only for now)."""
        print('  Summary for user {}:'.format(self.name))
        print('    datasets:')
        print('      {:4d}/{:4d} DIDs visible in rucio'.format(self.n_dids_done, self.n_dids_distributed))
        if self.n_dids_missing > 0:
            print('      {:4d}/{:4d} DIDs missing on grid'.format(self.n_dids_missing, self.n_dids_distributed))
        print('    systematics:')
        print('      {:4d}/{:4d} systematics completely visible in rucio'.format(
            self.n_systematics_done, len(self.systematics)))
        if self.n_systematics_incomplete > 0:
            print('      {:4d}/{:4d} systematics incomplete'.format(
                self.n_systematics_incomplete, len(self.systematics)))
        print('')

    # --- class user ------------------------------------------------------------------ #
    def print_summary_panda(self):
        # use counters! check old code for how
        """Print summary of DID production status using panda task statuses."""
        print('  Summary for user {}:'.format(self.name))
        print('    datasets:')
        n_dids_total = len(self.dids)
        for status in self.task_status_counter.keys():
            print('      {:4d}/{:4d} DIDs in task status "{}"'.format(
                self.task_status_counter[status], n_dids_total, status))
        print('    systematics:')
        n_systematics_total = len(self.systematics)
        for status in self.systematics_task_status_counter.keys():
            print('      {:4d}/{:4d} systematics {}'.format(
                self.systematics_task_status_counter[status], n_systematics_total, status))
        print('')

    # --- class user ------------------------------------------------------------------ #
    def print_summary_download(self):
        """Print summary of DID download status using rucio."""
        print('  Summary for user {}:'.format(self.name))
        print('    datasets:')
        n_dids_total = len(self.dids)
        for status in self.download_status_counter.keys():
            print('      {:4d}/{:4d} DIDs in download progress "{}"'.format(
                self.download_status_counter[status], n_dids_total, status))
        print('    systematics:')
        n_systematics_total = len(self.systematics)
        for status in self.systematics_download_status_counter.keys():
            print('      {:4d}/{:4d} systematics {}'.format(
                self.systematics_download_status_counter[status], n_systematics_total, status))
        print('')

    # --- class user ------------------------------------------------------------------ #
    def print_summary_convert(self):
        """Print summary of DID conversion status from grid jobs to A++ filenames."""
        print('  Summary for user {}:'.format(self.name))
        print('    datasets:')
        n_dids_total = len(self.dids)
        for status in self.convert_status_counter.keys():
            print('      {:4d}/{:4d} DIDs in convert status "{}"'.format(
                self.convert_status_counter[status], n_dids_total, status))
        print('    systematics:')
        n_systematics_total = len(self.systematics)
        for status in self.systematics_convert_status_counter.keys():
            print('      {:4d}/{:4d} systematics {}'.format(
                self.systematics_convert_status_counter[status], n_systematics_total, status))
        print('')
# --- end class user ------------------------------------------------------------------ #


# --- begin class systematic ---------------------------------------------------------- #
class Systematic(object):
    """Systematic object that owns the relevant samples."""
    def __init__(self, name, short_name, syst_type='regular'):
        self.name = name
        self.short_name = short_name
        self.type_ = syst_type
        self.samples = {}
        self.dids = {}
        self.n_dids_distributed = 0
        self.n_dids_done = 0
        self.n_dids_missing = 0
        self.status = None       # how complete is the sample on grid?
        self.task_status = None  # same, but with panda/jedi tasks
        self.download_status = None  # track download progress with rucio
        self.convert_status = None  # track hadd conversion progress

    # --- class systematic ------------------------------------------------------------ #
    def add_sample(self, sample):
        """Add a sample object to this systematic's sample dictionary."""
        self.samples[sample.name] = sample

    # --- class systematic ------------------------------------------------------------ #
    def get_sample(self, sample_name):
        """Get a sample object by name from this systematic's dictionary."""
        return self.samples[sample_name]

    # --- class systematic ------------------------------------------------------------ #
    def is_nominal_only(self):
        """Is this systematic considered nominal?"""
        return self.type_ in ['nominal', 'sample_syst']

    # --- class systematic ------------------------------------------------------------ #
    def update_statistics(self, recurse=True):
        """Check DIDs and optionally samples for 'done' or 'incomplete' statuses."""
        n_total = len(self.dids)
        did_progress = collections.Counter()
        task_status_counter = collections.Counter()
        download_status_counter = collections.Counter()
        convert_status_counter = collections.Counter()
        for did in self.dids.values():
            did_progress[did.rucio_status] += 1
            task_status_counter[did.task_status] += 1
            download_status_counter[did.download_status] += 1
            convert_status_counter[did.convert_status] += 1
        self.task_status = ','.join(sorted(map(str, task_status_counter.keys())))
        self.download_status = ','.join(sorted(map(str, download_status_counter.keys())))
        self.convert_status = ','.join(sorted(map(str, convert_status_counter.keys())))
        self.n_dids_distributed = n_total
        self.n_dids_done = did_progress['done']
        self.n_dids_missing = did_progress['missing']
        self.status = 'done' if n_total == did_progress['done'] else 'incomplete'
        if recurse:
            sample_progress = collections.Counter()
            for sample in self.samples.values():
                sample.update_statistics()
                sample_progress[sample.status] += 1
            self.n_samples_done = sample_progress['done']
            self.n_samples_incomplete = sample_progress['incomplete']
# --- end class systematic ------------------------------------------------------------ #


# --- begin class sample -------------------------------------------------------------- #
class Sample(object):
    """Sample object to track production and download progress."""
    rexSplitNumbers = re.compile(r'^([0-9]*[a-zA-Z]+)([0-9]+)?(l*)$')

    # --- class sample ---------------------------------------------------------------- #
    def __init__(self, name, nominal_only=False):
        print(name)
        self.name = name
        self.nominal_only = nominal_only
        self.dids = {}
        self.supersample = ""
        m = Sample.rexSplitNumbers.match(name)
        if m is None:
            raise ValueError('sample name "{}" does not match regex'.format(name))
        if self.nominal_only or m.group(2) is None:
            self.supersample = name
            self.is_part_of_split = False
        else:
            self.supersample = m.group(1)
            self.is_part_of_split = True
        self.status = None

    # --- class sample ---------------------------------------------------------------- #
    def update_statistics(self):
        """Check DIDs for 'done' or 'incomplete' statuses."""
        n_total = len(self.dids)
        did_progress = collections.Counter()
        for did in self.dids.values():
            did_progress[did.rucio_status] += 1
        self.n_dids_distributed = n_total
        self.n_dids_done = did_progress['done']
        self.n_dids_missing = did_progress['missing']
        self.status = 'done' if n_total == did_progress['done'] else 'incomplete'
# --- end class sample ---------------------------------------------------------------- #


# --- begin class diddata ------------------------------------------------------------- #
class DidData(object):
    """DidData object to track associated task and download."""
    def __init__(self, did, campaign=None):
        # general
        self.did = did
        self.name = did
        self.campaign = campaign
        self.verbose = 0 if campaign is None else campaign.verbose
        # rucio status
        self.rucio_status = None
        # connection to panda task
        self.taskID = None
        self.task_dict = {}
        self.task_status = None
        # download
        self._download_dir = None
        self._download_path = None
        self.download_status = None
        # convert
        self.hadd_target_dir = None if campaign is None else campaign.hadd_target_dir
        self.convert_status = None

    # --- class DidData --------------------------------------------------------------- #
    @property
    def download_dir(self):
        return self._download_dir

    # --- class DidData --------------------------------------------------------------- #
    @download_dir.setter
    def download_dir(self, d):
        self._download_dir = d
        self._download_path = os.path.join(self._download_dir, self.did)

    # --- class DidData --------------------------------------------------------------- #
    def update_download_status(self, require_panda_done=True):
        """Check completeness of local files with rucio and update status."""
        cwd = os.getcwd()
        if self._download_path is None:
            self.download_status = 'pathisnone'
            return
        if not os.path.exists(self._download_path):
            self.download_status = 'new'
            return
        if require_panda_done and self.task_status != 'done':
            self.download_status = 'upstreamnotdone'
            return
        os.chdir(self._download_path)
        m = rexScope.match(self.did)
        did_size = 0
        downloaded_size = 0
        n_files = 0
        for file_ in rucio_client.list_files(scope=m.group(1), name=self.did):
            did_size += int(file_['bytes'])
            if os.path.exists(file_['name']):
                downloaded_size += os.path.getsize(file_['name'])
            n_files += 1
        if n_files == 0:
            print('  Warning: rucio did not return any file for did {}'.format(self.did))
            print('    path for did is {}, remove manually'.format(self._download_path))
        if did_size != downloaded_size:
            self.download_status = 'incomplete'
            if self.verbose > 1:
                print('    Incomplete sample ({:4d}/{:4d} MB): {}'.format(
                    downloaded_size // 1024**2, did_size // 1024**2, self.did))
        else:
            self.download_status = 'done'
        os.chdir(cwd)

    # --- class DidData --------------------------------------------------------------- #
    def download(self, require_panda_done=True):
        """Download this did."""
        cwd = os.getcwd()
        if self.download_status is None:
            self.update_download_status(require_panda_done)
        if self.download_status == 'done':
            if self.verbose > 2:
                print('      DID {} already downloaded, skip'.format(self.did))
            return
        if require_panda_done and self.task_status != 'done':
            if self.verbose > 1:
                print('    task for DID {} not marked as "done", skip'.format(self.did))
            return
        m = rexScope.match(self.did)
        _mkdir(self._download_dir)
        os.chdir(self._download_dir)
        if self.verbose > 1:
            print('  Downloading DID {} in status {} into {}'.format(
                self.did, self.download_status, os.getcwd()))
        rcmd = 'rucio download --ndownloader 8 %s:%s' % (m.group(1), self.did)
        print(rcmd)
        sys.stdout.flush()
        subprocess.call(rcmd, shell=True)
        # os.system(rcmd)
        os.chdir(cwd)
        self.update_download_status(require_panda_done)

    # --- class DidData --------------------------------------------------------------- #
    def hadd(self, require_downloaded=True, overwrite=False):
        """Hadd for each SgTopDSID in this grid job did."""
        cwd = os.getcwd()

        print("START")
        print(self.did)
        print(self.name)
        self.convert_status = 'Failed'

        if self.verbose > 1:
            print('    Converting DID {} from grid to A++ file names.'.format(self.name))

        # check requirements
        if self.campaign is None:
            print('    Need campaign information to hadd did {}. Skip!', self.name)
            self.convert_status = 'NoCampaign'
            return

        # skip if source does not exist or is not completely downloaded
        source_dir_full = self._download_path
        source_dir = self.name
        if not os.path.isdir(source_dir_full):
            if self.verbose > 1:
                print('    download path for DID {} does not exist, skip'.format(self.did))
                print(source_dir_full)
                self.convert_status = 'NotDownloaded'
            return
        if require_downloaded:
            if self.download_status is None:
                self.update_download_status(require_panda_done=True)
            if self.download_status != 'done':
                if self.verbose > 1:
                    print('    download for DID {} not marked as "done", skip'.format(self.did))
                self.convert_status = 'NotDownloaded'
                return

        # define target format and target file somewhere else
        # for now take the default here but it should be defined in the campaign?
        prefix = 'ntup_schan'
        njets = '2j'
        channel = 'lnu'
        target_format = '{app_mem_}{prefix}_{njets}_{channel}_{syst}_{DIDsample}.root'

        # take info from input directory name with regex
        m = rexGridOutputDir.match(source_dir)
        if m is None:
            print('    Directory {} did not match regex. Skip!'.format(source_dir))
            self.convert_status = 'NoRegexMatch'
            return
        dscope, stream, sample, syst_short, ntup_ver, grid_ver, dext = m.groups()
        syst = self.campaign.syst_short2long[syst_short]
        gridID = ntup_ver + '.' + grid_ver
        app_mem = None
        if dext == '_app_output.root':
            app_mem = 'app'
        elif dext == '_mem_output.root':
            app_mem = 'mem'
        else:
            print('Could not determine if directory {} is from app or mem job. Skip!',
                  source_dir)
            self.convert_status = 'AppOrMem?'
            return

        # make sure target dir exists, is taken from campaign
        # similar to GridSubmission.py we append gridID
        if self.hadd_target_dir is None:
            print('<DidData.hadd> No target directory for hadd defined. Skip!')
            self.convert_status = 'NoHaddDir'
            return
        hadd_target_dir = os.path.join(self.hadd_target_dir, gridID)
        _mkdir(hadd_target_dir)
        #hadd_target_dir = os.path.join(hadd_target_dir, syst)
	#_mkdir(hadd_target_dir)
	#HERE
        # sample configuration: which grid samples contain which sgtop dsids?
        sample_config = self.campaign.sample_config
        sample_lists = {k: sample_config[k]['dids'] for k in sample_config.keys()}
        sample_nsubjobs = {k: sample_config[k].get('nsubjobs') for k in sample_config.keys()}
        sample_subjobid = {k: sample_config[k].get('subjobid') for k in sample_config.keys()}
        # sample_nfilesperjob = {k: sample_config[k].get('nfilesperjob') for k in sample_config.keys()}
        if sample not in sample_lists.keys():
            print('SgTop DSID {} not found in sample configuration file'
                  ' for DID {}. Skip!'.format(sample, self.name))
            self.convert_status = 'NotInSampleConfig'
            return
        
	
        # similarly, we need the DSID definition file from A++
        DSIDs_file = self.campaign.DSIDs_file
        if DSIDs_file is None:
            print('No DSIDs_file defined. Skip!')
            self.convert_status = 'NoDSIDsFile'
            return

        # first loop: get list of subsamples in this did
        # change from GridSubmission: can already fill a dictionary instead of globbing later
        # the use of the terms did and sample is a bit inconsistent here
        sgtop_dids = {}
        for rootfile in os.listdir(source_dir_full):
            rootfile_full = os.path.join(source_dir_full, rootfile)
            if not os.path.isfile(rootfile_full):
                print('  {} is not a regular file. Skip!'.format(rootfile))
                continue

           
            # check normal match
            n = rexGridOutputFile.match(rootfile)
            if n is not None:
                fscope, fdid, tags, taskid, number, fext = n.groups()
            else:
                # check data match
                n = rexGridOutputFileData.match(rootfile)
                if n is not None:
                    fscope, fdid, tags, taskid, number, fext = n.groups()
                    if 'p43' in tags:
                        fdid = "QCDANTIMUON" + fdid
                    else:
                        fdid = 'data' + fdid
                else:
                    print('  {} does not match any output file regex. Skip!'.format(rootfile))
                    continue
                        # make some sanity checks
            if fscope != dscope:
                print('Warning: non-matching scopes in file and sample:'
                      ' {} != {}. Fix this first! Skip!'.format(fscope, dscope))
                continue
            if fdid not in sample_lists[sample]:
                print('Warning: {} is not in list for sample {}'.format(fdid, sample))
            if fext != dext[1:]:  # dext has _ first
                print('Warning: non-matching file extensions in file and samples:'
                      ' {} != {}'.format(fext, dext[1:]))

            # add did to dictionary
            sgtop_dids[fdid] = sgtop_dids.get(fdid, [])
            this_sgtop_did = sgtop_dids[fdid]
            if rootfile in this_sgtop_did:
                print('Something went wrong; won\'t add rootfile {} twice '
                      'to SgTop DSID {}. Skip!'.format(rootfile, fdid))
            this_sgtop_did.append(rootfile)

        # second loop: run hadd for every sgtop_did found
        n_new = 0
        for sgtop_did in sorted(sgtop_dids.keys()):
            source_files = sorted(sgtop_dids[sgtop_did])
            # get DIDsample from our short DID, add split in case of subjobs
            DIDsample = find_DIDsample_in_DSIDs_list(sgtop_did, DSIDs_file)
            if sample in sample_nsubjobs and sample_nsubjobs[sample] > 1:
                nsubjobs = sample_nsubjobs[sample]
                subjobid = sample_subjobid[sample]
                DIDsample = '{:s}{:03d}'.format(DIDsample, subjobid)
                # sanity check
                if not sample.endswith('{}{}'.format(nsubjobs, subjobid)):
                    print('    Sample {} does not match with {}/{} subjobs. Skip!'.format(
                        sample, nsubjobs, subjobid))
                    continue
            format_dict = {'app_mem_': app_mem + '_', 'prefix': prefix, 'njets': njets,
                           'channel': channel, 'syst': syst, 'DIDsample': DIDsample}
            # build target
            hadd_target_dir_appmem = os.path.join(hadd_target_dir, app_mem)
            _mkdir(hadd_target_dir_appmem)
            hadd_target_dir_appmem = os.path.join(hadd_target_dir_appmem, syst)
            _mkdir(hadd_target_dir_appmem) 
            os.chdir(source_dir_full)
            target_file = target_format.format(**format_dict)
            target_file_full = os.path.join(hadd_target_dir_appmem, target_file)
            # check if target exists
            if os.path.exists(target_file_full):
                if not overwrite:
                    if self.verbose > 2:
                        print('      Target {} exists. Don\'t overwrite, skip!'.format(target_file_full))
                    continue
            # build hadd command
            hcmd = ['hadd', '-f9', target_file_full] + source_files
            #hcmd = ['hadd', '-f9', '/rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_GridRun_NominalOnly_MC16a/Test_run_check/v32lj_MC16a.v3/mem/mem_ntup_schan_2j_lnu_nominal_410644schanlt004.root', 'user.kkreul.410645.e6527_s3126_r9364_p4346.26647102._000044.mem_output.root']
            print("Running command " + str(hcmd) + ' in directory {}'.format(os.getcwd()))
            sys.stdout.flush()
            subprocess.call(hcmd)
           
            n_new += 1
            # os.system(rcmd)
        os.chdir(cwd)
        # set status for this did:
        # - Failed if this function returned in between
        # - NotDownloaded if download was not complete
        # - Done if every target already existed
        # - xNew if x target dsids were hadded
        if n_new == 0:
            self.convert_status = 'Done'
            if self.verbose > 1:
                print('    All conversions were already done.')
        else:
            self.convert_status = '{}Added'.format(n_new)
            if self.verbose > 1:
                print('    Hadd\'ed {} new files.'.format(n_new))
# --- end class diddata --------------------------------------------------------------- #


# --- begin class jeditask ------------------------------------------------------------ #
# do I need such an object or can I work with dictionaries?
class JediTask(object):
    def __init__(self, task_dict):
        self.task_dict = task_dict
        self.taskID = task_dict['jediTaskID']
        self.out_did_names = [ds.strip('/') for ds in task_dict['outDS'].split(',')]
        self.dids = {}
# --- end class jeditask -------------------------------------------------------------- #


# ------------------------------------------------------------------------------------- #
# ------------------------------------------------------------------------------------- #
# --- all that follows is old code ---------------------------------------------------- #
# ------------------------------------------------------------------------------------- #
# ------------------------------------------------------------------------------------- #
def get_panda_task_list(state, state_dict, systematic=None):
    """Get list of tasks in state 'state' from panda output in 'state_dict', with optional
filter for systematic 'systematic'."""
    task_list = []
    for syst, syst_dict in state_dict.items():
        if systematic is not None and syst != systematic:
            continue
        for sample, sample_dict in syst_dict.items():
            if sample_dict['status'] == state:
                task_name = sample_dict['name']
                task_list.append(task_name.strip('/'))
    return sorted(task_list)


def get_complete_did_list(state, state_dict, suffix):
    """Get list of dids from panda output."""
    did_list = []
    for syst, syst_dict in state_dict.items():
        for sample, sample_dict in syst_dict.items():
            if sample_dict['status'] == state:
                task_name = sample_dict['name']
                did_list.append(task_name.strip('/') + suffix)
    return sorted(did_list)


def create_user_task_list(user, version, syst_file, sample_file,
                          nominal_only=None):
    """Create list of task names for a user from syst and sample files."""
    if nominal_only is None:
        nominal_only = ['data15', 'data16', 'data17', 'QCDJetLepton',
                        'QCDANTIMUON15', 'QCDANTIMUON16', 'QCDANTIMUON17']
    systs = read_lines_from_file(syst_file)
    samples = read_lines_from_file(sample_file)
    task_list = []
    scope = 'user.' + user
    for syst in systs:
        for sample in samples:
            if syst != 'nominal' and sample in nominal_only:
                continue
            task = scope + '.schan_App_MemTk.' + sample + '_' + d_syst_toshort[syst] + '_' + version
            task_list.append(task)
    return task_list


def get_user_task_list(user, campaign, rversion, states=None):
    state, jediTask_list, jediTask_dict = pandatools.Client.getJobIDsJediTasksInTimeRange(
        earliest_date, dn=user_full_name[user])
    filtered_dict = {v['jediTaskID']: v for k, v in jediTask_dict.items()
                     # if re.findall('user\.'+user+'\..*_'+version.replace('.','\.')+ '/?$', v['taskName'])}
                     if re.findall(r'user\.{}\.{}\..*_{}/?$'.format(user, campaign, rversion), v['taskName'])}
    if states is not None:
        filtered_dict = {v['jediTaskID']: v for k, v in filtered_dict.items()
                         if v['status'] in states}
    task_list = [v['taskName'].rstrip('/') for k, v in filtered_dict.items()]
    return task_list


def read_dids_from_disk(path, suffix):
    """Get list of all dids on disk in path."""
    did_list = []
    dirs = os.listdir(path)
    for did in dirs:
        if not os.path.isdir(os.path.join(path, did)):
            continue
        if not (did.startswith("user.") and did.endswith(suffix)):
            continue
        did_list.append(did)
    return did_list


def check_did_download_size(did, target_dir):
    m = rexScope.match(did)
    try:
        scope = m.group(1)
    except AttributeError:
        print('Did "{}" has no scope?!'.format(did))
        raise
    did_size = 0  # from rucio
    downloaded_size = 0  # on disk
    did_target_dir = os.path.join(target_dir, did)
    for file_ in rucio_client.list_files(scope=scope, name=did):
        did_size += int(file_['bytes'])
        dl_file = os.path.join(did_target_dir, file_['name'])
        dl_file_size = 0
        try:
            dl_file_size = os.path.getsize(dl_file)
        except OSError as e:
            if e.errno != 2:
                raise
        downloaded_size += dl_file_size
    return did_size, downloaded_size


def download_dids(did_list, target_dir, check_size=True, dry_run=False):
    mm = rexMachine.match(target_dir)
    if mm is not None:
        target_cluster, target_machine = mm.groups()
        machine = os.getenv('HOST')[3]
        cluster = 'm' if os.getenv('HOST')[0:3] == 'ms0' else 's'
        if cluster != target_cluster or machine != target_machine:
            print('  Consider running on machine {}{} for faster access (you are on {}{})'.format(
                target_cluster, target_machine, cluster, machine))
    try:
        os.chdir(target_dir)
    except OSError as e:
        if e.errno != 2:
            raise
        os.makedirs(target_dir)
        os.chdir(target_dir)
    if debug > 0:
        print('  Downloading to target directory {}'.format(target_dir))
    n_total = len(did_list)
    n_done = 0
    n_downloaded = 0
    downloaded_dids = []
    for did in did_list:
        if check_size:
            todo, done = check_did_download_size(did, target_dir)
            if todo == done:
                if debug > 0:
                    print('    DID {} already downloaded'.format(did))
                n_done += 1
                downloaded_dids.append(did)
                continue
            elif debug > 0:
                print('    Incomplete sample ({:4d}/{:4d} MB): {}'.format(
                    done // 1024**2, todo // 1024**2, did))
        if not dry_run:
            m = rexScope.match(did)
            rucio_cmd = 'rucio download --ndownloader 5 --rse DESY-ZN_LOCALGROUPDISK %s:%s' % (m.group(1), did)
            print(rucio_cmd)
            sys.stdout.flush()
            subprocess.call(rucio_cmd, shell=True)
            n_downloaded += 1
            downloaded_dids.append(did)
            # break
    print('  Download completed. Already downloaded/new downloaded/total dids: {}/{}/{}'.format(
        n_done, n_downloaded, n_total))
    return downloaded_dids


def task_IDs_from_mail_path(path_):
    file_list = os.listdir(path_)
    task_list = []
    regex = re.compile(r'^JEDI.*:([0-9]*) \(.* Succeeded\) - .*\.eml')
    for f in file_list:
        m = regex.match(f)
        if m.groups is None:
            print('something went wrong, check the regex again')
        else:
            taskID = m.group(1)
            task_list.append(taskID)
    return task_list


# ------- main program ----------------------------------------------------------------- #

if __name__ == '__main__':
    print('No main function defined.\n'
          'See ~kaphle/v29_grid_submission/run01_submit_all/check_results/run_check.py\n'
          '  for an example on how to use this')
    exit(0)

    # use campaign class? this is not functional but close
    # import GridCampaign
    # camp = GridCampaign.Campaign('config_test.txt')
    camp = Campaign('config_test.txt')   # config file similar to GridSubmission.py
    camp.add_user_systematics_samples('skaphle', 'systematics_skaphle_v29.txt', 'samples_v29.txt')
    # check production progress
    camp.build_expected_datasets()       # expected datasets based on list of samples/systs for each user
    camp.update_did_production_status()  # run rucio to get produced datasets
    camp.check_expected_produced()       # compare the previous two
    camp.read_task_states('task_dump.pickle')  # read status of tasks from a previous dump
    camp.update_task_states()            # run panda to update the task states (takes long)
    camp.dump_task_states('task_dump.pickle')  # dump the status of tasks to file
    camp.update_did_task_status()        # update classes with panda information
    # download samples
    camp.download_dir = 'some/download/dir'  # need to define if not in config
    camp.download_datasets(require_panda_done=True)  # download 'Done' tasks
    # convert format to that expected by A++
    camp.hadd_target_dir = 'some/hadd/dir'
    camp.DSIDs_file = os.path.join(
        os.getenv('APP_HOME'),
        'libSingleTop/tasks/13TeV/SampleList/SgTopDSIDs_mc16a_v29_lj.txt')
    camp.convert_datasets(require_downloaded=True)   # convert formats
    camp.print_summary()  # list done, finished, expected, whatever
