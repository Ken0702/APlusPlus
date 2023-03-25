//____________________________________________________________________
//
// HEP vertex used in Monte Carlo simulations
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMCVertex
#include <HepMCVertex.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepMCVertex);
#endif

//____________________________________________________________________

HepMCVertex::HepMCVertex() {
    //
    // Default constructor
    //
    fDaughters = new TRefArray;
    fMother = new TRef;
}

//____________________________________________________________________

HepMCVertex::HepMCVertex(Int_t Id, Float_t x, Float_t y, Float_t z) :
    fId(Id) {
    //
    // Normal constructor
    //
    fPos.SetXYZ(x, y, z);
    fDaughters = new TRefArray;
    fMother = new TRef;
}

//____________________________________________________________________

HepMCVertex::~HepMCVertex() {
    //
    // Default destructor
    //
    delete fMother; fMother = 0;
    delete fDaughters; fDaughters = 0;
}

//____________________________________________________________________

void HepMCVertex::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fId = 0;
    fPos.SetXYZ(0, 0, 1);
    fDaughters->Clear();
    delete fDaughters; fDaughters = 0;
    delete fMother; fMother = 0;
}

//____________________________________________________________________

void HepMCVertex::AddDaughter(HepMCParticle *Daughter) {
    //
    // Add outgoing particle
    //
    fDaughters->Add((TObject*)Daughter);
}

//____________________________________________________________________

void HepMCVertex::Print(Option_t *option) const {
    //
    // Print object information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of vertices
    //
    TString opt = option;
    opt.ToLower();

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(12); cout << X();
    cout.width(12); cout << Y();
    cout.width(12); cout << Z();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepMCVertex::PrintHeader() {
    //
    // Print information header
    //
    cout << "------------------------------------" << endl
         << " Id  X           Y           Z      " << endl
         << "------------------------------------" << endl;
}

//____________________________________________________________________

void HepMCVertex::PrintFooter() {
    //
    // Print footer
    //
    cout << "-------------------------------------" << endl;
}
