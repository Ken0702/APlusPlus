//____________________________________________________________________
//
// W -> l nu decay
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlWDecayLNu
#include <AtlWDecayLNu.h>
#endif

#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlWDecayLNu);
#endif

//____________________________________________________________________

AtlWDecayLNu::AtlWDecayLNu() {
    //
    // Default constructor
    //
    fChargedLepton = new HepParticle;
}

//____________________________________________________________________

AtlWDecayLNu::AtlWDecayLNu(Int_t Id, Float_t Px_W, Float_t Py_W,
			   Float_t Pz_W, Float_t E_W, HepParticle *lepton_orig,
			   Float_t Px_lep, Float_t Py_lep, Float_t Pz_lep,
			   Float_t E_lep, HepParticle *neutrino,
			   ProductionMode mode) :
    HepWDecay(Id, Px_W, Py_W, Pz_W, E_W, (TObject*)lepton_orig, (TObject*)neutrino,
	      mode) {
    //
    // Normal constructor
    //
    // The reference to the (unchanged) charged lepton used for the W
    // reconstruction is given by lepton_orig. The compoments Px_lep,
    // Py_lep, ... denote the 4-momentum of the charged lepton
    // improved by the W reconstruction. The neutrino is the reference
    // to the reconstructed outgoing neutrino. Note that the latter
    // has to be created and added to the event by using
    // AtlEvent::AddNeutrino() before.
    //
    fChargedLepton = new HepParticle(1, Px_lep, Py_lep, Pz_lep,
				     E_lep, lepton_orig->GetPdgCode());
}

//____________________________________________________________________

AtlWDecayLNu::~AtlWDecayLNu() {
    //
    // Default destructor
    //
    delete fChargedLepton;
}

//____________________________________________________________________

void AtlWDecayLNu::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepWDecay::Clear(option);
    fChargedLepton->Clear(option);
}

//____________________________________________________________________

void AtlWDecayLNu::Print(Option_t *option) {
    //
    // Print object information
    //
    cout << endl << endl
	 << "Leptonic W-boson decay #" << fId << endl
	 << "============================" << endl
	 << "Kinematic fit result: chi^2/NDoF = "
	 << fChi2 << " / " << fNDoF << "   Prob = " << fProb << endl << endl
	 << "Reconstructed W-boson:" << endl;
    HepParticle::Print();
    cout << endl
	 << "Outgoing (original) charged lepton:" << endl;
    GetChargedLeptonOrig()->Print();
    cout << "Improved charged lepton 4-momentum: ";
    HepParticle::PrintLorentzVector(GetChargedLepton()->P());
    cout << endl
	 << "Outgoing reconstructed neutrino:" << endl;
    GetNeutrino()->Print();
    cout << endl;
}
