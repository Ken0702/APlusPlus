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
    const char *ext     = "ttbar_combination/default_cuts_INTv04";
    
    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    
    AtlSelector::EJetMult jetbin = AtlSelector::kFourJet;
    
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
    main->SetCut("fNBTags_min", "2");
    main->SetCut("fNBTags_max", "2");
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fApplyBJetVeto", "kFALSE");
    main->SetCut("fVetoBTagWeight_min", "0.9195");  // MV1c@50%
    main->SetCut("fVetoBTagger", "AtlBTag::kMV1c"); // MV1c
    main->SetCut("fMCTruthStudy", "kFALSE");

    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "0.");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    // Use MV1c 50%
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
