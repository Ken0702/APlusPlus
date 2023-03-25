//____________________________________________________________________
//
// Base jet class
// 
// Note for developers: Some often used values like Et, Eta etc. are
// stored as transient data members for fast access. Their computation
// from the persistent data members is either done in the normal ctor
// or in SetPtEtaPhiE(), or triggered by the fCompute flag in case of
// using the default ctor.
//
// !!! IMPORTANT: Do not access the transient data members directly, !!!
// !!! but use ALWAYS their corresponding access function like Pt(), !!!
// !!! Eta() etc. to ensure that the variables are computed properly.!!!
//
// !!! IMPORTANT: Do not use Et(), Pt(), Pmag(), Thetha(), Phi(),    !!!
// !!! Eta() in TTree::Draw() or TTree::Scan() since these functions !!!
// !!! make use of a cache which will NOT be updated when jumping    !!!
// !!! to the next event inside TTree::Draw() or TTree::Scan().      !!!
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepJet
#include <HepJet.h>
#endif
#include <HepParticle.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepJet);
#endif

//____________________________________________________________________

HepJet::HepJet() {
    //
    // Default constructor
    //
    // The fCompute flag triggers the computation of the transient
    // member variables the next time one of them is accessed. This
    // mechanism is neccessary since the values of the persistent data
    // members needed for the computation are filles AFTER the call to
    // this ctor
    //
    fCompute = kTRUE;
}

//____________________________________________________________________

HepJet::HepJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz) :
    fId(Id) {
    //
    // Normal constructor
    //
    fP.SetPxPyPzE(Px, Py, Pz, E);
    ComputeTransientVars();
}

//____________________________________________________________________

HepJet::~HepJet() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepJet::Clear(Option_t *option) {
    //
    // Clear object
    //
    fId = 0;
    fP.SetPxPyPzE(0, 0, 1, 0);

    // Re-set cache
    fCompute = kTRUE;
}

//____________________________________________________________________

Int_t HepJet::Compare(const TObject *obj) const {
    //
    // Compares transverse energy of this jet with another.
    // Necessary for sorting
    //
    Float_t Pt = fP.Pt();
    if ( Pt <  ((HepJet*)obj)->Pt() ) return -1;
    if ( Pt == ((HepJet*)obj)->Pt() ) return  0;
    if ( Pt >  ((HepJet*)obj)->Pt() ) return  1;
    return 0;
}

//____________________________________________________________________

Float_t HepJet::DeltaPhi(HepParticle* prt) const {
    //
    // Compute azimuth difference between this jet and the given
    // particle in the range from -pi to +pi
    //
    return fP.DeltaPhi(prt->P());
}

//____________________________________________________________________

Float_t HepJet::DeltaPhi(HepJet *jet) const {
    //
    // Compute azimuth difference between this and the given jet in
    // the range from -pi to +pi
    //
    return fP.DeltaPhi(jet->P());
}

//____________________________________________________________________

Float_t HepJet::DeltaEta(HepParticle *prt) {
    //
    // Compute (signed) pseudo-rapidity difference between this jet
    // and the given particle
    //
    return Eta() - prt->Eta();
}

//____________________________________________________________________

Float_t HepJet::DeltaEta(HepJet *jet) {
    //
    // Compute (signed) pseudo-rapidity difference between this and
    // the given jet
    //
    return Eta() - jet->Eta();
}

//____________________________________________________________________

Float_t HepJet::DeltaR(HepParticle *prt) {
    //
    // Compute distance in eta-phi plane between this jet and the
    // given particle
    //
    Float_t dEta = Eta() - prt->Eta();
    Float_t dPhi = fP.DeltaPhi(prt->P());
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

Float_t HepJet::DeltaR(HepJet *jet) {
    //
    // Compute distance in eta-phi plane between this and the given
    // jet
    //
    Float_t dEta = Eta() - jet->Eta();
    Float_t dPhi = fP.DeltaPhi(jet->P());
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

Float_t HepJet::DeltaPtFrac(HepParticle *prt) {
    //
    // DeltaPtFrac = (Pt_1 - Pt_2)/Pt_1
    // 
    // Pt difference, unsigned, for better checking systematic errors, 
    // normalized to the first entry. The second term is a HepParticle  
    // object.
    //
  Float_t pt = Pt();
  if( pt>0. ){  return ( (pt - prt->Pt() ) / pt ); }
  return 10e10;
}

//____________________________________________________________________

Float_t HepJet::DeltaPtFrac(HepJet *jet) {
    //
    // DeltaPtFrac = (Pt_1 - Pt_2)/Pt_1
    // 
    // Pt difference, unsigned, for better checking systematic errors, 
    // normalized to the first entry. The second term is a HepJet  
    // object.  
    // 
  Float_t pt = Pt();
  if( pt>0. ){  return ( (pt - jet->Pt() ) / pt ); }
  return 10e10;
}

//____________________________________________________________________

Float_t HepJet::DeltaEtFrac(HepParticle *prt) {
    //
    // DeltaEtFrac = (Et_1 - Et_2)/Et_1
    // 
    // Et difference, unsigned, for better checking systematic errors, 
    // normalized to the first entry. The second term is a HepParticle  
    // object.
    //
  Float_t et = Et();
  if( et>0. ){  return ( (et - prt->Et() ) / et ); }
  return 10e10;
}

//____________________________________________________________________

Float_t HepJet::DeltaEtFrac(HepJet *jet) {
    //
    // DeltaEtFrac = (Et_1 - Et_2)/Et_1
    // 
    // Et difference, unsigned, for better checking systematic errors, 
    // normalized to the first entry. The second term is a HepJet  
    // object.  
    // 
  Float_t et = Et();
  if( et>0. ){  return ( (et - jet->Et() ) / et ); }
  return 10e10;
}

//____________________________________________________________________

Float_t HepJet::InvDeltaPtFrac(HepParticle *prt) {
    //
    // An "Inverse" DeltaPtFrac is needed, due to Multiple Scattering in 
    // the Inner Detector
    // InvDeltaPtFrac = ( 1/Pt_1 - 1/Pt_2)*Pt_1
    //
    // Unsigned again, for better checking systematic errors 
    // I dont know yet if I can write the same algorithm for the jets
    //
  Float_t pt1 = Pt();
  Float_t pt2 = prt->Pt();
  if( (pt1>0.) && (pt2>0.) ) {
    return ( ( 1/pt1 - 1/pt2 ) * pt1 );
  }
  return 10e10; 
}

//____________________________________________________________________

Double_t HepJet::PtRel(HepParticle* prt) {
    //
    // Computes the relative traverse momentum between the given
    // particle and this jet
    //
    //   Pt_rel = |P_prt x P_jet|
    //
    TVector3 jetdir = P3();
    return prt->P().Perp(jetdir);
} 

//____________________________________________________________________

void HepJet::ComputeTransientVars() {
    //
    // Compute transient variables like Et, Theta, Eta etc. for fast
    // access
    //
    fCompute = kFALSE;
    
    fEt = fP.Et();
    fPt = fP.Perp();
    fTheta = fP.Theta();
    fCosTheta = fP.CosTheta();
    fPhi = fP.Phi();
    // Suppress Root warning when computing eta
    if ( fPt != 0. ) {
	fEta = fP.Eta();
    } else {
	fEta = ( Pz() > 0. ) ? 10e10 : -10e10;
    }
}

//____________________________________________________________________

void HepJet::Print(Option_t *option) {
    //
    // Print jet information
    //
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of jets
    //
    TString opt = option;
    opt.ToLower();

    // =======
    // General
    // =======

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
    
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(12); cout << Et();
    cout.width(12); cout << M();
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(12); cout << Phi()*180/TMath::Pi();
    cout.width(12); cout << Eta();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepJet::PrintHeader() {
    //
    // Print information header
    //
    cout
	<<"----------------------------------------------------------------"
	<< endl
	<< " Id         Et          M         Theta         Phi         Eta"
	<< endl
	<< "---------------------------------------------------------------"
	<< endl;
}

//____________________________________________________________________

void HepJet::PrintFooter() {
    //
    // Print footer
    //
    cout
	<< "---------------------------------------------------------------"
	<< endl;
}
