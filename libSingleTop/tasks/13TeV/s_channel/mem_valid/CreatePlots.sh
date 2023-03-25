#!/bin/sh

# =============
# current plots
# =============

# LLH plots signal region
#root -l -b -q 'plot_llh_sig_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'

# LLH plots validation regions
#root -l -b -q 'plot_llh_wjets_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_wjets_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'
#root -l -b -q 'plot_llh_ttbar3j_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar3j_20th/AtlSgTop_sChannelAnalysis/MemDisc/3Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar3j_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar3j_20th/AtlSgTop_sChannelAnalysis/MemDisc/3Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'
#root -l -b -q 'plot_llh_ttbar4j_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j_20th_4JetVeto/AtlSgTop_sChannelAnalysis/MemDisc/4Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar4j_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j_20th_4JetVeto/AtlSgTop_sChannelAnalysis/MemDisc/4Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'


# MEM discriminant plots signal region
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc_Maxcut_alphas_wjetsMerged_oldrebinning/2Jets/lnu/nominal/SgTop_s_wjetsMerged/MCPlotter.root")'
root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc_Maxcut_alphas_wjetsMerged_newrebinning/2Jets/lnu/nominal/SgTop_s_wjetsMerged/MCPlotter.root")'

# # MEM discriminant plots validation regions
# root -l -b -q 'plot_schan_wjets_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
# root -l -b -q 'plot_schan_wjets_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'

#---------------------------------------------------------------------------

# =========
# old plots
# =========

#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr6/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr7/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_wjets_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v06_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDiscr_test/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_wjets_lnu.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v06_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDiscr_test/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", kTRUE)'
# root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr8/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
# root -l -b -q 'plot_schan_wjets_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v06_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDiscr2_test/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'
# root -l -b -q 'plot_schan_wjets_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v06_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/MemDiscr2_test/2Jets/lnu/nominal/SgTop_s/MCPlotter.root",kTRUE)'


# signal region discriminant for different alpha settings
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", "_newalphasnonorm")'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_oldalphas/2Jets/lnu/nominal/SgTop_s/MCPlotter.root", "_oldalphas")'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_newalphas/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", "_newalphas")'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_Maxcut/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", "_Maxcut", 0.5)'
#root -l -b -q 'plot_schan_sig_discr.C("/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDiscr_Maxcut_alphas/2Jets/lnu/nominal/plots_datamc/MCPlotter.root", "_Maxcut_alphas", 0.5)'

#---------------------------------------------------------------------------

# from ~/atlas/INT_ST_channelME_8TeV/figures/mem_valid_schannel/scripts/CreateAllPlots.sh:

#
# Signal
#
#root -l -b -q 'plot_llh_sig_enu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/signal/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_ptreweighted/2Jets/enu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_sig_munu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/signal/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_ptreweighted/2Jets/munu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_sig_lnu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/signal/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_ptreweighted/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'

#
# Wjets
#
#root -l -b -q 'plot_llh_wjets_enu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/enu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_wjets_munu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/munu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_wjets_lnu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/lnu/nominal/SgTop_s/MCPlotter.root")'

#
# ttbar
#
#root -l -b -q 'plot_llh_ttbar_enu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/ttbar/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/4Jets/enu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar_munu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/ttbar/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/4Jets/munu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar_lnu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/ttbar/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/4Jets/lnu/nominal/SgTop_s/MCPlotter.root")'

#
# ttbar 3j
#
#root -l -b -q 'plot_llh_ttbar3j_enu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/intv06/ttbar_3jets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/3Jets/enu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar3j_munu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/intv06/ttbar_3jets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/3Jets/munu/nominal/SgTop_s/MCPlotter.root")'
#root -l -b -q 'plot_llh_ttbar3j_lnu.C("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/intv06/ttbar_3jets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/3Jets/lnu/nominal/SgTop_s/MCPlotter.root")'

