//____________________________________________________________________
//
// Atlas Top Pair Item
//

// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Felix Thomas
//
//  This class records the top decays that belong to a given reconstructed
//  top pair.
//
//
//
//

#ifndef ATLAS_AtlTopPair
#include <AtlTopPair.h>
#endif
#include <iostream>
#include <TMath.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTopPair);
#endif


//____________________________________________________________________

AtlTopPair::AtlTopPair() {
    //
    // Default constructor
    //

    fTop1 = new TRef;   
    fTop2 = new TRef;   
    fId       = 0;
    fChi2     = 0;
    fNdof     = 0;     
    fChi2Prob = 0; 
    fType     = AtlTopPair::kInvalid;
    
    InitPulls();
    
}

//____________________________________________________________________

AtlTopPair::AtlTopPair(Int_t Id, HepTopDecay* top1, HepTopDecay* top2, 
                                  Double_t chi2, Int_t ndof, AtlTopPair::EType type){
    //
    // Custom constructor
    //

  fTop1 = new TRef;
  fTop2 = new TRef;
  *fTop1 = (TObject*)top1;
  *fTop2 = (TObject*)top2;
  fId   = Id;
  fChi2 = chi2;
  fNdof = ndof;
  fChi2Prob =  TMath::Prob(chi2, ndof);  
  fType = type;

  InitPulls();

}

//____________________________________________________________________

void AtlTopPair::InitPulls(){
  
  fChargedLeptonPull.ResizeTo(3,1);
  fNeutrinoPull.ResizeTo(3,1);
  fLepBJetPull.ResizeTo(3,1);
  fHadJet1Pull.ResizeTo(3,1);
  fHadJet2Pull.ResizeTo(3,1);
  fHadBJetPull.ResizeTo(3,1);
    
  fChargedLeptonPull.Zero();
  fNeutrinoPull.Zero();
  fLepBJetPull.Zero();
  fHadJet1Pull.Zero();
  fHadJet2Pull.Zero();
  fHadBJetPull.Zero();
  
}

//____________________________________________________________________

AtlTopPair::~AtlTopPair() {
    //
    // Default destructor
    //

  delete fTop1; fTop1 = 0;
  delete fTop2; fTop2 = 0;  

}

//____________________________________________________________________

void AtlTopPair::Clear(Option_t *option) {
    //
    // Clear this object
    //


    delete fTop1; fTop1 = 0;
    delete fTop2; fTop2 = 0;  
    fId       = 0;
    fChi2     = 0;
    fNdof     = 0;     
    fChi2Prob = 0; 
    fType     = kInvalid;

}


//____________________________________________________________________

HepTopDecay* AtlTopPair::GetLeptonicTop() const {
    //
    // retrieving the leptonic top
    //
  
  if (((HepTopDecay*)fTop1->GetObject())->IsSemiLeptonicDecay() == kTRUE) return (HepTopDecay*)fTop1->GetObject();
  if (((HepTopDecay*)fTop2->GetObject())->IsSemiLeptonicDecay() == kTRUE) return (HepTopDecay*)fTop2->GetObject();  

  return 0;
}

//____________________________________________________________________

HepTopDecay* AtlTopPair::GetHadronicTop() const {
    //
    // retrieving the hadronic top
    //
  
  if (((HepTopDecay*)fTop1->GetObject())->IsHadronicDecay() == kTRUE) return (HepTopDecay*)fTop1->GetObject();
  if (((HepTopDecay*)fTop2->GetObject())->IsHadronicDecay() == kTRUE) return (HepTopDecay*)fTop2->GetObject();  

  return 0;
}
//____________________________________________________________________

void AtlTopPair::SetPullMatrices(TMatrixD leptonpull, TMatrixD neutrinopull, TMatrixD lepbjetpull,
                                 TMatrixD hadjet1pull, TMatrixD hadjet2pull, TMatrixD hadbjetpull){
                                     
  fChargedLeptonPull = leptonpull;
  fNeutrinoPull = neutrinopull;
  fLepBJetPull = lepbjetpull;
  fHadJet1Pull = hadjet1pull;
  fHadJet2Pull = hadjet2pull;
  fHadBJetPull = hadbjetpull;                            
  
}


//____________________________________________________________________

void AtlTopPair::Print(Option_t *option) const {
    //
    // Print object information
    //

    ((HepTopDecay*)fTop1->GetObject())->Print();
    ((HepTopDecay*)fTop2->GetObject())->Print();

}
