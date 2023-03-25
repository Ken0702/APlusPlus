void draw_CtrlZjets3L(const char* infile = "MCPlotter_CtrlZjets3L.root") {
    //
    // Create final plots for Z+jets trilepton ctrl region
    //
    TFile *f = TFile::Open(infile);
    gROOT->LoadMacro("PlotHelper.C");
    HepDataMCPlot *hep;
    PlotHelper *plot = new PlotHelper();
    PlotHelper::fgRegion = new TString("Z+jets CR, Trilepton");

    // Adjust Atlas style
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.3);
    AtlasStyle->SetTitleYOffset(1.1);

    //////////////////////////////////////////////////////////////////////
    // No cuts
    //////////////////////////////////////////////////////////////////////
    
    // Open canvas and pdf file
    TCanvas *c_nocuts = new TCanvas("CtrlZjets3L_nocuts", "Z+jets trilepton ctrl region, no cuts");
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
    plot->Draw();
    plot->Save(c_nocuts);

    // Leading lepton Eta
    plot = new PlotHelper(f, "NoCuts/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 3.5e5;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 1.0;
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
    plot->Draw();
    plot->Save(c_nocuts);

    // Close pdf file
    c_nocuts->SaveAs(Form("%s.pdf]", c_nocuts->GetName()));


    //////////////////////////////////////////////////////////////////////
    // Pre-tagged
    //////////////////////////////////////////////////////////////////////
    
    TCanvas *c_pretag = new TCanvas("CtrlZjets3L_pretag",
				    "Z+jets trilepton ctrl region, pre-tagged");
    c_pretag->SaveAs(Form("%s.pdf[", c_pretag->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "PreTag/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 25.;
    plot->fXmax = 160.;
    plot->fYmin = 0.;
    plot->fYmax = 1e5;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Leading lepton eta
    plot = new PlotHelper(f, "PreTag/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 2e4;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->Draw();
    plot->Save(c_pretag);

    // ====
    // Jets
    // ====
    
    // Leading jet pt
    plot = new PlotHelper(f, "PreTag/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 1.8e5;
    plot->fRatioYmin = -0.1;
    plot->fRatioYmax = 0.3;
    plot->Draw();
    plot->Save(c_pretag);
    
    // Leading jet eta
    plot = new PlotHelper(f, "PreTag/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 2e4;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.2;
    plot->Draw();
    plot->Save(c_pretag);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "PreTag/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 2.5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 90.;
    plot->fYmin = 0.;
    plot->fYmax = 8e4;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.4;
    plot->Draw();
    plot->Save(c_pretag);

    // MtW
    plot = new PlotHelper(f, "PreTag/met/h_MtW");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 0.;
    plot->fYmax = 6e4;
    plot->fRatioYmin = -0.25;
    plot->fRatioYmax = 0.25;
    plot->Draw();
    plot->Save(c_pretag);

    // ================
    // Di-lepton system
    // ================

    //
    // Opposite charge
    //
    
    // Di-lepton mass
    plot = new PlotHelper(f, "PreTag/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}(lep,lep) [GeV]");
    plot->fYTitle = new TString("Events / 2 GeV");
    plot->fYmin = 0.;
    plot->fXmin = 0.;
    plot->fXmax = 200.;
    plot->fRatioYmin = -0.8;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetLegendPos(0.625, 0.428882, 0.935345, 0.930612);
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_pretag);

    // Di-lepton DeltaPhi
    plot = new PlotHelper(f, "PreTag/Z0_best/all/h_Z_DeltaPhi");
    plot->fYmin = 10;
    plot->fYmax = 6e4;
    plot->fRatioYmin = -0.5;
    plot->fRatioYmax = 2.0;
    plot->fRebin = 2;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_pretag);

    // Close pdf file
    c_pretag->SaveAs(Form("%s.pdf]", c_pretag->GetName()));

    
    //////////////////////////////////////////////////////////////////////
    // All cuts
    //////////////////////////////////////////////////////////////////////
    
    TCanvas *c_allcuts = new TCanvas("CtrlZjets3L_allcuts",
				     "Z+jets trilepton ctrl region, all cuts");
    c_allcuts->SaveAs(Form("%s.pdf[", c_allcuts->GetName()));

    // =======
    // Leptons
    // =======
    
    // Leading lepton pt
    plot = new PlotHelper(f, "AllCuts/leptons/h_lep1_Pt");
    plot->fXTitle = new TString("Leading lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 25.;
    plot->fXmax = 160.;
    plot->fYmin = 0.;
    plot->fYmax = 5e3;
    plot->fRatioYmin = -0.2;
    plot->fRatioYmax = 0.5;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Leading lepton eta
    plot = new PlotHelper(f, "AllCuts/leptons/h_lep1_Eta");
    plot->fYmin = 0.;
    plot->fYmax = 1500;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.8;
    plot->Draw();
    plot->Save(c_allcuts);

    // ====
    // Jets
    // ====
    
    // Leading jet pt
    plot = new PlotHelper(f, "AllCuts/jets/h_jet1_Pt");
    plot->fXTitle = new TString("Leading jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fYmin = 0.;
    plot->fYmax = 9000;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.8;
    plot->Draw();
    plot->Save(c_allcuts);
    
    // Leading jet eta
    plot = new PlotHelper(f, "AllCuts/jets/h_jet1_Eta");
    plot->fXmin = -2.5;
    plot->fXmax = 2.5;
    plot->fYmin = 0.;
    plot->fYmax = 1500;
    plot->fRatioYmin = -0.4;
    plot->fRatioYmax = 0.8;
    plot->Draw();
    plot->Save(c_allcuts);

    // ===========
    // Energy sums
    // ===========
    
    // MET
    plot = new PlotHelper(f, "AllCuts/met/h_MET_mag");
    plot->fYTitle = new TString("Events / 2.5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 100.;
    plot->fYmin = 1.;
    plot->fYmax = 2e4;
    plot->fRatioYmin = -0.6;
    plot->fRatioYmax = 0.8;
    plot->SetLegendPos(0.2, 0.2, 0.5, 0.7);
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // MtW
    plot = new PlotHelper(f, "AllCuts/met/h_MtW");
    plot->fYTitle = new TString("Events / 5 GeV");
    plot->fXmin = 0.;
    plot->fXmax = 180.;
    plot->fYmin = 1.;
    plot->fYmax = 2e4;
    plot->fRatioYmin = -0.25;
    plot->fRatioYmax = 0.6;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // ================
    // Di-lepton system
    // ================

    //
    // Opposite charge
    //
    
    // Di-lepton mass, lin. scale
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}(lep,lep) [GeV]");
    plot->fYTitle = new TString("Events / 2 GeV");
    plot->fYmin = 0;
    plot->fYmax = 1e4;
    plot->fXmin = 80.;
    plot->fXmax = 110.;
    plot->fRatioYmin = -0.3;
    plot->fRatioYmax = 0.7;
    plot->fRebin = 2;
    plot->Draw();
    plot->Save(c_allcuts);

    // Di-lepton mass, log scale
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_M");
    plot->fXTitle = new TString("#font[52]{M}(lep,lep) [GeV]");
    plot->fYTitle = new TString("Events / 2 GeV");
    plot->fXmin = 70.;
    plot->fXmax = 110.;
    plot->fRatioYmin = -0.3;
    plot->fRatioYmax = 0.7;
    plot->fRebin = 2;
    plot->SetLegendPos(0.64, 0.41, 0.94, 0.91);
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // Di-lepton DeltaPhi
    plot = new PlotHelper(f, "AllCuts/Z0_best/all/h_Z_DeltaPhi");
    plot->fYmin = 0.1;
    plot->fYmax = 2e5;
    plot->fRatioYmin = -0.5;
    plot->fRatioYmax = 1.8;
    plot->fRebin = 2;
    plot->Draw();
    plot->SetLogy(kTRUE);
    plot->Save(c_allcuts);

    // Close pdf file
    c_nocuts->SaveAs(Form("%s.pdf]", c_allcuts->GetName()));
}
