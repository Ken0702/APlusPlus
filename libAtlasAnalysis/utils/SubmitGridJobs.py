#!/usr/bin/env python
#
# Submit grid jobs from subdirectories
#
"""Submit grid jobs from subdirectories.

Before executing this file, source setup-grid-2.sh, initialize voms proxy
and check the entries from systematics.txt and samples.txt.
"""

from __future__ import print_function

import os
import shutil
import subprocess
import sys
import tarfile
import pdb
from io import BytesIO

# options
add_external = True      # add external libraries to tar ball
use_prun     = True      # execute the prun command
do_submit    = True     # otherwise use prun --noSubmit
test_one     = False      # just run one sample for testing
unpack_tarballs = False
cleanup      = False
# broken_suffix = '_v1'  # use for broken task rerunning; leave empty for normal

# # user submission script:

# save current dir
sav_pwd = os.getcwd()

# top directory: execute from top, so top=sav
top_dir = sav_pwd

# get systematics and sample list from files
if len(sys.argv) > 1:
    syst_file = sys.argv[1]
else:
    syst_file = 'systematics.txt'
systs = []
nsyst = 0
with open(syst_file) as f:
    for line in f:
        if line.startswith('#'):
            continue
        systs.append(line.strip())
        nsyst += 1
print('\nFound {0} systematics'.format(nsyst))
sample_file = 'samples.txt'
samples = []
nsamples = 0
with open(sample_file) as f:
    for line in f:
        if line.startswith('#'):
            continue
        samples.append(line.strip())
        nsamples += 1
print('\nFound {0} samples per systematic'.format(nsamples))

# setup athena, panda, rucio (maybe with my setup script?)
# -> execute setup script, and possibly voms-proxy-init

# tar balls for external libraries
app_basename = 'APlusPlus'
mem_basename = 'libMEM'
app_ext_tgz = os.path.join(top_dir, 'APlusPlus.tgz')
mem_ext_tgz = os.path.join(top_dir, 'libMEM.tgz')
prefix = 'ntup_schan_2j_lnu'
unpack_dir = os.path.join(top_dir, 'unpack')
app_unpacked = os.path.join(unpack_dir, app_basename)
mem_unpacked = os.path.join(unpack_dir, mem_basename)

force_recreate = False
if unpack_tarballs:
    exists = False
    try:
        os.makedirs(unpack_dir)
    except OSError as e:
        if e.errno != 17:
            raise
        exists = True
    if force_recreate:
        shutil.rmtree(unpack_dir)
        os.makedirs(unpack_dir)
    if not exists or force_recreate:
        print('Unpacking {0} in {1}'.format(app_ext_tgz, unpack_dir))
        with tarfile.open(app_ext_tgz, 'r:gz') as tfa:
            tfa.extractall(unpack_dir)
        print('Unpacking {0} in {1}'.format(mem_ext_tgz, unpack_dir))
        with tarfile.open(mem_ext_tgz, 'r:gz') as tfm:
            tfm.extractall(unpack_dir)
    # shutil.copy(app_ext_tgz, unpack_dir)
    # shutil.copy(mem_ext_tgz, unpack_dir)

# get username
user = os.getenv('RUCIO_ACCOUNT')
if user is None:
    raise ValueError('RUCIO_ACCOUNT unknown, setup rucio before running this script')

# output file to store the current state in
state_file = os.path.join(top_dir, 'state.txt')
if not os.path.exists(state_file):
    with open(state_file, 'w') as sf:
        sf.write('# save the current state for each outDS\n')
# read once for faster access
d_state = {}
with open(state_file) as sf:
    for line in sf:
        if line.startswith('#'):
            continue
        outDS, state = line.split()
        d_state[outDS] = state

# for each syst, for each sample
for syst in systs:
    print('  Submission for systematic {0}'.format(syst))
    for sample in samples:
        # go to sample dir
        if syst != 'nominal' and (sample.find('data') != -1 or sample.find('QCD') != -1):
            continue
        print('    Submission for sample {0}'.format(sample))
        sample_dir = os.path.join(top_dir, syst, sample)
        os.chdir(sample_dir)

        # remove all enlarged archives
        if cleanup:
            tgz_basename = prefix + '_' + syst + '_' + sample + '.tgz'
            if os.path.exists(tgz_basename + '.bkp') and os.path.exists(tgz_basename):
                os.remove(tgz_basename)
                shutil.move(tgz_basename + '.bkp', tgz_basename)
                print('    Cleanup: removed full archive ' + tgz_basename)
            continue

        # take command from file
        if use_prun:
            cmd_file = os.path.join(sample_dir, 'commands.txt')
            with open(cmd_file) as f:
                prun_cmd = f.read().strip()
            # check if it was already successful
            outDS = prun_cmd[prun_cmd.find('--outDS')+8:prun_cmd.find('--', prun_cmd.find('--outDS')+1)].strip().strip("'")
            if outDS in d_state.keys() and d_state[outDS] == 'submitted':
                print('    prun job for outDS %s already submitted' % outDS)
                continue
            # replace name with current user name (provide before or take from env)
            prun_cmd = prun_cmd.replace('skaphle', user)
            if not do_submit:
                prun_cmd = prun_cmd + ' --noSubmit '
            prun_cmd = prun_cmd + ' --forceStaged '

        # pdb.set_trace()
        # add external libraries from top directory
        tgz_basename = prefix + '_' + syst + '_' + sample + '.tgz'
        new_tgz = 'new_' + tgz_basename
        if add_external and not os.path.exists(tgz_basename + '.bkp'):
            print('    Adding external libraries to tarball')

            # shutil.copy(app_ext_tgz, sample_dir)
            # shutil.copy(mem_ext_tgz, sample_dir)
            print('      Extracting ' + tgz_basename)
            with tarfile.open(tgz_basename, 'r:gz') as tf1:
                tf1.extractall()
                names = tf1.getnames()
            print('      Creating ' + new_tgz)
            with tarfile.open(new_tgz, 'w:gz') as tf2:
                for f in names:
                    tf2.add(f)
                    # try:
                    #     os.remove(f)
                    # except OSError as e:
                    #     if e.errno != 21: raise
                    #     shutil.rmtree(f)
                if unpack_tarballs:
                    tf2.add(app_unpacked, arcname=app_basename)
                    tf2.add(mem_unpacked, arcname=mem_basename)
                else:
                    tf2.add(app_ext_tgz, arcname=os.path.basename(app_ext_tgz))
                    tf2.add(mem_ext_tgz, arcname=os.path.basename(mem_ext_tgz))
            # os.remove(os.path.basename(app_ext_tgz))
            # os.remove(os.path.basename(mem_ext_tgz))
            shutil.move(tgz_basename, tgz_basename + '.bkp')
            shutil.move(new_tgz, tgz_basename)

        # if add_external and not os.path.exists(new_tgz+'_test'):
        #     print('    Adding external libraries to tarball fast')
        #     # alternative and faster extraction method
        #     with tarfile.open(tgz_basename, 'r:gz') as tf1, tarfile.open(new_tgz+'_test', 'w:gz') as tf3: #, tarfile.open(app_ext_tgz, 'r:gz') as tfa, tarfile.open(mem_ext_tgz, 'r:gz') as tfm:
        #         for m in tf1.members:#+tfa.members+tfm.members:
        #             buf = m.tobuf()
        #             if isinstance(buf, str):
        #                 buf = buf.encode("utf-8")
        #             fileobj = BytesIO(buf)
        #             tarinfo = tarfile.TarInfo(m.name)
        #             tarinfo.size = len(fileobj.getvalue())
        #             tf3.addfile(tarinfo, fileobj)

            # print('changed to dir: {0}'.format(os.getcwd()))

        if use_prun:
            # execute prun command
            print('    Executing prun command: "{0}"'.format(prun_cmd))
            try:
                subprocess.check_call(prun_cmd, shell=True)
            except subprocess.CalledProcessError as e:
                if e.returncode != 90:
                    raise
            # save successful state
            d_state[outDS] = 'submitted'
            with open(state_file, 'a') as sf:
                sf.write('{0} {1}\n'.format(outDS, 'submitted'))

        # for testing, just run the first
        if test_one:
            break
    if test_one:
        break
