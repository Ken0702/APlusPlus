#include "../draw/PlotHelper.C"
void plot_llh_wjets_lnu(const char* infile = "../files/MCPlotter_wjets_lnu.root",
                        Bool_t do_scale = kFALSE) {

    // File
    TFile *f = TFile::Open(infile);

    PlotHelper *plot = new PlotHelper();
    TString cname = TString("MemLikelihoods_schan_wjets_lnu");

    // ========================
    //  Change global settings
    // ========================
    
    // Fit results
//    plot->SetScaleFactors(PlotHelper::kSchanWCR_lnu, 1);

    // Names of legend entries
    TList *lnames = new TList;
    lnames->Add(new TObjString("Data"));
    lnames->Add(new TObjString("#it{s}-channel"));
    lnames->Add(new TObjString("#it{t}-channel"));
    lnames->Add(new TObjString("Wt"));
    lnames->Add(new TObjString("t#bar{t}"));
    lnames->Add(new TObjString("W+jets light flavour"));
    lnames->Add(new TObjString("W+jets heavy flavour"));
    lnames->Add(new TObjString("Z+jets, diboson"));
    lnames->Add(new TObjString("Multi-jet"));
    lnames->Add(new TObjString("Sim. + norm. uncertainty"));
    plot->fgLegendNames = lnames;

    // Set Region
    plot->fgRegion = new TString("#it{s}-chan. W+jets VR, l+jets");

    // Change AtlasStyle locally
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.2);
    AtlasStyle->SetTitleYOffset(1.2);
    gROOT->SetStyle("ATLAS");

    // set scale factors
    PlotHelper::ERegion sfchoice = PlotHelper::kSchanWCR_lnu;    
    if ( do_scale ) {
	plot->SetScaleFactors(sfchoice);
    } else {
	cname.Append("_unscaled");
    }

    // Open Canvas
    TCanvas *c = new TCanvas(cname.Data(), "c");
    c->SaveAs(Form("%s.pdf[", c->GetName()));
    
    // ================
    //  Start Plotting
    // ================

    // s-channel 2j
    plot = new PlotHelper(f, "MemLikelihoods/logllh_sChannel2j");
    plot->fXTitle = new TString("Log_{10}(L_{#it{s}-channel 2j})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 8000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // s-channel 3j
    plot = new PlotHelper(f, "MemLikelihoods/logllh_sChannel3j");
    plot->fXTitle = new TString("Log_{10}(L_{#it{s}-channel 3j})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 10000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // t-channel 4FS
    plot = new PlotHelper(f, "MemLikelihoods/logllh_tChannel4FS");
    plot->fXTitle = new TString("Log_{10}(L_{#it{t}-channel})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 9000.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // ttbar SL
    plot = new PlotHelper(f, "MemLikelihoods/logllh_ttbarSL");
    plot->fXTitle = new TString("Log_{10}(L_{t#bar{t}, SL})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 12000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // ttbar DL
    plot = new PlotHelper(f, "MemLikelihoods/logllh_ttbarDL");
    plot->fXTitle = new TString("Log_{10}(L_{t#bar{t}, DL})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 8000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // Wjj
    plot = new PlotHelper(f, "MemLikelihoods/logllh_Wjj");
    plot->fXTitle = new TString("Log_{10}(L_{Wjj})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 10000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);


    // Wcj
    plot = new PlotHelper(f, "MemLikelihoods/logllh_Wcj");
    plot->fXTitle = new TString("Log_{10}(L_{Wcj})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 10000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);


    // Wbb
    plot = new PlotHelper(f, "MemLikelihoods/logllh_Wbb");
    plot->fXTitle = new TString("Log_{10}(L_{Wb#bar{b}})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 10000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // Close Canvas
    c->SaveAs(Form("%s.pdf]", c->GetName()));
}
