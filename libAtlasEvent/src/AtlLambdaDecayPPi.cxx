//____________________________________________________________________
//
// Lambda -> Proton Pi-
// 
//  
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlLambdaDecayPPi
#include <AtlLambdaDecayPPi.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlLambdaDecayPPi);
#endif

//____________________________________________________________________

AtlLambdaDecayPPi::AtlLambdaDecayPPi() {
    //
    // Default constructor
    //

}

//____________________________________________________________________

AtlLambdaDecayPPi::AtlLambdaDecayPPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
				     Float_t E, AtlIDTrack *Proton, AtlIDTrack *Pion, 
				     HepVertex *Vtx, HepVertex *PrimaryVtx,
				     HepParticle ReFitDaughter1, HepParticle ReFitDaughter2) :
    Hep2BodyDecay(Id, Px, Py, Pz, E, Proton, Pion, Vtx, PrimaryVtx, ReFitDaughter1, ReFitDaughter2) {
    //
    // Normal constructor
    //
    fPdgCode = 3122;
}

//____________________________________________________________________


AtlLambdaDecayPPi::~AtlLambdaDecayPPi(){
    //
    // Default destructor
    //

}


//____________________________________________________________________

void AtlLambdaDecayPPi::Clear(Option_t *option) {
    //
    // Clear this object
    //
    Hep2BodyDecay::Clear(option);
}

//____________________________________________________________________

void AtlLambdaDecayPPi::Print(Option_t *option) {
    //
    // Print K0s decay information
    //
    // Options available:
    //    "VTX" - Print information about vtx and outgoing tracks
    //
    TString opt = option;
    opt.ToLower();

    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(8);  cout << GetDecayLength();
    cout.width(8);  cout << GetLifeTime();
    cout.width(10); cout << Pmag();
    cout.width(12); cout << M("rec");
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(11); cout << Phi()*180/TMath::Pi();
    cout.width(8);  cout << Eta();
    cout.width(5);  cout << GetVtx()->GetId();
    cout.width(5);  cout << GetProton()->GetId();
    cout.width(5);  cout << GetPion()->GetId();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print vertex info
    if ( opt.Contains("vtx") || opt.Contains("all") ) {
	cout << endl << "Lambda Decay vertex:" << endl;
	GetVtx()->Print("daughters");
    }
}

//____________________________________________________________________

void AtlLambdaDecayPPi::PrintHeader() {
    //
    // Print information header
    //
    cout << "----------------------------------------------------------------------------------------"
	 << endl
	 << " Id   d (cm)  t (ps)   P (GeV)  Mrec (GeV) Theta (deg)  Phi (deg)     Eta  Vtx  Pi+  Pi-"
	 << endl
	 << "----------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlLambdaDecayPPi::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------------------------------"
	 << endl;
}
