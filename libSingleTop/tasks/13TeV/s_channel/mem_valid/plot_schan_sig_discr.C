#include "../draw/PlotHelper.C"
//#include "PlotHelper.C"
// copied from ~/atlas/INT_ST_stchannelME_8TeV/figures/classification/scripts/
void plot_schan_sig_discr(const char* infile = "../files/MCPlotter_sig_discr.root",
                          const char* suffix = "", Double_t y_scale = 1.) {

    // File
    TFile *f = TFile::Open(infile);

    // TFile *fwlnu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/lnu/nominal/SgTop_s/MCPlotter.root");
    // TFile *fwenu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/enu/nominal/SgTop_s/MCPlotter.root");
    // TFile *fwmunu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/munu/nominal/SgTop_s/MCPlotter.root");

    // gROOT->LoadMacro("../../PlotHelper.C");

    HepDataMCPlot *hep = 0;
    PlotHelper *plot = new PlotHelper();

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

    // Change AtlasStyle locally
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.2);
    AtlasStyle->SetTitleYOffset(1.2);

    // Open Canvas
    TCanvas *c = new TCanvas(Form("MemDiscriminant_schan_sig_lnu%s", suffix), "c");
    c->SaveAs(Form("%s.pdf[", c->GetName()));

    // All Plots
    // Rebinned, final x-axis

    // SF applied
//    plot->SetScaleFactors(PlotHelper::kSchanWCR_enu, 1);

    // Set Region
    plot->fgRegion = new TString("#it{s}-chan. signal region, l+jets");

// S.K.: cont. change here 
    // scale bc of different cuts
    y_scale=0.5;
    
    // Many bins, linear scale
    //
    // WARNING: Drawing with norm uncertainties will take ages!
    // --> This histogram has 20000 bins (x number of templates) --> Infinity
    //
    plot->fgDrawNormUncertainty = false;

    plot = new PlotHelper(f, "MemDiscriminant/ManyBins/sChannelProb_raw");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 25000*y_scale;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.3);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    plot->fRebin = 400;
    plot->Draw("nodata");
    plot->Save(c);

    // same as before but with logy
    plot = new PlotHelper(f, "MemDiscriminant/ManyBins/sChannelProb_raw");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.1;
    plot->fYmax = 100000;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.3);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    plot->fRebin = 400;
    plot->Draw("nodata");
    plot->SetLogy();
    plot->Save(c);

    // Rebinned, final x-axis
//    plot = new PlotHelper(f, "MemDiscriminant/sChannelProb_bins2N");
    plot = new PlotHelper(f, "MemDiscriminant/sChannelProb");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 5000*y_scale;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.2);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    // plot->fRebin = 10;
    plot->Draw("nodata");
    plot->DrawSchannelXaxis(-120.);
//    plot->DrawSchannelXaxis(-100);
    plot->Save(c);

    // Rebinned, final x-axis, zoom
//    plot = new PlotHelper(f, "MemDiscriminant/sChannelProb_bins2N");
    plot = new PlotHelper(f, "MemDiscriminant/sChannelProb");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 5000*y_scale;
    plot->fXmin = 0.4;
    plot->fXmax = 1;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.2);
    plot->SetLegendPos(0.587, 0.560, 0.947, 0.929);
    // plot->fRebin = 10;
    plot->Draw("nodata");
    plot->DrawSchannelXaxis(-120);
    plot->Save(c);

    c->SaveAs(Form("%s.pdf]", c->GetName()));
}
