#!/usr/bin/env python
#
# Collect submissions and place them in one single directory
#
# This file is a rewrite of CollectSubmissions.py and SubmitGridJobs.py
#   from the early 2019 MEM grid campaign.
# Targets for rewrite:
# - reduce number of tasks by combining samples
#   -> one job submission per folder or similar, not one per dsid
# - use same intarball for all jobs no matter sample or systematic
#
"""
Script to manage grid submission of A++ and MemTk jobs.

Run "python GridSubmission.py -h" for more info

The main ways to use this script are these subcommands:
prepare  -  prepare the directory structure for future submission
submit   -  submit from the prepared structures
convert  -  offline copy/move/rename utility (obsolete, use GridCampaign,py)
"""

# ------- imports ---------------------------------------------------------------------- #
from __future__ import print_function

import argparse
import collections
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import SafeConfigParser as ConfigParser
import glob
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile


# ------- argparse --------------------------------------------------------------------- #
parser = argparse.ArgumentParser(description='Manage and submit A++ and MemTk grid jobs')
parser.add_argument('--all-samples-file', help='file with all available samples [default: all_samples.txt]')
parser.add_argument('--all-systs-short-file',
                    help=('file with all allowed systematics and'
                          'shorted names [default: all_systematics_short.txt]'))
parser.add_argument('--sample-config-file', help='file with compacted DID lists [default: sample_config.json]')
parser.add_argument('--verbose', '-V', '-v', action='count', help='increase verbosity')
parser.add_argument('--config-file', '-c', help='config file to set CL arguments, campaign etc')
subparsers = parser.add_subparsers(dest='command')

parser_prep = subparsers.add_parser('prepare', help='prepare for submission')
parser_prep.add_argument('--create-intarball', action='store_true', help='(re)create intarball?')
parser_prep.add_argument('--sample-list', help='file with list of samples [default: samples.txt]')
parser_prep.add_argument('--syst-list', help='file with list of systematics [default: systematics.txt]')
parser_prep.add_argument('--target-dir', help='directory to save submission scripts in')
parser_prep.add_argument('--overwrite-target-dir', action='store_true',
                         help='remove previous target if it exists?')
parser_prep.add_argument('--default-txt-targets', action='store_true',
                         help='copy lists under default name in TARGET_DIR')

parser_sub = subparsers.add_parser('submit', help='submit prepared grid jobs')
parser_sub.add_argument('--base-dir', help='target of prepare step [default: current working dir]')
parser_sub.add_argument('--sample-list', help='file with list of samples')
parser_sub.add_argument('--syst-list', help='file with list of systematics')
parser_sub.add_argument('--do-submit', action='store_true', help='otherwise --noSubmit is used')
parser_sub.add_argument('--debug', action='store_true', help='submit only one job with --debugMode')
parser_sub.add_argument('--allowTaskDuplication', action='store_true',
                        help='submit with --allowTaskDuplication')
parser_sub.add_argument('--prun-options', help='options to pass on to prun')

parser_dl = subparsers.add_parser('download', help='download processed grid jobs')

parser_conv = subparsers.add_parser('convert', help='convert grid output to A++/MemTk form')
parser_conv.add_argument('--download-dir', help='directory with downloaded root files')
parser_conv.add_argument('--hadd-target-dir', help='target directory for hadded root files')
parser_conv.add_argument('--DSIDs-file', help='A++ DSID list for pattern matching')


# ------- global variables ------------------------------------------------------------- #

#
# BEGIN User variables: set manually for this file
#
# define campaign
campaign = 'schannel_App_MemTk'
# ntup_version = 'v29_a_nominal'
# grid_version = 'v0_nominal'
ntup_version = 'v29lj_MC16a'
grid_version = 'v4'
target_subdir = 'submit/' + ntup_version + '/' + grid_version

# debug options
test_one_syst = False
test_one_sample = False

#
# END User variables
#

# configuration defaults, may be overwritten by a config file
config_defaults = {
    'campaign': 'schannel_App_MemTk_test',
    'ntup_version': 'v29lj_MC16a',
    'grid_version': 'v0',

    # produce the template files with A++ and place them in the current directory before running this script
    # examples are:
    # /rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_test_submission_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/TEMPLATE/analysis_run_template.C
    # /rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_test_submission_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/TEMPLATE/app_template.run
    # /rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_test_submission_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemTkAnalysis/analysis/2Jets/lnu/TEMPLATE/410470ttbar/410470ttbar.run
    'analysis_run_template': 'analysis_run_template.C',
    'app_template': 'app_template.run',
    # 'mem_template': 'mem_template.run',  # original template from MemTk
    'mem_template': 'mem_template_subjobs.run',  # manually changed template

    # command line options
    'all_samples_file': 'all_samples.txt',
    'all_systs_short_file': 'all_systematics_short.txt',
    'sample_config_file': 'sample_config.json',  # note this has no default, i.e. is required
    'verbose': '0',

    'sample_file': 'samples.txt',
    'syst_file': 'systematics.txt',
    'target_dir': './submit/',
    'default_txt_targets': 'false',
    'create_intarball': 'false',

    'base_dir': '',
    'sample_file': 'samples.txt',
    'syst_file': 'systematics.txt',
    'prun_options': '',

    # options for convert
    'download_dir': './',
    'DSIDs_file': 'SgTopDSIDs_mc16a_v29_lj.txt',
    'hadd_target_dir': './'
}

cparser = ConfigParser(defaults=config_defaults)

# regular expressions
# GridOutput directory example and what should match:
# user.skaphle.schannel_App_MemTk.Zee_nominal_v29_a_nominal.v0_nominal_mem_output.root
# (<scope>).(<stream>).(<sample>)_(<syst_short>)_(<ntup_ver>).(<grid_ver>)_(<app/mem>_output.root)
rexGridOutputDir = re.compile(r'^(user\.\w+)\.(\w+)\.([a-zA-Z]+)_(\w+)_(v\w+)\.(v\w+)' +
                              r'(_(?:app|mem)_output\.root|\.log)(?:\.[0-9]+)?$')
# example file in this directory and what should match:
# user.skaphle.364114.e5299_s3126_r9364_p3830.19663067._000001.mem_output.root
# (<scope>).(<DID>).(<tags>).(<taskid>)._(<counting number>).(<app/mem>_output.root)
rexGridOutputFile = re.compile(r'^(user\.\w+)\.([0-9]+)\.(\w+)\.([0-9]+)\._([0-9]+)' +
                               r'\.((?:app|mem)_output\.root)$')

# reduce number of tasks by grouping dsids into samples
# check SampleSizeLists/v15_v25lj_default_MC16d_SR.txt for groupings?
# update 2019-11-12: do highest-event samples first
# update 2019-11-13: change to use json files
# update 2019-11-14: add options to run MemTk subjobs; add this to __main__

# # load sample list from json file
# with open('/users/eeh/kaphle/v29_grid_submission/run01_submit_all/sample_lists_2_highest_first.json') as f:
#     sample_lists = json.load(f)

# # original/first configuration
# with open('sample_lists_original.json') as f:
#     sample_lists_original = json.load(f)


# ------- functions -------------------------------------------------------------------- #
def __mkdir(target):
    """Create directory without error if it exists (like mkdir -p)."""
    try:
        os.makedirs(target)
    except OSError as e:
        if e.errno != 17:
            raise


def create_inDsTxt_file(dsid_list, ntuple_list_file, outfile, DEBUG=0):
    """Search lines matchind dsids from DSID_LIST in a file NTUPLE_LIST_FILE
and write it into a file OUTFILE intended for use as an inDsTxt file. """
    ntup_list = []
    # read in dsid list
    with open(ntuple_list_file) as f:
        for dsid in dsid_list:
            f.seek(0)
            ntup = ''
            regexp = re.compile(dsid)
            for line in f:
                line = line.strip()
                if line.startswith('#') or line.isspace():
                    continue
                if regexp.search(line) is not None:
                    ntup = line
                    ntup_list.append(ntup)
                    if DEBUG > 2:
                        print('dbg3:   Found DSID {} in {}'.format(
                            dsid, ntuple_list_file))
                    break
                # if dsid == '410658' or dsid == '410644':
                #     print('line: "{}"'.format(line))
                #     print('regex result: {}'.format(regexp.search(line)))
            if ntup == '':
                print('  Warning: could not find DSID {} in {}'.format(
                    dsid, ntuple_list_file))
    # write out ntuple files
    with open(outfile, 'w') as f:
        f.write('\n'.join(ntup_list))


def read_entries_from_file(file_):
    """Read entries from a file line-by-line and return a list. """
    entries = []
    nentries = 0
    with open(file_) as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            entry = line.strip()
            entries.append(entry)
            nentries += 1
    print('  Found {} entries in file {}'.format(nentries, file_))
    return entries


def read_syst_with_short_name_from_file(syst_file):
    """Read systematics from a file with long and short names. """
    systs = []
    nsyst = 0
    short_from_syst = {}
    with open(syst_file) as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            syst = line.strip().split()[0]
            syst_short = line.strip().split()[1]
            systs.append(syst)
            short_from_syst[syst] = syst_short
            nsyst += 1
    print('  Found {} systematics in file {}'.format(nsyst, syst_file))
    return systs, short_from_syst


def find_DIDsample_in_DSIDs_list(search_string, DSIDs_file):
    """Find the DIDsample name expected by A++ by searching a DSIDs list."""
    with open(DSIDs_file) as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            if line.find(search_string) != -1:
                return line.split(',')[0]
    # if not found:
    return None


# ------- main program ----------------------------------------------------------------- #
if __name__ == '__main__':
    # parse cmd line arguments for main parser
    if len(sys.argv) == 1:
        parser.print_usage()
        sys.exit(1)
    args = parser.parse_args()

    # read from config file if available
    config_file = args.config_file
    if config_file is not None:
        cparser.read(config_file)
    default_cfg_section = 'DEFAULT'
    if cparser.has_section('main'):
        cfg_section = 'main'
    else:
        cfg_section = default_cfg_section
    campaign = cparser.get(cfg_section, 'campaign')
    ntup_version = cparser.get(cfg_section, 'ntup_version')
    grid_version = cparser.get(cfg_section, 'grid_version')
    analysis_run_template = cparser.get(cfg_section, 'analysis_run_template')
    app_template = cparser.get(cfg_section, 'app_template')
    mem_template = cparser.get(cfg_section, 'mem_template')
    # command line options
    all_samples_file = cparser.get(cfg_section, 'all_samples_file')
    all_systs_short_file = cparser.get(cfg_section, 'all_systs_short_file')
    sample_config_file = cparser.get(cfg_section, 'sample_config_file')
    DEBUG = cparser.getint(cfg_section, 'verbose')

    # allow command line arguments to overwrite config file
    if args.verbose > 0:
        DEBUG = args.verbose
    if args.all_samples_file is not None:
        all_samples_file = args.all_samples_file
    if args.all_systs_short_file is not None:
        all_systs_short_file = args.all_systs_short_file
    if args.sample_config_file is not None:
        sample_config_file = args.sample_config_file

    # sample configuration with DID lists; also to define subjobs and nfilesperjob
    with open(sample_config_file) as f:
        sample_config = json.load(f)
    sample_lists = {k: sample_config[k]['dids'] for k in sample_config.keys()}
    sample_nsubjobs = {k: sample_config[k].get('nsubjobs') for k in sample_config.keys()}
    sample_subjobid = {k: sample_config[k].get('subjobid') for k in sample_config.keys()}
    sample_nfilesperjob = {k: sample_config[k].get('nfilesperjob') for k in sample_config.keys()}

    # debug message how this script was called
    if DEBUG > 0:
        print('dbg1: ', str(args))

    # fixed file names
    in_tarball = 'intarball.tgz'
    cuba_tarball = 'Cuba-3.0.tar.gz'
    app_tarball = 'APlusPlus.tgz'
    mem_tarball = 'libMEM.tgz'

    # prepare?
    if args.command == 'prepare':
        print('\nCalled "prepare" subcommand')

        # read from config file if available
        if cparser.has_section('prepare'):
            cfg_section = 'prepare'
        else:
            cfg_section = default_cfg_section
        sample_file = cparser.get(cfg_section, 'sample_file')
        syst_file = cparser.get(cfg_section, 'syst_file')
        target_dir = cparser.get(cfg_section, 'target_dir')
        default_txt_targets = cparser.getboolean(cfg_section, 'default_txt_targets')
        create_intarball = cparser.getboolean(cfg_section, 'create_intarball')

        # process arguments from prepare subparser
        create_intarball = create_intarball or args.create_intarball
        if args.syst_list is not None:
            syst_file = args.syst_list
        if args.sample_list is not None:
            sample_file = args.sample_list
        if args.target_dir is not None:
            target_dir = args.target_dir
        overwrite = args.overwrite_target_dir
        default_txt_targets = default_txt_targets or args.default_txt_targets

        # read available samples/ntuples
        all_samples = read_entries_from_file(all_samples_file)

        # read long and short names for available systs
        all_systs, short_from_syst = read_syst_with_short_name_from_file(all_systs_short_file)

        # read selected samples
        samples = read_entries_from_file(sample_file)

        # read selected systematics
        systs = read_entries_from_file(syst_file)

        # make sure GRID_HOME is set as base of output
        grid_home = os.getenv('GRID_HOME')
        if grid_home is None:
            raise OSError('Need to set up GRID_HOME first')

        # create target directory
        if overwrite:
            shutil.rmtree(target_dir)
        __mkdir(target_dir)

        # add systematics/samples files
        intxtfiles = [all_samples_file, all_systs_short_file, sample_file,
                      syst_file, sample_config_file]
        if default_txt_targets:
            outtxtfiles = [cparser.defaults()['all_samples_file'],
                           cparser.defaults()['all_systs_short_file'],
                           cparser.defaults()['sample_file'],
                           cparser.defaults()['syst_file'],
                           cparser.defaults()['sample_config_file']]
        else:
            outtxtfiles = intxtfiles
        if cparser.has_section('submit'):
            intxtfiles.append(config_file)
            outtxtfiles.append(os.path.basename(config_file))
        for infile, outfile in zip(intxtfiles, outtxtfiles):
            shutil.copy(infile, os.path.join(
                target_dir, os.path.basename(outfile)))

        # create the intarball and copy in a separate subdirectory
        # only create an intarball subdirectory at DEBUG > 1
        intar_dir = os.path.join(target_dir, os.path.splitext(in_tarball)[0])
        intarball_full_path = os.path.join(target_dir, in_tarball)
        cuba_full_input_path = os.path.join(grid_home, cuba_tarball)
        app_full_input_path = os.path.join(grid_home, app_tarball)
        mem_full_input_path = os.path.join(grid_home, mem_tarball)
        if create_intarball:
            if DEBUG > 1:
                __mkdir(intar_dir)
            with tarfile.open(intarball_full_path, 'w:gz') as itb:
                if DEBUG > 1:
                    print('dbg2:  Copy cuba library')
                    shutil.copy(cuba_full_input_path, intar_dir)
                print('Add cuba library')
                itb.add(cuba_full_input_path, cuba_tarball)
                if DEBUG > 1:
                    print('dbg2:  Copy A++ library')
                    shutil.copy(app_full_input_path, intar_dir)
                print('Add A++ library')
                itb.add(app_full_input_path, app_tarball)
                if DEBUG > 1:
                    print('dbg2:  Copy MemTk library')
                    shutil.copy(mem_full_input_path, intar_dir)
                print('Add MemTk library')
                itb.add(mem_full_input_path, mem_tarball)
                if DEBUG > 1:
                    print('dbg2:  Copy A++ analysis script template')
                    shutil.copy(analysis_run_template, intar_dir)
                print('Add A++ analysis script template')
                itb.add(analysis_run_template, os.path.basename(analysis_run_template))
                if DEBUG > 1:
                    print('dbg2:  Copy A++ shell run template')
                    shutil.copy(app_template, intar_dir)
                print('Add A++ shell run template')
                itb.add(app_template, os.path.basename(app_template))
                if DEBUG > 1:
                    print('dbg2:  Copy MEM shell run template')
                    shutil.copy(mem_template, intar_dir)
                print('Add MEM shell run template')
                itb.add(mem_template, os.path.basename(mem_template))

        # next step: loop to create command files
        print('\nCopy submission files to {}'.format(target_dir))
        for syst in systs:
            new_syst_dir = os.path.join(target_dir, syst)
            if DEBUG > 0:
                print('dbg1: Copy systematic {}'.format(syst))
            __mkdir(new_syst_dir)
            syst_short = short_from_syst[syst]
            prepared_DIDs = []
            for sample in samples:
                # read from sample configuration
                dsid_list = sample_lists[sample]
                nsubjobs = sample_nsubjobs[sample]
                subjobid = sample_subjobid[sample]
                nfilesperjob = sample_nfilesperjob[sample]
                if nsubjobs is None or subjobid is None:
                    nsubjobs = 1
                    subjobid = 1
                if nfilesperjob is None:
                    nfilesperjob = 1

                new_sample_dir = os.path.join(new_syst_dir, sample)
                if DEBUG > 1:
                    print('dbg2:  Copy sample {}'.format(sample))
                __mkdir(new_sample_dir)

                # skip nominal only samples
                if syst != 'nominal' and (sample.find('data') != -1 or
                                          sample.find('QCD') != -1):
                    continue

                # create inDsTxt file
                inDsTxt_file_base = 'inDS.txt'
                inDsTxt_file_full = os.path.join(new_sample_dir, inDsTxt_file_base)
                inDsTxt_file = os.path.join(syst, sample, inDsTxt_file_base)
                create_inDsTxt_file(dsid_list, all_samples_file, inDsTxt_file_full, DEBUG)

                # create prun command and save to file
                rootVer = '--rootVer=6.08/02'
                cmtConfig = '--cmtConfig=x86_64-slc6-gcc49-opt'
                bexec = "--bexec='tar -xzf {};tar -xzf {};tar -xzf {}'".format(
                     cuba_tarball, app_tarball, mem_tarball)
                _exec = "--exec='./{} {};./{} {} {}'".format(
                    app_template, syst, mem_template, nsubjobs, subjobid)
                destSE = '--destSE=DESY-ZN_LOCALGROUPDISK'
                inDsTxt = '--inDsTxt={}'.format(inDsTxt_file)
                inTarBall = '--inTarBall={}'.format(in_tarball)
                outDS = '--outDS=user.skaphle.{}.{}_{}_{}.{}'.format(
                    campaign, sample, syst_short, ntup_version, grid_version)
                outputs = '--outputs=app_output.root,mem_output.root'
                other = ['--long',  # required to get to long grid queue
                         # '--excludedSite=ANALY_GOEGRID',  # not sure if necessary any more
                         '--nFilesPerJob={}'.format(nfilesperjob),  # reduce load for MEM
                         '--writeInputToTxt=IN:input.txt',  # give list of input DIDs
                         '--addNthFieldOfInDSToLFN=4,7',  # be able to tell apart DIDs within grouping
                         '--forceStaged']  # avoid a certain grid error
                command = ' '.join(['prun', rootVer, cmtConfig, bexec, _exec, destSE,
                                    inDsTxt, inTarBall, outDS, outputs] + other)
                # print(command)
                command_file = os.path.join(new_sample_dir, 'command.txt')
                with open(command_file, 'w') as f:
                    f.write(command)

                # keep track of prepared DIDs
                prepared_DIDs.extend(dsid_list)

                if test_one_sample:
                    break

            # check duplicates and missed DIDs
            times_prepared = collections.Counter(prepared_DIDs)
            duplicates = [did for did, count in times_prepared.items() if count > 1]
            if len(duplicates) > 0:
                print('  Duplicates during preparation:')
                for did in duplicates:
                    print('   Prepared did {} {} times'.format(did, times_prepared[did]))
            missing_samples = list(all_samples)  # note this is full file name
            for d in prepared_DIDs:
                found = False
                for pos, s in enumerate(missing_samples):
                    if s.find(d) != -1:
                        found = True
                        missing_samples.pop(pos)
                        break
                if not found and did not in duplicates:
                    print('   Did not find DID {} in list of samples'.format(d))
            # # check missing samples: only makes sense with full list in samples.txt
            # if len(missing_samples) > 0:
            #     print('  Samples which were not prepared:')
            #     for ms in missing_samples:
            #         print('   {}'.format(ms))

            if test_one_syst:
                break

        print('Done, placed submission files in {0}'.format(target_dir))

    # submit?
    elif args.command == 'submit':
        print('\n Submit jobs')

        # read from config file if available
        if cparser.has_section('submit'):
            if DEBUG > 0:
                print('dbg1: Reading config file section "submit"')
            cfg_section = 'submit'
        else:
            cfg_section = default_cfg_section
        base_dir = cparser.get(cfg_section, 'base_dir')
        sample_file = cparser.get(cfg_section, 'sample_file')
        syst_file = cparser.get(cfg_section, 'syst_file')
        prun_options = cparser.get(cfg_section, 'prun_options')

        # process arguments from submit subparser
        if args.base_dir is not None:
            base_dir = args.base_dir
        if not base_dir:
            base_dir = os.getcwd()
        if args.syst_list is not None:
            syst_file = args.syst_list
        elif not os.path.isabs(syst_file):
            syst_file = os.path.join(base_dir, syst_file)
        if args.sample_list is not None:
            sample_file = args.sample_list
        elif not os.path.isabs(sample_file):
            sample_file = os.path.join(base_dir, sample_file)
        if args.prun_options is not None:
            prun_options = args.prun_options
        do_submit = args.do_submit
        debug = args.debug
        allow_task_duplication = args.allowTaskDuplication

        # append base_dir per default
        if args.all_systs_short_file is None and not os.path.isabs(all_systs_short_file):
            all_systs_short_file = os.path.join(base_dir, all_systs_short_file)
        if args.all_samples_file is None and not os.path.isabs(all_samples_file):
            all_samples_file = os.path.join(base_dir, all_samples_file)

        # read available samples/ntuples
        all_samples = read_entries_from_file(all_samples_file)

        # read long and short names for available systs
        all_systs, short_from_syst = read_syst_with_short_name_from_file(all_systs_short_file)

        # read selected samples
        samples = read_entries_from_file(sample_file)

        # read selected systematics
        systs = read_entries_from_file(syst_file)

        # use RUCIO_ACCOUNT information
        user = os.getenv('RUCIO_ACCOUNT')
        if user is None:
            raise ValueError('RUCIO_ACCOUNT unknown, setup rucio before running this script')

        # loop over systematics and samples
        for syst in systs:
            if DEBUG > 0:
                print('dbg1: Submitting systematic {}'.format(syst))
            syst_short = short_from_syst[syst]
            for sample in samples:
                if DEBUG > 1:
                    print('dbg2:  Submitting sample {}'.format(sample))
                if sample not in sample_lists.keys():
                    print('Warning: could not find sample {} in current'
                          ' distribution. Skip!'.format(sample))
                    continue

                # skip nominal only samples
                if syst != 'nominal' and (sample.find('data') != -1 or
                                          sample.find('QCD') != -1):
                    continue

                # # build directory for submission
                # submit_dir = os.path.join(base_dir, syst, sample, 'run')
                submit_dir = base_dir
                # __mkdir(submit_dir)
                os.chdir(submit_dir)
                # add inDS file
                inDsTxt_file = os.path.join(base_dir, syst, sample, 'inDS.txt')
                # shutil.copy(inDsTxt_file, submit_dir)

                # check existence of intarball
                if not os.path.exists(in_tarball):
                    print('Warning: no intarball {} found. Turning off'
                          ' submission'.format(in_tarball))
                    do_submit = False

                # take command from command file
                cmd_file = os.path.join(base_dir, syst, sample, 'command.txt')
                with open(cmd_file) as cf:
                    cmd = cf.read().strip()
                # replace name with current user name (env RUCIO_ACCOUNT)
                cmd = cmd.replace('skaphle', user)
                # add noSubmit if we don't explicitly ask for it
                if not do_submit:
                    cmd = cmd + ' --noSubmit'
                    print('  Using prun --noSubmit per default against accidental submissions. '
                          'If you are sure you want to submit, use --do-submit')
                # use debugMode
                if debug:
                    cmd = cmd + ' --debugMode'
                if allow_task_duplication:
                    cmd = cmd + ' --allowTaskDuplication'
                # general prun option
                if prun_options:
                    cmd = cmd + ' ' + prun_options

                # prun
                if DEBUG > 1:
                    print('dbg2:  Executing prun command: "{}"'.format(cmd))
                try:
                    subprocess.check_call(cmd, shell=True)
                except subprocess.CalledProcessError as e:
                    if e.returncode != 90:
                        raise

                # only use one job for --debugMode
                if debug:
                    break

            # need double break for double loops
            if debug:
                break

    # convert?
    # - this function replaces hadd_files_download2gridid.sh
    # - use copy_files_gridid2version.sh afterwards
    # - this function works but is replaced within GridCampaign.py
    elif args.command == 'convert':
        print('\n Convert offline root files')

        # read from config file if available
        if cparser.has_section('convert'):
            if DEBUG > 0:
                print('dbg1: Reading config file section "convert"')
            cfg_section = 'convert'
        else:
            cfg_section = default_cfg_section
        # download_dir = cparser.get(cfg_section, 'download_dir')
        # hadd_target_dir = cparser.get(cfg_section, 'hadd_target_dir')
        DSIDs_file = cparser.get(cfg_section, 'DSIDs_file')

        # process arguments from convert subparser
        if args.download_dir is not None:
            download_dir = args.download_dir
        else:
            print('--download-dir is necessary for now; exit')
            exit(1)
        if args.hadd_target_dir is not None:
            hadd_target_dir = args.hadd_target_dir
        else:
            print('--hadd-target-dir is necesssary for now; exit')
            exit(1)
        if args.DSIDs_file is not None:
            DSIDs_file = args.DSIDs_file

        # get necessary paths etc
        # MemDisc_target_dir = '3'  # directory in A++ directory structure as MemDisc input
        # filter_expr = '4'
        gridID = ntup_version + '.' + grid_version
        do_app = False
        do_mem = True
        hadd_target_dir = os.path.join(hadd_target_dir, gridID)
        __mkdir(hadd_target_dir)
        # target file format; place this in default area?
        prefix = 'ntup_schan'
        njets = '2j'
        channel = 'lnu'
        target_format = '{app_mem_}{prefix}_{njets}_{channel}_{syst}_{DIDsample}.root'

        # read long and short names for available systs
        all_systs, short_from_syst = read_syst_with_short_name_from_file(all_systs_short_file)

        # loop over entries in input directory
        for sample_dir in os.listdir(download_dir):
            sample_dir_full = os.path.join(download_dir, sample_dir)
            if not os.path.isdir(sample_dir_full):
                if DEBUG > 0:
                    print('dbg1: {} is not a directory. Skip!'.format(sample_dir))
                continue

            # TODO: add option to filter first

            # take info from directory name with regex
            m = rexGridOutputDir.match(sample_dir)
            if m is None:
                print('Unrecognized directory {}. Skip!'.format(sample_dir))
                continue
            dscope, stream, sample, syst_short, ntup_ver, grid_ver, dext = m.groups()

            # sanity checks
            # check scope?
            if stream != campaign:
                print('Unknown stream {}, expected {}. Skip!'.format(stream, campaign))
                continue
            if sample not in sample_lists.keys():
                print('Unknown sample {}. Skip!'.format(sample))
                continue
            if syst_short not in short_from_syst.values():
                print('Unknown short systematic {}. Skip!'.format(syst_short))
                continue
            # check ntup_ver?
            # check grid_ver?
            # check dext whether app/mem output; TODO: add neither option?
            app_mem = None
            if dext == '_app_output.root':
                app_mem = 'app'
            elif dext == '_mem_output.root':
                app_mem = 'mem'
            else:
                print('Could not determine if directory {} is from app or mem job. Skip!',
                      sample_dir)
                continue

            # reverse dict lookup from short form to long form of systematic
            syst = [s for s, ss in short_from_syst.items() if ss == syst_short][0]

            # loop over sample_dir files to check for dids
            dids_found_in_sample = []
            for rootfile in os.listdir(sample_dir_full):
                rootfile_full = os.path.join(sample_dir_full, rootfile)
                if not os.path.isfile(rootfile_full):
                    print('{} is not a regular file. Skip!', rootfile)
                    continue
                n = rexGridOutputFile.match(rootfile)
                fscope, fdid, tags, taskid, number, fext = n.groups()

                # make some sanity checks
                if fscope != dscope:
                    print('Warning: non-matching scopes in file and sample:'
                          ' {} != {}'.format(fscope, dscope))
                if fdid not in sample_lists[sample]:
                    print('Warning: {} is not in list for sample {}'.format(fdid, sample))
                if fext != dext[1:]:  # dext has _ first
                    print('Warning: non-matching file extensions in file and samples:'
                          ' {} != {}'.format(fext, dext[1:]))

                # add did to list
                if fdid not in dids_found_in_sample:
                    dids_found_in_sample.append(fdid)

            # now that we built the list, use this to restrict command for hadd
            for did in dids_found_in_sample:
                # get DIDsample from our short DID
                DIDsample = find_DIDsample_in_DSIDs_list(did, DSIDs_file)
                format_dict = {'app_mem_': app_mem + '_', 'prefix': prefix, 'njets': njets,
                               'channel': channel, 'syst': syst, 'DIDsample': DIDsample}
                hadd_target_dir_appmem = os.path.join(hadd_target_dir, app_mem)
                __mkdir(hadd_target_dir_appmem)
                target_file = target_format.format(**format_dict)
                target_file_full = os.path.join(hadd_target_dir_appmem, target_file)

                # for now don't need to add this as CLI option
                do_overwrite = False
                do_add = False
                # check if target exists
                if os.path.exists(target_file_full):
                    if do_overwrite:
                        pass  # overwrite is the default mode for hadd
                        # rm_cmd = ['rm', '-r', target_file_full]
                        # print('Target {} exists; removing first...'.format(target_file_full))
                        # subprocess.call(rm_cmd)
                    else:
                        print('Target {} exists. Don\'t overwrite, skip!'.format(target_file_full))
                        continue
                source_regex = "{}/{}.{}.*.{}".format(sample_dir_full, dscope, did, dext[1:])
                source_files = glob.glob(source_regex)
                hadd_cmd = ['hadd', '-f9', target_file_full] + source_files
                if DEBUG > 1:
                    print('dbg2: ' + str(hadd_cmd))
                subprocess.call(hadd_cmd)
