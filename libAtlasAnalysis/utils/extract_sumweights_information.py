#!/usr/bin/env python
#
#
"""Script to extract the sumWeights tree information and save to a file.

Usage: python extract_sumweights_information.py INFILE OUTFILE

Necessary arguments:
- INFILE: a SampleList file created via GenerateSampleList.sh
- OUTFILE: target file where to save the sumWeights information

Output syntax:
- some comment lines starting with #, they should be skipped when processing
- otherwise, each line contains:
<sample> <weight number> <weight name> <totalEventsWeighted>

Example usage:
python $APP_HOME/libAtlasAnalysis/utils/extract_sumweights_information.py $APP_HOME/libSingleTop/tasks/13TeV/SampleList/SampleList_vs_mc16_v29_lj_SgTopDSIDs_mc16e_v29_lj.txt $APP_HOME/libSingleTop/tasks/13TeV/SampleList/sumWeights_info_SampleList_vs_mc16_v29_lj_SgTopDSIDs_mc16e_v29_lj.txt"""
from __future__ import print_function

import datetime
import numpy as np
import os
# import pdb
import sys
import uproot

if __name__ == '__main__':
    usage = __doc__.split('\n')[2]
    if len(sys.argv) == 2 and sys.argv[1] in ['help', '-h', '--help']:
        print(__doc__)
        exit(0)
    elif len(sys.argv) < 3:
        print('Not enough arguments!')
        print(usage)
        sys.exit(1)
    inf = sys.argv[1]
    outf = sys.argv[2]
    print('# Running {} on {}'.format(
        ' '.join(sys.argv), datetime.datetime.now().strftime('%FT%T')))

    # print to outf header
    with open(outf, 'w') as f:
        f.write('# Created with {} on {}\n'.format(
            ' '.join(sys.argv), datetime.datetime.now().strftime('%FT%T')))
        f.write('#\n')
        f.write('# sample  weight number  weight name  totalEventsWeighted\n')

    # run over infile and make dict of samples location
    locations = {}
    with open(inf) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            sample, _, xsec, colour, loc = line.split()
            if sample in locations:
                print('warning: sample {} is listed twice, skip duplicate'.format(sample))
                continue
            locations[sample] = loc

    # loop over samples, read in sumweights information from root files
    # all_wnames: dict of {sample: list(wnames), ...}
    # all_totals: dict of {sample: list(totals), ...}
    all_wnames = {}
    all_totals = {}
    samples = sorted(locations.keys())
    for sample in samples:
        # only MC samples
        if sample.startswith('data') or sample.startswith('QCD'):
            continue
        print('#   Checking sample {}...'.format(sample))
        wnames = []  # note: will be byte strings
        totals = None
        root_dir = locations[sample]
        dsid = ''
        for rootfile in os.listdir(root_dir):
            # don't skip any, there are only real root files in here
            # now do uproot magic, test first
            rootfile_full = os.path.join(root_dir, rootfile)
            f = uproot.open(rootfile_full)
            # read some branches
            b_sw = f['sumWeights']
            b_n = b_sw['names_mc_generator_weights']
            b_tot = b_sw['totalEventsWeighted']
            b_tot_mc = b_sw['totalEventsWeighted_mc_generator_weights']
            b_d = b_sw['dsid']
            # convert to numpy arrays
            a_n = b_n.array()
            a_tot = b_tot.array()
            a_tot_mc = b_tot_mc.array()
            a_d = b_d.array()
            # loop/numpy evaluate entries of sumWeights
            # - check consistent did
            if not dsid:
                dsid = a_d[0]
            if not all(a_d == dsid):
                print('inconsistent dsids! expected: {}, found: {}'.format(
                    dsid, a_d))
            # - check consistent weight names
            if not wnames:
                wnames = a_n[0]
            #print([a_n[i] == wnames for i in range(len(a_n))])
            if not all([a_n[i] == wnames for i in range(len(a_n))]):
                print('inconsistent wnames! check file {}'.format(rootfile_full))
            # - sum totals, check equivalence of first
            if not np.allclose(a_tot, a_tot_mc[:, 0]):
                print('nominal and weight 0 totalEventsWeighted are not equal!'
                      ' difference is {} in file {}'.format(
                          a_tot - a_tot_mc[:, 0], rootfile_full))
            if totals is None:
                totals = np.sum(np.array(a_tot_mc))
            else:
                totals += np.sum(np.array(a_tot_mc))
            for i in range(0,len(a_tot_mc)):
                test = ((np.array(a_tot_mc))[i])[6]
                #print(test)
                #print(i)
                #print(rootfile)
            #print(len(test))
        # save in sample dicts
        # decode bytestrings
        all_wnames[sample] = [bs.decode('utf-8') for bs in wnames]
        all_totals[sample] = totals.tolist()
        #print("HEHE")
        # print to outf
        with open(outf, 'a') as f:
            wnames = all_wnames[sample]
            totals = all_totals[sample]
            print(len(wnames))
            for wnum in range(len(wnames)):
                #print("HHALL")
                f.write('{:s} {:3d} {:s} {:f}\n'.format(
                    sample, wnum, wnames[wnum], totals[wnum]))
