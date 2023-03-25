#!/usr/bin/env python
#
import pdb
import re
import sys

if __name__ == '__main__':
    outf = sys.argv[1]
    machine = sys.argv[2]
    ntup_ver = sys.argv[3]
    inf = sys.argv[4]
    xsecf = sys.argv[5]
    camp = sys.argv[6]
    try:
        didlistf = sys.argv[7]
    except IndexError:
        didlistf = '/users/eeh/kaphle/atlas/download/mc16_v25lj/sample_syst_DIDs.txt'
    called_command = ' '.join(sys.argv)

    print('\nGenerating sample list {}\n'.format(outf))

    # didlistf = '/users/eeh/kaphle/atlas/download/mc16_v25lj/current_list_2019-02-14.txt'

    # detector sim regex
    regex_af = re.compile(r"^.*_a[0-9]+_.*$")
    regex_fs = re.compile(r"^.*_s[0-9]+_.*$")

    dslist = []
    with open(inf, 'r') as f:
        for line in f:
            if line.startswith('#') or line.isspace():
                continue
            dslist.append(line)

    with open(outf, 'w') as fo, open(inf) as fi, open(xsecf) as fx:
        fo.write('#\n')
        fo.write('#  This file was automatically generated with GenerateSampleList_grid.py\n')
        fo.write('#    via "{}"\n'.format(called_command))
        fo.write('#\n\n')
        for sample_line in fi:
            if sample_line.startswith('#') or sample_line.isspace():
                continue
            words = sample_line.split(',')
            SampleShort, SampleType, DSID, Color = words[:4]
            if len(words) > 4:
                SimType = words[4]
            reco = ""
            if DSID.find('*') != -1:
                reco = DSID
                DSID = DSID.split('*')[0]
            append = 0
            if (SampleType.startswith('kDATA') or SampleType.startswith('kQCDMatrix') or
                SampleType.startswith('kQCD_AntiMu')):
                with open(didlistf) as fdid:
                    for line in fdid:
                        if re.match(r'^user.*\..*{}.*'.format(DSID), line) is None:
                            continue
                        if append > 0:
                            fo.write(' + + + + {}\n'.format(line))
                        else:
                            fo.write('{} {} + {} {}'.format(SampleShort, SampleType, Color, line))
                            append += 1
            elif SampleType.startswith('kQCDJetLepton'):
                with open(didlistf) as fdid:
                    for line in fdid:
                        if re.match(r'^user.*\..*{}.*'.format(reco), line) is None:
                            continue
                        if append > 0:
                            fo.write(' + + + + {}\n'.format(line))
                        else:
                            fo.write('{} {} + {} {}'.format(SampleShort, SampleType, Color, line))
                            append += 1
            else:
                Xsection = 0
                with open(xsecf) as fx:
                    for line in fx:
                        if line.startswith('#') or line.isspace():
                            continue
                        words = line.split()
                        did = words[0]
                        xs = float(words[1])
                        kfac = float(words[2])
                        # did, xs, kfac, gen = line.split()
                        if did != DSID:
                            continue
                        # print(line,did, DSID)
                        Xsection = xs*kfac
                if not Xsection:
                    print('ERROR: cross section of dataset with ID {} not known, skipping.'.format(DSID))
                else:
                    with open(didlistf) as fdid:
                        # print(' ', DSID)
                        for line in fdid:
                            if re.match(r'^user.*\.mc.*TeV\.{}.*'.format(DSID), line) is None:
                                continue
                            # print('regex for MC matched')
                            if (SimType.find('kFullSim') != -1 and regex_af.match(line) is not None or
                                SimType.find('kAtlFast') != -1 and regex_fs.match(line) is not None or
                                re.match(r'.*{}.*'.format('.*'.join(reco.split('*'))), line) is None):
                                # pdb.set_trace()
                                continue
                            # print('no AF2/FS or reco tag exclusion')
                            if append > 0:
                                fo.write(' + + + + {}\n'.format(line))
                            else:
                                fo.write('{} {} {} {} {}'.format(SampleShort, SampleType, Xsection, Color, line))
                                append += 1
