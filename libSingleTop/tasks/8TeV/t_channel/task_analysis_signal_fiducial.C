{
    //____________________________________________________________________
    //
    // SgTop t-channel analysis for the signal region 
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    Bool_t UseMV1c           = kTRUE;
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildPlottingTree = kTRUE;
    Bool_t BuildMergingTree  = kTRUE;

    const char* dir  = "/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f";
    const char* tdir = "/datc/atlas/stamm/8TeV_t_channel/v15f";


    // Study on modelling systematics
    // - Wuppertal w/o dilepton veto
    // const char *ext     = "Signal_study/fiducial_cuts_no_dlveto";
    // - Wuppertal w/ dilepton veto
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto";
    // - Wuppertal w/ dilepton veto w/ QCD veto
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_qcdveto";
    // - Wuppertal w/ dilepton veto lep pt > 30
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_lep30";
    // - Wuppertal w/ dilepton veto + Mem Opt cuts
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_memopt";
    // - Wuppertal w/ dilepton veto + jet cut 35
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35";
    // - Wuppertal w/ dilepton veto + jet cut 35 + jet veto
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35_jetveto";
    // - Wuppertal w/ dilepton veto + jet cut 35 + lep pt 30
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35_lep30";
    // - Wuppertal w/ dilepton veto + jet cut 35 + lep pt 30 + jet veto
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35_lep30_jetveto";
    // - Wuppertal w/ dilepton veto + jet cut 35 + lep pt 30 + jet veto + mem opt cuts
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35_lep30_jetveto_memopt";
    // - Wuppertal w/ dilepton veto + jet cut 35 + lep pt 30 + mem opt cuts
    // const char *ext     = "Signal_study/fiducial_cuts_dlveto_jet35_lep30_memopt";
    // - INT Note status from 1. EB Meeting (dijet7 calibration)
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting";
    // - INT Note status from 1. EB Meeting (dijet7 calibration)
    const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_nomcmc";
    // - INT Note status from 1. EB Meeting (dijet10 calibration)
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_bdijet10";
    // - INT Note status from 1. EB Meeting (dijet10 calibration)
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_bdijet10_nomcmc";
    // - INT Note status from 1. EB Meeting
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_bdefault";
    // - INT Note status from 1. EB Meeting
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_bdefault_nomcmc";
    // - INT Note status from 1. EB Meeting
    // const char *ext     = "Signal_study/fiducial_cuts_intnote_1eb_meeting_lead35";
    
    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;
    
    gROOT->Macro("task_analysis_main.C");

    // Selector Cuts
    main->SetCut("fPt_Jet_min", "35.");
    
    // Apply additional t-channel cuts    
    main->SetCut("fApplyDileptonVeto", "kTRUE");
    main->SetCut("fMET_min", "30.");
    main->SetCut("fMtW_minPre", "30.");
    main->SetCut("fMtW_min", "50.");
    main->SetCut("fQCDVeto", "kFALSE");
    main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
    main->SetCut("fNBTags_min", "1");
    main->SetCut("fNBTags_max", "1");
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fApplyBJetVeto", "kFALSE");
    main->SetCut("fVetoBTagWeight_min", "0.9195");  // MV1c@50%
    main->SetCut("fVetoBTagger", "AtlBTag::kMV1c"); // MV1c
    main->SetCut("fMCTruthStudy", "kFALSE");
    main->SetCut("fPtJetlead_min", "40.");

    // This cut applied?
    main->SetCut("fMljb_min", "40.");
    main->SetCut("fMljb_max", "160.");
    main->SetCut("fMjlfjb_min", "80.");
    main->SetCut("fMtW_max", "125.");
    main->SetCut("fHt_min", "180.");
    main->SetCut("fMjlfjbHt_cut", "kTRUE");
    main->SetCut("fApplySchannelVeto", "kTRUE");
    
    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "35.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "35.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "2.7");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "3.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    // Use MV1c 80%
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.9195"); // MV1c@50%

    main->SetToolParameter("SFTool", "fBTagWP",
     			   "AtlObjRecoScaleFactorTool::kMV1c_50");
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
