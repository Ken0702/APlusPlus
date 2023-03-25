//
// HistFactory Task configuration for the SgTop 13TeV s-channel analysis
//
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2017 (C) Stephan Kaphle
//
// based on similar 8TeV analysis files by Soeren Stamm
//
AtlHistFactoryChannel* CreateChannel(const char* name,
				     const char* histname,
				     const char* dir,
				     TList *Systs,
				     Bool_t use_lognormal) {
    //
    // Helper function
    //
    AtlHistFactoryChannel *ch = new AtlHistFactoryChannel(name, histname, dir);
    ch->AddSystematics(Systs);
    
    // Same config for all channels
    // TO DO: look for right MC uncertainties
    AtlHistFactorySample *ch_sig = new AtlHistFactorySample("sgtop_sChannel",
							    "sgtop s-channel",
							    "norm_schan", 0.04);
    AtlHistFactorySample *ch_bkg1 = new AtlHistFactorySample("sgtop_tChannel",
							     "sgtop t-channel",
							     "norm_tchan", 0.04);
    AtlHistFactorySample *ch_bkg2 = new AtlHistFactorySample("sgtop_WtChannel",
							     "sgtop Wt",
							     "norm_wt", 0.07);
    AtlHistFactorySample *ch_bkg3 = new AtlHistFactorySample("ttbar",
							     "ttbar",
							     "norm_ttbar", 0.06);
    AtlHistFactorySample *ch_bkg4 = new AtlHistFactorySample("wjetsLF",
							     "wjetsLF",
							     "norm_wjetsLF", 0.30);
    AtlHistFactorySample *ch_bkg5 = new AtlHistFactorySample("wjetsHF",
							     "wjetsHF",
							     "norm_wjetsHF", 0.60);
    AtlHistFactorySample *ch_bkg6 = new AtlHistFactorySample("zjets_diboson",
							     "norm_zjets_diboson",
							     "norm_zjets_diboson", 0.60);
    AtlHistFactorySample *ch_bkg7 = new AtlHistFactorySample("QCD",
    							     "QCD",
    							     "norm_QCD", 0.40);
    ch_bkg7->SetIsData(kTRUE);

    // Use of lognormal interpolation:
    // false: Uses Exp.+Polynom., SF can be taken from yield table Post/Pre
    // true: Uses lognormal, SF are in logfile, use GetSFfromFit.sh
    ch_sig->SetUseLogNormal(use_lognormal);
    ch_bkg1->SetUseLogNormal(use_lognormal);
    ch_bkg2->SetUseLogNormal(use_lognormal);
    ch_bkg3->SetUseLogNormal(use_lognormal);
    ch_bkg4->SetUseLogNormal(use_lognormal);
    ch_bkg5->SetUseLogNormal(use_lognormal);
    ch_bkg6->SetUseLogNormal(use_lognormal);
    ch_bkg7->SetUseLogNormal(use_lognormal);
    
    // Add samples
    ch->AddSample(ch_sig);
    ch->AddSample(ch_bkg1);
    ch->AddSample(ch_bkg2);
    ch->AddSample(ch_bkg3);
    ch->AddSample(ch_bkg4);
    ch->AddSample(ch_bkg5);
    ch->AddSample(ch_bkg6);
    ch->AddSample(ch_bkg7);

    return ch;
}


void task_fit_ttbar3j() {
    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");

    // AtlTopLevelAnalysis options
    main->SetBuildHistFactoryTree(kTRUE);

    // set up directories
    char const* user = gSystem->Getenv("USER");
    char const* version = "v03_QCDtests";
    TString InputDir = Form("/rdsk/datm5/atlas/%s/13TeV_s_channel/%s/sgtop_schan_ttbar3j", user, version);    
    TString OutputDir = Form("/rdsk/datm4/atlas/%s/HistFactory/13TeV_s_channel/%s/sgtop_schan_ttbar3j", user, version);

    main->SetJobHomeDir(Form("%s/JobHome", OutputDir.Data()));
    main->SetTempDir(Form("/datc/atlas/%s/sgtop_schan_ttbar3j.tmp", user));
    const char* path_el = Form("%s/%s/3Jets/enu/", InputDir.Data(), main->GetName());
    const char* path_mu = Form("%s/%s/3Jets/munu/", InputDir.Data(), main->GetName());
    const char* path_l = Form("%s/%s/3Jets/lnu/", InputDir.Data(), main->GetName());
//    main->SetHistDir(Form("%s/hists", ));
//    main->SetInputDir(Form("%s/%s/input", main->GetHistDir()->Data(), main->GetName()));
    main->SetPlotDir(Form("%s/%s", InputDir.Data(), main->GetName()));

    AtlHistFactoryMeasurement *meas = new AtlHistFactoryMeasurement("s_channel_analysis_ttbar3j",
								    "s-channel analysis fit, ttbar 3 jets region",
								    OutputDir.Data());
    
    // No parameter of interest
    meas->AddPOI("dummy_poi"); // apparently, histfactory needs a dummy poi
    
    // Histograms are already scaled to luminosity!
    // Thus, lumi = 1
    meas->SetLumiAndRelError(1., 0.028);
    // AsimovData?
    meas->SetUseAsimovData(kFALSE);

    // Create list of systematics for all channels (lnu)
    // Only nominal is used, no other systematics
    TList *Systs = new TList;
    // Nominal
    Systs->Add(new AtlHistFactorySystNominal("Nominal"));


    // Create channels
    // TO DO: replace doubled path_l with path_mu, path_el
    //    OR: change function to work with only 1 channel
    Bool_t use_lognormal = kFALSE;
    // AtlHistFactoryChannel *ch1 = CreateChannel("Comb_El_MtW",
    // 					    "bTagged/Sel/h_sel_MtW",
    // 					    path_el,
    // 					    Systs, use_lognormal);
    AtlHistFactoryChannel *ch2 = CreateChannel("Comb_Mu_MtW",
					       "bTagged/Sel/h_sel_MtW",
					       path_l,
					       Systs, use_lognormal);
    AtlHistFactoryChannel *ch3 = CreateChannel("Comb_Lep_MtW",
					       "bTagged/Sel/h_sel_MtW",
					       path_l,
					       Systs, use_lognormal);
    
    
    // Add channels to measurement
    // meas->AddChannel(ch1); // Combination
    meas->AddChannel(ch2); //
    meas->AddChannel(ch3); //

    // Finally, add measurement
    main->SetMeasurement(meas);
    
    // dummy colors; needed to draw anything at all
    meas->SetDrawOptions("sgtop_sChannel", kOrange, 1001, kOrange);
    meas->SetDrawOptions("sgtop_tChannel", kAzure-3, 1001, kAzure-3);
    meas->SetDrawOptions("sgtop_WtChannel", kAzure-6, 1001, kAzure-6);
    meas->SetDrawOptions("ttbar", kRed-4, 1001, kRed-4);
    meas->SetDrawOptions("wjets", kGreen-3, 1001, kGreen-3);
    meas->SetDrawOptions("zjets_diboson", kOrange-3, 1001, kOrange-3);

    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}


