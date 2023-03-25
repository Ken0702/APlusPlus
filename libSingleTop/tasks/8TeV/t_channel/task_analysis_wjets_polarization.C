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
    const char *ext     = "Wjets_polarization/default_cuts";
    
    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;
    
    gROOT->Macro("task_analysis_main.C");

    // Selector Cuts
    main->SetCut("fPt_Jet_min", "30.");
    main->SetCut("fEta_Jet_max", "4.");
    main->SetCut("fDeltaEta_Jets_max", "4.8");
    
    // Apply additional t-channel cuts    
    main->SetCut("fApplyDileptonVeto", "kFALSE");
    main->SetCut("fMET_min", "30.");
    main->SetCut("fMtW_minPre", "30.");
    main->SetCut("fMtW_min", "50.");
    main->SetCut("fQCDVeto", "kTRUE");
    main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
    main->SetCut("fNBTags_min", "1");
    main->SetCut("fNBTags_max", "1");
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fApplyBJetVeto", "kTRUE");
    main->SetCut("fVetoBTagWeight_min", "0.9195");  // MV1c@50%
    main->SetCut("fVetoBTagger", "AtlBTag::kMV1c"); // MV1c
    main->SetCut("fMCTruthStudy", "kFALSE");

    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "35.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "2.75");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "3.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "25.");
    // Use MV1c 80%
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.4051"); // MV1c@80%

    main->SetToolParameter("SFTool", "fBTagWP",
     			   "AtlObjRecoScaleFactorTool::kMV1c_80");
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
