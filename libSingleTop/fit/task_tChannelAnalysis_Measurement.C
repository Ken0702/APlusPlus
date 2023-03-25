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
    TString OutputDir = "/rdsk/dats2/atlas/stamm/HistFactory/t_channel/Measurement";

    main->SetJobHomeDir(Form("%s/JobHome", OutputDir.Data()));
    
    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("tChannel_Analysis",
								    "t-Channel Analysis incl. Fit",
								    OutputDir.Data());
    meas->AddPOI("tChannel_mu");
    // Histograms are already scaled to luminosity!
    // --> Luminosity is 1, only provide relativ error
    meas->SetLumiAndRelError(1., 0.028);
    // Shape tests using pseudo exp?
    meas->SetUsePseudoExp(kFALSE);
    // Export shape comparison plots to pdf?
    meas->SetExportShapePlots(kFALSE);
    // Export correlation matrix to root file?
    meas->SetExportCorrMatrix(kTRUE);
    // AsimovData?
    meas->SetUseAsimovData(true);
    // Pull Plots
    meas->SetRunPullPlots(false);
    // No Systematics?
    meas->SetNoSystematics(kFALSE);
    
    // Create list of systematics for all channels (lnu)
    TList *Systs = new TList;
    // Nominal
    Systs->Add(new AtlHistFactorySystNominal("Nominal"));
    // Default systematics
    Systs->Add(new AtlHistFactorySystOneSided("Jeff", "Jet reco. eff.", kJEFF, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystOneSided("Jer", "Jet energy res.", kJER, AtlHistFactorySystOneSided::kFullDiff));
    Systs->Add(new AtlHistFactorySystPair("JES", "Jet energy scale", kJES_UP, kJES_DOWN));
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
  
    // Default Systematics
    // --> The channel will loop over this list and add all systematic as rate uncertainties
    //     UNLESS it finds that the sample wants to use it additional shape information!

    // Signal channel
    TString SignalDir_t = "/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl/AtlSgTop_tChannelAnalysis/MemDiscrAnalysis/2Jets/lnu/";
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
							      "sChannel_mu", 0.04); // using lognorm as default
    AtlHistFactorySample *ch1_bkg2 = new AtlHistFactorySample("sgtop_WtChannel", "Norm. Single-top Wt",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch1_bkg3 = new AtlHistFactorySample("ttbar", "Norm. t#bar{t} production",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch1_bkg4 = new AtlHistFactorySample("wjets", "Norm. W+jets",
							      "norm_wjets", 0.60);
    // AtlHistFactorySample *ch1_bkg5 = new AtlHistFactorySample("wjetsHF", "Norm. W+jets HF",
    // 							      "norm_wjetsHF", 0.60);
    AtlHistFactorySample *ch1_bkg6 = new AtlHistFactorySample("zjets_diboson", "Norm. Z+jets, di-boson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch1_bkg7 = new AtlHistFactorySample("QCD", "Norm. Multi-jet",
							      "norm_QCD", 0.50);
    ch1_bkg7->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
                                // Data sample must be added separatly
    
    // Sample specific systematics
    // sgtop t-channel
    AtlHistFactorySystOneSided *gen_tchan = new AtlHistFactorySystOneSided("aMCatNLO_tChannel", "aMCatNLO sgtop t-channel",
									   kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff);
    gen_tchan->SetUseFullStats(kTRUE);
    ch1_sig->AddSystematic(gen_tchan);

    AtlHistFactorySystEnvelope *scale_tchan = new AtlHistFactorySystEnvelope("ScaleVariation_tChannel", "Scalevar. sgtop t-channel",
									     kScaleVar_tchan1, kScaleVar_tchan2,
									     kScaleVar_tchan3, kScaleVar_tchan4);
    scale_tchan->SetUseFullStats(kTRUE);
    ch1_sig->AddSystematic(scale_tchan);

    // sgtop s-channel
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
    isrfsr->SetUseFullStats(kTRUE);
    ch1_bkg3->AddSystematic(isrfsr);

    // Add samples to channel
    ch1->AddSample(ch1_sig);
    ch1->AddSample(ch1_bkg1);
    ch1->AddSample(ch1_bkg2);
    ch1->AddSample(ch1_bkg3);
    ch1->AddSample(ch1_bkg4);
    ch1->AddSample(ch1_bkg6);
    ch1->AddSample(ch1_bkg7);

    // Add channels to measurement
    meas->AddChannel(ch1);

    // Finally, add measurement
    main->SetMeasurement(meas);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
