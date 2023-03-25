//____________________________________________________________________
//
// K0s -> pi+pi-
// 
//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlK0sDecayPiPi
#include <AtlK0sDecayPiPi.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlK0sDecayPiPi);
#endif

//____________________________________________________________________

AtlK0sDecayPiPi::AtlK0sDecayPiPi() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlK0sDecayPiPi::AtlK0sDecayPiPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
				 Float_t E, AtlIDTrack *PiPlus, AtlIDTrack *PiMinus,
				 HepVertex *DecayVtx, HepVertex *PrimaryVtx,
				 HepParticle ReFitPiPlus, HepParticle ReFitPiMinus) :
    Hep2BodyDecay(Id, Px, Py, Pz, E, (TObject*)PiPlus, (TObject*)PiMinus,
		  DecayVtx, PrimaryVtx, ReFitPiPlus, ReFitPiMinus) {
    //
    // Normal constructor
    //
    fPdgCode = 310;
}

//____________________________________________________________________


AtlK0sDecayPiPi::~AtlK0sDecayPiPi(){
    //
    // Default destructor
    //
}


//____________________________________________________________________

void AtlK0sDecayPiPi::Clear(Option_t *option) {
    //
    // Clear this object
    //
    Hep2BodyDecay::Clear(option);
}

//____________________________________________________________________

void AtlK0sDecayPiPi::Print(Option_t *option) {
    //
    // Print K0s decay information
    //
    // Options available:
    //    "VTX"   - Print information about vtx and outgoing tracks
    //    "REFIT" - Print information about refitted outgoing momenta
    //    "ALL"   - Print all
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
    cout.width(5);  cout << GetPiPlus()->GetId();
    cout.width(5);  cout << GetPiMinus()->GetId();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print vertex info
    if ( opt.Contains("vtx") || opt.Contains("all") ) {
	cout << endl << "K0s Decay vertex:" << endl;
	GetVtx()->Print("daughters");
    }

    // Print refited daughters
    if ( opt.Contains("refit") || opt.Contains("all") ) {
	cout << endl << "Refitted daughters:" << endl;
	HepParticle::PrintHeader();
	fReFitDaughter1.Print("nohead");
	fReFitDaughter2.Print("nohead");
	HepParticle::PrintFooter();
    }
    
}

//____________________________________________________________________

void AtlK0sDecayPiPi::PrintHeader() {
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

void AtlK0sDecayPiPi::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------------------------------"
	 << endl;
}
