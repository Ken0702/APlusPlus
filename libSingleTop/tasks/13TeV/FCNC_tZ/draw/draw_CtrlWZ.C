void draw_CtrlWZ(const char* infile = "MCPlotter_CtrlWZ.root") {
    //
    // Create final plots for the signal region
    //
    TFile *f = TFile::Open(infile);
    gROOT->LoadMacro("PlotHelper.C");
    HepDataMCPlot *hep;
    PlotHelper *plot = new PlotHelper();
    PlotHelper::fgRegion = new TString("WZ Diboson CR, Trilepton");
    
	// Adjust Atlas style
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.3);
    AtlasStyle->SetTitleYOffset(1.1);

    //////////////////////////////////////////////////////////////////////
    // No cuts
    //////////////////////////////////////////////////////////////////////
    Float_t scale_nocuts = 1000.;
    
    // Open canvas and pdf file
    TCanvas *c_nocuts = new TCanvas("CtrlWZ_nocuts", "FCNC tZ WZ diboson ctrl region, no cuts");
    c_nocuts->SaveAs(Form("%s.pdf[", c_nocuts->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "NoCuts/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 3e6;
    plot->fXmin = 25.;
    plot->fXmax = 150.;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 1.0;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);

    // Leading lepton Eta
    plot = new PlotHelper(f, "NoCuts/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 3.5e5;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 1.0;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);

    // ====
    // Jets
    // ====

    // Leading jet pt
    plot = new PlotHelper(f, "NoCuts/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 4e6;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.8;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);
    
    // Leading jet eta
    plot = new PlotHelper(f, "NoCuts/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 4e5;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 1.;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "NoCuts/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 2.5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 90.;
    plot->fYmin = 0.;
    plot->fYmax = 1.5e6;
    plot->fRatioYmin = -3;
    plot->fRatioYmax = 10.;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);

    // MtW
    plot = new PlotHelper(f, "NoCuts/met/h_MtW");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 0.;
    plot->fYmax = 1e6;
    plot->fRatioYmin = -3;
    plot->fRatioYmax = 10;
    plot->fScaleOverlay = scale_nocuts;
    plot->Draw();
    plot->Save(c_nocuts);

    // Close pdf file
    c_nocuts->SaveAs(Form("%s.pdf]", c_nocuts->GetName()));


    //////////////////////////////////////////////////////////////////////
    // Pre-selected
    //////////////////////////////////////////////////////////////////////
    Float_t scale_presel = 10.;
    
    TCanvas *c_presel = new TCanvas("CtrlWZ_presel",
				    "FCNC tZ WZ diboson ctrl region, pre-selected");
    c_presel->SaveAs(Form("%s.pdf[", c_presel->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "PreSel/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 25.;
    plot->fXmax = 160.;
    plot->fYmin = 0.;
    plot->fYmax = 80.;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);
    
    // Leading lepton eta
    plot = new PlotHelper(f, "PreSel/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 120;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);

    // ====
    // Jets
    // ====
    
    // Leading jet pt
    plot = new PlotHelper(f, "PreSel/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 100.;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 0.3;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);
    
    // Leading jet eta
    plot = new PlotHelper(f, "PreSel/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 60.;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "PreSel/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 90.;
    plot->fYmin = 0.;
    plot->fYmax = 105.;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.4;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);

    // MtW
    plot = new PlotHelper(f, "PreSel/met/h_MtW");
    plot->fYTitle = new TString("Events / 20 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 0.;
    plot->fYmax = 100.;
    plot->fRatioYmin = -0.25;
    plot->fRatioYmax = 0.25;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_presel;
    plot->Draw();
    plot->Save(c_presel);

    // Close pdf file
    c_presel->SaveAs(Form("%s.pdf]", c_presel->GetName()));

    //////////////////////////////////////////////////////////////////////
    // Pre-tagged
    //////////////////////////////////////////////////////////////////////
    Float_t scale_pretag = 1.;
    
    TCanvas *c_pretag = new TCanvas("CtrlWZ_pretag",
				    "FCNC tZ WZ diboson ctrl region, pre-tagged");
    c_pretag->SaveAs(Form("%s.pdf[", c_pretag->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "PreTag/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 25.;
    plot->fXmax = 160.;
    plot->fYmin = 0.;
    plot->fYmax = 70;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Leading lepton eta
    plot = new PlotHelper(f, "PreTag/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 60.;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);

    // ====
    // Jets
    // ====
    
    // Leading jet pt
    plot = new PlotHelper(f, "PreTag/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 90.;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Leading jet eta
    plot = new PlotHelper(f, "PreTag/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 65.;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "PreTag/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 90.;
    plot->fYmin = 0.;
    plot->fYmax = 90.;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.4;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);

    // MtW
    plot = new PlotHelper(f, "PreTag/met/h_MtW");
    plot->fYTitle = new TString("Events / 20 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 0.;
    plot->fYmax = 90.;
    plot->fRatioYmin = -0.25;
    plot->fRatioYmax = 0.25;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);

    // =================
    // Z0 best candidate
    // =================
    
    // Invariant mass, linear
    plot = new PlotHelper(f, "PreTag/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}_{Z} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 200.;
    plot->fYmin = 0.;
    plot->fYmax = 130.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Invariant mass, logy
    plot = new PlotHelper(f, "PreTag/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}_{Z} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 200.;
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_pretag);

    // Di-lepton DeltaPhi
    plot = new PlotHelper(f, "PreTag/Z0_best/all/h_Z_DeltaPhi");
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -0.5;
    plot->fRatioYmax = 2.0;
    plot->fRebin = 10;
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_pretag);

    // ===================
    // Top-quark candidate
    // ===================
    
    // Invariant top mass, linear
    plot = new PlotHelper(f, "PreTag/top/h_top_M");
    plot->fXTitle = new TString("#font[52]{M}_{top} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 50.;
    plot->fXmax = 350.;
    plot->fYmin = 0.;
    plot->fYmax = 30.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Invariant W mass, logy
    plot = new PlotHelper(f, "PreTag/top/h_W_M");
    plot->fXTitle = new TString("#font[52]{M}_{W} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 150.;
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_pretag);

    // Neutrino Pz
    plot = new PlotHelper(f, "PreTag/top/h_nu_Pz");
    plot->fXTitle = new TString("#font[52]{p}_{z}(#nu) [GeV]");
    plot->fYTitle = new TString("Events / 20 GeV");
    plot->fXmin = -200.;
    plot->fXmax =  200.;
    plot->fYmin = 0.;
    plot->fYmax = 60.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 10;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_pretag;
    plot->Draw();
    plot->Save(c_pretag);

    // Close pdf file
    c_pretag->SaveAs(Form("%s.pdf]", c_pretag->GetName()));

    
    //////////////////////////////////////////////////////////////////////
    // All cuts
    //////////////////////////////////////////////////////////////////////
    Float_t scale_allcuts = 1.;
    
    TCanvas *c_allcuts = new TCanvas("CtrlWZ_allcuts",
				     "FCNC tZ WZ diboson ctrl region, all cuts");
    c_allcuts->SaveAs(Form("%s.pdf[", c_allcuts->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "AllCuts/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 25.;
    plot->fXmax = 160.;
    plot->fYmin = 0.;
    plot->fYmax = 10;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Leading lepton eta
    plot = new PlotHelper(f, "AllCuts/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 20.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);

    // ====
    // Jets
    // ====
    
    // Leading jet pt
    plot = new PlotHelper(f, "AllCuts/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 12.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 2;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Leading jet eta
    plot = new PlotHelper(f, "AllCuts/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 20.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 5;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "AllCuts/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 90.;
    plot->fYmin = 0.;
    plot->fYmax = 12.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);

    // MtW
    plot = new PlotHelper(f, "AllCuts/met/h_MtW");
    plot->fYTitle = new TString("Events / 20 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 0.;
    plot->fYmax = 12.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 4;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);

    // =================
    // Z0 best candidate
    // =================
    
    // Invariant mass, linear
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}_{Z} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 200.;
    plot->fYmin = 0.;
    plot->fYmax = 50.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Invariant mass, logy
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}_{Z} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 200.;
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // Di-lepton DeltaPhi
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_DeltaPhi");
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 10;
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // ===================
    // Top-quark candidate
    // ===================
    
    // Invariant top mass, linear
    plot = new PlotHelper(f, "AllCuts/top/h_top_M");
    plot->fXTitle = new TString("#font[52]{M}_{top} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 50.;
    plot->fXmax = 350.;
    plot->fYmin = 0.;
    plot->fYmax = 16.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Invariant W mass, logy
    plot = new PlotHelper(f, "AllCuts/top/h_W_M");
    plot->fXTitle = new TString("#font[52]{M}_{W} [GeV]");
    plot->fYTitle = new TString("Events / 4 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 150.;
    plot->fYmin = 0.1;
    plot->fYmax = 200.;
    plot->fRatioYmin = -1.4;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 4;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // Neutrino Pz
    plot = new PlotHelper(f, "AllCuts/top/h_nu_Pz");
    plot->fXTitle = new TString("#font[52]{p}_{z}(#nu) [GeV]");
    plot->fYTitle = new TString("Events / 20 GeV");
    plot->fXmin = -200.;
    plot->fXmax =  200.;
    plot->fYmin = 0.;
    plot->fYmax = 20.;
    plot->fRatioYmin = -1.8;
    plot->fRatioYmax =  1.8;
    plot->fRebin = 10;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->fScaleOverlay = scale_allcuts;
    plot->Draw();
    plot->Save(c_allcuts);

    // Close pdf file
    c_nocuts->SaveAs(Form("%s.pdf]", c_allcuts->GetName()));
}
