{
    //____________________________________________________________________
    //
    // MEM Plotting script main configuration
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!   Do not execute this file   !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    // This file is used by task_memplotting_Signal.C and the
    // corresponding scripts for the ctrl regions.
    //
    // Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Soeren Stamm
    //   

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "MEM Likelihood Plotting");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(BuildHforTree);
    main->SetBuildAnalysisTree(kFALSE);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    main->SetUseHforSamples(kTRUE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetSgTopVersion("v11");
    main->SetDSIDList("SgTopDSIDs.txt");
    main->ReadSampleList();

    // Grid config
    main->SetGridRootVersion("5.34/12");
    main->SetGridCmtVersion("x86_64-slc5-gcc43-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v1");
    main->SetGridJob(kFALSE);

    // Setup Directories
    main->SetOutputFilePrefix("ntup_schan");
    
    if ( Selection == kSignalRegion ) {
	main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/sig/");
	main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/sig/");
	main->SetTempDir("/datc/atlas/stamm/sig/MemTkAnalysis");
    } else if ( Selection == kWjetsCtrlRegion ) {
	main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_Wjets");
	main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/ctrl_Wjets");
	main->SetTempDir("/datc/atlas/stamm/ctrl_Wjets");
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_Ttbar");
	main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/ctrl_Ttbar");
	main->SetTempDir("/datc/atlas/stamm/ctrl_Ttbar");
    } else {
	cout << "Wrong configuration. Abort!" << endl;
	gSystem->Abort(0);
    }

    // For MemTk Plotting
    // (i.e. "histdir + main->GetName() + BDTAnalysis")
    //
    // Name of 'main' must be the same as task_sChannelAnalysis_Main.C
    main->SetPlotDir(Form("%s/%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName(),
			   "MemTkAnalysis"));
        
    // Set jet bins
    if ( Selection == kSignalRegion
	 || Selection == kWjetsCtrlRegion ) { 
	main->AddJetMultiplicity(AtlSelector::kTwoJet);
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->AddJetMultiplicity(AtlSelector::kFourJet);
    }
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);

    // Set systematics
    // main->AddSystematic(AtlTopLevelAnalysis::kAllSyst);
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_s"); // Ttbar or SgTop_s|t
    main->SetDataLuminosity(20276.900); // pb^1
    main->SetAtlasLabelStatus("Work in Progress"); 
    main->SetCenterOfMassEnergyLabel("8");
    main->SetLuminosityLabel("20 fb^{-1}");
    main->SetDrawDataZeroEntryErrors("kFALSE");
    main->SetDrawMCError("kTRUE");
    main->SetExportPDF("kFALSE");

    // ====================
    // Ntuple plot commands
    // ====================
    // Tree, VarExp, Selection, PlotName, PlotTitle, Binning, XTitle, YTitle
    TString EvtWeight = "evt_weight*(jet_pt[0]/1000. > 30. && jet_pt[1]/1000. > 30.)";

    main->AddNtuplePlotCmd("physics", "bdt_MtW", EvtWeight.Data(),
			   "p_bdt_MtW", "Single-top t-Channel",
			   50, 0, 200, "MtW (GeV)", "Number of Entries");

    
    // -----------
    // Likelihoods
    // -----------

    if ( Selection == kSignalRegion ) {
	// t-channel
	main->AddNtuplePlotCmd("physics", "llh_tChannel4FS", EvtWeight.Data(),
			       "p_llh_tChannel4FS", "Single-top t-Channel",
			       35, 0., 0.002, "L_{t}", "Number of Entries");
	
	// s-channel
	main->AddNtuplePlotCmd("physics", "llh_sChannel2j", EvtWeight.Data(),
			       "p_llh_sChannel2j", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s(2j)}", "Number of Entries");
	
	main->AddNtuplePlotCmd("physics", "llh_sChannel3j", EvtWeight.Data(),
			       "p_llh_sChannel3j", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s(3j)}", "Number of Entries");

	main->AddNtuplePlotCmd("physics", "llh_sChannel2j+llh_sChannel3j", EvtWeight.Data(),
			       "p_llh_sChannel", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s(2j)}+L_{s(3j)}", "Number of Entries");

	// ttbar
	main->AddNtuplePlotCmd("physics", "llh_ttbarSL", EvtWeight.Data(),
			       "p_llh_ttbarSL", "t#bar{t} Production",
			       35, 0., 0.0001, "L_{t#bar{t}}", "Number of Entries");
	
	// W2j (lf)
	main->AddNtuplePlotCmd("physics", "llh_Wjj", EvtWeight.Data(),
			       "p_llh_Wjj", "Wjj",
			       35, 0., 0.01, "L_{Wjj}", "Number of Entries");
	
	// Wbb
	main->AddNtuplePlotCmd("physics", "llh_Wbb", EvtWeight.Data(),
			       "p_llh_Wbb", "Wbb",
			       35, 0., 0.005, "L_{Wb#bar{b}}", "Number of Entries");

	// Wbb + Wjj
    	main->AddNtuplePlotCmd("physics", "llh_Wbb+llh_Wjj", EvtWeight.Data(),
			       "p_llh_WbbWjj", "Wbb + Wjj",
			       35, 0., 0.015, "L_{Wb#bar{b}}+L_{Wjj}", "Number of Entries");
    } else {
	// t-channel
	main->AddNtuplePlotCmd("physics", "llh_tChannel4FS", EvtWeight.Data(),
			       "p_llh_tChannel4FS", "Single-top t-Channel",
			       35, 0., 0.002, "L_{t}", "Number of Entries");
	
	// s-channel
	main->AddNtuplePlotCmd("physics", "llh_sChannel2j", EvtWeight.Data(),
			       "p_llh_sChannel2j", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s}", "Number of Entries");
	
	main->AddNtuplePlotCmd("physics", "llh_sChannel3j", EvtWeight.Data(),
			       "p_llh_sChannel3j", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s}", "Number of Entries");
	
	main->AddNtuplePlotCmd("physics", "llh_sChannel2j+llh_sChannel3j", EvtWeight.Data(),
			       "p_llh_sChannel", "Single-top s-Channel",
			       35, 0., 0.0002, "L_{s(2j)}+L_{s(3j)}", "Number of Entries");

	if ( Selection == kWjetsCtrlRegion ) {
	    // ttbar
	    main->AddNtuplePlotCmd("physics", "llh_ttbarSL", EvtWeight.Data(),
				   "p_llh_ttbarSL", "ttbar Production",
				   35, 0., 0.0001, "L_{t#bar{t}}", "Number of Entries");
	} else {
	    // ttbar
	    main->AddNtuplePlotCmd("physics", "llh_ttbarSL", EvtWeight.Data(),
				   "p_llh_ttbarSL", "ttbar Production",
				   35, 0., 1.e-16, "L_{t#bar{t}}", "Number of Entries");
	}
	
	// W2j (lf)
	main->AddNtuplePlotCmd("physics", "llh_Wjj", EvtWeight.Data(),
			       "p_llh_Wjj", "Wjj",
			       35, 0., 0.01, "L_{Wjj}", "Number of Entries");
	
	// Wbb
	main->AddNtuplePlotCmd("physics", "llh_Wbb", EvtWeight.Data(),
			       "p_llh_Wbb", "Wbb",
			       35, 0., 0.005, "L_{Wb#bar{b}}", "Number of Entries");

	// Wbb + Wjj
    	main->AddNtuplePlotCmd("physics", "llh_Wbb+llh_Wjj", EvtWeight.Data(),
			       "p_llh_WbbWjj", "Wbb + Wjj",
			       35, 0., 0.015, "L_{Wb#bar{b}}+L_{Wjj}", "Number of Entries");
    }

    // Log likelihoods
    
    // t-channel
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_tChannel4FS)", EvtWeight.Data(),
			   "p_log_llh_tChannel4FS", "Single-top t-Channel",
			   100, -40., 1., "Log_{10}(L_{t})", "Number of Entries");
    
    // s-channel
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_sChannel2j)", EvtWeight.Data(),
			   "p_log_llh_sChannel2j", "Single-top s-Channel",
			   100,-40., 1., "Log_{10}(L_{s})", "Number of Entries");
    
    main->AddNtuplePlotCmd("physics", "TMath::Log(llh_sChannel3j)", EvtWeight.Data(),
			   "p_log_llh_sChannel3j", "Single-top s-Channel",
			   100,-40., 1., "Log_{10}(L_{s})", "Number of Entries");
    
    main->AddNtuplePlotCmd("physics", "TMath::Log(llh_sChannel2j+llh_sChannel3j)", EvtWeight.Data(),
			   "p_log_llh_sChannel", "Single-top s-Channel",
			   100,-40., 1., "Log_{10}(L_{s(2j)}+L_{s(3j)})", "Number of Entries");
    
    // Ttbar
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_ttbarSL)", EvtWeight.Data(),
			   "p_log_llh_ttbarSL", "ttbar Production",
			   100,-40., 1., "Log_{10}(L_{t#bar{t}})", "Number of Entries");
    
    // W2j (lf)
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_Wjj)", EvtWeight.Data(),
			   "p_log_llh_Wjj", "Wjj",
			   100,-40., 1., "Log_{10}(L_{Wjj})", "Number of Entries");
    
    // Wbb
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_Wbb)", EvtWeight.Data(),
			   "p_log_llh_Wbb", "Wbb",
			   100,-40., 1., "Log_{10}(L_{Wb#bar{b}}=", "Number of Entries");
    
    // Wbb + Wjj
    main->AddNtuplePlotCmd("physics", "TMath::Log10(llh_Wbb+llh_Wjj)", EvtWeight.Data(),
			   "p_log_llh_WbbWjj", "Wbb + Wjj",
			   100,-40., 1., "Log_{10}(L_{Wb#bar{b}}+L_{Wjj})", "Number of Entries");
    
    
    // ----------
    // Ctrl Plots
    // ----------

    // MET
    main->AddNtuplePlotCmd("physics", "MET_et/1000.", EvtWeight.Data(),
			   "h_mem_met", "Missing E_{T}, pretag",
			   40, 0., 200., "E_{T, miss} (GeV)", "Number of Entries");

    // Jets
    //
    // 1. Jet pt
    main->AddNtuplePlotCmd("physics", "jet_pt[0]/1000.", EvtWeight.Data(),
			   "h_mem_LeadingJet_pt", "Leading Jet p_{T}",
			   44, 0., 220., "p_{T} (GeV)", "Number of Entries");
    // 1. Jet eta
    main->AddNtuplePlotCmd("physics", "jet_eta[0]", EvtWeight.Data(),
			   "h_mem_LeadingJet_eta", "Leading Jet #eta",
			   25, -5., 5., "#eta", "Number of Entries");
    // 1. Jet phi
    main->AddNtuplePlotCmd("physics", "jet_phi[0]", EvtWeight.Data(),
			   "h_mem_LeadingJet_phi", "Leading Jet #phi",
			    20, -TMath::Pi(), TMath::Pi(), "#phi", "Number of Entries");

    // 2. Jet pt
    main->AddNtuplePlotCmd("physics", "jet_pt[1]/1000.", EvtWeight.Data(),
			   "h_mem_2ndLeadingJet_pt", "Leading Jet p_{T}",
			   44, 0., 220., "p_{T} (GeV)", "Number of Entries");
    // 2. Jet eta
    main->AddNtuplePlotCmd("physics", "jet_eta[1]", EvtWeight.Data(),
			   "h_mem_2ndLeadingJet_eta", "Leading Jet #eta",
			   25, -5., 5., "#eta", "Number of Entries");
    // 2. Jet phi
    main->AddNtuplePlotCmd("physics", "jet_phi[1]", EvtWeight.Data(),
			   "h_mem_2ndLeadingJet_phi", "Leading Jet #phi",
			    20, -TMath::Pi(), TMath::Pi(), "#phi", "Number of Entries");

    // Leptons
    // pt
    main->AddNtuplePlotCmd("physics", "el_pt[0]/1000.", EvtWeight.Data(),
    			   "h_mem_Electron_pt", "Leading Jet p_{T}",
    			   44, 0., 220., "p_{T} (GeV)", "Number of Entries");
    // eta
    main->AddNtuplePlotCmd("physics", "el_eta[0]", EvtWeight.Data(),
    			   "h_mem_Electron_eta", "Leading Jet #eta",
    			   25, -5., 5., "#eta", "Number of Entries");
    // phi
    main->AddNtuplePlotCmd("physics", "el_phi[0]", EvtWeight.Data(),
    			   "h_mem_Electron_phi", "Leading Jet #phi",
    			    20, -TMath::Pi(), TMath::Pi(), "#phi", "Number of Entries");

    // pt
    main->AddNtuplePlotCmd("physics", "mu_pt[0]/1000.", EvtWeight.Data(),
    			   "h_mem_Muon_pt", "Leading Jet p_{T}",
    			   44, 0., 220., "p_{T} (GeV)", "Number of Entries");
    // eta
    main->AddNtuplePlotCmd("physics", "mu_eta[0]", EvtWeight.Data(),
    			   "h_mem_Muon_eta", "Leading Jet #eta",
    			   25, -5., 5., "#eta", "Number of Entries");
    // phi
    main->AddNtuplePlotCmd("physics", "mu_phi[0]", EvtWeight.Data(),
    			   "h_mem_Muon_phi", "Leading Jet #phi",
    			    20, -TMath::Pi(), TMath::Pi(), "#phi", "Number of Entries");

    
    // -----------------
    // Likelihood ratios
    // -----------------

    // self-made
    
    // s-channel
    main->AddNtuplePlotCmd("physics",
			   "(llh_sChannel2j+llh_sChannel3j)/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_sChannel", "Single-top s-Channel ratio",
			   35, 0., 1., "L_{s}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // t-channel
    main->AddNtuplePlotCmd("physics",
			   "llh_tChannel4FS/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_tChannel", "Single-top t-Channel ratio",
			   35, 0., 1., "L_{t}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // ttbar
    main->AddNtuplePlotCmd("physics",
			   "llh_ttbarSL/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_ttbarSL", "ttbar ratio",
			   35, 0., 1., "L_{t#bar{t}}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // Wjj
    main->AddNtuplePlotCmd("physics",
			   "llh_Wjj/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_Wjj", "Wjj ratio",
			   35, 0., 1., "L_{Wjj}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // Wbb
    main->AddNtuplePlotCmd("physics",
			   "llh_Wbb/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_Wbb", "Wbb ratio",
			   35, 0., 1., "L_{Wb#bar{b}}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // W
    main->AddNtuplePlotCmd("physics",
			   "(llh_Wbb+llh_Wjj)/(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_Wjj+llh_Wbb+llh_ttbarSL)",
			   EvtWeight.Data(),
			   "Unweighted_llh_ratio_WbbWjj", "Wbb+Wjj ratio",
			   35, 0., 1., "(L_{Wjj}+L_{Wb#bar{b}}/(L_{s}+L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");


    // From BDT training:
    main->AddNtuplePlotCmd("physics", "(llh_sChannel2j+0.5*llh_sChannel3j)/(llh_sChannel2j+0.5*llh_sChannel3j+1.1*llh_tChannel4FS+0.06*llh_ttbarSL+0.08*llh_Wjj+0.07*llh_Wbb)", EvtWeight.Data(),
			   "BDT_ratio_schannel", "Weighted s-Channel ratio",
			   32, 0., 1., "", "Number of Entries");
    
    
    main->AddNtuplePlotCmd("physics", "TMath::Power(0.06*llh_ttbarSL/(llh_sChannel2j+0.5*llh_sChannel3j+1.1*llh_tChannel4FS+0.06*llh_ttbarSL+0.08*llh_Wjj+0.07*llh_Wbb),0.5)", EvtWeight.Data(),
			   "BDT_ratio_ttbarSL", "Weighted ttbarSL ratio",
			   32, 0., 1.2, "", "Number of Entries");
    
    main->AddNtuplePlotCmd("physics", "(0.08*llh_Wjj+0.07*llh_Wbb)/(llh_sChannel2j+0.5*llh_sChannel3j+1.1*llh_tChannel4FS+0.06*llh_ttbarSL+0.08*llh_Wjj+0.07*llh_Wbb)", EvtWeight.Data(),
			   "BDT_ratio_Wjj_Wbb", "Weighted Wjj/Wbb ratio",
			   32, 0., 1., "", "Number of Entries");
    
    main->AddNtuplePlotCmd("physics", "TMath::Power(1.1*llh_tChannel4FS/(llh_sChannel2j+0.5*llh_sChannel3j+1.1*llh_tChannel4FS+0.06*llh_ttbarSL+0.08*llh_Wjj+0.07*llh_Wbb),0.5)", EvtWeight.Data(),
			   "BDT_ratio_tChannel", "Weighted t-channel ratio",
			   32, 0., 1., "", "Number of Entries");
	
    // MEM ratios
    // t-channel, unweighted
    main->AddNtuplePlotCmd("physics", "llhratio_tChannel4FS", EvtWeight.Data(),
			   "p_llhratio_tChannel4FS", "Likelihood Ratio, t-Channel",
			   27, -0.04, 1.04,
			   "#it{t}-Channel Discriminant", "Number of Entries");

    // t-channel, weighted
    main->AddNtuplePlotCmd("physics", "llhratioW_tChannel4FS", EvtWeight.Data(),
			   "p_llhratioW_tChannel4FS", "Likelihood Ratio (weighted), t-Channel",
			   27, -0.04, 1.04,
			   "#it{t}-Channel Discriminant (weighted)",
			   "Number of Entries");

    // // t-channel, weighted + finer binning
    // main->AddNtuplePlotCmd("physics", "tChannelLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH)",
    // 			   "EvtWeight", "p_LLHratio_w_morebins", "Likelihood Ratio (weighted), t-Channel",
    // 			   102, -0.01, 1.01, "L_{t}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // t-channel, weighted + log
    // main->AddNtuplePlotCmd("physics",
    // 			   "TMath::Log10(tChannelLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH))",
    // 			   "EvtWeight",
    // 			   "p_LLH_ratio_w_log", "Likelihood Ratio (weighted), t-Channel",
    // 			   50, -5, 0, "log_{10} L_{t}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // t-channel, weighted + nlog
    // main->AddNtuplePlotCmd("physics",
    // 			   "TMath::Log10(1.-tChannelLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH))",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_w_nlog", "Likelihood Ratio (weighted), t-Channel",
    // 			   25, -5, 0,
    // 			   "log_{10} (1-L_{t}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}}))",
    // 			   "Number of Entries");

    // s-channel, unweighted
    main->AddNtuplePlotCmd("physics", "llhratio_sChannel2j", EvtWeight.Data(),
    			   "p_llhratio_sChannel2j", "Likelihood Ratio, s-Channel",
    			   27, -0.04, 1.04,
			   "#it{s}-Channel Discriminant", "Number of Entries");

    // s-channel, weighted
    main->AddNtuplePlotCmd("physics", "llhratioW_sChannel2j", EvtWeight.Data(),
    			   "p_llhratioW_sChannel2j", "Likelihood Ratio (weighted), s-Channel",
    			   27, -0.04, 1.04,
			   "#it{s}-Channel Discriminant (weighted)", "Number of Entries");

    
    // ttbar, unweighted
    main->AddNtuplePlotCmd("physics", "llhratio_ttbarSL", EvtWeight.Data(),
    			   "p_llhratio_ttbarSL", "Likelihood Ratio, ttbar",
    			   27, -0.04, 1.04,
			   "t#bar{t} Discriminant", "Number of Entries");

    // ttbar, weighted
    main->AddNtuplePlotCmd("physics", "llhratioW_ttbarSL", EvtWeight.Data(),
    			   "p_llhratioW_ttbarSL", "Likelihood Ratio (weighted), ttbar",
    			   27, -0.04, 1.04,
			   "t#bar{t} Discriminant (weighted)", "Number of Entries");
    
    // Wjj, unweighted
    main->AddNtuplePlotCmd("physics", "llhratio_Wjj", EvtWeight.Data(),
    			   "p_llhratio_Wjj", "Likelihood Ratio, Wjj",
    			   27, -0.04, 1.04,
			   "Wjj Discriminant", "Number of Entries");

    // Wjj, weighted
    main->AddNtuplePlotCmd("physics", "llhratioW_Wjj", EvtWeight.Data(),
    			   "p_llhratioW_Wjj", "Likelihood Ratio (weighted), Wjj",
    			   27, -0.04, 1.04,
			   "Wjj Discriminant (weighted)", "Number of Entries");


    // Wbb, unweighted
    main->AddNtuplePlotCmd("physics", "llhratio_Wbb", EvtWeight.Data(),
    			   "p_llhratio_Wbb", "Likelihood Ratio, Wbb",
    			   27, -0.04, 1.04,
			   "Wb#bar{b} Discriminant", "Number of Entries");

    // Wbb, weighted
    main->AddNtuplePlotCmd("physics", "llhratioW_Wbb", EvtWeight.Data(),
    			   "p_llhratioW_Wbb", "Likelihood Ratio (weighted), Wbb",
    			   27, -0.04, 1.04,
			   "Wb#bar{b} Discriminant (weighted)", "Number of Entries");




    // s-channel, weighted2
    main->AddNtuplePlotCmd("physics",
			   "(llh_sChannel2j+llh_sChannel3j)/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+0.5*llh_ttbarSL+0.05*llh_Wjj+0.05*llh_Wbb)",
//			   "(llh_sChannel2j+llh_sChannel3j)/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+llh_ttbarSL+llh_Wjj+llh_Wbb)",
//     			   Form("%s*(llh_sChannel2j+llh_sChannel3j)>=0.&&llh_tChannel4FS>=0.&&llh_ttbarSL>=0.&&llh_Wjj>=0.&&llh_Wbb>=0.", EvtWeight.Data()),
     			   Form("((llh_sChannel2j+llh_sChannel3j)>0.)*%s", EvtWeight.Data()),
			   "p_llhratioV_schan",
			   "Likelihood Ratio (weighted), s-Channel",
			    27, -0.04, 1.04,
			    "#it{s}-Channel Discriminant (weighted)",
     			   "Number of Entries");

    // t-channel, weighted2
    main->AddNtuplePlotCmd("physics",
			   "llh_tChannel4FS/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+0.5*llh_ttbarSL+0.05*llh_Wjj+0.05*llh_Wbb)",
     			   EvtWeight.Data(), "p_llhratioV_tchan",
			   "Likelihood Ratio (weighted), t-Channel",
			    27, -0.04, 1.04,
			    "#it{t}-Channel Discriminant (weighted)",
     			   "Number of Entries");
    
    // ttbar, weighted2
    main->AddNtuplePlotCmd("physics",
			   "llh_ttbarSL/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+0.5*llh_ttbarSL+0.05*llh_Wjj+0.05*llh_Wbb)",
     			   EvtWeight.Data(), "p_llhratioV_ttbar",
			   "Likelihood Ratio (weighted), ttbar",
			    27, -0.04, 1.04,
			    "t#bar{t} Discriminant (weighted)",
     			   "Number of Entries");

    // Wjj, weighted2
    main->AddNtuplePlotCmd("physics",
			   "llh_Wjj/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+0.5*llh_ttbarSL+0.05*llh_Wjj+0.05*llh_Wbb)",
     			   EvtWeight.Data(), "p_llhratioV_Wjj",
			   "Likelihood Ratio (weighted), Wjj",
			    27, -0.04, 1.04,
			    "Wjj Discriminant (weighted)",
     			   "Number of Entries");

    // Wbb, weighted2
    main->AddNtuplePlotCmd("physics",
			   "llh_Wbb/(llh_tChannel4FS+(llh_sChannel2j+llh_sChannel3j)+0.5*llh_ttbarSL+0.05*llh_Wjj+0.05*llh_Wbb)",
     			   EvtWeight.Data(), "p_llhratioV_Wbb",
			   "Likelihood Ratio (weighted), Wbb",
			    27, -0.04, 1.04,
			    "Wb#bar{b} Discriminant (weighted)",
     			   "Number of Entries");


    


    // // ttbar, unweighted
    // main->AddNtuplePlotCmd("physics", "ttbarLLH/(tChannelLLH+sChannelLLH+ttbarLLH+W2JetsLLH+WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_llhratio_ttbarSL", "Likelihood Ratio, t#bar{t}",
    // 			   27, -0.04, 1.04,
    // 			   "L_{t#bar{t}}/(L_{t}+L_{s}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})", "Number of Entries");

    // // W2j (lf), unweighted
    // main->AddNtuplePlotCmd("physics", "W2JetsLLH/(tChannelLLH+sChannelLLH+ttbarLLH+W2JetsLLH+WbbLLH)", "EvtWeight",
    // 			   "p_LLHratio_wjj", "Likelihood Ratio, Wjj",
    // 			   27, -0.04, 1.04, "L_{Wjj}/(L_{t}+L_{s}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // Wbb, unweighted
    // main->AddNtuplePlotCmd("physics", "WbbLLH/(tChannelLLH+sChannelLLH+ttbarLLH+W2JetsLLH+WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_wbb", "Likelihood Ratio, Wb#bar{b}",
    // 			   27, -0.04, 1.04,
    // 			   "L_{Wb#bar{b}}/(L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // Wjets = W2j + Wbb, unweighted
    // main->AddNtuplePlotCmd("physics", "(W2JetsLLH+WbbLLH)/(tChannelLLH+sChannelLLH+ttbarLLH+W2JetsLLH+WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_wjets", "Likelihood Ratio, Wjj+Wb#bar{b}",
    // 			   27, -0.04, 1.04,
    // 			   "(L_{Wjj}+L_{Wb#bar{b}})/(L_{t}+L_{t#bar{t}}+L_{Wjj}+L_{Wb#bar{b}})",
    // 			   "Number of Entries");
    
    // // ttbar, weighted
    // main->AddNtuplePlotCmd("physics", "0.5*ttbarLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_llhratio_ttbarSL_w", "Likelihood Ratio (weighted), t#bar{t}",
    // 			   27, -0.04, 1.04,
    // 			   "0.5L_{t#bar{t}}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // W2j (lf), weighted
    // main->AddNtuplePlotCmd("physics", "0.05*W2JetsLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_wjj_w", "Likelihood Ratio (weighted), Wjj",
    // 			   27, -0.04, 1.04,
    // 			   "0.05L_{Wjj}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // Wbb, weighted
    // main->AddNtuplePlotCmd("physics", "0.05*WbbLLH/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_wbb_w", "Likelihood Ratio (weighted), Wb#bar{b}",
    // 			   27, -0.04, 1.04,
    // 			   "0.05L_{Wb#bar{b}}/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");

    // // Wjets = W2j + Wbb, weighted
    // main->AddNtuplePlotCmd("physics",
    // 			   "0.05*(W2JetsLLH+WbbLLH)/(tChannelLLH+sChannelLLH+0.5*ttbarLLH+0.05*W2JetsLLH+0.05*WbbLLH)",
    // 			   "EvtWeight",
    // 			   "p_LLHratio_wjets_w", "Likelihood Ratio (weighted), Wjj+Wb#bar{b}",
    // 			   27, -0.04, 1.04,
    // 			   "0.05(L_{Wjj}+L_{Wb#bar{b}})/(L_{t}+L_{s}+0.5L_{t#bar{t}}+0.05L_{Wjj}+0.05L_{Wb#bar{b}})",
    // 			   "Number of Entries");
    
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
