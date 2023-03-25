//____________________________________________________________________
//
// Abstract HEP particle class
// 
// Note for developers: Some often used values like Pt, Eta etc. are
// stored as transient data members for fast access. Their computation
// from the persistent data members is either done in the normal ctor
// or in SetPtEtaPhiE(), or triggered by the fCompute flag in case of
// using the default ctor.
//
// Also included here is a function allowing to print TLorentzVector
// Cartesian/(pt,eta,phi) coordinates.
//
// !!! IMPORTANT: Do not acces the transient data members directly,  !!!
// !!! but use ALWAYS their corresponding access function like Pt(), !!!
// !!! Eta()etc. to ensure that the variables are computed properly. !!!
//
// !!! IMPORTANT: Do not use Pt(), Pmag(), Thethe(), Phi(), Eta() in !!!
// !!! TTree::Draw() or TTree::Scan() since these functions make use !!!
// !!! of a cache which will NOT be updated when jumping to the next !!!
// !!! event inside TTree::Draw() or TTree::Scan().                  !!!
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif
#include <HepJet.h>
#include <TSystem.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepParticle);
#endif

HepDatabasePDG *HepParticle::fDbasePDG = HepDatabasePDG::Instance();

//____________________________________________________________________

HepParticle::HepParticle() {
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

HepParticle::HepParticle(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Int_t PdgCode) : fId(Id),
						     fPdgCode(PdgCode) {
    //
    // Normal constructor
    //
    fP.SetPxPyPzE(Px, Py, Pz, E);
    ComputeTransientVars();
}

//____________________________________________________________________

HepParticle::~HepParticle() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepParticle::Clear(Option_t *option) {
    //
    // Clear object
    //	
    fP.SetPxPyPzE(0, 0, 1., 0); // unit-vector in Pz direction
    // (instead of zero-vector). This is to avoid difficulties
    // when using polar coordinates
    fId      = 0;
    fPdgCode = 0;
    // Re-set cache
    fCompute = kTRUE;
}



//____________________________________________________________________

Double_t HepParticle::Mass(Option_t *option) const {
    //
    // Particle mass
    //
    // Options available:
    //    "PDG" = mass as given in the PDG dbase (default)
    //    "REC" = 'reconstructed' invariant mass stored in
    //             the 4-momentum vector
    //
    TString opt = option;
    opt.ToUpper();
    if ( opt.Contains("PDG") ) {
	return fDbasePDG->GetParticle(GetPdgCode())->Mass();
    } else {
	return fP.M();
    }
}

//____________________________________________________________________

Bool_t HepParticle::operator==(HepParticle Partner) {
    //
    // Compare Particle with Partner w.r.t. PdgCode and 4-Momentum
    // return kTRUE in case of Matching
    //
    // Comparison is made between Objects, not Pointers
    // 
    
    if (fPdgCode==Partner.GetPdgCode()
	&& TMath::Abs((fP.Px()-Partner.Px())/fP.Px())<1.e-05
	&& TMath::Abs((fP.Py()-Partner.Py())/fP.Py())<1.e-05
	&& TMath::Abs((fP.Pz()-Partner.Pz())/fP.Pz())<1.e-05
	&& TMath::Abs((fP.E()-Partner.E())/fP.E())<1.e-05) {return kTRUE;}
    return kFALSE;
}

//____________________________________________________________________

Int_t HepParticle::Compare(const TObject *obj) const {
    //
    // Compares transverse momentum of this particles with another.
    // Necessary for sorting
    //
    Float_t Pt = fP.Pt();
    if ( Pt <  ((HepParticle*)obj)->Pt() ) return -1;
    if ( Pt == ((HepParticle*)obj)->Pt() ) return  0;
    if ( Pt >  ((HepParticle*)obj)->Pt() ) return  1;
    return 0;
}

//____________________________________________________________________

Bool_t HepParticle::HasInvalidPdgCode() const {
    //
    // Test validity of PDG code
    // Compounds like eg. deuteron, tritium or alphas etc. are
    // unknown to the PDG database
    //
    TString prt_name(GetPdgName());
    if ( prt_name.Contains("invalid") ) return kTRUE;
    return kFALSE;
}

//____________________________________________________________________

const char* HepParticle::GetPdgName() const {
    //
    // Return particle's name
    //
    TParticlePDG *prt = fDbasePDG->GetParticle(fPdgCode);
    return ( prt != 0 ) ? prt->GetName() : "invalid";
}

//____________________________________________________________________

Float_t HepParticle::Charge() const {
    //
    // Get charge in units of e
    // (Note that in the PDG dbase the charges are stored in 1/3 of e)
    //
    return (Float_t)fDbasePDG->GetParticle(GetPdgCode())->Charge()/3.;
}

//____________________________________________________________________

Float_t HepParticle::DeltaPhi(HepParticle* prt) const {
    //
    // Compute azimuth difference between this and the given particle
    // in the range from -pi to +pi
    //
    return fP.DeltaPhi(prt->P());
}

//____________________________________________________________________

Float_t HepParticle::DeltaPhi(HepJet *jet) const {
    //
    // Compute azimuth difference between this particle and the given
    // jet in the range from -pi to +pi
    //
    return fP.DeltaPhi(jet->P());
}

//____________________________________________________________________

Float_t HepParticle::DeltaEta(HepParticle *prt) {
    //
    // Compute (signed) pseudo-rapidity difference between this and
    // the given particle
    //
    return Eta() - prt->Eta();
}

//____________________________________________________________________

Float_t HepParticle::DeltaEta(HepJet *jet) {
    //
    // Compute (signed) pseudo-rapidity difference between this
    // particle and the given jet
    //
    return Eta() - jet->Eta();
}

//____________________________________________________________________

Float_t HepParticle::DeltaR(HepParticle *prt) {
    //
    // Compute distance in eta-phi plane between this and the given
    // particle
    //
    Float_t dEta = Eta() - prt->Eta();
    Float_t dPhi = fP.DeltaPhi(prt->P());
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

Float_t HepParticle::DeltaR(HepJet *jet) {
    //
    // Compute distance in eta-phi plane between this particle and the
    // given jet
    //
    Float_t dEta = Eta() - jet->Eta();
    Float_t dPhi = fP.DeltaPhi(jet->P());
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//____________________________________________________________________

Float_t HepParticle::Mperp2() const {
    //
    // Mt^2 = Et^2 - Pt^2
    //
    // Use this function for the Jacobian-peak method
    //
    Float_t Et = fP.Et();
    Float_t Et2 = Et*Et;
    Float_t Pt2 = fP.Perp2();
    return Et2-Pt2;
}

//____________________________________________________________________

Float_t HepParticle::DeltaPtFrac(HepParticle *prt) {
    //
    // DeltaPtFrac = (Pt_1 - Pt_2)Pt_1
    //
    // Unsigned, for better checking systematic errors 
    // 
    Float_t pt = Pt();
    if( pt > 0. ){  return (pt - prt->Pt()) / pt; }
    return 10e10;
}

//____________________________________________________________________

Float_t HepParticle::DeltaPtFrac(HepJet *jet) {
  Float_t pt = Pt();
  if( pt > 0. ){  return (pt - jet->Pt()) / pt; }
  return 10e10;
}

//____________________________________________________________________

Float_t HepParticle::InvDeltaPtFrac(HepParticle *prt) {
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
    if( (pt1 > 0.) && (pt2 > 0.) ) {
	return ( 1/pt1 - 1/pt2 ) * pt1;
    }
    return 10e10; 
}

//____________________________________________________________________

void HepParticle::ComputeTransientVars() {
    //
    // Compute transient variables like Pt, Theta, Eta etc. for fast
    // access
    //
    fCompute = kFALSE;
    
    fPmag = fP.P();
    fPt = fP.Perp();
    fTheta = fP.Theta();
    fCosTheta = fP.CosTheta();
    fPhi = fP.Phi();
    // Suppress Root warning when computing eta
    if ( fCosTheta*fCosTheta < 1. ) {
	fEta = fP.Eta();
    } else {
	fEta = ( Pz() > 0. ) ? 10e10 : -10e10;
    }
}

//____________________________________________________________________

void HepParticle::Print(Option_t *option) {
    //
    // Print object information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of particles
    //
    TString opt = option;
    opt.ToLower();

    // Header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Main
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.width(4); cout << fId;
    cout.width(12);  cout << GetPdgName();
    cout.precision(3);
    cout.width(12); cout << fP.E();
    cout.width(12); cout << fP.P();
    cout.width(12); cout << fP.Pt();
    cout.width(12); cout << fP.Theta()*180/TMath::Pi();
    cout.width(12); cout << fP.Phi()*180/TMath::Pi();
    if ( fP.Pt() != 0. ) {
	cout.width(11); cout << fP.Eta();
    } else {
	if ( fP.Pz() > 0. ) {
	    cout << "    infinity";
	} else {
	    cout << "   -infinity";
	}
    }
    cout.width(12); cout << Mass("rec");
    cout << endl;
    
    // Footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepParticle::PrintHeader() {
    //
    // Print information header
    //
    //
    // GINUS denotes (in this order):
    //
    // IsGenerator()
    // IsGenInteracting()
    // IsGenNonInteracting()
    // IsGenSimulStable()
    // IsGenStable()
    //
    cout << "---------------------------------------------------------------------------------------------------" << endl
         << " Id     Type            E           P           Pt         Theta       Phi          Eta       M_inv" << endl
         << "---------------------------------------------------------------------------------------------------" << endl; 
}

//____________________________________________________________________

void HepParticle::PrintFooter() {
    //
    // Print footer
    //
    cout << "---------------------------------------------------------------------------------------------------" << endl;
}

//____________________________________________________________________

void HepParticle::GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const {
    //
    // Dummy for returning the covariance matrix in some derived
    // classes (reconstructed electrons and muons, ...)
    //
    Error("GetCovMatEtaPhi",
	  "Not yet implemented for this particle type. Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void HepParticle::PrintLorentzVector(TLorentzVector vec) {
    //
    // Print the Cartesian and (pt, eta, phi) coordinates of a
    // TLorentVector. Needed, since this functionality is not
    // provided by RooT. Guards Eta() from Pt = 0.
    //
    cout << endl;
    Printf("(x,y,z,t)=(%f,%f,%f,%f)\n(Pt,eta,theta,phi)=(%f,%f,%f,%f)\n(Pt,eta,theta_deg,phi_deg)=(%f,%f,%f,%f)",
	   vec.Px(),vec.Py(),vec.Pz(),vec.E(),
	   vec.Perp(),vec.Eta(),vec.Theta(),vec.Phi(),
	   vec.Perp(),vec.Eta(),vec.Theta()*180./TMath::Pi(),
	   vec.Phi()*180./TMath::Pi());
    cout << endl;
}
