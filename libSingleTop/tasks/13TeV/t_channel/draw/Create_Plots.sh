#!/bin/sh

#################
# Control Plots #
#################

# # Signal region (arguments: infile, is_wjetsMerged, channel)
root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE)'
root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_mu")'
root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_el")'

# # Wjets validation region (arguments: infile, do_scale, channel, is_wjetsMerged) -> 3 unscaled, 3 scaled
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE)'
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_el")'
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_mu")'
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE, "_el")'
root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE, "_mu")'

# ttbar 4j validation region (same args as Wjets)
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE)'
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_el")'
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE, "_mu")'
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE, "_el")'
root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE, "_mu")'

# ttbar 3j validation region (same args as wjets and ttbar4j, no is_wjetsWerged))
#  note: scale factors for emu/munu regions are not available right now (always use lnu SF)
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE)'
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, "_el")'
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, "_mu")'
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE)'
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE, "_el")'
root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE, "_mu")'

#  note: currently in v07_MEM only the 1/20 reduced sample is used -> use older files with identical selection above
# root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/SgTop_s/MCPlotter.root", kFALSE)'
# root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'

############################################################################################################
# old content 
############################################################################################################

# add root commands here for simpler access
#root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v02_copied/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v02_copied/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlTtbar4j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/plots_datamc/MCPlotter.root",kFALSE)'
#root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root",kFALSE)'
#root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests_reco_fake_param/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v03_QCDtests_reco_fake_param/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE)'

# (file, do_scale, is_merged, channel)
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE, kTRUE)'
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, kTRUE)'
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE, kTRUE, "_el")'
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, kTRUE, "_el")'
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kTRUE, kTRUE, "_mu")'
# root -l -b -q 'draw_CtrlWjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_emusplit/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, kTRUE, "_mu")'
# save_pwd=$PWD
# scriptdir=$APP_HOME/libSingleTop/tasks/13TeV/s_channel/draw
# script_wjets=$scriptdir/draw_CtrlWjets.C
# script_ttbar=$scriptdir/draw_CtrlTtbar4j.C
# script_ttbar3j=$scriptdir/draw_CtrlTtbar3j.C
# script=$script_ttbar
# cd $scriptdir

# # Max: plot everything unscaled and scaled
# plotdir="Max_param_split_on_tag77"
# #MCPlotter="/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/param_split_on_tag77_2/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root"
# #MCPlotter="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit_tag77/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root"
# MCPlotter="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit_tag77/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal/plots_datamc/MCPlotter.root"
# scales="kFALSE kTRUE"
# #scales="kFALSE"
# #channels="_mu _el _lep"
# channels="_lep"
# mkdir -p $plotdir
# cd $plotdir
# # unscaled (pre-fit) and scaled (post-fit)
# for scale in $scales; do 
#     for channel in $channels; do
# 	root -l -b -q "${script}(\"${MCPlotter}\", ${scale}, kFALSE, \"${channel}\")"
#     done
# done
# #root -l -b -q '../draw_CtrlWjets.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/param_split_on_tag77/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, kFALSE, "_el")'
# #root -l -b -q '../draw_CtrlWjets.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/param_split_on_tag77/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", kFALSE, kFALSE, "")'
# cd $save_pwd


# Max' files with different SF for Top Background Talk (see mail from 21.8. for details)
# save_pwd=$PWD
# scriptdir=$APP_HOME/libSingleTop/tasks/13TeV/s_channel/draw
# script_wjets=draw_CtrlWjets.C
# dirs="old_settingII" #"old_settingI old_settingII new_setting"
# scales="kFALSE kTRUE"
# for dir in $dirs; do
#     script=$scriptdir/$dir/draw_CtrlWjets.C
#     cd $scriptdir/$dir
#     MCPlotter=/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/$dir/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root
#     for scale in $scales; do
# 	root -l -b -q "${script}(\"${MCPlotter}\", ${scale})"
#     done
# done
# cd $save_pwd

# test signal
#root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/old_settingII/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'
#root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'

# signal for 2017-12-08 physics workshop
#root -l -b -q 'draw_Signal.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/plots_datamc/MCPlotter.root")'

#root -l -b -q 'draw_CtrlTtbar3j.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_ttbar/old_settingII/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal/plots_datamc/MCPlotter.root",kFALSE)'
