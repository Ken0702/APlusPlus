//____________________________________________________________________
//
// Phi -> K-Plus + K-Minus
// 
//  
// Author: Sophia Schade<mailto: schade@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sophia Schade 
//
#include <AtlPhiDecayKK.h>
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TMath.h>              // for Pi
#include <TString.h>            // for TString
#include <TVector3.h>           // for TVector3
#include <iostream>             // for ostream, cout, operator<<, endl, basi...
#include "AtlIDTrack.h"         // for AtlIDTrack
#include "Hep2BodyDecay.h"      // for Hep2BodyDecay
class TObject;

using namespace std;

#ifndef __CINT__
ClassImp(AtlPhiDecayKK);
#endif

//____________________________________________________________________

AtlPhiDecayKK::AtlPhiDecayKK() {
  //
  // Default constructor
  //
        
}

//____________________________________________________________________

AtlPhiDecayKK::AtlPhiDecayKK(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			     Float_t E, AtlIDTrack *KPlus, AtlIDTrack *KMinus,
			     HepVertex *DecayVtx, HepVertex *PrimaryVtx,
			     HepParticle ReFitKPlus, HepParticle ReFitKMinus) :
    Hep2BodyDecay(Id, Px, Py, Pz, E, (TObject*)KPlus, (TObject*)KMinus,
		  DecayVtx, PrimaryVtx, ReFitKPlus, ReFitKMinus) {
    //
    // Normal constructor
    //
    fPdgCode = 333 ;
    fAngle   = KPlus->P().Angle(KMinus->P());
}

//____________________________________________________________________


AtlPhiDecayKK::~AtlPhiDecayKK(){
    //
    // Default destructor
    //
  
}


//____________________________________________________________________

void AtlPhiDecayKK::Clear(Option_t *option) {
    //
    // Clear this object
    //
    Hep2BodyDecay::Clear(option);
    fAngle     = 0;
}

//____________________________________________________________________

void AtlPhiDecayKK::Print(Option_t *option) {
    //
    // Print Phi decay information
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
    cout.width(7);  cout << Pmag();
    cout.width(10); cout << M("rec");
    cout.width(12); cout << Theta()*180/TMath::Pi();
    cout.width(13); cout << Phi()*180/TMath::Pi();
    cout.width(10); cout << Eta();
    cout.width(8);  cout << GetKPlus()->GetId();
    cout.width(5);  cout << GetKMinus()->GetId();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print vertex info
    if ( opt.Contains("vtx") || opt.Contains("all") ) {
	cout << endl << "Phi Decay vertex:" << endl;
	GetVtx()->Print("daughters");
    }
}

//____________________________________________________________________

void AtlPhiDecayKK::PrintHeader() {
    //
    // Print information header
    //
    cout << "------------------------------------------------------------------------"
	 << endl
	 << " Id   P (GeV)  Mrec (GeV) Theta (deg)  Phi (deg)     Eta      K+    K-"
	 << endl
	 << "------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlPhiDecayKK::PrintFooter() {
    //
    // Print footer
    //
    cout << "------------------------------------------------------------------------"
	 << endl;
}
