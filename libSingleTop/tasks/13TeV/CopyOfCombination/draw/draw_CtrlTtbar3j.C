#include "PlotHelper.C"
void draw_CtrlTtbar3j(const char* infile = "MCPlotter_CtrlTtbar3j.root",
		      Bool_t do_scale = kTRUE,
                      const char* channel = "") {
    //
    // Create control plots for the ttbar 3 jets control region
    //
    TFile *f = TFile::Open(infile);

    HepDataMCPlot *hep;
    PlotHelper *plot = new PlotHelper();
    PlotHelper::fgRegion = new TString("#it{s}-chan. ttbar 3 jets VR, l+jets");
    TString cname = TString("CtrlTtbar3j_schan_lnu");

    // Adjust Atlas style
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.3);
    AtlasStyle->SetTitleYOffset(1.1);

    // Add normalization uncertainty to plots
    PlotHelper::fgDrawNormUncertainty = kTRUE;

    // directory structure
    TString dir = TString("bTagged");
    PlotHelper::ERegion sfchoice;
    if ( strcmp(channel, "_el") == 0 ) {
	dir += channel;
	cname.Append(channel);
//	sfchoice = PlotHelper::kSchanTtbar3jCR_enu;
    } else if ( strcmp(channel, "_mu") == 0 ) {
	dir += channel;
	cname.Append(channel);
//	sfchoice = PlotHelper::kSchanTtbar3jCR_munu;
    } else {
	sfchoice = PlotHelper::kSchanTtbar3jCR_lnu;
    }
    sfchoice = PlotHelper::kSchanTtbar3jCR_lnu;
    dir += "/Sel";

    // set scale factors
    if ( do_scale ) {
	plot->SetScaleFactors(sfchoice);
    } else {
	cname.Append("_unscaled");
    }

    // set scale factor for reduced input samples
    Double_t sf = 1.;///20.;

    // ============================
    //  start plotting on bTagged 
    // ============================
    TCanvas *c = new TCanvas(cname.Data(), "c");
    c->SaveAs(Form("%s.pdf[",c->GetName()));

    // Lepton p_T
    plot = new PlotHelper(f, Form("%s/h_sel_Lepton_pt", dir.Data()));
    plot->fXTitle = new TString("Lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 90000.*sf;
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->Draw();
    plot->Save(c);

    // Lepton eta
    plot = new PlotHelper(f, Form("%s/h_sel_Lepton_eta", dir.Data()));
    plot->fXTitle = new TString("Lepton #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.04");
    plot->fYmin = 0.;
    plot->fYmax = 50000.*sf;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->SetLegendPos(0.75, 0.46, 0.93, 0.93);
    plot->Draw();
    plot->Save(c);

    // m_T^W
    plot = new PlotHelper(f, Form("%s/h_sel_MtW", dir.Data()));
    plot->fXTitle = new TString("#font[52]{m}_{T}^{W} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 50000.*sf;
    plot->fXmin = 0.;
    plot->fXmax = 210.;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.64, 0.45, 0.9, 0.91);
    plot->Draw();
    plot->Save(c);
  
    // E_T^miss
    plot = new PlotHelper(f, Form("%s/h_sel_met", dir.Data()));
    plot->fXTitle = new TString("#font[52]{E}_{T}^{miss} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 70000.*sf;
    plot->fXmin = 30.;
    plot->fXmax = 200.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    // plot->SetLegendPos(0.58, 0.40, 0.92, 0.91);
    plot->SetAtlasCMSLumiX(0.3);
    plot->Draw();
    plot->Save(c);

    // ======
    //  Jets
    // ======

    // 1st p_T
    plot = new PlotHelper(f, Form("%s/h_sel_LeadingJet_pt", dir.Data()));
    plot->fXTitle = new TString("Leading Jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 45000.*sf;
    plot->fXmin = 40.;
    plot->fXmax = 220.;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.3;
    plot->fRebin = 2;
    // plot->SetLegendPos(0.62, 0.45, 0.94, 0.91);
    plot->Draw();
    plot->Save(c);

    // 1st eta
    plot = new PlotHelper(f, Form("%s/h_sel_LeadingJet_eta", dir.Data()));
    plot->fXTitle = new TString("Leading Jet #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.4");
    plot->fYmin = 0.;
    plot->fYmax = 55000.*sf;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.3;
    plot->SetLegendPos(0.735, 0.482, 0.945, 0.933);
    plot->Draw();
    plot->Save(c);

    // 2nd p_T
    plot = new PlotHelper(f, Form("%s/h_sel_2ndLeadingJet_pt", dir.Data()));
    plot->fXTitle = new TString("Sub-leading Jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 60000.*sf;
    plot->fXmin = 30.;
    plot->fXmax = 220.;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    // plot->SetLegendPos(0.58, 0.40, 0.92, 0.91);
    plot->SetAtlasCMSLumiX(0.3);
    plot->Draw();
    plot->Save(c);

    // 2nd eta
    plot = new PlotHelper(f, Form("%s/h_sel_2ndLeadingJet_eta", dir.Data()));
    plot->fXTitle = new TString("Sub-leading Jet #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.4");
    plot->fYmin = 0.;
    plot->fYmax = 55000.*sf;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.8;
    plot->fRatioYmax = 1.2;
    plot->SetLegendPos(0.75, 0.46, 0.93, 0.93);
    plot->Draw();
    plot->Save(c);

    // Delta R lep - 1st jet
    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_Lepton_LeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Lepton, Leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 45000.*sf;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.5, 0.9);//, 0.545, 0.933);
    plot->SetAtlasCMSLumiX(0.65);
    plot->Draw();
    plot->Save(c);

    // Delta R lep - 2nd jet
    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_Lepton_2ndLeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Lepton, Sub-leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 45000.*sf;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.5, 0.9);
    plot->SetAtlasCMSLumiX(0.65);
    plot->Draw();
    plot->Save(c);

    // Delta R 1st jet - 2nd Jet
    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_LeadingJet_2ndLeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Leading Jet, Sub-leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 45000.*sf;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.5, 0.9);
    plot->SetAtlasCMSLumiX(0.65);
    plot->Draw();
    plot->Save(c);

    // Close pdf file
    c->SaveAs(Form("%s.pdf]", c->GetName()));
}
