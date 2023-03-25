//____________________________________________________________________
//
// Basic photon class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepPhoton
#include <HepPhoton.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepPhoton);
#endif

//____________________________________________________________________

HepPhoton::HepPhoton() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

HepPhoton::HepPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E) :
    HepParticle(Id, Px, Py, Pz, E, 22) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepPhoton::~HepPhoton() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepPhoton::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepParticle::Clear(option);
}

//____________________________________________________________________

void HepPhoton::Print(Option_t *option) {
    //
    // Print photon information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of photons
    //
    TString opt = option;
    opt.ToLower();
    
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

void HepPhoton::PrintHeader() {
    //
    // Print information header
    //
    cout << "----------------------------------------------------------------"
	 << endl
	 << " Id         Et          M         Theta         Phi         Eta "
	 << endl
	 << "----------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void HepPhoton::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------"
	 << endl;
}
