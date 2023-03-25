{
    //____________________________________________________________________
    //
    // SgTop t-channel analysis for the signal region 
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildPlottingTree = kTRUE;
    Bool_t BuildMergingTree  = kTRUE;
     
    const char* dir  = "/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f";
    const char* tdir = "/datc/atlas/stamm/8TeV_s_channel/v15f";

    const char *ext     = "signal_mc2mc";
    
    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;
    
    gROOT->Macro("task_analysis_main.C");

    // Selector Cuts
    main->SetCut("fPt_Jet_min", "30.");
    main->SetCut("fPt_LeadingJet_min", "40.");
    main->SetCut("fApplyDileptonVeto", "kTRUE");
    main->SetCut("fMET_min", "35.");
    main->SetCut("fMtW_minPre", "30.");
    main->SetCut("fMtW_min", "30.");
    main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
    main->SetCut("fNBTags_min", "2");
    main->SetCut("fNBTags_max", "2");
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fMCTruthStudy", "kFALSE");

    
    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.7892"); // MV1@70%

    main->SetToolParameter("SFTool", "fBTagWP",
     			   "AtlObjRecoScaleFactorTool::kMV1_70");

    // PdfReweightingTool + Cuts
    AtlPdfReweightingTool *tool_pdf = new AtlPdfReweightingTool("PdfReweightingTool", "Pdf Reweighting");
    main->AddTool(tool_pdf);

    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
