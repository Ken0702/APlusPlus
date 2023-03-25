#!/usr/bin/env python
#
#
"""Script to manually compute the sum of weights information and save to a file.

Usage: python extract_sumweights_information_manual.py INFILE OUTFILE

Necessary arguments:
- INFILE: a SampleList file created via GenerateSampleList.sh
- OUTFILE: target file where to save the sumWeights information

Output syntax:
- some comment lines starting with #, they should be skipped when processing
- otherwise, each line contains:
<sample> <weight number> <weight name> <totalEventsWeighted> <total event weights manual> <manual/sumWeights total>

Manual here means that instead of looking into the sumWeights tree, we
take the sum of weights by summing over the complete nominal_Loose
tree. This workaround is necessary because of a bug with huge negative
events and sumWeights in wjets samples. To avoid this in the manual
recalculation, all weights with |w|>100 are set to 1.

Also, since the input samples have been skimmed, we multiply by
sumWeights[0]/sum(nominal_Loose[0]) to get the correct normalisation.

Example usage:
python $APP_HOME/libAtlasAnalysis/utils/extract_sumweights_information_manual.py $APP_HOME/libSingleTop/tasks/13TeV/SampleList/SampleList_vs_mc16_v29_lj_SgTopDSIDs_mc16e_v29_lj.txt $APP_HOME/libSingleTop/tasks/13TeV/SampleList/sumWeights_info_manual_SampleList_vs_mc16_v29_lj_SgTopDSIDs_mc16e_v29_lj.txt"""
from __future__ import print_function

import datetime
import numpy as np
import os
import pdb
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
        f.write('# sample  <weight number>  <weight name>  <manual sum of weights> <factor between manual and sumWeights> <n high events>\n')

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
    all_totals_manual = {}
    all_totals_workaround = {}
    samples = sorted(locations.keys())
    
    for sample in samples:
        # only MC samples
        if sample.startswith('data') or sample.startswith('QCD'):
            continue
        print('#   Checking sample {}...'.format(sample))
        wnames = []  # note: will be byte strings
        totals = None
        totals_manual = None
        totals_workaround = None
        root_dir = locations[sample]
        dsid = ''
        totals_n_high_weights = None
        debug_n_high_weights = 0
        debug_n_total_weights = 0
        print('#     Found {} input files'.format(len(os.listdir(root_dir))))
        for rootfile in os.listdir(root_dir):
            # pdb.set_trace()
            # don't skip any, there are only real root files in here
            # now do uproot magic, test first
            rootfile_full = os.path.join(root_dir, rootfile)
            f = uproot.open(rootfile_full)
            # read some branches from sumWeights tree
            b_sumWeights = f['sumWeights']
            b_names = b_sumWeights['names_mc_generator_weights']
            b_totEW = b_sumWeights['totalEventsWeighted']
            b_totEW_mcgw = b_sumWeights['totalEventsWeighted_mc_generator_weights']
            b_dsid = b_sumWeights['dsid']
            # read from nominal_Loose tree
            b_nominalLoose = f['nominal_Loose']
            b_mcgw = b_nominalLoose['mc_generator_weights']
            # convert to numpy arrays
            a_names = np.stack(b_names.array())            # np.ndarray (nfiles, nsyst)
            a_totEW = b_totEW.array()                      # np.ndarray (nfiles,)
            a_totEW_mcgw = np.stack(b_totEW_mcgw.array())  # np.ndarray (nfiles, nsyst)
            a_dsid = b_dsid.array()                        # np.ndarray (nfiles,)
            a_mcgw = np.stack(b_mcgw.array())              # np.ndarray (nevts, nsyst)
            # loop/numpy evaluate entries of sumWeights
            # - check consistent did
            if not dsid:
                dsid = a_dsid[0]
            if not all(a_dsid == dsid):
                print('inconsistent dsids! expected: {}, found: {}'.format(
                    dsid, a_dsid))
            # - check consistent weight names
            if len(wnames) == 0:
                wnames = a_names[0]
            if not all([all(a_names[i] == wnames) for i in range(len(a_names))]):
                print('inconsistent wnames! check file {}'.format(rootfile_full))
            # - sum totals, check equivalence of first
            if not np.allclose(a_totEW, a_totEW_mcgw[:, 0]):
                print('nominal and weight 0 totalEventsWeighted are not equal!'
                      ' difference is {} in file {}'.format(
                          a_totEW - a_totEW_mcgw[:, 0], rootfile_full))
            # set high weights to 1
            # - count high weights and total weights
            n_high_weights = np.count_nonzero(np.abs(a_mcgw) > 100, axis=0)
            n_total_weights = np.count_nonzero(a_mcgw, axis=0)
            debug_n_high_weights += np.sum(n_high_weights)
            debug_n_total_weights += np.sum(n_total_weights)
            # - set high weights to 1
            a_mcgw_corrected = a_mcgw.copy()
            a_mcgw_corrected[np.abs(a_mcgw) >= 100] = 1
            # sum over all events or input files (result is n_syst long array)
            mc_total_sumWeights = np.sum(a_totEW_mcgw, axis=0)
            mc_total_manual = np.sum(a_mcgw, axis=0)
            mc_total_manual_corrected = np.sum(a_mcgw_corrected, axis=0)
            # use workaround because of skimming: multiply with nominal/nominal(manual)
            mc_total_manual_workaround = mc_total_manual_corrected*mc_total_sumWeights[0]/mc_total_manual_corrected[0]
            if totals is None:
                totals = mc_total_sumWeights
                totals_manual = mc_total_manual
                totals_workaround = mc_total_manual_workaround
                totals_n_high_weights = n_high_weights
            else:
                totals += mc_total_sumWeights
                totals_manual += mc_total_manual
                totals_workaround += mc_total_manual_workaround
                totals_n_high_weights = n_high_weights

        # debug print statements
        if debug_n_high_weights > 0:
            print('#     Found {}/{}={:.3f}% high weights with |w|>100'.format(
                debug_n_high_weights, debug_n_total_weights,
                100.*debug_n_high_weights/debug_n_total_weights))

        # save in sample dicts
        # decode bytestrings
        all_wnames[sample] = [bs.decode('utf-8') for bs in wnames]
        all_totals[sample] = totals.tolist()
        all_totals_manual[sample] = totals_manual.tolist()
        all_totals_workaround[sample] = totals_workaround.tolist()

        # print to outf
        with open(outf, 'a') as f:
            wnames = all_wnames[sample]
            totals = all_totals[sample]
            for wnum in range(len(wnames)):
                f.write('{:s} {:3d} {:s} {:f} {:f} {:d}\n'.format(
                    sample, wnum, wnames[wnum],
                    totals_workaround[wnum],
                    totals_workaround[wnum]/totals[wnum],
                    totals_n_high_weights[wnum]))
