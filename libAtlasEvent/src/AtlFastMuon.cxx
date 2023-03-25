//____________________________________________________________________
//
// Atlas muon class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastMuon
#include <AtlFastMuon.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlFastMuon);
#endif

//____________________________________________________________________

AtlFastMuon::AtlFastMuon() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlFastMuon::AtlFastMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, Bool_t IsMuPlus) :
    HepMuon(Id, Px, Py, Pz, E, IsMuPlus) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlFastMuon::~AtlFastMuon() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlFastMuon::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepMuon::Clear(option);
}

//_____________________________________________________________

void AtlFastMuon::Print(Option_t *option) {
    //
    // Print muon information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of muons
    //
    TString opt = option;
    opt.ToLower();
    
    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
    
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(3);  cout << (( IsPositive() ) ? "+" : "-");
    cout.width(12); cout << Et();
    cout.precision(6);
    cout.width(12); cout << M("REC");
    cout.precision(3);
    cout.width(12);
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(12); cout << Phi()*180/TMath::Pi();
    cout.width(12); cout << Eta();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlFastMuon::PrintHeader() {
    //
    // Print information header
    //
    cout << "-------------------------------------------------------------------"
	 << endl
	 << " Id  Chg     Et         Minv         Theta         Phi         Eta "
	 << endl
	 << "-------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlFastMuon::PrintFooter() {
    //
    // Print footer
    //
    cout << "-------------------------------------------------------------------"
	 << endl;
}

