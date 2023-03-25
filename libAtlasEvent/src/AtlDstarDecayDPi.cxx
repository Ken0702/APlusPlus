//____________________________________________________________________
//
// Dstar -> D0 Pion
// 
//  
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlDstarDecayDPi
#include <AtlDstarDecayDPi.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlDstarDecayDPi);
#endif

//____________________________________________________________________

AtlDstarDecayDPi::AtlDstarDecayDPi() {
    //
    // Default constructor
    //

}

//____________________________________________________________________

AtlDstarDecayDPi::AtlDstarDecayDPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
				     Float_t E, AtlD0DecayKPi *D0, AtlIDTrack *Pion, 
				     HepVertex *Vtx, HepVertex *PrimaryVtx,
				     HepParticle ReFitDaughter1, HepParticle ReFitDaughter2) :
    Hep2BodyDecay(Id, Px, Py, Pz, E, D0, Pion, Vtx, PrimaryVtx, ReFitDaughter1, ReFitDaughter2) {
    //
    // Normal constructor
    //
    fPdgCode = 3122;
}

//____________________________________________________________________


AtlDstarDecayDPi::~AtlDstarDecayDPi(){
    //
    // Default destructor
    //

}


//____________________________________________________________________

void AtlDstarDecayDPi::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
}

//____________________________________________________________________

void AtlDstarDecayDPi::Print(Option_t *option) {
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
    cout.width(5);  cout << "-1"; // GetKaon()->GetId();
    cout.width(5);  cout << GetPion()->GetId();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print vertex info
    if ( opt.Contains("vtx") || opt.Contains("all") ) {
	cout << endl << "D0 Decay vertex:" << endl;
	GetVtx()->Print("daughters");
    }
}

//____________________________________________________________________

void AtlDstarDecayDPi::PrintHeader() {
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

void AtlDstarDecayDPi::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------------------------------"
	 << endl;
}
