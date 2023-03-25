//____________________________________________________________________
//
// HEP particle used in Monte Carlo simulations
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepMCParticle);
#endif

//____________________________________________________________________

HepMCParticle::HepMCParticle() {
    //
    // Default constructor
    //
    fMothers     = new TRefArray;
    fDaughters   = new TRefArray;
    fStartVertex = new TRef;
    fDecayVertex = new TRef;
}

//____________________________________________________________________

HepMCParticle::HepMCParticle(Int_t Id, Int_t PdgCode,
			     Float_t Px, Float_t Py,Float_t Pz,
			     Float_t E, Int_t MCStatus,
			     Bool_t IsGenerator,
			     Bool_t IsGenNonInteracting,
			     Bool_t IsGenStable,
			     Bool_t IsGenSimulStable,
			     Bool_t IsGenInteracting,
			     Bool_t IsConversion,
			     Bool_t IsBremsstrahlung ) :
    HepParticle(Id, Px, Py, Pz, E, PdgCode),
    fMCStatus(MCStatus),
    fIsGenerator(IsGenerator),
    fIsGenNonInteracting(IsGenNonInteracting),
    fIsGenStable(IsGenStable),
    fIsGenSimulStable(IsGenSimulStable),
    fIsGenInteracting(IsGenInteracting),
    fIsConversion(IsConversion),
    fIsBremsstrahlung(IsBremsstrahlung) {
    //
    // Normal constructor
    //
    fDaughters   = new TRefArray;
    fMothers     = new TRefArray;
    fStartVertex = new TRef;
    fDecayVertex = new TRef;
}

//____________________________________________________________________

HepMCParticle::~HepMCParticle() {
    //
    // Default destructor
    //
    delete fDaughters;   fDaughters = 0;
    delete fMothers;     fMothers = 0;
    delete fStartVertex; fStartVertex = 0;
    delete fDecayVertex; fDecayVertex = 0;
}

//____________________________________________________________________

void HepMCParticle::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepParticle::Clear(option);
    delete fMothers;  fMothers = 0;
    delete fDaughters; fDaughters = 0;
    delete fStartVertex; fStartVertex = 0;
    delete fDecayVertex; fDecayVertex = 0;
    fIsGenerator = 0;
    fIsGenNonInteracting = 0;
    fIsGenStable = 0;
    fIsGenSimulStable = 0;
    fIsGenInteracting = 0;
    fIsConversion = 0;
    fIsBremsstrahlung = 0;
}

//____________________________________________________________________

void HepMCParticle::Print(Option_t *option) {
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
	cout.width(12); cout << fP.Eta();
    } else {
	if ( fP.Pz() > 0. ) {
	    cout << "    infinity";
	} else {
	    cout << "   -infinity";
	}
    }
    cout.width(10); cout << fIsGenerator;
    cout.width(1); cout << fIsGenInteracting;
    cout.width(1); cout << fIsGenNonInteracting;
    cout.width(1); cout << fIsGenSimulStable;
    cout.width(1); cout << fIsGenStable;
    cout << endl;
    
    // Footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepMCParticle::PrintHeader() {
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
    cout << "---------------------------------------------------------------------------------------------------------" << endl
         << " Id     Type            E           P           Pt         Theta       Phi          Eta          GINUS   " << endl
         << "---------------------------------------------------------------------------------------------------------" << endl; 
}

//____________________________________________________________________

void HepMCParticle::PrintFooter() {
    //
    // Print footer
    //
    cout << "---------------------------------------------------------------------------------------------------------" << endl;
}

//____________________________________________________________________

Bool_t HepMCParticle::IsGoodMother(){
    //
    // True if particle does not give birth to a particle of the same kind
    // (and has a daughter)
    //
    
    if (!fDaughters->GetEntries()) {return kFALSE;}

    HepParticle *Daughter;
    Daughter=(HepParticle*)fDaughters->At(0);
    
    if ( this->GetPdgCode() == Daughter->GetPdgCode() ) {return kFALSE;}    
    return kTRUE;
}

//____________________________________________________________________


Double_t HepMCParticle::GetBremsLoss() {
    //
    // return the lost energy of the particle due to Bremsstrahlung.
    // that means: find all it's mothers of the same kind (e.g. electrons)
    // and find corresponding photons. sum the energy of these photons.
    //

    Double_t Bremsstrahlung=0;
    HepMCParticle *Mother;
    HepMCParticle *PhotonCandidate;

    if ( !fMothers->GetEntries() ) { return 0.;}
    
    Mother=(HepMCParticle*)fMothers->At(0);
    
    while ( Mother->GetPdgCode() == fPdgCode ){

	for ( Int_t i=1; i<Mother->GetN_Daughters() ; i++ ){
	    PhotonCandidate=(HepMCParticle*)Mother->GetDaughters()->At(i);
	    if ( PhotonCandidate->IsPhoton() ){ Bremsstrahlung+=PhotonCandidate->E(); }
	}

	if ( Mother->GetN_Mothers() ) { Mother=Mother->GetMother(); }
	else { return Bremsstrahlung; }
	
    }
    
    return Bremsstrahlung;

}
