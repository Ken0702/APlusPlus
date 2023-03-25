#!/usr/bin/env python
#
# change in v29: use print_function, add -v (-v->-V)
"""
Script to split samples and download.

By default, the script
 - calculates free size as minimum of as/ms,
 - looks for downloaded and to be downloaded samples and
 - prints a list of machines and respective free/used/remaining space.
The sample list files are dat[m/s][1-8].txt and any additional argument.
The location of samples on the disks are calculated automatically.
Options for the script:
 -m, -s, -v  use datm/s/v for download (determined by HOST by default)
 -i  ignore other cluster (default: make samples fit on all)
 -f  force recalculation of sample splitting to dats[1-8]_new.txt;
       note that this only applies to additional samples from arguments, not dats[1-8].txt
     (advice: move samples allocated this way from *_new.txt to *.txt before download)
 -d  download samples (off by default)
 -V  increase verbosity
 -k  skip DIDs only defined in default files dats[1-8].txt when reading DIDs

To let the cluster download the samples in the background, do
$ echo 'source ~/atlas/setup-grid.sh && ~/atlas/download/mc16_v23lj/split_downloads.py -d' | qsub -l nodes=ms01b -
"""

# ------- imports ---------------------------------------------------------------------- #
from __future__ import division, print_function

import argparse
import datetime
import errno
import math
import operator
import os
import pdb
import re
import rucio.client
import rucio.common.exception
import shutil
import sys


# ------- argparse --------------------------------------------------------------------- #
parser = argparse.ArgumentParser(description='Distribute and download samples')
parser.add_argument('--ignore-cluster', '-i', action='store_true',
                    help=' ignore other clusters [default: make samples fit on all]')
parser.add_argument('--datm', '-m', action='store_const', dest='machine_location',
                    const='datm', help='use datm for download [default: determined by HOST]')
parser.add_argument('--dats', '-s', action='store_const', dest='machine_location',
                    const='dats', help='use dats for download [default: determined by HOST]')
parser.add_argument('--datv', '-v', action='store_const', dest='machine_location',
                    const='datv', help='use datv for download [default: determined by HOST]')
parser.add_argument('--force-recalc', '-f', action='store_true',
                    help='force recalculation of sample splitting to dats[1-8]_new.txt; '
                    'note that this only applies to additional samples from arguments, '
                    'not dats[1-8].txt (advice: move samples allocated this way from '
                    '*_new.txt to *.txt before download)')
parser.add_argument('--download', '-d', action='store_true', help='download samples [default: off]')
parser.add_argument('--verbose', '-V', action='count', help='increase verbosity')
parser.add_argument('--skip-default', '-k', action='store_true', help='skip DIDs only '
                    'defined in default files dats[1-8].txt when reading DIDs')
parser.add_argument('files', nargs='*', help='list of samples to add')
parser.add_argument('--delete-old', action='store_true',
                    help='delete samples on disk but not listed')
parser.add_argument('--remove-old', action='store_true',
                    help='remove old DIDs from dat[msv][1-8].txt files')
parser.add_argument('--print-location', '-p', action='store_true',
                    help='print a list of locations for samples in input file')


# ------- global variables ------------------------------------------------------------- #
this_file = os.path.abspath(sys.argv[0])
print('Running {}'.format(this_file))
print('Current time: {}'.format(datetime.datetime.now()))

rucio_client = rucio.client.Client()  # rucio client for grid file checks/downloads
machines = [1, 2, 3, 4, 5, 6, 7, 8]  # each cluster group (m,s,v) has 8 machines

# regex compilation for overview and speed
rexScope = re.compile(r'^(user\.\w+)\.')       # extract scope from did
rexAntiMuon = re.compile(r'.*\.AntiMuon\..*')  # to check if did is AntiMuon sample
rexJetLepton = re.compile(r'.*JetLepton.*')    # to check if did is JetLepton sample
# rexDid = re.compile(                           # matches all dids
#     r'^(user\.\w+)\.'     # scope
#     r'(mc|data)?.*\.'     # data/mc; JetLepton does not have this
#     r'(?:sys[0-9._]*_)?'  # optional: AntiMuon systematics
#     r'(v[0-9a-z]+)'       # ntuple version string
#     r'(?:_AT[0-9.]+)?'    # optional: AT version
#     r'(?:\.[0-9])?'       # optional: recreated sample version
#     r'_out(?:(?:put_)|\.)root$')  # root outputfile suffix
rexDidDataMC = re.compile(                     # matches all non-fake dids
    r'^(user\.\w+)\.'     # scope
    r'(mc|data)'          # data/mc
    r'.*'                 # middle part
    r'\.(v[0-9a-z]+)'     # ntuple version string
    r'(?:_AT[0-9.]+)?'    # optional: AT version
    r'(?:\.[0-9])?_out\.root$')  # root outputfile suffix
rexDidJetLepton = re.compile(                  # matches a Jet-Lepton DID
    r'^(user\.\w+)\.'     # scope
    r'()'                 # no data/mc part here
    r'.*'                 # middle part
    r'\.(v[0-9a-z]+)'     # ntuple version string
    # r'(?:_AT[0-9.]+)?'  # no AT version in JetLepton yet
    r'(?:\.[0-9])?'       # optional: recreated sample version
    r'_out(?:(?:put_)|\.)root$')  # root outputfile suffix
rexDidAntiMuon = re.compile(                   # matches an Anti-Muon DID
    r'^(user\.\w+)\.'     # scope
    r'(data)'             # data only
    r'.*\.AntiMuon\.'     # middle part
    r'(?:sys[0-9._]*_)?'  # optional: AntiMuon systematics
    r'(v[0-9a-z]+)'       # ntuple version string
    # r'(?:_AT[0-9.]+)?'  # no AT version in AntiMuon yet
    r'(?:\.[0-9])?'       # optional: recreated sample version
    r'_out(?:(?:put_)|\.)root$')  # root outputfile suffix
rexDid = re.compile('|'.join([rexDidDataMC.pattern, rexDidJetLepton.pattern, rexDidAntiMuon.pattern]))



# rexDid = re.compile(r'^(user\.\w+)\.(mc|data).*\.(v[0-9a-z]+)(?:\.[0-9])?_out\.root$')
# some samples have AT version included:
# rexDid = re.compile(
#     r'^(user\.\w+)\.'     # scope
#     r'(mc|data).*\.'      # data/mc
#     r'(v[0-9a-z]+)'       # ntuple version string
#     r'(?:_AT[0-9.]+)?'    # optional: AT version
#     r'(?:\.[0-9])?_out\.root$')  # root outputfile suffix
# rexAntiMuon = re.compile(r'.*\.AntiMuon\..*')
# rexJetLepton = re.compile(r'.*JetLepton.*')
# # JetLepton has a different syntax, use this if rexDid fails
# # rexDidRelaxed = re.compile(r'^(user\.\w+)\.(mc|data)?.*\.(v[0-9a-z]+)(?:\.[0-9])?_out(?:(?:put_)|\.)root$')
# # some samples have AT version included
# rexDidAll = re.compile(r'^(user\.\w+)\.(mc|data)?.*\.(v[0-9a-z]+)(?:_AT[0-9.]+)?(?:\.[0-9])?_out(?:(?:put_)|\.)root$')
# rexDidJetLepton = re.compile(r'^(user\.\w+)\.(mc|data)?.*\.(v[0-9a-z]+)(?:_AT[0-9.]+)?(?:\.[0-9])?_out(?:(?:put_)|\.)root$')
# # match AM systematic samples
# rexDidAntiMuon = re.compile(
#     r'^(user\.\w+)\.'     # scope
#     r'(data).*\.'         # data only
#     r'(?:sys[0-9._]*_)?'  # optional: AntiMuon systematics
#     r'(v[0-9a-z]+)'       # ntuple version string
#     # r'(?:_AT[0-9.]+)?'    # optional: AT version
#     r'(?:\.[0-9])?'       # optional: recreated sample version
#     r'_out(?:(?:put_)|\.)root$')  # root outputfile suffix
machine_data = {}  # save disk space and dids per machine
dids = {}          # did: DidData pairs (from input files, default files)
dids_default = {}  # did: DidData pairs only from default files)
dids_input = {}    # did: DidData pairs only from input files
version_dids_ondisk = {}      # version: {did: DidData} from samples on disk, above dids get removed
did_sizes = {}
version = os.path.basename(os.path.dirname(this_file))
# version = os.path.basename(os.getcwd())
dids_new = []
incomplete_samples = []

# print('WARNING: turned off datv space check because /mnt/datv3 was missing. Turn on again later!!')


# ------- functions -------------------------------------------------------------------- #
# function to get which dids are already on disk
def ReadDIDsFromDisk(version):
    if DEBUG:
        print('  Reading existing DIDs from disk for version mc16_{}...'.format(version))
    disk_dids = {}
    for machine in machines:
        for sample_type in ['MC', 'DATA', 'AntiMuon', 'JetLepton']:
            if sample_type in ['MC', 'DATA']:
                mcdata = sample_type
                path = '/mnt/%s%d/atlas/SgTopD3PD/mc16_%s/%s' % (machine_location, machine, version, mcdata)
            else:
                qcdtype = sample_type
                path = '/mnt/%s%d/atlas/SgTop_Fake/%s_%s' % (machine_location, machine, qcdtype, version)
            if not os.path.exists(path):
                continue
            if DEBUG:
                print('    reading %s' % path)
            dirs = os.listdir(path)
            for did in dirs:
                if not os.path.isdir(os.path.join(path, did)):
                    if DEBUG:
                        print('    did %s is not a directory' % did)
                    continue
                if rexDid.match(did) is None:
                    if DEBUG:
                        print('    did %s has an invalid name' % did)
                    continue
                if DEBUG > 1:
                    print('      Adding did %s' % did)
                disk_dids[did] = DidData(did, machine)
    return disk_dids


def calculate_free_space():
    print('Calculating free space on cluster...')
    total_free_size = 0
    for machine in machines:
        stm = os.statvfs('/mnt/datm%d' % machine)
        free_size_m = stm.f_bavail * stm.f_frsize
        sts = os.statvfs('/mnt/dats%d' % machine)
        free_size_s = sts.f_bavail * sts.f_frsize
        try:
            stv = os.statvfs('/mnt/datv%d' % machine)
            free_size_v = stv.f_bavail * stv.f_frsize
        except OSError as e:
            if e.errno != 2:
                raise
            else:
                print('warning: could not stat /mnt/datv%d' % machine)
                free_size_v = -1.
        # free_size_v = 0
        #    print free_size_m // 1024 //1024 // 1024 , free_size_s // 1024 //1024 // 1024
        free_size = None
        if (ignore_cluster):
            if machine_location == 'datm':
                free_size = free_size_m
            elif machine_location == 'dats':
                free_size = free_size_s
            elif machine_location == 'datv':
                free_size = free_size_v
        else:
            free_size = min([free_size_m, free_size_s, free_size_v])
        # st = os.statvfs('/mnt/%s%d' % (machine_location, machine))
        # free_size = st.f_bavail * st.f_frsize
        total_free_size += free_size
        if ignore_cluster:
            machinefile = os.path.join(os.path.dirname(this_file), '%s%d.txt' % (machine_location, machine))
        else:
            machinefile = os.path.join(os.path.dirname(this_file), 'dats%d.txt' % machine)
        machine_data[machine] = {
            'share': 0, 'allocated': 0, 'free': free_size, 'new_dids': [],
            'alloc_dids': [], 'alloc_file': machinefile, 'downloaded': 0}  # 'block_size': st.f_frsize,
        machine_data[machine]
    return total_free_size


def read_dids_from_file(filename, dict_=None, machine=None, DEBUG=0):
    """Reads DIDs from FILENAME and adds {did: DidData} pairs to DICT_."""
    if dict_ is None:
        dict_ = {}
    n_entries = 0
    n_doubles = 0
    try:
        with open(filename, 'rb') as f:
            for line in f:
                did = line.strip()
                if did.startswith('#') or line.isspace():
                    continue
                n_entries += 1
                if did in dict_.keys():
                    n_doubles += 1
                    if DEBUG > 0:
                        print('  Double DID: {}'.format(did))
                    if dict_[did].machine is None:
                        # machineless did should be replaced
                        dict_[did] = DidData(did, machine)
                else:
                    # new did should be added
                    dict_[did] = DidData(did, machine)
    except IOError as e:
        if e.errno != errno.ENOENT:
            raise
        if DEBUG > 0:
            print('  No such file {}'.format(filename))
    if DEBUG > 0:
        print('  Read {} entries from file {}, with {} double entries'.format(
            n_entries, filename, n_doubles))
    return dict_


def read_dids_from_disk():
    print('Reading existing DIDs from disk...')
    dids_downloaded = {}
    n_downloaded = 0
    for did, data in sorted(dids.items()):
        if data.version not in dids_downloaded.keys():
            dids_downloaded[data.version] = ReadDIDsFromDisk(data.version)
            n_downloaded += len(dids_downloaded[data.version])
    # version_dids_ondisk = dids_downloaded.copy()
    print('  Found %d DIDs on disk' % n_downloaded)
    return dids_downloaded


def check_download_status():
    print('Checking status of samples on disk...')
    total_download_size, total_downloaded_size = 0, 0
    for did, data in sorted(dids.items()):
        m = rexScope.match(did)
        did_size = 0
        downloaded_size = 0
        path = data.getPath()
        if path is not None:
            try:
                os.chdir(path)
            except OSError as e:
                if e.errno != 2:  # No such file or directory
                    raise
                path = None
        n_files = 0
        try:
            for file_ in rucio_client.list_files(scope=m.group(1), name=did):
                did_size += int(file_['bytes'])
                if path is not None:
                    try:
                        downloaded_size += os.path.getsize(file_['name'])
                    except OSError as e:
                        if e.errno != 2:  # No such file or directory
                            raise
                n_files += 1
        except rucio.common.exception.DataIdentifierNotFound as e:
            print(e)
        if n_files == 0:
            print('  Warning: rucio did not return any file for did {}'.format(did))
            if path is not None:
                print('    path for did is {}, remove manually'.format(path))
        total_download_size += did_size
        total_downloaded_size += downloaded_size
        if data.new:
            dids_new.append((did, did_size))
        else:
            machine_data[data.machine]['allocated'] += max(0, did_size - downloaded_size)
            machine_data[data.machine]['downloaded'] += downloaded_size
            if did_size != downloaded_size:
                incomplete_samples.append(did)
                if DEBUG > 0:
                    print('    Incomplete sample on machine %d (%4d/%4d GB): %s ' %
                          (data.machine, downloaded_size // 1024**3, did_size // 1024**3, did))
            if downloaded_size > 0:
                version_dids_ondisk[data.version].pop(did)
            # print('did {}, dl size {}, # old dids {}'.format(
            #     did, downloaded_size, len(version_dids_ondisk[data.version])))
            # print('  path {}, did_size {}'.format(path, did_size))
    if DEBUG and len(incomplete_samples) > 0:
        print('  Found {}/{} incomplete samples'.format(
            len(incomplete_samples), len(dids)))
    dids_new.sort(key=operator.itemgetter(1), reverse=True)
    if DEBUG and len(dids_new) > 0:
        print('  Found {}/{} new samples (i.e. in input files '
              'but not assigned to a disk yet)'.format(len(dids_new), len(dids)))
    n_old_samples = sum([len(x) for x in version_dids_ondisk.values()])
    if DEBUG and n_old_samples > 0:
        print('  Found {} old samples (i.e. on disk '
              'but not in input files)'.format(n_old_samples))
    return total_download_size, total_downloaded_size


def allocate_samples(did_list):
    print('Allocating new samples to disks...')
    for did, did_size in did_list:
        scores = []
        for machine in machines:
            free = machine_data[machine]['free']
            alloc = machine_data[machine]['allocated']
            share = machine_data[machine]['share']
            exppart = math.exp(- (free - alloc - share - did_size) / total_download_size) * 8.
            # exppart = math.exp(- machine_data[machine]['free'] / total_download_size / 8.)
            # exppart = math.exp(- machine_data[machine]['free'] + machine_data[machine]['share'] / 8.)
            sharepart = 8.*(share + alloc) / total_download_size
            didpart = 8. * did_size / total_download_size
            scores.append([machine, exppart + (sharepart + didpart), exppart, sharepart, didpart])
            #        print machine, machine_data[machine], exppart, sharepart, didpart, exppart+sharepart+didpart
            #    scores = [ [machine, exppart + sharepart + didpart, exppart,
            #                sharepart, didpart] for machine in machines ]
        scores.sort(key=operator.itemgetter(1))
        #   print did, scores
        machine_data[scores[0][0]]['share'] += did_size
        dids[did].machine = scores[0][0]
        #    pdb.set_trace()


def download_samples():
    # version='mc16_v20_lj'
    machine = int(os.getenv('HOST')[3])
    # sample_list = machine_data[machine]['alloc_file']
    # os.path.join(os.getenv('HOME'), 'atlas/download', version, 'dats%d.txt' % machine)
    min_free_space = 200
    free = machine_data[machine]['free'] // gb
    alloc = machine_data[machine]['allocated'] // gb
    share = machine_data[machine]['share'] // gb
    if (free - alloc - share < min_free_space):
        raise IOError('Not enough free space on disk (%d GB free, %d GB needed)' % (
            free, alloc+share+min_free_space))
    for did in machine_data[machine]['alloc_dids'] + machine_data[machine]['new_dids']:
        if did not in incomplete_samples:
            # skip complete samples to speed up download
            if DEBUG:
                print('  Skipping complete DID {}'.format(did))
            continue
        m = rexScope.match(did)
        data = DidData(did, machine)
        path = os.path.dirname(data.getPath())
        try:
            os.chdir(path)
        except OSError as e:
            if e.errno != 2:
                raise
            os.makedirs(path)
            os.chdir(path)
        # pdb.set_trace()
        if DEBUG:
            print('  Downloading DID {} into {}'.format(did, os.getcwd()))
        rcmd = 'rucio download %s:%s' % (m.group(1), did)
        print(rcmd)
        os.system(rcmd)
        # rucio_client.get_did(scope=m.group(1), name=did)


# ------- classes ---------------------------------------------------------------------- #
# DidData object definition
class DidData(object):
    def __init__(self, did, machine=None):
        self.did = did
        self.machine = machine
        self.new = (machine is None)
        # check for QCD samples; beware of syntax changes
        if rexAntiMuon.match(did) is not None:
            self.qcdtype = 'AntiMuon'
        elif rexJetLepton.match(did) is not None:
            self.qcdtype = 'JetLepton'
        else:
            self.qcdtype = 'datamc'
        # JetLepton and AntiMuon can have a different did structure
        rex = None
        if self.qcdtype == 'JetLepton':
            rex = rexDidJetLepton
        elif self.qcdtype == 'AntiMuon':
            rex = rexDidAntiMuon
        else:
            rex = rexDidDataMC
        m = rex.match(did)
        if m is None:
            raise ValueError('unrecognised DID "%s"' % did)
        try:
            self.scope, self.mcdata, self.version = m.groups()
        except ValueError as e:
            print('DID: {}, regex: {}'.format(did, rex.pattern))
            raise
        # AntiMuon has no lj in the did
        if (m.group(0).find("lj.%s" % self.version) != -1
                or self.qcdtype == 'AntiMuon'):
            self.version += "_lj"
        # pdb.set_trace()
        # if m.group(0).contains

    def getPath(self):
        if self.machine is None:
            return None
        if self.qcdtype == 'datamc':
            path = '/mnt/{}{}/atlas/SgTopD3PD/mc16_{}/{}/{}'.format(
                machine_location, self.machine, self.version, self.mcdata.upper(), self.did)
        elif self.qcdtype in ['AntiMuon', 'JetLepton']:
            path = '/mnt/{}{}/atlas/SgTop_Fake/{}_{}/{}'.format(
                machine_location, self.machine, self.qcdtype, self.version, self.did)
        return path


# ------- main program ----------------------------------------------------------------- #
if __name__ == '__main__':

    # some defaults
    n_entries = 0
    n_doubles = 0

    # parse cmd line arguments
    args = parser.parse_args()
    machine_location = args.machine_location
    ignore_cluster = args.ignore_cluster
    calc_new = args.force_recalc
    download = args.download
    DEBUG = args.verbose
    skip_samples_default_only = args.skip_default
    files = args.files
    delete_old = args.delete_old
    remove_old = args.remove_old
    print_location = args.print_location
    if machine_location is None:
        cluster = os.getenv('HOST')[:3]
        if cluster == 'as0':
            machine_location = 'dats'
        elif cluster == 'ms0':
            machine_location = 'datm'
        elif cluster == 'as1':
            machine_location = 'datv'
    if DEBUG > 0:
        print('  Using debug level {}'.format(DEBUG))
        print('    Machine location is {}'.format(machine_location))
        if ignore_cluster:
            print('    Ignoring disk space requirements on other cluster')
        if calc_new:
            print('    Forcing recalculation of additional samples')
        if download:
            print('    Enabling download of samples')
        if skip_samples_default_only:
            print('  Skipping samples that are only defined in default files')
        if delete_old:
            print('  Deleting old samples that are on disk but not in sample list')

    # process cmd line files (optional)
    for filename in files:
        print('Reading entries from file {}'.format(filename))
        read_dids_from_file(filename, dids, DEBUG=DEBUG)
    dids_input = dids.copy()

    # calculate free space and fill machine dids from default files
    total_free_size = calculate_free_space()

    # read DIDs from default files
    print('Reading DIDs from default files...')
    machine_dids = {}
    n_only_default = 0
    n_entries_pre = len(dids)
    for machine in machines:
        machinefile = machine_data[machine]['alloc_file']
        # fill dids_default without debug notes
        read_dids_from_file(machinefile, dids_default, machine)
        if skip_samples_default_only:
            # create machine_dids to have a disk-to-did info
            machine_dids[machinefile] = read_dids_from_file(
                machinefile, machine=machine, DEBUG=DEBUG)
            # update dids with machine if possible, or
            # print out machine_dids which are not in dids
            for did in sorted(machine_dids[machinefile]):
                if did in dids.keys():
                    dids[did] = DidData(did, machine)
                else:
                    print('    {}: default-only DID {} '.format(
                        os.path.basename(machinefile), did))
                    n_only_default += 1
        else:
            # read default files again for dids, with debug
            read_dids_from_file(machinefile, dids, machine, DEBUG=DEBUG)
    if skip_samples_default_only:
        print('  Found {}/{} dids which are only in default files'.format(
            n_only_default, len(dids_default)))
    else:
        # number of doubles is #dids_default - #new_dids
        n_new = len(dids) - n_entries_pre
        n_doubles = len(dids_default) - n_new
        print('  Found {}/{} double DIDs in default files'.format(
            n_doubles, len(dids_default)))

    # read dids that were already downloaded for the versions of the input dids
    version_dids_ondisk = read_dids_from_disk()
    #    print(version_dids_ondisk)
    # print(dids)

    if print_location:
        n_input = len(dids_input)
        if n_input == 0:
            print('Cannot print location: there were no explicit input samples')
        else:
            print('Location for {} input samples:'.format(n_input))
            for did in sorted(dids_input.keys()):
                path = dids[did].getPath()
                if path is not None:
                    print('  ', path)

    # check downloaded files
    total_download_size, total_downloaded_size = check_download_status()

    # useful score:
    # - take lowest score
    # - average of 2 values: 1. still free size -> neg. exp, 2. part of total occupied on this machine
    if (calc_new and len(dids_new) > 0):
        # allocate new samples to disks
        allocate_samples(dids_new)
    elif len(dids_new) > 0:
        # take new samples from *_new.txt files
        for machine in machines:
            machinefile = os.path.join(os.path.dirname(this_file), '%s%d_new.txt' % (machine_location, machine))
            if not os.path.exists(machinefile):
                continue
            machine_data[machine]['new_file'] = machinefile
            dids_new_d = dict(dids_new)
            # print dids_new_d
            # print machinefile
            with open(machinefile) as f:
                for line in f:
                    did = line.strip()
                    if did.startswith('#') or line.isspace():
                        continue
                    # print did
                    did = did.strip()
                    machine_data[machine]['share'] += dids_new_d[did]
                    dids[did].machine = machine

    # print out disk space allocation
    gb = 1024*1024*1024
    # print('display space ' % machine_location)
    print('Disk space overview:')
    print('  total free size (min): %5f' % (total_free_size / gb))
    print('  total download size:   %5f' % (total_download_size / gb))
    print('  total downloaded size: %5f' % (total_downloaded_size / gb))
    print('machine\t  free\t alloc\t  done\t share\t f-a-s')
    for machine in machines:
        free = machine_data[machine]['free'] // gb
        alloc = machine_data[machine]['allocated'] // gb
        share = machine_data[machine]['share'] // gb
        downloaded = machine_data[machine]['downloaded'] // gb
        print('   {:d} \t {:5d} \t {:5d} \t {:5d} \t {:5d} \t {:5d}'.format(
            machine, free, alloc, downloaded, share, free-alloc-share))

    # classify dids as new or allocated
    for did, data in sorted(dids.items()):
        # pdb.set_trace()
        if data.new:
            if data.machine is None:
                print('Unknown sample %s. Need to run with -f first or add the sample manually' % did)
            else:
                machine_data[data.machine]['new_dids'].append(did)
        else:
            machine_data[data.machine]['alloc_dids'].append(did)

    # write out _new files
    for machine in machines:
        machinefile = os.path.join(os.path.dirname(this_file), '%s%d_new.txt' % (machine_location, machine))
        machine_data[machine]['new_file'] = machinefile
        if calc_new:
            with open(machinefile, 'w') as f:
                for did in sorted(machine_data[machine]['new_dids']):
                    f.write('%s\n' % did)

    # write out _replace files to remove default-only dids
    for machine in machines:
        machinefile = os.path.join(os.path.dirname(this_file),
                                   '{:s}{:d}.txt'.format(
                                       machine_location, machine))
        replacefile = os.path.join(os.path.dirname(this_file),
                                   '{:s}{:d}_replace.txt'.format(
                                       machine_location, machine))
        machine_data[machine]['replace_file'] = replacefile
        if remove_old:
            with open(replacefile, 'w') as f:
                for did in sorted(machine_dids[machinefile].keys()):
                    if did in dids.keys():
                        f.write('{:s}\n'.format(did))

    # find dids that are on disk but not allocated (=old)
    n_old_left = 0
    n_old_deleted = 0
    for ver in version_dids_ondisk:
        if len(version_dids_ondisk[ver]) > 0:
            if DEBUG:
                action = 'deleting' if delete_old else 'listing'
                print('Old DIDs for version mc16_{} ({}):'.format(
                    ver, action))
            for did, data in sorted(version_dids_ondisk[ver].items()):
                if DEBUG:
                    print('  %s' % data.getPath())
                if delete_old:
                    shutil.rmtree(data.getPath())
                    n_old_deleted += 1
                else:
                    n_old_left += 1
    if n_old_left > 0:
        print('%d old DIDs left on disk, use --delete-old to '
              'delete old DIDs' % n_old_left)
    if n_old_deleted > 0:
        print('%d old DIDs deleted' % n_old_deleted)
    # delete downloads?

    # Download samples
    if download:
        download_samples()
