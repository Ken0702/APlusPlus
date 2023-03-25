//____________________________________________________________________
//
// W -> q q decay
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlWDecayJJ
#include <AtlWDecayJJ.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlWDecayJJ);
#endif

//____________________________________________________________________

AtlWDecayJJ::AtlWDecayJJ() {
    //
    // Default constructor
    //
    fJet1 = new HepJet;
    fJet2 = new HepJet;
}

//____________________________________________________________________

AtlWDecayJJ::AtlWDecayJJ(Int_t Id, Float_t Px_W, Float_t Py_W,
			 Float_t Pz_W, Float_t E_W,
			 AtlJet *jet1_orig, AtlJet *jet2_orig,
			 Float_t Px_j1, Float_t Py_j1, Float_t Pz_j1, Float_t E_j1,
			 Float_t Px_j2, Float_t Py_j2, Float_t Pz_j2, Float_t E_j2,
			 ProductionMode mode) :
    HepWDecay(Id, Px_W, Py_W, Pz_W, E_W,
	      (TObject*)jet1_orig, (TObject*)jet2_orig, mode) {
    //
    // Normal constructor
    //
    // The reference to the (unchanged) jets used for the W
    // reconstruction is given by jet1_orig and jet2_orig.
    // The compoments Px_j1, Px_j2, ... denote the 4-momentum
    // components of the jets improved by the W reconstruction.
    //
    fJet1 = new HepJet(1, E_j1, Px_j1, Py_j1, Pz_j1);
    fJet2 = new HepJet(2, E_j2, Px_j2, Py_j2, Pz_j2);
}

//____________________________________________________________________

AtlWDecayJJ::~AtlWDecayJJ() {
    //
    // Default destructor
    //
    delete fJet1;
    delete fJet2;    
}

//____________________________________________________________________

void AtlWDecayJJ::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepWDecay::Clear(option);
    fJet1->Clear(option);
    fJet2->Clear(option);
}

//____________________________________________________________________

void AtlWDecayJJ::Print(Option_t *option) {
    //
    // Print object information
    //
    cout << endl << endl
	 << "Hadronic W-boson decay #" << fId << endl
	 << "============================" << endl
	 << "Kinematic fit result: chi^2/NDoF = "
	 << fChi2 << " / " << fNDoF << "   Prob = " << fProb << endl << endl
	 << "Reconstructed W-boson:" << endl;
    HepParticle::Print();
    cout << endl
	 << "Outgoing (original) jet1:" << endl;
    GetJet1_Orig()->Print();
    cout << "Improved jet1 4-momentum: ";
    HepParticle::PrintLorentzVector(GetJet1()->P());
    cout << endl
	 << "Outgoing (original) jet2:" << endl;
    GetJet2_Orig()->Print();
    cout << "Improved jet2 4-momentum: ";
    HepParticle::PrintLorentzVector(GetJet2()->P());
    cout << endl << endl;
}
    
