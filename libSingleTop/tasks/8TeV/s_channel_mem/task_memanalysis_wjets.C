{
    //____________________________________________________________________
    //
    // SgTop t-channel analysis for the signal region 
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    Bool_t ForceRetry        = kTRUE;

    const char* dir  = "/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f";
    const char* tdir = "/datc/atlas/stamm/8TeV_t_channel/v15f";
    const char *ext     = "wjets";

    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    const char *outdir  = "MemDiscAnalysis";
    
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;
    
    gROOT->Macro("task_memanalysis_main.C");

    // Selector Cuts
    main->SetCut("fApplyDileptonVeto", "kFALSE");
    main->SetCut("fPt_LeadingJet_min", "40.");

    
    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1"); 
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.3511 "); // MV1@80%

    main->SetToolParameter("SFTool", "fBTagWP",
     			   "AtlObjRecoScaleFactorTool::kMV1_80");
    
    // PdfReweightingTool + Cuts
    AtlPdfReweightingTool *tool_pdf = new AtlPdfReweightingTool("PdfReweightingTool", "Pdf Reweighting");
    main->AddTool(tool_pdf);

    // Wjets SF Tool
    // AtlWjetsScaleFactorTool *tool_wjets = new AtlWjetsScaleFactorTool("WjetsSF", "Wjets SF Tool");
    // main->AddTool(tool_wjets);

    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
