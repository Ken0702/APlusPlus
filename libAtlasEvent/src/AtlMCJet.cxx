//____________________________________________________________________
//
// Atlas MC truth jet
//
// The jet is formed by taking all truth particles - with the
// exception of neutrinos, muons and non interacting particles -
// within a cone around the reconstructed jet.
// BEGIN_HTML
// For details see the <a href="https://twiki.cern.ch/twiki/bin/view/Atlas/JetS#Truth_Jets">Atlas Wiki</a>.
// END_HTML
// 
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMCJet
#include <AtlMCJet.h>
#endif
#ifndef HEP_Jet
#include <HepJet.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMCJet);
#endif

//____________________________________________________________________

AtlMCJet::AtlMCJet() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlMCJet::AtlMCJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz) :
    HepJet(Id, E, Px, Py, Pz) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlMCJet::~AtlMCJet() {
    //
    // Default destructor
    //
}

//_____________________________________________________________

void AtlMCJet::Print(Option_t *option) {
    //
    // Print jet information
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of jets
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

void AtlMCJet::PrintHeader() {
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

void AtlMCJet::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------"
	 << endl;
}
