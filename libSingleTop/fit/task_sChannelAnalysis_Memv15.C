{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis Hist factory configuration
    //
    // Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2015 (C) Soeren Stamm
    //   

    using namespace AtlTopLevelAnalysis;

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");
    
    // AtlTopLevelAnalysis options
    main->SetBuildHistFactoryTree(kTRUE);

    // TString OutputDir = Form("/rdsk/dats2/atlas/%s/HistFactory/Mem_v15_intnote_inclallSyst/",
    //  			     gSystem->Getenv("USER"));
    // TString OutputDir = Form("/rdsk/dats2/atlas/%s/HistFactory/TEST4/",
    //  			     gSystem->Getenv("USER"));
    TString OutputDir = Form("/rdsk/dats2/atlas/%s/HistFactory/INT_sChanME/v15f/DiscBins2N/",
     			     gSystem->Getenv("USER"));
    
    main->SetJobHomeDir(Form("%s/JobHome", OutputDir.Data()));
    
    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("sChannel_Analysis",
								    "s-Channel Analysis incl. Fit",
								    OutputDir.Data());
	meas->AddPOI("sChannel_mu"); // This needs to be a parameter set by a sample!
    // Histograms are already scaled to luminosity!
    // --> Luminosity is 1, only provide relativ error
    meas->SetLumiAndRelError(1., 0.028);
    // Shape tests using pseudo exp?
    meas->SetUsePseudoExp(kTRUE);
    // Export shape comparison plots to pdf?
    meas->SetExportShapePlots(kFALSE);
    // AsimovData?
    meas->SetUseAsimovData(false);
    // Pull Plots
    // meas->SetRunPullPlots(true);
    
    // Create list of systematics for all channels (lnu)
    TList *Systs = new TList;
    // Nominal
    Systs->Add(new AtlHistFactorySystNominal("Nominal"));
    // Default systematics
    Systs->Add(new AtlHistFactorySystOneSided("Jeff", "Jet reco. eff.", kJEFF, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystOneSided("Jer", "Jet energy res.", kJER, AtlHistFactorySystOneSided::kFullDiff));
    // Systs->Add(new AtlHistFactorySystPair("JES", "Jet energy scale", kJES_UP, kJES_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JVF", "Jet vertex fraction", kJVF_UP, kJVF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("RES_SOFT", "Etmiss res.",  kRES_SOFT_UP, kRES_SOFT_DOWN));
    Systs->Add(new AtlHistFactorySystPair("SC_SOFT", "Etmiss soft", kSC_SOFT_UP, kSC_SOFT_DOWN));
    // Lepton specific systematics
    Systs->Add(new AtlHistFactorySystPair("EER", "El. energy res.", kEER_UP, kEER_DOWN));
    Systs->Add(new AtlHistFactorySystPair("EES", "El. energy scale.", kEES_UP, kEES_DOWN));
    Systs->Add(new AtlHistFactorySystOneSided("MuId", "Muon ID", kMUID_RES, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystOneSided("MuMS", "Muon mom. res.", kMUMS_RES, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystPair("MUSC", "Muon mom. scale", kMUSC_UP, kMUSC_DOWN));
    // Jes components
    Systs->Add(new AtlHistFactorySystPair("BJES", "BJES", kBJES_UP, kBJES_DOWN));
    Systs->Add(new AtlHistFactorySystPair("EtaIntercalibModel", "Eta intercalib. model", kETAINTERCALIB_MODEL_UP, kETAINTERCALIB_MODEL_DOWN));
    Systs->Add(new AtlHistFactorySystPair("EtaIntercalibTotalStat", "Eta intercal. stat.", kETAINTERCALIB_TOTALSTAT_UP, kETAINTERCALIB_TOTALSTAT_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_DET1", "Jes eff. det. 1", kJESEFF_DET1_UP, kJESEFF_DET1_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_DET2", "Jes eff. det. 2", kJESEFF_DET2_UP, kJESEFF_DET2_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_DET3", "Jes eff. det. 3", kJESEFF_DET3_UP, kJESEFF_DET3_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MIX1", "Jes eff. mix. 1", kJESEFF_MIX1_UP, kJESEFF_MIX1_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MIX2", "Jes eff. mix. 2", kJESEFF_MIX2_UP, kJESEFF_MIX2_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MIX3", "Jes eff. mix. 3", kJESEFF_MIX3_UP, kJESEFF_MIX3_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MIX4", "Jes eff. mix. 4", kJESEFF_MIX4_UP, kJESEFF_MIX4_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MODEL1", "Jes eff. model 1", kJESEFF_MODEL1_UP, kJESEFF_MODEL1_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MODEL2", "Jes eff. model 2", kJESEFF_MODEL2_UP, kJESEFF_MODEL2_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MODEL3", "Jes eff. model 3", kJESEFF_MODEL3_UP, kJESEFF_MODEL3_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_MODEL4", "Jes eff. model 4", kJESEFF_MODEL4_UP, kJESEFF_MODEL4_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_STAT1", "Jes eff. stat. 1", kJESEFF_STAT1_UP, kJESEFF_STAT1_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_STAT2", "Jes eff. stat. 2", kJESEFF_STAT2_UP, kJESEFF_STAT2_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_STAT3", "Jes eff. stat. 3", kJESEFF_STAT3_UP, kJESEFF_STAT3_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESEFF_STAT4", "Jes eff. stat. 4", kJESEFF_STAT4_UP, kJESEFF_STAT4_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESPILEUP_OFFMU", "Jes Pileup Off mu", kJESPILEUP_OFFMU_UP, kJESPILEUP_OFFMU_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESPILEUP_OFFNPV", "Jes Pileup Off NPV", kJESPILEUP_OFFNPV_UP, kJESPILEUP_OFFNPV_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESPILEUP_PT", "Jes Pileup Pt", kJESPILEUP_PT_UP, kJESPILEUP_PT_DOWN));
    Systs->Add(new AtlHistFactorySystPair("JESPILEUP_RHO", "Jes Pileup Rho", kJESPILEUP_RHO_UP, kJESPILEUP_RHO_DOWN));
    Systs->Add(new AtlHistFactorySystPair("SINGLEPART",  "Single part.",kSINGLEPART_UP, kSINGLEPART_DOWN));
    Systs->Add(new AtlHistFactorySystPair("FLAVOR_COMP", "Flavor comp.", kFLAVOR_COMP_UP, kFLAVOR_COMP_DOWN));
    Systs->Add(new AtlHistFactorySystPair("FLAVOR_RESP", "Flavor resp.", kFLAVOR_RESP_UP, kFLAVOR_RESP_DOWN));
    Systs->Add(new AtlHistFactorySystPair("PUNCH_THROUGH", "Punch through", kPUNCH_THROUGH_UP, kPUNCH_THROUGH_DOWN));
    // Lepton Scale Factors
    Systs->Add(new AtlHistFactorySystPair("LEP_RECO_SF", "Lep. reco. SF", kLEP_RECO_SF_UP, kLEP_RECO_SF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("LEP_ID_SF", "Lep. ID SF", kLEP_ID_SF_UP, kLEP_ID_SF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("LEP_TRIG_SF", "Lep. trig. SF", kLEP_TRIG_SF_UP, kLEP_TRIG_SF_DOWN));
    // BTag Scale Factors
    Systs->Add(new AtlHistFactorySystPair("BTagSF", "b-tag SF", kBTAGSF_UP, kBTAGSF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("CTagSF", "c-tag SF", kCTAGSF_UP, kCTAGSF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("MisTagSF", "mis-tag SF", kMISTAGSF_UP, kMISTAGSF_DOWN));
    // Pdf Uncertainty
    Systs->Add(new AtlHistFactorySystPdf("Pdf", "Pdf", kPdf_up, kPdf_down));
    // Default Systematics
    // --> The channel will loop over this list and add all systematic as rate uncertainties
    //     UNLESS it finds that the sample wants to use it additional shape information!





    // ---------------------------------------
    //            Signal region
    // ---------------------------------------
    //
    //    
	// v15 : incorrect JES calibration in data
	// TString SignalDir = "/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/sig_INTschanME/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_JetCut40/2Jets/lnu/";
	// v15f: correct JES calibration in data
	TString SignalDir = "/rdsk/dats8/atlas/rieck/8TeV_s_channel/v15f/sig_INTschanME/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_JetCut40/2Jets/lnu/";
	
    AtlHistFactoryChannel *ch1 = 0;
    ch1 = new AtlHistFactoryChannel("Signal",
				    "MemDiscriminant/sChannelProb_bins2N",
				    SignalDir);
    ch1->SetShapeDiscriminants("MemDiscriminant/sChannelProb_bins2N_even",
     			       "MemDiscriminant/sChannelProb_bins2N_odd");
    ch1->AddSystematics(Systs);
    
    // Add samples for channel 1
    AtlHistFactorySample *ch1_sig = new AtlHistFactorySample("sgtop_sChannel", "Single-top #it{s}-channel",
							     "sChannel_mu", 0.04, kTRUE);
    // the last parameter flags this sample as signal ( = 'true')
    AtlHistFactorySample *ch1_bkg1 = new AtlHistFactorySample("sgtop_tChannel", "Norm. Single-top #it{t}-channel",
							      "norm_tchan", 0.04);
    AtlHistFactorySample *ch1_bkg2 = new AtlHistFactorySample("sgtop_WtChannel", "Norm. Single-top Wt",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch1_bkg3 = new AtlHistFactorySample("ttbar", "Norm. t#bar{t} production",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch1_bkg4 = new AtlHistFactorySample("wjets", "Norm. W+jets",
							      "norm_wjets", 0.60);
    AtlHistFactorySample *ch1_bkg5 = new AtlHistFactorySample("zjets_diboson", "Norm. Z+jets, di-boson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch1_bkg6 = new AtlHistFactorySample("QCD", "Norm. Multi-jet",
							      "norm_QCD", 0.50);
    ch1_bkg6->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
    
    // Sample specific systematics
    // sgtop s-channel
    AtlHistFactorySystematic *gen_schan = new AtlHistFactorySystOneSided("aMCatNLO_sChannel", "aMCatNLO sgtop s-channel",
									 kaMCatNLO_schan, AtlHistFactorySystOneSided::kFullDiff);
    gen_schan->SetUseFullStats(kTRUE);
    // Use shape?
    // gen_schan->SetUseShape(kTRUE);
    ch1_sig->AddSystematic(gen_schan);

    // Consider shape for systematic from common list of systematics?
    ch1_sig->AddShapeSystematic("EtaIntercalibModel", Systs);
    ch1_sig->AddShapeSystematic("JESEFF_DET3", Systs);
    ch1_sig->AddShapeSystematic("Jer", Systs);
    ch1_sig->AddShapeSystematic("MUSC", Systs);
    
    AtlHistFactorySystEnvelope *scale_schan = new AtlHistFactorySystEnvelope("ScaleVariation_sChannel", "Scalevar. sgtop s-channel",
									     kScaleVar1, kScaleVar2, kScaleVar3,
									     kScaleVar4, kScaleVar5, kScaleVar6);
    scale_schan->SetUseFullStats(kTRUE);
    ch1_sig->AddSystematic(scale_schan);

    // sgtop t-channel
    AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    gen_tchan->SetUseFullStats(kTRUE);
    ch1_bkg1->AddSystematic(gen_tchan);

    // Consider shape for systematic from common list of systematics?
    ch1_bkg1->AddShapeSystematic("JESEFF_DET1", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_MODEL3", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_STAT1", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_STAT2", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_STAT4", Systs);
    ch1_bkg1->AddShapeSystematic("Jer", Systs);
    
    // sgtop wt
    AtlHistFactorySystOneSided *gen_wt = new AtlHistFactorySystOneSided("WtDS", "Wt DS",
									kWtDS, AtlHistFactorySystOneSided::kFullDiff);
    gen_wt->SetUseFullStats(kTRUE);
    ch1_bkg2->AddSystematic(gen_wt);

    AtlHistFactorySystOneSided *gen2_wt = new AtlHistFactorySystOneSided("MCatNLO_wt", "MCatNLO Wt",
									 kMCatNLO_wt, AtlHistFactorySystOneSided::kFullDiff);
    gen2_wt->SetUseFullStats(kTRUE);
    ch1_bkg2->AddSystematic(gen2_wt);

    // ttbar
    AtlHistFactorySystOneSided *gen_ttbar = new AtlHistFactorySystOneSided("MCatNLO_ttbar", "MCatNLO ttbar",
									   kMCatNLO_ttbar, AtlHistFactorySystOneSided::kFullDiff);
    gen_ttbar->SetUseFullStats(kTRUE);
    ch1_bkg3->AddSystematic(gen_ttbar);

    // old radiation systematic
    // AtlHistFactorySystModelPair *isrfsr = new AtlHistFactorySystModelPair("ISR_FSR", "ISR/FSR",
    // 									  kRadHi_ttbar, kRadLo_ttbar);
    // isrfsr->SetUseFullStats(kTRUE);

    AtlHistFactorySystEnvelope *isrfsr = new AtlHistFactorySystEnvelope("ISR_FSR", "ISR/FSR",
									kRadHi_ttbar, kRadLo_ttbar);
    isrfsr->SetUseFullStats(kTRUE);
    ch1_bkg3->AddSystematic(isrfsr);

    // wjets
    AtlHistFactorySystOneSided *gen_wjets = new AtlHistFactorySystOneSided("Wjets_Alpgen", "Alpgen+Pythia W+jets",
									   kWjetsGen, AtlHistFactorySystOneSided::kFullDiff);
    gen_wjets->SetUseFullStats(kTRUE);
    ch1_bkg4->AddSystematic(gen_wjets);

    
    // Add samples to channel
    ch1->AddSample(ch1_sig);
    ch1->AddSample(ch1_bkg1);
    ch1->AddSample(ch1_bkg2);
    ch1->AddSample(ch1_bkg3);
    ch1->AddSample(ch1_bkg4);
    ch1->AddSample(ch1_bkg5);
    ch1->AddSample(ch1_bkg6);


    // ---------------------------------------
    //       W+Jets ctrl region
    // ---------------------------------------
    //
    //    
	// v15 : incorrect JES calibration in data
	// TString WjetsDir = "/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_wjets_INTschanME/AtlSgTop_sChannelAnalysis/JetPtCut/2Jets/lnu/";
	// v15f: correct JES calibration in data
	TString WjetsDir = "/rdsk/dats8/atlas/rieck/8TeV_s_channel/v15f/ctrl_wjets_INTschanME/AtlSgTop_sChannelAnalysis/2Jets/lnu/";
	
    AtlHistFactoryChannel *ch2 = new AtlHistFactoryChannel("WjetsCtrlRegion",
     							   "bTagged/Sel/h_sel_Lepton_charge",
    							   WjetsDir);
    ch2->SetShapeDiscriminants("bTagged/Sel/h_sel_Lepton_charge_even",
     			       "bTagged/Sel/h_sel_Lepton_charge_odd");
    ch2->AddSystematics(Systs);
    
    // Add samples for channel 2
    AtlHistFactorySample *ch2_sig = new AtlHistFactorySample("sgtop_sChannel", "Single-top #it{s}-channel",
							     "sChannel_mu", 0.04, kTRUE);
    // the last parameter flags this sample as signal ( = 'true')
    AtlHistFactorySample *ch2_bkg1 = new AtlHistFactorySample("sgtop_tChannel", "Norm. Single-top #it{t}-channel",
							      "norm_tchan", 0.04);
    AtlHistFactorySample *ch2_bkg2 = new AtlHistFactorySample("sgtop_WtChannel", "Norm. Single-top Wt",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch2_bkg3 = new AtlHistFactorySample("ttbar", "Norm. t#bar{t} production",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch2_bkg4 = new AtlHistFactorySample("wjets", "Norm. W+jets",
							      "norm_wjets", 0.60);
    AtlHistFactorySample *ch2_bkg5 = new AtlHistFactorySample("zjets_diboson", "Norm. Z+jets, di-boson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch2_bkg6 = new AtlHistFactorySample("QCD", "Norm. Multi-jet",
							      "norm_QCD", 0.50);
    ch2_bkg6->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
                                // Data sample must be added separatly
    
    // Sample specific systematics
    // sgtop s-channel
    AtlHistFactorySystematic *gen_schan = new AtlHistFactorySystOneSided("aMCatNLO_sChannel", "aMCatNLO sgtop s-channel",
									 kaMCatNLO_schan, AtlHistFactorySystOneSided::kFullDiff);
    gen_schan->SetUseFullStats(kTRUE);
    ch2_sig->AddSystematic(gen_schan);

    AtlHistFactorySystEnvelope *scale_schan = new AtlHistFactorySystEnvelope("ScaleVariation_sChannel", "Scalevar. sgtop s-channel",
									     kScaleVar1, kScaleVar2, kScaleVar3,
									     kScaleVar4, kScaleVar5, kScaleVar6);
    scale_schan->SetUseFullStats(kTRUE);
    ch2_sig->AddSystematic(scale_schan);

    // sgtop t-channel
    AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    gen_tchan->SetUseFullStats(kTRUE);
    ch2_bkg1->AddSystematic(gen_tchan);

    // sgtop wt
    AtlHistFactorySystOneSided *gen_wt = new AtlHistFactorySystOneSided("WtDS", "Wt DS",
									kWtDS, AtlHistFactorySystOneSided::kFullDiff);
    gen_wt->SetUseFullStats(kTRUE);
    ch2_bkg2->AddSystematic(gen_wt);

    AtlHistFactorySystOneSided *gen2_wt = new AtlHistFactorySystOneSided("MCatNLO_wt", "MCatNLO Wt",
									 kMCatNLO_wt, AtlHistFactorySystOneSided::kFullDiff);
    gen2_wt->SetUseFullStats(kTRUE);
    ch2_bkg2->AddSystematic(gen2_wt);

    // ttbar
    AtlHistFactorySystOneSided *gen_ttbar = new AtlHistFactorySystOneSided("MCatNLO_ttbar", "MCatNLO ttbar",
									   kMCatNLO_ttbar, AtlHistFactorySystOneSided::kFullDiff);
    gen_ttbar->SetUseFullStats(kTRUE);
    ch2_bkg3->AddSystematic(gen_ttbar);

    // old raditation systematic
    // AtlHistFactorySystModelPair *isrfsr = new AtlHistFactorySystModelPair("ISR_FSR", "ISR/FSR",
    // 									  kRadHi_ttbar, kRadLo_ttbar);
    // isrfsr->SetUseFullStats(kTRUE);
    // ch2_bkg3->AddSystematic(isrfsr);

    AtlHistFactorySystEnvelope *isrfsr = new AtlHistFactorySystEnvelope("ISR_FSR", "ISR/FSR",
									kRadHi_ttbar, kRadLo_ttbar);
    isrfsr->SetUseFullStats(kTRUE);
    ch2_bkg3->AddSystematic(isrfsr);

    // wjets
    AtlHistFactorySystOneSided *gen_wjets = new AtlHistFactorySystOneSided("Wjets_Alpgen", "Alpgen+Pythia W+jets",
									   kWjetsGen, AtlHistFactorySystOneSided::kFullDiff);
    gen_wjets->SetUseFullStats(kTRUE);
    ch2_bkg4->AddSystematic(gen_wjets);

    ch2->AddSample(ch2_sig);
    ch2->AddSample(ch2_bkg1);
    ch2->AddSample(ch2_bkg2);
    ch2->AddSample(ch2_bkg3);
    ch2->AddSample(ch2_bkg4);
    ch2->AddSample(ch2_bkg5);
    ch2->AddSample(ch2_bkg6);
    
    // Add channels to measurement
    meas->AddChannel(ch1);
    meas->AddChannel(ch2);

    // Set Draw options for all samples
    // (colors and style will be used across channels)
    // Fill color, fill style, line color, line style
    // meas->SetDrawOptions("sgtop_sChannel", kAzure+3, 1001, kAzure+3);
    // meas->SetDrawOptions("sgtop_tChannel", kAzure+5, 1001, kAzure+5);
    // meas->SetDrawOptions("sgtop_WtChannel", kAzure-3, 1001, kAzure-3);
    meas->SetDrawOptions("sgtop_sChannel", kOrange, 1001, kOrange);
    meas->SetDrawOptions("sgtop_tChannel", kAzure-3, 1001, kAzure-3);
    meas->SetDrawOptions("sgtop_WtChannel", kAzure-6, 1001, kAzure-6);
    meas->SetDrawOptions("ttbar", kRed-4, 1001, kRed-4);
    meas->SetDrawOptions("wjets", kGreen-3, 1001, kGreen-3);
    meas->SetDrawOptions("zjets_diboson", kOrange-3, 1001, kOrange-3);
    meas->SetDrawOptions("QCD", kMagenta+2, 1001, kMagenta+2);
    
    // Finally, add measurement
    main->SetMeasurement(meas);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
