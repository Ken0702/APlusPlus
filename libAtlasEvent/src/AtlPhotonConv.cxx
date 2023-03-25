//____________________________________________________________________
//
// gamma -> e+e- conversion
// 
//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlPhotonConv
#include <AtlPhotonConv.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlPhotonConv);
#endif

//____________________________________________________________________

AtlPhotonConv::AtlPhotonConv() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlPhotonConv::AtlPhotonConv(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			   Float_t E, AtlIDTrack *ePlus, AtlIDTrack *eMinus,
			   HepVertex *DecayVtx, HepVertex *PrimaryVtx,
			   HepParticle ReFitePlus, HepParticle ReFiteMinus) :
    Hep2BodyDecay(Id, Px, Py, Pz, E, (TObject*)ePlus, (TObject*)eMinus,
		  DecayVtx, PrimaryVtx, ReFitePlus, ReFiteMinus) {
    //
    // Normal constructor
    //
    fPdgCode = 22;
}

//____________________________________________________________________


AtlPhotonConv::~AtlPhotonConv(){
    //
    // Default destructor
    //
}


//____________________________________________________________________

void AtlPhotonConv::Clear(Option_t *option) {
    //
    // Clear this object
    //
    Hep2BodyDecay::Clear(option);
}

//____________________________________________________________________

void AtlPhotonConv::Print(Option_t *option) {
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
    cout.width(5);  cout << GetePlus()->GetId();
    cout.width(5);  cout << GeteMinus()->GetId();
    cout << endl;

    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();

    // Print vertex info
    if ( opt.Contains("vtx") || opt.Contains("all") ) {
	cout << endl << "Conversion  vertex:" << endl;
	GetVtx()->Print("daughters");
    }

    // Print refitted daughters
    if ( opt.Contains("refit") || opt.Contains("all") ) {
	cout << endl << "Refitted daughters:" << endl;
	HepParticle::PrintHeader();
	fReFitDaughter1.Print("nohead");
	fReFitDaughter2.Print("nohead");
	HepParticle::PrintFooter();
    }
    
}

//____________________________________________________________________

void AtlPhotonConv::PrintHeader() {
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

void AtlPhotonConv::PrintFooter() {
    //
    // Print footer
    //
    cout << "----------------------------------------------------------------------------------------"
	 << endl;
}
