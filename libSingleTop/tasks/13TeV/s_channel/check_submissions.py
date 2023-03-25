#!/usr/bin/env python
# script to check which of the jobs were already successfully carried out
# this file was copied from $APP_SCHANNEL
#
# FOR USER CONFIG OPTIONS, SEE BOTTOM OR LAUNCH WITH THE -h FLAG
#
""" Check which of the jobs were already successfully carried out.

Takes a set of users and their submission files containing the systematics.
Then checks how many samples were submitted/completed for each user via rucio.
"""

# ------- imports ---------------------------------------------------------------------- #
from __future__ import print_function

import argparse
import collections
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import SafeConfigParser as ConfigParser
import datetime
import os
import cPickle as pickle
import re
import subprocess
import sys

import pandatools.Client
import rucio.client


# ------- argparse --------------------------------------------------------------------- #
parser = argparse.ArgumentParser(description='Check status of MEM submission')
parser.add_argument('--do-rucio', '-r', action='store_true',
                    help='find DIDs with rucio')
parser.add_argument('--do-panda', '-p', action='store_true',
                    help='find tasks with panda')
parser.add_argument('--verbose', '-v', action='count',
                    help='increase verbosity to debug')
parser.add_argument('--syst-file', action='store', default='systematics.txt',
                    metavar='FILE', help='systematics file (default "systematics.txt")')
parser.add_argument('--mc-ver', '-m', action='store', choices=['mc16a', 'mc16d', 'mc16e'],
                    metavar='MC', default=None,
                    help='lowercase MC version to use (default mc16a)')
parser.add_argument('--download', '-d', action='store_true',
                    help='download dids in state "done" (only with -p)')
parser.add_argument('--dump-dicts', action='store_true',
                    help='save/load instead of getting from grid')
parser.add_argument('--dry-run', action='store_true', help='don\'t download, show stats only')
parser.add_argument('--replicate', action='store_true', help='check/create replication rules')
parser.add_argument('--retry', action='store_true', help='retry failed tasks for current user')
parser.add_argument('--config-file', '-c', default='config.txt', help='config file to set campaign etc')


# ------- global variables ------------------------------------------------------------- #
rucio_client = rucio.client.Client()
# earliest_date = datetime.datetime.utcnow() - datetime.timedelta(days=28)  # days < 30
earliest_date = datetime.datetime(2019, 1, 15)
debug = 0

# users
users = ['finelli', 'lohse', 'pseema', 'skaphle', 'smergelm']
user_full_names = ['Kevin Daniel Finelli', 'Thomas Lohse',
                   'Pienpen Seema-Mergelmeyer', 'Stephan Kaphle',
                   'Sebastian Mergelmeyer']
user_full_name = dict(zip(users, user_full_names))

# regex
rexScope = re.compile(r'^(user\.\w+)\.')
# rexDid = re.compile(r'^(user\.\w+)\.(mc|data).*\.(v[0-9a-z]+)(?:\.[0-9])?_out\.root$')
rexDid = re.compile(r'^(user\.\w+)\.(\w+)\.([0-9a-zA-Z]+)_(\w+)_(v\w+)_(a|d|e)\.(v\d+)' +
                    r'(_(?:app|mem)_output\.root|\.log)(?:\.[0-9]+)?$')
rexTask = re.compile(r'^(user\.\w+)\.(\w+)\.([0-9a-zA-Z]+)_(\w+)_(v\w+)_(a|d|e)\.(v\d+)$')
rexMachine = re.compile(r'^/rdsk/dat([ms])([1-8])/.*$')
rexCutEnd = re.compile(r'(^.*?)(?:\.[0-9]+)?$')


# ------- functions -------------------------------------------------------------------- #


def read_lines_from_file(file_):
    """Read file and return list of non-empty lines, ignoring lines starting with #."""
    lines = []
    with open(file_) as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            lines.append(line.strip())
    return lines


def read_long_short_syst_file(file_):
    lines = read_lines_from_file(file_)
    d_syst_toshort = {}
    d_syst_tolong = {}
    for line in lines:
        s_long, s_short = line.split()
        d_syst_toshort[s_long] = s_short
        d_syst_tolong[s_short] = s_long
    return d_syst_toshort, d_syst_tolong


def check_for_rucio_samples(users, version, n_MC_expected=-1, n_data_expected=-1):
    """Check for rucio samples."""
    print('Checking for rucio samples')
    incomplete = []
    # loop over users
    for user in users:
        # user data
        scope = 'user.' + user

        # open txt file to get systematics for that user
        systematics = []
        with open(user_syst_file[user]) as sf:
            for line in sf:
                syst = line.strip()
                if syst.startswith('#') or syst.isspace():
                    continue
                systematics.append(d_syst_toshort[syst])

        # loop over the systematics
        for syst in systematics:
            n_expected = n_MC_expected
            if syst == 'nominal':
                n_expected += n_data_expected

            # build did
            # user.skaphle.schan_App_MemTk.363360WplvWmqq_MU_SCALE_u_v25_a.v1
            did = scope + '.schan_App_MemTk.*_' + syst + '_' + version + '_mem_output.root'

            # build rucio command?
            n_dids = 0
            for item in rucio_client.list_dids(scope=scope, filters={'name': did}):
                n_dids += 1

            if n_dids != n_expected:
                incomplete.append((user, did, n_dids))
                print('  Number of samples found for did {0}: {1}'.format(did, n_dids))

    # try to find submissions under a different user's name
    print('Checking for rucio samples under different names')
    for old_user, old_did, old_n_dids in incomplete:
        old_scope = 'user.' + old_user
        for user in users:
            if old_user == user:
                continue
            new_scope = 'user.' + user
            new_did = old_did.replace(old_scope, new_scope)
            n_dids = 0
            for item in rucio_client.list_dids(scope=new_scope, filters={'name': new_did}):
                n_dids += 1
            if n_dids > 0:
                print('  Number of samples found for did {0}: {1}'.format(new_did, n_dids))
    return incomplete


def panda_get_jedi_tasks(users):
    """Get all jedi tasks for USERS as returned by panda.

    The results are filtered to match a regex including USER, CAMPAIGN and VERSION.
    Returns a dictionary of {jediTaskID: jediTaskDictionary} pairs."""
    task_dict = {}
    for user in users:
        # get a list/dict of all grid jobs
        state, jediTask_list, jediTask_dict = pandatools.Client.getJobIDsJediTasksInTimeRange(
            earliest_date, dn=user_full_name[user])
        # filter by campaign/version
        filtered_dict = {v['jediTaskID']: v for k, v in jediTask_dict.items()
                         # if re.findall('user\.'+user+'\..*_'+version.replace('.','\.')+ '/?$', v['taskName'])}
                         if re.findall(r'user\.{}\.{}\..*_{}/?$'.format(user, campaign, rversion), v['taskName'])}
        # check uniqueness:
        old_list = task_dict.keys()
        inter = set.intersection(set(old_list), set(jediTask_list))
        if len(inter) > 0:
            print('Warning: lists are not unique; will overwrite {} tasks!'.format(len(inter)))
            print('Intersection:', inter)
        task_dict.update(filtered_dict)
    return task_dict


def get_sample_task_state_user(syst_short, sample, task_dict, sample_state_dict):
    #   check in global dict if there is an entry marked done, finished, or else
    # find all task numbers that have syst/sample combination:
    # problem with this: jobids are not unique!! need to take Jedi task IDs
    sample_state = None
    sample_user = None
    sample_name = None
    tasks = [k for k, v in task_dict.items()
             if v['taskName'].find(syst_short) != -1
             and v['taskName'].find(sample) != -1]
    if debug > 0 and len(tasks) > 1:
        print('    Found multiple ({2}) tasks for {0} {1}:'.format(syst_short, sample, len(tasks)))
    # different behavior depending on how many tasks where found:
    if len(tasks) == 0:
        if debug > 1:
            print('  Could not find any task for {0} {1}'.format(syst_short, sample))
        return len(tasks), None, None, None

    # implicit else:
    # if more than one entry for this syst/sample:
    # take the last "done" task as final status, but print every task for debug
    for task in tasks:
        task_state = task_dict[task]['status']
        task_user = task_dict[task]['taskName'].split('.')[1]
        task_name = task_dict[task]['taskName']
        sample_state_dict[task] = task_state
        old_state = sample_state

        # if I find a "done" task, this should be marked in global state
        # 2nd priority is a "running" or "finished" task, everything else last
        if (task_state == 'done' or
                task_state in ['running', 'finished'] and not old_state == 'done' or
                old_state not in ['done', 'running', 'finished']):
            sample_state = task_state
            sample_user = task_user
            sample_name = task_name

        # if task_state == 'done':
        #     sample_state = 'done'
        #     sample_user = task_user
        # elif task_state in ['running', 'finished'] and not old_state == 'done':
        #     sample_state = task_state
        #     sample_user = task_user
        # elif old_state not in ['done', 'running', 'finished']:
        #     sample_state = task_state
        #     sample_user = task_user

        # debug: if there are more then one, print every task with status
        if debug > 1 and len(tasks) > 1:
            print('      task "{0}" has status "{1}"'.format(task, task_state))

    sample_state_dict['status'] = sample_state
    sample_state_dict['user'] = sample_user
    sample_state_dict['name'] = sample_name
    return len(tasks), sample_state, sample_user, sample_name


def check_for_panda_tasks(state_dict, task_dict):
    """Get tasks with panda and fill them in state and task dictionaries."""
    # get from online //TODO: save as dictionary?
    print('Checking for panda tasks')
    task_dict = panda_get_jedi_tasks(users)

    # offline: check what is available
    n_complete = 0
    completed_systs = []
    # syst loop
    for syst in systs:
        # init syst-specific variables
        syst_state_dict = {}
        state_dict[syst] = syst_state_dict  # copy by name; dict will be changed later
        syst_state_dict['state_counter'] = collections.Counter()
        syst_state_dict['user_counter'] = collections.Counter()
        syst_short = d_syst_toshort[syst]

        # sample loop
        for sample in samples:
            # init sample-specific variables
            sample_state = None
            sample_user = None
            sample_state_dict = {'status': sample_state, 'user': sample_user, 'name': None}
            syst_state_dict[sample] = sample_state_dict  # copy by name

            n_tasks, sample_state, sample_user, sample_name = \
                get_sample_task_state_user(syst_short, sample, task_dict, sample_state_dict)
            if n_tasks == 0:  # implied that no tasks were found
                continue

            # count done, finished, running, broken, failed etc samples
            syst_state_dict['state_counter'][sample_state] += 1
            syst_state_dict['user_counter'][sample_user] += 1

        # print statistics if there were not the expected amount of 'done' tasks
        n_expected = n_MC_expected
        if syst == 'nominal':
            n_expected += n_data_expected
        if not syst_state_dict['state_counter']['done'] == n_expected:
            prnt_str = []
            # print(syst_state_dict['state_counter'])
            for k, v in syst_state_dict['state_counter'].items():
                prnt_str.append('{0} "{1}"'.format(v, k))
            prnt_users = ['{} by {}'.format(v, k) for k, v in syst_state_dict['user_counter'].items()]

            if not prnt_str:
                print('  Warning: ' + syst_short + ' has no tasks at all')
            else:
                print('  ' + syst_short + ' has ' + ', '.join(prnt_str) + ' samples'
                      '; submitted ' + ', '.join(prnt_users))
        else:
            n_complete += 1
            completed_systs.append(syst)

    print('There are {0}/{1} completed systematics'.format(n_complete, len(systs)))
    with open('completed_systs_' + mc_ver + '.txt', 'w') as csf:
        csf.write('\n'.join(completed_systs))
    return completed_systs


class Campaign(object):
    """Top level object to store all tasks, samples, systematics."""
    nominal_only_default = ['data15', 'data16', 'data17', 'QCDJetLepton',
                            'QCDANTIMUON15', 'QCDANTIMUON16', 'QCDANTIMUON17']
    filters_default = [lambda x: x.find('_JER_') == -1 or x.find('_d_') == -1]

    def __init__(self, name='unnamed_campaign', version='v0', main_dir=None):
        print('Creating campaign "{}" with version string "{}"'.format(campaign, version))

        # members taken from init
        self.name = name
        self.version = version
        if main_dir is None:
            self.main_dir = os.getcwd()

        # other members' defaults
        self.downloaded_dids = []              # list of dids fully downloaded
        self.expected_datasets_scopeless = []  # datasets to be produced (no scope)
        self.produced_datasets = []            # datasets that were produced
        self.samples = []
        self.systematics = []
        self.systs = []  # short version of systematics
        self.users = users  # warning: global variable
        self.debug = debug
        self.dry_run = True
        self.dl_target_dir = ''
        self.save_downloaded = True
        self.did_type = 'mem'  # this does a lot more then just 1 variable
        # self.dl_suffix = '_output.root'
        # self.dl_dids_file = 'downloaded_dids_' + version + '.txt'

        # helper members without intended public use
        self.__d_expected_dids = {}
        self._d_syst_toshort = {}
        self._d_syst_tolong = {}

        self.systs = []  # read_lines_from_file(syst_file)  # note syst_file must be set
        self.completed_systs = []
        self.downloaded_systs = []
        self.tasks = {}
        self.state_dict = {}
        self.task_dict = {}
        if self.debug > 0:
            print('Created campaign {} in version {}'.format(name, version))
        # not necessarily needed here
        # self.panda_update_user_tasks(users)
        # self.update_task_states(self)

    @property
    def did_type(self):
        return self.__did_type

    @did_type.setter
    def did_type(self, t):
        if t in ['mem', 'app']:
            if debug > 0:
                print('  Setting did_type "{}"'.format(t))
            self.__did_type = t
            self.dl_dids_file = 'downloaded_dids_' + t + '_' + self.version + '.txt'
            self.did_suffix = '_' + t + '_output.root'
            if debug > 0:
                print('  Using did_sufix "{}"'.format(self.did_suffix))
            if self.save_downloaded and os.path.exists(self.dl_dids_file):
                self.downloaded_dids = read_lines_from_file(self.dl_dids_file)
                if debug > 0:
                    print('  Read {} entries from file {}'.format(
                        len(self.downloaded_dids), self.dl_dids_file))
            else:
                self.downloaded_dids = []
        else:
            print('Not changing did_type, value {} is invalid'.format(t))

    def read_systematics_long_short(self, file_):
        """Read systematics from a file with long and short systematic names."""
        lines = read_lines_from_file(file_)
        self.systematics = []
        self.systs = []
        self._d_syst_toshort = {}
        self._d_syst_tolong = {}
        for line in lines:
            s_long, s_short = line.split()
            self._d_syst_toshort[s_long] = s_short
            self._d_syst_tolong[s_short] = s_long
            self.systematics.append(s_long)
            self.systs.append(s_short)

    def build_expected_datasets(self, nominal_only_samples=nominal_only_default,
                                filters=filters_default):
        """Create list of expected dids from systs and samples."""
        self.expected_datasets_scopeless = []
        for syst in self.systs:
            for sample in self.samples:
                if syst != 'nominal' and sample in nominal_only_samples:
                    continue
                did = self.name + '.' + sample + '_' + syst + '_' + self.version
                filtered = False
                for f in filters:
                    if not f(did):
                        filtered = True
                        break
                if filtered:
                    continue
                self.expected_datasets_scopeless.append(did)
        if debug > 0:
            print('  build_expected_datasets: expecting {} datasets'.format(
                len(self.expected_datasets_scopeless)))
        # self._d_expected_dids = {k: [] for k in self._scopeless_expected_dids}

    def download_dids(self, did_list=None, check_downloaded=True,
                      dl_target_dir=None, dry_run=None):
        """Download DIDs from a list or created from all produced datasets."""
        if did_list is None:
            datasets = self.produced_datasets
            did_list = [x.rstrip('/') + self.did_suffix for x in datasets]
        if dry_run is None:
            dry_run = self.dry_run
        if dl_target_dir is None:
            dl_target_dir = self.dl_target_dir
        if check_downloaded:
            did_list = [d for d in did_list if d not in self.downloaded_dids]
        dl_dids = download_dids(did_list, dl_target_dir, dry_run=dry_run)
        self.downloaded_dids.extend(dl_dids)
        if save_downloaded:
            with open(self.dl_dids_file, 'w') as f:
                f.write('\n'.join(self.downloaded_dids))

    def check_expected_produced(self, expected_datasets_scopeless=None,
                                produced_datasets=None, write_missing=None):
        if expected_datasets_scopeless is None:
            expected_datasets_scopeless = self.expected_datasets_scopeless
        if produced_datasets is None:
            produced_datasets = self.produced_datasets
        # loop: count datasets and save them
        counter = collections.Counter()
        found = {}
        downloaded_datasets = list(map(lambda x: x.replace(self.did_suffix, ""), self.downloaded_dids))
        # print('Example dl ds:', downloaded_datasets[0])
        n_expected = len(expected_datasets_scopeless)
        n_found = 0
        n_notfound = 0
        n_toomany = 0
        n_notexpected = 0
        n_dlbutnotinprodlist = 0
        prod_remaining = list(set(produced_datasets).union(set(downloaded_datasets)))
        # print('Remaining:', len(prod_remaining))
        ds_missing = list(expected_datasets_scopeless)
        for dss in expected_datasets_scopeless:
            # count datasets
            found[dss] = found.get(dss, [])
            for user in self.users:
                scope = 'user.' + user
                ds = scope + '.' + dss
                # print(ds)
                # print(dss)
                if ds in produced_datasets or ds in downloaded_datasets:
                    counter[dss] += 1
                    found[dss].append(ds)
                    prod_remaining.remove(ds)
                if ds not in produced_datasets and ds in downloaded_datasets:
                    n_dlbutnotinprodlist += 1
            # print if not exactly one
            if counter[dss] != 1 and self.debug > 1:
                print('Found {} datasets for {}: {}'.format(counter[dss], dss, found[dss]))
            if counter[dss] == 0:
                n_notfound += 1
            elif counter[dss] > 1:
                n_toomany += 1
                ds_missing.remove(dss)
            elif counter[dss] == 1:
                n_found += 1
                ds_missing.remove(dss)
            else:
                print('unexpected count value:', counter[dss])
            # print(counter)
            # print(found)
            # break
        n_notexpected = len(prod_remaining)
        if debug > 0:
            print('  In check_expected_produced:')
            print('    Expected:       ', n_expected)
            print('    Found:          ', n_found)
            print('    Not found:      ', n_notfound)
            print('    >1 scope found: ', n_toomany)
            print('    Unexpected:     ', n_notexpected)
            print('    Only in dl list:', n_dlbutnotinprodlist)
            # print('Remaining:', len(prod_remaining), prod_remaining[:10])
            # print('Produced:', len(produced_datasets), produced_datasets[:10])
            # print('Downloaded:', len(downloaded_datasets),
            # downloaded_datasets[:10])
        self.__ep_count = counter
        self.__ep_found = found
        self.__ep_missing = ds_missing
        if write_missing is not None:
            with open(write_missing, 'w') as f:
                f.write('\n'.join(ds_missing))
            if debug > 0:
                print('  Saved {} missing datasets to {}'.format(
                    len(ds_missing), write_missing))

    def check_download_status(self):
        """WIP"""
        self.__downloaded_systs = []

    def print_summary(self):
        """WIP"""
        print('Summary for campaign {} in version {}'.format(self.name, self.version))
        print('  {} systematics, {} samples'.format(len(self.systs), len(self.samples)))
        print('  {} datasets expected'.format(len(self.expected_datasets_scopeless)))
        print('  {} datasets produced'.format(len(self.produced_datasets)))

    def panda_update_user_tasks(self, users):
        """WIP"""
        self.tasks.update(panda_get_jedi_tasks(users))

    def update_task_states(self, do_panda=False, dump_dicts=True):
        """ WIP"""
        if do_panda:
            self.completed_systs = check_for_panda_tasks(self.state_dict, self.task_dict)
            if dump_dicts:
                pass


def main():
    version_a = 'v25_a.v1'
    version_d = 'v25_d.v1'
    campaign_name = 'schan_App_MemTk'
    syst_short_list_file = '/mnt/eeh/kaphle/atlas/APlusPlus_git/libSingleTop/tasks/13TeV/s_channel/systematics_v25.txt'
    systematics_file = 'sytematics.txt'
    sample_file_mc16a = 'samples_mc16a.txt'
    sample_file_mc16d = 'samples_mc16d.txt'
    camp_a = Campaign(campaign_name, version_a)
    camp_a.read_systematics_long_short(syst_short_list_file)
    camp_a.samples = read_lines_from_file(sample_file_mc16a)
    camp_a.users = users  # warning: global variable
    nominal_only = ['data15', 'data16', 'data17', 'QCDJetLepton',
                    'QCDANTIMUON15', 'QCDANTIMUON16', 'QCDANTIMUON17']
    camp_a.build_expected_datasets(nominal_only)

    # how to do this?
    # - build list of expected dids by combining sample_syst as search string (scopeless)
    # - for all combinations, check which scope has that did (rucio)
    # - run check: how many samples have !=1 entries? however ignore for now

    camp_d = Campaign(campaign_name, version_d)
    camp_d.systematics = read_lines_from_file(systematics_file)
    camp_d.samples = read_lines_from_file(sample_file_mc16d)


class Task(object):
    pass


class Systematic(object):
    """Systematic object that owns the relevant samples."""
    def __init__(self, name, syst_type='regular'):
        self.name = name
        self.type_ = syst_type
        self.samples = {}
        self.status = 'empty'  # how complete is the sample?

    def add_sample(self, sample):
        self.samples[sample.name] = sample

    def get_sample(self, sample_name):
        return self.samples[sample_name]


class Sample(object):
    """Sample object to track production and download progress."""
    def __init__(self, name):
        pass


class DidData(object):
    def __init__(self, did):
        self.did = did
        m = rexDid.match(did)
        if m is None:
            raise ValueError('unrecognised DID "%s"' % did)
        try:
            self.scope, self.campaign, self.sample, self.syst, self.ntup_version, self.mc,\
                self.grid_version, self.suffix = m.groups()
        except ValueError:
            print('DID is {}, pattern match is {}'.format(did, m.groups()))
            raise


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


def get_existing_rules():
    rules_dict = {}
    n_rules = 0
    for rule in rucio_client.list_account_rules('skaphle'):
        if rule['scope'] != 'user.finelli':
            continue
        did = rexCutEnd.match(rule['name']).group(1)  # remove trailing numbers
        # did_obj = DidData(did)
        # syst = did_obj.syst
        # sample = did_obj.sample
        rules_dict[did] = rules_dict.get(did, {})
        rules_dict[did][rule['id']] = rule['expires_at']
        n_rules += 1
    return rules_dict


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


# def create_rse_rules(task_list, target_disk,
#                      suffixes=('.log', '_app_output.root', '_mem_output.root')):
#     for task in task_list:
#         for suffix in suffixes:
#             did = task + suffix
#             # rucio_client.


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
            rucio_cmd = 'rucio download --rse DESY-ZN_LOCALGROUPDISK %s:%s' % (m.group(1), did)
            print(rucio_cmd)
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


# functions needed:
# - 1. function that gets all "done" jobs from a user
# - 2. function that get all expected dsids
# - 3. find a user that submitted the did_size
# - 4. check if it is already downloaded (size comparison like split_downloads)
#


# def create_task_list_for_state_user(state, user, task_dict):
#     """Create lists for each user with tasks that require retries."""
#     # check in state_dict which samples are in finished state
#     # get task numbers for that
#     for syst, syst_dict

#     for user in users:
#         retry_file = '{}_retry.txt'.format(user)
#         with open(retry_file, 'w') as f:
#             for task, jedi in task_dict.items():

#             for syst, syst_dict in state_dict.items():
#                 for sample, sample_dict in syst_dict.items():
#                     if sample_dict['user'] != user:
#                         continue
#                     if sample_dict['status'] in ['finished']:  # may add other state later


# ------- main program ----------------------------------------------------------------- #

if __name__ == '__main__':

    # parse cmd line arguments
    args = parser.parse_args()
    # print(args)
    debug = args.verbose
    do_rucio = args.do_rucio
    do_panda = args.do_panda
    syst_file = args.syst_file
    mc_ver = args.mc_ver
    download = args.download
    dump_dicts = args.dump_dicts
    dry_run = args.dry_run
    replicate = args.replicate
    config_file = args.config_file
    retry = args.retry

    # init info
    this_file = os.path.abspath(sys.argv[0])
    print('Running', this_file)
    print('  Current time:', datetime.datetime.now())

    # parse configuration
    config_defaults = {'campaign': 'schan_App_MemTk', 'ntup_ver': 'v25',
                       'mc_ver': 'mc16a', 'grid_ver': 'v1', 'version': '',
                       'submit_dir': os.getcwd(), 'dl_type': 'mem',
                       'base_dl_dir': '', 'app_version': '', 'dl_target_dir': ''}
    cparser = ConfigParser(defaults=config_defaults)
    cparser.read(config_file)

    # main
    campaign = cparser.get('main', 'campaign')
    ntup_ver = cparser.get('main', 'ntup_ver')
    if mc_ver is None:
        mc_ver = cparser.get('main', 'mc_ver')
    grid_ver = cparser.get('main', 'grid_ver')
    version = cparser.get('main', 'version')
    if not version:
        version = ntup_ver + '_' + mc_ver[4] + '.' + grid_ver
    submit_dir = cparser.get('main', 'submit_dir')

    # download
    base_dl_dir = cparser.get('download', 'base_dl_dir')
    if not base_dl_dir:
        base_dl_dir = "/rdsk/dats5/atlas/kaphle/13TeV_s_channel/"
    app_version = cparser.get('download', 'app_version')
    if not app_version:
        app_version = "v15_v25lj_default_MC16" + mc_ver[4]
    dl_target_dir = cparser.get('download', 'dl_target_dir')
    if not dl_target_dir:
        dl_target_dir = os.path.join(base_dl_dir, app_version, 'rucio', 'download')

    # set stuff manually for temp solutions
    # version = 'v25_' + mc_ver[4] + '.v1'
    # version = 'v25_' + mc_ver[4] + '.v2'
    # version = 'v25_a.v1'
    rversion = version.replace('.', r'\.')

    # create campaign object and start doing stuff
    camp = Campaign(campaign, version)

    # # # users and directories
    # ntup_ver = version.split('.')[0].split('_')[0]
    # mc_ver = version.split('.')[0].split('_')[1]
    # camp_ver = version.split('.')[1]
    # version_dir = ntup_ver+'lj_default_'+mc_ver+'/'+camp_ver
    # submit_dir = os.path.join(os.getenv('HOME'), 'submit', version_dir)
    # submit_dir = os.getcwd()
    sample_file = os.path.join(submit_dir, 'samples_' + mc_ver + '.txt')
    user_syst_file = dict(zip(users, map(lambda x: os.path.join(submit_dir, x + '.txt'), users)))

    # read systematics and samples files
    systs = read_lines_from_file(syst_file)
    print('\nFound {0} systematics in {1}'.format(len(systs), syst_file))
    samples = read_lines_from_file(sample_file)
    print('\nFound {0} samples per systematic in {1}'.format(len(samples), sample_file))
    # "data" samples (only nominal): data15/16/17, QCDAntimuon15/16/17, QCDJetLepton
    n_data_expected = 5 if mc_ver == 'mc16a' else 3
    n_MC_expected = len(samples) - n_data_expected

    # convert short and long systematic names
    syst_short_list_file = '/mnt/eeh/kaphle/atlas/APlusPlus_git/libSingleTop/tasks/13TeV/s_channel/systematics_v25.txt'
    d_syst_toshort, d_syst_tolong = read_long_short_syst_file(syst_short_list_file)

    camp.read_systematics_long_short(syst_short_list_file)
    camp.samples = samples

    # main actions
    if do_rucio:
        check_for_rucio_samples(users, version, n_MC_expected, n_data_expected)

    # global_jedi_dict = { taskid1: {taskName: <name>, status: <status>,...}, taskid2:...}
    # simple concatenation of user jedi dicts
    global_jedi_dict = {}
    # global_state_dict = {syst1: {sample1: {'status': <status>, taskid1: <status>, ...}}}
    global_state_dict = {}
    dumpfile = 'jedi_state_' + mc_ver + '.dump'
    if do_panda:
        check_for_panda_tasks(global_state_dict, global_jedi_dict)
        if dump_dicts:
            print('Dumping dictionaries to {}'.format(dumpfile))
            with open(dumpfile, 'wb') as df:
                pickle.dump([global_jedi_dict, global_state_dict], df)
    elif dump_dicts:
        print('Reading dictionaries from {}'.format(dumpfile))
        with open(dumpfile, 'rb') as df:
            global_jedi_dict, global_state_dict = pickle.load(df)
    # camp.update_task_states(do_panda, dump_dicts)

    # download samples
    combined = False  # name to be changed
    save_downloaded = True
    state = 'done'  # 'done'
    dl_type = 'mem'
    camp.did_type = dl_type

    # TODO: app version easier
    #    base_dl_dir = "/rdsk/dats5/atlas/kaphle/13TeV_s_channel/"
    #    app_version = "v15_v25lj_default_MC16" + mc_ver[4]
    #    app_version = "v15_v25lj_default_samplesyst_MC16" + mc_ver[4]
    #    dl_target_dir = os.path.join(base_dl_dir, app_version, 'rucio', 'download')
    tasks_to_dl_file = "tasknames.txt"

    camp.dry_run = False
    camp.dl_target_dir = dl_target_dir
    camp.produced_datasets = [x.rstrip('/') for x in read_lines_from_file(tasks_to_dl_file)]
    if download:
        camp.download_dids()
    nominal_only = ['data15', 'data16', 'data17', 'QCDJetLepton',
                    'QCDANTIMUON15', 'QCDANTIMUON16', 'QCDANTIMUON17']
    camp.build_expected_datasets(nominal_only)
    camp.check_expected_produced(write_missing='missing_datasets_'+mc_ver+'.txt')
    camp.print_summary()

    run_old = False
    if run_old:
        dl_suffix = ''
        filter_nominal = False
        # app_filters = ['nominal', ]  # dids have to match to one of the elements to pass

        downloaded_dids_mem_file = os.path.join(submit_dir, 'downloaded_dids_' + mc_ver + '.txt')
        downloaded_dids_app_file = os.path.join(submit_dir, 'downloaded_dids_app_' + mc_ver + '.txt')
        downloaded_systs_mem_file = os.path.join(submit_dir, 'downloaded_systs_' + mc_ver + '.txt')
        downloaded_systs_app_file = os.path.join(submit_dir, 'downloaded_systs_app_' + mc_ver + '.txt')
        if dl_type == 'mem':
            downloaded_dids_file = downloaded_dids_mem_file
            downloaded_systs_file = downloaded_systs_mem_file
            dl_suffix = '_mem_output.root'
        elif dl_type == 'app':
            downloaded_dids_file = downloaded_dids_app_file
            downloaded_systs_file = downloaded_systs_app_file
            dl_suffix = '_app_output.root'
        else:
            raise ValueError('need to chose "mem" or "app" for download')

        if save_downloaded and os.path.exists(downloaded_dids_file):
            downloaded_dids = read_lines_from_file(downloaded_dids_file)
        else:
            downloaded_dids = []
        if download and not combined:
            if not global_state_dict and not tasks_to_dl_file:
                print('Can\'t download, activate --do-panda to get list')
            else:
                print('Downloading samples')
                if tasks_to_dl_file:
                    did_list = read_lines_from_file(tasks_to_dl_file)
                    did_list = [x.replace('/', '_mem_output.root') for x in did_list]
                else:
                    did_list = get_complete_did_list(state, global_state_dict, dl_suffix)
                new_did_list = [d for d in did_list if d not in downloaded_dids]
                # if dl_type == 'app':
                #     # filter did list:
                #     nd = []
                #     for d in new_did_list:
                #         filtered = True
                #         for f in app_filters:
                #             if d.find(f) != -1:
                #                 filtered = False
                #                 break
                #         if not filtered:
                #             nd.append(d)
                #             print('dbg    Appended {} to new did list'.format(d))
                #     new_did_list = nd
                if dl_type == 'app' and filter_nominal:
                    old_did_list = new_did_list
                    new_did_list = [d for d in new_did_list if (d.find('nominal') != -1)]
                    print(new_did_list)
                dl_dids = download_dids(new_did_list, dl_target_dir, dry_run=dry_run)
                downloaded_dids.extend(dl_dids)

        # some combined action:
        # - get the systematics that have 100% done samples -> fill in a list/file
        # - get a list of all dids belonging to these systematics
        # - download the dids on the list
        # (- outside this script: hadd, copy, run MEM disc)
        if download and combined:
            print('Downloading completed systematics')
            completed_systs = read_lines_from_file('completed_systs_'+mc_ver+'.txt')
            if not completed_systs:
                global_jedi_dict = {}
                # global_state_dict = {syst1: {sample1: {'status': <status>, taskid1: <status>, ...}}}
                global_state_dict = {}
                completed_systs = check_for_panda_tasks(global_state_dict, global_jedi_dict)
            if debug > 0:
                print('  Found {} completed systematics'.format(len(completed_systs)))
            completed_dids = []
            for syst in completed_systs:
                task_list = get_panda_task_list(state, global_state_dict, systematic=syst)
                did_list = map(lambda x: x + '_mem_output.root', task_list)
                completed_dids.extend(did_list)
            if debug > 0:
                print('  Found {} dids for completed systematics'.format(len(completed_dids)))
            new_did_list = [d for d in completed_dids if d not in downloaded_dids]
            dl_dids = download_dids(new_did_list, dl_target_dir, dry_run=dry_run)
            downloaded_dids.extend(dl_dids)
            # print('dl_dids =')
            # print(dl_dids)

        # create list of completed downloads
        # need to find out quick which samples are downloaded completely
        if save_downloaded and bool(downloaded_dids) and state == 'done':
            with open(downloaded_dids_file, 'w') as f:
                f.write('\n'.join(downloaded_dids))

        # create list of completely downloaded systematics
        # for every syst, loop over samples, check if there is a syst+samples comb
        # in the list of downloaded samples
        downloaded_systs = []
        nominal_only_samples = ['data15', 'data16', 'data17', 'QCDJetLepton',
                                'QCDANTIMUON15', 'QCDANTIMUON16', 'QCDANTIMUON17']
        for syst in systs:
            is_downloaded = True
            for sample in samples:
                if syst != 'nominal' and sample in nominal_only_samples:
                    continue
                found_did = False
                for did in downloaded_dids:
                    if did.find(sample) != -1 and did.find(d_syst_toshort[syst]) != -1:
                        found_did = True
                        break
                if not found_did:
                    is_downloaded = False
                    break
            if is_downloaded:
                downloaded_systs.append(syst)

        print('Found {} completely downloaded systematics'.format(
            len(downloaded_systs)))
        if save_downloaded and bool(downloaded_systs):
            print('  Writing downloaded systematics to {}'.format(downloaded_systs_file))
            with open(downloaded_systs_file, 'w') as f:
                f.write('\n'.join(downloaded_systs))

    # use the above in rucio_hadd

    # create replication rules:
    # - create list of all dids for a user from sample X systematic
    # - get existing rules
    # - create rules where non-existant
    # still easier to just use r2d2? seems to show an error

    # # replicate finelli to desy ZN
    # target_disk = 'DESY-ZN_LOCALGROUPDISK'
    # done_finelli_dids = get_panda_task_list('done', global_state_dict)
    # # check which are done but not in all or vice versa

    # get existing rules:
    check_finelli = replicate
    replicate_finelli = not dry_run
    check_expected = False  # check built list from samples X systs, otherwise use all tasks on grid
    if check_finelli:
        print('Checking replication rules')
        existing_rules = get_existing_rules()
        if check_expected:
            all_finelli_tasks = create_user_task_list(
                'finelli', version, user_syst_file['finelli'], sample_file)
        else:
            all_finelli_tasks = get_user_task_list('finelli', campaign, rversion)
        n_ex_rules = len(existing_rules)
        n_all_tasks = len(all_finelli_tasks)
        dids_without_rule = []
        for task in all_finelli_tasks:
            for suffix in ['_mem_output.root', '_app_output.root']:
                did = task + suffix
                if did not in existing_rules.keys():
                    if debug > 1:
                        print('There is no rule for DID {} yet!'.format(did))
                    dids_without_rule.append(did)
                else:
                    if debug > 1:
                        print('There are {} rules for DID {}'.format(
                            len(existing_rules[did].keys()), did))

        print('Found {} rules, {} tasks and {} dids without a rule'.format(
            len(existing_rules), len(all_finelli_tasks), len(dids_without_rule)))
        with open('dids_without_rule_' + mc_ver + '.txt', 'w') as f:
            f.write('\n'.join(dids_without_rule))

        if replicate_finelli:
            # create new rules
            rse = 'DESY-ZN_LOCALGROUPDISK'
            ncopies = 1
            for did in dids_without_rule:
                rucio_cmd = 'rucio add-rule {} {} {}'.format(did, ncopies, rse)

                # print(rucio_cmd)
                subprocess.call(rucio_cmd, shell=True)
                # os.system(rucio_cmd)
                # stop the blank line
                # break

    # note:
    # 1. might need to add rules without removing existing rules first, bc there could be rules to other scratch disks
    #    and there is an option to skip existing rules
    # 2. check if other users need rules - is our content forever on localgroupdisk?

    # retry = False
    retry_states = ['finished', 'failed']
    retry_list_file = 'retry_task_list.txt'
    # mail_path = os.path.join(os.getenv('HOME'), 'atlas/download/v25lj_MEM/jobs_notification_mails_2019-03-20')
    # mail_path = os.path.join(os.getenv('HOME'), 'atlas/download/v25lj_MEM/jobs_notification_mails_2019-01')
    mail_path = os.path.join(os.getenv('HOME'), 'atlas/download/v25lj_MEM/jobs_notification_mails_2019-03-28_finished')
    mail_file = 'jobs_from_mail.txt'
    mail_tasks = task_IDs_from_mail_path(mail_path)
    with open(mail_file, 'w') as f:
        f.write('\n'.join(mail_tasks))
    retry_list_file = mail_file
    user = 'skaphle'
    use_retry_list = True
    if retry:
        # campaign_dict = get_user_task_list('skaphle', campaign, rversion, retry_states)
        # for JobID in campaign_dict.keys():
        #     task = campaign_dict[JobID]['jediTaskID']
        #     print('Retrying task with ID {} and name {}'.format(
        #         task, campaign_dict[JobID]['taskName']))
        #     if not dry_run:
        #         pandatools.Client.retryTask(task, verbose=False, properErrorCode=True)

        retry_list = []
        with open(retry_list_file) as f:
            for line in f:
                if line.startswith('#') or line.isspace():
                    continue
                line = line.strip()
                retry_list.append(int(line))

        if use_retry_list:
            print('Found {} tasks in {}'.format(len(retry_list), retry_list_file))
            for task in retry_list:
                print('Retrying task with ID {}'.format(task))
                if not dry_run:
                    pandatools.Client.retryTask(task, verbose=False, properErrorCode=True)
        else:
            # from retry_tasks.py
            # get tasks for user
            state, jediTask_list, jediTask_dict = pandatools.Client.\
                getJobIDsJediTasksInTimeRange(earliest_date, dn=user_full_name[user])

            if state != 0:
                print('Something went wrong; check grid proxy')
                exit(1)

            # filter for campaign/version
            campaign_dict = {k: v for k, v in jediTask_dict.items()
                             if re.findall(r'user\.{}\.{}\..*_{}/?$'.format(user, campaign, rversion), v['taskName'])}

            # filter for finished tasks
            finished_dict = campaign_dict
            finished_dict = {k: v for k, v in campaign_dict.items()
                             if v['status'] == 'finished'}

            # retry tasks (keys are panda job IDs, tasks are from the jediTaskID keyword)
            for JobID in finished_dict.keys():
                task = finished_dict[JobID]['jediTaskID']
                task_state = finished_dict[JobID]['status']
                print('Retrying task with ID {} and name {} currently in state {}'.format(
                    task, finished_dict[JobID]['taskName'], task_state))
                if not dry_run:
                    pandatools.Client.retryTask(task, verbose=False, properErrorCode=True)
