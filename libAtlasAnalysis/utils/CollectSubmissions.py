#!/usr/bin/env python
#
# Collect submissions and place them in one single directory
"""Collect submission scripts from A++ output tree and add archives

GRID_HOME has to be set and point to a directory including APlusPlus.tgz,
libMEM.tgz, Cuba-3.0.tar.gz, and the output dir is created as a subdirectory.

Parameters
----------
Edit lines marked below.
"""
from __future__ import print_function

import os
import tarfile
import pdb
import shutil
import tempfile

from io import BytesIO

#
# BEGIN User variables
#

# input
prefix = 'ntup_schan_2j_lnu'
app_src_dir = '/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_signal_localNominalRun/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu'
mem_src_dir = '/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_signal_localNominalRun/AtlSgTop_sChannelAnalysis/MemTkAnalysis/analysis/2Jets/lnu'

# output
target_subdir = 'submit/v25lj_default_d/v3'
outDS_suffix = ''

# other options
unpack_tarballs = False
test_one = False

#
# END User variables
#


# pack later before submission?
# when to pack A++ and libMEM?
# - option 1: directly put in every directory from here, with --extFile
# - option 2: same, but pack inside tgz file
# - option 3: keep in root directory, but link/copy later -> this
# pack_now = True

# temp path for tar operations
grid_home = os.getenv('GRID_HOME')
if grid_home is None:
    raise OSError('Need to set up GRID_HOME first')
tmp_dir = os.path.join(grid_home, 'tmp')
try:
    os.makedirs(tmp_dir)
except OSError as e:
    if e.errno != 17:
        raise

# target directory
target_dir = os.path.join(os.getenv('GRID_HOME'), target_subdir)
# target_dir = os.path.join(os.getenv('HOME'), target_subdir)
try:
    os.makedirs(target_dir)
except OSError as e:
    if e.errno != 17:
        raise

# other files to place in collection directory
submit_script = '/mnt/eeh/kaphle/atlas/APlusPlus_git/libAtlasAnalysis/utils/SubmitGridJobs.py'
grid_setup_script = '/mnt/eeh/kaphle/atlas/setup-grid-2.sh'
app_tarball = 'APlusPlus.tgz'
mem_tarball = 'libMEM.tgz'
cuba = 'Cuba-3.0'
cuba_tarball = cuba + '.tar.gz'
cuba_fullpath = os.path.join(os.getenv('GRID_HOME'), cuba)

try:
    # shutil.copy(submit_script, target_dir)
    os.symlink(submit_script, os.path.join(target_dir, os.path.basename(submit_script)))
except OSError as e:
    if e.errno != 17:
        raise
shutil.copy(grid_setup_script, target_dir)
shutil.copy(os.path.join(os.getenv('GRID_HOME'), app_tarball), target_dir)
shutil.copy(os.path.join(os.getenv('GRID_HOME'), mem_tarball), target_dir)

# run through source structure directory by directory
src_dir = app_src_dir
systs = []
print("\nFound systematics:")
for syst in os.listdir(src_dir):
    if not os.path.isdir(os.path.join(src_dir, syst)):
        continue
    print(syst)
    systs.append(syst)
systematics_list_file = os.path.join(target_dir, 'systematics.txt')
with open(systematics_list_file, 'w') as f:
    for syst in systs:
        f.write(syst + '\n')

# get sample list from nominal
samples = []
print("\nFound samples:")
for sample in os.listdir(os.path.join(src_dir, 'nominal')):
    if not os.path.isdir(os.path.join(src_dir, 'nominal', sample)):
        continue
    print(sample)
    samples.append(sample)
samples_list_file = os.path.join(target_dir, 'samples.txt')
with open(samples_list_file, 'w') as f:
    for sample in samples:
        f.write(sample + '\n')

# get short systematics names from list
syst_short_list_file = '/mnt/eeh/kaphle/atlas/APlusPlus_git/libSingleTop/tasks/13TeV/s_channel/systematics_v25.txt'
d_syst_toshort = {}
d_syst_tolong = {}
with open(syst_short_list_file) as f:
    for line in f:
        if line.startswith('#') or line.isspace():
            continue
        s_long, s_short = line.split()
        d_syst_toshort[s_long] = s_short
        d_syst_tolong[s_short] = s_long

# loop
print('\nCopy submission files to %s' % target_dir)
for syst in systs:
    new_syst_dir = os.path.join(target_dir, syst)
    print('  Copy systematic %s' % syst)
    try:
        os.mkdir(new_syst_dir)
    except OSError as e:
        if e.errno != 17:
            raise
    for sample in samples:
        # skip nominal only
        if syst != 'nominal' and (sample.find('data') != -1 or sample.find('QCD') != -1):
            continue
        # source files
        tgz_basename = prefix + '_' + syst + '_' + sample + '.tgz'
        full_app_src_dir = os.path.join(app_src_dir, syst, sample, 'run')
        full_mem_src_dir = os.path.join(mem_src_dir, syst, sample, 'run')
        # files to copy: commands.txt and tgz archive
        app_file = os.path.join(full_app_src_dir, 'app_command.txt')
        mem_file = os.path.join(full_mem_src_dir, 'mem_command.txt')
        app_tgz_file = os.path.join(full_app_src_dir, tgz_basename)
        mem_tgz_file = os.path.join(full_mem_src_dir, 'mem_' + tgz_basename)
        # skip if nonexistent inputs
        if not os.path.exists(app_file):
            print('      Warning: app_file missing for {} {}; skip'.format(syst, sample))
            continue
        if not os.path.exists(mem_file):
            print('      Warning: mem_file missing for {} {}; skip'.format(syst, sample))
            continue
        with open(app_file, 'r') as f:
            cmd_app = f.read()
        with open(mem_file, 'r') as f:
            cmd_mem = f.read()
        # target files
        new_sample_dir = os.path.join(new_syst_dir, sample)
        if os.path.exists(os.path.join(new_sample_dir, tgz_basename)):
            continue
        print('    Copy sample %s' % sample)
        try:
            os.mkdir(new_sample_dir)
        except OSError as e:
            if e.errno != 17:
                raise

        # copy to target: use appropriate naming scheme
        # try:
        #     shutil.copy(app_file, new_sample_dir)
        #     # shutil.copy(app_tgz_file, os.path.join(new_sample_dir, 'app_'+tgz_basename))
        # except IOError as e:
        #     if e.errno != 2: raise
        #     print('    Could not find source file(s) in %s' % full_app_src_dir)
        # try:
        #     shutil.copy(mem_file, new_sample_dir)
        #     # shutil.copy(mem_tgz_file, os.path.join(new_sample_dir, 'mem_'+tgz_basename))
        # except IOError as e:
        #     if e.errno != 2: raise
        #     print('    Could not find source file(s) in %s' % full_mem_src_dir)

        # # copy to target: tarballs from A++ and MemTk as hard links
        # try:
        #     os.link(os.path.join(target_dir, app_tarball), os.path.join(new_sample_dir, app_tarball))
        #     os.link(os.path.join(target_dir, mem_tarball), os.path.join(new_sample_dir, mem_tarball))
        # except OSError as e:
        #     if e.errno != 17: raise # 17 = File exists

        #
        # combine
        #
        # print('\nfrom A++: ')
        # print(cmd_app)
        # print('\nfrom MEM: ')
        # print(cmd_mem)

        # combine bexec:
        # can be removed if tarballs are extracted before
        #        print('\ncombine bexec: ')
        # old_bexec = cmd_app[cmd_app.find('--bexec='):cmd_app.find('--exec')-1]
        app_bexec = cmd_app[cmd_app.find('--bexec='):cmd_app.find('--', cmd_app.find('--bexec=')+1)]
        if unpack_tarballs:
            cmd_comb = cmd_app.replace(app_bexec, '')
        else:
            # new_bexec = "--bexec='tar -xzf APlusPlus.tgz;./bexec.sh' "
            new_bexec = "--bexec='tar -xzf APlusPlus.tgz;tar -xzf libMEM.tgz' "
            cmd_comb = cmd_app.replace(app_bexec, new_bexec)
#        print(cmd_comb)

        # combine exec:
#        print('\ncombine exec: ')
        app_exec = cmd_app[cmd_app.find('--exec=')+7:cmd_app.find('--', cmd_app.find('--exec=')+1)-1]
        # app_exec = cmd_app[cmd_app.find('--exec=')+7:cmd_app.find('--long')-1]
        new_exec = ("'" + app_exec.strip("'") + ';' +
                    app_exec.strip("'").replace('./', './mem_') + "'")
        cmd_comb = cmd_comb.replace(app_exec, new_exec)
#        print(cmd_comb)

        # combine outputs:
        # - changed output files from specific names to generic (app|mem)_output.root
        # app_outp = cmd_app[cmd_app.find('--outputs=')+10:cmd_app.find('--outDS=')-1]
        app_outp = cmd_app[cmd_app.find('--outputs=')+10:cmd_app.find('--', cmd_app.find('--outputs=')+1)-1]
        mem_outp = cmd_mem[cmd_mem.find('--outputs=')+10:cmd_mem.find('--', cmd_mem.find('--outputs=')+1)-1]
        # ntup_file = app_outp[app_outp.find('app_out'):app_outp.find('.root')+5]
        # ntup_file = app_outp[app_outp.find('ntup'):app_outp.find('.root')+5]
        new_outp = app_outp + ',' + mem_outp
        # new_outp = app_outp + ',mem_' + ntup_file + ' '
        cmd_comb = cmd_comb.replace(app_outp, new_outp)

        # change outDS
        # - problem is, some systematics have too long names -> replace in outDS
        syst_short = d_syst_toshort[syst]
        outDS = cmd_comb[cmd_comb.find('--outDS'):cmd_comb.find('--', cmd_comb.find('--outDS')+1)]
        newDS = outDS.replace("schannelAPP", "schan_App_MemTk%s" % (outDS_suffix)).replace(syst, syst_short)
        cmd_comb = cmd_comb.replace(outDS, newDS)
        # cmd_comb = cmd_comb.replace(
        # syst_short = d_syst_toshort[syst]
        # cmd_comb = cmd_comb.replace(syst, syst_short)

        # # combine extfile:
        # print('\ncombine extfile')
        # app_extfile = cmd_app[cmd_app.find('--extFile='):cmd_app.find('--inTarBall=')-1]
        # mem_extfile = cmd_mem[cmd_mem.find('--extFile=')+10:cmd_mem.find('--inTarBall=')-1]
        # new_extfile = app_extfile + ',' + mem_extfile
        # cmd_comb = cmd_comb.replace(app_extfile, new_extfile)
        # print(cmd_comb)

        with open(os.path.join(new_sample_dir, 'commands.txt'), 'w') as f:
            f.write(cmd_comb)
        # pdb.set_trace()
        # print(cmd_comb)

        # combine tar files in a temporary directory
        shutil.rmtree(tmp_dir)
        os.makedirs(tmp_dir)
        tar1 = os.path.join(tmp_dir, 'app_'+tgz_basename)
        tar2 = os.path.join(tmp_dir, 'mem_'+tgz_basename)
        tar3 = os.path.join(tmp_dir, tgz_basename)
        shutil.copy(app_tgz_file, tar1)
        shutil.copy(mem_tgz_file, tar2)
        os.chdir(tmp_dir)
        with tarfile.open(tar1, 'r:gz') as tf1:
            tf1.extractall()
            with tarfile.open(tar2, 'r:gz') as tf2:
                tf2.extractall()
                with tarfile.open(tar3, 'w:gz') as tf3:
                    for f in tf1.getnames():
                        tf3.add(f)
                    for f in tf2.getnames():
                        if f.find('bexec.sh') != -1:  # don't include bexec.sh
                            continue
                        # if unpack_tarballs and f == cuba_tarball:
                        if f == cuba_tarball:
                            tf3.add(cuba_fullpath, arcname=cuba)
                        else:
                            tf3.add(f)
        shutil.copy(tar3, new_sample_dir)
        # subprocess.call("tar -")

        # try with buffer stream without extractall:
        with tarfile.open(tar1, 'r:gz') as tf1, tarfile.open(tar2, 'r:gz') as tf2, tarfile.open(tar3+'_test', 'w:gz') as tf3:
            for m in tf1.members+tf2.members:
                buf = m.tobuf()
                if isinstance(buf, str):
                    buf = buf.encode("utf-8")
                fileobj = BytesIO(buf)
                tarinfo = m  # tarfile.TarInfo(m.name)
                tarinfo.size = len(fileobj.getvalue())
                tf3.addfile(tarinfo, fileobj)
        #         with tempfile.TemporaryFile() as tmp:
        #             tmp.addfile(m, fileobj)
        # with tempfile.TemporaryFile() as tmp:

        if test_one:
            break
    if test_one:
        break
print('Done, placed submission files in {0}'.format(target_dir))
