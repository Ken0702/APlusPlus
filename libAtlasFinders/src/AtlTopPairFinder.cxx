//___________________________________________________________
//
// Top Pair Event Finder
//
// This Finder is looking for events with top pairs whereas one W-boson decays leptonically
// ( W -> e nu, w-> mu nu ) and one decays hadronically ( W -> jj ). Reconstruction of the
// W-bosons and top-quarks is carried out by employing a kinematic fit.
//
// First, the finder makes sure that sufficient numbers of physics objects for the 
// reconstruction of a top pair are present in the event. This requires the following:
// 
//  - 4 jets
//  - 1 lepton
//
// Since there is no requirement concerning b-jets some special cases have to be taken into
// account:
//
//  - 0 b-jets in b-jet list:   b-jet list is replaced by jet list
//  - 1 b-jet in b-jet list:    b-jet list is replaced by jet list, if statement makes sure that
//                              the b-jet in the event is used for the b-jets of the candidates
//  - > 2 b-jets in b-jet list: list is kept as it is
//
// Because of the case with 1 b-jet in the list the information concerning the tagger (type 
// and cut weight) have to be given to the finder. This is not optimal and if someone has a
// solution it would be highly appreciated.
// 
// Now the top pair candidates are built by combining the contents of the lists (jet list, 
// b-jet list, lepton list). If-statements make sure that all candidates are unique. The evaluation
// of the candidates takes place and assigns a chi2 and a chi2 probability value to each candidate.
// All candidates that satisfy a certain chi2prob cut are recorded in the event and may be 
// accessed at a later point in the analysis via fEvent->GetTopPairs().
// The candidate with the lowest chi2 (the highest chi2 probability) is deemed to be best top pair of
// the event. It is stored in the variable fBestTopPair and may be obtained from the TopFinder via
// the GetBestTopPair() - method.
//
//
//
// When the AtlTopCommonObjectsTool is used, all of the required information is provided automatically.
// In order to do this you have to give a pointer of the AtlTopCommonObjectsTool itself to the top #
// finder. How this is done is illustrated in the AtlFlavorRatioAnalysis in libTTbar.
//
// The fitter will use the first n jets from the fJets list provided by the user. The number of jets n can be set
// by the user with fJets_N_Max. The default is 20 jets. Take care that the fJets list is sorted when forwarding them to the fitter.
//
//
//    Author: Felix Thomas <mailto:Felix.Thomas@mailbox.tu-dresden.de> 
//    Update: $Id$
//    Copyright: 2010 (C) Felix Thomas  
//
//
//
#ifndef ATLAS_AtlTopPairFinder
#include <AtlTopPairFinder.h>
#endif
#include <TH1F.h>
#include <TH2F.h>
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <cmath>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTopPairFinder);
#endif

//___________________________________________________________

AtlTopPairFinder::AtlTopPairFinder(const char* name, const char* title) : 
AtlKinFitterTool(name, title) {
  //
  // Default constructor
  //

  fBestTopPair = 0;
  
  fLeptons     = 0;
  fBJets       = 0;
  fJets        = 0;
  fKinFitJets  = 0;

  fLepton      = 0;
  fNeutrino    = 0;
  fLepBJet     = 0;
  fHadBJet     = 0;
  fHadJet1     = 0;
  fHadJet2     = 0;

  fEtMissMag   = 0;
  fEtMissPhi   = 0;


  // Initialize covariance matrices
  cov_lep.ResizeTo(3,3);
  cov_nu.ResizeTo(3,3);
  cov_LepBJet.ResizeTo(3,3);
  cov_HadBJet.ResizeTo(3,3);
  cov_HadJet1.ResizeTo(3,3);
  cov_HadJet2.ResizeTo(3,3);

  cov_lep.Zero();
  cov_nu.Zero();
  cov_LepBJet.Zero();
  cov_HadBJet.Zero();
  cov_HadJet1.Zero();
  cov_HadJet2.Zero();

  fLeptonpull.ResizeTo(3,1);
  fNeutrinopull.ResizeTo(3,1);
  fLepbjetpull.ResizeTo(3,1);
  fHadjet1pull.ResizeTo(3,1);
  fHadjet2pull.ResizeTo(3,1);
  fHadbjetpull.ResizeTo(3,1);

  SetCutDefaults();

}

//___________________________________________________________

AtlTopPairFinder::~AtlTopPairFinder(){
  //
  // default destructor
  //

    //cout<<"TopPairFinder is deconstructed"<<endl;

  if ( fBJets      != 0 ) delete fBJets;
  if ( fKinFitJets != 0 ) delete fKinFitJets;
  if ( fNeutrino   != 0 ) delete fNeutrino;

  fBestTopPair = 0;

  fLepton   = 0;
  fLepBJet  = 0;
  fHadBJet  = 0;
  fHadJet1  = 0;
  fHadJet2  = 0;


}

//___________________________________________________________

void AtlTopPairFinder::SetBranchStatus() {
  //
  // Switch on needed branches
  //

  fTree->SetBranchStatus("fEventHeader*",               kTRUE);
  fTree->SetBranchStatus("fN_AntiKt4TopoEMJESJets*",    kTRUE);
  fTree->SetBranchStatus("fAntiKt4TopoEMJESJets*",      kTRUE);
  fTree->SetBranchStatus("fN_AntiKt4TopoEMJets*",       kTRUE);
  fTree->SetBranchStatus("fAntiKt4TopoEMJets*",         kTRUE);
  fTree->SetBranchStatus("fN_Electrons*",               kTRUE);
  fTree->SetBranchStatus("fElectrons*",                 kTRUE);
  fTree->SetBranchStatus("fN_Muons*",                   kTRUE);
  fTree->SetBranchStatus("fMuons*",                     kTRUE);
  fTree->SetBranchStatus("fEnergySum*",                 kTRUE);
  fTree->SetBranchStatus("fWDecaysLNu*",                kTRUE);
  fTree->SetBranchStatus("fN_WDecaysLNu*",              kTRUE);
  fTree->SetBranchStatus("fWDecaysJJ*",                 kTRUE);
  fTree->SetBranchStatus("fN_WDecaysJJ*",               kTRUE);
  fTree->SetBranchStatus("fTopDecays*",                 kTRUE);
  fTree->SetBranchStatus("fN_TopDecays*",               kTRUE);
  fTree->SetBranchStatus("fTrigger*",                   kTRUE);
  fTree->SetBranchStatus("fTopPairs*",                  kTRUE);
}

//___________________________________________________________

void AtlTopPairFinder::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    
    // Define when the finder runs and how
    fProcessMode = AtlAnalysisTool::kMainAnalysis;
    fTmassconstraint = AtlTopPairFinder::kFixedTmass;
    fMinChi2Prob = 0.001;
    
    fIsFwdElectronMode = kFALSE;
    
    // Histogram filling mode
    fAlwaysFillHistos = kFALSE;
    
    // compute maximum chi2 for histograms
    Int_t dof = 0;
    
    if (fTmassconstraint == kFixedTmass)   dof = 3;
    else if (fTmassconstraint == kSameTmass)    dof = 4;
    
    for (Double_t f = 0; f < 100; f += 0.1){
	if (TMath::Prob(f, dof) < fMinChi2Prob){
	    fMaxChi2 = f;
	    break;
	}
    }  
    
    
    // ======================
    // Pole masses and widths
    // ======================

    fW_Mass   = 80.385;// <-- +-0.015 online PDG value 2012 
    fW_Width  = 2.085;// <-- +- 0.042 online PDG value 2012
    fTop_Mass = 173.5;// <-- +-0.6 +-0.8 (direct measurements) online PDG value 2012
    fTop_Width = 2.0; // <-- +0.7-0.6  online PDG value 2012;  quite uncertain, not measured, depends on mass
    
  
    // fBJets                                                  
    fBJetTagger        = AtlBTag::kMV1;
    fBJetWeightMin     = 0.905363;         
    fIterMax	       = 200;
    
    fApplyBTagInReco   = kTRUE;        
    fBJets_N_Min       = 1;
    fBJets_N_Min_InHadCombo = 0;
    fJets_N_Max        = 20;
    fQuietMode         = kFALSE; 
}

//___________________________________________________________

void AtlTopPairFinder::Clear(){
 //
 //  Clears the dynamically allocated objects
 //

  if ( fKinFitJets != 0 ){
    delete fKinFitJets;
    fKinFitJets = 0;
  }

  if ( fBJets != 0 ){
    delete fBJets;
    fBJets = 0;      
  }

  if ( fNeutrino != 0 ){
    delete fNeutrino;
    fNeutrino = 0;
  }

  fBestTopPair = 0;

}

//___________________________________________________________

void AtlTopPairFinder::BookHistograms(){
  //
  // Book histograms (after kinematic fit)
  //

  fHistNbIterAll = new TH1F("h_NbIterAll",
			      "No. of Iterations for all Fitting Procedures",
			      25, 0, 250);
  fHistNbIterAll->SetXTitle("# Iterations");
  fHistNbIterAll->SetYTitle("# fitting procedures");
  
  fHistNbIterConv = new TH1F("h_NbIterConv",
			       "No. of Iterations for converged Fitting Procedures",
			       25, 0, 250);
  fHistNbIterConv->SetXTitle("# Iterations");
  fHistNbIterConv->SetYTitle("# fitting procedures");

  fHistNTopPairs = new TH1F("h_NTopPairs",
			       "No. Top Pairs reconstructed in the event",
			       50, 0, 50);
  fHistNTopPairs->SetXTitle("# Top Pairs");
  fHistNTopPairs->SetYTitle("# Events");

  fHistNTopPairCandidates = new TH1F("h_NTopPairCandidates",
			       "No. Top Pair Candidates in the event",
			       300, 0, 300);
  fHistNTopPairCandidates->SetXTitle("# Top Pair Candidates");
  fHistNTopPairCandidates->SetYTitle("# Events");

  fHistTopPairsVsCandidates = new TH2F("h_toppairsvscandidates", "No of reconstructed Top Pairs vs Top Pair Candidates in the Event",
                                        300,0,300,50,0,50);
  fHistTopPairsVsCandidates->SetXTitle("# Top Pair Candidates");
  fHistTopPairsVsCandidates->SetYTitle("# Top Pairs");
  fHistTopPairsVsCandidates->SetZTitle("# Events");

  fHistChi2VsCandidates = new TH2F("h_chi2vscandidates", "Best Chi2 vs Top Pair Candidates in the Event",
                                        300,0,300,20,0,fMaxChi2);
  fHistChi2VsCandidates->SetXTitle("# Top Pair Candidates");
  fHistChi2VsCandidates->SetYTitle("# Chi2");
  fHistChi2VsCandidates->SetZTitle("# Events");

  fHistChi2ProbVsCandidates = new TH2F("h_chi2probvscandidates", "Best Chi2Prob vs Top Pair Candidates in the Event",
                                        300,0,300,20,0,1);
  fHistChi2ProbVsCandidates->SetXTitle("# Top Pair Candidates");
  fHistChi2ProbVsCandidates->SetYTitle("# Chi2Prob");
  fHistChi2ProbVsCandidates->SetZTitle("# Events");

  fHistSimpleChi2VsChi2 = new TH2F("h_simplechi2vschi2", "Simple Chi2 vs. Chi2 of all top pair candidates in the event",
                                        100,0,fMaxChi2,100,0,fMaxChi2);
  fHistSimpleChi2VsChi2->SetXTitle("Simple Chi2");
  fHistSimpleChi2VsChi2->SetYTitle("Chi2");
  fHistSimpleChi2VsChi2->SetZTitle("# Candidates ");



  fAllTopPairDocumenter = new AtlTopPairDocumenter("all", "all", fOutputFile, "TopPairs_all");
  fElectronChannelTopPairDocumenter = new AtlTopPairDocumenter("electron", "electron", fOutputFile, 
                                                               "TopPairs_electron");
  fMuonChannelTopPairDocumenter = new AtlTopPairDocumenter("muon", "muon", fOutputFile,
                                                           "TopPairs_muon");
  
}

//___________________________________________________________

Bool_t AtlTopPairFinder::AnalyzeEvent() {
    //
    // Event analysis
    //
    //cout<<"TopPairFinder: AnalyzeEvent"<<endl;
    
    // Initialize the current event
    InitEvent();
    
    fNTopPairCandidates = 0;
    fBestChi2Prob = fMinChi2Prob;
  
    //
    // Check the requirements for the reconstruction
    //
    
    if ( !fAlwaysFillHistos ) {
     	if (fQuietMode == kTRUE) return kFALSE;
    }
    
    if (fKinFitJets->GetEntries() < 4 || fKinFitJets->GetEntries() > fJets_N_Max || fLeptons->GetEntries() < 1) return kFALSE;
    
    // Split analysis on jet-electrons into events with signal electrons that
    // are forward and central
    HepParticle *lepton = (HepParticle*)fLeptons->At(0);
  if ( (lepton->IsElectron()) && fUseLeptonCovRand ) {
      Float_t lep_eta_abs = TMath::Abs(lepton->Eta());
      if ( ( ( lep_eta_abs < 1.5) && fIsFwdElectronMode ) ||
	   ( ( lep_eta_abs >= 1.5) && !fIsFwdElectronMode ) )
	  return kFALSE;
  }
  
  
  if (fApplyBTagInReco == kTRUE && fBJets->GetEntries() < fBJets_N_Min) return kFALSE;
  
  if (fBJets->GetEntries() == 0 || fApplyBTagInReco == kFALSE){
      ReconstructionKinFit(fLeptons, fKinFitJets, fKinFitJets, fKinFitJets, fKinFitJets);}
  else if (fBJets->GetEntries() == 1){
    ReconstructionKinFit(fLeptons, fBJets, fKinFitJets, fKinFitJets, fKinFitJets);
    ReconstructionKinFit(fLeptons, fKinFitJets, fKinFitJets, fKinFitJets, fBJets);      
  }
  else{
    ReconstructionKinFit(fLeptons, fBJets, fKinFitJets, fKinFitJets, fBJets);     
  }
  return kTRUE;
}

//___________________________________________________________

void AtlTopPairFinder::InitEvent() {
  //
  // Prepare event analysis
  //

  // ======
  // Etmiss
  // ======
  
  fEtMissMag = fEtMiss.Mod();
  fEtMissPhi = TVector2::Phi_mpi_pi((Double_t)fEtMiss.Phi());
  
  // Initialize jets for kinematic fit if not done already
  fKinFitJets = new TList();
  fBJets = new TList();		

  if( fVerbosityLevel==2 ) Info("InitEvent", "Got %d Jets", fJets->GetEntries() );

  TObjLink *lnk = fJets->FirstLink();
  AtlJet* jet = 0;
  TIter next_jet(fJets);

  if( fVerbosityLevel==2 ){
      Info("InitEvent", "fJetList \n" );
      while ( (jet=(AtlJet*)next_jet()) ) {
	  jet->Print();
      }
  }
  
  int n = 0;
  if( fVerbosityLevel==2 ) Info("InitEvent", "fKinFitJetList \n" );
  while ( lnk && n < fJets_N_Max ) {
      jet = (AtlJet*)lnk->GetObject();
      lnk = lnk->Next();
      fKinFitJets->Add(jet);
      //jet->Print();
      if ( jet->GetTag(fBJetTagger)->GetWeight() > fBJetWeightMin){
	  fBJets->Add(jet);
      }
      n++;      
  }

  if( fVerbosityLevel==2 ) Info("InitEvent", "Got %d KinFitter Jets", fKinFitJets->GetEntries() );
  
  fNeutrino = new HepParticle(1, 0., 0., 1., 0., 12);
  
  fNeutrino->SetPtEtaPhiE(fEtMissMag,
			  0.,
			  fEtMissPhi,
			  fEtMissMag);
}

//___________________________________________________________

void AtlTopPairFinder::ReconstructionKinFit(TList* Leptons, TList* LepBJets, TList* HadJets1, TList* HadJets2, TList* HadBJets) {


    //cout<<"TopPairFinder reconstructs kinfit"<<endl;
    
  //
  // Top Pair leptonic channel event reconstruction (semi-leptonic)
  //
  // --> loop over all lepton and bjet combinations
  // --> perform the fit
  // --> select the combination with the greatest chi2prob as candidate 
  //     for a signal event
  //
  // Use fMinChi2Prob as starting value for chi2prob,
  // reconstruction successfull if chi2prob gets bigger
    Double_t Chi2Prob = 0;
  Double_t SimpleChi2 = 0;
  AtlTopPair* toppair = 0;
  
  // Declaring variables for best fit storage in the loop
  TLorentzVector P_lep_refit;
  HepParticle* LeptonOrig = 0;
  TLorentzVector P_nu_refit;
  HepParticle* BestNeutrino = 0;
  TLorentzVector P_HadJet1_refit;
  AtlJet* HadJet1Orig = 0;
  TLorentzVector P_HadJet2_refit;
  AtlJet* HadJet2Orig = 0;
  TLorentzVector P_LepBJet_refit;
  AtlJet* LepBJetOrig = 0;
  TLorentzVector P_HadBJet_refit;
  AtlJet* HadBJetOrig = 0;

  // ==============================================
  // Loop over all combinations of leptons and bjets
  // ==============================================

  // Initializing links for the iteration of the lists
  TObjLink *LeptonLink  = Leptons->FirstLink();
  TObjLink *LepBJetLink = LepBJets->FirstLink();
  TObjLink *HadJet1Link = HadJets1->FirstLink();
  TObjLink *HadJet2Link = HadJets2->FirstLink();
  TObjLink *HadBJetLink = HadBJets->FirstLink();

  // Loop over all leptons
  while( LeptonLink ) {
      fLepton = (HepParticle*)LeptonLink->GetObject();
      LeptonLink = LeptonLink->Next();
      
      
      // Loop over all Jets
      HadJet1Link = HadJets1->FirstLink();
      while( HadJet1Link ) {
	  fHadJet1 = (AtlJet*)HadJet1Link->GetObject();
	  HadJet1Link = HadJet1Link->Next();
	  
	  // HadJet2Link is set to the position that HadJet1Link is pointing to
	  // This is done in order to avoid that the same jet is used for both
	  // jets in the fitting routine and in order to speed up calculation
	  
	  HadJet2Link = HadJet1Link;                         
	  while( HadJet2Link ) {
	      fHadJet2 = (AtlJet*)HadJet2Link->GetObject();
	      HadJet2Link = HadJet2Link->Next(); 
	      
	      // Looping over all bjets for the leptonic decay
	      LepBJetLink = LepBJets->FirstLink();
	      while( LepBJetLink ) {
		  fLepBJet = (AtlJet*)LepBJetLink->GetObject();
		  LepBJetLink = LepBJetLink->Next();
		  
		  // Avoid using the same BJet
		  if ( (fLepBJet->P() == fHadJet1->P()) ||
		       (fLepBJet->P() == fHadJet2->P()) ) {
		      continue;
		  }
		  
		  // Setting the kinematic variables for the neutrino
		  Double_t NuEta = 0;
		  Double_t NuE   = 0;
		  GuessTopDecayNeutrinoEtaE(NuEta, NuE, fEtMiss, fLepton, fLepBJet);
		  fNeutrino->SetPtEtaPhiE(NuE/TMath::CosH(NuEta), NuEta, fNeutrino->Phi(), NuE);
		  
		  // Looping over all bjets for the hadronic decay
		  HadBJetLink = HadBJets->FirstLink();
		  while( HadBJetLink ) {
		      fHadBJet = (AtlJet*)HadBJetLink->GetObject();
		      HadBJetLink = HadBJetLink->Next();
		      
		      // Avoiding using the same bjet
		      if ( (fHadBJet->P() == fHadJet1->P()) ||
			   (fHadBJet->P() == fHadJet2->P()) ||
			   (fHadBJet->P() == fLepBJet->P()) ){
			  continue;
		      }

		      if ( fBJets_N_Min_InHadCombo > 0 ) {
			  int taggedJetsInHadCombo = 0; 
			  AtlJet* fitjets[3] = {fHadJet1,fHadJet2,fHadBJet};
			  for( int hadjet=0; hadjet<3; ++hadjet ) {
			      if ( fitjets[hadjet]->GetTag(fBJetTagger)->GetWeight() > fBJetWeightMin ){
				  taggedJetsInHadCombo++;
			      }
			  }
			  if ( taggedJetsInHadCombo < fBJets_N_Min_InHadCombo ) continue;
		      }
		      SimpleChi2 = (pow(((fHadBJet->P() + fHadJet1->P() + fHadJet2->P()).M() - fTop_Mass)/fTop_Width,2) + 	    
				    pow(((fLepBJet->P() + fLepton->P() + fNeutrino->P()).M() - fTop_Mass)/fTop_Width,2) + 
				    pow(((fHadJet1->P() + fHadJet2->P()).M() - fW_Mass)/fW_Width,2) + 
				    pow(((fLepton->P() + fNeutrino->P()).M() - fW_Mass)/fW_Width,2))/25.;
		      
		      // ================================================
		      // Perform the fit and increment candidates counter
		      // ================================================
		      
		      DoFit();
		      fNTopPairCandidates++;
		      
		      // if fit fails because of bad chi2 or invalid covariance matrix the number of iterations
		      // is not recorded 
		      if (fKinFitter->getStatus() != -1) {
			  fHistNbIterAll->Fill(fKinFitter->getNbIter());
		      }
		      // Did KinFitter converge ? ("0" means "yes")

		      if ( (fKinFitter->getStatus() != 0) ||
			   (TMath::Prob(fKinFitter->getS(), fKinFitter->getNDF()) < fMinChi2Prob) ){ 
			  continue;
		      }
		      fHistNbIterConv->Fill(fKinFitter->getNbIter());
	        
		      SetChi2(fKinFitter->getS());
		      SetNDoF(fKinFitter->getNDF());
		      fHistSimpleChi2VsChi2->Fill(SimpleChi2, fKinFitter->getS(), GetTagEvtWeight());
		      Chi2Prob = TMath::Prob(fKinFitter->getS(), fKinFitter->getNDF());
		      
		      // Is the Chi2 smaller than that of the previous fit?
		      if (Chi2Prob < fBestChi2Prob) continue;
		      
		      // Set the correct neutrino type w.r.t. the lepton
		      if ( fLepton->IsEPlus() ) {
			  fNeutrino->SetPdgCode(12);
		      } 
		      else if ( fLepton->IsEMinus()   ) {
			  fNeutrino->SetPdgCode(-12);
		      } 
		      else if ( fLepton->IsMuPlus()   ) {
			  fNeutrino->SetPdgCode(14);
		      } 
		      else if ( fLepton->IsMuMinus()  ) {
			  fNeutrino->SetPdgCode(-14);
		      } 
		      else if ( fLepton->IsTauPlus()  ) {
			  fNeutrino->SetPdgCode(16);
		      } 
		      else if ( fLepton->IsTauMinus() ) {
			  fNeutrino->SetPdgCode(-16);
		      }
		      
		      // Get the improved 4-momenta of the outgoing particles (temporary storage)
		      P_lep_refit  = *(((TAbsFitParticle*)fKinFitter->getParticle(0))->getCurr4Vec());
		      LeptonOrig = fLepton;
		      
		      P_nu_refit   = *(((TAbsFitParticle*)fKinFitter->getParticle(1))->getCurr4Vec());
		      BestNeutrino = fNeutrino;
		      
		      P_LepBJet_refit = *(((TAbsFitParticle*)fKinFitter->getParticle(2))->getCurr4Vec());
		      LepBJetOrig = fLepBJet;
		      
		      P_HadJet1_refit  = *(((TAbsFitParticle*)fKinFitter->getParticle(3))->getCurr4Vec());
		      HadJet1Orig = fHadJet1;
		      
		      P_HadJet2_refit  = *(((TAbsFitParticle*)fKinFitter->getParticle(4))->getCurr4Vec());
		      HadJet2Orig = fHadJet2;
		      
		      P_HadBJet_refit  = *(((TAbsFitParticle*)fKinFitter->getParticle(5))->getCurr4Vec());
		      HadBJetOrig = fHadBJet;
		      
		      // =============================================================
		      // Add newly reconstructed particles/decays to the current event
		      // =============================================================
		      
		      // Add neutrino
		      HepParticle *nu_refit = fEvent->AddNeutrino(P_nu_refit.Px(), P_nu_refit.Py(),
								  P_nu_refit.Pz(), P_nu_refit.E(),
								  BestNeutrino->GetPdgCode());
		      
		      // Add leptonic W decay
		      TLorentzVector P_WLNu = P_lep_refit + P_nu_refit;
		      HepWDecay *WdecayLNu = fEvent->AddWDecayLNu(P_WLNu.Px(), P_WLNu.Py(),
								  P_WLNu.Pz(), P_WLNu.E(),
								  LeptonOrig,
								  P_lep_refit.Px(), P_lep_refit.Py(),
								  P_lep_refit.Pz(), P_lep_refit.E(),
								  nu_refit,HepWDecay::kTTBar);
		      WdecayLNu->SetChi2NDoF(GetChi2(), GetNDoF());
            
            
		      // Add leptonic top decay
		      TLorentzVector P_toplep = P_WLNu + P_LepBJet_refit;
		      HepTopDecay *toplepdecay = fEvent->AddTopDecay(P_toplep.Px(), P_toplep.Py(), 
								     P_toplep.Pz(), P_toplep.E(), 
								     WdecayLNu, LepBJetOrig,
								     P_LepBJet_refit.Px(), P_LepBJet_refit.Py(),
								     P_LepBJet_refit.Pz(), P_LepBJet_refit.E(), 
								     HepTopDecay::kTTBar);
		      
		      toplepdecay->SetChi2NDoF(GetChi2(), GetNDoF() );
		      
		      // Add hadronic W decay
		      TLorentzVector P_WJJ = P_HadJet1_refit + P_HadJet2_refit;
		      HepWDecay *WdecayJJ = fEvent->AddWDecayJJ(P_WJJ.Px(), P_WJJ.Py(),
                                                      P_WJJ.Pz(), P_WJJ.E(),
								HadJet1Orig, HadJet2Orig,
								P_HadJet1_refit.Px(), P_HadJet1_refit.Py(),
								P_HadJet1_refit.Pz(), P_HadJet1_refit.E(),
								P_HadJet2_refit.Px(), P_HadJet2_refit.Py(),
								P_HadJet2_refit.Pz(), P_HadJet2_refit.E(),
								HepWDecay::kTTBar);
		      WdecayJJ->SetChi2NDoF(GetChi2(), GetNDoF() );
		      
		      // Add hadronic top decay
		      TLorentzVector P_tophad = P_WJJ + P_HadBJet_refit;
		      HepTopDecay *tophaddecay = fEvent->AddTopDecay(P_tophad.Px(), P_tophad.Py(), 
								     P_tophad.Pz(), P_tophad.E(), 
								     WdecayJJ, HadBJetOrig,
								     P_HadBJet_refit.Px(), P_HadBJet_refit.Py(),
								     P_HadBJet_refit.Pz(), P_HadBJet_refit.E(), 
								     HepTopDecay::kTTBar);
            
		      tophaddecay->SetChi2NDoF(GetChi2(), GetNDoF() );
		      
		      toppair = fEvent->AddTopPair(toplepdecay, tophaddecay, GetChi2(), GetNDoF(), AtlTopPair::kSemiLeptonic);
	    
		      toppair->SetPullMatrices(fLeptonpull, fNeutrinopull, fLepbjetpull, 
					       fHadjet1pull, fHadjet2pull, fHadbjetpull);
          
		      if ( toppair->GetChi2Prob() > fBestChi2Prob){
			  fBestTopPair = toppair;
			  fBestChi2Prob = fBestTopPair->GetChi2Prob();
		      }
            
		  } // end of hadronic bjet loop
	      } // end of leptonic bjet loop
	  } // end of second hadronic jet loop
      } // end of first hadronic jet loop
  } // end of charged lepton loop
}

//___________________________________________________________

void AtlTopPairFinder::DoFit() {
  //
  // Perform the kinematic fit for the current charged lepton,
  // neutrino and b-jet combination
  //

  // Initialize pull matrices
  fLeptonpull.Zero();  
  fNeutrinopull.Zero();
  fLepbjetpull.Zero();  
  fHadjet1pull.Zero();  
  fHadjet2pull.Zero();  
  fHadbjetpull.Zero();  
    
  // Re-set and init fitter
  fKinFitter->reset();
  fKinFitter->setMaxNbIter(fIterMax);    // maximum number of iterations
  fKinFitter->setMaxDeltaS(5.e-5);  // maximum deviation of the minimum function
                                    // within two iterations
  fKinFitter->setMaxF(1.e-4);       // maximum value of constraints
  fKinFitter->setVerbosity(0);      // verbosity level

  if ( fUseLeptonCovRand )
      // Get its covariance matrix
      AtlKinFitterTool::GetLeptonCovMatRand(fLepton,cov_lep);
  else
      // Get its covariance matrix
      fLepton->GetCovMatrixPtEtaPhi(cov_lep);
  
  // Preliminary correlation values for missing Et
  SetCovMatrix(cov_nu, fNeutrino);
  
  // Preliminary correlation values for b-jets
  SetCovMatrix(cov_LepBJet, fLepBJet);
  SetCovMatrix(cov_HadBJet, fHadBJet);
  
  // Preliminary correlation values for jets
  SetCovMatrix(cov_HadJet1, fHadJet1);
  SetCovMatrix(cov_HadJet2, fHadJet2);

  // As the covariance matrix of the EtMiss sometimes has 0 for the variance of the
  // Et (which makes the fit somewhat suspicious ) this is tested and cases where it 
  // arises are discarded  
  if (cov_nu[0][0] == 0){
    Error("DoFit",
  	  "Variance of Transverse Energy of Neutrino is 0! Candidate discarded.");
    fKinFitter->resetStatus();
    return;  
  }

  // =============
  // Get 3-momenta
  // =============

  TVector3 p3_lepton   = TVector3(fLepton->P3());
  TVector3 p3_neutrino = TVector3(fNeutrino->P3());
  TVector3 p3_LepBJet  = TVector3(fLepBJet->P3());
  TVector3 p3_HadJet1  = TVector3(fHadJet1->P3());
  TVector3 p3_HadJet2  = TVector3(fHadJet2->P3());
  TVector3 p3_HadBJet  = TVector3(fHadBJet->P3());
  
  // ================
  // Define particles
  // ================

  // Necessary for execution of the fit
  TFitParticlePtEtaPhi   KinFit_lepton ("KinFit_lepton", "Charged lepton", &p3_lepton, fLepton->Mass("PDG"), &cov_lep);
  TFitParticlePtThetaPhi KinFit_nu     ("KinFit_nu", "Neutrino", &p3_neutrino, 0., &cov_nu);
  TFitParticlePtEtaPhi   KinFit_LepBJet("KinFit_LepBJet", "LepBJet", &p3_LepBJet, 4.5, &cov_LepBJet); // use B mass
  TFitParticlePtEtaPhi   KinFit_HadJet1("KinFit_HadJet1", "HadJet1", &p3_HadJet1, 0, &cov_HadJet1);
  TFitParticlePtEtaPhi   KinFit_HadJet2("KinFit_HadJet2", "HadJet1", &p3_HadJet2, 0, &cov_HadJet2);
  TFitParticlePtEtaPhi   KinFit_HadBJet("KinFit_HadBJet", "HadBJet", &p3_HadBJet, 4.5, &cov_HadBJet); // use B mass
  
  
  
  // Add particles to fitter
  fKinFitter->addMeasParticle(&KinFit_lepton);
  fKinFitter->addMeasParticle(&KinFit_nu);
  fKinFitter->setParamUnmeas(&KinFit_nu, 1); // eta (component 1) of neutrino unmeasured
  fKinFitter->addMeasParticle(&KinFit_LepBJet);
  fKinFitter->addMeasParticle(&KinFit_HadJet1);
  fKinFitter->addMeasParticle(&KinFit_HadJet2);
  fKinFitter->addMeasParticle(&KinFit_HadBJet);


  // ==================
  // Define Constraints
  // ==================
  // Definition of top-quark and W-boson mass constraints

  // W-boson mass constraint
  TFitConstraintMGaus ConstraintLepM_W("WMassConstraint Leptonic", "WMassConstraintGaus Leptonic", 
                                        0, 0, fW_Mass, fW_Width);
  TFitConstraintMGaus ConstraintHadM_W("WMassConstraint Hadronic", "WMassConstraintGaus Hadronic",
                                        0, 0, fW_Mass, fW_Width);
  
  ConstraintLepM_W.addParticles1(&KinFit_lepton, &KinFit_nu);
  ConstraintHadM_W.addParticles1(&KinFit_HadJet1, &KinFit_HadJet2);
  
  fKinFitter->addConstraint(&ConstraintLepM_W);
  fKinFitter->addConstraint(&ConstraintHadM_W);
  

  // Same top mass fit constraint
  TFitConstraintM ConstraintM_t( "MassConstraint Tops", "Mass-Constraint Tops", 0, 0, 0.);
  ConstraintM_t.addParticles1(&KinFit_lepton, &KinFit_nu, &KinFit_LepBJet);
  ConstraintM_t.addParticles2(&KinFit_HadJet1, &KinFit_HadJet2, &KinFit_HadBJet);

  // fixed top mass fit constraint
  TFitConstraintMGaus ConstraintLepM_t( "MassConstraint Tops", "Mass-Constraint Tops",
                                        0, 0, fTop_Mass, fTop_Width);
  TFitConstraintMGaus ConstraintHadM_t( "MassConstraint Tops", "Mass-Constraint Tops",
                                        0, 0, fTop_Mass, fTop_Width);
  ConstraintLepM_t.addParticles1(&KinFit_lepton, &KinFit_nu, &KinFit_LepBJet);
  ConstraintHadM_t.addParticles1(&KinFit_HadJet1, &KinFit_HadJet2, &KinFit_HadBJet);
  
  // setting which constraint is used
  if ( fTmassconstraint == kSameTmass){
    fKinFitter->addConstraint(&ConstraintM_t);
  }
  else if ( fTmassconstraint == kFixedTmass ){
    fKinFitter->addConstraint(&ConstraintLepM_t);
    fKinFitter->addConstraint(&ConstraintHadM_t);
  }
  else{
    Error("DoFit",
    "No valid mass constraint given. Abort!");
      
  }
  
  
  // =========================
  // Perform Kinematic Fitting 
  // =========================

  fKinFitter->fit();
  
  //
  // Get pull values ( pull := [value(fit) - value(initial)] / sqrt[variance of corrections] )
  //
  fLeptonpull   = *((TMatrixD*)((TAbsFitParticle*)fKinFitter->getParticle(0))->getPull());
  fNeutrinopull = *((TMatrixD*)((TAbsFitParticle*)fKinFitter->getParticle(1))->getPull());
  fLepbjetpull  = *((TMatrixD*)(((TAbsFitParticle*)fKinFitter->getParticle(2))->getPull()));
  fHadjet1pull  = *((TMatrixD*)(((TAbsFitParticle*)fKinFitter->getParticle(3))->getPull()));
  fHadjet2pull  = *((TMatrixD*)(((TAbsFitParticle*)fKinFitter->getParticle(4))->getPull()));
  fHadbjetpull  = *((TMatrixD*)(((TAbsFitParticle*)fKinFitter->getParticle(5))->getPull()));

  
  // Fitting okay ?
  if ( fKinFitter->getS() < 0. ) {
    Error("DoFit",

	  "Chi2 negative!!! Possibly bad input of covariance matrices. FitStatus set to not converged!");
      fKinFitter->resetStatus();
      return;
  }
}

//___________________________________________________________

void AtlTopPairFinder::FillHistograms() {

  //
  // Method for filling histograms, only events that survive 
  // AnalyzeEvent() are taken into account
  //
  
  // Check if 2 top decays labeled as ttbar are available
  if ( fBestTopPair == 0 ) return;
  
  fHistNTopPairs->Fill(fEvent->GetN_TopPairs(), GetTagEvtWeight());
  fHistNTopPairCandidates->Fill(fNTopPairCandidates, GetTagEvtWeight());
  
  fHistTopPairsVsCandidates->Fill(fNTopPairCandidates, fEvent->GetN_TopPairs(), GetTagEvtWeight());
  fHistChi2ProbVsCandidates->Fill(fNTopPairCandidates, fBestTopPair->GetChi2Prob(), GetTagEvtWeight());
  fHistChi2VsCandidates->Fill(fNTopPairCandidates, fBestTopPair->GetChi2(), GetTagEvtWeight());
 
  // Get lepton from reconstruction
  HepParticle* Lepton = 0;
  Lepton = ((AtlWDecayLNu*)((HepTopDecay*)fBestTopPair->GetLeptonicTop())->GetWDecay())->GetChargedLeptonOrig();  
    
  // Invoking procedures for filling histograms
  fAllTopPairDocumenter->Document(fBestTopPair, &fEtMiss, GetTagEvtWeight());
  if (Lepton->IsElectron()) fElectronChannelTopPairDocumenter->Document(fBestTopPair, &fEtMiss, GetTagEvtWeight());
  else if (Lepton->IsMuon()) fMuonChannelTopPairDocumenter->Document(fBestTopPair, &fEtMiss, GetTagEvtWeight());
  
}

//___________________________________________________________

void AtlTopPairFinder::Print() const {
    //
    // Print configuration of tool
    //
    
    cout << endl
	 << "========================================================" << endl
	 << "  Top Pair Finder Tool                                  " << endl
	 << "========================================================" << endl
	 << "  fProcessMode               = " << GetProcessModeName(fProcessMode) << endl
	 << "--------------------------------------------------------" << endl
	 << "  fIterMax                   = " << fIterMax  << endl
	 << "  fQuietMode                 = " << fQuietMode            << endl
	 << "  fUseLeptonCovRand          = " << ( (fUseLeptonCovRand) ? "kTRUE" : "kFALSE" ) << endl;
    if ( fUseLeptonCovRand )
	cout
	    << "  fIsFwdElectronMode         = " << ( (fIsFwdElectronMode) ? "kTRUE" : "kFALSE" ) << endl;
    cout
	 << "  fAlwaysFillHistos          = " << ( ( fAlwaysFillHistos == kTRUE ) ?
	                                         "kTRUE" : "kFALSE" )            << endl
	 << "  fTmassconstraint           = " << ( (fTmassconstraint == AtlTopPairFinder::kFixedTmass) ?
						   "kFixedTmass" : "kSameTmass" ) << endl
	 << "  fW_Mass                    = " << fW_Mass               << endl
	 << "  fW_Width                   = " << fW_Width              << endl
	 << "  fTop_Mass                  = " << fTop_Mass             << endl
	 << "  fTop_Width                 = " << fTop_Width            << endl
	 << "  fMinChi2Prob               = " << fMinChi2Prob          << endl
	 << "  fJets_N_Max                = " << fJets_N_Max           << endl
	 << "--------------------------------------------------------" << endl
	 << "  fBJetTagger                = " << AtlBTag::GetTaggerName(fBJetTagger)     << endl
	 << "  fBJetWeightMin             = " << fBJetWeightMin        << endl
	 << "  fBJets_N_Min               = " << fBJets_N_Min          << endl
	 << "========================================================" << endl
	 << endl;

}

//___________________________________________________________

void AtlTopPairFinder::Terminate(){

 //
 // Executed at the end of the tool
 //

}

