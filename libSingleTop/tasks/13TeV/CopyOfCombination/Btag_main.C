{
    //____________________________________________________________________
    //
    // SgTop combination analysis main configuration
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!   Do not execute this file   !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    // This file is used by task_combination_SR.C and the
    // corresponding scripts for the ctrl regions.
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id: task_analysis_main.C,v 1.2 2016/04/19 08:42:03 stamm Exp $
    // Copyright: 2014 (C) Oliver Maria Kind
    //

    // original file: /users/eeh/stamm/atlas/APlusPlusIntNote_v15/libSingleTop/tasks/8TeV/s_channel/task_analysis_main.C
    // S.K. To do: consider renaming directory setter functions

    // ========================
    // Configure top-level task
    // ========================

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildAnalysisTree(BuildAnalysisTree);
    main->SetBuildMemDiscAnalysisTree(BuildMemDiscAnalysisTree); // memdiscr
    main->SetBuildHforTree(BuildHforTree);
    main->SetBuildMemTkAnalysisTree(BuildMemTkAnalysisTree);
    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    main->SetTtbarIsSplitted(UseTtbarSplit);
    main->SetUseHforSamples(kFALSE);

    // Grid config
    main->SetGridRootVersion("6.08/02");
    main->SetGridCmtVersion("x86_64-slc6-gcc49-opt");
    main->SetGridUserName("kkreul");
    main->SetGridIdSuffix(GridIdSuffix);
    main->SetGridSuffix(GridSuffix);
    main->SetGridJob(GridJob);
//    main->SetGridHomeSetupScript("/mnt/eeh/kaphle/atlas/grid_home/setup_grid_home.sh");

    // Tree name: systematics part is done automatically in A++ for Cut; MEM uses "physics"
    if ( mode == kCut ) {
        main->SetTreeName("");
    } else {
        main->SetTreeName("physics");
    }
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);

    // cross section file: does not change except to include more DSIDs
    main->SetCrossSectionFilePath("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/AnalysisTop/TopDataPreparation/XSection-MC15-13TeV.data");
    // main->SetCrossSectionFilePath("/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisTop/2.4.32/TopDataPreparation/data/XSection-MC15-13TeV.data");

    // Lumi in pb^{-1} for p3794, v29
    // Data 2015: 3219.56 pb⁻¹ ± 2.1%
    // Data 2016: 32988.1 pb⁻¹ ± 2.2%
    // Data 2017: 44307.4 pb⁻¹ ± 2.4%
    // Data 2018: 58450.1 pb⁻¹ ± 2.0%
    // NEW Data 2015-2018: 138965.16 pb⁻¹ ± 1.7% (unc. info.).

    TString DSIDList("../SampleList/SgTopDSIDs_");
    // Set ntuple versions
    switch ( sgtop_v ) {
    case k15lj:
        main->SetCampaign(15);
        main->SetDataLuminosity(36470.16); // pb^{-1}
        main->SetSgTopVersion("v15_lj");
        DSIDList.Append("mc15_v15_lj.txt");
        break;
    case k20lj:
        main->SetCampaign(16);
        main->SetSgTopVersion("v20_lj");
        DSIDList.Append("mc16_v20_lj.txt");
        break;
    case k21lj:
    case k23lj:
    case k24lj:
    case k25lj:
    case k29lj:
        main->SetCampaign(16);
        main->SetSgTopVersion(ST_v_str[sgtop_v]);
        switch(mc_v) {
        case kMC16a:
            DSIDList.Append(Form("mc16a_%s.txt", ST_v_str[sgtop_v]));
            main->SetDataLuminosity(36184.86);
            if ( sgtop_v == k29lj )
                main->SetDataLuminosity(3219.56 + 32988.1);
            break;
        case kMC16d:
            DSIDList.Append(Form("mc16d_%s.txt", ST_v_str[sgtop_v]));
            main->SetDataLuminosity(43813.7);
            if ( sgtop_v == k29lj )
                main->SetDataLuminosity(44307.4);
            break;
        case kMC16e:
            DSIDList.Append(Form("mc16e_%s.txt", ST_v_str[sgtop_v]));
            main->SetDataLuminosity(58450.1);
            break;
        default:
            DSIDList.Append(Form("mc16_%s.txt", ST_v_str[sgtop_v]));
            main->SetDataLuminosity(36184.86 + 43813.7);
            if ( sgtop_v == k29lj )
                main->SetDataLuminosity(3219.56 + 32988.1 + 43813.7 + 58450.1);
            break;
        }
        main->SetLooseTreeOnly();
        break;
    case k31lj:
    case k32lj:
    	//DSID List is set in SR script...
    	main->SetCampaign(16);
    	main->SetSgTopVersion(ST_v_str[sgtop_v]);

    	switch(mc_v) {
    	        case kMC16a:
    	            DSIDList.Append(Form("mc16a_%s.txt", ST_v_str[sgtop_v]));
    	            main->SetDataLuminosity(36184.86);
    	            if ( sgtop_v == k31lj || sgtop_v == k32lj )
    	                main->SetDataLuminosity(3219.56 + 32988.1);
    	            break;
    	        case kMC16d:
    	            DSIDList.Append(Form("mc16d_%s.txt", ST_v_str[sgtop_v]));
    	            //main->SetDataLuminosity(43813.7);
    	            if ( sgtop_v == k31lj || sgtop_v == k32lj)
    	                main->SetDataLuminosity(44307.4);
    	            break;
    	        case kMC16e:
    	            DSIDList.Append(Form("mc16e_%s.txt", ST_v_str[sgtop_v]));
    	            main->SetDataLuminosity(58450.1);
    	            break;
    	        default:
    	            DSIDList.Append(Form("mc16_%s.txt", ST_v_str[sgtop_v]));
    	            main->SetDataLuminosity(36184.86 + 43813.7);
    	            if ( sgtop_v == k31lj || sgtop_v == k32lj)
    	                main->SetDataLuminosity(3219.56 + 32988.1 + 43813.7 + 58450.1);
    	            break;
    	        }

    	main->SetLooseTreeOnly();
        break;
    default:
        Error(__FUNCTION__, "Invalid ntuple version. Abort!");
        gSystem->Abort();
    }

    if ( UseTtbarSplit )
        DSIDList.ReplaceAll(".txt", "_ttbarSplit.txt");
    main->SetDSIDList(DSIDList.Data());

    // Setup (local) directories
    // InputDir: irrelevant; gets done with SampleList;input for plots (chose mem/analysis above)
    // PlotDir: input directory for plots
    TString inputdir = Form("%s/%s", histdir, main->GetName());
    TString plotdir = Form("%s/%s", histdir, main->GetName());
//    if ( !force_plotdir_append.IsNull() )
//        plotdir.Append(force_plotdir_append.Data());
    main->SetHistDir(histdir);
    main->SetTempDir(tempdir);
    if ( mode == kMemDisc ) {
        inputdir.Append("/MemTkAnalysis");
        plotdir.Append(Form("/%s", memllhdir));
    }
    main->SetJobHomeDir(jobdir);
    main->SetInputDir(inputdir.Data());
    main->SetPlotDir(plotdir.Data());

    // Set output tree and format
    if ( UseOutputTree )
        main->SetOutputTree("physics", "SgTopD3PD t-channel SR");

    // Set output file prefix
    main->SetOutputFilePrefix("ntup_schan");

    // user envs
    main->AddUserEnv("LHAPDF6");

    // ===============
    // Set systematics
    // ===============
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpQCD);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);

    // ==============
    // Setup Analysis
    // ==============

    // Set jet bins
    main->AddJetMultiplicity(jetbin);

    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);

    // Select selector
    if ( mode == kMemDisc ) {
        main->SetSelector("AtlSgTop_combinationMemDiscR2");
    } else {
        main->SetSelector("AtlSgTop_combinationAnalysis");
    }

    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj =
        new AtlObjectsToolD3PDSgTop("D3PDObjects","Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor",
                           "AtlJet::kAntiKt4TopoEM");
    main->SetToolParameter("D3PDObjects", "fAddToCutFlow", "kTRUE");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.0");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fLeptonN_min", "1");
    main->SetToolParameter("D3PDObjects", "fLeptonN_max", "1");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kDL1r_85");
    //main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_85"); //MV"C10
    //main->SetToolParameter("D3PDObjects", "fBTagWeight_min", "0.94");

    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf =
        new AtlObjRecoScaleFactorTool("SFTool", "SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fOperationMode", "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!
    //main->SetToolParameter("SFTool", "fOperationMode", "AtlObjRecoScaleFactorTool::kNoSF");
    main->SetToolParameter("SFTool", "fUseCDI", "kFALSE");

    main->SetToolParameter("SFTool", "fBTagWP", "AtlObjRecoScaleFactorTool::kDL1r_85");
    //main->SetToolParameter("SFTool", "fBTagWP", "AtlObjRecoScaleFactorTool::kMV2c10_85");

    // Cut-flow Tool
    AtlCutFlowTool *tool_cf = new AtlCutFlowTool("CFTool", "Cutflow Tool");
    main->AddTool(tool_cf);
    main->SetToolParameter("CFTool", "fNBins", "20");
    main->SetToolParameter("CFTool", "fLabelsDeflate", "kFALSE");

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme(plotter_scheme); //Ttbar or SgTop
    if ( force_lumi != 0. )
        main->SetDataLuminosity(force_lumi);
    main->SetAtlasLabelStatus("Work in Progress");
    main->SetCenterOfMassEnergyLabel("13");
    main->SetLuminosityLabel("36.5 fb^{-1}");
    main->SetUseOverflowUnderflow(kTRUE);
    main->SetDrawDataZeroEntryErrors(kFALSE);
    main->SetDrawMCError(kTRUE);
    main->SetExportPDF(kFALSE);
    main->SetDrawSignalOverlay(kFALSE);
    main->SetUsePlotterSchemeDir(UsePlotterSchemeDir);
}
