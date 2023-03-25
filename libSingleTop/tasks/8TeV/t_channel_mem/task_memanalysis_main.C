{
    //____________________________________________________________________
    //
    // SgTop t-channel analysis configuration for running
    // the Mem Discriminant computation.
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
				  "SgTop tChannel Mem Discriminant Analysis");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildAnalysisTree(kFALSE);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    
    main->SetBuildMergingTree(kTRUE);
    main->SetBuildPlottingTree(kTRUE);
    main->SetBuildBDTAnalysisTree(kTRUE);
    // main->SetBuildBDTAnalysisTree(kFALSE);
    
    main->SetUseSubJobsAsInput(kTRUE);
    main->SetUseHforSamples(kFALSE);

    // Grid config
    main->SetGridRootVersion("5.34/17");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v2");
    main->SetGridJob(kFALSE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetSgTopVersion("v15");
    main->SetDSIDList("SgTopDSIDs_v15.txt");
    // main->SetDSIDList("SgTopDSIDs_v15_pdf.txt");
    main->SetCampaign(12);
    main->ReadSampleList("../SampleList");
    main->SetCrossSectionFilePath(Form("%s/dbase/TopDataPreparation/XSection-MC12-8TeV.data",
				       gSystem->Getenv("LIBATLASANALYSIS")));

    // Input directories
    main->SetHistDir(histdir);
    main->SetJobHomeDir(jobdir);
    main->SetTempDir(tempdir);
    
    // main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
    // main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
    // main->SetTempDir("/datc/atlas/stamm/v15/cut7dl");
    
   
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + MemTkAnalysis")
    //
    // Name of 'main' must be the same as task_tChannelAnalysis_Main.C
    main->SetInputDir(Form("%s/%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName(),
			   "MemTkAnalysis"));
    
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + AnalysisName")
    //
    // Name of 'main' must be the same as task_tChannelAnalysis_Main.C
    main->SetPlotDir(Form("%s/%s/%s",
			  main->GetHistDir()->Data(),
			  main->GetName(),
			  outdir));
    
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_tchan");
    
    // Set jet bin
    main->AddJetMultiplicity(jetbin);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);

    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJESCompOnly);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpBTagEV);
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
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_tchan);
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_schan);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadHi_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadLo_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar1);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar4);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar5);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar6);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan1);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan4);

    // main->AddSystematic(AtlTopLevelAnalysis::kPowhegPythia_t);
    // main->AddSystematic(AtlTopLevelAnalysis::kPowhegHerwig_t);

    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_down);
    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_up);

    // main->AddSystematic(AtlTopLevelAnalysis::kWjetsGen); // 2 for WbbNp1 and WcNp1

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");
	
    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_tChannelMemDiscr");
    
    // ------------------------------------------------------------
    // Check tools with task_tChannelAnalysis_Main - Signal region!
    // ------------------------------------------------------------
    
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
    // main->SetPlotterScheme("SgTop_s_wjetsCB"); //Ttbar or SgTop
    // main->SetPlotterScheme("SgTop_s_wjetsMerged"); //Ttbar or SgTop
    // main->SetPlotterScheme("detail"); //Ttbar or SgTop
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
