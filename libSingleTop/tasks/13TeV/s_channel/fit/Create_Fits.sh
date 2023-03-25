#!/bin/sh

# add root commands here for simpler access

#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/new_setting", "v05_Max_new_setting")'
#~/atlas/AtlasUtils/GetSFfromYields.sh /rdsk/datm4/atlas/kaphle/HistFactory/13TeV_s_channel/v05_Max_new_setting/sgtop_schan_wjets/yields/yields_Comb_Lep_MtW_obsData.tex

#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/old_settingI", "v05_Max_old_settingI")'
#~/atlas/AtlasUtils/GetSFfromYields.sh /rdsk/datm4/atlas/kaphle/HistFactory/13TeV_s_channel/v05_Max_old_settingI/sgtop_schan_wjets/yields/yields_Comb_Lep_MtW_obsData.tex
#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/gottwalm/13TeV_s_channel/v04_QCDtests_Nedaa_eff_files_trigsplit/sgtop_schan_wjets/old_settingII", "v05_Max_old_settingII_2")'
#~/atlas/AtlasUtils/GetSFfromYields.sh /rdsk/datm4/atlas/kaphle/HistFactory/13TeV_s_channel/v05_Max_old_settingII_2/sgtop_schan_wjets/yields/yields_Comb_Lep_MtW_obsData.tex

# v07 wjets
#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets", "v07_MEM")'
#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets", "v07_MEM", 1)'
#root -l 'task_fit_wjets.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets", "v07_MEM", 2)'
# ~/atlas/AtlasUtils/GetSFfromYields.sh /rdsk/datm4/atlas/kaphle/HistFactory/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/yields/yields_Comb_El_MtW_obsData.tex

# v07 MEM disc SR: create only templates
#root -l 'task_sChannel_SR_templates.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_oldalphas", "v07_MEM")'
root -l 'task_sChannel_SR_templates.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_oldalphas", "v07_MEM_optimized_bins_8TeV")'
