{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis main configuration
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!   Do not execute this file   !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    // This file is used by task_sChannelAnlysis_Signal.C and the
    // corresponding scripts for the ctrl regions.
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
				  "SgTop tChannel Analysis");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(BuildHforTree);

    main->SetBuildAnalysisTree(kTRUE);
    main->SetBuildMemTkAnalysisTree(kFALSE);

    // main->SetBuildAnalysisTree(kFALSE);
    // main->SetBuildMemTkAnalysisTree(kTRUE);

    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    main->SetUseHforSamples(kFALSE);
    
    // Grid config
    main->SetGridRootVersion("5.34/21");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v04");
    // main->SetGridJob(kFALSE);
    main->SetGridJob(kTRUE);

    // MemTk config
    // main->SetMemTkRunScript("run_tChannel.C");
    // t-channel defaults - SR
    // main->SetNSubJobsData(4);
    // main->SetNSubJobsQCD(4);
    // main->SetNSubJobsSgTopSChannel(1); // 16
    // main->SetNSubJobsSgTopTChannel(30); // 30
    // main->SetNSubJobsSgTopWt(3); // 1
    // main->SetNSubJobsTtbar(2);   // 2
    // main->SetNSubJobsWjetsB(12); // 12
    // main->SetNSubJobsWjetsC(1);

    // t-channel defaults - WCR
    main->SetMemTkRunScript("run_tChannel_wjets.C");
    main->SetNSubJobsData(20);
    main->SetNSubJobsQCD(20);
    main->SetNSubJobsSgTopSChannel(18);
    main->SetNSubJobsSgTopTChannel(25);
    main->SetNSubJobsSgTopWt(3);
    main->SetNSubJobsTtbar(2);
    main->SetNSubJobsWjetsB(30);
    main->SetNSubJobsWjetsC(25);
    main->SetNSubJobsWjetsLight(25);

    // t-channel defaults - tt CR
    // main->SetMemTkRunScript("run_tChannel_ttbar.C");
    // main->SetNSubJobsData(1);
    // main->SetNSubJobsQCD(1);
    // main->SetNSubJobsSgTopSChannel(2);
    // main->SetNSubJobsSgTopTChannel(2);
    // main->SetNSubJobsSgTopWt(1);
    // main->SetNSubJobsTtbar(2);
    // main->SetNSubJobsWjetsB(1);
    // main->SetNSubJobsWjetsC(1);
    // main->SetNSubJobsWjetsLight(1);

    // main->SetNSubJobsSgTopTChannel(50);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetSgTopVersion("v15");
    // main->SetDSIDList("SgTopDSIDs_v15_pdf.txt");
    main->SetDSIDList("SgTopDSIDs_v15.txt");
    main->SetCampaign(12);
    main->ReadSampleList("../SampleList");
    main->SetCrossSectionFilePath(Form("%s/dbase/TopDataPreparation/XSection-MC12-8TeV.data",
				       gSystem->Getenv("LIBATLASANALYSIS")));

    // Setup Directories
    main->SetHistDir(histdir);
    main->SetJobHomeDir(jobdir);
    main->SetTempDir(tempdir);

    // For MemTk Analysis (i.e. "the histdir + main->GetName()")
    main->SetInputDir(Form("%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName()));
    
    // For A++-Analysis
    // (i.e. "histdir + main->GetName()")
    //
    // n.b. MemTk plotting --> 2nd task-file needed
    main->SetPlotDir(Form("%s/%s",
			  main->GetHistDir()->Data(),
			  main->GetName()));
        
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_tchan");
    
    // Set jet bins
    main->AddJetMultiplicity(jetbin);

    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJESCompOnly);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);

    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);

    // main->AddSystematic(AtlTopLevelAnalysis::kBJES_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kBJES_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX3_UP);

    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX4_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX4_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL4_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL4_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT4_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT4_UP);

    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFMU_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFMU_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_PT_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_PT_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_RHO_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_RHO_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kSINGLEPART_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kSINGLEPART_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_COMP_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_COMP_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_RESP_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_RESP_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kPUNCH_THROUGH_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kPUNCH_THROUGH_UP);

    // main->AddSystematic(AtlTopLevelAnalysis::kWtDS);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_wt);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_ttbar); // 2 subjob < 15h
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_tchan); // 50 subjobs
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_schan);
    
    // main->AddSystematic(AtlTopLevelAnalysis::kRadHi_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadLo_ttbar);

    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar1); // 4 Subjobs a 24h
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar4);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar5);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar6);

    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan1); // 15 subjobs a 22h
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan4);

    // main->AddSystematic(AtlTopLevelAnalysis::kPowhegPythia_t); // 20 subjobs a 24h
    // main->AddSystematic(AtlTopLevelAnalysis::kPowhegPythia2011C_t); // 20 subjobs a 24h
    // main->AddSystematic(AtlTopLevelAnalysis::kPowhegHerwig_t); // 35 subjobs a 24h

    // main->AddSystematic(AtlTopLevelAnalysis::kWjetsGen); // 2 for WbbNp1 and WcNp1

    
    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_tChannelAnalysis");

    // Set output tree and format
    main->SetOutputTree("physics", "SgTopD3PD t-channel selection");

    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj =
	new AtlObjectsToolD3PDSgTop("D3PDObjects","Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor",
			   "AtlJet::kAntiKt4LCTopo");
    
    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
								       "SF tool");
    main->AddTool(tool_sf);

    // If analyses uses the CDI tool, the WP is determined
    // by the given min. btag weight from the Object Tool.
    //
    // Otherwise, the WP needs to be set manually with the following line
    //
    // main->SetToolParameter("SFTool", "fBTagWP",
    // 			   "AtlObjRecoScaleFactorTool::kMV1c_50");
    //
    // NB: Then, the CDI tool needs to be turned off here.
    main->SetToolParameter("SFTool", "fUseCDI", "kTRUE");
    main->SetToolParameter("SFTool", "fOperationMode",
			   "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!
    
    // QCDMM Tool + Cuts
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD MatrixMethod");
    main->AddTool(tool_qcd);
    
    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_t"); //Ttbar or SgTop
    main->SetDataLuminosity(20276.900); // pb^{-1}
    main->SetAtlasLabelStatus("Work in Progress"); 
    main->SetCenterOfMassEnergyLabel("8");
    main->SetLuminosityLabel("20.3 fb^{-1}");
    main->SetDrawDataZeroEntryErrors(kFALSE);
    main->SetDrawMCError(kTRUE);
    main->SetUsePlotterSchemeDir(kTRUE);
    // main->SetUsePlotterSchemeDir(kFALSE);
    main->SetExportPDF(kFALSE);
}
