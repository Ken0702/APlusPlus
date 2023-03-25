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

    const char* dir  = "/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f";
    const char* tdir = "/datc/atlas/stamm/8TeV_t_channel/v15f";
    const char *ext     = "Signal_combination/final_cuts_schanveto";
    // const char *ext     = "Signal_combination/final_cuts_schanveto_pdfsyst";

    const char* histdir = Form("%s/%s", dir, ext);
    const char* jobdir  = histdir;
    const char* tempdir = Form("%s/%s", tdir, ext);
    const char *outdir  = "MemDiscrAnalysis_dijet7_mcmcsf";
    // const char *outdir  = "MemDiscrAnalysis_dijet7_mcmcsf_largettbarweight";
    
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;
    
    gROOT->Macro("task_memanalysis_main.C");

    // Selector Cuts
    main->SetCut("fSchannelVeto", "kTRUE");
    
    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "0.");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.9195"); // MV1c@50%

    main->SetToolParameter("SFTool", "fBTagWP",
     			   "AtlObjRecoScaleFactorTool::kMV1c_50");

    // Wjets SF Tool
    // AtlWjetsScaleFactorTool *tool_wjets = new AtlWjetsScaleFactorTool("WjetsSF", "Wjets SF Tool");
    // main->AddTool(tool_wjets);

    AtlPdfReweightingTool *tool_pdf = new AtlPdfReweightingTool("PdfReweightingTool", "Pdf Reweighting");
    main->AddTool(tool_pdf);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
