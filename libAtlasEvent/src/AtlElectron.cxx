//____________________________________________________________________
//
// Atlas electron class
// 
// Note that the electron container contains different kind of
// electrons reconstructed with different algorithms
// (authors). Information about the algorithm used is stored in the
// fAuthor datamember and can be tested with the member functions
// IsHighPtElectron(), IsSoftElectron(), IsForwardElectron(),
// IsPhotoConversion(). Details about the reconstruction can be found
// on the Atlas Twiki pages at
// BEGIN_HTML
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ElectronReconstruction
// END_HTML
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlElectron
#include <AtlElectron.h>
#endif
#include <TSystem.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlElectron);
#endif

//____________________________________________________________________

AtlElectron::AtlElectron() {
    //
    // Default constructor
    //
    fTrackEMRefit = new HepTrackHelix;
    fIDTrack = new TRef;
}

//____________________________________________________________________

AtlElectron::AtlElectron(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Bool_t IsPositron, 
                         Float_t EMWeight, Float_t BkgWeight,
			 UInt_t OQFlag, 
			 UInt_t IsEMBitField, EIsEM IsEM, EAuthor Author, 
			 TLorentzVector PCluster) :
    HepElectron(Id, Px, Py, Pz, E, IsPositron),
    AtlEMShower(Author, 
		IsEMBitField, IsEM, EMWeight, BkgWeight, OQFlag,
		PCluster) {
    //
    // Normal constructor
    //
    fTrackEMRefit = new HepTrackHelix;
    fIDTrack = new TRef;
}

//____________________________________________________________________

AtlElectron::~AtlElectron() {
    //
    // Default destructor
    //
    delete fTrackEMRefit;
    delete fIDTrack; fIDTrack = 0;
}


//____________________________________________________________________

void AtlElectron::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepElectron::Clear(option);
    AtlEMShower::Clear(option);
    AtlTriggerMatch::Clear(option);    
    delete fTrackEMRefit; fTrackEMRefit = 0;
    delete fIDTrack; fIDTrack = 0;
    fPtCone30  = 0.;
    fPCluster.SetPxPyPzE(0, 0, 1., 0); // unit-vector in Pz direction
    fMCTruthClassifier.Clear(option);
}

//_____________________________________________________________

UInt_t AtlElectron::IsGoodOQ() {
    //
    // OQ decision
    //
    // 0: is good OQ
    // 1: is bad OQ
    //
    return ( (fOQFlag & 1446) == 0 ) ? 0 : 1;
}

//_____________________________________________________________

void AtlElectron::Print(Option_t *option) {
    //
    // Print electron information
    //
    // Note that in the standard view only the main authors and
    // quality flags are being displayed.
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of electrons
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
    cout.width(10); cout << Pt();
    cout.width(9); cout << E();
    cout.precision(6);
    cout.width(11); cout << M("REC");
    cout.precision(3);
    cout.width(9); cout << Theta()*180/TMath::Pi();
    cout.width(9); cout << Phi()*180/TMath::Pi();
    cout.width(7); cout << Eta();
    cout.width(9); cout << GetEtCone20();
    cout << "  ";
    if ( HasInvalidIsEM() ) {
	cout << "e";
    } else {
	cout << "-";
    }
    if ( IsLoose() ) {
	cout << "l";
    } else {
	cout << "-";
    }
    if ( IsMedium() || IsMediumIso() ) {
	cout << "m";
    } else {
	cout << "-";
    }
    if ( IsTight() || IsTightIso() ) {
	cout << "t";
    } else {
	cout << "-";
    }
    if ( IsMediumIso() || IsTightIso() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    cout << "  ";
    if ( HasInvalidAuthor() ) {
	cout << "i";
    } else {
	cout << "-";
    }
    if ( IsHighPtElectron() ) {
	cout << "h";
    } else {
	cout << "-";
    }
    if ( IsSoftElectron() ) {
	cout << "s";
    } else {
	cout << "-";
    }
    if ( IsForwardElectron() ) {
	cout << "f";
    } else {
	cout << "-";
    }
    cout << "        " << IsGoodOQ();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlElectron::PrintHeader() {
    //
    // Print information header
    //
    cout << "---------------------------------------------------------------------------------------------"
	 << endl
	 << " Id  Chg Pt (GeV) E (GeV)  Minv (GeV)    Theta      Phi    Eta EtCone20  IsEM  Author   OQ   "
	 << endl
	 << "---------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlElectron::PrintFooter() {
    //
    // Print footer
    //
    cout << "   e=invalid l=loose m=medium t=tight i=isolated h=highPt s=soft f=forward" << endl
	 << "---------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________
