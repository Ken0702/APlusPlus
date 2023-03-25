#include "TMVA/ROCCurve.h"
#include <iostream>

TGraph* GetROCCurve(TH1F* hist_sig, TH1F* hist_bkg, const UInt_t points)
{
   
     const UInt_t ndivisions = points - 1;
     std::vector<Float_t> eps_sig(points);
     std::vector<Float_t> eps_bkg(points);
     // Fixed values.

     eps_sig[0] = 0.0;
     eps_sig[ndivisions] = 1.0;
     eps_bkg[0] = 1.0;
     eps_bkg[ndivisions] = 0.0;
 
     for(UInt_t i = 1; i < ndivisions; i++)
     {
         // Float_t threshold = -1.0 + i * 2.0 / (Float_t) ndivisions;
         Float_t threshold = i *1.0 / (Float_t) ndivisions;
         // Float_t true_positives = 0.0;
         // Float_t false_positives = 0.0;
         // Float_t true_negatives = 0.0;
         // Float_t false_negatives = 0.0;

         Float_t true_positives = hist_sig->Integral(hist_sig->FindBin(threshold), hist_sig->GetNbinsX());
         Float_t false_negatives = hist_sig->Integral(1, hist_sig->FindBin(threshold));
         Float_t false_positives = hist_bkg->Integral(hist_bkg->FindBin(threshold), hist_bkg->GetNbinsX());
         Float_t true_negatives = hist_bkg->Integral(1, hist_bkg->FindBin(threshold));
         
         // for(UInt_t j=0; j<fMvaS.size(); j++)
         // {
         //     if(fMvaS[j] > threshold)
         //         true_positives += 1.0;
         //     else
         //         false_negatives += 1.0;
 
         //     if(fMvaB[j] > threshold)
         //         false_positives += 1.0;
         //     else
         //         true_negatives += 1.0;
         // }
 
         eps_sig[ndivisions - i] = 0.0;
         if ((true_positives > 0.0) || (false_negatives > 0.0))
             eps_sig[ndivisions - i] =
                 true_positives / (true_positives + false_negatives);
 
         eps_bkg[ndivisions - i] =0.0;
         if ((true_negatives > 0.0) || (false_positives > 0.0))
             eps_bkg[ndivisions - i] =
                 true_negatives / (true_negatives + false_positives);
 
     }  
   
     // if(!fGraph)    fGraph=new TGraph(eps_sig.size(),&eps_sig[0],&eps_bkg[0]);
     // return fGraph;
     return new TGraph(eps_sig.size(),&eps_sig[0],&eps_bkg[0]);
}

void Overlay_roc_curve() {
    //
    //
    //

    // Adjust Atlas style
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    AtlasStyle->SetTitleXOffset(1.3);
    AtlasStyle->SetTitleYOffset(1.0);
    AtlasStyle->SetTextSize(0.037);
    gROOT->SetStyle("ATLAS");

    //
    // INPUT FILES
    //
    // this path contains links to the originals/combinations:
    // /rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged

    // // MC16a files
    // TString savename("ROC_MEM_v23lj_MC16a.pdf");
    // TString filename_sig("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16a/sgtop_sChannel.root");
    // TString filename_ttbar("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16a/ttbar.root");
    // TString filename_wjets("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16a/wjets.root");

    // // MC16d files
    // TString savename("ROC_MEM_v23lj_MC16d.pdf");
    // TString filename_sig("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16d/sgtop_sChannel.root");
    // TString filename_ttbar("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16d/ttbar.root");
    // TString filename_wjets("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default/sgtop_schan_signal/MemDisc/SgTop_s_wjetsMerged/mc16d/wjets.root");

    // combined files
    TString savename("ROC_MEM_v32_Thesis.pdf");
    TString filename_sig(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.65_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
    TString filename_sig2(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.1_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
    TString filename_sig3(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.5_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
    TString filename_sig4(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.8_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
    

    TString filename_bkg2(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.1_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/bkg.root");
    TString filename_bkg3(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.5_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/bkg.root");
    TString filename_bkg(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.65_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/bkg.root");
    TString filename_bkg4(" /rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.8_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/adapted_for_trexfitter_allhist/bkg.root");


    //
    // INPUT HISTOGRAMS
    //

    // TString histname("MemDiscriminant/sChannelProb");
    TString histname("MemDiscriminant/ManyBins/sChannelProb_raw");
    // TString histname_sig("MemDiscriminant/ManyBins/sChannelProb_raw");
    // TString histname_ttbar("MemDiscriminant/ManyBins/ttbarProb_raw");
    // TString histname_wjets("MemDiscriminant/ManyBins/wjetsProb_raw");
    TString histname_sig = histname;
    TString histname_sig3 = histname;
    TString histname_sig2 = histname;
    TString histname_bkg2 = histname;
    TString histname_bkg3 = histname;
    TString histname_bkg = histname;
    TString histname_bkg4 = histname;
    TString histname_sig4 = histname;



    TFile *file_sig = TFile::Open(filename_sig.Data());
    TH1F * hist_sig = (TH1F*)file_sig->Get(histname_sig.Data())->Clone();
    hist_sig->SetDirectory(0);
    file_sig->Close();

TFile *file_sig2 = TFile::Open(filename_sig2.Data());
    TH1F * hist_sig2 = (TH1F*)file_sig2->Get(histname_sig2.Data())->Clone();
    hist_sig2->SetDirectory(0);
    file_sig2->Close();

TFile *file_sig3 = TFile::Open(filename_sig3.Data());
    TH1F * hist_sig3 = (TH1F*)file_sig3->Get(histname_sig3.Data())->Clone();
    hist_sig3->SetDirectory(0);
    file_sig3->Close();



TFile *file_sig4 = TFile::Open(filename_sig4.Data());
    TH1F * hist_sig4 = (TH1F*)file_sig4->Get(histname_sig4.Data())->Clone();
    hist_sig4->SetDirectory(0);
    file_sig4->Close();


    TFile *file_bkg2 = TFile::Open(filename_bkg2.Data());
    TH1F * hist_bkg2 = (TH1F*)file_bkg2->Get(histname_bkg2.Data())->Clone();
    hist_bkg2->SetDirectory(0);
    file_bkg2->Close();

    TFile *file_bkg3 = TFile::Open(filename_bkg3.Data());
    TH1F * hist_bkg3 = (TH1F*)file_bkg3->Get(histname_bkg3.Data())->Clone();
    hist_bkg3->SetDirectory(0);
    file_bkg3->Close();

    TFile *file_bkg = TFile::Open(filename_bkg.Data());
    TH1F * hist_bkg = (TH1F*)file_bkg->Get(histname_bkg.Data())->Clone();
    hist_bkg->SetDirectory(0);
    file_bkg->Close();

    TFile *file_bkg4 = TFile::Open(filename_bkg4.Data());
    TH1F * hist_bkg4 = (TH1F*)file_bkg4->Get(histname_bkg4.Data())->Clone();
    hist_bkg4->SetDirectory(0);
    file_bkg4->Close();


    //
    // GRAPHS
    //

    // MC16a graph
    TGraph * gr_bkg2 = GetROCCurve(hist_sig2, hist_bkg2, 10000);
    gr_bkg2->Draw("AL");
    gr_bkg2->SetLineColor(2);
    gr_bkg2->SetLineWidth(3);
    gr_bkg2->SetLineStyle(1);

    gr_bkg2->GetXaxis()->SetTitle("signal efficiency");
    gr_bkg2->GetYaxis()->SetTitle("background rejection");
    gr_bkg2->GetXaxis()->SetRangeUser(0., 1.);
    gr_bkg2->GetYaxis()->SetRangeUser(0., 1.);
    //gPad->SetGridx(kTRUE);
    //gPad->SetGridy(kTRUE);

    TGraph * gr_bkg3 = GetROCCurve(hist_sig3, hist_bkg3, 10000);
    gr_bkg3->Draw("L");
    gr_bkg3->SetLineColor(8);
    gr_bkg3->SetLineWidth(3);
    gr_bkg3->SetLineStyle(1);

    TGraph * gr_bkg = GetROCCurve(hist_sig, hist_bkg, 10000);
    gr_bkg->Draw("L");
    gr_bkg->SetLineColor(1);
    gr_bkg->SetLineWidth(3);
    gr_bkg->SetLineStyle(1);

    TGraph * gr_bkg4 = GetROCCurve(hist_sig4, hist_bkg4, 10000);
    gr_bkg4->Draw("L");
    gr_bkg4->SetLineColor(9);
    gr_bkg4->SetLineWidth(3);
    gr_bkg4->SetLineStyle(1);


    // gPad->BuildLegend();
    TLegend * leg = new TLegend(0.2, 0.2, 0.45, 0.45);
    leg->SetHeader("2j vs 3j s-channel ratio");
    leg->AddEntry(gr_bkg2, "0.1", "L");
    leg->AddEntry(gr_bkg3, "0.5", "L");
    leg->AddEntry(gr_bkg, "0.65", "L");
    leg->AddEntry(gr_bkg4, "0.8", "L");
    
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetLineColor(0);
    leg->SetBorderSize(0);
    leg->Draw();

    gPad->GetCanvas()->SaveAs(savename.Data());

    // Int_t nbins_sig = hist_sig->GetNbinsX();
    // Double_t edge_sig[nbins_sig];
    // hist_sig->GetLowEdge(edge_sig);
    // std::vector<Double_t> edge_sig_vec, content_sig_vec;
    // Double_t content_sig[nbins_sig];
    // for ( Int_t i = 0; i < nbins_sig; i++ ) {
    //     edge_sig_vec.push_back(edge_sig[i]);
    //     content_sig_vec.push_back(hist_sig->GetBinContent(i+1));
    //     content_sig[i] = hist_sig->GetBinContent(i+1); // 0 is underflow
    // }
    // Int_t nbins_ttbar = hist_ttbar->GetNbinsX();
    // Double_t edge_ttbar[nbins_ttbar];
    // hist_ttbar->GetLowEdge(edge_ttbar);
    // std::vector<Double_t> edge_ttbar_vec, content_ttbar_vec;
    // Double_t content_ttbar[nbins_ttbar];
    // for ( Int_t i = 0; i < nbins_ttbar; i++ ) {
    //     edge_ttbar_vec.push_back(edge_ttbar[i]);
    //     content_ttbar_vec.push_back(hist_ttbar->GetBinContent(i+1));
    //     content_ttbar[i] = hist_ttbar->GetBinContent(i+1); // 0 is underflow
    // }    

    // std::vector<Double_t> mvaSignal(edge_sig_vec);
    // std::vector<Double_t> mvaBackground(edge_bkg_vec);
    // std::vector<Double_t> mvaSignalWeights(content_sig_vec);
    // std::vector<Double_t> mvaBackgroundWeights(content_bkg_vec);
    // TMVA::ROCCurve::ROCCurve roc(mvaSignal, mvaBackground, mvaSignalWeights, mvaBackgroundWeights);
    // std::cout << "ROC Integral: " << roc.GetROCIntegral() << std::endl;
}
//     TString filename_sig("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
//     TString filename_ttbar("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/ttbar.root");
//     TString filename_wjets("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/wjets.root");
// //    TString filename("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/MCPlotter.root");

    // TString filename_sig("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/sgtop_sChannel.root");
    // TString filename_ttbar("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/ttbar.root");
    // TString filename_wjets("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_wjetsMerged/adapted_for_trexfitter_allhist/wjets.root");
