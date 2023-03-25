#include "../draw/PlotHelper.C"
// copied from ~/atlas/INT_ST_stchannelME_8TeV/figures/classification/scripts/
void plot_schan_wjets_discr(const char* infile = "../files/MCPlotter_wjets_discr.root",
                            Bool_t do_scale = kFALSE) {

    // File
    TFile *fwlnu = TFile::Open(infile);

    // TFile *fwlnu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/lnu/nominal/SgTop_s/MCPlotter.root");
    // TFile *fwenu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/enu/nominal/SgTop_s/MCPlotter.root");
    // TFile *fwmunu = new TFile("/rdsk/datm5/atlas/stamm/8TeV_s_channel/v15f/wjets/AtlSgTop_sChannelAnalysis/MemDiscAnalysis_wjets_reweighted/2Jets/munu/nominal/SgTop_s/MCPlotter.root");

    PlotHelper *plot = new PlotHelper();
    TString cname = TString("MemDiscriminant_schan_wjets");

    // Change AtlasStyle locally
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.2);
    AtlasStyle->SetTitleYOffset(1.2);
    gROOT->SetStyle("ATLAS");

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


    // set scale factors
    PlotHelper::ERegion sfchoice = PlotHelper::kSchanWCR_lnu;    
    if ( do_scale ) {
	plot->SetScaleFactors(sfchoice);
	cname.Append("_scaled");
    }

    // Open Canvas
    TCanvas *c = new TCanvas(cname.Data(), "c");
    c->SaveAs(Form("%s.pdf[", c->GetName()));

    // All Plots
    // Rebinned, final x-axis

    // SF applied
//    plot->SetScaleFactors(PlotHelper::kSchanWCR_enu, 1);

    // Set Region
    plot->fgRegion = new TString("#it{s}-chan. W+jets VR, l+jets");

// S.K.: cont. change here 
    
    // Many bins, linear scale
    //
    // WARNING: Drawing with norm uncertainties will take ages!
    // --> This histogram has 20000 bins (x number of templates) --> Infinity
    //
    plot->fgDrawNormUncertainty = false;

    plot = new PlotHelper(fwlnu, "MemDiscriminant/ManyBins/sChannelProb_raw");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 50000;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.3);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    plot->fRebin = 400;
    plot->Draw("");
    plot->Save(c);

    // same as before but with logy
    plot = new PlotHelper(fwlnu, "MemDiscriminant/ManyBins/sChannelProb_raw");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.1;
    plot->fYmax = 100000;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.3);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    plot->fRebin = 400;
    plot->Draw("");
    plot->SetLogy();
    plot->Save(c);

    // Rebinned, final x-axis
    plot = new PlotHelper(fwlnu, "MemDiscriminant/sChannelProb_bins2N");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 15000;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.2);
    plot->SetLegendPos(0.59, 0.55, 0.9, 0.92);
    // plot->fRebin = 10;
    plot->Draw("");
//    plot->DrawSchannelXaxis(-1000.);
    plot->DrawSchannelXaxis(0.45, 0.);
    plot->Save(c);

    // Rebinned, final x-axis, zoom
    plot = new PlotHelper(fwlnu, "MemDiscriminant/sChannelProb_bins2N");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 5000;
    plot->fXmin = 0.4;
    plot->fXmax = 1;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.2;
    plot->SetAtlasCMSLumiX(0.2);
    plot->SetLegendPos(0.587, 0.560, 0.947, 0.929);
    // plot->fRebin = 10;
    plot->Draw("");
    // plot->DrawSchannelXaxis(-2000);
    plot->DrawSchannelXaxis(0.45, 0.);
    plot->Save(c);

    // old discr file
    plot = new PlotHelper(fwlnu, "MemDiscriminant/sChannelProb");
    plot->fXTitle = new TString("P(S|X)");
    plot->fYTitle = new TString("Events");
    plot->fYmin = 0.;
    plot->fYmax = 20000;
    plot->fRatioYmin = 0.6;
    plot->fRatioYmax = 1.4;
    plot->SetAtlasCMSLumiX(0.3);
    plot->SetLegendPos(0.59, 0.38, 0.9, 0.92);
    plot->Draw("");
    plot->DrawSchannelXaxis(0.45, 0.);
    plot->Save(c);

    c->SaveAs(Form("%s.pdf]", c->GetName()));
}
