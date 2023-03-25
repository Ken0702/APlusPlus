//____________________________________________________________________
//
// Atlas photon class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlPhoton
#include <AtlPhoton.h>
#endif
#include <TSystem.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlPhoton);
#endif

//____________________________________________________________________

AtlPhoton::AtlPhoton() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlPhoton::AtlPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E, 
		     Float_t EMWeight, Float_t BkgWeight,
		     UInt_t OQFlag,
		     UInt_t IsEMBitField, EIsEM IsEM, EAuthor Author,
		     TLorentzVector PCluster) :
    HepPhoton(Id, Px, Py, Pz, E),
    AtlEMShower(Author, 
		IsEMBitField, IsEM, EMWeight, BkgWeight, OQFlag,
		PCluster) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlPhoton::~AtlPhoton() {
    //
    // Default destructor
    //

}

//____________________________________________________________________

void AtlPhoton::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepPhoton::Clear(option);
    AtlEMShower::Clear(option);
    AtlTriggerMatch::Clear(option);
}

//_____________________________________________________________

UInt_t AtlPhoton::IsGoodOQ() {
    //
    // OQ decision
    //
    // 0: is good OQ
    // 1: is bad OQ
    //
    return ( (fOQFlag & 34214) == 0 ) ? 0 : 1;
}

//_____________________________________________________________

void AtlPhoton::Print(Option_t *option) {
    //
    // Print photon information
    //
    // Note that in the standard view only the main authors and
    // quality flags are being displayed.
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
    cout.width(10); cout << Pt();
    cout.width(9); cout << E();
    cout.precision(6);
    cout.width(11); cout << M("REC");
    cout.precision(3);
    cout.width(9); cout << Theta()*180/TMath::Pi();
    cout.width(9); cout << Phi()*180/TMath::Pi();
    cout.width(7); cout << Eta();
    cout.width(9); cout << GetEtCone20();
    cout << "   ";
    if ( HasInvalidIsEM() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    if ( IsLoose() ) {
	cout << "l";
    } else {
	cout << "-";
    }
    if ( IsTight() ) {
	cout << "t";
    } else {
	cout << "-";
    }
    cout << "    ";
    if ( HasInvalidAuthor() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    if ( IsPhoton() ) {
	cout << "p";
    } else {
	cout << "-";
    }
    if ( IsConvPhoton() ) {
	cout << "c";
    } else {
	cout << "-";
    }
    cout << "        " << IsGoodOQ();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlPhoton::PrintHeader() {
    //
    // Print information header
    //
    cout << "-------------------------------------------------------------------------------------------"
	 << endl
	 << " Id   Pt (GeV) E (GeV)  Minv (GeV)    Theta      Phi    Eta EtCone20  IsEM  Author   OQ    "
	 << endl
	 << "-------------------------------------------------------------------------------------------"
	 << endl;

}

//____________________________________________________________________

void AtlPhoton::PrintFooter() {
    //
    // Print footer
    //
    cout << "   i=invalid l=loose t=tight p=photon c=conversion" << endl
       	 << "---------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________
