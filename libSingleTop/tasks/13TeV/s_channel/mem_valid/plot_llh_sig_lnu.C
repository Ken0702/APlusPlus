#include "../draw/PlotHelper.C"
void plot_llh_sig_lnu(const char* infile = "../files/MCPlotter_sig_lnu.root",
                      const char* suffix = "") {

    // File
    TFile *f = TFile::Open(infile);

//    gROOT->LoadMacro("PlotHelper.C");

    HepDataMCPlot *hep;
    PlotHelper *plot = new PlotHelper();

    // ========================
    //  Change global settings
    // ========================
    
    // Fit results
//    plot->SetScaleFactors(PlotHelper::kSchanSR_lnu, PlotHelper::kNoReweighting);

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

    TList *lnames_nodata = new TList;
    lnames_nodata->Add(new TObjString("#it{s}-channel"));
    lnames_nodata->Add(new TObjString("#it{t}-channel"));
    lnames_nodata->Add(new TObjString("Wt"));
    lnames_nodata->Add(new TObjString("t#bar{t}"));
    lnames_nodata->Add(new TObjString("W+jets light flavour"));
    lnames_nodata->Add(new TObjString("W+jets heavy flavour"));
    lnames_nodata->Add(new TObjString("Z+jets, diboson"));
    lnames_nodata->Add(new TObjString("Multi-jet"));
    lnames_nodata->Add(new TObjString("Sim. uncertainty"));
    plot->fgLegendNames = lnames_nodata;

    // Set Region
    plot->fgRegion = new TString("#it{s}-chan. SR, l+jets");

    // Change AtlasStyle locally
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.2);
    AtlasStyle->SetTitleYOffset(1.2);
    
    // Open Canvas
    TCanvas *c = new TCanvas(Form("MemLikelihoods_schan_signal_lnu%s", suffix), "c");
    c->SaveAs(Form("%s.pdf[", c->GetName()));
    
    // ================
    //  Start Plotting
    // ================

    // s-channel 2j
    plot = new PlotHelper(f, "MemLlh/logllh_sChannel2j");
    plot->fXTitle = new TString("Log_{10}(L_{#it{s}-channel 2j})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 15000.;
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
    plot = new PlotHelper(f, "MemLlh/logllh_sChannel3j");
    plot->fXTitle = new TString("Log_{10}(L_{#it{s}-channel 3j})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 16000.;
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
    plot = new PlotHelper(f, "MemLlh/logllh_tChannel4FS");
    plot->fXTitle = new TString("Log_{10}(L_{#it{t}-channel})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 16000.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.20, 0.48, 0.48, 0.93); 
    plot->Draw();
    plot->Save(c);

    // ttbar SL
    plot = new PlotHelper(f, "MemLlh/logllh_ttbarSL");
    plot->fXTitle = new TString("Log_{10}(L_{t#bar{t}, SL})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 20000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.18, 0.48, 0.45, 0.93); 
    plot->Draw();
    plot->Save(c);

    // ttbar DL
    plot = new PlotHelper(f, "MemLlh/logllh_ttbarDL");
    plot->fXTitle = new TString("Log_{10}(L_{t#bar{t}, DL})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 25000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.18, 0.48, 0.45, 0.93); 
    plot->Draw();
    plot->Save(c);

    // Wjj
    plot = new PlotHelper(f, "MemLlh/logllh_Wjj");
    plot->fXTitle = new TString("Log_{10}(L_{Wjj})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 15000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.18, 0.48, 0.45, 0.93); 
    plot->Draw();
    plot->Save(c);


    // Wcj
    plot = new PlotHelper(f, "MemLlh/logllh_Wcj");
    plot->fXTitle = new TString("Log_{10}(L_{Wcj})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 15000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.18, 0.48, 0.45, 0.93); 
    plot->Draw();
    plot->Save(c);


    // Wbb
    plot = new PlotHelper(f, "MemLlh/logllh_Wbb");
    plot->fXTitle = new TString("Log_{10}(L_{Wb#bar{b}})");
    plot->fYTitle = new TString("Events / 0.6");
    plot->fYmin = 0.;
    plot->fYmax = 16000.;
    // plot->fXmin = 30.;
    // plot->fXmax = 125.;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->fRebin = 2;
    plot->SetAtlasCMSLumiX(0.5);
    plot->SetLegendPos(0.18, 0.48, 0.45, 0.93); 
    plot->Draw();
    plot->Save(c);

    // Close Canvas
    c->SaveAs(Form("%s.pdf]", c->GetName()));
}
