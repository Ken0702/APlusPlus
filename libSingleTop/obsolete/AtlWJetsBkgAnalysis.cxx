//____________________________________________________________________
//
//W+JetUndergroundAnalysis
//Analyse Undergroundevents fo SingleTop Events
// Author: Steven Bilski <mailto:bilski@physik.hu-berlin.de>
// Update: $Id: AtlWJetsBkgAnalysis.cxx,v 2011/06/20 Bilski
// Copyright: 2011 (C) Steven Bilski


#ifndef ATLAS_AtlWJetsBkgAnalysis
#include <AtlWJetsBkgAnalysis.h>
#endif
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <AtlEvent.h>
#include <AtlJet.h>
#include <HepMCParticle.h>





using namespace std;

#ifndef __CINT__
ClassImp(AtlWJetsBkgAnalysis);
#endif

//____________________________________________________________________

AtlWJetsBkgAnalysis::AtlWJetsBkgAnalysis(const char* OutputFile) :
  AtlSelector(OutputFile) {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlWJetsBkgAnalysis::~AtlWJetsBkgAnalysis() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlWJetsBkgAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    
    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
    // Switch on only branches we are going to use
    // This will speed up the analysis process considerably
    fTree->SetBranchStatus("fEventHeader*",   kTRUE);
    fTree->SetBranchStatus("fEnergySum*",     kTRUE);
    fTree->SetBranchStatus("fN_AtlFastJets*", kTRUE);
    fTree->SetBranchStatus("fAtlFastJets*",   kTRUE);
    fTree->SetBranchStatus("fN_MCParticles*", kTRUE);
    fTree->SetBranchStatus("fMCParticles*",   kTRUE);
    fTree->SetBranchStatus("fN_Muons*",       kTRUE);
    fTree->SetBranchStatus("fMuons*",         kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4TopoEMJets*",  kTRUE);
    fTree->SetBranchStatus("fAntiKt4TopoEMJets*",  kTRUE);
    fTree->SetBranchStatus("fCone4H1TowerJets*",    kTRUE);
    fTree->SetBranchStatus("fN_IDTracks*",          kTRUE);  
    fTree->SetBranchStatus("fIDTracks*",            kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",         kTRUE);
    fTree->SetBranchStatus("fVertices*",           kTRUE);
    fTree->SetBranchStatus("fMCVertices*",         kTRUE);
    fTree->SetBranchStatus("fN_MCVertices*",       kTRUE);

}

//____________________________________________________________________

void AtlWJetsBkgAnalysis::BookHistograms() {
    //
    // Book histograms for WJetsBkg_Analysis
    //
       
    // Save present working directory
    //TDirectory *savdir = gDirectory;

    fOutputFile->cd();
    gDirectory->mkdir("MC_Particle_informations","Informations of Monte Carlo Particle");
    gDirectory->cd("MC_Particle_informations");
    
    gDirectory->mkdir("Single Tops","Histogramms for Top and Top_bar");
    gDirectory->cd("Single Tops");

    //Number of Single Tops per Event in MC Sample
    fHist_NSgTop = new TH1I("h_NSgTop","Number of MC Single Tops per Event",10,0.,10.);
    fHist_NSgTop->SetXTitle("#SgTops/Event (GeV)");
    fHist_NSgTop->SetYTitle("Number of Entries");

   
    fHist_NSgTop_bar = new TH1F("h_NSgTop_bar","Number of MC Single Tops_bar per Event",10,0.,10.);
    fHist_NSgTop_bar->SetXTitle("#SgTops/Event (GeV)");
    fHist_NSgTop_bar->SetYTitle("Number of Entries");
    
    fHist_NSgTopTop_bar = new TH1F("h_NSgTopTop_bar","Number of MC Single Tops+Tops_bar per Event",10,0.,10.);
    fHist_NSgTopTop_bar->SetXTitle("#SgTops/Event (GeV)");
    fHist_NSgTopTop_bar->SetYTitle("Number of Entries");
        

    //Measure Pt of Single_Tops
    fHist_SgTop_Pt = new TH1F("h_SgTop_Pt","MC Single Top Transverse Momentum",200,0,200);
    fHist_SgTop_Pt->SetXTitle("P_{t} (GeV)");
    fHist_SgTop_Pt->SetYTitle("Number of Entries");
    
    fHist_SgTopBar_Pt = new TH1F("h_SgTopBar_Pt","MC Single Top_bar Transverse Momentum",200,0,200);
    fHist_SgTopBar_Pt->SetXTitle("P_{t} (GeV)");
    fHist_SgTopBar_Pt->SetYTitle("Number of Entries");

    fHist_SgTopTopBar_Pt = new TH1F("h_SgTopTopBar_Pt","MC Single Top-Top_bar Transverse Momentum",200,0,200);
    fHist_SgTopTopBar_Pt->SetXTitle("P_{t} (GeV)");
    fHist_SgTopTopBar_Pt->SetYTitle("Number of Entries");
    
    fHist_NJets = new TH1F("h_NJets"," Number of Jets per Event",20,0,20);
    fHist_NJets->SetXTitle("# Jets per Event");
    fHist_NJets->SetYTitle("# entries");
    
    fHist_FwrdJet_Pt = new TH1F("h_FwrdJet_Pt","MC FwrdJet Single Top Transverse Momentum",200,0,200);
    fHist_FwrdJet_Pt->SetXTitle("P_{t} (GeV)");
    fHist_FwrdJet_Pt->SetYTitle("Number of Entries");
    
    fHist_SumPxPyOfTopAndJet = new TH1F("h_SumPxPYOfTopAndJet","Sum Of Momentum in X-Y-Plane for Top and Jet",200,0,200);
    fHist_SumPxPyOfTopAndJet->SetXTitle("|P_{t}^{Jet}+P_{t}^{Top}| (GeV)");
    fHist_SumPxPyOfTopAndJet->SetYTitle("Number of Entries");
    
    fHist_SumPxPyOfTopAndJet_EMScale = new TH1F("h_SumPxPYOfTopAndJet_EMScale","Sum Of Momentum EMScale in X-Y-Plane for Top and Jet",200,0,200);
    fHist_SumPxPyOfTopAndJet_EMScale->SetXTitle("|P_{t}^{Jet}+P_{t}^{Top}| (GeV)");
    fHist_SumPxPyOfTopAndJet_EMScale->SetYTitle("Number of Entries");
    
    fHist_SumPxPyOfTopAndJet_JES = new TH1F("h_SumPxPYOfTopAndJet_JES","Sum Of Momentum JES in X-Y-Plane for Top and Jet",200,0,200);
    fHist_SumPxPyOfTopAndJet_JES->SetXTitle("|P_{t}^{Jet}+P_{t}^{Top}| (GeV)");
    fHist_SumPxPyOfTopAndJet_JES->SetYTitle("Number of Entries");
    
    fHist_SumPxPyOfTopAndJetQuark = new TH1F("h_SumPxPYOfTopAndJetQuark","Sum Of Momentum in X-Y-Plane for Top and JetQuark",200,0,200);
    fHist_SumPxPyOfTopAndJetQuark->SetXTitle("|P_{t}^{JetQuark}+P_{t}^{Top}| (GeV)");
    fHist_SumPxPyOfTopAndJetQuark->SetYTitle("Number of Entries");
    
    fHist_DiffPtOfTopAndJetQuark = new TH1F("h_DiffPtOfTopAndJetQuark","Diff Of Ptfor Top and JetQuark",200,0,200);
    fHist_DiffPtOfTopAndJetQuark->SetXTitle("|P_{t}^{JetQuark}-P_{t}^{Top}| (GeV)");
    fHist_DiffPtOfTopAndJetQuark->SetYTitle("Number of Entries");
    
    
    
    fHist_JetTop_DeltaPhi = new TH1F("h_JetTop_DeltaPhi","Difference of Phi in x-y-plane for Jet and Top",63,0,3.15);
    fHist_JetTop_DeltaPhi->SetXTitle("|Phi_{Jet}-Phi_{Top}|");
    fHist_JetTop_DeltaPhi->SetYTitle("Number of entries");
    
    fHist_JetQuarkTop_DeltaPhi = new TH1F("h_JetQuarkTop_DeltaPhi","Difference of Phi in x-y-plane for Quark,which products the jet and Top",63,0,3.15);
    fHist_JetQuarkTop_DeltaPhi->SetXTitle("|Phi_{JetQuark}-Phi_{Top}|");
    fHist_JetQuarkTop_DeltaPhi->SetYTitle("Number of entries");    
    
    // No. of AtlFast jets
    /*fHistAtlFastNJets = new TH1F("h_atlfast_njets", "No. of AtlFast Jets",
				 10, 0, 10);
    fHistAtlFastNJets->SetXTitle("N_{Jets}");
    fHistAtlFastNJets->SetYTitle("Number of Entries");
    
    //Create folder for MC truth information
    fOutputFile->cd();
    gDirectory->mkdir("mc_truth", "Monte Carlo Truth info");
    gDirectory->cd("mc_truth");
    
    // MC particle's Pt
    fHistMCPrtPt = new TH1F("h_mcprt_pt", "MC Particle Transverse Momentum",
			    200, 0, 200);
    fHistMCPrtPt->SetXTitle("P_{t} (GeV)");
    fHistMCPrtPt->SetYTitle("Number of Entries");
    
    // MC particle's Eta-Phi plane
    fHistMCPrtEtaPhi = new TH2F("h_mcprt_etaphi", "MC Particle Eta-Phi",
				100, -5, 5, 128, -3.2, 3.2);
    fHistMCPrtEtaPhi->SetXTitle("#eta");
    fHistMCPrtEtaPhi->SetYTitle("#phi (rad)");
    fHistMCPrtEtaPhi->SetZTitle("Number of Entries");
    
    // Create folder for muon information
    fOutputFile->cd();
    gDirectory->mkdir("muon", "Staco Muons");
    gDirectory->cd("muon");

    // No. of muons
    fHistNMuons = new TH1F("h_nmuons", "No. of Muons per Event", 10, 0, 10);
    fHistNMuons->SetXTitle("N_{#mu}");
    fHistNMuons->SetYTitle("Number of Entries");

    // Muon Pt
    fHistMuonPt = new TH1F("h_muon_pt", "Muon Transverse Momentum",
			   100, 0, 100);
    fHistMuonPt->SetXTitle("P_{t} (GeV)");
    fHistMuonPt->SetYTitle("Number of Entries");
    
    // Restore pwd
    savdir->cd();*/
}

//____________________________________________________________________

void AtlWJetsBkgAnalysis::FillHistograms() {
    //
    // Fill histograms
    //

    //MC STUDY
    if (fEvent->IsMC()){
	//Fill Histograms to get number of Tops 
	fHist_NSgTop		->Fill(fEvent->GetMCParticles("t")->GetEntries(),fEvtWeight);
	fHist_NSgTop_bar	->Fill(fEvent->GetMCParticles("t_bar")->GetEntries(),fEvtWeight);
	fHist_NSgTopTop_bar	->Fill(fEvent->GetMCParticles("t")->GetEntries()+fEvent->GetMCParticles("t_bar")->GetEntries(),fEvtWeight);
	
	//get the Single Top 
	TList *GoodMCTops = fEvent->GetMCTops();
	if (GoodMCTops->GetEntries()>1){cout<<"\n\n\t ATTENTION! More than one Top -> No Single Top Event!\n\n";}
	//get foward jet
	AtlJet *FwrdJet = GetFwrdJet();
	if (FwrdJet==0){cout<<"\n\n\t No Forwards Jet found!\n\n";}
	//
	//
	//Fill histogramms for FrdJet and good top only for singletop events
	if ((GoodMCTops->GetEntries()==1) && !(FwrdJet==0)){
	    
	    //get the true mc quark,which products the jet
	    HepMCParticle *JetQuark=0;
	    TIter nextQuarkOfJet(FwrdJet->GetTruthParticles());
	    while ((JetQuark=(HepMCParticle*)nextQuarkOfJet())){
		if (!(JetQuark->IsBeautyQuark())) break;
	    }//end while
	    
	    //Fill Histogramm with momentum of tops
	    HepMCParticle * top = (HepMCParticle*)GoodMCTops->At(0);
	    if (top->IsTop()){
		fHist_SgTop_Pt		->Fill(top->Pt(),fEvtWeight);
		fHist_SgTopTopBar_Pt	->Fill(top->Pt(),fEvtWeight);
	    }else if (top->IsTopBar()){
		fHist_SgTopBar_Pt	->Fill(top->Pt(),fEvtWeight);
		fHist_SgTopTopBar_Pt	->Fill(top->Pt(),fEvtWeight);
	    }//end else if
	    //Fill Histograms with Pt of Forward Jet
	    fHist_FwrdJet_Pt		->Fill(FwrdJet->Pt(),fEvtWeight);
	    
	    //calculate sum of momentum in x-y-plane
	    if (TMath::Abs(FwrdJet->DeltaPhi(top))>2.7){
		fHist_SumPxPyOfTopAndJet->Fill(GetSumPt(top->Px(),top->Py(),(Float_t)FwrdJet->Px(),(Float_t)FwrdJet->Py()),fEvtWeight);
		fHist_SumPxPyOfTopAndJet_EMScale->Fill(GetSumPt(top->Px(),top->Py(),FwrdJet->EMScalePx(),FwrdJet->EMScalePy()),fEvtWeight);
		fHist_SumPxPyOfTopAndJet_JES->Fill(GetSumPt(top->Px(),top->Py(),FwrdJet->JES_Px(),FwrdJet->JES_Py()),fEvtWeight);
		//fill Hist delta phi
		fHist_JetTop_DeltaPhi->Fill(TMath::Abs(FwrdJet->DeltaPhi(top)),fEvtWeight);
		if (!(JetQuark==0)){
		    fHist_JetQuarkTop_DeltaPhi->Fill(TMath::Abs(JetQuark->DeltaPhi(top)),fEvtWeight);
		    fHist_SumPxPyOfTopAndJetQuark->Fill(GetSumPt(top->Px(),top->Py(),JetQuark->Px(),JetQuark->Py()),fEvtWeight);
		    fHist_DiffPtOfTopAndJetQuark->Fill(TMath::Abs(top->Pt()-JetQuark->Pt()),fEvtWeight);
		}
	    }
    
	    //fill Hist delta phi
	    fHist_JetTop_DeltaPhi->Fill(TMath::Abs(FwrdJet->DeltaPhi(top)),fEvtWeight);
	    if (!(JetQuark==0)){
		fHist_JetQuarkTop_DeltaPhi->Fill(TMath::Abs(JetQuark->DeltaPhi(top)),fEvtWeight);
	    }
	}//end if
    delete GoodMCTops;
    }//Is MC  
}

//____________________________________________________________________

Bool_t AtlWJetsBkgAnalysis::ProcessCut() {
    //
    // Event selection
    //
    // In this example only events containing at least one muon with a
    // transverse momentum higher than 10 GeV are chosen
    //

    //if ( fEvent->GetN_PhiDecaysKK() > 0 ) return kTRUE;
    //  return kFALSE;
    // return kTRUE;
    // if ( fEvent->GetN_Muons() > 0 ) {
// 	for ( Int_t i = 0; i < fEvent->GetN_Muons(); i++ ) {
// 	    if ( ((AtlMuon*)fEvent->GetMuons()->At(i))->Pt() >= 10. )
		return kTRUE;
// 	}
//     }
//     return kFALSE;
}

//___________________________________________________________________

void AtlWJetsBkgAnalysis::Terminate(){
    
      AtlSelector::Terminate();
     
}

//_______________________________________________________________________

AtlJet* AtlWJetsBkgAnalysis::GetFwrdJet(){
    //
    //returns the highest pt foward jet which is not a b-jet
    //
    
    Float_t Et_min = 20.;
    Float_t Et_max = 10e10;
    Float_t Eta_min = -2.5;
    Float_t Eta_max = 2.5;
    Bool_t Good = kTRUE;
    Bool_t sort = kTRUE;
    Bool_t remove_faked = kTRUE;
    
    //Loop over all Jet-Algorithms
    TList *Jets = fEvent -> GetJets(AtlJet::kAntiKt4TopoEM,Et_min,Et_max,Eta_min,Eta_max,Good,sort,remove_faked);
    fHist_NJets->Fill(Jets->GetEntries(),fEvtWeight);
    AtlJet *FwrdJet = 0;
    //Float_t JetPt = 0.;
    if (Jets->GetEntries()==0){
	cout<<"\n\n\tError in function GetFwrdJet: No Jets in Event -> List is empty!\n";
	delete Jets;
	return FwrdJet;
    }
    //AtlJet *Jet=0;
    //cout<<"\n\tNumber of Jets: "<<Jets->GetEntries()<<"\n";
    
    TIter nextjet(Jets);
    //Int_t i=0;
    while ((FwrdJet=(AtlJet*)nextjet())){
	if (!(FwrdJet->GetTruthFlavour()==AtlJet::kBeauty)) break;
	//i++;
	//cout<<"Jet "<<i<<"\tPt = "<<FwrdJet->Pt()<<"\tJetQuark: Pt = "<<((HepMCParticle*)FwrdJet->GetTruthParticles()->At(0))->Pt()<<"\n";
    }//end while    
    delete Jets;
    return FwrdJet;
}

//_______________________________________________________________________



/*void AtlWJetsBkgAnalysis::FillHistograms() {
    //
    // Fill histograms
    //

    
    if (fEvent->IsMC()){
      

      //Declaration
      TList *top_MCList    = fEvent->GetMCParticles("t");
      TList *topbar_MCList = fEvent->GetMCParticles("t_bar");
      HepMCParticle *top   =0;		//MC tops
      HepMCParticle *topbar=0;
      Int_t NrMCtop    = top_MCList->GetEntries();	//Number of Entries in MC lists
      Int_t NrMCtopbar = topbar_MCList->GetEntries();
      
      //Fill Histogramm: Get Number of Top per Event
      fHist_NSgTop  	  -> Fill(NrMCtop);
      fHist_NSgTop_bar 	  -> Fill(NrMCtopbar);
      fHist_NSgTopTop_bar -> Fill(NrMCtop+NrMCtopbar);
      
     
      //GET THE CORRECT TOP QUARK
      //--------------------------------------------------------------
      //check if list of top or topbar is filled
      if (NrMCtop>0){
	  
	  Bool_t correctTop=kFALSE;
	  //go through list and get top quark with quark and W-Boson as daughter, t is generator and t is good mother
	  for (Int_t i=NrMCtop-1;i>=0;i--){
	  	
	      Bool_t containsW=kFALSE;
	      Bool_t containsQuark=kFALSE;
	      
	      top = (HepMCParticle*)top_MCList->At(i);
	      Int_t NDaughters=top->GetN_Daughters();
	    	      
	      //SELECTION OF TOP
	      if (NDaughters<2) continue;
	      if (!top->IsGenerator()) continue;
	      if (!top->IsGoodMother()) continue;
	      //check if W and quark are daughters of top
	      for (Int_t j=0;j<NDaughters;j++){
		  if (((HepMCParticle*)top->GetDaughters()->At(j))->IsWBoson()) containsW=kTRUE;  
		  if (((HepMCParticle*)top->GetDaughters()->At(j))->IsQuark()&& !((HepMCParticle*)top->GetDaughters()->At(j))->IsTop()) containsQuark=kTRUE; 
	      }//end for j
	      if (containsW && containsQuark){
		  correctTop=kTRUE;
		  break; //correct top is found
	      }
	  }//end for i
	  //fill Histogramm if correct top was found
	  if (correctTop){
	      cout<<"\n\tDaughters:\n\n";
	      for (Int_t j=0;j<top->GetN_Daughters();j++){
		  cout<<"\t"<<((HepMCParticle*)top->GetDaughters()->At(j))->GetPdgName()<<"\t ID: "<<((HepMCParticle*)top->GetDaughters()->At(j))->GetId();
	      }
	      cout<<"\n";
	  }else {
	      top=0;
	      cout<<"\n\n\tNo Good TopQuark found!\n\n";
	  }
      }//end if
      else if (NrMCtopbar>0){
	  
	  Bool_t correctTopbar=kFALSE;
	  //go through list and get topbar quark with quark and W-Boson as daughter, t is generator and t is good mother
	  for (Int_t i=NrMCtopbar-1;i>=0;i--){
	  	
	      Bool_t containsW=kFALSE;
	      Bool_t containsQuark=kFALSE;
	      
	      topbar = (HepMCParticle*)topbar_MCList->At(i);
	      Int_t NDaughters=topbar->GetN_Daughters();
	    
	      
	      //SELECTION OF TOP_BAR
	      if (NDaughters<2) continue;
	      if (!topbar->IsGenerator()) continue;
	      if (!topbar->IsGoodMother()) continue;
	      //check if W and quark are daughters of topbar
	      for (Int_t j=0;j<NDaughters;j++){
		  if (((HepMCParticle*)topbar->GetDaughters()->At(j))->IsWBoson()) containsW=kTRUE;  
		  if (((HepMCParticle*)topbar->GetDaughters()->At(j))->IsQuark()&& !((HepMCParticle*)topbar->GetDaughters()->At(j))->IsTop()) containsQuark=kTRUE; 
	      }//end for j
	      if (containsW && containsQuark){
		  correctTopbar=kTRUE;
		  break; //correct top_bar is found
	      }
	  }//end for i
	  //fill Histogramm if correct top_bar was found
	  if (correctTopbar){
	      cout<<"\n\tDaughters:\n\n";
	      for (Int_t j=0;j<topbar->GetN_Daughters();j++){
		  cout<<"\t"<<((HepMCParticle*)topbar->GetDaughters()->At(j))->GetPdgName()<<"\t ID: "<<((HepMCParticle*)topbar->GetDaughters()->At(j))->GetId();
	      }
	      cout<<"\n";
	  }else {
	      topbar=0;
	      cout<<"\n\n\tNo Good TopQuark found!\n\n";
	  }
	
      }//end else if
      else {
	cout<<"\n\n\tNo TopQuarks in Event! RunNr.: "<<fEvent->RunNr()<<"EventNr.:"<<fEvent->EventNr()<<"\n\n";
      }
      //---------------------------------------------------------------------------------------------------------
      //FINISH GET TOP CORRECT TOP QUARK
      //---------------------------------------------------------------------------------------------------------
      
      
      //---------------------------------------------------------------------------------------------------------
      //FILL HISTOGRAMMS
      //---------------------------------------------------------------------------------------------------------
      
      if (!top==0){
	  fHist_SgTop_Pt->Fill(top->Pt());
	  fHist_SgTopTopBar_Pt->Fill(top->Pt());
      }else if (!topbar==0){
	  fHist_SgTopBar_Pt->Fill(topbar->Pt());
	  fHist_SgTopTopBar_Pt->Fill(topbar->Pt());
      }
      
      //delete the lists
      delete top_MCList;
      delete topbar_MCList;
      }//is MC
}*/
