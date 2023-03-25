void TRExFitter_task() {
    //
    // v23
    //
    // TString filepath = TString("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v08_v20lj/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc_reduced/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_withAddJetVeto_pt_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_testnewTF_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_testnewTF_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    //
    // v25
    //
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/users/eeh/kaphle/atlas/plots/s-channel/v15_v25_default_a/MEMDisc_SR/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/users/eeh/kaphle/atlas/plots/s-channel/v15_v25_default_d/ctrl_SR/nominal/SgTop_s_QCDsplit_wjetsMerged");
//        TString filepath("/users/eeh/kaphle/atlas/plots/s-channel/v15_v25_default_a/ctrl_WjetsVR/nominal/SgTop_s_QCDsplit_wjetsMerged");
//        TString filepath("/users/eeh/kaphle/atlas/plots/s-channel/v15_v25_default_d/ctrl_ttbar3jVR/nominal/SgTop_s_QCDsplit_wjetsMerged");
//        TString filepath("/users/eeh/kaphle/atlas/plots/s-channel/v15_v25_default_a/ctrl_ttbar4jVR/nominal/SgTop_s_QCDsplit_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_QCD_scale_factors_MC16d/sgtop_schan_signal_noMETcut/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_QCD_scale_factors_MC16d/sgtop_schan_signal_noMtWcut/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_wjetsMerged");
//    TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_QCD_scale_factors_MC16d/sgtop_schan_signal_noMETMtWcuts/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_QCD_scale_factors_MC16d/sgtop_schan_signal_noMtWcut/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");
    //
    // default:
    //
//    TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");
//    TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");
//    TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_samplesyst_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");
//    TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16d/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");
    //
    // // test btag 85% WP for JetLepton
    // TString filepath("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_QCDJetLepton_MC16d/sgtop_schan_signal_test_JetLepton_bTag85/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");
    //
    // // use btag85%
//    TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_QCDJetLepton_MC16d/sgtop_schan_signal_bTag85_noinvalid/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");
    //
    // use no tag at all
//    TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_QCDJetLepton_MC16d/sgtop_schan_signal_noBTag/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");


//    TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");
//    TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_samplesyst_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");
    //
    // latest default MemDisc with skipping invalid (+added QCDJL)
    //
//   TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_MC16a/sgtop_schan_signal_noinvalid/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit");
//    TString filepath("/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v15_v25lj_default_samplesyst_MC16d/sgtop_schan_signal_noinvalid/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");
    // note that not every use is listed here; some are in ~/atlas/plots/s-channel/v15_v25_default_a/d
//////////////////////// TCHANNEL
TString filepath("/rdsk/dats1/atlas/kkreul/combination/A++/v02_v34lj_sampleSysts_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");
//TString filepath("/rdsk/dat23/atlas/kkreul/combination/A++/v02_v34lj_77Btag_sampleSysts_MC16e/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");
//TString filepath("/rdsk/dats1/atlas/kkreul/combination/A++/v02_v34lj_NewFullMEM_MC16d/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged");  
//TString filepath("/rdsk/dat23/atlas/kkreul/combination/A++/v02_v34lj_sampleSysts_MC16e/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_splitall");  
//TString filepath("/rdsk/dats1/atlas/kkreul/combination/A++/v02_v34lj_QCDFIT_ShapeSyst_NoMTW_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/2Jets/lnu/nominal/SgTop_s_splitall");

Bool_t discr_only = kFALSE;   

// don't touch

    TString DataMCFile =  filepath + TString("/MCPlotter.root");
    TString trex_subdir;
    TString DiscrPath;
    if ( discr_only ) {
        DiscrPath = "/MemDiscriminant/ManyBins/sChannelProb_raw";
        //DiscrPath = "/MemDiscriminant/ManyBins";
        trex_subdir = "/adapted_for_trexfitter/";
    } else {
        DiscrPath = "";
        trex_subdir = "/adapted_for_trexfitter_allhist/";
    }
    TString OutputDir = filepath + trex_subdir;
    gSystem->mkdir(OutputDir.Data(), kTRUE);
    const char* OutDiscr = "MEdiscr";

    std::unique_ptr<AtlTRExFitterTask> rex_fitter (new AtlTRExFitterTask("rex_fitter", "TRExFitterTask"));
    rex_fitter->SetInputDataMCFile(DataMCFile.Data());
    // the input discr path decides whether to use one or all histograms; set to "" (default) for all histograms
    rex_fitter->SetInputDiscrPath(DiscrPath.Data());
    rex_fitter->SetOutputDir(OutputDir.Data());
    rex_fitter->SetOutputDiscrName(OutDiscr);

    // // Start task browser
    // gROOT->GetListOfBrowsables()->Add(rex_fitter, "TRExFitterTask");
    // new TBrowser;
    // gDebug = 1;
    rex_fitter->ExecuteTask();

}
