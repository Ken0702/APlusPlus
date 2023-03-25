{
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("SgTop t-Channel",
				  "Single-Top t-Channel Analysis");
    // AtlTopLevelAnalysis options
    main->SetForceRetry(kTRUE);
    main->SetBuildHforTree(kTRUE);
    main->SetUseHforSamples(kTRUE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetSgTopVersion("v6");
    main->SetDSIDList("InputList.txt");
    main->ReadSampleList("SampleList_SgTopv6_ms.dat");

    // Setup Directories
    main->SetHistDir("/users/eeh/stamm/atlas/APlusPlus/libSingleTop/hist");
    main->SetJobHomeDir("/rdsk/datm8/atlas/stamm/");
    main->SetTempDir("/datc/atlas/stamm/");
    
    // Set jet bins
    // main->AddJetMultiplicity(AtlSelector::kOneJet);
    main->AddJetMultiplicity(AtlSelector::kTwoJet);
    // main->AddJetMultiplicity(AtlSelector::kThreeJet);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);

    // Set systematics
    main->AddSystematic(AtlTopLevelAnalysis::kAllSyst);
    //main->AddSystematic(AtlTopLevelAnalysis::kNominal);
    //main->AddSystematic(AtlTopLevelAnalysis::kJES_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJES_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJER_DOWN);


    // Setup Analysis
    // ------------------------------------------
    // 1. User environments:
    main->AddUserEnv("LIBSINGLETOP");

    // 2. Analysis Selector + Cuts
    main->SetSelector("Atl_tChannelAnalysis");
    // main->SetCut("fJetEta_min","1.5");
    // main->SetCut("fJetEta_max","2.74");
    main->SetCut("fJetsAuthor","AtlJet::kAntiKt4LCTopo");
    main->SetCut("fMET_Mag_min", "30");
    
    // 3. Analysis Tools (Cuts and Parameters)

    // Append PDFReweightingTool
    // =========================
    AtlPdfReweightingTool *pdf_tool = new AtlPdfReweightingTool("AtlPdfReweightingTool",
								"Pdf Reweighting Tool for Pdf Systematics");
    main->AddTool(pdf_tool);
    
    // Append CutFlowTool
    // ==================
    AtlCutFlowTool *CF_tool = new AtlCutFlowTool("AtlCutFlowTool","Cut-flow Tool");
    CF_tool->IsA();
    main->AddTool(CF_tool);
    main->SetToolCut("AtlCutFlowTool", "fLabelsDeflate", "kFALSE");
    main->SetToolCut("AtlCutFlowTool", "fNBins", "14");
    main->SetToolCut("AtlCutFlowTool", "fBinLabels", "\"No Cuts (D3PD),Etmiss > 30 GeV,Signal Lepton,No. of Jets #geq 2,No. of Jets #leq 2,Mt(W) > 30 GeV,b-Jet,1 b-Tag,H_{T} > 160 GeV,Convergence,t#bar{t} Veto,W_{had} Veto,P(#chi^{2}) > 0.05%\"");

    // For cut-based analysis
    // WHY DO I NEED A SECOND CUTFLOW TOOL IF I AM NOT USING IT!!!
    AtlCutFlowTool *CF_tool_2 = new AtlCutFlowTool("AtlCutFlowTool2","Cut-flow Tool 2");
    main->SetToolCut("AtlCutFlowTool2", "fLabelsDeflate", "kFALSE");
    main->SetToolCut("AtlCutFlowTool2", "fNBins", "13");
    main->AddTool(CF_tool_2);

    // Add TopPairFinder tool
    // ======================    
    AtlTopPairFinder *TopPairKinFit = new AtlTopPairFinder("AtlTopPairFinder",
     							   "Top Pair Reconstruction by KinFitter");
    
    main->SetToolCut("AtlTopPairFinder", "fTmassconstraint", "AtlTopPairFinder::kSameTmass");
    main->SetToolCut("AtlTopPairFinder", "fApplyBTagInReco", "kTRUE");
    main->SetToolCut("AtlTopPairFinder", "fBJets_N_Min",     "1");
    main->SetToolCut("AtlTopPairFinder", "fBJetTagger",      "AtlBTag::kMV1");
    // // SetToolCut("AtlTopPairFinder", "fBJetWeightMin",   "0.905363");
    main->SetToolCut("AtlTopPairFinder", "fMin_KFJet_Pt",    "30."); // was 25.
    main->SetToolCut("AtlTopPairFinder", "fMax_KFJet_Eta",   "2.5");
    main->SetToolCut("AtlTopPairFinder", "fIterMax",         "100");
    main->SetToolCut("AtlTopPairFinder", "fAlwaysFillHistos","kTRUE");
    main->SetToolCut("AtlTopPairFinder", "fMinChi2Prob",     "0.001");
    // // NOT 1000., but in sync with tChannelFinder instead:
    main->SetToolCut("AtlTopPairFinder", "fJets_N_Max", "2"); 
    main->AddTool(TopPairKinFit);

        

    // ============================================
    // Add KinFitter-mode of sgtop t-channel finder
    // ============================================
    AtlSgTop_tChannelFinder *finder = new AtlSgTop_tChannelFinder("AtlSgTop_tChannelFinder","Single-Top t-Channel Reconstruction by Finder Tool");
    main->AddTool(finder);
    
    // Running over signal MC?
    main->SetToolCut("AtlSgTop_tChannelFinder", "fSignalSampleMode", "kTRUE"); // ???
    main->SetToolCut("AtlSgTop_tChannelFinder", "fIsAcceptanceChallenge", "kFALSE"); // Doing acceptance challenge? ???
// not working    main->SetToolCut("AtlSgTop_tChannelFinder", "fVerbosityLevel", "0"); // ??? // Set finder's verbosity level
    main->SetToolCut("AtlSgTop_tChannelFinder", "fMaxNbIter", "100"); // ???
    
    // Switch on needed vetos
    main->SetToolCut("AtlSgTop_tChannelFinder", "fDoWhadVeto","kTRUE");
    main->SetToolCut("AtlSgTop_tChannelFinder", "fDoTtbarVeto","kTRUE");
    
    // Set P(chi2) cuts for top fit and Whad veto
    main->SetToolCut("AtlSgTop_tChannelFinder", "fChi2ProbMin", "0.05");
    main->SetToolCut("AtlSgTop_tChannelFinder", "fVetoWhadProbMin", "0.05");

    // Set further options
    main->SetToolCut("AtlSgTop_tChannelFinder", "fGuessNeutrinoEtaE", "kFALSE");
    main->SetToolCut("AtlSgTop_tChannelFinder", "fTruthMatch", "kFALSE");

    // Minimum W transverse mass
    main->SetToolCut("AtlSgTop_tChannelFinder", "fWmassT_min", "30");


    // cut-based analysis cuts
    // SetToolCut("AtlSgTop_tChannelFinder", "fLeadNonBJetEtaMin","2."); // ???
    main->SetToolCut("AtlSgTop_tChannelFinder", "fH_TMin","160."); // ???
    main->SetToolCut("AtlSgTop_tChannelFinder", "fDeltaEtaBJetNonBJetMin","0.8"); // ???
    main->SetToolCut("AtlSgTop_tChannelFinder", "fMassAllJetsMin","450."); // ???
    
    // Jet bins
    main->SetToolCut("AtlSgTop_tChannelFinder","fNJetsMin", "2"); // move to toplevel
    main->SetToolCut("AtlSgTop_tChannelFinder","fNJetsMax", "3"); // move to toplevel
    
    
    // B-jet cuts (in finder)
    main->SetToolCut("AtlSgTop_tChannelFinder","fBJetTagger","AtlBTag::kMV1");
//    main->SetToolCut("AtlSgTop_tChannelFinder","fBJetWeightMin","0.905363");
    main->SetToolCut("AtlSgTop_tChannelFinder","fBJets_Et_min","30.0");
    main->SetToolCut("AtlSgTop_tChannelFinder","fBJets_Eta_min","-2.5");
    main->SetToolCut("AtlSgTop_tChannelFinder","fBJets_Eta_max","2.5");

    // Whad jets (in finder)
    main->SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Et_min", "30.0"); // JES only up to 25 GeV...
    main->SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Eta_min","-2.0"); // was 2.5 (both)
    main->SetToolCut("AtlSgTop_tChannelFinder","fWhadJets_Eta_max","2.0");  // here, one could try to lower further (|eta| < 1.5 etc. --  needs some study)
    
    // Fwd jets (in finder)
    main->SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Et_min","30.0");
    main->SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Eta_min","2.5");
    main->SetToolCut("AtlSgTop_tChannelFinder","fFwdJets_Eta_max","5.");
    main->SetToolCut("AtlSgTop_tChannelFinder","fFwdJetTagger","AtlBTag::kSV0");
    main->SetToolCut("AtlSgTop_tChannelFinder","fFwdBJetWeightMax","5.85");


    // PdfReweightingTool
    main->SetToolCut("AtlPdfReweightingTool", "fSystMode", "AtlPdfReweightingTool::kNominal"); 
    // ??? fix this !!!!!!!!!!!!!!!

    // main->Print();
    main->BuildTree( kTRUE );
}
