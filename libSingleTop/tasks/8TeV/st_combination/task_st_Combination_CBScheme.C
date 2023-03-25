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
	= new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
				  "SgTop tChannel Analysis");
    
    // AtlTopLevelAnalysis options
    main->SetBuildHistFactoryTree(kTRUE);
    // 2nd EB Meeting
    TString OutputDir = "/rdsk/dats2/atlas/stamm/HistFactory/st_combination_dijet7_mcmcsf_CB";

    // TString OutputDir = "/rdsk/dats2/atlas/stamm/HistFactory/st_combination_1stEBupdate";
    // TString OutputDir = "/rdsk/dats2/atlas/stamm/HistFactory/st_combination_statonly";

    main->SetJobHomeDir(Form("%s/JobHome", OutputDir.Data()));
    
    // Alternative plotter scheme
    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("st_Channel_Analysis",
    								    "s+t-Channel Analysis",
    								    OutputDir.Data(),
    								    "SgTop_s_wjetsCB");
    meas->AddPOI("tChannel_mu");
    meas->AddPOI("sChannel_mu");
    
    // Histograms are already scaled to luminosity!
    // --> Luminosity is 1, only provide relativ error
    meas->SetLumiAndRelError(1., 0.028);
    // Shape tests using pseudo exp?
    meas->SetUsePseudoExp(true);
    // Export shape comparison plots to pdf?
    meas->SetExportShapePlots(kTRUE);
    // Export correlation matrix to root file?
    meas->SetExportCorrMatrix(kTRUE);
    // AsimovData?
    meas->SetUseAsimovData(true);
    // Pull Plots
    meas->SetRunPullPlots(true);
    // No Systematics?
    meas->SetNoSystematics(false);
    
    // Create list of systematics for all channels (lnu)
    TList *Systs = new TList;
    // Nominal
    Systs->Add(new AtlHistFactorySystNominal("Nominal"));
    // Default systematics
    Systs->Add(new AtlHistFactorySystOneSided("Jeff", "Jet reco. eff.", kJEFF, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystOneSided("Jer", "Jet energy res.", kJER, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystPair("JVF", "Jet vertex fraction", kJVF_UP, kJVF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("RES_SOFT", "Etmiss res.",  kRES_SOFT_UP, kRES_SOFT_DOWN));
    Systs->Add(new AtlHistFactorySystPair("SC_SOFT", "Etmiss soft", kSC_SOFT_UP, kSC_SOFT_DOWN));
    // Lepton specific systematics
    Systs->Add(new AtlHistFactorySystPair("EER", "El. energy res.", kEER_UP, kEER_DOWN));
    Systs->Add(new AtlHistFactorySystPair("EES", "El. energy scale.", kEES_UP, kEES_DOWN));
    Systs->Add(new AtlHistFactorySystOneSided("MuId", "Muon ID", kMUID_RES, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystOneSided("MuMS", "Muon mom. res.", kMUMS_RES, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystPair("MUSC", "Muon mom. scale", kMUSC_UP, kMUSC_DOWN));
    // Lepton Scale Factors
    Systs->Add(new AtlHistFactorySystPair("LEP_RECO_SF", "Lep. reco. SF", kLEP_RECO_SF_UP, kLEP_RECO_SF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("LEP_ID_SF", "Lep. ID SF", kLEP_ID_SF_UP, kLEP_ID_SF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("LEP_TRIG_SF", "Lep. trig. SF", kLEP_TRIG_SF_UP, kLEP_TRIG_SF_DOWN));
    // BTag Scale Factors
    Systs->Add(new AtlHistFactorySystPair("BTagSF", "b-tag SF", kBTAGSF_UP, kBTAGSF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("CTagSF", "c-tag SF", kCTAGSF_UP, kCTAGSF_DOWN));
    Systs->Add(new AtlHistFactorySystPair("MisTagSF", "mis-tag SF", kMISTAGSF_UP, kMISTAGSF_DOWN));
    // Pdf Uncertainty
    // Systs->Add(new AtlHistFactorySystPdf("Pdf", "Pdf", kPdf_up, kPdf_down));
    // Jes Splitting?
    // Systs->Add(new AtlHistFactorySystPair("JES", "Jet energy scale", kJES_UP, kJES_DOWN));
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
  
    // Default Systematics
    // --> The channel will loop over this list and add all systematic as rate uncertainties
    //     UNLESS it finds that the sample wants to use it additional shape information!

    // Signal channel
    TString SignalDir_t = "/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/Signal_combination/final_cuts_schanveto/AtlSgTop_tChannelAnalysis/MemDiscrAnalysis_dijet7_mcmcsf/2Jets/lnu/";
    AtlHistFactoryChannel *ch1 = new AtlHistFactoryChannel("Signal_t",
							   "MemDiscriminant/tChannelProb4FS",
    							   SignalDir_t);
    ch1->SetShapeDiscriminants("MemDiscriminant/tChannelProb4FS_even",
     			       "MemDiscriminant/tChannelProb4FS_odd");
    ch1->AddSystematics(Systs);

    // Add samples for channel 1
    AtlHistFactorySample *ch1_sig = new AtlHistFactorySample("sgtop_tChannel", "Single-top #it{t}-channel",
							     "tChannel_mu", 0.04, kTRUE);
    // the last parameter flags this sample as signal ( = 'true')
    // for all samples, but the signal sample a log norm distr. will be added
    AtlHistFactorySample *ch1_bkg1 = new AtlHistFactorySample("sgtop_sChannel", "Single-top #it{s}-channel",
							      "sChannel_mu", 0.04, kTRUE); // using lognorm as default
    AtlHistFactorySample *ch1_bkg2 = new AtlHistFactorySample("sgtop_WtChannel", "Norm. Single-top Wt",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch1_bkg3 = new AtlHistFactorySample("ttbar", "Norm. t#bar{t} production",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch1_bkg4a = new AtlHistFactorySample("wjetsC", "Norm. W+jets",
							       "norm_wjetsC", 0.60);
    AtlHistFactorySample *ch1_bkg4b = new AtlHistFactorySample("wjetsB", "Norm. W+jets",
							       "norm_wjetsB", 0.60);
    AtlHistFactorySample *ch1_bkg5 = new AtlHistFactorySample("zjets_diboson", "Norm. Z+jets, di-boson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch1_bkg6 = new AtlHistFactorySample("QCD", "Norm. Multi-jet",
							      "norm_QCD", 0.50);
    ch1_bkg6->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
                                // Data sample must be added separatly
    
    // Sample specific systematics
    // sgtop t-channel
    AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    gen_tchan->SetUseFullStats(kTRUE);
    gen_tchan->SetSystNominal(kPowhegHerwig_t);
    ch1_sig->AddSystematic(gen_tchan);

    // old wrong scale variation setup
    // AtlHistFactorySystEnvelope *scale_tchan = new AtlHistFactorySystEnvelope("ScaleVariation_tChannel", "Scalevar. sgtop t-channel",
    // 									     kScaleVar_tchan1, kScaleVar_tchan2,
    // 									     kScaleVar_tchan3, kScaleVar_tchan4);
    // scale_tchan->SetUseFullStats(kTRUE);
    // ch1_sig->AddSystematic(scale_tchan);

    // new shower systematics
    AtlHistFactorySystOneSided *gen_shower = new AtlHistFactorySystOneSided("Shower_tChannel", "Shower t-channel",
									    kPowhegHerwig_t, AtlHistFactorySystOneSided::kFullDiff);
    gen_shower->SetUseFullStats(kTRUE);
    gen_shower->SetSystNominal(kPowhegPythia_t);
    ch1_sig->AddSystematic(gen_shower);

    AtlHistFactorySystModelPair *scale_tchan = new AtlHistFactorySystModelPair("ScaleVariation_tChannel", "Scalevar. sgtop t-channel",
									       kScaleVar_tchan1, kScaleVar_tchan3);
    scale_tchan->SetUseFullStats(kTRUE);
    scale_tchan->SetSystNominal(kPowhegPythia_t);
    ch1_sig->AddSystematic(scale_tchan);

    // Consider shape for systematic from common list of systematics?
    ch1_sig->AddShapeSystematic("EtaIntercalibTotalStat", Systs);
    ch1_sig->AddShapeSystematic("JESEFF_MODEL2", Systs);
    ch1_sig->AddShapeSystematic("JESEFF_MIX3", Systs);
    ch1_sig->AddShapeSystematic("JESEFF_MIX4", Systs);
    ch1_sig->AddShapeSystematic("RES_SOFT", Systs);
    ch1_sig->AddShapeSystematic("EER", Systs);
    
    // sgtop s-channel
    // Consider shape for systematic from common list of systematics?
    ch1_bkg1->AddShapeSystematic("MUSC", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_MIX1", Systs);
    ch1_bkg1->AddShapeSystematic("JESEFF_STAT2", Systs);
    ch1_bkg1->AddShapeSystematic("JESPILEUP_OFFNPV", Systs);
    
    AtlHistFactorySystematic *gen_schan = new AtlHistFactorySystOneSided("aMCatNLO_sChannel", "aMCatNLO sgtop s-channel",
									   kaMCatNLO_schan, AtlHistFactorySystOneSided::kFullDiff);
    gen_schan->SetUseFullStats(kTRUE);
    ch1_bkg1->AddSystematic(gen_schan);

    AtlHistFactorySystEnvelope *scale_schan = new AtlHistFactorySystEnvelope("ScaleVariation_sChannel", "Scalevar. sgtop s-channel",
									     kScaleVar1, kScaleVar2, kScaleVar3,
									     kScaleVar4, kScaleVar5, kScaleVar6);
    scale_schan->SetUseFullStats(kTRUE);
    ch1_bkg1->AddSystematic(scale_schan);

    // sgtop wt
    // Consider shape for systematic from common list of systematics?
    ch1_bkg2->AddShapeSystematic("JESPILEUP_OFFMU", Systs);
    ch1_bkg2->AddShapeSystematic("JESEFF_MODEL2", Systs);
    ch1_bkg2->AddShapeSystematic("JESEFF_STAT3", Systs);

    AtlHistFactorySystOneSided *gen_wt = new AtlHistFactorySystOneSided("WtDS", "Wt DS",
									kWtDS, AtlHistFactorySystOneSided::kFullDiff);
    gen_wt->SetUseFullStats(kTRUE);
    ch1_bkg2->AddSystematic(gen_wt);
    
    // ttbar
    AtlHistFactorySystOneSided *gen_ttbar = new AtlHistFactorySystOneSided("MCatNLO_ttbar", "MCatNLO ttbar",
									   kMCatNLO_ttbar, AtlHistFactorySystOneSided::kFullDiff);
    gen_ttbar->SetUseFullStats(kTRUE);
    ch1_bkg3->AddSystematic(gen_ttbar);
    
    AtlHistFactorySystEnvelope *isrfsr = new AtlHistFactorySystEnvelope("ISR_FSR", "ISR/FSR",
     									kRadHi_ttbar, kRadLo_ttbar);
    isrfsr->SetUseFullStats(kFALSE);
    ch1_bkg3->AddSystematic(isrfsr);

    // Wjets
    AtlHistFactorySystOneSided *gen_wjets = new AtlHistFactorySystOneSided("Wjets_Alpgen", "Alpgen+Pythia W+jets",
									   kWjetsGen, AtlHistFactorySystOneSided::kFullDiff);
    gen_wjets->SetUseFullStats(kTRUE);
    gen_wjets->SetUseShape(kFALSE);
    ch1_bkg4a->AddSystematic(gen_wjets);
    ch1_bkg4b->AddSystematic(gen_wjets);

    // Consider shape for systematic from common list of systematics?
    ch1_bkg4a->AddShapeSystematic("Jer", Systs);
    ch1_bkg4a->AddShapeSystematic("FLAVOR_COMP", Systs);
    ch1_bkg4a->AddShapeSystematic("JESEFF_STAT4", Systs);
    ch1_bkg4a->AddShapeSystematic("JESEFF_MIX1", Systs);

    // Z+Jets
    ch1_bkg5->AddShapeSystematic("JESEFF_STAT2", Systs);
    ch1_bkg5->AddShapeSystematic("JESEFF_STAT3", Systs);
    ch1_bkg5->AddShapeSystematic("JESEFF_DET2", Systs);
    
    TString SignalDir_s = "/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/signal/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/2Jets/lnu/";
    
    AtlHistFactoryChannel *ch2 = 0;
    ch2 = new AtlHistFactoryChannel("Signal_s",
				    "MemDiscriminant/sChannelProb_bins2N",
				    SignalDir_s);
    ch2->SetShapeDiscriminants("MemDiscriminant/sChannelProb_bins2N_even",
     			       "MemDiscriminant/sChannelProb_bins2N_odd");
    ch2->AddSystematics(Systs);
    
    // Add samples for channel 1
    AtlHistFactorySample *ch2_sig = new AtlHistFactorySample("sgtop_sChannel", "Single-top #it{s}-channel",
							     "sChannel_mu", 0.04, kTRUE);
    // the last parameter flags this sample as signal ( = 'true')
    AtlHistFactorySample *ch2_bkg1 = new AtlHistFactorySample("sgtop_tChannel", "Norm. Single-top #it{t}-channel",
							      "tChannel_mu", 0.04, kTRUE);
    AtlHistFactorySample *ch2_bkg2 = new AtlHistFactorySample("sgtop_WtChannel", "Norm. Single-top Wt",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch2_bkg3 = new AtlHistFactorySample("ttbar", "Norm. t#bar{t} production",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch2_bkg4a = new AtlHistFactorySample("wjetsC", "Norm. W+jets C",
							       "norm_wjetsC", 0.60);
    AtlHistFactorySample *ch2_bkg4b = new AtlHistFactorySample("wjetsB", "Norm. W+jets B",
							       "norm_wjetsB", 0.60);
    AtlHistFactorySample *ch2_bkg5 = new AtlHistFactorySample("zjets_diboson", "Norm. Z+jets, di-boson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch2_bkg6 = new AtlHistFactorySample("QCD", "Norm. Multi-jet",
							      "norm_QCD", 0.50);
    ch2_bkg6->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
    
    // Sample specific systematics
    // sgtop s-channel
    AtlHistFactorySystematic *gen_schan = new AtlHistFactorySystOneSided("aMCatNLO_sChannel", "aMCatNLO sgtop s-channel",
									 kaMCatNLO_schan, AtlHistFactorySystOneSided::kFullDiff);
    gen_schan->SetUseFullStats(kTRUE);
    ch2_sig->AddSystematic(gen_schan);

    // Consider shape for systematic from common list of systematics?
    ch2_sig->AddShapeSystematic("EtaIntercalibModel", Systs);
    ch2_sig->AddShapeSystematic("JESEFF_DET3", Systs);
    ch2_sig->AddShapeSystematic("Jer", Systs);
    ch2_sig->AddShapeSystematic("MUSC", Systs);
    
    AtlHistFactorySystEnvelope *scale_schan = new AtlHistFactorySystEnvelope("ScaleVariation_sChannel", "Scalevar. sgtop s-channel",
									     kScaleVar1, kScaleVar2, kScaleVar3,
									     kScaleVar4, kScaleVar5, kScaleVar6);
    scale_schan->SetUseFullStats(kTRUE);
    ch2_sig->AddSystematic(scale_schan);

    // old setup
    // AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
    // 									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    // gen_tchan->SetUseFullStats(kTRUE);
    // ch2_bkg1->AddSystematic(gen_tchan);

    AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
    									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    gen_tchan->SetUseFullStats(kTRUE);
    gen_tchan->SetSystNominal(kPowhegHerwig_t);
    ch2_bkg1->AddSystematic(gen_tchan);

    // new shower systematics
    AtlHistFactorySystOneSided *gen_shower = new AtlHistFactorySystOneSided("Shower_tChannel", "Shower t-channel",
									    kPowhegHerwig_t, AtlHistFactorySystOneSided::kFullDiff);
    gen_shower->SetUseFullStats(kTRUE);
    gen_shower->SetSystNominal(kPowhegPythia_t);
    ch2_bkg1->AddSystematic(gen_shower);

    // old scale var setup
    // AtlHistFactorySystEnvelope *scale_tchan = new AtlHistFactorySystEnvelope("ScaleVariation_tChannel", "Scalevar. sgtop t-channel",
    // 									     kScaleVar_tchan1, kScaleVar_tchan2,
    // 									     kScaleVar_tchan3, kScaleVar_tchan4);
    // scale_tchan->SetUseFullStats(kTRUE);
    // ch2_bkg1->AddSystematic(scale_tchan);
    
    AtlHistFactorySystModelPair *scale_tchan = new AtlHistFactorySystModelPair("ScaleVariation_tChannel", "Scalevar. sgtop t-channel",
									       kScaleVar_tchan1, kScaleVar_tchan3);
    scale_tchan->SetUseFullStats(kTRUE);
    scale_tchan->SetSystNominal(kPowhegPythia_t);
    ch2_bkg1->AddSystematic(scale_tchan);


    // Consider shape for systematic from common list of systematics?
    ch2_bkg1->AddShapeSystematic("JESEFF_DET1", Systs);
    ch2_bkg1->AddShapeSystematic("JESEFF_MODEL3", Systs);
    ch2_bkg1->AddShapeSystematic("JESEFF_STAT1", Systs);
    ch2_bkg1->AddShapeSystematic("JESEFF_STAT2", Systs);
    ch2_bkg1->AddShapeSystematic("JESEFF_STAT4", Systs);
    ch2_bkg1->AddShapeSystematic("Jer", Systs);
    
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

    AtlHistFactorySystEnvelope *isrfsr = new AtlHistFactorySystEnvelope("ISR_FSR", "ISR/FSR",
     									kRadHi_ttbar, kRadLo_ttbar);
    isrfsr->SetUseFullStats(kTRUE);
    isrfsr->SetUseShape(kTRUE);
    ch2_bkg3->AddSystematic(isrfsr);

    // wjets
    AtlHistFactorySystOneSided *gen_wjets = new AtlHistFactorySystOneSided("Wjets_Alpgen", "Alpgen+Pythia W+jets",
									   kWjetsGen, AtlHistFactorySystOneSided::kFullDiff);
    gen_wjets->SetUseFullStats(kTRUE);
    ch2_bkg4a->AddSystematic(gen_wjets);

    // AtlHistFactorySystOneSided *gen_wjets = new AtlHistFactorySystOneSided("Wjets_Alpgen", "Alpgen+Pythia W+jets",
    // 									   kWjetsGen, AtlHistFactorySystOneSided::kFullDiff);
    // gen_wjets->SetUseFullStats(kTRUE);
    // gen_wjets->SetUseShape(kTRUE);
    ch2_bkg4b->AddSystematic(gen_wjets);
    
    // Add samples to channel 1
    ch1->AddSample(ch1_sig);
    ch1->AddSample(ch1_bkg1);
    ch1->AddSample(ch1_bkg2);
    ch1->AddSample(ch1_bkg3);
    ch1->AddSample(ch1_bkg4a);
    ch1->AddSample(ch1_bkg4b);
    ch1->AddSample(ch1_bkg5);
    ch1->AddSample(ch1_bkg6);

    // Add samples to channel 2
    ch2->AddSample(ch2_sig);
    ch2->AddSample(ch2_bkg1);
    ch2->AddSample(ch2_bkg2);
    ch2->AddSample(ch2_bkg3);
    ch2->AddSample(ch2_bkg4a);
    ch2->AddSample(ch2_bkg4b);
    ch2->AddSample(ch2_bkg5);
    ch2->AddSample(ch2_bkg6);


    // Add channels to measurement
    meas->AddChannel(ch1);
    meas->AddChannel(ch2);

    // Finally, add measurement
    main->SetMeasurement(meas);

    // Set Draw options for all samples
    // (colors and style will be used across channels)
    // Fill color, fill style, line color, line style
    meas->SetDrawOptions("sgtop_sChannel", kOrange, 1001, kOrange);
    meas->SetDrawOptions("sgtop_tChannel", kAzure-3, 1001, kAzure-3);
    meas->SetDrawOptions("sgtop_WtChannel", kAzure-6, 1001, kAzure-6);
    meas->SetDrawOptions("ttbar", kRed-4, 1001, kRed-4);
    meas->SetDrawOptions("wjetsC", kGreen-3, 1001, kGreen-3);
    meas->SetDrawOptions("wjetsB", kGreen-3, 1001, kGreen+3);
    meas->SetDrawOptions("zjets_diboson", kOrange-3, 1001, kOrange-3);
    meas->SetDrawOptions("QCD", kMagenta+2, 1001, kMagenta+2);

    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
