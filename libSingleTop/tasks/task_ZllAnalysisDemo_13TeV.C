void task_ZllAnalysisDemo_13TeV(Int_t task = 0) {

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main = new AtlTopLevelAnalysis("AtlZllAnalysisDemo", "Z->ll Analysis Demo");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(kTRUE);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildAnalysisTree(kTRUE);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    main->SetBuildMergingTree(kFALSE);
    main->SetBuildPlottingTree(kTRUE);
    main->SetUseHforSamples(kFALSE);
    main->SetTtbarIsSplitted(kFALSE);

    // Grid config
    main->SetGridRootVersion("5.34/19");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("kind");
    main->SetGridSuffix("v2");
    main->SetGridJob(kFALSE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetCampaign(15);
    main->SetSgTopVersion("v05");
    main->SetCrossSectionFilePath("/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisTop/2.3.26/TopDataPreparation/data/XSection-MC15-13TeV-fromSusyGrp.data");
    main->SetDSIDList("SgTopDSIDs_mc15_v05.txt");
    main->ReadSampleList();

    // Setup Directories
    char const* user = gSystem->Getenv("USER");
    main->SetHistDir(Form("/rdsk/datm5/atlas/%s/ZllAnalysisDemo", user));
    main->SetJobHomeDir(Form("/rdsk/datm5/atlas/%s/ZllAnalysisDemo", user));
    main->SetTempDir(Form("/datc/atlas/%s/ZllAnalysisDemo.tmp", user));
    main->SetInputDir(Form("%s/%s", main->GetHistDir()->Data(), main->GetName()));
    main->SetPlotDir(Form("%s/%s", main->GetHistDir()->Data(), main->GetName()));

    // Set output file prefix
    main->SetOutputFilePrefix("ntup_z");

    // ===============
    // Set systematics
    // ===============
    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);
    
    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Set jet multiplicity (exactly 1 jet)
    main->AddJetMultiplicity(AtlSelector::kOneJet);

    // Set lepton channel (e or u)
    main->AddLeptonChannel(AtlSelector::kLepton);
    
    // Analysis selector + Cuts
    main->SetSelector("AtlZllAnalysisDemo");
    main->SetCut("fChgCombination",
		 "AtlZllAnalysisDemo::kChgOpposite");
    main->SetCut("fMZ_min", "70.");
    main->SetCut("fMZ_max", "110.");
//  main->SetCut("fPt_Lep1_min", "35.");

    // Set output tree and format
    main->SetOutputTree("physics", "SgTopD3PD Z->ll selection");

    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj = new AtlObjectsToolD3PDSgTop("D3PDObjects", "Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor", "AtlJet::kAntiKt4TopoEM");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "15.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "15.");
    // Use MV2c20 77% (the only choice for v02)
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c20");
    main->SetToolParameter("D3PDObjects", "fBTagWeight_min", "-0.4434");

    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
                                                                       "SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV2c20_77");
    main->SetToolParameter("SFTool", "fOperationMode",
                           "AtlObjRecoScaleFactorTool::kAllSF"); // Check with D3PDObjects config!
    main->SetToolParameter("SFTool", "fUseCDI", "kFALSE"); // Do not use CDI interface

    
    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("tZ");
    main->SetDataLuminosity(85.); // pb^{-1}
    main->SetAtlasLabelStatus("Work in Progress"); 
    main->SetCenterOfMassEnergyLabel("13");
    main->SetLuminosityLabel("85 pb^{-1}");
    main->SetDrawDataZeroEntryErrors("kFALSE");
    main->SetDrawMCError("kTRUE");
    main->SetExportPDF("kTRUE");

    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );

    switch ( task ) {
    case 1: ((AtlTask *)main->GetListOfTasks()->At(0))->ExecuteTask(); break;
    case 2: ((AtlTask *)main->GetListOfTasks()->At(2))->ExecuteTask(); break;
    }
}
