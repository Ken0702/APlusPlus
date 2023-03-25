//____________________________________________________________________
//
// Atlas photon class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastPhoton
#include <AtlFastPhoton.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlFastPhoton);
#endif

//____________________________________________________________________

AtlFastPhoton::AtlFastPhoton() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlFastPhoton::AtlFastPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E) :
    HepPhoton(Id, Px, Py, Pz, E) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlFastPhoton::~AtlFastPhoton() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlFastPhoton::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepPhoton::Clear(option);
}

//_____________________________________________________________

void AtlFastPhoton::Print(Option_t *option) {
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
    cout.precision(6);
    cout.width(12); cout << M("RECO");
    cout.precision(3);
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(12); cout << Phi()*180/TMath::Pi();
    cout.width(12); cout << Eta();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlFastPhoton::PrintHeader() {
    //
    // Print information header
    //
    cout << "-------------------------------------------------------------------"
	 << endl
	 << " Id         Et          Minv         Theta         Phi         Eta "
	 << endl
	 << "-------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlFastPhoton::PrintFooter() {
    //
    // Print footer
    //
    cout << "-------------------------------------------------------------------"
	 << endl;
}

