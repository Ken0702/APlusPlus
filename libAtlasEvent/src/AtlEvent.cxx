//____________________________________________________________________
//
// ATLAS event class
//

//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#include <HepDatabasePDG.h>
#include <TString.h>
#include <TSystem.h>
#include <TArrayF.h>
#include <TArrayI.h>
#include <TDirectory.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvent);
#endif


//____________________________________________________________________

AtlEvent::AtlEvent() {
    //
    // Default constructor
    //
    
    // Init datamember contents
    Init();

    // Create sub-objects
    fCone4H1TowerJets    = new TClonesArray("AtlJet",          0);
    fCone7H1TowerJets    = new TClonesArray("AtlJet",          0);
    fCone4H1TopoJets     = new TClonesArray("AtlJet",          0);
    fCone7H1TopoJets     = new TClonesArray("AtlJet",          0);
    fMCCone4HadronJets   = new TClonesArray("AtlJet",          0);
    fMCCone7HadronJets   = new TClonesArray("AtlJet",          0);
    fMCAntiKt4HadronJets = new TClonesArray("AtlJet",          0);
    fMCAntiKt6HadronJets = new TClonesArray("AtlJet",          0);
    fMCAntiKt4HadronPileupJets = new TClonesArray("AtlJet",    0);
    fAntiKt4H1TopoJets   = new TClonesArray("AtlJet",          0);
    fAntiKt4H1TowerJets  = new TClonesArray("AtlJet",          0);
    fAntiKt6H1TowerJets  = new TClonesArray("AtlJet",          0);
    fAntiKt4TopoEMJets   = new TClonesArray("AtlJet",          0);
    fAntiKt4TopoEMJESJets= new TClonesArray("AtlJet",          0);
    fAntiKt4LCTopoJets   = new TClonesArray("AtlJet",          0);
    fAntiKt4TowerJets    = new TClonesArray("AtlJet",          0);
    fAntiKt6TopoEMJets   = new TClonesArray("AtlJet",          0);
    fAntiKt4TopoJets     = new TClonesArray("AtlJet",          0);
    fAntiKt6TowerJets    = new TClonesArray("AtlJet",          0);
    fAntiKt6LCTopoJets   = new TClonesArray("AtlJet",          0);
    fAntiKt6TopoJets     = new TClonesArray("AtlJet",          0);
    fDL1rJets            = new TClonesArray("AtlJet",          0);
    fAtlFastJets         = new TClonesArray("AtlJet",          0);
    fMCParticles         = new TClonesArray("HepMCParticle",   0);
    fMCVertices          = new TClonesArray("HepMCVertex",     0);
    fIDTracks            = new TClonesArray("AtlIDTrack",      0);
    fPixelHits           = new TClonesArray("AtlPixelHit",     0);
    fSCTHits             = new TClonesArray("AtlSCT3DHit",     0);
    fTRTHits             = new TClonesArray("AtlTRTDigit",     0);
    fElectrons           = new TClonesArray("AtlElectron",     0);
    fMuons               = new TClonesArray("AtlMuon",         0);
    fTaus                = new TClonesArray("AtlTau",          0);
    fPhotons             = new TClonesArray("AtlPhoton",       0);
    fAtlFastElectrons    = new TClonesArray("AtlFastElectron", 0);
    fAtlFastMuons        = new TClonesArray("AtlFastMuon",     0);
    fAtlFastTaus         = new TClonesArray("AtlFastTau",      0);
    fAtlFastPhotons      = new TClonesArray("AtlFastPhoton",   0);
    fTopPairs            = new TClonesArray("AtlTopPair",      0);
    fTopDecays           = new TClonesArray("HepTopDecay",     0);
    fWDecaysLNu          = new TClonesArray("AtlWDecayLNu",    0);
    fWDecaysJJ           = new TClonesArray("AtlWDecayJJ",     0);
    fNeutrinos           = new TClonesArray("HepParticle",     0);
    fZ0Decays            = new TClonesArray("HepZ0Decay",      0);
    fPhiDecaysKK         = new TClonesArray("AtlPhiDecayKK",   0);
    fK0sDecaysPiPi       = new TClonesArray("AtlK0sDecayPiPi", 0);
    fPhotonConv          = new TClonesArray("AtlPhotonConv",   0);
    fLambdaDecaysPiPi    = new TClonesArray("AtlLambdaDecayPPi",0);
    fD0DecaysKPi         = new TClonesArray("AtlD0DecayKPi",   0);
    fDstarDecaysDPi      = new TClonesArray("AtlDstarDecayDPi",0);
    fVertices            = new TClonesArray("HepVertex",       1);
    fTrigger             = new AtlTrigger;
}

//____________________________________________________________________

AtlEvent::~AtlEvent() {
    //
    // Default destructor
    //
    Clear("C");
    delete fCone4H1TowerJets;
    delete fCone7H1TowerJets;
    delete fCone4H1TopoJets;
    delete fCone7H1TopoJets;
    delete fMCCone4HadronJets;
    delete fMCCone7HadronJets;
    delete fMCAntiKt4HadronJets;
    delete fMCAntiKt6HadronJets;
    delete fMCAntiKt4HadronPileupJets;
    delete fAntiKt4H1TopoJets;
    delete fAntiKt4H1TowerJets;
    delete fAntiKt6H1TowerJets;
    delete fAntiKt4TopoEMJets;
    delete fAntiKt4TopoEMJESJets;
    delete fAntiKt4LCTopoJets;
    delete fAntiKt4TowerJets;
    delete fAntiKt6TopoEMJets;
    delete fAntiKt4TopoJets;
    delete fAntiKt6TowerJets;
    delete fAntiKt6LCTopoJets;
    delete fAntiKt6TopoJets;
    delete fDL1rJets;
    delete fAtlFastJets;
    delete fMCParticles;
    delete fMCVertices;
    delete fIDTracks;
    delete fPixelHits;
    delete fSCTHits;
    delete fTRTHits;
    delete fElectrons;
    delete fMuons;
    delete fTaus;
    delete fPhotons;
    delete fAtlFastElectrons;
    delete fAtlFastMuons;
    delete fAtlFastTaus;
    delete fAtlFastPhotons;
    delete fTopPairs;
    delete fTopDecays;
    delete fWDecaysLNu;
    delete fWDecaysJJ;
    delete fNeutrinos;
    delete fZ0Decays;
    delete fPhiDecaysKK;
    delete fK0sDecaysPiPi;
    delete fPhotonConv;
    delete fLambdaDecaysPiPi;
    delete fD0DecaysKPi;
    delete fDstarDecaysDPi;
    delete fVertices;
    delete fTrigger;
}

//____________________________________________________________________

void AtlEvent::Clear(Option_t *option) {
    //
    // Clear event
    //
    Init();

    fEventHeader.Clear();
    fTrigger->Clear();
    fEnergySum.Clear();

    fCone4H1TowerJets    ->Clear("C");
    fCone7H1TowerJets    ->Clear("C");
    fCone4H1TopoJets     ->Clear("C");
    fCone7H1TopoJets     ->Clear("C");
    fMCCone4HadronJets   ->Clear("C");
    fMCCone7HadronJets   ->Clear("C");
    fMCAntiKt4HadronJets ->Clear("C");
    fMCAntiKt6HadronJets ->Clear("C");
    fMCAntiKt4HadronPileupJets->Clear("C");
    fAntiKt4H1TopoJets   ->Clear("C");
    fAntiKt4H1TowerJets  ->Clear("C");
    fAntiKt6H1TowerJets  ->Clear("C");
    fAntiKt4TopoEMJets   ->Clear("C");
    fAntiKt4TopoEMJESJets->Clear("C");
    fAntiKt4LCTopoJets   ->Clear("C");
    fAntiKt4TowerJets    ->Clear("C");
    fAntiKt6TopoEMJets   ->Clear("C");
    fAntiKt4TopoJets     ->Clear("C");
    fAntiKt6TowerJets    ->Clear("C");
    fAntiKt6LCTopoJets   ->Clear("C");
    fAntiKt6TopoJets     ->Clear("C");
    fDL1rJets            ->Clear("C");
    fAtlFastJets         ->Clear("C");
    fMCParticles         ->Clear("C");
    fMCVertices          ->Clear("C");
    fIDTracks            ->Clear("C");
    fPixelHits           ->Clear("C");
    fSCTHits             ->Clear("C");
    fTRTHits             ->Clear("C");
    fElectrons           ->Clear("C");
    fMuons               ->Clear("C");
    fTaus                ->Clear("C");
    fPhotons             ->Clear("C");
    fAtlFastElectrons    ->Clear("C");
    fAtlFastMuons        ->Clear("C");
    fAtlFastTaus         ->Clear("C");
    fAtlFastPhotons      ->Clear("C");
    fTopPairs            ->Clear("C");
    fTopDecays           ->Clear("C");
    fWDecaysLNu          ->Clear("C");
    fWDecaysJJ           ->Clear("C");
    fNeutrinos           ->Clear("C");
    fZ0Decays            ->Clear("C");
    fPhiDecaysKK         ->Clear("C");
    fK0sDecaysPiPi       ->Clear("C");
    fPhotonConv          ->Clear("C"); 
    fLambdaDecaysPiPi    ->Clear("C");
    fD0DecaysKPi         ->Clear("C");
    fDstarDecaysDPi      ->Clear("C");
    fVertices            ->Clear("C");
}

//____________________________________________________________________

void AtlEvent::Init() {
    //
    // Initialise event content
    //
    fN_Cone4H1TowerJets    = 0;
    fN_Cone7H1TowerJets    = 0;
    fN_Cone4H1TopoJets     = 0;
    fN_Cone7H1TopoJets     = 0;
    fN_MCCone4HadronJets   = 0;
    fN_MCCone7HadronJets   = 0;
    fN_MCAntiKt4HadronJets = 0;
    fN_MCAntiKt4HadronPileupJets = 0;
    fN_MCAntiKt6HadronJets = 0;
    fN_AntiKt4H1TopoJets   = 0;
    fN_AntiKt4H1TowerJets  = 0;
    fN_AntiKt6H1TowerJets  = 0;
    fN_AntiKt4TopoEMJets   = 0;
    fN_AntiKt4TopoEMJESJets= 0;
    fN_AntiKt4LCTopoJets   = 0;
    fN_AntiKt4TowerJets    = 0;
    fN_AntiKt6TopoEMJets   = 0;
    fN_AntiKt4TopoJets     = 0;
    fN_AntiKt6TowerJets    = 0;
    fN_AntiKt6LCTopoJets   = 0;
    fN_AntiKt6TopoJets     = 0;
    fN_DL1rJets     = 0;
    fN_AtlFastJets         = 0;
    fN_MCParticles         = 0;
    fN_MCVertices          = 0;
    fN_IDTracks            = 0;
    fN_PixelHits           = 0;
    fN_SCTHits             = 0;
    fN_TRTHits             = 0;
    fN_Electrons           = 0;
    fN_Muons               = 0;
    fN_Taus                = 0;
    fN_Photons             = 0;
    fN_AtlFastElectrons    = 0;
    fN_AtlFastMuons        = 0;
    fN_AtlFastTaus         = 0;
    fN_AtlFastPhotons      = 0;
    fN_TopPairs            = 0;
    fN_TopDecays           = 0;
    fN_WDecaysLNu          = 0;
    fN_WDecaysJJ           = 0;
    fN_Neutrinos           = 0;
    fN_Z0Decays            = 0;
    fN_PhiDecaysKK         = 0;
    fN_K0sDecaysPiPi       = 0;
    fN_PhotonConv          = 0;
    fN_LambdaDecaysPiPi    = 0;
    fN_D0DecaysKPi         = 0;
    fN_DstarDecaysDPi      = 0;
    fN_Vertices            = 0;
    fN_vpx                 = -1;
    fbdt_scale             = 0.0;
}    

//____________________________________________________________________

void AtlEvent::Print(Option_t *option) const {
    //
    // Print event information
    //
    // Options available:
    //   "ALL"      - all of the above (default)
    //   "HDR"      - event header
    //   "TRIG"     - trigger
    //   "NOTRIG"   = Do not print trigger information, eg "ALL-NOTRIG"
    //   "SUM"      - energy sums
    //   "JET"      - jets
    //   "PRT"      - MC truth particles
    //   "TRT"      - TRT hits
    //   "ELEC"     - Electrons
    //   "MUON"     - Muons
    //   "TAU"      - Taus
    //   "PHOT"     - Photons
    //   "TRK"      - Tracks
    //   "VTX"      - Vertices
    //   "K0S"      - Reconstructed K0s
    //   "CONV"     - Reconstructed photoconversions
    //   "ZDEC"     - Reconstructed Z0 decays
    //   "WDEC"     - Reconstructed W decays
    //   "TOP"      - Reconstructed top-quark decays
    //
    TString opt = option;
    opt.ToUpper();

    // ============
    // Print header
    // ============
    cout << endl << endl
	 << "====================================================================================================" << endl
	 << " Run " << RunNr() << "   Event " << EventNr() << "   LumiBlock " << LumiBlock();
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    cout << "   BeamEnergy " << BeamEnergy()/1000. << "TeV" << endl
	 << "====================================================================================================" << endl;
    if ( opt.Contains("ALL") || opt.Contains("HDR") ) {
	fEventHeader.Print();
    }
    
    // ==================
    // Print trigger info
    // ==================
    if ( !opt.Contains("NOTRIG") ) {
	if ( opt.Contains("ALL") || opt.Contains("TRIG") ) {
	    // Trigger config already loaded ?
 	    if ( GetTrigger()->GetTriggerConf() == 0 ) {
 		AtlTriggerConf *trig_conf = AtlTriggerConf::Instance()->LoadTree(gDirectory);
		if ( trig_conf == 0 ) {
		  Info("Print",
		       "No trigger information available due to missing trigger configuration");
		} else {
		    GetTrigger()->SetConfig(trig_conf);
		}
	    }
 	    if ( GetTrigger()->GetTriggerConf() != 0 ) {
 		fTrigger->Print(RunNr());
	    }
	}
    }
    
    // ===========
    // Energy sums
    // ===========
    if ( opt.Contains("ALL") || opt.Contains("SUM") ) {
	if ( IsMC() ) {
	    fEnergySum.Print("MC");
	} else {
	    fEnergySum.Print();
	}
    }

    // ========
    // QCD Jets
    // ========
    if ( opt.Contains("ALL") || opt.Contains("JET") ) {
	Int_t njets = 0;
	for ( Int_t j = 0; j < AtlJet::kNumTypes-1; j++ ) {
	    njets = GetN_Jets((AtlJet::EType)j);
	    cout << endl << AtlJet::NameOfType((AtlJet::EType)j)
		 << " Jets: #" << njets << endl;
	    if ( njets > 0 ) {
		AtlJet::PrintHeader();
		for ( Int_t i = 0; i < njets; i++ )
		    ((AtlJet*)GetJets((AtlJet::EType)j)->At(i))->Print("nohead");
		AtlJet::PrintFooter();
	    }
	}
    }

    // ============
    // MC particles
    // ============
    if ( IsMC() && (opt.Contains("ALL") || opt.Contains("PRT")) ) {
	cout << endl << "MC particles: #" << fN_MCParticles << endl;
	if ( fN_MCParticles > 0 ) {
	    HepMCParticle::PrintHeader();
	    for ( Int_t i = 0; i < fN_MCParticles; i++ )
		((HepMCParticle*)fMCParticles->At(i))->Print("nohead");
	    HepMCParticle::PrintFooter();
	}
    }

    // =====================
    // Inner detector tracks
    // =====================
    if ( opt.Contains("ALL") || opt.Contains("TRK") ) {
	cout << endl << "ID tracks: #" << fN_IDTracks << endl;
	if ( fN_IDTracks > 0 ) {
	    AtlIDTrack::PrintHeader();
	    for ( Int_t i = 0; i < fN_IDTracks; i++ )
		((AtlIDTrack*)fIDTracks->At(i))->Print("nohead");
	    AtlIDTrack::PrintFooter();
	}
    }

    // =================
    // TRT digitisations
    // =================
    if ( opt.Contains("ALL") || opt.Contains("TRT") ) {
	cout << endl << "TRT hits: #" << fN_TRTHits << endl;
	if ( fN_TRTHits > 0 ) {
	    AtlTRTDigit::PrintHeader();
	    for ( Int_t i = 0; i < fN_TRTHits; i++ )
		((AtlTRTDigit*)fTRTHits->At(i))->Print("nohead");
	    AtlTRTDigit::PrintFooter();
	}
    }

    // ========
    // Vertices
    // ========
    if ( opt.Contains("ALL") || opt.Contains("VTX") ) {
	cout << endl << "Vertices: #" << fN_Vertices << endl;
	if ( fN_Vertices > 0 ) {
	    HepVertex::PrintHeader();
	    for ( Int_t i = 0; i < fN_Vertices; i++ )
		((HepVertex*)fVertices->At(i))
		    ->Print(Form("nohead,%s", opt.Data()));
	    HepVertex::PrintFooter();
	}
    }
	
    // =========
    // Electrons
    // =========
    if ( opt.Contains("ALL") || opt.Contains("ELEC") ) {
	cout << endl << "Electrons: #" << fN_Electrons << endl;
	if ( fN_Electrons > 0 ) {
	    AtlElectron::PrintHeader();
	    for ( Int_t i = 0; i < fN_Electrons; i++ )
		((AtlElectron*)fElectrons->At(i))->Print("nohead");
	    AtlElectron::PrintFooter();
	}

	// AtlFast electrons
	if ( IsMC() ) {
	  cout << endl << "AtlFastElectrons: #" << fN_AtlFastElectrons << endl;
	  if ( fN_AtlFastElectrons > 0 ) {
	    AtlFastElectron::PrintHeader();
	    for ( Int_t i = 0; i < fN_AtlFastElectrons; i++ )
	      ((AtlFastElectron*)fAtlFastElectrons->At(i))->Print("nohead");
	    AtlFastElectron::PrintFooter();
	  }
	}
    }

    // =====
    // Muons
    // =====
    if ( opt.Contains("ALL") || opt.Contains("MUON") ) {
	cout << endl << "Muons: #" << fN_Muons << endl;
	if ( fN_Muons > 0 ) {
	    AtlMuon::PrintHeader();
	    for ( Int_t i = 0; i < fN_Muons; i++ )
		((AtlMuon*)fMuons->At(i))->Print("nohead");
	    AtlMuon::PrintFooter();
	}

    	// AtlFast muons
	if ( IsMC() ) {
	  cout << endl << "AtlFastMuons: #" << fN_AtlFastMuons << endl;
	  if ( fN_AtlFastMuons > 0 ) {
	    AtlFastMuon::PrintHeader();
	    for ( Int_t i = 0; i < fN_AtlFastMuons; i++ )
	      ((AtlFastMuon*)fAtlFastMuons->At(i))->Print("nohead");
	    AtlFastMuon::PrintFooter();
	  }
	}
    }

    // ====
    // Taus
    // ====
    if ( opt.Contains("ALL") || opt.Contains("TAU") ) {
	cout << endl << "Taus: #" << fN_Taus << endl;
	if ( fN_Taus > 0 ) {
	    AtlTau::PrintHeader();
	    for ( Int_t i = 0; i < fN_Taus; i++ )
		((AtlTau*)fTaus->At(i))->Print("nohead");
	    AtlTau::PrintFooter();
	}

    	// AtlFast taus
	if ( IsMC() ) {
	  cout << endl << "AtlFastTaus: #" << fN_AtlFastTaus << endl;
	  if ( fN_AtlFastTaus > 0 ) {
	    AtlFastTau::PrintHeader();
	    for ( Int_t i = 0; i < fN_AtlFastTaus; i++ )
	      ((AtlFastTau*)fAtlFastTaus->At(i))->Print("nohead");
	    AtlFastTau::PrintFooter();
	  }
	}
    }

    // =======
    // Photons
    // =======
    if ( opt.Contains("ALL") || opt.Contains("PHOT") ) {
	cout << endl << "Photons: #" << fN_Photons << endl;
	if ( fN_Photons > 0 ) {
	    AtlPhoton::PrintHeader();
	    for ( Int_t i = 0; i < fN_Photons; i++ )
		((AtlPhoton*)fPhotons->At(i))->Print("nohead");
	    AtlPhoton::PrintFooter();
	}

	// AtlFastPhotons
	if ( IsMC() ) {
	  cout << endl << "AtlFastPhotons: #" << fN_AtlFastPhotons << endl;
	  if ( fN_AtlFastPhotons > 0 ) {
	    AtlFastPhoton::PrintHeader();
	    for ( Int_t i = 0; i < fN_AtlFastPhotons; i++ )
	      ((AtlFastPhoton*)fAtlFastPhotons->At(i))->Print("nohead");
	    AtlFastPhoton::PrintFooter();
	  }
	}
    }

    // ==========
    // K0s Decays
    // ==========
    if ( opt.Contains("ALL") || opt.Contains("K0S") ) {
	cout << endl << "K0s Decays: #" << fN_K0sDecaysPiPi << endl;
	if ( fN_K0sDecaysPiPi > 0 ) {
	    AtlK0sDecayPiPi::PrintHeader();
	    for ( Int_t i = 0; i < fN_K0sDecaysPiPi; i++ )
		((AtlK0sDecayPiPi*)fK0sDecaysPiPi->At(i))->Print("nohead");
	    AtlK0sDecayPiPi::PrintFooter();
	}
    }

    // ===========
    // Conversions
    // ===========
    if ( opt.Contains("ALL") || opt.Contains("CONV") ) {
	cout << endl << "Conversions #" << fN_PhotonConv << endl;
	if ( fN_PhotonConv > 0 ) {
	    AtlPhotonConv::PrintHeader();
	    for ( Int_t i = 0; i < fN_PhotonConv; i++ )
		((AtlPhotonConv*)fPhotonConv->At(i))->Print("nohead");
	    AtlPhotonConv::PrintFooter();
	}
    }

    // ========
    // Z decays
    // ========
    if ( opt.Contains("ALL") || opt.Contains("ZDEC") ) {
	cout << endl << "Z0 decays #" << fN_Z0Decays << endl;
	if ( fN_Z0Decays > 0 ) {
	    HepParticle::PrintHeader();
	    for ( Int_t i = 0; i < fN_Z0Decays; i++ )
		((HepZ0Decay*)fZ0Decays->At(i))->Print("nohead");
	    HepParticle::PrintFooter();
	}
    }

    // ========
    // W decays
    // ========
    if ( opt.Contains("ALL") || opt.Contains("WDEC") ) {
	cout << endl << "W->l+nu decays #" << fN_WDecaysLNu << endl;
	if ( fN_WDecaysLNu > 0 ) {
	    for ( Int_t i = 0; i < fN_WDecaysLNu; i++ )
		((AtlWDecayLNu*)fWDecaysLNu->At(i))->Print("nohead");
	}
	cout << endl << "W->jj decays #" << fN_WDecaysJJ << endl;
	if ( fN_WDecaysJJ > 0 ) {
	    for ( Int_t i = 0; i < fN_WDecaysJJ; i++ )
		((AtlWDecayJJ*)fWDecaysJJ->At(i))->Print("nohead");
	}
    }
    
    // ================
    // Top-quark decays
    // ================
    if ( opt.Contains("ALL") || opt.Contains("TOP") ) {
	cout << endl << "Top-quark decays #" << fN_TopDecays << endl;
	if ( fN_TopDecays > 0 ) {
	    for ( Int_t i = 0; i < fN_TopDecays; i++ )
		((HepTopDecay*)fTopDecays->At(i))->Print("nohead");
	}
    }

    // ============
    // End of event
    // ============
    cout << endl
	 << "====================================================================================================" << endl
	 << "end of event " << RunNr() << "/" << EventNr() << endl << endl;
}

//____________________________________________________________________

AtlJet* AtlEvent::AddJet(AtlJet::EType type,
			 Float_t E, Float_t Px, Float_t Py, Float_t Pz,
			 AtlJet::EJetQuality JetQuality,
			 TLorentzVector P_EMSCALE, TLorentzVector P_JESCorrSCALE,
			 Double_t EMJES_EtaCorr, 
			 Double_t BCH_CORR_CELL, Double_t BCH_CORR_JET,
			 Float_t eta_offsetJES) {
    //
    // Add jet of given type to the list of jets
    //
    TClonesArray &jets = *GetJets(type);
    Int_t njets = GetN_Jets(type);
    AtlJet *jet = new(jets[njets]) AtlJet(njets+1, E, Px, Py, Pz, JetQuality,
					  P_EMSCALE, P_JESCorrSCALE,
					  EMJES_EtaCorr,
					  BCH_CORR_CELL, BCH_CORR_JET,
					  eta_offsetJES);
    SetN_Jets(type, njets+1); 
    return jet;
}

//____________________________________________________________________

HepMCParticle* AtlEvent::AddMCParticle(Int_t PdgCode, Float_t Px, Float_t Py,
				       Float_t Pz, Float_t E, Int_t MCStatus,
				       Bool_t IsGenerator,
				       Bool_t IsGenNonInteracting,
				       Bool_t IsGenStable,
				       Bool_t IsGenSimulStable,
				       Bool_t IsGenInteracting,
				       Bool_t IsConversion,
				       Bool_t IsBremsstrahlung) {
    //
    // Add MC truth particle to the list of particles
    //
    TClonesArray &mcparticles = *fMCParticles;
    HepMCParticle *mcprt = new(mcparticles[fN_MCParticles++])
	HepMCParticle(fN_MCParticles, PdgCode, Px, Py, Pz, E, MCStatus,
		      IsGenerator, IsGenNonInteracting,
		      IsGenStable, IsGenSimulStable,
		      IsGenInteracting, IsConversion,
		      IsBremsstrahlung);
    return mcprt;
}

//____________________________________________________________________

HepMCVertex* AtlEvent::AddMCVertex(Float_t x, Float_t y, Float_t z) {
    //
    // Add MC truth vertex to the list of MC vertices
    //
    TClonesArray &mcvertices = *fMCVertices;
    HepMCVertex *mcvtx = new(mcvertices[fN_MCVertices++])
	HepMCVertex(fN_MCVertices, x, y, z);
    return mcvtx;
}

//____________________________________________________________________

AtlIDTrack* AtlEvent::AddIDTrack(Float_t Chi2, Int_t NDoF,
				 Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
				 Float_t QovP, Float_t D0, Float_t Z0,
				 Float_t Theta, const Float_t CovMat[15]) {
    //
    // Add inner detector track to list of ID tracks
    //
    TClonesArray &id_tracks = *fIDTracks;
    AtlIDTrack *trk = new(id_tracks[fN_IDTracks++])
	AtlIDTrack(fN_IDTracks, Chi2, NDoF, Xref, Yref, Zref, Phi0, QovP,
		   D0, Z0, Theta, CovMat);
    return trk;
}

//____________________________________________________________________

AtlIDTrack* AtlEvent::AddIDTrack(AtlIDTrack *trk) {
    //
    // Add inner detector track to list of ID tracks
    //
    TClonesArray &id_tracks = *fIDTracks;
    AtlIDTrack *trk_clone = new(id_tracks[fN_IDTracks++])
	AtlIDTrack(fN_IDTracks, trk->GetChi2(), trk->GetNDoF(),
		   trk->GetRef().X(), trk->GetRef().Y(), trk->GetRef().Z(), trk->GetPhi0(),
		   trk->QovP(), trk->GetD0(), trk->GetZ0(),
		   trk->GetTheta0(), trk->GetCovMatrix());
    return trk_clone;
}

//____________________________________________________________________

AtlTRTDigit* AtlEvent::AddTRTHit(Float_t DriftTime, Float_t DriftRadius,
				 UInt_t Digit, Int_t EndCapOrBarrel,
				 Int_t PhiSector, Int_t ModuleOrWheel,
				 Int_t StrawLayer, Int_t Straw) {
    //
    // Add TRT digitisation to list of TRT hits
    //
    TClonesArray &hits = *fTRTHits;
    AtlTRTDigit *hit = new(hits[fN_TRTHits++])
        AtlTRTDigit(fN_TRTHits, DriftTime, DriftRadius, Digit);

    hit->SetEndCapOrBarrel(EndCapOrBarrel);
    hit->SetPhiSector(PhiSector);
    hit->SetModuleOrWheel(ModuleOrWheel);
    hit->SetStrawLayer(StrawLayer);
    hit->SetStraw(Straw);

    return hit;
}

//____________________________________________________________________

AtlPixelHit* AtlEvent::AddPixelHit(Float_t X, Float_t Y, Float_t Z) {
    //
    // Add Pixel digitisation to list of Pixel hits
    //
    TClonesArray &hits = *fPixelHits;
    AtlPixelHit *hit = new(hits[fN_PixelHits++])
      AtlPixelHit(fN_PixelHits, X, Y, Z);
    return hit;
}

//____________________________________________________________________

AtlSCT3DHit* AtlEvent::AddSCTHit(Float_t X, Float_t Y, Float_t Z) {
    //
    // Add SCT digitisation to list of SCT hits
    //
    TClonesArray &hits = *fSCTHits;
    AtlSCT3DHit *hit = new(hits[fN_SCTHits++])
      AtlSCT3DHit(fN_SCTHits, X, Y, Z);
    return hit;
}

//____________________________________________________________________

AtlElectron* AtlEvent::AddElectron(Float_t Px, Float_t Py, Float_t Pz,
				   Float_t E, Bool_t IsPositron,
				   Float_t EMWeight, Float_t BkgWeight, 
				   UInt_t OQFlag,
				   UInt_t IsEMBitField,
				   AtlEMShower::EIsEM IsEM,
				   AtlEMShower::EAuthor Author,
				   TLorentzVector PCluster) {
    //
    // Add electron to the list of electrons
    //
    TClonesArray &electrons = *fElectrons;
    AtlElectron *electron = new(electrons[fN_Electrons++])
	AtlElectron(fN_Electrons, Px, Py, Pz, E, IsPositron, 
		    EMWeight, BkgWeight, OQFlag, 
		    IsEMBitField, IsEM, Author, PCluster); 
    return electron;
}

//____________________________________________________________________

AtlMuon* AtlEvent::AddMuon(Float_t Px, Float_t Py, Float_t Pz,
			   Float_t E, Bool_t IsMuPlus, 
			   Float_t EtCone10, Float_t EtCone20, 
			   Float_t EtCone30, Float_t EtCone40, Int_t NtrkCone10,
			   Int_t NtrkCone20, Int_t NtrkCone30, Int_t NtrkCone40,
			   Float_t PtCone10, Float_t PtCone20, Float_t PtCone30,
			   Float_t PtCone40, AtlMuon::EAuthor Author,
			   AtlMuon::EQuality Quality, 
			   Float_t MatchingChi2, Int_t MatchingNDoF, 
			   Bool_t IsCombinedMuon,
			   TLorentzVector PMuonSpecExtrapol, 
			   Int_t MuonSpecExtrapolCharge) {
    //
    // Add muon to the list of muons
    //
    TClonesArray &muons = *fMuons;
    AtlMuon *muon = new(muons[fN_Muons++])
	AtlMuon(fN_Muons, Px, Py, Pz, E, IsMuPlus, EtCone10, EtCone20,
		EtCone30, EtCone40, NtrkCone10, NtrkCone20, NtrkCone30,
		NtrkCone40, PtCone10, PtCone20, PtCone30, PtCone40,
		Author, Quality, MatchingChi2, MatchingNDoF, IsCombinedMuon,
		PMuonSpecExtrapol, MuonSpecExtrapolCharge);
    return muon;
}

//____________________________________________________________________

AtlTau* AtlEvent::AddTau(Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Bool_t IsTauPlus, 
			 AtlTau::EAuthor Author, AtlTau::ETauFlag TauFlag) {
    //
    // Add tau to the list of taus
    //
    TClonesArray &taus = *fTaus;
    AtlTau *tau = new(taus[fN_Taus++])
      AtlTau(fN_Taus, Px, Py, Pz, E, IsTauPlus, 
	     Author, TauFlag);
    return tau;
}

//____________________________________________________________________

AtlPhoton* AtlEvent::AddPhoton(Float_t Px, Float_t Py, Float_t Pz,
			       Float_t E,
			       Float_t EMWeight, Float_t BkgWeight, 
			       UInt_t OQFlag, 
			       UInt_t IsEMBitField, 
			       AtlEMShower::EIsEM IsEM,
			       AtlEMShower::EAuthor Author,
			       TLorentzVector PCluster) {
  //
  // Add photon to the list of photons
  //
  TClonesArray &photons = *fPhotons;
  AtlPhoton *photon = new(photons[fN_Photons++])
    AtlPhoton(fN_Photons, Px, Py, Pz, E,
	      EMWeight, BkgWeight,
	      OQFlag, IsEMBitField,
	      IsEM, Author, PCluster);
  return photon;
}

//____________________________________________________________________

AtlFastElectron* AtlEvent::AddAtlFastElectron(Float_t Px, Float_t Py, Float_t Pz,
				   Float_t E, Bool_t IsPositron) {
    //
    // Add electron to the list of electrons
    //
    TClonesArray &electrons = *fAtlFastElectrons;
    AtlFastElectron *electron = new(electrons[fN_AtlFastElectrons++])
	AtlFastElectron(fN_AtlFastElectrons, Px, Py, Pz, E, IsPositron);
    return electron;
}

//____________________________________________________________________

AtlFastMuon* AtlEvent::AddAtlFastMuon(Float_t Px, Float_t Py, Float_t Pz,
			   Float_t E, Bool_t IsMuPlus) {
    //
    // Add muon to the list of muons
    //
    TClonesArray &muons = *fAtlFastMuons;
    AtlFastMuon *muon = new(muons[fN_AtlFastMuons++])
	AtlFastMuon(fN_Muons, Px, Py, Pz, E, IsMuPlus);
    return muon;
}

//____________________________________________________________________

AtlFastTau* AtlEvent::AddAtlFastTau(Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Bool_t IsTauPlus) {
    //
    // Add tau to the list of taus
    //
    TClonesArray &taus = *fAtlFastTaus;
    AtlFastTau *tau = new(taus[fN_AtlFastTaus++])
	AtlFastTau(fN_AtlFastTaus, Px, Py, Pz, E, IsTauPlus);
    return tau;
}

//____________________________________________________________________

AtlFastPhoton* AtlEvent::AddAtlFastPhoton(Float_t Px, Float_t Py, Float_t Pz,
			       Float_t E) {
    //
    // Add photon to the list of photons
    //
    TClonesArray &photons = *fAtlFastPhotons;
    AtlFastPhoton *photon = new(photons[fN_AtlFastPhotons++])
	AtlFastPhoton(fN_AtlFastPhotons, Px, Py, Pz, E);
    return photon;
}

//____________________________________________________________________

AtlTopPair*  AtlEvent::AddTopPair(HepTopDecay* top1, HepTopDecay* top2, 
                                  Double_t chi2, Int_t ndof, AtlTopPair::EType type){
    //
    // Add reconstructed Top pair to list of Top pairs
    //

  TClonesArray &TopPairs = *fTopPairs;
  AtlTopPair *TopPair = new(TopPairs[fN_TopPairs++])
  AtlTopPair(fN_TopPairs, top1, top2, chi2, ndof, type);
  return TopPair;

}

//____________________________________________________________________

HepTopDecay* AtlEvent::AddTopDecay(Float_t Px, Float_t Py, Float_t Pz,
				   Float_t E, HepWDecay *WDecay,
				   HepJet *BJetOrig,Float_t Px_j,
				   Float_t Py_j, Float_t Pz_j, Float_t E_j,
				   HepTopDecay::ProductionMode mode) {
    //
    // Add reconstructed Top decay to list of Top decays
    //
    
    TClonesArray &TopDecays = *fTopDecays;
    HepTopDecay *TopDecay = new(TopDecays[fN_TopDecays++])
	HepTopDecay(fN_TopDecays, Px, Py, Pz, E, WDecay, BJetOrig,
		    Px_j, Py_j, Pz_j, E_j, mode);
    return TopDecay;
}

//____________________________________________________________________

AtlWDecayLNu* AtlEvent::AddWDecayLNu(Float_t Px_W, Float_t Py_W, Float_t Pz_W,
				     Float_t E_W, HepParticle *lepton_orig, Float_t Px_lep,
				     Float_t Py_lep, Float_t Pz_lep, Float_t E_lep,
				     HepParticle* neutrino,
				     HepWDecay::ProductionMode mode) {
    //
    // Add reconstructed W -> l+nu decay to list of W decays
    //
    TClonesArray &WdecaysLNu = *fWDecaysLNu;
    AtlWDecayLNu *WDecayLNu = new(WdecaysLNu[fN_WDecaysLNu++])
	AtlWDecayLNu(fN_WDecaysLNu, Px_W, Py_W, Pz_W, E_W,
		     lepton_orig, Px_lep, Py_lep, Pz_lep, E_lep, neutrino, mode);
    return WDecayLNu;
}

//____________________________________________________________________

AtlWDecayJJ* AtlEvent::AddWDecayJJ(Float_t Px_W, Float_t Py_W, Float_t Pz_W,
				   Float_t E_W, AtlJet *jet1_orig, AtlJet *jet2_orig,
				   Float_t Px_j1, Float_t Py_j1, Float_t Pz_j1, Float_t E_j1,
				   Float_t Px_j2, Float_t Py_j2, Float_t Pz_j2, Float_t E_j2,
				   HepWDecay::ProductionMode mode) {
        TClonesArray &WdecaysJJ = *fWDecaysJJ;
    AtlWDecayJJ *WDecayJJ = new(WdecaysJJ[fN_WDecaysJJ++])
	AtlWDecayJJ(fN_WDecaysJJ, Px_W, Py_W, Pz_W, E_W, jet1_orig, jet2_orig,
		    Px_j1, Py_j1, Pz_j1, E_j1, Px_j2, Py_j2, Pz_j2, E_j2, mode);
    return WDecayJJ;
}

//____________________________________________________________________

HepParticle* AtlEvent::AddNeutrino(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				   Int_t PdgCode) {
    //
    // Add neutrino to the list of reconstructed neutrinos
    //
    // Valid PDG codes:
    //    12 = nu_e
    //   -12 = nu_e_bar
    //    14 = nu_mu
    //   -14 = nu_mu_bar
    //    16 = nu_tau
    //   -16 = nu_tau_bar
    //

    // Check PDG code first
    Int_t pdg = TMath::Abs(PdgCode);
    if ( pdg != 12 && pdg != 14 && pdg != 16 ) {
	Error("AddNeutrino", "Invalid PDG code %d given. Abort!",
	      PdgCode);
	gSystem->Abort(0);
    }

    // Now add
    TClonesArray &neutrinos = *fNeutrinos;
    HepParticle *neutrino = new(neutrinos[fN_Neutrinos++])
	HepParticle(fN_Neutrinos, Px, Py, Pz, E, PdgCode);
    return neutrino;
}

//____________________________________________________________________

HepZ0Decay* AtlEvent::AddZ0Decay(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				 TObject* Daughter1, TObject* Daughter2,
                                 HepParticle ReFitDaughter1, HepParticle ReFitDaughter2) {
    //
    // Add reconstructed Z0 decay to list of Z0 decays
    //
    TClonesArray &Z0decays = *fZ0Decays;
    HepZ0Decay *Z0Decay = new(Z0decays[fN_Z0Decays++])
	HepZ0Decay(fN_Z0Decays, Px, Py, Pz, E, Daughter1, Daughter2,
		   ReFitDaughter1, ReFitDaughter2);
    return Z0Decay;
}

//____________________________________________________________________

HepZ0Decay* AtlEvent::AddZ0Decay(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				 TObject* Daughter1, TObject* Daughter2) {
    //
    // Add reconstructed Z0 decay to list of Z0 decays
    //
    TClonesArray &Z0decays = *fZ0Decays;
    HepZ0Decay *Z0Decay = new(Z0decays[fN_Z0Decays++])
	HepZ0Decay(fN_Z0Decays, Px, Py, Pz, E, Daughter1, Daughter2);
    return Z0Decay;
}

//____________________________________________________________________

AtlK0sDecayPiPi* AtlEvent::AddK0sDecayPiPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				       AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
				       HepVertex* Vtx,
				       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {

    TClonesArray &K0sdecays =  *fK0sDecaysPiPi;
    AtlK0sDecayPiPi *K0sDecayClone = new(K0sdecays[fN_K0sDecaysPiPi++])
	AtlK0sDecayPiPi(fN_K0sDecaysPiPi, Px, Py, Pz, E, Daughter1, Daughter2,
		    Vtx, GetPrimaryVtx(), Fit_Daughter1, Fit_Daughter2);

    return K0sDecayClone;
}

//____________________________________________________________________

AtlPhotonConv* AtlEvent::AddPhotonConv(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				     AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
				     HepVertex* Vtx,
				     HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {
    
    TClonesArray &PhotonConv =  *fPhotonConv;
    AtlPhotonConv *PhotonConvClone = new(PhotonConv[fN_PhotonConv++])
	AtlPhotonConv(fN_PhotonConv, Px, Py, Pz, E, Daughter1, Daughter2,
		     Vtx, GetPrimaryVtx(), Fit_Daughter1, Fit_Daughter2);
    
    return PhotonConvClone;
}

//____________________________________________________________________


AtlLambdaDecayPPi* AtlEvent::AddLambdaDecayPiPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
						AtlIDTrack* Proton, AtlIDTrack* Pion, 
						HepVertex* Vertex, HepVertex* PrimaryVtx,
						HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {
    //
    // Add reconstructed Lambda decay to list of Lambda decays
    //
    TClonesArray &LambdaDecays = *fLambdaDecaysPiPi;
    AtlLambdaDecayPPi *LambdaDecayClone = new(LambdaDecays[fN_LambdaDecaysPiPi++])
      AtlLambdaDecayPPi(fN_LambdaDecaysPiPi, Px, Py, Pz, E, Proton, Pion, Vertex, PrimaryVtx,
			Fit_Daughter1, Fit_Daughter2);
    return LambdaDecayClone;
}

//____________________________________________________________________

AtlD0DecayKPi* AtlEvent::AddD0DecayKPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				       AtlIDTrack* Kaon, AtlIDTrack* Pion, 
				       HepVertex* Vertex, HepVertex* PrimaryVtx,
				       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {
    //
    // Add reconstructed D0 decay to list of D0 decays
    //
    TClonesArray &D0Decays = *fD0DecaysKPi;
    AtlD0DecayKPi *D0DecayClone = new(D0Decays[fN_D0DecaysKPi++])
      AtlD0DecayKPi(fN_D0DecaysKPi, Px, Py, Pz, E, Kaon, Pion, Vertex, PrimaryVtx,
		    Fit_Daughter1, Fit_Daughter2);
    return D0DecayClone;
}

//____________________________________________________________________

AtlDstarDecayDPi* AtlEvent::AddDstarDecayDPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				       AtlD0DecayKPi* D0, AtlIDTrack* Pion, 
				       HepVertex* Vertex, HepVertex* PrimaryVtx,
				       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {
    //
    // Add reconstructed Dstar decay to list of Dstar decays
    //
    TClonesArray &DstarDecays = *fDstarDecaysDPi;
    AtlDstarDecayDPi *DstarDecayClone = new(DstarDecays[fN_DstarDecaysDPi++])
	AtlDstarDecayDPi(fN_DstarDecaysDPi, Px, Py, Pz, E, D0, Pion, Vertex, PrimaryVtx,
			 Fit_Daughter1, Fit_Daughter2);
    return DstarDecayClone;
}

//____________________________________________________________________

AtlPhiDecayKK* AtlEvent::AddPhiDecayKK(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				       AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
				       HepVertex* Vtx,
				       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) {
    //
    // Add reconstructed Phi decay to list of Phi decays
    //
    TClonesArray &Phidecays = *fPhiDecaysKK;
    AtlPhiDecayKK *PhiDecayClone = new(Phidecays[fN_PhiDecaysKK++])
	AtlPhiDecayKK(fN_PhiDecaysKK, Px, Py, Pz, E, Daughter1, Daughter2,
		      Vtx,GetPrimaryVtx(),Fit_Daughter1, Fit_Daughter2);
    return PhiDecayClone;
}

//____________________________________________________________________

HepVertex* AtlEvent::AddVertex(Float_t X, Float_t Y, Float_t Z,
			       Float_t Chi2, Int_t NDoF, 
			       Float_t errX, Float_t errY, Float_t errZ,
			       Int_t n_tracks, Int_t type) {
    //
    // Add reconstructed vertex to the list of vertices
    //
    TClonesArray &vertices = *fVertices;
    HepVertex *vtx = new(vertices[fN_Vertices++])
	HepVertex(fN_Vertices, X, Y, Z, Chi2, NDoF);
    vtx->SetNDaughters(n_tracks);
    vtx->SetErrors(errX, errY, errZ);
    if ( type == 1 ) 
      vtx->SetPrimary();
    else if ( type == 3 )
      vtx->SetPileUp();
    else 
      vtx->SetNotSpecified();

    return vtx;
}

//____________________________________________________________________

HepMCParticle* AtlEvent::GetMCParticle_ById(Int_t Id) const {
    //
    // Get MC particle by its Id
    //
    // Try to avoid this function since it is rather slow.
    // Also, the Id numbers might not be unique.
    // Instead fetch the particle from the clones array via
    // GetMCParticles()->At(i)
    //
    for ( Int_t i = 0; i < fN_MCParticles; i++ ) {
	HepMCParticle *particle = (HepMCParticle*)fMCParticles->At(i);
	if ( Id == particle->GetId() ) return particle;
    }
    if ( gDebug && Id != 0 )
	Warning("GetMCParticle_ById",
		"MC particle with Id %-d does not exist.", Id);
    return 0;
}

//____________________________________________________________________

TList* AtlEvent::GetMCTops(Bool_t good) const {
    //
    //good=kTRUE (default): Returns all MC Tops,which decay in W and quark(but not t-quark)
    //good=kFALSE	  :Returns all MCTops produced by the generator
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    //
    
    if (!IsMC()){
	Error("GetMCTops"," Event is not MC! -> Abort");
	gSystem->Abort(0);
    }
    TList *ListMCTops = new TList;
    ListMCTops->AddAll(GetMCParticles("t"));
    ListMCTops->AddAll(GetMCParticles("t_bar"));
    
    if (good){
	TList *GoodTops = new TList;
	HepMCParticle *top = 0;
	TIter nexttop(ListMCTops);
	while ((top=(HepMCParticle*)nexttop())){
	    Bool_t containsW=kFALSE;
	    Bool_t containsQuark=kFALSE;
	    Int_t NDaughters=top->GetN_Daughters();
	    if (NDaughters<2) continue;
	    if (!top->IsGenerator()) continue;
	    if (!top->IsGoodMother()) continue;
	    for (Int_t j=0;j<NDaughters;j++){
		if (((HepMCParticle*)top->GetDaughters()->At(j))->IsWBoson()) containsW=kTRUE;  
		if (((HepMCParticle*)top->GetDaughters()->At(j))->IsQuark()&& !((HepMCParticle*)top->GetDaughters()->At(j))->IsTopQuark()) containsQuark=kTRUE; 
	    }//end for j
	    if (containsW && containsQuark){
		GoodTops->Add(top);	      
	    }
	}//end while 
	delete ListMCTops;
	return GoodTops;
    }//good
    else return ListMCTops;    
}

//____________________________________________________________________

HepMCParticle* AtlEvent::GetGoodMother(HepMCParticle *particle){
    //
    // Look at chain of daughters of 'particle'
    // and return the daughter which actually
    // gives birth to another particle.
    // This is written to help out in case of bad MC samples
    // where mother particles give birth to themselves.
    //
    HepMCParticle *null = 0;
    
    if ( particle->GetN_Daughters() == 0 ) {
	Error("GetGoodMother","This particle does not have any daughters.");
	return null;
    }
    
    HepMCParticle *daughter = particle;
    while ( !daughter->IsGoodMother() ) {
	if( daughter->GetN_Daughters() != 0 ) {
	    daughter = (HepMCParticle*)daughter->GetDaughters()->At(0);
        }
        else {
	    Error("GetGoodMother","This particle has daughters that are no good mothers because they do not have any daughters. DEAD END");
	    return null;
        }
    }
    
    return daughter;
}

//____________________________________________________________________

AtlElectron* AtlEvent::GetElectron_ById(Int_t Id) const {
    //
    // Get Electron by its Id
    //
    // Try to avoid this function since it is rather slow.
    // Also, the Id numbers might not be unique.
    // Instead fetch the particle from the clone array via
    // GetElectrons()->At(i)
    //
    for ( Int_t i = 0; i < fN_Electrons; i++ ) {
	AtlElectron *el = (AtlElectron*)fElectrons->At(i);
	if ( Id == el->GetId() ) return el;
    }
    if ( gDebug && Id != 0 )
	Warning("GetElectron_ById",
		"AtlElectron with Id %-d does not exist.", Id);
    return 0;
}

//____________________________________________________________________

AtlMuon* AtlEvent::GetMuon_ById(Int_t Id) const {
    //
    // Get Muon by its Id
    //
    // Try to avoid this function since it is rather slow.
    // Also, the Id numbers might not be unique.
    // Instead fetch the particle from the clone array via
    // GetElectrons()->At(i)
    //
    for ( Int_t i = 0; i < fN_Muons; i++ ) {
	AtlMuon *mu = (AtlMuon*)fMuons->At(i);
	if ( Id == mu->GetId() ) return mu;
    }
    if ( gDebug && Id != 0 )
	Warning("GetMuon_ById",
		"AtlMuon with Id %-d does not exist.", Id);
    return 0;
}

//____________________________________________________________________

TList* AtlEvent::GetMCParticles(const char* PrtType) const {
    //
    // Return list of all MC particles of given type
    //
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    //
    TList *particles = new TList;
    for ( Int_t i = 0; i < fN_MCParticles; i++ ) {
	HepMCParticle *prt = (HepMCParticle*)fMCParticles->At(i);
	if ( HepDatabasePDG::Instance()->GetParticle(PrtType)->PdgCode()
	     == prt->GetPdgCode() ) particles->Add(prt);
    }
    return particles;
}

//____________________________________________________________________

void AtlEvent::PrintMCParticles(const char* PrtType) const {
    //
    // Print all MC particle of given type
    //
    cout << endl;
    HepMCParticle::PrintHeader();
    TList *particles = GetMCParticles(PrtType);
    TIter next_prt(particles);
    HepMCParticle *prt = 0;
    while ( (prt = (HepMCParticle*)next_prt()) ) {
	prt->Print("nohead");
    }
    HepMCParticle::PrintFooter();
    cout << endl;
    delete particles;
}

//____________________________________________________________________

TList* AtlEvent::GetMCNeutrinos(Bool_t sort) const {
    //
    // Get list of all MC true neutrino particles
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !! IMPORTANT: The user is responsible for deleting the !!
    // !! returned TList in order to avoid memory leaks       !!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *neutrinos = new TList;
    HepMCParticle *prt = 0;
    for ( Int_t i = 0; i < fN_MCParticles; i++ ) {
	prt = (HepMCParticle*)fMCParticles->At(i);
	if ( ((prt)->GetPdgCode()== 12) || ((prt)->GetPdgCode()== -12) || 
	     ((prt)->GetPdgCode()== 14) || ((prt)->GetPdgCode()== -14) ||
	     ((prt)->GetPdgCode()== 16) || ((prt)->GetPdgCode()== -16) ) 
	    neutrinos->Add(prt);
    }
    
    if ( sort ) neutrinos->Sort(kSortDescending);
    
    return neutrinos; 
}

//____________________________________________________________________

HepMCParticle* AtlEvent::GetLeadingMCNeutrino() const {
    //
    // Get MC truth neutrino with largest Pt
    //
    TList *neutrinos = GetMCNeutrinos();
    HepMCParticle *nu = 0;
    if ( neutrinos->GetEntries() > 0 )
	nu = (HepMCParticle*)neutrinos->At(0);
    delete neutrinos;
    return nu;
}

//____________________________________________________________________

void AtlEvent::PrintMCDaughters(HepMCParticle *prt) const {
    //
    // Print all daughters of given MC particle
    //
    if ( prt == 0 ) return;
    TRefArray *daughters = prt->GetDaughters();

    cout << endl;
    HepMCParticle::PrintHeader();
    prt->Print("nohead");
    cout << "   |" << endl
	 << "   |" << endl
	 << "   V" << endl;
    TIter next(daughters);
    while ( HepMCParticle *daughter = (HepMCParticle*)next() )
	daughter->Print("nohead");
    HepMCParticle::PrintFooter();
    cout << endl;
}

//____________________________________________________________________

void AtlEvent::PrintMCMothers(HepMCParticle *prt) const {
    //
    // Print all mother particles of given MC particle
    //
    if ( prt == 0 ) return;
    TRefArray *mothers = prt->GetMothers();

    cout << endl;
    HepMCParticle::PrintHeader();
    TIter next(mothers);
    while ( HepMCParticle *mother = (HepMCParticle*)next() )
	mother->Print("nohead");
    cout << "   |" << endl
	 << "   |" << endl
	 << "   V" << endl;
    prt->Print("nohead");
    HepMCParticle::PrintFooter();
    cout << endl;
}

//____________________________________________________________________

TList* AtlEvent::GetMCGenealogy(HepMCParticle *prt) const {
    //
    // Get list of all anchestors of given MC particle
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting the returned list     !!!
    // !!! (not the list elements!) afterwards to avoid memory leaks. !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *history = new TList;
    while ( prt != 0 ) {
	history->Add(prt);
	HepMCParticle *mother = prt->DaughterOf();
	prt = mother;
    }
    return history;
}

//____________________________________________________________________

void AtlEvent::PrintMCGenealogy(HepMCParticle *prt) const {
    //
    // Print all anchestors of given MC particle
    //

    // Get list of anchestors
    TList *history = GetMCGenealogy(prt);

    // Print list backwards
    cout << endl;
    HepMCParticle::PrintHeader();
    TIter last(history, kIterBackward);
    Bool_t first = kTRUE;
    HepMCParticle* p = 0;
    while ( (p = (HepMCParticle*)last()) ) {
	if ( first ) {
	    first = kFALSE;
	} else {
	    cout << "   |" << endl
		 << "   |" << endl
		 << "   V" << endl;
	}
	p->Print("nohead");
    }

    // Add info about all mother IDs if more than one
    if (prt->GetN_Mothers() > 1) {
	printf("        (mother IDs=");
	TIter next(prt->GetMothers());
	Bool_t first = kTRUE;
	while ( HepMCParticle* mother = (HepMCParticle*)next() ) {
	    if(!first) printf(",");
	    first = kFALSE;
	    printf("%-d", mother->GetId());
	}
	printf(")\n");
    }

    HepMCParticle::PrintFooter();
    cout << endl;

    // Clean up
    delete history;
}

//____________________________________________________________________

void AtlEvent::PrintMCGenealogyTree(HepMCParticle *prt, TString *padding,
                                    TList *CheckList) const {
    //
    // Helper function
    //
    // Print full tree of daughters, grand-daughters, ... for given particle
    // with the exception of any particles in check list
    // (this is to prevent from double-printing)
    //
    //
    // GINUS denotes (in this order):
    //
    // IsGenerator()
    // IsGenInteracting()
    // IsGenNonInteracting()
    // IsGenSimulStable()
    // IsGenStable()
    //

    Bool_t cleanup = kFALSE;

    // Create check list if not exists already.
    // In this case the list must be removed at the end
    if ( CheckList == 0 ) {
	CheckList = new TList;
	cleanup = kTRUE;
    }

    // Check if particle was already printed
    if ( CheckList->FindObject(prt) == 0 ) {
	CheckList->Add(prt);
	TString *padding_new = new TString(padding->Data());

	// Print particle information
	printf("%-s (ID=%-d, E=%-.2fGeV, Theta=%-.fdeg, Phi=%-.fdeg, GINUS=%-d%-d%-d%-d%-d",
	       prt->GetPdgName(), prt->GetId(),
	       prt->E(), prt->Theta()*180/TMath::Pi(),
	       prt->Phi()*180/TMath::Pi(),
	       prt->IsGenerator(),
	       prt->IsGenInteracting(),
	       prt->IsGenNonInteracting(),
	       prt->IsGenSimulStable(),
	       prt->IsGenStable());

	if (prt->GetN_Mothers() > 1) {
	    //
	    // If there is more than one mother print all
	    // mother Ids of the particle
	    //
	    printf(", mother IDs=");
	    TIter next(prt->GetMothers());
	    Bool_t first = kTRUE;
	    while ( HepMCParticle* mother = (HepMCParticle*)next() ) {
		if(!first) printf(",");
		first = kFALSE;
		printf("%-d", mother->GetId());
	    }
	}

	printf(")\n");

	// Get list of daughters
	TRefArray *daughters = prt->GetDaughters();
	if ( daughters->GetEntries() > 0 ) padding_new->Append("|       ");

	// Loop over all daughters and print them in a recursive way
	TIter next(daughters);
	while ( HepMCParticle *daughter = (HepMCParticle*)next() ) {
	    if ( daughter == daughters->Last() )
		padding_new->Replace(padding_new->Length()-8, 1, " ");
	    cout << padding->Data() << "|" << endl;
	    cout << padding->Data() << "+----- ";
	    PrintMCGenealogyTree(daughter, padding_new, CheckList);
	}
	if ( daughters->GetSize() > 0 )
	    cout << padding->Data() << endl;
	delete padding_new;
    }

    // Clean-up
    if ( cleanup ) delete CheckList;
}

//____________________________________________________________________

void AtlEvent::PrintMCGenealogyTree(Int_t First, Int_t Last) const {
    //
    // Print genealogical tree of all MC particles within
    // given range of IDs
    //
    // !!! Warning: Output might be huge !!!
    //
    TList *CheckList = new TList;
    TString *padding = new TString("");

    // Validate range
    if ( First < 1 ) First = 1;
    if ( Last > fN_MCParticles ) Last = fN_MCParticles;
    if ( Last < First ) { Int_t tmp = First; First = Last; Last = tmp; }

    // Loop over all particles
    cout << endl;
    for ( Int_t i = First-1; i < Last; i++ ) {
	HepMCParticle *prt = (HepMCParticle*)fMCParticles->At(i);
	PrintMCGenealogyTree(prt, padding, CheckList);
    }

    // Clean-up
    delete CheckList;
    delete padding;
}

//____________________________________________________________________

AtlJet* AtlEvent::GetLeadingJet(AtlJet::EType type) const {
    //
    // Get jet of given type with largest Et
    //
    if ( GetN_Jets(type) < 1 ) {
	Warning("GetLeadingJet",
		"No jets of type \"%s\" in this event!",
		AtlJet::NameOfType(type));
	return 0;
    }
    TClonesArray *jets = GetJets(type);
    if ( !jets->IsSorted() ) jets->Sort();
    // Note that currently TCloneArrays can only be sorted in
    // ascending order :(
    return (AtlJet*)jets->Last();
}

//____________________________________________________________________

Int_t AtlEvent::GetN_Jets(AtlJet::EType type) const {
  //
  // Return no. of jets for given type
  //
  switch ( type ) {
  case AtlJet::kCone4H1Tower:
    return fN_Cone4H1TowerJets;
  case AtlJet::kCone7H1Tower:
    return fN_Cone7H1TowerJets;
  case AtlJet::kCone4H1Topo:
    return fN_Cone4H1TopoJets;
  case AtlJet::kCone7H1Topo:
    return fN_Cone7H1TopoJets;
  case AtlJet::kMCCone4Hadron:
    return fN_MCCone4HadronJets;
  case AtlJet::kMCCone7Hadron:
    return fN_MCCone7HadronJets;
  case AtlJet::kMCAntiKt4Hadron:
    return fN_MCAntiKt4HadronJets;
  case AtlJet::kMCAntiKt6Hadron:
    return fN_MCAntiKt6HadronJets;
  case AtlJet::kMCAntiKt4HadronPileup:
    return fN_MCAntiKt4HadronPileupJets;
  case AtlJet::kAntiKt4H1Topo:
    return fN_AntiKt4H1TopoJets;
  case AtlJet::kAntiKt4H1Tower:
    return fN_AntiKt4H1TowerJets;
  case AtlJet::kAntiKt6H1Tower:
    return fN_AntiKt6H1TowerJets;
  case AtlJet::kAntiKt4TopoEM:
    return fN_AntiKt4TopoEMJets;
  case AtlJet::kAntiKt4TopoEMJES:
    return fN_AntiKt4TopoEMJESJets;
  case AtlJet::kAntiKt4LCTopo:
    return fN_AntiKt4LCTopoJets;
  case AtlJet::kAntiKt4Tower:
    return fN_AntiKt4TowerJets;
  case AtlJet::kAntiKt6TopoEM:
    return fN_AntiKt6TopoEMJets;
  case AtlJet::kAntiKt4Topo:
    return fN_AntiKt4TopoJets;
  case AtlJet::kAntiKt6Tower:
    return fN_AntiKt6TowerJets;
  case AtlJet::kAntiKt6LCTopo:
    return fN_AntiKt6LCTopoJets;
  case AtlJet::kAntiKt6Topo:
    return fN_AntiKt6TopoJets;
  case AtlJet::kAtlFast:
    return fN_AtlFastJets;
  case AtlJet::kDL1r:
  	return fN_DL1rJets;
  default:
    Error("Get_NJets", "Invalid jet type given. Abort!");
    gSystem->Abort(0);
  }
  return 0;
}

//____________________________________________________________________

void AtlEvent::SetN_Jets(AtlJet::EType type, Int_t NJets) {
  //
  // Set no. of jets for given type
  //
  switch ( type ) {
  case AtlJet::kCone4H1Tower:
    fN_Cone4H1TowerJets = NJets;
    break;
  case AtlJet::kCone7H1Tower:
    fN_Cone7H1TowerJets = NJets;
    break;
  case AtlJet::kCone4H1Topo:
    fN_Cone4H1TopoJets = NJets;
    break;
  case AtlJet::kCone7H1Topo:
    fN_Cone7H1TopoJets = NJets;
    break;
  case AtlJet::kMCCone4Hadron:
    fN_MCCone4HadronJets = NJets;
    break;
  case AtlJet::kMCCone7Hadron:
    fN_MCCone7HadronJets = NJets;
    break;
  case AtlJet::kMCAntiKt4Hadron:
    fN_MCAntiKt4HadronJets = NJets;
    break;
  case AtlJet::kMCAntiKt6Hadron:
    fN_MCAntiKt6HadronJets = NJets;
    break;
  case AtlJet::kMCAntiKt4HadronPileup:
    fN_MCAntiKt4HadronPileupJets = NJets;
    break;
  case AtlJet::kAntiKt4H1Topo:
    fN_AntiKt4H1TopoJets = NJets;
    break;
  case AtlJet::kAntiKt4H1Tower:
    fN_AntiKt4H1TowerJets = NJets;
    break;
  case AtlJet::kAntiKt6H1Tower:
    fN_AntiKt6H1TowerJets = NJets;
    break;
  case AtlJet::kAntiKt4TopoEM:
    fN_AntiKt4TopoEMJets = NJets;
    break;
    break;
  case AtlJet::kAntiKt4TopoEMJES:
    fN_AntiKt4TopoEMJESJets = NJets;
    break;
  case AtlJet::kAntiKt4LCTopo:
    fN_AntiKt4LCTopoJets = NJets;
    break;
  case AtlJet::kAntiKt4Tower:
    fN_AntiKt4TowerJets = NJets;
    break;
  case AtlJet::kAntiKt6TopoEM:
    fN_AntiKt6TopoEMJets = NJets;
    break;
  case AtlJet::kAntiKt4Topo:
    fN_AntiKt4TopoJets = NJets;
    break;
  case AtlJet::kAntiKt6Tower:
    fN_AntiKt6TowerJets = NJets;
    break;
  case AtlJet::kAntiKt6LCTopo:
    fN_AntiKt6LCTopoJets = NJets;
    break;
  case AtlJet::kAntiKt6Topo:
    fN_AntiKt6TopoJets = NJets;
    break;
  case AtlJet::kAtlFast:
    fN_AtlFastJets = NJets;
    break;
  case AtlJet::kDL1r:
	fN_DL1rJets = NJets;
    break;
  default:

    Error("Set_NJets", "Invalid jet type given. Abort!");
    gSystem->Abort(0);
  }
}

//____________________________________________________________________

TClonesArray* AtlEvent::GetJets(AtlJet::EType type) const {
  //
  // Return all jets of given type
  //
  switch ( type ) {
  case AtlJet::kCone4H1Tower:
    return fCone4H1TowerJets;
  case AtlJet::kCone7H1Tower:
    return fCone7H1TowerJets;
  case AtlJet::kCone4H1Topo:
    return fCone4H1TopoJets;
  case AtlJet::kCone7H1Topo:
    return fCone7H1TopoJets;
  case AtlJet::kMCCone4Hadron:
    return fMCCone4HadronJets;
  case AtlJet::kMCCone7Hadron:
    return fMCCone7HadronJets;
  case AtlJet::kMCAntiKt4Hadron:
    return fMCAntiKt4HadronJets;
  case AtlJet::kMCAntiKt6Hadron:
    return fMCAntiKt6HadronJets;
  case AtlJet::kMCAntiKt4HadronPileup:
    return fMCAntiKt4HadronPileupJets;
  case AtlJet::kAntiKt4H1Topo:
    return fAntiKt4H1TopoJets;
  case AtlJet::kAntiKt4H1Tower:
    return fAntiKt4H1TowerJets;
  case AtlJet::kAntiKt6H1Tower:
    return fAntiKt6H1TowerJets;
  case AtlJet::kAntiKt4TopoEM:
    return fAntiKt4TopoEMJets;
  case AtlJet::kAntiKt4TopoEMJES:
    return fAntiKt4TopoEMJESJets;
  case AtlJet::kAntiKt4LCTopo:
    return fAntiKt4LCTopoJets;
  case AtlJet::kAntiKt4Tower:
    return fAntiKt4TowerJets;
  case AtlJet::kAntiKt6TopoEM:
    return fAntiKt6TopoEMJets;
  case AtlJet::kAntiKt4Topo:
    return fAntiKt4TopoJets;
  case AtlJet::kAntiKt6Tower:
    return fAntiKt6TowerJets;
  case AtlJet::kAntiKt6LCTopo:
    return fAntiKt6LCTopoJets;
  case AtlJet::kAntiKt6Topo:
    return fAntiKt6TopoJets;  
  case AtlJet::kDL1r:
    return fDL1rJets;
  case AtlJet::kAtlFast:
    return fAtlFastJets;
  default:
    Error("GetJets", "Invalid jet type given. Abort!");
    gSystem->Abort(0);
  }
  return 0;
}

//____________________________________________________________________

TList* AtlEvent::GetElectrons(AtlElectron::EAuthor author,
			      AtlEMShower::EIsEM IsEM, Float_t Pt_min,
			      Float_t Pt_max, Float_t Eta_min,
			      Float_t Eta_max, Float_t EtCone20_max,
			      Bool_t sort, Bool_t exclude_crack,
    			      Float_t EtCone20_IsoFactor,
			      Bool_t use_cluster_eta, 
			      AtlEMShower::ECaloIsoCorrection CaloIsoCorrection) {
    //
    // Returns a list of electrons matching the given conditions.
    //
    // The author variable indicates the reconstruction type for the
    // electrons. Combinations are possible. For details see the
    // AtlElectron class.
    // With the help of the IsEM type one can search for loose, medium
    // or tight electrons.
    // The list is sorted in descending order of Pt by default.
    // Pt limits are given in GeV.
    // The list can be composed of electrons excluding the crack region
    // of the EM calorimeter (default) or including it.
    // If the isolation requirement EtCone20 should be modified by a
    // pt-dependence like EtCone20 + factor*Pt, then the factor can be
    // given as EtCone20_IsoFactor.
    // If the use of cluster eta is desired for the eta cut, then set
    // use_cluster_eta to kTRUE.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *electrons = new TList();
    AtlElectron *el = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    Float_t eta_abs = 0.;
    for ( Int_t i = 0; i < fN_Electrons; i++ ) {
	el = (AtlElectron*)fElectrons->At(i);
	if ( (el->GetAuthor() & author) && (el->IsEM(IsEM))
	     && (el->GetEtCone20(CaloIsoCorrection) <= (EtCone20_max + EtCone20_IsoFactor*el->Pt())) ) {
	    Pt  = el->Pt();
	    if ( use_cluster_eta ){
		  eta = el->ClusterEta();
	    }
	    else{
		  eta = el->Eta();
        }
	    eta_abs = TMath::Abs(eta);
	    if ( Pt > Pt_min && Pt < Pt_max ) {
		  if ( eta > Eta_min && eta < Eta_max ) {
		    if ( exclude_crack ) {
		      if ( !((1.37 < eta_abs) && (eta_abs < 1.52)) ){
		        electrons->Add(el);
		      }
		    }
		    else{
		  	  electrons->Add(el);
		    }
		  }
	    }
	}
    }
    
    if ( sort ) electrons->Sort(kSortDescending);
    
    return electrons;
}

//____________________________________________________________________


TList* AtlEvent::GetVtxTracks(Float_t Chi2ovNDoF_max, Float_t Pt_min, Float_t Pt_max,
			      Float_t Eta_min, Float_t Eta_max, Bool_t sort) {
    //
    // Returns a list of tracks matching the given conditions.
    // Pt limits are given in GeV.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *tracks = new TList;
    AtlIDTrack *trk = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    
    for ( Int_t i = 0; i < fN_IDTracks; i++ ) {
	trk = (AtlIDTrack*)fIDTracks->At(i);
	if ( trk->IsVtxTrack() ) {
	    Pt  = trk->Pt();
	    eta = TMath::Abs(trk->Eta());
	    if ( Pt >= Pt_min && Pt <= Pt_max ) {
		if ( eta >= Eta_min && eta <= Eta_max ) {
		    if (  trk->Chi2ovNDoF() <= Chi2ovNDoF_max )  tracks->Add(trk);
		}
	    }
	}
    }

    if ( sort ) tracks->Sort(kSortDescending);
    return tracks;
}

//____________________________________________________________________


TList* AtlEvent::GetSecVtxTracks(Float_t Chi2ovDoF_max, Float_t Pt_min, Float_t Pt_max,
				 Float_t Eta_min, Float_t Eta_max, Bool_t sort) {
    //
    // Returns a list of tracks matching the given conditions.
    // Pt limits are given in GeV.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *tracks = new TList;
    AtlIDTrack *trk = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    
    for ( Int_t i = 0; i < fN_IDTracks; i++ ) {
	trk = (AtlIDTrack*)fIDTracks->At(i);
	if ( trk->IsSecondaryVtxTrack() ) {
	    Pt  = trk->Pt();
	    eta = TMath::Abs(trk->Eta());
	    if ( Pt >= Pt_min && Pt <= Pt_max ) {
		if ( eta >= Eta_min && eta <= Eta_max ) {
		    if ( (trk->Chi2() / trk->NDoF() ) <= Chi2ovDoF_max )  tracks->Add(trk);
		}
	    }
	}
    }

    if ( sort ) tracks->Sort(kSortDescending);
    return tracks;
}

//____________________________________________________________________


TList* AtlEvent::GetPrimVtxTracks(Float_t Chi2ovDoF_max, Float_t Pt_min, Float_t Pt_max,
				 Float_t Eta_min, Float_t Eta_max, Bool_t sort) {
    //
    // Returns a list of tracks matching the given conditions.
    // Pt limits are given in GeV.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *tracks = new TList;
    AtlIDTrack *trk = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;

    for ( Int_t i = 0; i < fN_IDTracks; i++ ) {
	trk = (AtlIDTrack*)fIDTracks->At(i);
	if (trk->IsPrimaryVtxTrack() ) {
	    Pt  = trk->Pt();
	    eta = TMath::Abs(trk->Eta());
	    if ( Pt >= Pt_min && Pt <= Pt_max ) {
		if ( eta >= Eta_min && eta <= Eta_max ) {
		    if ( (trk->Chi2() / trk->NDoF() ) <= Chi2ovDoF_max )  tracks->Add(trk);
		}
	    }
	}
    }

    if ( sort ) tracks->Sort(kSortDescending);
    return tracks;
}

//____________________________________________________________________


TList* AtlEvent::GetPhotons(AtlPhoton::EAuthor author,
			    AtlEMShower::EIsEM IsEM, Float_t Pt_min,
			    Float_t Pt_max, Float_t Eta_min,
			    Float_t Eta_max, Float_t EtCone20_max,
			    Bool_t sort, Bool_t exclude_crack, 
			    AtlEMShower::ECaloIsoCorrection CaloIsoCorrection) {
    //
    // Returns a list of photons matching the given conditions.
    //
    // The author variable indicates the reconstruction type for the
    // photons. Combinations are possible. For details see the
    // AtlPhoton class.
    // With the help of the IsEM type one can search for loose, medium
    // or tight photons.
    // The list is sorted in descending order of Pt by default.
    // Pt limits are given in GeV
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *photons = new TList;
    AtlPhoton *ph = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    Float_t eta_abs = 0.;
    for ( Int_t i = 0; i < fN_Photons; i++ ) {
	ph = (AtlPhoton*)fPhotons->At(i);
	if ( (ph->GetAuthor() & author) && (ph->IsEM(IsEM))
	     && ph->GetEtCone20(CaloIsoCorrection) <= EtCone20_max ) {
	    Pt  = ph->Pt();
	    eta = ph->Eta();
	    eta_abs = TMath::Abs(eta);
	    if ( Pt >= Pt_min && Pt <= Pt_max ) {
		if ( eta >= Eta_min && eta <= Eta_max ) {
		    if ( exclude_crack ) {
			if ( (eta_abs <= 1.37) || (eta_abs >= 1.52) ) photons->Add(ph);
		    }
		    else{
			photons->Add(ph);
		    }
		}
	    }
	}
    }
    if ( sort ) photons->Sort(kSortDescending);

    return photons;
}

//____________________________________________________________________

TList* AtlEvent::GetMuons(AtlMuon::EAuthor author, Float_t Pt_min,
			  Float_t Pt_max, Float_t Eta_min, Float_t Eta_max,
			  Float_t Chi2_max, Float_t EtCone20_max,
			  Bool_t staco_combined, Bool_t sort,
			  AtlMuon::EQuality quality) {
    //
    // Returns a list of muons matching the given conditions.
    //
    // The author variable indicates the reconstruction type for the
    // muons. Combinations are possible. For details see the AtlMuon
    // class.
    // The Chi2_max value indicates the upper allowed limit
    // for the chi2/ndof of the track-segment matching of the muons.
    // Per default, muons that are combined STACO are chosen now
    // (option combined_staco).
    // The list is sorted in descending order of Pt by default.
    // Pt limits are given in GeV
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *muons = new TList;
    AtlMuon *mu = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    for ( Int_t i = 0; i < fN_Muons; i++ ) {
	mu = (AtlMuon*)fMuons->At(i);
	if ( mu->GetAuthor() & author
	     && mu->GetQuality() & quality
	     && mu->GetMatchingChi2overNDoF() <= Chi2_max
	     && mu->GetEtCone20() <= EtCone20_max ) {
	    Pt  = mu->Pt();
	    eta = mu->Eta();
	    if ( Pt >= Pt_min && Pt < Pt_max ) {
		if ( eta >= Eta_min && eta < Eta_max ) {
		  if ( staco_combined && mu->IsCombinedMuon() )
		    muons->Add(mu);
		  if ( !staco_combined )
		    muons->Add(mu);
		}
	    }
	}
    }
    if ( sort ) muons->Sort(kSortDescending);
    
    return muons;
}

//____________________________________________________________________

TList* AtlEvent::GetJets(AtlJet::EType type, Float_t Et_min,
			  Float_t Et_max, Float_t Eta_min, Float_t Eta_max,
			 Bool_t is_good, Bool_t sort, Bool_t remove_faked) {
    //
    // Returns a list of jets matching the given conditions.
    //
    // The list is sorted in descending order of Et by default.
    // Et limits are given in GeV.
    // If is_good = kTRUE, only good jets will be accepted.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *jets = new TList;
    AtlJet *jet = 0;
    Float_t Et  = 0.;
    Float_t eta = 0.;

    for ( Int_t i = 0; i < GetN_Jets(type); i++ ) {
	jet = (AtlJet*)GetJets(type)->At(i);
	
	// test whether is good jet?
	if ( is_good ) {
	    if ( !jet->IsGoodJet() ) continue;
	}
	
	Et  = jet->Et();
	eta = jet->Eta();
	if ( Et >= Et_min && Et <= Et_max ) {
	    if ( eta >= Eta_min && eta <= Eta_max ) {
		if ( remove_faked && jet->IsFaked() ) {
		    continue;
		}
		else {
		    jets->Add(jet);
		}
	    }
	}
    }
    if ( sort ) jets->Sort(kSortDescending);

    return jets;
}

//____________________________________________________________________

TList* AtlEvent::GetTaus(AtlTau::EAuthor author,
			 AtlTau::ETauFlag flag, Float_t Pt_min,
			 Float_t Pt_max, Float_t Eta_min,
			 Float_t Eta_max, Bool_t sort) {
    //
    // Returns a list of taus matching the given conditions.
    //
    // The author variable indicates the reconstruction type for the
    // taus. Combinations are possible. For details see the AtlTau
    // class. With the help of the TauFlag a quality cut of the taus
    // is possible.
    // The list is sorted in descending order of Pt by default.
    // Pt limits are given in GeV
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *taus = new TList;
    AtlTau *tau = 0;
    Float_t Pt  = 0.;
    Float_t eta = 0.;
    for ( Int_t i = 0; i < fN_Taus; i++ ) {
	tau = (AtlTau*)fTaus->At(i);
	if ( (tau->GetAuthor() & author) && (tau->GetTauFlag() & flag) ) {
	    Pt  = tau->Pt();
	    eta = tau->Eta();
	    if ( Pt >= Pt_min && Pt <= Pt_max ) {
		if ( eta >= Eta_min && eta <= Eta_max ) 
		    taus->Add(tau);
	    }
	}
    }
    if ( sort ) taus->Sort(kSortDescending);

    return taus;
}

//____________________________________________________________________

TList* AtlEvent::GetStableMCParticles() const {
    // 
    // Returns list of HepMCParticle objects characterized by
    // IsGenStable() = 1 , meaning that they either
    // - escape the detector
    //   or
    // - are detected.
    // 1st Purpose: feeding the FindMatchedParticles function 
    // with a more sensible list in case of MC particles.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *particles = new TList;
    HepMCParticle *prt = 0;
    for ( Int_t i = 0; i < GetN_MCParticles(); i++ ) {
	prt = (HepMCParticle*)GetMCParticles()->At(i);
	if ( prt->IsGenStable() ) particles->Add(prt);
    } 
    return particles;
}

//____________________________________________________________________

TList* AtlEvent::FindMCParticlesByName(const char* PdgName,
				       Bool_t RemoveDouble) {
    //
    // Returns list of particles with given type
    // Since the MC particles generation operates at following steps, 
    // there are many identical particles that 
    // either - propagate without changing their kinematics 
    //     or - are just deviated by under-threshold interactions
    // being anyway unuseful (and misleading) in a MCTruth Plotting 
    // context.
    // The RemoveDouble option is used to remove the same particles 
    // from the list
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    Int_t PdgCode = HepDatabasePDG::Instance()->GetParticle(PdgName)->PdgCode();
    TList *particles = new TList;
    HepMCParticle *prt = 0;
    for ( Int_t i = 0; i < GetN_MCParticles(); i++ ) {
	prt = (HepMCParticle*)GetMCParticles()->At(i);
	if ( prt->GetPdgCode() != PdgCode ) continue;
	if ( RemoveDouble && prt->GetN_Daughters() == 1 ) {
	    if ( ((HepMCParticle*)prt->GetDaughters()->First())
		 ->GetPdgCode() == PdgCode ) continue;
	}
	particles->Add(prt);
    }
    return particles;
}

//____________________________________________________________________

TList* AtlEvent::FindOverlaps_ElectronCluster_Jets(AtlElectron *el,
				  TCollection *search_list,
				  Float_t DeltaR) {
    //
    // Returns a list of jets which overlap with the given  
    // electron by using the variable DeltaR, computed by usage
    // of the cluster eta and phi of the electron (default 
    // value DeltaR = 0.1).
    // The user must give the sample of objects (for example HepJets, 
    // AtlJets, ...).
    // The returned list is sorted in order of increasing DeltaR.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_overlapping   = new TList; 
    TList *unsorted_overlapping = new TList;
    
    // Reject all particles not surviving the selection cuts
    TIter next_jet(search_list);
    HepJet *jet_cmp = 0;
    Float_t DeltaR_cur = 0.;
    TArrayF *dist = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (jet_cmp = (HepJet*)next_jet()) ) {
	DeltaR_cur = el->ClusterDeltaR(jet_cmp);
      if ( (DeltaR_cur < DeltaR) ) {
	  unsorted_overlapping->Add(jet_cmp);
	  dist->AddAt(DeltaR_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, dist->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_overlapping->Add(unsorted_overlapping->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_overlapping;
    delete dist;
    delete index;
    
    return sorted_overlapping;
}

//____________________________________________________________________

TList* AtlEvent::FindOverlaps_ElectronTrack_EMScaleJets(AtlElectron *el,
							TCollection *search_list,
							Float_t DeltaR,
							const char* option) {
    //
    // Returns a list of jets which overlap with the given  
    // electron by using the variable DeltaR, optionally computed
    // by usage of the ID-track eta and phi of the electron (default 
    // value DeltaR = 0.1) and the jet detector positions
    // (eta_EMScale+EMJES_EtaCorr and phi_EMSCALE).
    // The user must give the sample of objects (MUST BE AtlJet!).
    // The returned list is sorted in order of increasing DeltaR.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_overlapping   = new TList; 
    TList *unsorted_overlapping = new TList;
    
    // Reject all particles not surviving the selection cuts
    TIter next_jet(search_list);
    AtlJet *jet_cmp = 0;
    Float_t DeltaR_cur = 0.;
    TArrayF *dist = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    
    while( (jet_cmp = (AtlJet*)next_jet()) ) {
	DeltaR_cur = jet_cmp->EMScaleDeltaR(el, option);
      if ( (DeltaR_cur < DeltaR) ) {
	  unsorted_overlapping->Add(jet_cmp);
	  dist->AddAt(DeltaR_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, dist->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_overlapping->Add(unsorted_overlapping->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_overlapping;
    delete dist;
    delete index;
    
    return sorted_overlapping;
}


//____________________________________________________________________

TList* AtlEvent::FindOverlaps_MuonTrack_EMScaleJets(AtlMuon *mu,
				  TCollection *search_list,
				  Float_t DeltaR) {
    //
    // Returns a list of jets which overlap with the given  
    // muon by using the variable DeltaR (default 
    // value DeltaR = 0.1), computed by usage of the jet
    // detector positions (eta_EMScale+EMJES_EtaCorr and phi_EMSCALE).
    // The user must give the sample of objects (MUST BE AtlJet!).
    // The returned list is sorted in order of increasing DeltaR.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_overlapping   = new TList; 
    TList *unsorted_overlapping = new TList;
    
    // Reject all particles not surviving the selection cuts
    TIter next_jet(search_list);
    AtlJet *jet_cmp = 0;
    Float_t DeltaR_cur = 0.;
    TArrayF *dist = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    const char* option = "trk";
    
    while( (jet_cmp = (AtlJet*)next_jet()) ) {
	DeltaR_cur = jet_cmp->EMScaleDeltaR(mu, option);
      if ( (DeltaR_cur < DeltaR) ) {
	  unsorted_overlapping->Add(jet_cmp);
	  dist->AddAt(DeltaR_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, dist->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_overlapping->Add(unsorted_overlapping->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_overlapping;
    delete dist;
    delete index;
    
    return sorted_overlapping;
}


//____________________________________________________________________


TList* AtlEvent::FindOverlaps_Particle_Jets(HepParticle *prt,
					    TCollection *search_list,
					    Float_t DeltaR) {
    //
    // Returns a list of jets which overlap with the given  
    // HepParticle object by using the variable DeltaR, default 
    // value DeltaR = 0.1.
    // The user must give the sample of objects (for example HepJets, 
    // AtlJets, ...).
    // The returned list is sorted in order of increasing DeltaR.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_overlapping   = new TList; 
    TList *unsorted_overlapping = new TList;
    
    // Reject all particles not surviving the selection cuts
    TIter next_jet(search_list);
    HepJet *jet_cmp = 0;
    Float_t DeltaR_cur = 0.;
    TArrayF *dist = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (jet_cmp = (HepJet*)next_jet()) ) {
      DeltaR_cur = prt->DeltaR(jet_cmp);
      if ( (DeltaR_cur < DeltaR) ) {
	  unsorted_overlapping->Add(jet_cmp);
	  dist->AddAt(DeltaR_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, dist->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_overlapping->Add(unsorted_overlapping->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_overlapping;
    delete dist;
    delete index;
    
    return sorted_overlapping;
}



//____________________________________________________________________

TList* AtlEvent::FindMatchedParticles(HepParticle *prt,
				       TCollection *search_list,
				       Float_t DeltaR,
				       Float_t DeltaPtFrac) {
    //
    // Returns a list of particles which best match the given HepParticle 
    // object by using the variables DeltaR, default value DeltaR = 0.1.
    // and DeltaPtFrac, also default of 0.1.  
    // The user must provide a list of objects (for example HepMCParticles, 
    // AtlElectrons, ...).
    // For feeding it with a list of more sensible MC objects please use 
    // the GetStableMCParticles function at line 1397. It bypasses the 
    // compilation issues (impossible casting) encountered for a "Remove" 
    // option (e.g. in lines 1497-1499 of AtlEvent:article)  
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_matched = new TList; 
    TList *unsorted_matched = new TList;
    
    // Reject all particle not surviving the selection cuts
    // Compute Chi2 = dR^2 + dPt_frac^2 for the ones surviving the cuts
    TIter next_prt(search_list);
    HepParticle *prt_cmp = 0;
    Float_t DeltaPtFrac_cur = 0.;
    Float_t DeltaR_cur = 0.;
    TArrayF *chi2 = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (prt_cmp = (HepParticle*)next_prt()) ) {
      DeltaPtFrac_cur = TMath::Abs(prt->DeltaPtFrac(prt_cmp));
      DeltaR_cur = prt->DeltaR(prt_cmp);
      if ( (DeltaR_cur < DeltaR) && (DeltaPtFrac_cur < DeltaPtFrac) ) {
	  unsorted_matched->Add(prt_cmp);
	  chi2->AddAt(DeltaR_cur*DeltaR_cur + DeltaPtFrac_cur*DeltaPtFrac_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, chi2->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_matched->Add(unsorted_matched->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_matched;
    delete chi2;
    delete index;
    
    return sorted_matched;
}

//____________________________________________________________________

TList* AtlEvent::FindMatchedParticles(HepJet *jet,
				       TCollection *search_list,
				       Float_t DeltaR,
				       Float_t DeltaPtFrac) {
    //
    // Returns a list of jets which best match the given HepJet
    // object by using the variable DeltaR, default value DeltaR = 0.2.
    // and DeltaPtFrac, default 0.2  
    // The user must give the sample of objects (for examples HepMCParticles, 
    // AtlElectrons, ...)
    // For feeding it with a sample of more sensible MC objects please use 
    // the GetStableMCParticles function at line 1397. It bypasses the 
    // compilation issues (impossible casting) encountered for a "Remove" 
    // option (e.g. in lines 1497-1499 of AtlEvent::FindMatchedMCParticle)  
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_matched = new TList; 
    TList *unsorted_matched = new TList;
    
    // Reject all particle not surviving the selection cuts
    // Compute Chi2 = dR^2 + dPt_frac^2 for the ones surviving the cuts
    TIter next_prt(search_list);
    HepParticle *prt_cmp = 0;
    Float_t DeltaPtFrac_cur = 0.;
    Float_t DeltaR_cur = 0.;
    TArrayF *chi2 = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (prt_cmp = (HepParticle*)next_prt()) ) {
      DeltaPtFrac_cur = TMath::Abs(jet->DeltaPtFrac(prt_cmp));
      DeltaR_cur = jet->DeltaR(prt_cmp);
      if ( (DeltaR_cur < DeltaR) && (DeltaPtFrac_cur < DeltaPtFrac) ) {
	  unsorted_matched->Add(prt_cmp);
	  chi2->AddAt(DeltaR_cur*DeltaR_cur + DeltaPtFrac_cur*DeltaPtFrac_cur, i++);
      }
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, chi2->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_matched->Add(unsorted_matched->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_matched;
    delete chi2;
    delete index;
    
    return sorted_matched;
}

//____________________________________________________________________

TList* AtlEvent::FindTruthMatchedJets(AtlJet::EType type) {
    //
    // Returns a list of truth-matched jets of the requested type.
    // For each jet type the search list is simply the list of all
    // jets of this type in the event.
    // The value for DeltaR is set according to the (cone-)radius used
    // in the jet definition of this type. There is no PtFrac requirement
    // used for the truth-matching here. All MC particles are considered.
    // Default is set to jet type AntiKt4H1Topo.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    Float_t DeltaR = 0;
    Float_t DeltaPtFrac = 0;
    
    TList* jets = new TList();
    
    switch ( type ) {
	case AtlJet::kCone4H1Tower :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fCone4H1TowerJets);
	    break;
	case AtlJet::kCone7H1Tower :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fCone7H1TowerJets);
	    break;
	case AtlJet::kCone4H1Topo :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fCone4H1TopoJets);
	    break;
	case AtlJet::kCone7H1Topo :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fCone7H1TopoJets);
	    break;
	case AtlJet::kAntiKt4H1Topo :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt4H1TopoJets);
	    break;
	case AtlJet::kAntiKt4TopoEM:
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt4TopoEMJets);
	    break;
	case AtlJet::kAntiKt4TopoEMJES:
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt4TopoEMJESJets);
	    break;
	case AtlJet::kAntiKt4H1Tower :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt4H1TowerJets);
	    break;
	case AtlJet::kAntiKt6H1Tower :
	    DeltaR = 0.6;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt6H1TowerJets);
	    break;
	case AtlJet::kMCCone4Hadron :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fMCCone4HadronJets);
	    break;
	case AtlJet::kMCCone7Hadron :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fMCCone7HadronJets);
	    break;
	case AtlJet::kMCAntiKt4Hadron :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fMCAntiKt4HadronJets);
	    break;
	default :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    jets->AddAll(fAntiKt4H1TopoJets);
    }

    TIter next_jet(jets);
    AtlJet* jet = 0;
    HepMCParticle* cone_prt = 0;
    TList* truth_matched_jets = new TList();
    
    while ( ( jet = (AtlJet*)next_jet() ) ) {
	
	// Das ist vl. nicht noetig, da jet->GetTruthParticles() existiert...
	// Uebereinstimmung pruefen!
	TList *cone_mc_particles =
	    FindMatchedParticles(jet,fMCParticles,DeltaR,DeltaPtFrac);
	// Alternativ (Athena Definition):
	// TList *cone_mc_particles = jet->GetTruthParticles();
	// .. bis hier
	
	TIter next_cone_prt(cone_mc_particles);
	
	while ( ( cone_prt = (HepMCParticle*)next_cone_prt() ) ) {
	    if ( cone_prt->GetN_Mothers() != 0 ) {
		if ( cone_prt->GetMother()->IsStringOrCluster() ) {
		    truth_matched_jets->Add(jet);
		    break;
		}
	    }
	}

	
	delete cone_mc_particles;
	
    }
    
    delete jets;
    
    return truth_matched_jets;
}

//____________________________________________________________________

TList* AtlEvent::FindTruthMatchedJets(TCollection *search_list,
				      AtlJet::EType type) {
    //
    // Returns a list of truth-matched jets of the requested type.
    // A search list must be provided by the user.
    // The value for DeltaR is set according to the (cone-)radius used
    // in the jet definition of this type. There is no PtFrac requirement
    // used for the truth-matching here. All MC particles are considered.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    Float_t DeltaR = 0;
    Float_t DeltaPtFrac = 0;
    
    
    switch ( type ) {
	case AtlJet::kCone4H1Tower :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kCone7H1Tower :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kCone4H1Topo :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kCone7H1Topo :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kAntiKt4H1Topo :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kAntiKt4TopoEM :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kAntiKt4TopoEMJES :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kAntiKt4H1Tower :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kAntiKt6H1Tower :
	    DeltaR = 0.6;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kMCCone4Hadron :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kMCCone7Hadron :
	    DeltaR = 0.7;
	    DeltaPtFrac = 10.e10;
	    break;
	case AtlJet::kMCAntiKt4Hadron :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
	    break;
	default :
	    DeltaR = 0.4;
	    DeltaPtFrac = 10.e10;
    }

    TIter next_jet(search_list);
    AtlJet* jet = 0;
    HepMCParticle* cone_prt = 0;
    TList* truth_matched_jets = new TList();
    
    while ( ( jet = (AtlJet*)next_jet() ) ) {
	
	// Das ist vl. nicht noetig, da jet->GetTruthParticles() existiert...
	// Uebereinstimmung pruefen!
	TList *cone_mc_particles =
	    FindMatchedParticles(jet,fMCParticles,DeltaR,DeltaPtFrac);
	// Alternativ (Athena Definition):
	// TList *cone_mc_particles = jet->GetTruthParticles();
	// .. bis hier
	
	TIter next_cone_prt(cone_mc_particles);
	
	while ( ( cone_prt = (HepMCParticle*)next_cone_prt() ) ) {
	    if ( cone_prt->GetN_Mothers() != 0 ) {
		if ( cone_prt->GetMother()->IsStringOrCluster() ) {
		    truth_matched_jets->Add(jet);
		    break;
		}
	    }
	}

	
	delete cone_mc_particles;
	
    }
    
    
    return truth_matched_jets;
}

//____________________________________________________________________

TList* AtlEvent::FindMatchedJets(HepParticle *prt,
				 TCollection *search_list,
				 Float_t DeltaR,
				 Float_t DeltaPtFrac,
				 Bool_t UseDeltaRonly) {
    //
    // Returns a list of jets which best match the given HepParticle
    // object by using the variable DeltaR, default value DeltaR =
    // 0.2. and DeltaEtFrac, also default of 0.2. The given search
    // list is the list of jets to search. The option UseDeltaRonly
    // (default=false) can be used to disable the DeltaPtFrac usage
    // completely.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_matched = new TList; 
    TList *unsorted_matched = new TList;
    
    // Reject all jets not surviving the selection cuts
    // Compute Chi2 = dR^2 + dPt_frac^2 for the ones surviving the cuts
    TIter next_jet(search_list);
    HepJet *jet_cmp = 0;
    Float_t DeltaPtFrac_cur = 0.;
    Float_t DeltaR_cur = 0.;
    TArrayF *chi2 = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (jet_cmp = (HepJet*)next_jet()) ) {
	DeltaPtFrac_cur = TMath::Abs(prt->DeltaPtFrac(jet_cmp));
	DeltaR_cur = prt->DeltaR(jet_cmp);
	if ( DeltaR_cur < DeltaR ) {
	    if ( UseDeltaRonly && (DeltaPtFrac_cur < DeltaPtFrac) ) {
		unsorted_matched->Add(jet_cmp);
		chi2->AddAt(DeltaR_cur*DeltaR_cur + DeltaPtFrac_cur*DeltaPtFrac_cur, i++);
	    } else if ( !UseDeltaRonly ) {
		unsorted_matched->Add(jet_cmp);
		chi2->AddAt(DeltaR_cur*DeltaR_cur, i++);
	    }
	}
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, chi2->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_matched->Add(unsorted_matched->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_matched;
    delete chi2;
    delete index;
    
    return sorted_matched;
}

//____________________________________________________________________

TList* AtlEvent::FindMatchedJets(HepJet *jet,
				 TCollection *search_list,
				 Float_t DeltaR,
				 Float_t DeltaEtFrac) {
    //
    // Returns a list of jets which best match the given HepJet
    // object by using the variable DeltaR, default value DeltaR = 0.2.
    // and DeltaEtFrac, also default of 0.2.
    // The user must give the sample of HepJet objects for comparison 
    // (HepJet or AtlJet).  It bypasses the compilation issues
    // (impossible casting) encountered for a "Remove" 
    // option (e.g. in lines 1497-1499 of AtlEvent::FindMatchedMCParticle)  
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sorted_matched = new TList; 
    TList *unsorted_matched = new TList;
    
    // Reject all jets not surviving the selection cuts
    // Compute Chi2 = dR^2 + dPt_frac^2 for the ones surviving the cuts
    TIter next_jet(search_list);
    HepJet *jet_cmp = 0;
    Float_t DeltaEtFrac_cur = 0.;
    Float_t DeltaR_cur = 0.;
    TArrayF *chi2 = new TArrayF(search_list->GetEntries());
    Int_t i = 0;
    while( (jet_cmp = (HepJet*)next_jet()) ) {
	DeltaEtFrac_cur = TMath::Abs(jet->DeltaEtFrac(jet_cmp));
	DeltaR_cur = jet->DeltaR(jet_cmp);
	if ( (DeltaR_cur < DeltaR) && (DeltaEtFrac_cur < DeltaEtFrac) ) {
	    unsorted_matched->Add(jet_cmp);
	    chi2->AddAt(DeltaR_cur*DeltaR_cur + DeltaEtFrac_cur*DeltaEtFrac_cur, i++);
	}
    }
    
    // Create index array
    TArrayI *index = new TArrayI(i);
    TMath::Sort(i, chi2->GetArray(), index->GetArray(), kFALSE);
    
    // Fill sorted list
    for ( Int_t j = 0; j < i; j++){
	sorted_matched->Add(unsorted_matched->At(index->At(j)));
    }
    
    // Clean-up
    delete unsorted_matched;
    delete chi2;
    delete index;
    
    return sorted_matched;
}

//____________________________________________________________________

HepMCParticle* AtlEvent::FindMatchedMCParticle(HepParticle *prt,
					       Bool_t RemoveUnstable,
					       Float_t DeltaR,
					       Float_t DeltaPtFrac) {
    //
    // Returns the MC particle which best matches the given HepParticle object
    // by using:
    //
    // 1) cone difference 
    //    - default value DeltaR = 0.1
    //
    // 2) percentual Pt difference normalized to the true particle's Pt 
    //    - default value DeltaPtFrac = 0.1
    //
    // "RemoveUnstable" option allows the user to search a MC list where 
    // unstable MC particles are not taken into account (e.g. clusters,
    // quarks which do not reach the detector). 
    //
    HepMCParticle *mc_prt = 0;
    TList *mc_particles = FindMatchedParticles(prt, GetMCParticles(),
					       DeltaR, DeltaPtFrac);
    if ( mc_particles->GetSize() == 0 ) ;
//	Error("FindMatchedMCParticle","No matching MC particles found.");
    else {
	Int_t z = 0;
	if ( RemoveUnstable ) {
	    TIter next_mcprt(mc_particles);
	    while ( (mc_prt = (HepMCParticle*)next_mcprt()) ) {
		if ( mc_prt->IsGenStable() ) break;
		if (z == 25){
		    mc_prt = 0;
		    Error("FindMatchedMCParticle","No GenStable MC particle found within given DeltaR and DeltaPtFrac amongst first 25 candidates that match best.");
		    break;
		}
		z++;   
	    }
	} else {
	    mc_prt = (HepMCParticle*)mc_particles->First();
	}
    }
    
    delete mc_particles;
    return mc_prt;
}

//____________________________________________________________________

HepMCParticle* AtlEvent::FindMatchedMCParticle(HepJet *jet,
					       Bool_t RemoveUnstable,
					       Float_t DeltaR,
					       Float_t DeltaPtFrac) {
    //
    // Returns the MC particle which best matches the given HepJet object
    // by using:
    //
    // 1) cone difference 
    //    - default value DeltaR = 0.2
    //
    // 2) percentual Pt difference normalized to the true particle's Pt 
    //    - default value DeltaPtFrac = 0.2
    //
    // "RemoveUnstable" option allows the user to search a MC list where 
    // unstable MC particles are not taken into account (e.g. clusters,
    // quarks which do not reach the detector).  
    //
    HepMCParticle *mc_prt = 0;
    TList *mc_particles = FindMatchedParticles(jet, GetMCParticles(),
					       DeltaR, DeltaPtFrac);

    if ( mc_particles->GetSize() == 0 ) ;
//	Error("FindMatchedMCParticle","No matching MC particles found.");
    else {
	Int_t z = 0;
	if ( RemoveUnstable ) {
	    TIter next_mcprt(mc_particles);
	    while ( (mc_prt = (HepMCParticle*)next_mcprt()) ) {
		if ( mc_prt->IsGenStable() ) break;
		if (z == 25){
		    mc_prt = 0;
		    Error("FindMatchedMCParticle","No GenStable MC particle found within given DeltaR and DeltaPtFrac amongst first 25 candidates that match best.");
		    break;
		}
		z++;	
	    }
	} else {
	    mc_prt = (HepMCParticle*)mc_particles->First();
	}
    }
    
    delete mc_particles;
    return mc_prt;
}

//____________________________________________________________________

HepJet* AtlEvent::FindMatchedMCJet(HepParticle *prt,
				   Float_t DeltaR,
				   Float_t DeltaPtFrac) {
  //
  // Returns the MC HL jet which best matches the given HepParticle
  // object by assuming JetType AntiKt4H1Topo and using:
  //
  // 1) cone difference 
  //    - default value DeltaR = 0.2
  //
  // 2) percentual Pt difference normalized to the true particle's Pt 
  //    - default value DeltaEtFrac = 0.2
  //
  TList *mc_jets = FindMatchedJets(prt, GetMCAntiKt4HadronJets(),
				   DeltaR, DeltaPtFrac);
  HepJet *mc_jet = (HepJet*)mc_jets->First();
  delete mc_jets;
  return mc_jet;
}


//____________________________________________________________________

HepJet* AtlEvent::FindMatchedMCJet(HepJet *jet,
				   Float_t DeltaR,
				   Float_t DeltaEtFrac) {
  //
  // Returns the MC HL jet which best matches the given HepJet object
  // by assuming JetType AntiKt4H1Topo and using:
  //
  // 1) cone difference 
  //    - default value DeltaR = 0.2
  //
  // 2) percentual Pt difference normalized to the true particle's Pt 
  //    - default value DeltaEtFrac = 0.2
  //
  TList *mc_jets = FindMatchedJets(jet, GetMCAntiKt4HadronJets(),
				   DeltaR, DeltaEtFrac);
  HepJet *mc_jet = (HepJet*)mc_jets->First();
  delete mc_jets;
  return mc_jet;
}

//____________________________________________________________________

Float_t AtlEvent::W_Mperp2(HepParticle *lepton, TVector2 MissingEt) const {
    //
    // Mt_W^2 = 2*Et_lepton*Et_miss*(1 - cos(DeltaPhi)) which is
    // equivalent to Mt^2 = Et^2 - Pt^2 for vanishing neutrino and
    // lepton masses.
    // Use this function for the Jacobian-peak method
    //
    return 2* lepton->Et()*MissingEt.Mod()
	*(1. - TMath::Cos(lepton->P3().XYvector().DeltaPhi(MissingEt)));
}

//____________________________________________________________________

TLorentzVector AtlEvent::GetMCSumEnergy_NonInt() const {
    //
    // Returns the summed 4-vector of all non-interacting
    // MC particles
    //
    HepMCParticle *mc_prt = 0;
    TList *mc_particles = GetStableMCParticles();
    TIter next_mcprt(mc_particles);
    TLorentzVector mc_nonint_sum;
    
    while ( (mc_prt = (HepMCParticle*)next_mcprt()) ) {
	if ( mc_prt->IsGenNonInteracting() ) mc_nonint_sum += mc_prt->P();
	}
    
    delete mc_particles;
    return mc_nonint_sum;
}
//____________________________________________________________________

Float_t AtlEvent::GetMCSumEnergy_NonInt_Eta() const {
    //
    // Returns the eta of the summed 4-vector of all
    // non-interacting MC particles
    //
    TLorentzVector mc_nonint_sum = GetMCSumEnergy_NonInt();
    Float_t mc_nonint_sum_eta;

    if ( mc_nonint_sum.Pt() != 0. ) mc_nonint_sum_eta = mc_nonint_sum.Eta();
    else {
	mc_nonint_sum_eta = ( mc_nonint_sum.Pz() > 0. ) ? 10e10 : -10e10;
    }
    return mc_nonint_sum_eta;
}

//____________________________________________________________________

Float_t AtlEvent::GetMCSumEnergy_NonInt_Phi() const {
    //
    // Returns the phi of the summed 4-vector of all
    // non-interacting MC particles
    //
    TLorentzVector mc_nonint_sum = GetMCSumEnergy_NonInt();
    Float_t mc_nonint_sum_phi;

    mc_nonint_sum_phi = mc_nonint_sum.Phi();
    
    return mc_nonint_sum_phi;
}

//____________________________________________________________________

Float_t AtlEvent::Sphericity(TList *Vectors) {
    //
    // takes List of vectors (TVector3)
    // returns Sphericity computed w.r.t. these vectors
    //
    Float_t Sphericity = 0;
    Float_t denom      = 0;
    Float_t alpha      = 0;
    Float_t beta       = 0;
    TVector3 *p        = 0;
    TMatrixDSym S(3);
    S.Zero();
    for ( Int_t n = 0; n < Vectors->GetEntries(); n++){
	p = (TVector3*)Vectors->At(n);
	for (Int_t i = 0; i < 3; i++ ){
	    if ( i == 0 ) {
		alpha = p->X();  
	    }
	    else if ( i == 1 ) {
		alpha = p->Y(); 
	    }
	    else {
		alpha = p->Z();
	    }
	    for ( Int_t j = i; j < 3; j++ ){
		if ( j == 0 ) {
		    beta = p->X();  
		}
		else if ( j == 1 ){
		    beta = p->Y(); 
		}
		else {
		    beta = p->Z();
		}	
		S(i,j) += alpha*beta;
	    }
	}
    }
    for( Int_t i=0; i < Vectors->GetEntries(); i++ ){
	denom += ((TVector3*)Vectors->At(i))->Mag2();
    }
    S *= 1./denom;
    S[1][0]=S[0][1];
    S[2][0]=S[0][2];
    S[2][1]=S[1][2];
    TMatrixDSymEigen Eigen(S);
    TVectorD EigenValues = Eigen.GetEigenValues();
    Sphericity = 1.5*(EigenValues[1]+EigenValues[2])/EigenValues.Sum();
    return Sphericity;
}

//____________________________________________________________________

TList* AtlEvent::GetVertices(HepVertex::EType type) const {
    //
    // Return list of vertices of given type
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting this list    !!!
    // !!! (not its contents!) when it is not needed anymore !!!
    // !!! to avoid memory leaks.                            !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *vertices = new TList;
    HepVertex *vtx = 0;
    for ( Int_t i = 0; i < fN_Vertices; i++ ) {
	vtx = (HepVertex*)fVertices->At(i);
	if ( vtx->GetType() & type ) vertices->Add(vtx);
    }
    return vertices;
}

//____________________________________________________________________

Int_t AtlEvent::GetN_PrimaryVertices() const {
    //
    // Return number of primary vertices
    //

    // Number already set? (SgTopD3PD case)
    if ( fN_vpx != -1 ) return fN_vpx;

    // Else: compute!
    HepVertex *vtx = 0;
    Int_t N_PrimVertixes = 0;
    for ( Int_t i = 0; i < fN_Vertices; i++ ) {
	vtx = (HepVertex*)fVertices->At(i);
	if(vtx->IsPrimary()) N_PrimVertixes++;
    }

    return N_PrimVertixes;
}

//____________________________________________________________________

HepVertex* AtlEvent::GetPrimaryVtx() const {
    //
    // Get the primary vertex of this event
    //
    HepVertex *vtx = 0;
    if (fVertices == NULL)
	return 0;
    for ( Int_t i = 0; i < fN_Vertices; i++ ) {
	vtx = (HepVertex*)fVertices->At(i);
	if ( vtx->GetType() & HepVertex::kPrimary ) break;
    }
    return vtx;
}

//____________________________________________________________________

Float_t AtlEvent::GetBDTResponse() const {

    return fbdt_scale;  
}

//____________________________________________________________________

AtlIDTrack* AtlEvent::GetAssociatedIDTrack(HepMCParticle *prt) const {
    //
    // Get the IDTrack that is associated to the HepMCParticle
    // by the hitbased Truthmatching (if it exists);
    // otherwise return NULL
    //
    // Default Value for the Matching Probability is set to 0.0001

    // !!!
    // Should be replaced by a datamember on HepMCParticle that contains
    // the link to a HepTrackHelix/AtlIDTrack in the future
    // !!!
    
    TClonesArray *idtrks = GetIDTracks();
    AtlIDTrack *trk = 0;

    for (Int_t i = 0; i < idtrks->GetEntries(); i++){
	trk = (AtlIDTrack*)idtrks->At(i);
	if ( trk->HasValidTruthMatch(0.0001) ){
	    if(trk->GetMCParticle() == prt) return trk;
	}
    }
    return 0;
}
 
//____________________________________________________________________

void AtlEvent::PrintTriggerMatches(Option_t *option) const {
    //
    // Print all matches between reconstructed objects (electrons,
    // muons, photons, tau) and trigger objects.
    // For jets see member function PrintTriggerMatches(AtlJet::EType).
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! Currently only the matches for electrons, photons and !!!
    // !!! muons are filled                                      !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    // Options available:
    //    "ELEC" - Print electron matches
    //    "PHOT" - Print photon matches
    //    "MUON" - Print muon matches
    //    "TAU"  - Print tau lepton matches
    //    "ALL"  - Print all (default)
    //
    TString opt = option;
    opt.ToUpper();
    
    // Trigger config already loaded ?
    if ( GetTrigger()->GetTriggerConf() == 0 ) {
	AtlTriggerConf *conf = AtlTriggerConf::Instance();
	if ( conf->LoadTree(gDirectory) == 0 ) {
	    Error("PrintTriggerMatches",
		  "Cannot load trigger configuration. Please check.");
	    return;
	}
	AtlTrigger::SetConfig(conf);
    }
    
    cout << endl << endl
	 << "Trigger <-> Reco Object Matching:" << endl
	 << "=================================" << endl;

    // Electrons
    if ( opt.Contains("ALL") || opt.Contains("ELEC") ) {
	cout << endl
	     << "Electrons:" << endl
	     << "----------" << endl;
	AtlElectron *el = 0;
	for ( Int_t i = 0; i < GetN_Electrons(); i++ ) {
	    el = (AtlElectron*)fElectrons->At(i);
	    if ( el->GetN_TriggerMatches() > 0 ) {
		cout << "Id = " << el->GetId() << endl;
		PrintTriggerMatches(el);
	    }
	}
	cout << endl;
    }
    
    // Photons
    if ( opt.Contains("ALL") || opt.Contains("PHOT") ) {
	cout << endl
	     << "Photons:" << endl
	     << "--------" << endl;
	AtlPhoton *ph = 0;
	for ( Int_t i = 0; i < GetN_Photons(); i++ ) {
	    ph = (AtlPhoton*)fPhotons->At(i);
	    if ( ph->GetN_TriggerMatches() > 0 ) {
		cout << "Id = " << ph->GetId() << endl;
		PrintTriggerMatches(ph);
	    }
	    cout << endl;
	}
    }
    
    // Muons
    if ( opt.Contains("ALL") || opt.Contains("MUON") ) {
	cout << endl
	     << "Muons:" << endl
	     << "------" << endl;
	AtlMuon *mu = 0;
	for ( Int_t i = 0; i < GetN_Muons(); i++ ) {
	    mu = (AtlMuon*)fMuons->At(i);
	    if ( mu->GetN_TriggerMatches() > 0 ) {
		cout << "Id = " << mu->GetId() << endl;
		PrintTriggerMatches(mu);
	    }
	}
	cout << endl;
    }

    // Tau leptons
    if ( opt.Contains("ALL") || opt.Contains("TAU") ) {
	cout << endl
	     << "Tau leptons:" << endl
	     << "------------" << endl;
	AtlTau *tau = 0;
	for ( Int_t i = 0; i < GetN_Taus(); i++ ) {
	    tau = (AtlTau*)fTaus->At(i);
	    if ( tau->GetN_TriggerMatches() > 0 ) {
		cout << "Id = " << tau->GetId() << endl;
		PrintTriggerMatches(tau);
	    }
	}
	cout << endl;
    }
}
 
//____________________________________________________________________

void AtlEvent::PrintTriggerMatches(AtlJet::EType type) const {
    //
    // Print all matches between given type of jets
    // and trigger objects
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! Currently only the matches for electrons, photons and !!!
    // !!! muons are filled                                      !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    
    // Trigger config already loaded ?
    if ( GetTrigger()->GetTriggerConf() == 0 ) {
	AtlTriggerConf *conf = AtlTriggerConf::Instance();
	if ( conf->LoadTree(gDirectory) == 0 ) {
	    Error("PrintTriggerMatches",
		  "Cannot load trigger configuration. Please check.");
	    return;
	}
	AtlTrigger::SetConfig(conf);
    }
    
    cout << endl << endl
	 << "Trigger <-> Reco Object Matching:" << endl
	 << "=================================" << endl;

    cout << endl
	 << AtlJet::NameOfType(type) << " Jets:" << endl << endl;
    AtlJet *jet = 0;
    for ( Int_t i = 0; i < GetN_Jets(type); i++ ) {
	jet = (AtlJet*)GetJets(type)->At(i);
	if ( jet->GetN_TriggerMatches() > 0 ) {
	    cout << "Id = " << jet->GetId() << endl;
	    PrintTriggerMatches(jet);
	}
    }
    cout << endl;
}

//____________________________________________________________________

void AtlEvent::PrintTriggerMatches(AtlTriggerMatch *obj) const {
    //
    // Helper function for PrintTriggerMatches()
    //
    // Print triggger match for the given reconstructed object
    // (electron, photn, jet, muon or tau)
    //
    TObject *item = 0;
    TIter next_L1(obj->GetMatchedL1Items());
    cout << "Matched L1 items:";
    while ( (item = next_L1()) ) {
	Int_t bit = GetTrigger()->GetL1Items()->IndexOf(item);
	cout << " "
	     << GetTrigger()->GetTriggerConf()->GetL1ItemName(bit, RunNr());
    }
    TIter next_HLT(obj->GetMatchedHLTItems());
    cout << endl << "Matched HLT items:";
    while ( (item = next_HLT()) ) {
	Int_t bit = GetTrigger()->GetHLTItems()->IndexOf(item);
	cout << " "
	     << GetTrigger()->GetTriggerConf()->GetHLTItemName(bit, RunNr());
    }
    cout << endl;
}

//____________________________________________________________________

AtlTriggerItem* AtlEvent::AddL1Match(AtlTriggerMatch* RecoObject,
				     const char* L1ItemName) {
    //
    // Add matched L1 item to reconstructed object
    //
    AtlTriggerItem *item = GetTrigger()->GetL1Item(L1ItemName);
    RecoObject->AddL1Match(item);
    return item;
}

//____________________________________________________________________

AtlTriggerItem* AtlEvent::AddHLTMatch(AtlTriggerMatch* RecoObject,
				      const char* HLTItemName) {
    //
    // Add matched HLT item to reconstructed object
    //
    AtlTriggerItem *item = GetTrigger()->GetHLTItem(HLTItemName);
    RecoObject->AddHLTMatch(item);
    return item;
}

//____________________________________________________________________

void AtlEvent::ComputePtRel(HepParticle *prt,
			    TCollection *search_list,
			    Float_t DeltaRmax,
			    Double_t &PtRel, Double_t &DeltaR) {
    //
    // Compute the relative transverse momentum Pt_rel between the
    // given particle and the given list of jets nearest in DeltaR.
    // Only jets within the given cone of DeltaRmax are considered.
    // The corresponding DeltaR between the lepton and the selected
    // jet is also given.
    //
    // The return values PtRel and DeltaR are passed by reference. In
    // case no jet is found inside the given search cone, -1 will be
    // returned for both values.
    //
    TList *jets = FindMatchedJets(prt, search_list, DeltaRmax, 1., kTRUE);
    if ( jets->GetEntries() < 1 ) {
	delete jets;
	PtRel = -1.; DeltaR = -1.;
	return;
    }
    AtlJet *jet = (AtlJet*)jets->First();
    delete jets;
    PtRel  = jet->PtRel(prt);
    DeltaR = jet->DeltaR(prt);
    return;
}

