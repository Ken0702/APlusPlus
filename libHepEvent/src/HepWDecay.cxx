//____________________________________________________________________
//
// Abstract W boson decay class
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef HEP_HepWDecay
#include <HepWDecay.h>
#endif
#include <TClass.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepWDecay);
#endif

//____________________________________________________________________

HepWDecay::HepWDecay() {
    //
    // Default constructor
    //
    fDaughter1 = new TRef;
    fDaughter2 = new TRef;
    fTruthW    = new TRef;
}

//____________________________________________________________________

HepWDecay::HepWDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E, TObject *Daughter1, TObject *Daughter2,
		     ProductionMode mode) :
    HepDecayParticle(Id, Px, Py, Pz, E, 24) {
    //
    // Normal constructor
    //

    // Set charge for leptonic decays - otherwise PDG code is set to W+
    if ( TClass(((TObject*)Daughter1)->ClassName()).InheritsFrom("HepParticle") ) {
	fPdgCode = ( ((HepParticle*)Daughter1)->IsPositive() ) ? 24 : -24;
    }

    // Set links to the outgoing particles
    fDaughter1 = new TRef; *fDaughter1 = Daughter1;
    fDaughter2 = new TRef; *fDaughter2 = Daughter2;
    fTruthW    = new TRef;
    fMode      = mode;
}

//____________________________________________________________________

HepWDecay::~HepWDecay() {
    //
    // Default destructor
    //
    delete fDaughter1;	fDaughter1 = 0;
    delete fDaughter2;	fDaughter2 = 0;
    delete fTruthW;	fTruthW = 0;
}

//____________________________________________________________________

void HepWDecay::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
    delete fDaughter1; fDaughter1 = 0;
    delete fDaughter2; fDaughter2 = 0;
    delete fTruthW;    fTruthW = 0;
}

//____________________________________________________________________

Bool_t HepWDecay::IsHadronicDecay() const {
    //
    // W has decayed hadronically ?
    //
    if ( TClass(((TObject*)GetDaughter1())
		->ClassName()).InheritsFrom("HepJet") )
	return kTRUE;
    return kFALSE;
}

