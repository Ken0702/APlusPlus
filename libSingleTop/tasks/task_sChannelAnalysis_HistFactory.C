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
    main->SetJobHomeDir("/rdsk/dats8/atlas/stamm/HistFactory/Fit/JobHome");
    
    TString OutputDir = "/rdsk/dats8/atlas/stamm/HistFactory/Fit/";
    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("sChannel_Analysis",
								    "s-Channel Analysis Fit",
								    OutputDir.Data());
    meas->AddPOI("sChannel_mu"); // This needs to be a parameter set by a sample!
    meas->SetLumiAndRelError(1., 0.028);    // Histograms are already scaled to luminosity!

    TString SignalDir_e = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v14/sig_INTschanME/AtlSgTop_sChannelAnalysis/MemDiscAnalysis/2Jets/lnu/";
    
    AtlHistFactoryChannel *ch1 = new AtlHistFactoryChannel("Signal",
     							   "MemDiscriminant/sChannelProb",
    							   SignalDir_e);
    ch1->SetShapeDiscriminants("MemDiscriminant/sChannelProb_odd",
     			       "MemDiscriminant/sChannelProb_even");
    // Add Systematics to channel
    // 1) Systematics in this list are common to all samples in this channel
    // 2) Samples can overwrite settings of this list by adding a systematic
    //    with the _same name_ to their own list
    // 3) A sample can be excluded for all systematics (specified by a sample's data member)
    //
    TList *Systs_el = new TList;
    // Nominal
    Systs_el->Add(new AtlHistFactorySystNominal("Nominal"));
    // Default systematics
    Systs_el->Add(new AtlHistFactorySystOneSided("Jeff", kJEFF, AtlHistFactorySystOneSided::kFullDiff));
    Systs_el->Add(new AtlHistFactorySystOneSided("Jer", kJER, AtlHistFactorySystOneSided::kFullDiff));
    Systs_el->Add(new AtlHistFactorySystPair("JES", kJES_UP, kJES_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JVF", kJVF_UP, kJVF_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("EER", kEER_UP, kEER_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("EES", kEES_UP, kEES_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("RES_SOFT", kRES_SOFT_UP, kRES_SOFT_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("SC_SOFT", kSC_SOFT_UP, kSC_SOFT_DOWN));
    // Jes components
    Systs_el->Add(new AtlHistFactorySystPair("BJES", kBJES_UP, kBJES_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("EtaIntercalibModel", kETAINTERCALIB_MODEL_UP, kETAINTERCALIB_MODEL_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("EtaIntercalibTotalStat", kETAINTERCALIB_TOTALSTAT_UP, kETAINTERCALIB_TOTALSTAT_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_DET1", kJESEFF_DET1_UP, kJESEFF_DET1_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_DET2", kJESEFF_DET2_UP, kJESEFF_DET2_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_DET3", kJESEFF_DET3_UP, kJESEFF_DET3_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MIX1", kJESEFF_MIX1_UP, kJESEFF_MIX1_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MIX2", kJESEFF_MIX2_UP, kJESEFF_MIX2_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MIX3", kJESEFF_MIX3_UP, kJESEFF_MIX3_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MIX4", kJESEFF_MIX4_UP, kJESEFF_MIX4_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MODEL1", kJESEFF_MODEL1_UP, kJESEFF_MODEL1_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MODEL2", kJESEFF_MODEL2_UP, kJESEFF_MODEL2_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MODEL3", kJESEFF_MODEL3_UP, kJESEFF_MODEL3_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_MODEL4", kJESEFF_MODEL4_UP, kJESEFF_MODEL4_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_STAT1", kJESEFF_STAT1_UP, kJESEFF_STAT1_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_STAT2", kJESEFF_STAT2_UP, kJESEFF_STAT2_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_STAT3", kJESEFF_STAT3_UP, kJESEFF_STAT3_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESEFF_STAT4", kJESEFF_STAT4_UP, kJESEFF_STAT4_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESPILEUP_OFFMU", kJESPILEUP_OFFMU_UP, kJESPILEUP_OFFMU_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESPILEUP_OFFNPV", kJESPILEUP_OFFNPV_UP, kJESPILEUP_OFFNPV_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESPILEUP_PT", kJESPILEUP_PT_UP, kJESPILEUP_PT_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("JESPILEUP_RHO", kJESPILEUP_RHO_UP, kJESPILEUP_RHO_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("SINGLEPART", kSINGLEPART_UP, kSINGLEPART_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("FLAVOR_COMP", kFLAVOR_COMP_UP, kFLAVOR_COMP_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("FLAVOR_RESP", kFLAVOR_RESP_UP, kFLAVOR_RESP_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("PUNCH_THROUGH", kPUNCH_THROUGH_UP, kPUNCH_THROUGH_DOWN));
    // Lepton Scale Factors
    Systs_el->Add(new AtlHistFactorySystPair("LEP_RECO_SF", kLEP_RECO_SF_UP, kLEP_RECO_SF_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("LEP_ID_SF", kLEP_ID_SF_UP, kLEP_ID_SF_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("LEP_TRIG_SF", kLEP_TRIG_SF_UP, kLEP_TRIG_SF_DOWN));
    // BTag Scale Factors
    Systs_el->Add(new AtlHistFactorySystPair("BTagSF", kBTAGSF_UP, kBTAGSF_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("CTagSF", kCTAGSF_UP, kCTAGSF_DOWN));
    Systs_el->Add(new AtlHistFactorySystPair("MisTagSF", kMISTAGSF_UP, kMISTAGSF_DOWN));

    // Default Systematics
    // --> The channel will loop over this list and add all systematic as rate uncertainties
    //     UNLESS it finds that the sample wants to use it additional shape information!
    ch1->AddSystematics(Systs_el);
    
    // Add samples for channel 1
    // Only specify shape uncertainties (if not shape, it's a rate uncertainty)
    AtlHistFactorySample *ch1_sig = new AtlHistFactorySample("sgtop_sChannel",
							     "sChannel_mu", 0.04, kTRUE);
    // the last parameter flags this sample as signel ( = 'true')
    // for all samples, but the signal sample a log norm distr. will be added
    // ch1_sig->AddShapeSyst("BTagSF");
    AtlHistFactorySample *ch1_bkg1 = new AtlHistFactorySample("sgtop_tChannel",
							      "norm_tchan", 0.04); // using lognorm as default
    AtlHistFactorySample *ch1_bkg2 = new AtlHistFactorySample("sgtop_WtChannel",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch1_bkg3 = new AtlHistFactorySample("ttbar",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch1_bkg4 = new AtlHistFactorySample("wjets",
							      "norm_wjets", 0.60);
    AtlHistFactorySample *ch1_bkg5 = new AtlHistFactorySample("zjets_diboson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch1_bkg6 = new AtlHistFactorySample("QCD",
							      "norm_QCD", 0.50);
    ch1_bkg6->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
                                // Data sample must be added separatly

    // Systematics per channel
    // sgtop s-channel
    ch1_sig->AddSystematic(new AtlHistFactorySystOneSided("MCatNLO_sChannel", kMCatNLO_schan, AtlHistFactorySystOneSided::kFullDiff));
    ch1_sig->AddSystematic(new AtlHistFactorySystEnvelope("ScaleVariation_sChannel",
							  kScaleVar1, kScaleVar2, kScaleVar3,
							  kScaleVar4, kScaleVar5, kScaleVar6));
    // sgtop t-channel
    ch1_bkg1->AddSystematic(new AtlHistFactorySystOneSided("aMCatNLO_tChannel", kaMCatNLO_tchan, AtlHistFactorySystOneSided::kFullDiff));
    // sgtop wt
    ch1_bkg2->AddSystematic(new AtlHistFactorySystOneSided("WtDS", kWtDS, AtlHistFactorySystOneSided::kFullDiff));
    // sgtop ttbar
    ch1_bkg3->AddSystematic(new AtlHistFactorySystOneSided("MCatNLO_ttbar", kMCatNLO_ttbar, AtlHistFactorySystOneSided::kFullDiff));
    ch1_bkg3->AddSystematic(new AtlHistFactorySystModelPair("ISR_FSR", kRadHi_ttbar, kRadLo_ttbar));
    
    ch1->AddSample(ch1_sig);
    ch1->AddSample(ch1_bkg1);
    ch1->AddSample(ch1_bkg2);
    ch1->AddSample(ch1_bkg3);
    ch1->AddSample(ch1_bkg4);
    ch1->AddSample(ch1_bkg5);
    ch1->AddSample(ch1_bkg6);

    // Add channels to measurement
    meas->AddChannel(ch1);

    // Finally, add measurement
    main->SetMeasurement(meas);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
