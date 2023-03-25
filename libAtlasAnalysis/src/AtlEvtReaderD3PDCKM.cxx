//____________________________________________________________________
//
// Event reader for slimmed Top D3PDs used by AtlSelector
// 
//  
// Author: Christopher Schmitt <mailto: Christopher.Schmitt@cern.ch>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDCKM
#include <AtlEvtReaderD3PDCKM.h>
#endif
#include <AtlEventHeader.h>
#include <HepVertex.h>
#include <iostream>
#include <TTree.h>


using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderD3PDCKM);
#endif


//____________________________________________________________________

AtlEvtReaderD3PDCKM::AtlEvtReaderD3PDCKM(AtlSelector *parent) :
    AtlEvtReaderD3PDSgTop(parent) {
    //
    // Default constructor
    //
    fJPM_bQuarkJet 	= new TList;
    fJPM_sQuarkJet	= new TList;
    fJPM_bQuark	    	= new TList;
    fJPM_sQuark     	= new TList; 
    
}

//____________________________________________________________________

AtlEvtReaderD3PDCKM::~AtlEvtReaderD3PDCKM() {
    //
    // Default destructor
    //
    delete fJPM_bQuarkJet;    
    delete fJPM_sQuarkJet;    
    delete fJPM_bQuark;    
    delete fJPM_sQuark; 
    
   
}


// //___________________________________________________________
// 
// void AtlEvtReaderD3PDCKM::Clear() {
//     //
//     //  Clears the dynamically allocated objects
//     //     
// //     fJPM_bQuarkJet->Clear();
// //     fJPM_sQuarkJet->Clear();
// //     fJPM_bQuark->Clear();
// //     fJPM_sQuark->Clear();  
// // 
// //     fJPM_NumbJetsMatching.clear();
//     
//   
// }


//____________________________________________________________________

void AtlEvtReaderD3PDCKM::InitObjPointers() {
  
  
    AtlEvtReaderD3PDSgTop::InitObjPointers();

    //
    // Init object pointers
    //

    
    // v0
    v_v0_n = 0;
    v_v0_E = 0;
    v_v0_m = 0;
    v_v0_sumPt = 0;
    v_v0_x = 0;
    v_v0_y = 0;
    v_v0_z = 0;
    v_v0_nTracks = 0;
    v_v0_radius = 0;

   
    
    // mc truth   
    v_mc_n = 0;  
    v_mc_E = 0;
    v_mc_pt = 0;
    v_mc_eta = 0;
    v_mc_phi = 0;
    v_mc_status = 0;
    v_mc_index = 0;
    v_mc_pdgId = 0;     
    v_mc_parent_index = 0;     
    v_mc_child_index = 0;     
    
    
    
    // Tracks
    v_trk_n = 0;
    v_trk_n_jet = 0;
    v_trk_index = 0;
    v_trk_pt = 0;
    v_trk_eta = 0;
    v_trk_d0_wrtPV = 0;
    v_trk_z0_wrtPV = 0;
    v_trk_phi_wrtPV = 0;
    v_trk_theta_wrtPV = 0;
    v_trk_qoverp_wrtPV = 0; 

    v_trk_cov_d0_wrtPV = 0;
    v_trk_cov_z0_wrtPV = 0;    
    v_trk_cov_phi_wrtPV = 0;
    v_trk_cov_theta_wrtPV = 0;   
    v_trk_cov_qoverp_wrtPV = 0; 
    v_trk_chi2 = 0;
    v_trk_ndof = 0;
    
    v_trk_nSCTHits = 0;
    v_trk_nSCTSpoiltHits = 0;
    
    
    
    
    //Jets  
    v_jet_flavor_weight_SV1 = 0;
    v_jet_flavor_weight_JetFitterCOMBNNc = 0;
    v_jet_flavor_component_jfitcomb_pu = 0;
    v_jet_flavor_component_jfitcomb_pb = 0;
    v_jet_flavor_component_jfitcomb_pc = 0;
    v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc = 0;
    v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu = 0;
    v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu = 0;
    
    v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass = 0;
    v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc = 0;
    v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2 = 0;
    v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid = 0;
    v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv = 0;

    v_jet_AntiKt4LCTopoJets_nTrk = 0;
    
    
    
    
    //vxp
    // v_vxp_n = 0;// already in AtlEvtReaderD3PDSgTop   
    v_vxp_nTracks = 0;
    v_vxp_type = 0;
    v_vxp_x = 0;
    v_vxp_y = 0;
    // v_vxp_z = 0;// already in AtlEvtReaderD3PDSgTop   
    v_vxp_trk_index = 0;
    
    

    
    
}

//____________________________________________________________________

void AtlEvtReaderD3PDCKM::SetBranches(TTree *t) {
  
  //
    // Set branch assignments of the input D3PD tree.
    //
    // Note that at this stage methods like TTree::FindBranch()
    // etc. do not work because the tree is not yet loaded from the
    // input file(s) and thus its structure not really known. For this
    // the Notify() function can be used.
    //
  
  AtlEvtReaderD3PDSgTop::SetBranches(t);

    // ====================
    // Set branch addresses
    // ====================

    
    // v0
    SetupBranch(t, "v0_n", &v_v0_n, &b_v0_n);
    SetupBranch(t, "v0_E", &v_v0_E, &b_v0_E);
    SetupBranch(t, "v0_m", &v_v0_m, &b_v0_E);
    SetupBranch(t, "v0_sumPt", &v_v0_sumPt, &b_v0_sumPt);
    SetupBranch(t, "v0_x", &v_v0_x, &b_v0_x);
    SetupBranch(t, "v0_y", &v_v0_y, &b_v0_y);
    SetupBranch(t, "v0_z", &v_v0_z, &b_v0_z);
    SetupBranch(t, "v0_nTracks", &v_v0_nTracks, &b_v0_nTracks);
    SetupBranch(t, "v0_radius", &v_v0_radius, &b_v0_radius);

   
    
    // mc truth   
    SetupBranch(t, "mc_n", &v_mc_n, &b_mc_n);
    SetupBranch(t, "mc_E", &v_mc_E, &b_mc_E);
    SetupBranch(t, "mc_pt", &v_mc_pt, &b_mc_pt);
    SetupBranch(t, "mc_eta", &v_mc_eta, &b_mc_eta);
    SetupBranch(t, "mc_phi", &v_mc_phi, &b_mc_phi);
    SetupBranch(t, "mc_status", &v_mc_status, &b_mc_status);
    SetupBranch(t, "mc_index", &v_mc_index, &b_mc_index);
    SetupBranch(t, "mc_pdgId", &v_mc_pdgId, &b_mc_pdgId); 
    SetupBranch(t, "mc_parent_index", &v_mc_parent_index, &b_mc_parent_index);
    SetupBranch(t, "mc_child_index", &v_mc_child_index, &b_mc_child_index);
    
    
    
    // Tracks
    SetupBranch(t, "trk_n", &v_trk_n, &b_trk_n);
    SetupBranch(t, "trk_n_jet", &v_trk_n_jet, &b_trk_n_jet);
    SetupBranch(t, "trk_index", &v_trk_index, &b_trk_index);
    SetupBranch(t, "trk_pt", &v_trk_pt, &b_trk_pt);
    SetupBranch(t, "trk_eta", &v_trk_eta, &b_trk_eta);
    SetupBranch(t, "trk_d0_wrtPV", &v_trk_d0_wrtPV, &b_trk_d0_wrtPV);
    SetupBranch(t, "trk_z0_wrtPV", &v_trk_z0_wrtPV, &b_trk_z0_wrtPV);
    SetupBranch(t, "trk_phi_wrtPV", &v_trk_phi_wrtPV, &b_trk_phi_wrtPV);
    SetupBranch(t, "trk_theta_wrtPV", &v_trk_theta_wrtPV, &b_trk_theta_wrtPV);
    SetupBranch(t, "trk_qoverp_wrtPV", &v_trk_qoverp_wrtPV, &b_trk_qoverp_wrtPV);

    SetupBranch(t, "trk_cov_d0_wrtPV", &v_trk_cov_d0_wrtPV, &b_trk_cov_d0_wrtPV);
    SetupBranch(t, "trk_cov_z0_wrtPV", &v_trk_cov_z0_wrtPV, &b_trk_cov_z0_wrtPV);
    SetupBranch(t, "trk_cov_phi_wrtPV", &v_trk_cov_phi_wrtPV, &b_trk_cov_phi_wrtPV);
    SetupBranch(t, "trk_cov_theta_wrtPV", &v_trk_cov_theta_wrtPV, &b_trk_cov_theta_wrtPV);
    SetupBranch(t, "trk_cov_qoverp_wrtPV", &v_trk_cov_qoverp_wrtPV, &b_trk_cov_qoverp_wrtPV);
    SetupBranch(t, "trk_chi2", &v_trk_chi2, &b_trk_chi2);
    SetupBranch(t, "trk_ndof", &v_trk_ndof, &b_trk_ndof);
    
    SetupBranch(t, "trk_nSCTHits", &v_trk_nSCTHits, &b_trk_nSCTHits);
    SetupBranch(t, "trk_nSCTSpoiltHits", &v_trk_nSCTSpoiltHits, &b_trk_nSCTSpoiltHits);
    
    
    
    
    //Jets  
    SetupBranch(t, "jet_flavor_weight_SV1", &v_jet_flavor_weight_SV1, &b_jet_flavor_weight_SV1);
    SetupBranch(t, "jet_flavor_weight_JetFitterCOMBNNc", &v_jet_flavor_weight_JetFitterCOMBNNc, &b_jet_flavor_weight_JetFitterCOMBNNc);
    SetupBranch(t, "jet_flavor_component_jfitcomb_pu", &v_jet_flavor_component_jfitcomb_pu, &b_jet_flavor_component_jfitcomb_pu);
    SetupBranch(t, "jet_flavor_component_jfitcomb_pb", &v_jet_flavor_component_jfitcomb_pb, &b_jet_flavor_component_jfitcomb_pb);
    SetupBranch(t, "jet_flavor_component_jfitcomb_pc", &v_jet_flavor_component_jfitcomb_pc, &b_jet_flavor_component_jfitcomb_pc);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc", &v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc, &b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSc);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu", &v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu, &b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_bVSu);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu", &v_jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu, &b_jet_AntiKt4LCTopoJets_flavor_weight_MV3_cVSu);
    
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass", &v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass, &b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_mass);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc", &v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc, &b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_efrc);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2", &v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2, &b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_chi2);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid", &v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid, &b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_isValid);
    SetupBranch(t, "jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv", &v_jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv, &b_jet_AntiKt4LCTopoJets_flavor_component_sv0p_ntrkv);

    SetupBranch(t, "jet_AntiKt4LCTopoJets_nTrk", &v_jet_AntiKt4LCTopoJets_nTrk, &b_jet_AntiKt4LCTopoJets_nTrk);
    
    
    
    
    //vxp
    // v_vxp_n", &v_// already in AtlEvtReaderD3PDSgTop   
    SetupBranch(t, "vxp_nTracks", &v_vxp_nTracks, &b_vxp_nTracks);
    SetupBranch(t, "vxp_type", &v_vxp_type, &b_vxp_type);
    SetupBranch(t, "vxp_x", &v_vxp_x, &b_vxp_x);
    SetupBranch(t, "vxp_y", &v_vxp_y, &b_vxp_y);
    // v_vxp_z", &v_// already in AtlEvtReaderD3PDSgTop   
    SetupBranch(t, "vxp_trk_index", &v_vxp_trk_index, &b_vxp_trk_index);
    
}

//____________________________________________________________________

void AtlEvtReaderD3PDCKM::BuildEvent() {
    //
    // Build A++ event from D3PD content
    //
    // Build header

    AtlEvtReaderD3PDSgTop::BuildEvent();
    
    
    // Build MC neutrinos
    BuildMCParticles(); // no problem with BuildMCNeutrinos since "mc_neutrino_final_E" does not exist.
    
    // Perform Jet Parton Matching
    FillJPM_sbQuarkJet();

}

//____________________________________________________________________



void AtlEvtReaderD3PDCKM::BuildJets() {
    //
    // Adding Trks to Jet
    //

    // ==================
    // Loop over all jets
    // ==================

    
    
    AtlEvtReaderD3PDSgTop::BuildJets();

    for ( unsigned int j = 0; j < v_trk_ndof->size(); ++j ) {
	AtlIDTrack *trk = fEvent->AddIDTrack(v_trk_chi2->at(j), v_trk_ndof->at(j),
						 0., 0., 0., // No ref values (might be set to prim vtx)
						 v_trk_phi_wrtPV->at(j), //not stored in ntuple
						 v_trk_qoverp_wrtPV->at(j)*1000.,
						 v_trk_d0_wrtPV->at(j)/10.,
						 v_trk_z0_wrtPV->at(j)/10.,
						 v_trk_theta_wrtPV->at(j));
	
	
// 	Int_t Id, Float_t Chi2, Int_t NDoF,
// 		       Float_t Xref, Float_t Yref, Float_t Zref,
// 		       Float_t Phi0, Float_t QovP, Float_t D0,
// 		       Float_t Z0, Float_t Theta0, const Float_t CovMat[15]

// 	 trk->SetIDTrack(trk);
	 
	 
      if ( v_trk_cov_d0_wrtPV != 0 ) {
		Float_t CovMat[15];
		CovMat[0]  = v_trk_cov_d0_wrtPV->at(j)/100.;				
		CovMat[1]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks, da in TopD3PDs nicht vorhanden
		CovMat[2]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[3]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[4]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[5]  = v_trk_cov_z0_wrtPV->at(j)/100.;
		CovMat[6]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[7]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[8]  = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[9]  = v_trk_cov_phi_wrtPV->at(j);
		CovMat[10] = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[11] = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[12] = v_trk_cov_theta_wrtPV->at(j);
		CovMat[13] = 0.;                                                             //CS: Nebendiagonalen mit 0 füllen für trks
		CovMat[14] = v_trk_cov_qoverp_wrtPV->at(j)*1000000.;                         //CS: Nebendiagonalen mit 0 füllen für trks
		trk->SetCovMatrix(CovMat);
    }
    
    
      AtlJet *jet = 0;
      TClonesArray *jets = fEvent->GetJets(AtlJet::kAntiKt4LCTopo);
      for ( Int_t i = 0; i < fEvent->GetN_Jets(AtlJet::kAntiKt4LCTopo); ++i ) {
	jet = (AtlJet*)jets->At(i);
	jet->AddTrack(trk, 0.6);
      }
      
    }
  
}



//____________________________________________________________________

void AtlEvtReaderD3PDCKM::BuildMCParticles() {
    //
    // Build list of MC particles
    //
    

//	from mounted_ssh/berlin1/kind/atlas/APlusPlus/libAtlasAnalysis/obsolete/

  for ( Int_t i = 0 ; i < v_mc_n; ++i) {
   
    HepMCParticle* mcParticles = fEvent->AddMCParticle(v_mc_pdgId->at(i),
						0., 0., 1, 1, 	
						v_mc_status->at(i), kTRUE,
						kFALSE, kFALSE, kFALSE,
						kFALSE, kFALSE, kFALSE);
				  //pdgId, ---  x, py, pz, E, ---  Int_t MCStatus, Bool_t IsGenerator, ---  Bool_t IsGenNonInteracting, Bool_t IsGenStable, Bool_t IsGenSimulStable, Bool_t IsGenInteracting, Bool_t IsConversion, Bool_t IsBremsstrahlung);
    mcParticles->SetPtEtaPhiE(v_mc_pt->at(i)/1000., v_mc_eta->at(i), v_mc_phi->at(i), v_mc_E->at(i)/1000.);
  }

  
  
  
  
  for ( Int_t i = 0 ; i < v_mc_n; ++i) {
    
    
    
    
    

      HepMCParticle* mcParticles = (HepMCParticle*)fEvent->GetMCParticles()->At(i);
      
      
// 	  cout << "# L1 daughters = " << v_mc_child_index->at(i).size() << endl;
      

      for (UInt_t j = 0; j < v_mc_child_index->at(i).size(); ++j) {
/*	
  //       HepMCParticle* mcDaughterL1 = (HepMCParticle*)fEvent->GetMCParticles()->At(daughterL1index);
	
	
	if ( abs( ((HepMCParticle*)fEvent->GetMCParticles()->At(i) )->GetPdgCode()) == 6 ){ 
	  Int_t daughterL1index = v_mc_child_index->at(i).at(j);     


	  cout << "\nName of L1 daughter from top: " << v_mc_pdgId->at(daughterL1index) << "\n"<< endl;
	  cout << "Number of L2 daughters = " << v_mc_child_index->at(daughterL1index).size() <<  endl;

	  for (UInt_t k = 0; k < v_mc_child_index->at(daughterL1index).size(); ++k) {
	    Int_t daughterL2index = v_mc_child_index->at(j).at(k);
	    cout << "Name of L2 daughter from top: " << v_mc_pdgId->at(daughterL2index) << endl;
	    cout << "Number of L3 daughters = " << v_mc_child_index->at(daughterL2index).size() << endl;
	  
	}
	
	
      }
	*/
      
      
      
      
      HepMCParticle* mcdaughter = (HepMCParticle*)fEvent->GetMCParticles()->At(v_mc_child_index->at(i).at(j));
      if ( mcdaughter != 0 ) mcParticles->AddDaughter(mcdaughter);
    }    
    
    
    
    
    
    
    
    
    
    
    
    
    for (UInt_t j = 0; j < v_mc_parent_index->at(i).size(); ++j) {
      HepMCParticle* mcmother = (HepMCParticle*)fEvent->GetMCParticles()->At(v_mc_parent_index->at(i).at(j));
      if ( mcmother != 0 ) mcParticles->AddMother(mcmother);
    }    
    
  }



  
}




//____________________________________________________________________

void AtlEvtReaderD3PDCKM::FillJPM_sbQuarkJet() {
    // 
    // Performing Jet Parton Matching
    // 

    fJPM_bQuarkJet->Clear();
    fJPM_sQuarkJet->Clear();
    fJPM_bQuark->Clear();
    fJPM_sQuark->Clear();  
    
    fJPM_NumbJetsMatching.clear();
    

    TClonesArray* jets_all = fEvent->GetJets(AtlJet::kAntiKt4LCTopo);    
    TList* MCt = fEvent->GetMCParticles("t");
    TList* MCtbar = fEvent->GetMCParticles("t_bar");

    for ( Int_t i = 0; i < MCtbar->GetEntries(); ++i){
      MCt->Add((HepMCParticle*)MCtbar->At(i));
    }    

    
    for ( Int_t i = 0; i < MCt->GetEntries(); ++i){
      
      top_quark = (HepMCParticle*)MCt->At(i);

      for (Int_t j = 0; j < top_quark->GetN_Daughters(); ++j){
	top_daughter = (HepMCParticle*)top_quark->GetDaughters()->At(j);
// 	cout << "EvtReaderD3PDCKM:: pdgId top daughter (#" << j << ") = " << top_daughter->GetPdgCode() << "  and name = " << top_daughter->GetPdgName() << "\n";
// 	cout << "EvtReaderD3PDCKM:: Daughter of top: pT = " << top_daughter->Pt() << " -- # Level2 daughters: " << top_daughter->GetN_Daughters() << "\n\n";
	bsd_quark = 0;
	
	  
	if ( abs(top_daughter->GetPdgCode()) == 5){ //looking for b/bbar-quarks	
// 	  cout << "EvtReaderD3PDCKM:: b quark found\n" << endl;
	  bsd_quark = pdgIdB;	
	}
	else if ( abs(top_daughter->GetPdgCode()) == 3){ //looking for s/sbar-quarks arising from tops
// 	  cout << "EvtReaderD3PDCKM:: s quark found\n" << endl;
	  bsd_quark = pdgIdS;
	}
	else if ( abs(top_daughter->GetPdgCode()) == 1){ //looking for d/dbar-quarks arising from tops
// 	  cout << "EvtReaderD3PDCKM:: d quark found\n" << endl;
	  bsd_quark = pdgIdD;	  
	}
	
	else continue; //continue with next daughter of top
	  

	bsdJet_index = 9999;
	dR_JetQuark = 0.4;
	dRTemp_JetQuark = 1.0;
	RatioPt_JetQuark = 0.1;
	numJetsMatching = 0;

	
//Jet parton matching main part
	for ( Int_t k = 0; k < fEvent->GetN_Jets(AtlJet::kAntiKt4LCTopo); ++k ){
	  dRTemp_JetQuark = top_daughter->DeltaR((AtlJet*)jets_all->At(k));
	  RatioPt_JetQuark = (((AtlJet*)jets_all->At(k))->Pt())/top_daughter->Pt();
// 	  cout << "EvtReaderD3PDCKM:: dR(Jet,quark)  = " << dRTemp_JetQuark << endl;
// 	  cout << "EvtReaderD3PDCKM:: RatioPt(Jet,q) = " << RatioPt_JetQuark << endl;
	  
	  
	  if ( dRTemp_JetQuark < 0.4 &&  RatioPt_JetQuark > 0.5 && RatioPt_JetQuark < 1.5){
	    numJetsMatching++;
	      
	    if ( dR_JetQuark > dRTemp_JetQuark ){
	      dR_JetQuark = dRTemp_JetQuark;
	      bsdJet_index = k;
	    }
	  }
	}

	fJPM_NumbJetsMatching.push_back(numJetsMatching);
  
  
  // kopiert aus bisheriger CKMAnalyses -> JPM dort:
  	
	if ( !numJetsMatching ){
// 	  cout << "EvtReaderD3PDCKM:: Oh no! No Jet is matching the Quark we are looking for!!!!!!!!!!!!!!!!!!\n No!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nNo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << endl;
	  continue;
	}
	
	bsdJet = (AtlJet*)jets_all->At(bsdJet_index);
// 	cout << "EvtReaderD3PDCKM:: ok, Jet matches with index " << bsdJet_index << endl;
// 	cout << "EvtReaderD3PDCKM:: quark | eta = " << top_daughter->Eta() << " , phi = " << top_daughter->Phi() << endl;
// 	cout << "EvtReaderD3PDCKM:: Jet     | eta = " << bsdJet->Eta() << " , phi = " << bsdJet->Phi() << endl;

  
	if (bsd_quark == 5){
	  fJPM_bQuarkJet->Add(bsdJet);
	  fJPM_bQuark->Add(top_daughter);
	}
	
	if (bsd_quark == 3){
	  fJPM_sQuarkJet->Add(bsdJet);
	  fJPM_sQuark->Add(top_daughter);
	}
	
      }
    }
    
    
}