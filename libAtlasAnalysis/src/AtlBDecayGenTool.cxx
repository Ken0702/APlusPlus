//____________________________________________________________________
//
// A++ tool for generating semileptonic B-meson decays for fake lepton
// studies
// 
// The tool runs in 'pre-analysis' mode (see AtlAnalysisTool and
// AtlSelector for details), ie. after object defintion/selection
// and event scaling
//
// !!! IMPORTANT !!! The tool has to be run _after_ the object
// selection&definition tool, because otherwise the required b-jets
// are not yet defined.
//
// The tool is only active for the given list of valid DSIDs.
//
// Example:
//
//    // B-meson decay generation tool
//    AtlBDecayGenTool *tool_bdecay = new AtlBDecayGenTool("BDecayGenTool", "B-meson decay generation");
//    main->AddTool(tool_bdecay);
//    main->SetToolParameter("BDecayGenTool", "fValidDSIDs", "\"361106,361107,361108\"");
//
// Leptons are generated only if at least one b-jet is present in the
// event which can serve as parent jet. If more than one b-jet exists,
// the parent jet can be picked in several ways, see the data member
// fBJetMode (default=random).
//
// For a better modelling an overlap removal of the parent jet can be
// performed (see public data member fDoOverlapRemoval, default=true).
// If the DeltaR between the generated lepton and its parent jet is
// smaller than the value given in fORdeltaR_max (default=0.4), the
// parent jet will be removed.
//
// !!! IMPORTANT !!! In case of the overlap removal the jet is removed
// only from the jet and b-jet lists of the object definition tool.
// These are the very same lists used by this tool and the analysis
// selector. However, the jet is _not_ removed from the corresponding
// TClonesArray of the A++ event record, nor is the number of jet
// variable of the A++ event changed.
//
// Systematics:
//
// In order to study effects of shape variations for the generated pT
// spectrum of the lepton fakes, the parameter 'fSystDecayLength' is
// used. If the tool runs in the systematic mode
// 'kLEP_FAKE_BDECAYGEN_SHAPE', the pre-tag and b-tag event weights
// are multiplied by a re-weight factor for the pT spectrum which is
// given by exp(pT_lep/fSystDecayLength). The default value for
// fSystDecayLength is 30. The idea behind the expontential and the
// default value are studies by the ttH group about the impact of
// phonton conversions induced by tt+gamma production.
//
//
// Author:  <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2017 (C) 
//
#ifndef ATLAS_AtlBDecayGenTool
#include <AtlBDecayGenTool.h>
#endif
#include <AtlEvent.h>
#include <AtlObjectsToolD3PDSgTop.h>
#include <TH2D.h>
#include <TSystem.h>
#include <TRandom3.h>
#include <TList.h>
#include <TLine.h>
#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

static const char* fgBJetModeNames[AtlBDecayGenTool::kNBJetModes] = {
    "random", "low Pt", "high Pt", "half-random"
};

#ifndef __CINT__
ClassImp(AtlBDecayGenTool);
#endif

//___________________________________________________________

template<typename _T>
static void hash_add(unsigned int & seed, _T const & value) {
    // a la boost::hash_combine
    seed ^= std::hash<_T>()(value) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

//____________________________________________________________________

AtlBDecayGenTool::AtlBDecayGenTool(const char* name, const char* title) : 
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    SetCutDefaults();
    fFirstEvt = kTRUE;
    fRandom = 0;
    fJets = 0;
    fBJets = 0;
    CreateHistGen();
    fHistDeltaR = 0;
    fHistAlphaLoc = 0;
    fHistPhiLoc = 0;
    fHistPzLoc = 0;
    fHistPtLoc = 0;
    fHistPmagLoc = 0;
    fHistAlphaLab = 0;
    fHistEtaLab = 0;
    fHistPhiLab = 0;
    fHistPzLab = 0;
    fHistPtLab = 0;
    fHistPmagLab = 0;
}

//____________________________________________________________________

AtlBDecayGenTool::~AtlBDecayGenTool() {
    //
    // Default destructor
    //
    delete fRandom;
    delete fHistGen;
    if ( fHistDeltaR != 0 ) delete fHistDeltaR;
    if ( fHistAlphaLoc != 0 ) delete fHistAlphaLoc;
    if ( fHistPhiLoc != 0 ) delete fHistPhiLoc;
    if ( fHistPzLoc != 0 ) delete fHistPzLoc;
    if ( fHistPtLoc != 0 ) delete fHistPtLoc;
    if ( fHistPmagLoc != 0 ) delete fHistPmagLoc;
    if ( fHistAlphaLab != 0 ) delete fHistAlphaLab;
    if ( fHistEtaLab != 0 ) delete fHistEtaLab;
    if ( fHistPhiLab != 0 ) delete fHistPhiLab;
    if ( fHistPzLab != 0 ) delete fHistPzLab;
    if ( fHistPtLab != 0 ) delete fHistPtLab;
    if ( fHistPmagLab != 0 ) delete fHistPmagLab;
}

//____________________________________________________________________

void AtlBDecayGenTool::Init() {
    //
    // Get b-jet list from object definition tool
    //
    AtlObjectsToolD3PDSgTop *ObjTool = (AtlObjectsToolD3PDSgTop*)
	fParent->GetTool("AtlObjectsToolD3PDSgTop", "", kTRUE);
    if ( ObjTool == 0 ) {
	Error(__FUNCTION__, "Cannot find object definition tool in order to set the b-jet list. Abort!");
	gSystem->Abort(1);
    }
    fBJets = ObjTool->GetBJets();
    fJets = ObjTool->GetJets();
    fElectrons = ObjTool->GetElectrons();
    fMuons = ObjTool->GetMuons();
    fLeptons = ObjTool->GetLeptons();
}

//___________________________________________________________

void AtlBDecayGenTool::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
}

//___________________________________________________________

void AtlBDecayGenTool::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fProcessMode = kPreAnalysis;
    fDoOverlapRemoval = kTRUE;
    fIsZjets = kFALSE;
    fArtificialLeptons = kTRUE;
    fRequireOverlapRemoval = kFALSE;
    fORdeltaR_max = 0.4;
    fBJetMode = kRandom;
    fSystematicMode = AtlTopLevelAnalysis::kNOMINAL;
    fSystDecayLength = 30.;
}

//___________________________________________________________

const char* AtlBDecayGenTool::GetBJetModeName(AtlBDecayGenTool::EBJetMode BJetMode) const {
    //
    // Get name of current b-jet pick mode
    //
    return fgBJetModeNames[BJetMode];
}

//___________________________________________________________

void AtlBDecayGenTool::BookHistograms() {
    //
    // Book histograms
    //

    // DeltaR for overlap removal
    fHistDeltaR = new TH1F("h_deltaR", "DeltaR", 100, 0., 2.);
    fHistDeltaR->SetXTitle("#DeltaR");
    fHistDeltaR->SetYTitle("Number of Entries");

    // Alpha, local
    fHistAlphaLoc = new TH1F("h_alpha_loc", "Alpha, local", 64, 0, 3.2);
    fHistAlphaLoc->SetXTitle("#alpha_{loc} [rad]");
    fHistAlphaLoc->SetYTitle("Number of Entries");

    // Phi, local
    fHistPhiLoc = new TH1F("h_phi_loc", "Phi, local", 64, -3.2, 3.2);
    fHistPhiLoc->SetXTitle("#phi_{loc} [rad]");
    fHistPhiLoc->SetYTitle("Number of Entries");

    // Pz, local
    fHistPzLoc = new TH1F("h_pz_loc", "Pz, local", 200, -100., 100.);
    fHistPzLoc->SetXTitle("P_{z}^{loc} [GeV]");
    fHistPzLoc->SetYTitle("Number of Entries");
    
    // Pt, local
    fHistPtLoc = new TH1F("h_pt_loc", "Pt, local", 150, 0., 30.);
    fHistPtLoc->SetXTitle("P_{t}^{loc} [GeV]");
    fHistPtLoc->SetYTitle("Number of Entries");

    // P mag., local
    fHistPmagLoc = new TH1F("h_pmag_loc", "P magnitude, local", 200, -100., 100.);
    fHistPmagLoc->SetXTitle("|P|^{loc} [GeV]");
    fHistPmagLoc->SetYTitle("Number of Entries");

    // Alpha, lab frame
    fHistAlphaLab = new TH1F("h_alpha_lab", "Alpha, lab frame", 64, 0, 3.2);
    fHistAlphaLab->SetXTitle("#alpha_{lab} [rad]");
    fHistAlphaLab->SetYTitle("Number of Entries");

    // Eta, lab frame
    fHistEtaLab = new TH1F("h_eta_lab", "Eta, lab frame", 100, -5., 5.);
    fHistEtaLab->SetXTitle("#eta_{lab}");
    fHistEtaLab->SetYTitle("Number of Entries");

    // Phi, lab frame
    fHistPhiLab = new TH1F("h_phi_lab", "Phi, lab frame", 64, -3.2, 3.2);
    fHistPhiLab->SetXTitle("#phi_{lab} [rad]");
    fHistPhiLab->SetYTitle("Number of Entries");

    // Pz, lab frame
    fHistPzLab = new TH1F("h_pz_lab", "Pz, lab frame", 200, -200., 200.);
    fHistPzLab->SetXTitle("P_{z}^{lab} [GeV]");
    fHistPzLab->SetYTitle("Number of Entries");
    
    // Pt, lab frame
    fHistPtLab = new TH1F("h_pt_lab", "Pt, lab frame", 160, 0., 80.);
    fHistPtLab->SetXTitle("P_{t}^{lab} [GeV]");
    fHistPtLab->SetYTitle("Number of Entries");

    // P mag., lab frame
    fHistPmagLab = new TH1F("h_pmag_lab", "P magnitude, lab frame", 250, 0., 250.);
    fHistPmagLab->SetXTitle("|P|^{lab} [GeV]");
    fHistPmagLab->SetYTitle("Number of Entries");
}

//___________________________________________________________

Bool_t AtlBDecayGenTool::AnalyzeEvent() {
    //
    // Invocation of tool
    //
    
    // Set array of valid DSIDs
    if ( fFirstEvt ) {
	fFirstEvt = kFALSE;
	SetDSIDArray();
    }

    // Check current DSID. Disable tool in case of invalid DSID.
    Bool_t IsValid = kFALSE;
    for ( Int_t i = 0; i < fNDSIDs; i++ ) {
	if ( fEvent->GetEventHeader()->GetChannelNumber() == fDSID[i] ) {
	    IsValid = kTRUE;
	    break;
	}
    }
    if ( !IsValid ) {
	Info(__FUNCTION__, "\nThe current input sample is not a valid DSID of the BDecayGen tool. The tool will be disabled for this analysis job.\n");
	fProcessMode = kOff;
	return kTRUE;
    }

    // Re-seed random number generator for every event (for
    // reproducible results)
    {
        unsigned int seed = 0xbdeca10;
        hash_add(seed, fEvent->GetEventHeader()->GetChannelNumber());
        hash_add(seed, fEvent->GetEventHeader()->EventNr());
        while ( !seed ) {
            hash_add(seed, 0);
        }
        if ( fRandom ) {
            fRandom->SetSeed(seed);
        }
        else {
            fRandom = new TRandom3(seed);
        }
    }

    Bool_t return_code;

    if ( fArtificialLeptons){
        // Generate B-meson decay and add/replace leptons
        return_code = AddLeptons();
    }
    else {
        return_code = kTRUE;
    }

    // Re-weight the event in case of systematic studies of shape
    // variations of the fake lepton pT spectrum
    if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_FAKE_BDECAYGEN_SHAPE ) {
        if ( fLeptons->GetEntries() >= 3 ) {
        HepParticle *p_lep3 = 0;
        p_lep3 = (HepParticle*) fLeptons->At(2);
        TLorentzVector lep3 = p_lep3->P();  
	//Double_t weight = TMath::Exp(p_lep.Pt()/fSystDecayLength);
	Double_t weight = TMath::Min((lep3.Pt()),100.)/fSystDecayLength;
	fEvent->SetPreTagEvtWeight(GetPreTagEvtWeight()*weight);
	fEvent->SetTagEvtWeight(GetTagEvtWeight()*weight);
        }
    }

    return return_code;
}

//___________________________________________________________

void AtlBDecayGenTool::Print() const {
    //
    // Print tool configuration
    //
    cout << endl
         << "========================================================" << endl
         << "  BDecayGen Tool \"" << GetName() << "\"" << endl
         << "========================================================"
	 << endl << endl;
    AtlAnalysisTool::Print();
    cout << endl
	 << "  fDoOverlapRemoval = ";
    if ( fDoOverlapRemoval ) {
	cout << "true" << endl;
    } else {
	cout << "false" << endl;
    }
    cout << "  fORdeltaR_max     = " << fORdeltaR_max << endl
	 << "  fBJetMode         = " << GetBJetModeName(fBJetMode) << endl;
    cout << endl
	 << "  The following DSIDs will be processed by the tool:" << endl
	 << "     " << fValidDSIDs << endl
	 << "  For all other samples the tool will be disabled."
	 << endl << endl
	 << "  fSystematicMode   = " << AtlTopLevelAnalysis::GetSystematicEnum(fSystematicMode) << endl
	 << "  fSystDecayLength  = " << fSystDecayLength
	 << endl << endl
	 << "  fIsZjets = ";
    if ( fIsZjets ) {
	cout << "true" << endl;
    }
    else {
    cout << "false" << endl;
    }
	cout << "  fArtificialLeptons = ";
    if ( fArtificialLeptons ) {
	cout << "true" << endl;
    }
    else {
    cout << "false" << endl;
    }
     cout 
     << "========================================================" << endl
	 << endl;
}

//___________________________________________________________

void AtlBDecayGenTool::Terminate() {
    //
    // Executed at the end of the tool
    //
    AtlAnalysisTool::Terminate();
    TLine *l = new TLine(fORdeltaR_max, 0., fORdeltaR_max,
			 fHistDeltaR->GetMaximum()*1.15);
    l->SetLineStyle(2);
    fHistDeltaR->GetListOfFunctions()->Add(l);
}

//___________________________________________________________

void AtlBDecayGenTool::SetDSIDArray() {
    //
    // Setup DSID array from DSID string
    //
    fNDSIDs = 0;
    for ( Int_t i = 0; i < fgNDSIDsMax; i++ ) fDSID[i] = 0;
    TString dsid_str(fValidDSIDs);
    TObjArray *dsid_tok = dsid_str.Tokenize(",");
    if (  dsid_tok->GetEntries() > fgNDSIDsMax ) {
	Error(__FUNCTION__,
	      "Number of given DSID list larger than reserved array length. Abort!");
	gSystem->Abort(1);
    }
    for ( Int_t i = 0; i < dsid_tok->GetEntries(); i++ ) {
	fDSID[i] = ((TObjString*)dsid_tok->At(i))->GetString().Atoi();
	fNDSIDs++;
    }
}

//____________________________________________________________________

Bool_t AtlBDecayGenTool::AddLeptons() {
    //
    // Generate a lepton from a semileptonic B-meson decay for the
    // leading b-jet and add it to the A++ event record as well as to
    // the lepton lists of the analysis selector
    //

    // Require at least one b-jet
    TList bquark_jets;
    for ( TIter next(fBJets); AtlJet * jet = static_cast<AtlJet *>(next()); ) {
        if ( jet->GetTruthFlavour() == AtlJet::kBeauty ) {
            bquark_jets.Add(jet);
        }
    }
    TList * jets = &bquark_jets;
    Int_t njets = jets->GetEntries();
    if (  njets < 1 ) return kFALSE;

    // Pick parent b-jet
    AtlJet *bjet = 0;
    switch ( fBJetMode ) {
	case kRandom: {
	    Int_t ijet = (Int_t)(fRandom->Rndm()*njets);
	    if ( ijet == njets ) ijet--;
	    bjet = (AtlJet*)jets->At(ijet);
	    break;
	}
        case kHalfRandom: {
	    Int_t ijet = (Int_t)(fRandom->Rndm()*njets);
	    if ( ijet == njets ) ijet--;
            if ( fRandom->Rndm() < 0.5 ) {
                bjet = (AtlJet*)jets->At(ijet);
            }
            else {
                bjet = (AtlJet*)jets->Last();
            }
            break;
	}
	case kLowPt: {
	    bjet = (AtlJet*)jets->Last();
	    break;
	}
	case kHighPt: {
	    bjet = (AtlJet*)jets->First();
	    break;
	}
	default: {
	    Error(__FUNCTION__, "Wrong BJetMode given. Abort !");
	    gSystem->Abort(1);
	}
    }
    
    // Generate lepton 
    HepParticle *lep = 0;
    lep = GenerateBDecay(bjet->P(), lep);

    // Perform overlap removal
    Float_t DeltaR = lep->DeltaR(bjet);
    fHistDeltaR->Fill(DeltaR, fEvent->GetPreTagEvtWeight());
    if ( fDoOverlapRemoval && DeltaR < fORdeltaR_max ) {
	fJets->Remove(bjet);
	fBJets->Remove(bjet);
	return kTRUE;
    }
    return ( fRequireOverlapRemoval ? kFALSE : kTRUE );
}

//____________________________________________________________________

HepParticle* AtlBDecayGenTool::GenerateBDecay(TLorentzVector p_mother,
					      HepParticle *lep) {
    //
    // Generate lepton from B-meson decay
    //
    // If the lepton pointer is 0, a new lepton will be added to
    // the current event - otherwise the given lepton momentum vector
    // will be changed. The lepton pointer is returned.
    //
    Double_t m_lep = 0.;
    Int_t PdgCode = 0;

    // Generate lepton feature in case the lepton does not exist yet
    if ( lep != 0 ) {
	m_lep = lep->Mass();
    } else {
	// Equal prob. for e/mu and +/-
	m_lep = 5.10998909999999971e-04;
	PdgCode = 11;
	if ( fRandom->Rndm() > 0.5 ) {
	    m_lep = 1.05658000000000002e-01;
	    PdgCode = 13;
	}
	if ( fRandom->Rndm() > 0.5 ) {
	    PdgCode *= -1;
	}
    }
    
    // Throw random numbers for energy and decay angle
    Double_t E_rf = 0.; Double_t alpha = 0.;
    fHistGen->GetRandom2(alpha, E_rf);

    // Phi is isotropic
    Double_t phi = fRandom->Rndm() * 2.*TMath::Pi();
    
    // Create lepton momentum 4-vector in B-meson rest frame
    Double_t p_mag = TMath::Sqrt(E_rf*E_rf - m_lep*m_lep);
    Double_t px = p_mag * TMath::Cos(phi) * TMath::Sin(alpha);
    Double_t py = p_mag * TMath::Sin(phi) * TMath::Sin(alpha);
    Double_t pz = p_mag * TMath::Cos(alpha);
    TLorentzVector p_lep(px, py, pz, E_rf);
    
    fHistAlphaLoc->Fill(p_lep.Theta(), fEvent->GetPreTagEvtWeight());
    fHistPhiLoc  ->Fill(p_lep.Phi(),   fEvent->GetPreTagEvtWeight());
    fHistPzLoc   ->Fill(p_lep.Pz(),    fEvent->GetPreTagEvtWeight());
    fHistPtLoc   ->Fill(p_lep.Pt(),    fEvent->GetPreTagEvtWeight());
    fHistPmagLoc ->Fill(p_lep.Rho(),   fEvent->GetPreTagEvtWeight());

    // Boost along b-jet direction (in local system)
    TLorentzVector p_mother_loc(0., 0., p_mother.Rho(), p_mother.E());
    TVector3 boost = p_mother_loc.BoostVector();
    p_lep.Boost(boost);
	
    // Rotate from local frame (B-meson / b-jet axis is the
    // z-direction) to the lab frame. Neglect displacements by the
    // primary and the decay vertices.
    TVector3 zdir = p_mother.Vect().Unit();
    p_lep.RotateUz(zdir);

    fHistAlphaLab->Fill(p_lep.Theta(), fEvent->GetPreTagEvtWeight());
    fHistEtaLab  ->Fill(p_lep.Eta(),   fEvent->GetPreTagEvtWeight());
    fHistPhiLab  ->Fill(p_lep.Phi(),   fEvent->GetPreTagEvtWeight());
    fHistPzLab   ->Fill(p_lep.Pz(),    fEvent->GetPreTagEvtWeight());
    fHistPtLab   ->Fill(p_lep.Pt(),    fEvent->GetPreTagEvtWeight());
    fHistPmagLab ->Fill(p_lep.Rho(),   fEvent->GetPreTagEvtWeight());

    // Add/set daughter lepton
    if ( lep == 0 ) {
	// Add non-existing lepton
	if ( TMath::Abs(PdgCode) == 11 ) { // Electron
	    AtlElectron *el = fEvent->AddElectron(p_lep.Px(),
						  p_lep.Py(),
						  p_lep.Pz(),
						  p_lep.E(),
						  (PdgCode < 0),
						  NAN, NAN, 0, 0,
						  AtlEMShower::kElectronTight,
						  AtlElectron::kHighPtElectron,
						  TLorentzVector(NAN, NAN, NAN, NAN));
	    el->SetEtConeVars(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
			      NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
			      NAN, NAN, NAN, NAN, NAN, NAN);
	    el->SetElectronScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
	    el->SetIsTriggerMatched(kTRUE);
	    if ( fEvent->IsMC() ) {
		el->SetMCTruthClassifier(AtlMCTruthClassifier::kBottomMeson);
	    }
	    fElectrons->Add(el);
	    fLeptons->Add(el);
	    fElectrons->Sort(kSortDescending);
	    fLeptons->Sort(kSortDescending);
	    lep = el;
	} else if ( TMath::Abs(PdgCode) == 13 ) { // Muon
	    AtlMuon *mu = fEvent->AddMuon(p_lep.Px(),
					  p_lep.Py(),
					  p_lep.Pz(),
					  p_lep.E(),
					  (PdgCode < 0),
					  NAN, NAN, NAN, NAN,
					  -999, -999, -999, -999,
					  NAN, NAN, NAN, NAN,
					  AtlMuon::kMuidCo, AtlMuon::kTight,
					  NAN, -999, kTRUE,
					  TLorentzVector(NAN, NAN, NAN, NAN), 0);
	    mu->SetMuonScaleFactors(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);
	    mu->SetIsTriggerMatched(kTRUE);
	    if ( fEvent->IsMC() ) {
		mu->SetMCTruthClassifier(AtlMCTruthClassifier::kBottomMeson);
	    }
	    fMuons->Add(mu);
	    fLeptons->Add(mu);
	    fMuons->Sort(kSortDescending);
	    fLeptons->Sort(kSortDescending);
	    lep = mu;
	} else {
	    Error(__FUNCTION__, "Wrong particle type. Abort!");
	    gSystem->Abort(1);
	}
    } 
    else {
        //if ( fIsZjets ){
	    // Change momentum vector of existing lepton
        lep->SetPxPyPzE(p_lep.Px(), p_lep.Py(), p_lep.Pz(), p_lep.E());
        //}
    }
   
	if ( fEvent->IsMC() ) {
	    if ( lep->IsElectron() ) {
		((AtlElectron*)lep)->SetMCTruthClassifier(AtlMCTruthClassifier::kBottomMeson);
	    } else if ( lep->IsMuon() ) {
		((AtlMuon*)lep)->SetMCTruthClassifier(AtlMCTruthClassifier::kBottomMeson);
	    }
	}

    return lep;
}

//___________________________________________________________

void AtlBDecayGenTool::CreateHistGen() {
    //
    // Create histogram for decay angle / energy generation
    //
    Double_t yAxis1[40] = {0.1, 0.125893, 0.158489, 0.199526, 0.251189, 0.316228, 0.398107, 0.501187, 0.630957, 0.794328, 1, 1.25893, 1.58489, 1.99526, 2.51189, 3.16228, 3.98107, 5.01187, 6.30957, 7.94328, 10, 12.5893, 15.8489, 19.9526, 25.1189, 31.6228, 39.8107, 50.1187, 63.0957, 79.4328, 100, 125.893, 158.489, 199.526, 251.189, 316.228, 398.107, 501.187, 630.957, 794.328};

    fHistGen = new TH2D("h_lep_alpha_E_rf", "Decay angle vs. energy in B rest-frame",
			32, 0, 3.2, 39, yAxis1);
    fHistGen->SetBinContent(240,0.701885);
    fHistGen->SetBinContent(275,0.768827);
    fHistGen->SetBinContent(285,0.778158);
    fHistGen->SetBinContent(308,0.700119);
    fHistGen->SetBinContent(310,2.31565);
    fHistGen->SetBinContent(311,1.44597);
    fHistGen->SetBinContent(312,0.962111);
    fHistGen->SetBinContent(313,0.797437);
    fHistGen->SetBinContent(315,0.746081);
    fHistGen->SetBinContent(316,0.677152);
    fHistGen->SetBinContent(318,1.48187);
    fHistGen->SetBinContent(341,1.26213);
    fHistGen->SetBinContent(342,1.3747);
    fHistGen->SetBinContent(343,2.45267);
    fHistGen->SetBinContent(344,0.703623);
    fHistGen->SetBinContent(345,3.70964);
    fHistGen->SetBinContent(346,9.76517);
    fHistGen->SetBinContent(347,1.47078);
    fHistGen->SetBinContent(348,1.36329);
    fHistGen->SetBinContent(349,3.67737);
    fHistGen->SetBinContent(350,2.56587);
    fHistGen->SetBinContent(356,0.775472);
    fHistGen->SetBinContent(375,3.35723);
    fHistGen->SetBinContent(376,14.365);
    fHistGen->SetBinContent(377,12.0348);
    fHistGen->SetBinContent(378,25.0383);
    fHistGen->SetBinContent(379,21.4893);
    fHistGen->SetBinContent(380,11.7668);
    fHistGen->SetBinContent(381,17.2946);
    fHistGen->SetBinContent(382,18.4826);
    fHistGen->SetBinContent(383,7.15634);
    fHistGen->SetBinContent(384,7.36349);
    fHistGen->SetBinContent(385,8.49107);
    fHistGen->SetBinContent(386,9.79438);
    fHistGen->SetBinContent(387,5.18412);
    fHistGen->SetBinContent(388,2.26754);
    fHistGen->SetBinContent(389,2.3444);
    fHistGen->SetBinContent(392,0.566273);
    fHistGen->SetBinContent(409,15.0782);
    fHistGen->SetBinContent(410,50.4661);
    fHistGen->SetBinContent(411,60.9552);
    fHistGen->SetBinContent(412,71.3095);
    fHistGen->SetBinContent(413,68.5219);
    fHistGen->SetBinContent(414,65.0189);
    fHistGen->SetBinContent(415,64.3684);
    fHistGen->SetBinContent(416,67.8564);
    fHistGen->SetBinContent(417,57.0361);
    fHistGen->SetBinContent(418,34.3728);
    fHistGen->SetBinContent(419,36.337);
    fHistGen->SetBinContent(420,24.8433);
    fHistGen->SetBinContent(421,13.0614);
    fHistGen->SetBinContent(422,5.16621);
    fHistGen->SetBinContent(423,9.2007);
    fHistGen->SetBinContent(424,12.1919);
    fHistGen->SetBinContent(425,0.735782);
    fHistGen->SetBinContent(426,0.806922);
    fHistGen->SetBinContent(443,13.7669);
    fHistGen->SetBinContent(444,35.6108);
    fHistGen->SetBinContent(445,70.0889);
    fHistGen->SetBinContent(446,98.6953);
    fHistGen->SetBinContent(447,77.3303);
    fHistGen->SetBinContent(448,108.314);
    fHistGen->SetBinContent(449,96.224);
    fHistGen->SetBinContent(450,96.1529);
    fHistGen->SetBinContent(451,85.7218);
    fHistGen->SetBinContent(452,82.9347);
    fHistGen->SetBinContent(453,73.658);
    fHistGen->SetBinContent(454,71.1393);
    fHistGen->SetBinContent(455,40.4474);
    fHistGen->SetBinContent(456,47.3821);
    fHistGen->SetBinContent(457,29.3957);
    fHistGen->SetBinContent(458,13.4427);
    fHistGen->SetBinContent(459,9.53975);
    fHistGen->SetBinContent(460,2.8608);
    fHistGen->SetBinContent(461,1.53814);
    fHistGen->SetBinContent(462,2.82603);
    fHistGen->SetBinContent(477,1.79874);
    fHistGen->SetBinContent(478,0.658307);
    fHistGen->SetBinContent(479,5.40832);
    fHistGen->SetBinContent(480,6.75223);
    fHistGen->SetBinContent(481,12.5913);
    fHistGen->SetBinContent(482,11.0623);
    fHistGen->SetBinContent(483,20.1987);
    fHistGen->SetBinContent(484,22.4911);
    fHistGen->SetBinContent(485,28.1815);
    fHistGen->SetBinContent(486,38.364);
    fHistGen->SetBinContent(487,44.0488);
    fHistGen->SetBinContent(488,42.6043);
    fHistGen->SetBinContent(489,52.0973);
    fHistGen->SetBinContent(490,56.5999);
    fHistGen->SetBinContent(491,38.9287);
    fHistGen->SetBinContent(492,34.9232);
    fHistGen->SetBinContent(493,14.4351);
    fHistGen->SetBinContent(494,11.4856);
    fHistGen->SetBinContent(495,13.2494);
    fHistGen->SetBinContent(496,1.82541);
    fHistGen->SetBinContent(497,0.635383);
    fHistGen->SetBinContent(519,0.621134);
    fHistGen->SetBinContent(520,2.6585);
    fHistGen->SetBinContent(521,2.8988);
    fHistGen->SetBinContent(522,7.05411);
    fHistGen->SetBinContent(523,15.862);
    fHistGen->SetBinContent(524,15.4317);
    fHistGen->SetBinContent(525,35.527);
    fHistGen->SetBinContent(526,36.7505);
    fHistGen->SetBinContent(527,29.7054);
    fHistGen->SetBinContent(528,30.4206);
    fHistGen->SetBinContent(529,11.946);
    fHistGen->SetBinContent(530,7.24759);
    fHistGen->SetBinContent(531,1.68212);
    fHistGen->SetBinContent(532,2.29139);
    fHistGen->SetBinContent(533,1.32379);
    fHistGen->SetBinContent(559,3.6832);
    fHistGen->SetBinContent(560,7.9728);
    fHistGen->SetBinContent(561,14.1516);
    fHistGen->SetBinContent(562,25.245);
    fHistGen->SetBinContent(563,35.9267);
    fHistGen->SetBinContent(564,23.1646);
    fHistGen->SetBinContent(565,21.3072);
    fHistGen->SetBinContent(566,7.87172);
    fHistGen->SetBinContent(567,2.90445);
    fHistGen->SetBinContent(594,0.644825);
    fHistGen->SetBinContent(595,2.0851);
    fHistGen->SetBinContent(596,2.1657);
    fHistGen->SetBinContent(597,14.1529);
    fHistGen->SetBinContent(598,26.8554);
    fHistGen->SetBinContent(599,19.6887);
    fHistGen->SetBinContent(600,20.9557);
    fHistGen->SetBinContent(601,9.00327);
    fHistGen->SetBinContent(602,5.55987);
    fHistGen->SetBinContent(631,0.724829);
    fHistGen->SetBinContent(632,3.82183);
    fHistGen->SetBinContent(633,13.633);
    fHistGen->SetBinContent(634,16.8974);
    fHistGen->SetBinContent(635,25.1052);
    fHistGen->SetBinContent(636,8.6439);
    fHistGen->SetBinContent(637,9.44115);
    fHistGen->SetBinContent(638,1.1778);
    fHistGen->SetBinContent(667,5.95153);
    fHistGen->SetBinContent(668,8.85053);
    fHistGen->SetBinContent(669,23.9611);
    fHistGen->SetBinContent(670,22.0313);
    fHistGen->SetBinContent(671,10.9586);
    fHistGen->SetBinContent(672,4.01959);
    fHistGen->SetBinContent(703,9.13614);
    fHistGen->SetBinContent(704,14.4111);
    fHistGen->SetBinContent(705,19.5849);
    fHistGen->SetBinContent(706,9.02752);
    fHistGen->SetBinContent(707,2.73644);
    fHistGen->SetBinContent(737,0.00155579);
    fHistGen->SetBinContent(738,3.40822);
    fHistGen->SetBinContent(739,13.1045);
    fHistGen->SetBinContent(740,25.3608);
    fHistGen->SetBinContent(741,5.99301);
    fHistGen->SetBinContent(742,3.3363);
    fHistGen->SetBinContent(772,1.35853);
    fHistGen->SetBinContent(773,6.13863);
    fHistGen->SetBinContent(774,8.32797);
    fHistGen->SetBinContent(775,20.0902);
    fHistGen->SetBinContent(776,2.74145);
    fHistGen->SetBinContent(808,6.07718);
    fHistGen->SetBinContent(809,14.0382);
    fHistGen->SetBinContent(810,11.9871);
    fHistGen->SetBinContent(811,2.8444);
    fHistGen->SetBinContent(840,0.762896);
    fHistGen->SetBinContent(842,1.521);
    fHistGen->SetBinContent(843,13.3846);
    fHistGen->SetBinContent(844,10.9977);
    fHistGen->SetBinContent(845,2.67976);
    fHistGen->SetBinContent(876,0.710711);
    fHistGen->SetBinContent(877,6.80887);
    fHistGen->SetBinContent(878,13.5056);
    fHistGen->SetBinContent(879,11.3882);
    fHistGen->SetBinContent(880,2.63618);
    fHistGen->SetBinContent(912,7.04308);
    fHistGen->SetBinContent(913,11.2736);
    fHistGen->SetBinContent(914,2.15752);
    fHistGen->SetBinContent(946,0.675937);
    fHistGen->SetBinContent(947,15.9777);
    fHistGen->SetBinContent(948,11.1354);
    fHistGen->SetBinContent(980,0.798667);
    fHistGen->SetBinContent(981,4.64004);
    fHistGen->SetBinContent(982,5.84392);
    fHistGen->SetBinContent(1015,1.4931);
    fHistGen->SetBinContent(1016,7.49609);
    fHistGen->SetBinContent(1050,1.86895);
    fHistGen->SetBinContent(1084,0.689512);
    fHistGen->SetBinContent(1118,0.594383);
    fHistGen->SetBinContent(1119,0.794655);
    fHistGen->SetBinContent(1153,2.50961);
    fHistGen->SetBinError(240,0.701885);
    fHistGen->SetBinError(275,0.768827);
    fHistGen->SetBinError(285,0.778158);
    fHistGen->SetBinError(308,0.700119);
    fHistGen->SetBinError(310,2.31565);
    fHistGen->SetBinError(311,1.02431);
    fHistGen->SetBinError(312,0.962111);
    fHistGen->SetBinError(313,0.797437);
    fHistGen->SetBinError(315,0.746081);
    fHistGen->SetBinError(316,0.677152);
    fHistGen->SetBinError(318,1.48187);
    fHistGen->SetBinError(341,0.895489);
    fHistGen->SetBinError(342,0.972132);
    fHistGen->SetBinError(343,1.23873);
    fHistGen->SetBinError(344,0.703623);
    fHistGen->SetBinError(345,1.66539);
    fHistGen->SetBinError(346,5.41911);
    fHistGen->SetBinError(347,1.04006);
    fHistGen->SetBinError(348,0.96455);
    fHistGen->SetBinError(349,1.91671);
    fHistGen->SetBinError(350,1.91709);
    fHistGen->SetBinError(356,0.775472);
    fHistGen->SetBinError(375,1.72742);
    fHistGen->SetBinError(376,3.66155);
    fHistGen->SetBinError(377,3.07739);
    fHistGen->SetBinError(378,4.91635);
    fHistGen->SetBinError(379,7.29311);
    fHistGen->SetBinError(380,3.24446);
    fHistGen->SetBinError(381,3.65899);
    fHistGen->SetBinError(382,3.9915);
    fHistGen->SetBinError(383,2.46985);
    fHistGen->SetBinError(384,2.71722);
    fHistGen->SetBinError(385,3.06532);
    fHistGen->SetBinError(386,3.19518);
    fHistGen->SetBinError(387,2.18724);
    fHistGen->SetBinError(388,1.31989);
    fHistGen->SetBinError(389,1.36082);
    fHistGen->SetBinError(392,0.566273);
    fHistGen->SetBinError(409,4.03836);
    fHistGen->SetBinError(410,9.35968);
    fHistGen->SetBinError(411,9.01063);
    fHistGen->SetBinError(412,8.04586);
    fHistGen->SetBinError(413,7.83907);
    fHistGen->SetBinError(414,7.52192);
    fHistGen->SetBinError(415,7.84364);
    fHistGen->SetBinError(416,9.49134);
    fHistGen->SetBinError(417,9.28317);
    fHistGen->SetBinError(418,5.82483);
    fHistGen->SetBinError(419,6.53993);
    fHistGen->SetBinError(420,4.78481);
    fHistGen->SetBinError(421,3.69996);
    fHistGen->SetBinError(422,1.97261);
    fHistGen->SetBinError(423,2.69039);
    fHistGen->SetBinError(424,8.49908);
    fHistGen->SetBinError(425,0.735782);
    fHistGen->SetBinError(426,0.806922);
    fHistGen->SetBinError(443,4.31701);
    fHistGen->SetBinError(444,5.70737);
    fHistGen->SetBinError(445,10.7079);
    fHistGen->SetBinError(446,14.4318);
    fHistGen->SetBinError(447,8.44301);
    fHistGen->SetBinError(448,14.4405);
    fHistGen->SetBinError(449,11.9602);
    fHistGen->SetBinError(450,10.5317);
    fHistGen->SetBinError(451,9.3791);
    fHistGen->SetBinError(452,8.94829);
    fHistGen->SetBinError(453,9.87964);
    fHistGen->SetBinError(454,8.94586);
    fHistGen->SetBinError(455,6.49625);
    fHistGen->SetBinError(456,7.76538);
    fHistGen->SetBinError(457,5.82302);
    fHistGen->SetBinError(458,3.35909);
    fHistGen->SetBinError(459,3.37859);
    fHistGen->SetBinError(460,1.43803);
    fHistGen->SetBinError(461,1.08765);
    fHistGen->SetBinError(462,1.43099);
    fHistGen->SetBinError(477,1.27579);
    fHistGen->SetBinError(478,0.658307);
    fHistGen->SetBinError(479,2.05019);
    fHistGen->SetBinError(480,2.25682);
    fHistGen->SetBinError(481,3.41608);
    fHistGen->SetBinError(482,3.18205);
    fHistGen->SetBinError(483,4.32242);
    fHistGen->SetBinError(484,4.69668);
    fHistGen->SetBinError(485,4.98069);
    fHistGen->SetBinError(486,5.84808);
    fHistGen->SetBinError(487,6.34513);
    fHistGen->SetBinError(488,6.23752);
    fHistGen->SetBinError(489,10.034);
    fHistGen->SetBinError(490,8.30106);
    fHistGen->SetBinError(491,6.16805);
    fHistGen->SetBinError(492,8.48212);
    fHistGen->SetBinError(493,3.49508);
    fHistGen->SetBinError(494,3.00593);
    fHistGen->SetBinError(495,8.44607);
    fHistGen->SetBinError(496,1.33608);
    fHistGen->SetBinError(497,0.635383);
    fHistGen->SetBinError(519,0.621134);
    fHistGen->SetBinError(520,1.59326);
    fHistGen->SetBinError(521,1.45161);
    fHistGen->SetBinError(522,2.3868);
    fHistGen->SetBinError(523,4.02067);
    fHistGen->SetBinError(524,4.31271);
    fHistGen->SetBinError(525,5.87375);
    fHistGen->SetBinError(526,8.03439);
    fHistGen->SetBinError(527,5.13243);
    fHistGen->SetBinError(528,5.53414);
    fHistGen->SetBinError(529,3.9201);
    fHistGen->SetBinError(530,2.31358);
    fHistGen->SetBinError(531,1.21196);
    fHistGen->SetBinError(532,1.32712);
    fHistGen->SetBinError(533,0.938508);
    fHistGen->SetBinError(559,1.88804);
    fHistGen->SetBinError(560,2.5798);
    fHistGen->SetBinError(561,3.68739);
    fHistGen->SetBinError(562,5.22303);
    fHistGen->SetBinError(563,8.74782);
    fHistGen->SetBinError(564,5.18326);
    fHistGen->SetBinError(565,4.4082);
    fHistGen->SetBinError(566,3.62539);
    fHistGen->SetBinError(567,1.48124);
    fHistGen->SetBinError(594,0.644825);
    fHistGen->SetBinError(595,1.20748);
    fHistGen->SetBinError(596,1.25569);
    fHistGen->SetBinError(597,3.51875);
    fHistGen->SetBinError(598,6.2228);
    fHistGen->SetBinError(599,4.51697);
    fHistGen->SetBinError(600,4.87622);
    fHistGen->SetBinError(601,2.80298);
    fHistGen->SetBinError(602,1.97336);
    fHistGen->SetBinError(631,0.724829);
    fHistGen->SetBinError(632,1.73131);
    fHistGen->SetBinError(633,3.56913);
    fHistGen->SetBinError(634,3.75112);
    fHistGen->SetBinError(635,5.99412);
    fHistGen->SetBinError(636,2.824);
    fHistGen->SetBinError(637,3.73978);
    fHistGen->SetBinError(638,1.1778);
    fHistGen->SetBinError(667,2.30543);
    fHistGen->SetBinError(668,2.7054);
    fHistGen->SetBinError(669,5.46481);
    fHistGen->SetBinError(670,4.51904);
    fHistGen->SetBinError(671,3.08331);
    fHistGen->SetBinError(672,1.83774);
    fHistGen->SetBinError(703,3.029);
    fHistGen->SetBinError(704,3.95402);
    fHistGen->SetBinError(705,4.30052);
    fHistGen->SetBinError(706,2.75811);
    fHistGen->SetBinError(707,1.37342);
    fHistGen->SetBinError(737,0.00155579);
    fHistGen->SetBinError(738,2.03144);
    fHistGen->SetBinError(739,3.23178);
    fHistGen->SetBinError(740,4.95917);
    fHistGen->SetBinError(741,2.31424);
    fHistGen->SetBinError(742,1.50597);
    fHistGen->SetBinError(772,0.963826);
    fHistGen->SetBinError(773,2.4606);
    fHistGen->SetBinError(774,2.8066);
    fHistGen->SetBinError(775,4.77836);
    fHistGen->SetBinError(776,1.37582);
    fHistGen->SetBinError(808,2.15396);
    fHistGen->SetBinError(809,3.97324);
    fHistGen->SetBinError(810,3.31248);
    fHistGen->SetBinError(811,1.6823);
    fHistGen->SetBinError(840,0.762896);
    fHistGen->SetBinError(842,1.07559);
    fHistGen->SetBinError(843,3.40029);
    fHistGen->SetBinError(844,2.87467);
    fHistGen->SetBinError(845,1.34203);
    fHistGen->SetBinError(876,0.710711);
    fHistGen->SetBinError(877,2.5751);
    fHistGen->SetBinError(878,3.20324);
    fHistGen->SetBinError(879,2.96561);
    fHistGen->SetBinError(880,1.32045);
    fHistGen->SetBinError(912,2.36879);
    fHistGen->SetBinError(913,3.05941);
    fHistGen->SetBinError(914,1.24583);
    fHistGen->SetBinError(946,0.675937);
    fHistGen->SetBinError(947,3.51362);
    fHistGen->SetBinError(948,3.1815);
    fHistGen->SetBinError(980,0.798667);
    fHistGen->SetBinError(981,2.2024);
    fHistGen->SetBinError(982,2.07579);
    fHistGen->SetBinError(1015,1.05812);
    fHistGen->SetBinError(1016,2.56546);
    fHistGen->SetBinError(1050,1.09343);
    fHistGen->SetBinError(1084,0.689512);
    fHistGen->SetBinError(1118,0.594383);
    fHistGen->SetBinError(1119,0.794655);
    fHistGen->SetBinError(1153,1.45493);
    fHistGen->SetEntries(3754);
    fHistGen->GetXaxis()->SetTitle("#alpha");
    fHistGen->GetYaxis()->SetTitle("E [GeV]");
    fHistGen->GetZaxis()->SetTitle("Entries");
}
