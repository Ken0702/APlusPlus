//____________________________________________________________________
//
// Class for the documentation of Top Pairs
// 
//    Author: Felix Thomas <mailto:Felix.Thomas@mailbox.tu-dresden.de> 
//    Update: $Id$
//    Copyright: 2011 (C) Felix Thomas  
//

#ifndef ATLAS_AtlTopPairDocumenter
#include <AtlTopPairDocumenter.h>
#endif

#include <iostream>

using namespace std;


#ifndef __CINT__
ClassImp(AtlTopPairDocumenter);
#endif

const char* AtlTopPairDocumenter::fgHistOriginNames[] = {
    "orig",
    "fitted"};

const char* AtlTopPairDocumenter::fgOriginNames[] = {
    "original",
    "fitted"};

const char* AtlTopPairDocumenter::fgSideNames[] = {
    "inclusive",
    "leptonic",
    "hadronic"};
    
const char* AtlTopPairDocumenter::fgHistSideNames[] = {
    "incl",
    "lep",
    "had"};

//____________________________________________________________________

AtlTopPairDocumenter::AtlTopPairDocumenter(const char* name, const char* title, TFile* Output, const char* path = "" ) : 
AtlDocumenter(name,title,Output, path){

    //
    // Default constructor
    //

    MakeDirectoryStructure();

}

//____________________________________________________________________

AtlTopPairDocumenter::~AtlTopPairDocumenter() {
    //
    // Default destructor
    //
}

//___________________________________________________________

void AtlTopPairDocumenter::BookHistograms(){
  //
  // Book histograms (after kinematic fit)
  //

  TDirectory *savdir = gDirectory;

  // k cycles through the sides inclusive, leptonic, hadronic
  for (Int_t k = 0; k < fgNSides; k++){

    gDirectory->mkdir(fgSideNames[k]);
    gDirectory->cd(fgSideNames[k]);
    fHistChi2[k] = new TH1F(Form("h_chi2_%s_%s", fgHistSideNames[k], this->GetTitle()),
                                                      Form("#chi^{2} Distribution of Reco (%s, %s)", fgSideNames[k], this->GetTitle()),
                                                      40, 0, 20);
    fHistChi2[k]->SetXTitle("#chi^{2}");
    fHistChi2[k]->SetYTitle("#(Best Candidates)");
    
    fHistChi2Prob[k] = new TH1F(Form("h_chi2prob_%s_%s", fgHistSideNames[k], this->GetTitle()),
                                                              Form("P(#chi^{2}) Distribution of Reco (%s, %s)", fgSideNames[k], this->GetTitle()),
                                                              40, 0, 1);
    fHistChi2Prob[k]->SetXTitle("P(#chi^{2})");
    fHistChi2Prob[k]->SetYTitle("#(Best Candidates)/0.1");
    
    gDirectory->mkdir("WReco", "W Reconstruction");
    gDirectory->cd("WReco");
  
    for (Int_t l = 0; l < fgNOrigins; ++l){
      fHistWreco_M[k][l] = new TH1F(Form("h_Wreco_M_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                 Form("W Invariant Mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                 40, 30, 130);
      fHistWreco_M[k][l]->SetXTitle("M_{W} (GeV)");
      fHistWreco_M[k][l]->SetYTitle(Form("#W bosons (%s)/2.5",fgHistSideNames[k]));
      
      fHistWreco_phi[k][l] = new TH1F(Form("h_Wreco_phi_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                       Form("W Azimuth Angle (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                       16, -3.2, 3.2);
      fHistWreco_phi[k][l]->SetXTitle("#phi (rad)");
      fHistWreco_phi[k][l]->SetYTitle(Form("#W bosons (%s)/0.4",fgHistSideNames[k]));
      
      fHistWreco_pt[k][l] = new TH1F(Form("h_Wreco_pt_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                      Form("W Transverse Momentum (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                      20, 0, 200);
      fHistWreco_pt[k][l]->SetXTitle("P_{t} (GeV)");
      fHistWreco_pt[k][l]->SetYTitle(Form("#W bosons (%s)/10",fgHistSideNames[k]));
      
      
      fHistWreco_eta[k][l] = new TH1F(Form("h_Wreco_eta_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                       Form("W Pseudo-Rapidity (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                       10, -2.5, 2.5);
      fHistWreco_eta[k][l]->SetXTitle("#eta");
      fHistWreco_eta[k][l]->SetYTitle(Form("#W bosons (%s)/0.5",fgHistSideNames[k]));
    }
 
    gDirectory->cd("..");
    gDirectory->mkdir("TopReco", "Top Reconstruction");
    gDirectory->cd("TopReco");
  
    for (Int_t l = 0; l < fgNOrigins; ++l){
      fHistTopReco_M[k][l] = new TH1F(Form("h_topreco_M_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                       Form("Top Invariant Mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                       15, 100, 250);
      fHistTopReco_M[k][l]->SetXTitle("M_{top} (GeV)");
      fHistTopReco_M[k][l]->SetYTitle(Form("#top quarks (%s)/10",fgHistSideNames[k]));
      
      fHistTopReco_phi[k][l] = new TH1F(Form("h_topreco_phi_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                         Form("Top Azimuth Angle (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                         16, -3.2, 3.2);
      fHistTopReco_phi[k][l]->SetXTitle("#phi (rad)");
      fHistTopReco_phi[k][l]->SetYTitle(Form("#top quarks (%s)/0.4",fgHistSideNames[k]));
      
      fHistTopReco_pt[k][l] = new TH1F(Form("h_topreco_pt_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                        Form("Top Transverse Momentum (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                        20, 0, 200);
      fHistTopReco_pt[k][l]->SetXTitle("P_{t} (GeV)");
      fHistTopReco_pt[k][l]->SetYTitle(Form("#top quarks (%s)/10",fgHistSideNames[k]));
      
      fHistTopReco_eta[k][l] = new TH1F(Form("h_topreco_eta_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                         Form("Top Pseudo-Rapidity (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                         10, -2.5, 2.5);
      fHistTopReco_eta[k][l]->SetXTitle("#eta");
      fHistTopReco_eta[k][l]->SetYTitle(Form("#top quarks (%s)/0.5",fgHistSideNames[k]));
      
      fHistTopMvsChi2[k][l] = new TH2F(Form("h_top_M_vs_chi2_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                        Form("#chi^{2} vs. top mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                        15, 100, 250, 40, 0, 20);
      fHistTopMvsChi2[k][l]->SetXTitle("M_{top} (GeV)");
      fHistTopMvsChi2[k][l]->SetYTitle("#chi^{2}");
      fHistTopMvsChi2[k][l]->SetZTitle(Form("#top quarks (%s)",fgHistSideNames[k]));
      
      
      fHistTopMvsChi2prob[k][l] = new TH2F(Form("h_top_M_vs_chi2prob_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),  
                                                                Form("P(#chi^{2}) vs. top mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                                15, 100, 250, 10, 0, 1);
      fHistTopMvsChi2prob[k][l]->SetXTitle("M_{top} (GeV)");
      fHistTopMvsChi2prob[k][l]->SetYTitle("P(#chi^{2})");
      fHistTopMvsChi2prob[k][l]->SetZTitle(Form("#top quarks (%s)/10/0.1",fgHistSideNames[k]));
      
      fHistTopPtvsChi2[k][l] = new TH2F(Form("h_top_Pt_vs_chi2_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                         Form("#chi^{2} vs. top mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                         20, 0, 200, 20, 0, 20);
      fHistTopPtvsChi2[k][l]->SetXTitle("P_{t,top} (GeV)");
      fHistTopPtvsChi2[k][l]->SetYTitle("#chi^{2}");
      fHistTopPtvsChi2[k][l]->SetZTitle(Form("#top quarks (%s)",fgHistSideNames[k]));
      
      fHistTopPtvsChi2prob[k][l] = new TH2F(Form("h_top_Pt_vs_chi2prob_%s_%s_%s", fgHistSideNames[k], this->GetTitle(), fgHistOriginNames[l]),
                                                                 Form("#chi^{2} vs. top mass (%s, %s, %s)", fgSideNames[k], this->GetTitle(), fgOriginNames[l]),
                                                                 20, 0, 200, 10, 0, 1);
      fHistTopPtvsChi2prob[k][l]->SetXTitle("P_{t,top} (GeV)");
      fHistTopPtvsChi2prob[k][l]->SetYTitle("P(#chi^{2})");
      fHistTopPtvsChi2prob[k][l]->SetZTitle(Form("#top quarks (%s)/10/0.1",fgHistSideNames[k]));
    }
  
     gDirectory->cd("../..");
  }

  BookLepHistograms();
  BookHadHistograms();
  BookResidualHistograms();
  BookPullHistograms();

  savdir->cd();

}

//___________________________________________________________

void AtlTopPairDocumenter::BookLepHistograms(){
  //
  //  Book histograms that record the data for the leptonic side of the top decay (after kinematic fit)
  //
  //

  gDirectory->cd("leptonic");

  gDirectory->mkdir("Neutrino-reco", "Neutrino reconstruction");
  gDirectory->cd("Neutrino-reco");

  fHistLepNuReco_phi = new TH1F(Form("h_nureco_phi_lep_%s", this->GetTitle()),
                                   Form("#nu Azimuth Angle (%s)", this->GetTitle()),
                                   16, -3.2, 3.2);
  fHistLepNuReco_phi->SetXTitle("#phi (rad)");
  fHistLepNuReco_phi->SetYTitle("#Neutrinos/0.4");
  
  fHistLepNuReco_pt = new TH1F(Form("h_nureco_pt_lep_%s", this->GetTitle()),
                                  Form("#nu Transverse Momentum (%s)", this->GetTitle()),
                                  20, 0, 200);
  fHistLepNuReco_pt->SetXTitle("P_{t} (GeV)");
  fHistLepNuReco_pt->SetYTitle("#Neutrinos/10");
  
  fHistLepNuReco_eta = new TH1F(Form("h_nureco_eta_lep_%s", this->GetTitle()),
                                   Form("#nu Pseudo-Rapidity (%s)", this->GetTitle()),
                                   10, -2.5, 2.5);
  fHistLepNuReco_eta->SetXTitle("#eta");
  fHistLepNuReco_eta->SetYTitle("#Neutrinos/0.5");
  
  gDirectory->cd("../..");
}

//___________________________________________________________
void AtlTopPairDocumenter::BookHadHistograms() {
  //
  //  Book histograms that record the data unique to 
  //  the hadronic side of the top decay (after kinematic fit)
  //

    return;
}

//___________________________________________________________

void AtlTopPairDocumenter::BookResidualHistograms() {
   //
   // Residual distributions used for checking if the kinfitter works as intended
   //

  gDirectory->mkdir("residuals","Residual histograms");
  gDirectory->cd("residuals");

  // Lepton  
  gDirectory->mkdir("lepton_residual","Residual histograms for leptons");
  gDirectory->cd("lepton_residual");

  // Lepton pt
  fHistResidualLepton_pt = new TH1F(Form("h_residual_Lepton_pt_%s", this->GetTitle()),
                                "Residual of Fit on Lepton p_{T}",
                                16, -0.04, 0.04);
  fHistResidualLepton_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
  fHistResidualLepton_pt->SetYTitle("#(Best Candidates)/0.005");

  // Lepton eta
  fHistResidualLepton_eta = new TH1F(Form("h_residual_Lepton_eta_%s", this->GetTitle()),
                                 "Residual of Fit on Lepton #eta",
                                 16, -0.04, 0.04);
  fHistResidualLepton_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
  fHistResidualLepton_eta->SetYTitle("#(Best Candidates)/0.005");

  // Lepton phi
  fHistResidualLepton_phi = new TH1F(Form("h_residual_Lepton_phi_%s", this->GetTitle()),
                                 "Residual of Fit on Lepton #varphi",
                                 16, -0.04, 0.04);
  fHistResidualLepton_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualLepton_phi->SetYTitle("#(Best Candidates)/0.005");

  gDirectory->cd("..");



  // B-Jets  
  gDirectory->mkdir("bjet_residual","Residual histograms for B-Jets");
  gDirectory->cd("bjet_residual");

  // B-Jet Et
  fHistResidualLepBJet_Et = new TH1F(Form("h_residual_LepBJet_Et_%s", this->GetTitle()),
                                 "Residual of Fit on B-Jet E_{T}",
                                 16, -0.2, 0.2);
  fHistResidualLepBJet_Et->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
  fHistResidualLepBJet_Et->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet pt
  fHistResidualLepBJet_pt = new TH1F(Form("h_residual_LepBJet_pt_%s", this->GetTitle()),
                                 "Residual of Fit on B-Jet p_{T}",
                                 16, -0.2, 0.2);
  fHistResidualLepBJet_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
  fHistResidualLepBJet_pt->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet eta
  fHistResidualLepBJet_eta = new TH1F(Form("h_residual_LepBJet_eta_%s", this->GetTitle()),
                                  "Residual of Fit on B-Jet #eta",
                                  16, -0.2, 0.2);
  fHistResidualLepBJet_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
  fHistResidualLepBJet_eta->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet phi
  fHistResidualLepBJet_phi = new TH1F(Form("h_residual_LepBJet_phi_%s", this->GetTitle()),
                                  "Residual of Fit on B-Jet #varphi",
                                  16, -0.2, 0.2);
  fHistResidualLepBJet_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualLepBJet_phi->SetYTitle("#(Best Candidates)/0.025");



  // B-Jet Et
  fHistResidualHadBJet_Et = new TH1F(Form("h_residual_HadBJet_Et_%s", this->GetTitle()),
                                 "Residual of Fit on B-Jet E_{T}",
                                 16, -0.2, 0.2);
  fHistResidualHadBJet_Et->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
  fHistResidualHadBJet_Et->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet pt
  fHistResidualHadBJet_pt = new TH1F(Form("h_residual_HadBJet_pt_%s", this->GetTitle()),
                                 "Residual of Fit on B-Jet p_{T}",
                                 16, -0.2, 0.2);
  fHistResidualHadBJet_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
  fHistResidualHadBJet_pt->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet eta
  fHistResidualHadBJet_eta = new TH1F(Form("h_residual_HadBJet_eta_%s", this->GetTitle()),
                                  "Residual of Fit on B-Jet #eta",
                                  16, -0.2, 0.2);
  fHistResidualHadBJet_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
  fHistResidualHadBJet_eta->SetYTitle("#(Best Candidates)/0.025");

  // B-Jet phi
  fHistResidualHadBJet_phi = new TH1F(Form("h_residual_HadBJet_phi_%s", this->GetTitle()),
                                  "Residual of Fit on B-Jet #varphi",
                                  16, -0.2, 0.2);
  fHistResidualHadBJet_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualHadBJet_phi->SetYTitle("#(Best Candidates)/0.025");


  gDirectory->cd("..");

  // Jets  
  gDirectory->mkdir("jet_residual","Residual histograms for Jets");
  gDirectory->cd("jet_residual");

  // B-Jet Et
  fHistResidualHadJet1_Et = new TH1F(Form("h_residual_HadJet1_Et_%s", this->GetTitle()),
                                 "Residual of Fit on Jet1 E_{T}",
                                 16, -0.4, 0.4);
  fHistResidualHadJet1_Et->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
  fHistResidualHadJet1_Et->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet pt
  fHistResidualHadJet1_pt = new TH1F(Form("h_residual_HadJet1_pt_%s", this->GetTitle()),
                                 "Residual of Fit on Jet1 p_{T}",
                                 16, -0.4, 0.4);
  fHistResidualHadJet1_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
  fHistResidualHadJet1_pt->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet eta
  fHistResidualHadJet1_eta = new TH1F(Form("h_residual_HadJet1_eta_%s", this->GetTitle()),
                                  "Residual of Fit on Jet1 #eta",
                                  12, -0.3, 0.3);
  fHistResidualHadJet1_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
  fHistResidualHadJet1_eta->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet phi
  fHistResidualHadJet1_phi = new TH1F(Form("h_residual_HadJet1_phi_%s", this->GetTitle()),
                                  "Residual of Fit on Jet1 #varphi",
                                  12, -0.3, 0.3);
  fHistResidualHadJet1_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualHadJet1_phi->SetYTitle("#(Best Candidates)/0.05");



  // B-Jet Et
  fHistResidualHadJet2_Et = new TH1F(Form("h_residual_HadJet2_Et_%s", this->GetTitle()),
                                 "Residual of Fit on Jet2 E_{T}",
                                 16, -0.4, 0.4);
  fHistResidualHadJet2_Et->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
  fHistResidualHadJet2_Et->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet pt
  fHistResidualHadJet2_pt = new TH1F(Form("h_residual_HadJet2_pt_%s", this->GetTitle()),
                                 "Residual of Fit on Jet2 p_{T}",
                                 16, -0.4, 0.4);
  fHistResidualHadJet2_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
  fHistResidualHadJet2_pt->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet eta
  fHistResidualHadJet2_eta = new TH1F(Form("h_residual_HadJet2_eta_%s", this->GetTitle()),
                                  "Residual of Fit on Jet2 #eta",
                                  12, -0.3, 0.3);
  fHistResidualHadJet2_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
  fHistResidualHadJet2_eta->SetYTitle("#(Best Candidates)/0.05");

  // B-Jet phi
  fHistResidualHadJet2_phi = new TH1F(Form("h_residual_HadJet2_phi_%s", this->GetTitle()),
                                  "Residual of Fit on Jet2 #varphi",
                                  12, -0.3, 0.3);
  fHistResidualHadJet2_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualHadJet2_phi->SetYTitle("#(Best Candidates)/0.05");


  gDirectory->cd("..");


  // MET  
  gDirectory->mkdir("etmiss_Residual","Residual histograms for MET");
  gDirectory->cd("etmiss_Residual");

  // MET Magnitude
  fHistResidualEtmiss_Mag = new TH1F(Form("h_residual_Etmiss_Mag_%s", this->GetTitle()),
                                 "Residual of Fit on Magnitude of E_{T}^{miss}",
                                 15, -0.75, 0.75);
  fHistResidualEtmiss_Mag->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
  fHistResidualEtmiss_Mag->SetYTitle("#(Best Candidates)/0.1");

  // MET phi
  fHistResidualEtmiss_phi = new TH1F(Form("h_residual_Etmiss_phi_%s", this->GetTitle()),
                                 "Residual of Fit on #varphi of E_{T}^{miss}",
                                 15, -0.75, 0.75);
  fHistResidualEtmiss_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
  fHistResidualEtmiss_phi->SetYTitle("#(Best Candidates)/0.1");

  gDirectory->cd("../..");
}

//___________________________________________________________

void AtlTopPairDocumenter::BookPullHistograms() {
   //
   // Pull distributions used for checking if the kinfitter works as intended
   //

  gDirectory->mkdir("Pulls","Pull histograms");
  gDirectory->cd("Pulls");

  // Lepton  
  gDirectory->mkdir("lepton_pull","Pull histograms for leptons");
  gDirectory->cd("lepton_pull");

  // Lepton pt
  fHistPullLepton_pt = new TH1F(Form("h_PullLepton_pt_%s", this->GetTitle()),
                                "Pull of Fit on Lepton p_{T}",
                                50, -5, 5);
  fHistPullLepton_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{#sigma_{#Delta p_{T}}}");
  fHistPullLepton_pt->SetYTitle("#(Best Candidates)/0.2");

  // Lepton eta
  fHistPullLepton_eta = new TH1F(Form("h_PullLepton_eta_%s", this->GetTitle()),
                                 "Pull of Fit on Lepton #eta",
                                 50, -5, 5);
  fHistPullLepton_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#sigma_{#Delta #eta}}");
  fHistPullLepton_eta->SetYTitle("#(Best Candidates)/0.2");

  // Lepton phi
  fHistPullLepton_phi = new TH1F(Form("h_PullLepton_phi_%s", this->GetTitle()),
                                 "Pull of Fit on Lepton #varphi",
                                 50, -5, 5);
  fHistPullLepton_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}}");
  fHistPullLepton_phi->SetYTitle("#(Best Candidates)/0.2");

  gDirectory->cd("..");



  // B-Jets  
  gDirectory->mkdir("bjet_pull","Pull histograms for B-Jets");
  gDirectory->cd("bjet_pull");

  // B-Jet pt
  fHistPullLepBJet_pt = new TH1F(Form("h_PullLepBJet_pt_%s", this->GetTitle()),
                                 "Pull of Fit on B-Jet p_{T}",
                                 50, -5, 5);
  fHistPullLepBJet_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{#sigma_{#Delta p_{T}}}");
  fHistPullLepBJet_pt->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet eta
  fHistPullLepBJet_eta = new TH1F(Form("h_PullLepBJet_eta_%s", this->GetTitle()),
                                  "Pull of Fit on B-Jet #eta",
                                  50, -5, 5);
  fHistPullLepBJet_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#sigma_{#Delta #eta}}");
  fHistPullLepBJet_eta->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet phi
  fHistPullLepBJet_phi = new TH1F(Form("h_PullLepBJet_phi_%s", this->GetTitle()),
                                  "Pull of Fit on B-Jet #varphi",
                                  50, -5, 5);
  fHistPullLepBJet_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}}");
  fHistPullLepBJet_phi->SetYTitle("#(Best Candidates)/0.2");


  // B-Jet pt
  fHistPullHadBJet_pt = new TH1F(Form("h_PullHadBJet_pt_%s", this->GetTitle()),
                                 "Pull of Fit on B-Jet p_{T}",
                                 50, -5, 5);
  fHistPullHadBJet_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{#sigma_{#Delta p_{T}}}");
  fHistPullHadBJet_pt->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet eta
  fHistPullHadBJet_eta = new TH1F(Form("h_PullHadBJet_eta_%s", this->GetTitle()),
                                  "Pull of Fit on B-Jet #eta",
                                  50, -5, 5);
  fHistPullHadBJet_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#sigma_{#Delta #eta}}");
  fHistPullHadBJet_eta->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet phi
  fHistPullHadBJet_phi = new TH1F(Form("h_PullHadBJet_phi_%s", this->GetTitle()),
                                  "Pull of Fit on B-Jet #varphi",
                                  50, -5, 5);
  fHistPullHadBJet_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}}");
  fHistPullHadBJet_phi->SetYTitle("#(Best Candidates)/0.2");


  gDirectory->cd("..");

  // Jets  
  gDirectory->mkdir("jet_pull","Pull histograms for Jets");
  gDirectory->cd("jet_pull");

  // B-Jet pt
  fHistPullHadJet1_pt = new TH1F(Form("h_PullHadJet1_pt_%s", this->GetTitle()),
                                 "Pull of Fit on Jet1 p_{T}",
                                 50, -5, 5);
  fHistPullHadJet1_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{#sigma_{#Delta p_{T}}}");
  fHistPullHadJet1_pt->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet eta
  fHistPullHadJet1_eta = new TH1F(Form("h_PullHadJet1_eta_%s", this->GetTitle()),
                                  "Pull of Fit on Jet1 #eta",
                                  50, -5, 5);
  fHistPullHadJet1_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#sigma_{#Delta #eta}}");
  fHistPullHadJet1_eta->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet phi
  fHistPullHadJet1_phi = new TH1F(Form("h_PullHadJet1_phi_%s", this->GetTitle()),
                                  "Pull of Fit on Jet1 #varphi",
                                  50, -5, 5);
  fHistPullHadJet1_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}}");
  fHistPullHadJet1_phi->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet pt
  fHistPullHadJet2_pt = new TH1F(Form("h_PullHadJet2_pt_%s", this->GetTitle()),
                                 "Pull of Fit on Jet2 p_{T}",
                                 50, -5, 5);
  fHistPullHadJet2_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{#sigma_{#Delta p_{T}}}");
  fHistPullHadJet2_pt->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet eta
  fHistPullHadJet2_eta = new TH1F(Form("h_PullHadJet2_eta_%s", this->GetTitle()),
                                  "Pull of Fit on Jet2 #eta",
                                  50, -5, 5);
  fHistPullHadJet2_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#sigma_{#Delta #eta}}");
  fHistPullHadJet2_eta->SetYTitle("#(Best Candidates)/0.2");

  // B-Jet phi
  fHistPullHadJet2_phi = new TH1F(Form("h_PullHadJet2_phi_%s", this->GetTitle()),
                                  "Pull of Fit on Jet2 #varphi",
                                  50, -5, 5);
  fHistPullHadJet2_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}}");
  fHistPullHadJet2_phi->SetYTitle("#(Best Candidates)/0.2");


  gDirectory->cd("..");


  // MET  
  gDirectory->mkdir("etmiss_pull","Pull histograms for MET");
  gDirectory->cd("etmiss_pull");

  // MET Magnitude
  fHistPullEtmiss_Mag = new TH1F(Form("h_PullEtmiss_Mag_%s", this->GetTitle()),
                                 "Pull of Fit on Magnitude of E_{T}^{miss}",
                                 50, -5, 5);
  fHistPullEtmiss_Mag->SetXTitle("#frac{E_{T}^{miss,final}-E_{T}^{miss,initial}}{#sigma_{#Delta E_{T}^{miss}}}");
  fHistPullEtmiss_Mag->SetYTitle("#(Best Candidates)/0.2");

  // MET phi
  fHistPullEtmiss_phi = new TH1F(Form("h_PullEtmiss_phi_%s", this->GetTitle()),
                                 "Pull of Fit on #varphi of E_{T}^{miss}",
                                 50, -5, 5);
  fHistPullEtmiss_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#sigma_{#Delta #varphi}");
  fHistPullEtmiss_phi->SetYTitle("#(Best Candidates)/0.2");

  gDirectory->cd("../..");
}


//____________________________________________________________________

void AtlTopPairDocumenter::Document(AtlTopPair *toppair, TVector2 *etmiss, Double_t weight) {

  //
  // Invocation method for the documentation of the given particles  
  //
  
  fTopPair = toppair;
  fEvtWeight = weight;
    
  fEtMissMag = etmiss->Mod();
  fEtMissPhi = TVector2::Phi_mpi_pi((Double_t)etmiss->Phi());
    
  FillHistograms();

}

//____________________________________________________________________

void AtlTopPairDocumenter::FillHistograms() {

  //
  // Method for filling histograms, only events that survive 
  // AnalyzeEvent() are taken into account
  //

  // Check if 2 top decays labeled as ttbar are available
  if ( fTopPair == 0 ) return;





  // Get lepton from reconstruction
  HepParticle* Lepton = 0;
  Lepton = ((AtlWDecayLNu*)((HepTopDecay*)fTopPair->GetLeptonicTop())->GetWDecay())->GetChargedLeptonOrig();



  // Invoking procedures for filling histograms

  FillLepHistograms();
  FillHadHistograms();
  FillResidualHistograms();
  FillPullHistograms();

}

//___________________________________________________________

void AtlTopPairDocumenter::FillLepHistograms() {
  //
  // Fill Leptonic histograms (after kinematic fit)
  //

  // Get appropriate W-bosons and top quarks
  HepTopDecay*  t = fTopPair->GetLeptonicTop();
  AtlWDecayLNu* W = (AtlWDecayLNu*)t->GetWDecay();

  HepJet* BJet        = t->GetBJetOrig();
  HepParticle* Lepton = W->GetChargedLeptonOrig();
  HepParticle* nu     = W->GetNeutrino();


  // 4-vectors derived from original data
  TLorentzVector P_W = Lepton->P() + nu->P();
  TLorentzVector P_Top = BJet->P() + P_W;

  // Fill inclusive diagrams
  fHistChi2[kInclusive]        ->Fill(W->GetChi2(), fEvtWeight);
  fHistChi2Prob[kInclusive]    ->Fill(W->GetProb(), fEvtWeight);


  // Filling inclusive top-reco diagrams

  fHistTopReco_M[kInclusive][kFitted]   ->Fill(t->M("REC"), fEvtWeight);
  fHistTopReco_phi[kInclusive][kFitted] ->Fill(t->Phi(), fEvtWeight);
  fHistTopReco_pt[kInclusive][kFitted]  ->Fill(t->Pt(), fEvtWeight);
  fHistTopReco_eta[kInclusive][kFitted] ->Fill(t->Eta(), fEvtWeight);

  fHistTopReco_M[kInclusive][kOriginal]   ->Fill(P_Top.M(), fEvtWeight);
  fHistTopReco_phi[kInclusive][kOriginal] ->Fill(P_Top.Phi(), fEvtWeight);
  fHistTopReco_pt[kInclusive][kOriginal]  ->Fill(P_Top.Pt(), fEvtWeight);
  fHistTopReco_eta[kInclusive][kOriginal] ->Fill(P_Top.Eta(), fEvtWeight);


  fHistTopMvsChi2[kInclusive][kFitted]      ->Fill(t->M("REC"), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kInclusive][kFitted]  ->Fill(t->M("REC"), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kInclusive][kFitted]     ->Fill(t->Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kInclusive][kFitted] ->Fill(t->Pt(), W->GetProb(), fEvtWeight);

  fHistTopMvsChi2[kInclusive][kOriginal]      ->Fill(P_Top.M(), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kInclusive][kOriginal]  ->Fill(P_Top.M(), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kInclusive][kOriginal]     ->Fill(P_Top.Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kInclusive][kOriginal] ->Fill(P_Top.Pt(), W->GetProb(), fEvtWeight);

  // Filling inclusive W-Reco diagrams
  fHistWreco_M[kInclusive][kFitted]   ->Fill(W->M("REC"), fEvtWeight);
  fHistWreco_phi[kInclusive][kFitted] ->Fill(W->Phi(), fEvtWeight);
  fHistWreco_pt[kInclusive][kFitted]  ->Fill(W->Pt(), fEvtWeight);
  fHistWreco_eta[kInclusive][kFitted] ->Fill(W->Eta(), fEvtWeight);

  fHistWreco_M[kInclusive][kOriginal]   ->Fill(P_W.M(), fEvtWeight);
  fHistWreco_phi[kInclusive][kOriginal] ->Fill(P_W.Phi(), fEvtWeight);
  fHistWreco_pt[kInclusive][kOriginal]  ->Fill(P_W.Pt(), fEvtWeight);
  fHistWreco_eta[kInclusive][kOriginal] ->Fill(P_W.Eta(), fEvtWeight);


  fHistChi2[kLeptonic]        ->Fill(W->GetChi2(), fEvtWeight);
  fHistChi2Prob[kLeptonic]    ->Fill(W->GetProb(), fEvtWeight);


  // Filling leptonic top-reco diagrams

  fHistTopReco_M[kLeptonic][kFitted]   ->Fill(t->M("REC"), fEvtWeight);
  fHistTopReco_phi[kLeptonic][kFitted] ->Fill(t->Phi(), fEvtWeight);
  fHistTopReco_pt[kLeptonic][kFitted]  ->Fill(t->Pt(), fEvtWeight);
  fHistTopReco_eta[kLeptonic][kFitted] ->Fill(t->Eta(), fEvtWeight);

  fHistTopReco_M[kLeptonic][kOriginal]   ->Fill(P_Top.M(), fEvtWeight);
  fHistTopReco_phi[kLeptonic][kOriginal] ->Fill(P_Top.Phi(), fEvtWeight);
  fHistTopReco_pt[kLeptonic][kOriginal]  ->Fill(P_Top.Pt(), fEvtWeight);
  fHistTopReco_eta[kLeptonic][kOriginal] ->Fill(P_Top.Eta(), fEvtWeight);


  fHistTopMvsChi2[kLeptonic][kFitted]      ->Fill(t->M("REC"), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kLeptonic][kFitted]  ->Fill(t->M("REC"), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kLeptonic][kFitted]     ->Fill(t->Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kLeptonic][kFitted] ->Fill(t->Pt(), W->GetProb(), fEvtWeight);

  fHistTopMvsChi2[kLeptonic][kOriginal]      ->Fill(P_Top.M(), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kLeptonic][kOriginal]  ->Fill(P_Top.M(), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kLeptonic][kOriginal]     ->Fill(P_Top.Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kLeptonic][kOriginal] ->Fill(P_Top.Pt(), W->GetProb(), fEvtWeight);


  // Filling leptonic W-Reco diagrams

  fHistWreco_M[kLeptonic][kFitted]   ->Fill(W->M("REC"), fEvtWeight);
  fHistWreco_phi[kLeptonic][kFitted] ->Fill(W->Phi(), fEvtWeight);
  fHistWreco_pt[kLeptonic][kFitted]  ->Fill(W->Pt(), fEvtWeight);
  fHistWreco_eta[kLeptonic][kFitted] ->Fill(W->Eta(), fEvtWeight);

  fHistWreco_M[kLeptonic][kOriginal]   ->Fill(P_W.M(), fEvtWeight);
  fHistWreco_phi[kLeptonic][kOriginal] ->Fill(P_W.Phi(), fEvtWeight);
  fHistWreco_pt[kLeptonic][kOriginal]  ->Fill(P_W.Pt(), fEvtWeight);
  fHistWreco_eta[kLeptonic][kOriginal] ->Fill(P_W.Eta(), fEvtWeight);

  // Lepton histograms
  fHistLepNuReco_phi  ->Fill(nu->Phi(), fEvtWeight);
  fHistLepNuReco_pt   ->Fill(nu->Pt(), fEvtWeight);
  fHistLepNuReco_eta  ->Fill(nu->Eta(), fEvtWeight);

}

//___________________________________________________________

void AtlTopPairDocumenter::FillHadHistograms() {
  //
  // Fill Hadronic histograms (after kinematic fit)
  //

  // Get appropriate W-bosons and top quarks
  HepTopDecay* t = fTopPair->GetHadronicTop();
  AtlWDecayJJ* W = (AtlWDecayJJ*)t->GetWDecay();

  HepJet* BJet  = t->GetBJetOrig();
  AtlJet* Jet1  = W->GetJet1_Orig();
  AtlJet* Jet2  = W->GetJet2_Orig();

  // 4-vectors derived from original data
  TLorentzVector P_W = Jet1->P() + Jet2->P();
  TLorentzVector P_Top = BJet->P() + P_W;


  // Fill inclusive diagrams
  fHistChi2[kInclusive]        ->Fill(W->GetChi2(), fEvtWeight);
  fHistChi2Prob[kInclusive]    ->Fill(W->GetProb(), fEvtWeight);

  // Filling inclusive top-reco diagrams
  fHistTopReco_M[kInclusive][kFitted]   ->Fill(t->M("REC"), fEvtWeight);
  fHistTopReco_phi[kInclusive][kFitted] ->Fill(t->Phi(), fEvtWeight);
  fHistTopReco_pt[kInclusive][kFitted]  ->Fill(t->Pt(), fEvtWeight);
  fHistTopReco_eta[kInclusive][kFitted] ->Fill(t->Eta(), fEvtWeight);

  fHistTopReco_M[kInclusive][kOriginal]   ->Fill(P_Top.M(), fEvtWeight);
  fHistTopReco_phi[kInclusive][kOriginal] ->Fill(P_Top.Phi(), fEvtWeight);
  fHistTopReco_pt[kInclusive][kOriginal]  ->Fill(P_Top.Pt(), fEvtWeight);
  fHistTopReco_eta[kInclusive][kOriginal] ->Fill(P_Top.Eta(), fEvtWeight);

  fHistTopMvsChi2[kInclusive][kFitted]      ->Fill(t->M("REC"), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kInclusive][kFitted]  ->Fill(t->M("REC"), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kInclusive][kFitted]     ->Fill(t->Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kInclusive][kFitted] ->Fill(t->Pt(), W->GetProb(), fEvtWeight);

  fHistTopMvsChi2[kInclusive][kOriginal]      ->Fill(P_Top.M(), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kInclusive][kOriginal]  ->Fill(P_Top.M(), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kInclusive][kOriginal]     ->Fill(P_Top.Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kInclusive][kOriginal] ->Fill(P_Top.Pt(), W->GetProb(), fEvtWeight);

  // Filling inclusive W-Reco diagrams
  fHistWreco_M[kInclusive][kFitted]   ->Fill(W->M("REC"), fEvtWeight);
  fHistWreco_phi[kInclusive][kFitted] ->Fill(W->Phi(), fEvtWeight);
  fHistWreco_pt[kInclusive][kFitted]  ->Fill(W->Pt(), fEvtWeight);
  fHistWreco_eta[kInclusive][kFitted] ->Fill(W->Eta(), fEvtWeight);

  fHistWreco_M[kInclusive][kOriginal]   ->Fill(P_W.M(), fEvtWeight);
  fHistWreco_phi[kInclusive][kOriginal] ->Fill(P_W.Phi(), fEvtWeight);
  fHistWreco_pt[kInclusive][kOriginal]  ->Fill(P_W.Pt(), fEvtWeight);
  fHistWreco_eta[kInclusive][kOriginal] ->Fill(P_W.Eta(), fEvtWeight);


  // Filling hadronic diagrams
  fHistChi2[kHadronic]     ->Fill(W->GetChi2(), fEvtWeight);
  fHistChi2Prob[kHadronic] ->Fill(W->GetProb(), fEvtWeight);

  // Filling hadronic top-reco diagrams
  fHistTopReco_M[kHadronic][kFitted]   ->Fill(t->M("REC"), fEvtWeight);
  fHistTopReco_phi[kHadronic][kFitted] ->Fill(t->Phi(), fEvtWeight);
  fHistTopReco_pt[kHadronic][kFitted]  ->Fill(t->Pt(), fEvtWeight);
  fHistTopReco_eta[kHadronic][kFitted] ->Fill(t->Eta(), fEvtWeight);

  fHistTopReco_M[kHadronic][kOriginal]   ->Fill(P_Top.M(), fEvtWeight);
  fHistTopReco_phi[kHadronic][kOriginal] ->Fill(P_Top.Phi(), fEvtWeight);
  fHistTopReco_pt[kHadronic][kOriginal]  ->Fill(P_Top.Pt(), fEvtWeight);
  fHistTopReco_eta[kHadronic][kOriginal] ->Fill(P_Top.Eta(), fEvtWeight);


  fHistTopMvsChi2[kHadronic][kFitted]      ->Fill(t->M("REC"), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kHadronic][kFitted]  ->Fill(t->M("REC"), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kHadronic][kFitted]     ->Fill(t->Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kHadronic][kFitted] ->Fill(t->Pt(), W->GetProb(), fEvtWeight);

  fHistTopMvsChi2[kHadronic][kOriginal]      ->Fill(P_Top.M(), W->GetChi2(), fEvtWeight);
  fHistTopMvsChi2prob[kHadronic][kOriginal]  ->Fill(P_Top.M(), W->GetProb(), fEvtWeight);
  fHistTopPtvsChi2[kHadronic][kOriginal]     ->Fill(P_Top.Pt(), W->GetChi2(), fEvtWeight);
  fHistTopPtvsChi2prob[kHadronic][kOriginal] ->Fill(P_Top.Pt(), W->GetProb(), fEvtWeight);


  // Filling hadronic W-Reco diagrams
  fHistWreco_M[kHadronic][kFitted]   ->Fill(W->M("REC"), fEvtWeight);
  fHistWreco_phi[kHadronic][kFitted] ->Fill(W->Phi(), fEvtWeight);
  fHistWreco_pt[kHadronic][kFitted]  ->Fill(W->Pt(), fEvtWeight);
  fHistWreco_eta[kHadronic][kFitted] ->Fill(W->Eta(), fEvtWeight);

  fHistWreco_M[kHadronic][kOriginal]   ->Fill(P_W.M(), fEvtWeight);
  fHistWreco_phi[kHadronic][kOriginal] ->Fill(P_W.Phi(), fEvtWeight);
  fHistWreco_pt[kHadronic][kOriginal]  ->Fill(P_W.Pt(), fEvtWeight);
  fHistWreco_eta[kHadronic][kOriginal] ->Fill(P_W.Eta(), fEvtWeight);


}


//___________________________________________________________

void AtlTopPairDocumenter::FillResidualHistograms(){
  //
  // Filling the pull histograms in order to check if kinfitter is working as intended
  //

  // Get appropriate W-bosons and top quarks
  HepTopDecay*  tlep = fTopPair->GetLeptonicTop();
  AtlWDecayLNu* WLNu = (AtlWDecayLNu*)tlep->GetWDecay();


  // Before fit
  AtlJet *lepbjet_orig = (AtlJet*)tlep->GetBJetOrig();
  HepParticle* lepton_orig = WLNu->GetChargedLeptonOrig();

  // After fit
  HepParticle   *lepton_fit   = WLNu->GetChargedLepton();
  HepParticle   *neutrino_fit = WLNu->GetNeutrino();
  TLorentzVector p_lepbjet_fit   = tlep->GetP_BJet();

  // Pull distributions
  fHistResidualLepBJet_Et->Fill((p_lepbjet_fit.Et() - lepbjet_orig->Et())/lepbjet_orig->Et(), fEvtWeight);
  fHistResidualLepBJet_pt->Fill((p_lepbjet_fit.Pt() - lepbjet_orig->Pt())/lepbjet_orig->Pt(), fEvtWeight);
  fHistResidualLepBJet_eta->Fill((p_lepbjet_fit.Eta() - lepbjet_orig->Eta())/lepbjet_orig->Eta(), fEvtWeight);
  fHistResidualLepBJet_phi->Fill((p_lepbjet_fit.Phi() - lepbjet_orig->Phi())/lepbjet_orig->Phi(), fEvtWeight);

  fHistResidualLepton_pt->Fill((lepton_fit->Pt() - lepton_orig->Pt())/lepton_orig->Pt(), fEvtWeight);
  fHistResidualLepton_eta->Fill((lepton_fit->Eta() - lepton_orig->Eta())/lepton_orig->Eta(), fEvtWeight);
  fHistResidualLepton_phi->Fill((lepton_fit->Phi() - lepton_orig->Phi())/lepton_orig->Phi(), fEvtWeight);

  fHistResidualEtmiss_Mag->Fill((neutrino_fit->Et() - fEtMissMag)/fEtMissMag, fEvtWeight);
  fHistResidualEtmiss_phi->Fill((neutrino_fit->Phi() - fEtMissPhi)/fEtMissPhi, fEvtWeight);


  // Get appropriate W-bosons and top quarks  
  HepTopDecay*  thad = fTopPair->GetHadronicTop();
  AtlWDecayJJ* WJJ = (AtlWDecayJJ*)thad->GetWDecay();

  // Before fit
  AtlJet *hadbjet_orig = (AtlJet*)thad->GetBJetOrig();
  AtlJet *hadjet1_orig = (AtlJet*)WJJ->GetJet1_Orig();
  AtlJet *hadjet2_orig = (AtlJet*)WJJ->GetJet2_Orig();

  // After fit
  TLorentzVector p_hadbjet_fit = thad->GetP_BJet();
  HepJet *hadjet1_fit = (HepJet*)WJJ->GetJet1();
  HepJet *hadjet2_fit = (HepJet*)WJJ->GetJet2();

  // Pull distributions
  fHistResidualHadBJet_Et->Fill((p_hadbjet_fit.Et() - hadbjet_orig->Et())/hadbjet_orig->Et(), fEvtWeight);
  fHistResidualHadBJet_pt->Fill((p_hadbjet_fit.Pt() - hadbjet_orig->Pt())/hadbjet_orig->Pt(), fEvtWeight);
  fHistResidualHadBJet_eta->Fill((p_hadbjet_fit.Eta() - hadbjet_orig->Eta())/hadbjet_orig->Eta(), fEvtWeight);
  fHistResidualHadBJet_phi->Fill((p_hadbjet_fit.Phi() - hadbjet_orig->Phi())/hadbjet_orig->Phi(), fEvtWeight);

  fHistResidualHadJet1_Et->Fill((hadjet1_fit->Et() - hadjet1_orig->Et())/hadjet1_orig->Et(), fEvtWeight);
  fHistResidualHadJet1_pt->Fill((hadjet1_fit->Pt() - hadjet1_orig->Pt())/hadjet1_orig->Pt(), fEvtWeight);
  fHistResidualHadJet1_eta->Fill((hadjet1_fit->Eta() - hadjet1_orig->Eta())/hadjet1_orig->Eta(), fEvtWeight);
  fHistResidualHadJet1_phi->Fill((hadjet1_fit->Phi() - hadjet1_orig->Phi())/hadjet1_orig->Phi(), fEvtWeight);

  fHistResidualHadJet2_Et->Fill((hadjet2_fit->Et() - hadjet2_orig->Et())/hadjet2_orig->Et(), fEvtWeight);
  fHistResidualHadJet2_pt->Fill((hadjet2_fit->Pt() - hadjet2_orig->Pt())/hadjet2_orig->Pt(), fEvtWeight);
  fHistResidualHadJet2_eta->Fill((hadjet2_fit->Eta() - hadjet2_orig->Eta())/hadjet2_orig->Eta(), fEvtWeight);
  fHistResidualHadJet2_phi->Fill((hadjet2_fit->Phi() - hadjet2_orig->Phi())/hadjet2_orig->Phi(), fEvtWeight);
}

//___________________________________________________________

void AtlTopPairDocumenter::FillPullHistograms(){
  //
  // Filling the pull histograms in order to check if kinfitter is working as intended
  //



  TMatrixD LeptonPull   = fTopPair->GetLeptonPull();
  TMatrixD NeutrinoPull = fTopPair->GetNeutrinoPull();
  TMatrixD LepBJetPull  = fTopPair->GetLepBJetPull(); 
  TMatrixD HadJet1Pull  = fTopPair->GetHadJet1Pull(); 
  TMatrixD HadJet2Pull  = fTopPair->GetHadJet2Pull(); 
  TMatrixD HadBJetPull  = fTopPair->GetHadBJetPull(); 

  // Pull distributions
  fHistPullLepBJet_pt->Fill(LepBJetPull(0,0), fEvtWeight);
  fHistPullLepBJet_eta->Fill(LepBJetPull(1,0), fEvtWeight);
  fHistPullLepBJet_phi->Fill(LepBJetPull(2,0), fEvtWeight);

  fHistPullLepton_pt->Fill(LeptonPull(0,0), fEvtWeight);
  fHistPullLepton_eta->Fill(LeptonPull(1,0), fEvtWeight);
  fHistPullLepton_phi->Fill(LeptonPull(2,0), fEvtWeight);

  fHistPullEtmiss_Mag->Fill(NeutrinoPull(0,0), fEvtWeight);
  fHistPullEtmiss_phi->Fill(NeutrinoPull(2,0), fEvtWeight);

  fHistPullHadBJet_pt->Fill(HadBJetPull(0,0), fEvtWeight);
  fHistPullHadBJet_eta->Fill(HadBJetPull(1,0), fEvtWeight);
  fHistPullHadBJet_phi->Fill(HadBJetPull(2,0), fEvtWeight);

  fHistPullHadJet1_pt->Fill(HadJet1Pull(0,0), fEvtWeight);
  fHistPullHadJet1_eta->Fill(HadJet1Pull(1,0), fEvtWeight);
  fHistPullHadJet1_phi->Fill(HadJet1Pull(2,0), fEvtWeight);

  fHistPullHadJet2_pt->Fill(HadJet2Pull(0,0), fEvtWeight);
  fHistPullHadJet2_eta->Fill(HadJet2Pull(1,0), fEvtWeight);
  fHistPullHadJet2_phi->Fill(HadJet2Pull(2,0), fEvtWeight);
}




