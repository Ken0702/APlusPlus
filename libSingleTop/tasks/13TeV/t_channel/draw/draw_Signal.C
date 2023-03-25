#include "PlotHelper.C"
void draw_Signal(const char* infile = "MCPlotter_Signal.root",
		 Bool_t is_wjetsMerged = kFALSE,
		 const char* channel = "") {
    //
    // Create (control?) plots for the signal region
    //
    TFile *f = TFile::Open(infile);
    PlotHelper *plot = new PlotHelper();
    PlotHelper::fgRegion = new TString("#it{s}-chan. signal region, l+jets");
    TString cname = TString("Signal_schan_lnu");

    // Adjust Atlas style
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.3);
    AtlasStyle->SetTitleYOffset(1.35);
    gROOT->SetStyle("ATLAS");

    // Add normalization uncertainty to plots
    PlotHelper::fgDrawNormUncertainty = kTRUE;

    // because of blinding, don't draw data for the signal region
    Option_t *option = "nodata";

    // directory structure for channel (el,mu,lep)
    TString dir = TString("bTagged");
    PlotHelper::ERegion sfchoice;
    if ( strcmp(channel, "_el") == 0 ) {
	dir += channel;
	cname.Append(channel);
    } else if ( strcmp(channel, "_mu") == 0 ) {
	dir += channel;
	cname.Append(channel);
    } else {
    }
    dir += "/Sel";
    
    // set if "wjetsLF" and "wjetsHF" are merged to "wjets"
    if ( is_wjetsMerged ) {
	PlotHelper::fgIs_wjetsMerged = kTRUE;
	cname.Append("_wjetsMerged");
    }
    
    // ============================
    //  start plotting on bTagged 
    // ============================
    TCanvas *c = new TCanvas(cname.Data(), "c");
    c->SaveAs(Form("%s.pdf[",c->GetName()));

    // Lepton pt
    plot = new PlotHelper(f, Form("%s/h_sel_Lepton_pt", dir.Data()));
    plot->fXTitle = new TString("Lepton #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 30000.;
    plot->fXmin = 30.;
    plot->fXmax = 150.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->Draw(option);
    plot->Save(c);

    // Lepton eta
    plot = new PlotHelper(f, Form("%s/h_sel_Lepton_eta", dir.Data()));
    plot->fXTitle = new TString("Lepton #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.04");
    plot->fYmin = 0.;
    plot->fYmax = 16000.;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->SetLegendPos(0.75, 0.46, 0.93, 0.93);
    plot->Draw(option);
    plot->Save(c);

    // mtW
    plot = new PlotHelper(f, Form("%s/h_sel_MtW", dir.Data()));
    plot->fXTitle = new TString("#font[52]{m}_{T}^{W} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 15000.;
    plot->fXmin = 0.;
    plot->fXmax = 210.;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.64, 0.45, 0.9, 0.91);
    plot->Draw(option);
    plot->Save(c);
  
    // met
    plot = new PlotHelper(f, Form("%s/h_sel_met", dir.Data()));
    plot->fXTitle = new TString("#font[52]{E}_{T}^{miss} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 22000.;
    plot->fXmin = 30.;
    plot->fXmax = 200.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetLegendPos(0.58, 0.40, 0.92, 0.91);
    plot->Draw(option);
    plot->Save(c);

    // ======
    //  Jets
    // ======

    plot = new PlotHelper(f, Form("%s/h_sel_LeadingJet_pt", dir.Data()));
    plot->fXTitle = new TString("Leading Jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 15000.;
    plot->fXmin = 40.;
    plot->fXmax = 220.;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.3;
    plot->fRebin = 2;
    plot->SetLegendPos(0.62, 0.45, 0.94, 0.91);
    plot->Draw(option);
    plot->Save(c);


    plot = new PlotHelper(f, Form("%s/h_sel_LeadingJet_eta", dir.Data()));
    plot->fXTitle = new TString("Leading Jet #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.4");
    plot->fYmin = 0.;
    plot->fYmax = 18000.;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.7;
    plot->fRatioYmax = 1.3;
    plot->SetLegendPos(0.735, 0.482, 0.945, 0.933);
    plot->Draw(option);
    plot->Save(c);


    plot = new PlotHelper(f, Form("%s/h_sel_2ndLeadingJet_pt", dir.Data()));
    plot->fXTitle = new TString("Sub-leading Jet #font[52]{p}_{T} [GeV]");
    plot->fYTitle = new TString("Events / 10 GeV");
    plot->fYmin = 0.;
    plot->fYmax = 30000.;
    plot->fXmin = 30.;
    plot->fXmax = 220.;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.58, 0.40, 0.92, 0.91);
//    plot->SetAtlasCMSLumiX(0.3);
    plot->Draw(option);
    plot->Save(c);

    plot = new PlotHelper(f, Form("%s/h_sel_2ndLeadingJet_eta", dir.Data()));
    plot->fXTitle = new TString("Sub-leading Jet #font[152]{h}");
    plot->fYTitle = new TString("Events / 0.4");
    plot->fYmin = 0.;
    plot->fYmax = 18000.;
    plot->fXmin = -2.5;
    plot->fXmax = +2.5;
    plot->fRatioYmin = 0.8;
    plot->fRatioYmax = 1.2;
    plot->SetLegendPos(0.75, 0.46, 0.93, 0.93);
    plot->Draw(option);
    plot->Save(c);

    // Delta R
    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_Lepton_LeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Lepton, Leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 16000.;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.545, 0.933);
    plot->SetAtlasCMSLumiX(0.55);
    plot->Draw(option);
    plot->Save(c);


    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_Lepton_2ndLeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Lepton, Sub-leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 18000.;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.545, 0.933);
    plot->SetAtlasCMSLumiX(0.55);
    plot->Draw(option);
    plot->Save(c);


    plot = new PlotHelper(f, Form("%s/h_sel_DeltaR_LeadingJet_2ndLeadingJet", dir.Data()));
    plot->fXTitle = new TString("#font[152]{D}#font[52]{R} (Leading Jet, Sub-leading Jet)");
    plot->fYTitle = new TString("Events / 0.2");
    plot->fYmin = 0.;
    plot->fYmax = 18000.;
    plot->fXmin = 0.4;
    plot->fXmax = 4.0;
    plot->fRatioYmin = 0.5;
    plot->fRatioYmax = 1.5;
    plot->fRebin = 2;
    plot->SetLegendPos(0.2, 0.482, 0.545, 0.933);
    plot->SetAtlasCMSLumiX(0.55);
    plot->Draw(option);
    plot->Save(c);


    // Close pdf file
    c->SaveAs(Form("%s.pdf]", c->GetName()));

}
