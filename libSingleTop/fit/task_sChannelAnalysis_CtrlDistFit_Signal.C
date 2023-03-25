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
    TString OutputDir = "/rdsk/dats2/atlas/stamm/HistFactory/CtrlDistFit/Signal/";
    main->SetJobHomeDir(Form("%s/JobHome", OutputDir.Data()));
    
    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("sChannel_Analysis_CtrlDistSR",
								    "s-Channel Analysis Ctrl Distribution Fit, Signal Region",
								    OutputDir.Data());
    // Fit real data
    meas->SetUseAsimovData(kFALSE);
    
    // No parameter of interest
    meas->AddPOI("dummy_poi"); // apparently, histfactory needs a dummy poi
    
    // Histograms are already scaled to luminosity!
    // Thus, lumi = 1
    meas->SetLumiAndRelError(1., 0.028);

    // Create list of systematics for all channels (lnu)
    // Only nominal is used, no other systematics
    TList *Systs = new TList;
    // Nominal
    Systs->Add(new AtlHistFactorySystNominal("Nominal"));


    // Samples (valid for all channels)
    // All samples will be constrained by a log normal distribution
    // no signal process the ctrl distribution fit.
    AtlHistFactorySample *ch1_sig = new AtlHistFactorySample("sgtop_sChannel",
							     "norm_schan", 0.04);
    AtlHistFactorySample *ch1_bkg1 = new AtlHistFactorySample("sgtop_tChannel",
							      "norm_tchan", 0.04);
    AtlHistFactorySample *ch1_bkg2 = new AtlHistFactorySample("sgtop_WtChannel",
							      "norm_wt", 0.07);
    AtlHistFactorySample *ch1_bkg3 = new AtlHistFactorySample("ttbar",
							      "norm_ttbar", 0.06);
    AtlHistFactorySample *ch1_bkg4 = new AtlHistFactorySample("wjetsLF",
							      "norm_wjetsLF", 0.60);
    AtlHistFactorySample *ch1_bkg5 = new AtlHistFactorySample("wjetsHF",
							      "norm_wjetsHF", 0.60);
    AtlHistFactorySample *ch1_bkg6 = new AtlHistFactorySample("zjets_diboson",
							      "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch1_bkg7 = new AtlHistFactorySample("QCD",
							      "norm_QCD", 0.50);
    ch1_bkg7->SetIsData(kTRUE); // i.e. this is a data driven sample
                                // thus, all channel systematics are skipped
                                // Individual systematics can be added here
                                // Data sample must be added separatly


    // No dilepton veto
    // ----------------
    //
    // Signal channel e+jets (no veto)
    TString SignalDir_e_noveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto/AtlSgTop_sChannelAnalysis/2Jets/enu/";
    AtlHistFactoryChannel *ch1 = new AtlHistFactoryChannel("Signal_e_noveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_e_noveto);
    // no shape discriminants needed
    ch1->AddSystematics(Systs);

    // Add samples
    ch1->AddSample(ch1_sig);
    ch1->AddSample(ch1_bkg1);
    ch1->AddSample(ch1_bkg2);
    ch1->AddSample(ch1_bkg3);
    ch1->AddSample(ch1_bkg4);
    ch1->AddSample(ch1_bkg5);
    ch1->AddSample(ch1_bkg6);
    ch1->AddSample(ch1_bkg7);
    
    // Signal channel mu+jets (no veto)
    TString SignalDir_mu_noveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto/AtlSgTop_sChannelAnalysis/2Jets/munu/";
    AtlHistFactoryChannel *ch2 = new AtlHistFactoryChannel("Signal_mu_noveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_mu_noveto);
    // Only nominal
    ch2->AddSystematics(Systs);
    
    // Add samples (same config as electron channel)
    ch2->AddSample(ch1_sig);
    ch2->AddSample(ch1_bkg1);
    ch2->AddSample(ch1_bkg2);
    ch2->AddSample(ch1_bkg3);
    ch2->AddSample(ch1_bkg4);
    ch2->AddSample(ch1_bkg5);
    ch2->AddSample(ch1_bkg6);
    ch2->AddSample(ch1_bkg7);

    // Signal channel l+jets (no veto)
    TString SignalDir_lnu_noveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto/AtlSgTop_sChannelAnalysis/2Jets/lnu/";
    AtlHistFactoryChannel *ch3 = new AtlHistFactoryChannel("Signal_lnu_noveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_lnu_noveto);
    // Only nominal
    ch3->AddSystematics(Systs);
    
    // Add samples (same config as electron channel)
    ch3->AddSample(ch1_sig);
    ch3->AddSample(ch1_bkg1);
    ch3->AddSample(ch1_bkg2);
    ch3->AddSample(ch1_bkg3);
    ch3->AddSample(ch1_bkg4);
    ch3->AddSample(ch1_bkg5);
    ch3->AddSample(ch1_bkg6);
    ch3->AddSample(ch1_bkg7);

    // Signal channel e+jets (di-lepton veto)
    TString SignalDir_e_dileptonveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto/AtlSgTop_sChannelAnalysis/2Jets/enu/";
    AtlHistFactoryChannel *ch4 = new AtlHistFactoryChannel("Signal_e_dileptonveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_e_dileptonveto);
    // no shape discriminants needed
    ch4->AddSystematics(Systs);

    // Add samples
    ch4->AddSample(ch1_sig);
    ch4->AddSample(ch1_bkg1);
    ch4->AddSample(ch1_bkg2);
    ch4->AddSample(ch1_bkg3);
    ch4->AddSample(ch1_bkg4);
    ch4->AddSample(ch1_bkg5);
    ch4->AddSample(ch1_bkg6);
    ch4->AddSample(ch1_bkg7);
    
    // Signal channel mu+jets (di-lepton veto)
    TString SignalDir_mu_dileptonveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto/AtlSgTop_sChannelAnalysis/2Jets/munu/";
    AtlHistFactoryChannel *ch5 = new AtlHistFactoryChannel("Signal_mu_dileptonveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_mu_dileptonveto);
    // Only nominal
    ch5->AddSystematics(Systs);
    
    // Add samples (same config as electron channel)
    ch5->AddSample(ch1_sig);
    ch5->AddSample(ch1_bkg1);
    ch5->AddSample(ch1_bkg2);
    ch5->AddSample(ch1_bkg3);
    ch5->AddSample(ch1_bkg4);
    ch5->AddSample(ch1_bkg5);
    ch5->AddSample(ch1_bkg6);
    ch5->AddSample(ch1_bkg7);

    // Signal channel lnu+jets (di-lepton veto)
    TString SignalDir_lnu_dileptonveto = "/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto/AtlSgTop_sChannelAnalysis/2Jets/lnu/";
    AtlHistFactoryChannel *ch6 = new AtlHistFactoryChannel("Signal_lnu_dileptonveto",
     							   "bTagged/Sel/h_sel_MtW",
    							   SignalDir_lnu_dileptonveto);
    // Only nominal
    ch6->AddSystematics(Systs);
    
    // Add samples (same config as electron channel)
    ch6->AddSample(ch1_sig);
    ch6->AddSample(ch1_bkg1);
    ch6->AddSample(ch1_bkg2);
    ch6->AddSample(ch1_bkg3);
    ch6->AddSample(ch1_bkg4);
    ch6->AddSample(ch1_bkg5);
    ch6->AddSample(ch1_bkg6);
    ch6->AddSample(ch1_bkg7);

    // Add channels to measurement
    meas->AddChannel(ch1); // noveto e
    meas->AddChannel(ch2); //        mu
    meas->AddChannel(ch3); //        l
    meas->AddChannel(ch4); // di-lepton veto e
    meas->AddChannel(ch5); //                mu
    meas->AddChannel(ch6); //                l
    
    // Finally, add measurement
    main->SetMeasurement(meas);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}
