//____________________________________________________________________
//
// Single-top FCNC tZ analysis: tri-lepton base class
// 
//  
// Author:  <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2016 (C) 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_Base3L
#include <AtlSgTopFCNC_tZ_Base3L.h>
#endif

#include <cassert>
#include <cmath>

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_Base3L);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_Base3L::AtlSgTopFCNC_tZ_Base3L(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base(OutputFileName) {
    //
    // Default constructor
    //
    fReconstructedTopFrom = 0;
    fReconstructedWFrom = 0;
    fReconstructedNeutrinoFrom = 0;
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_Base3L::~AtlSgTopFCNC_tZ_Base3L() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::Clear(Option_t * option) {
    //
    // Clear variables and lists
    //
    fReconstructedTopFrom = 0;
    fReconstructedWFrom = 0;
    fReconstructedNeutrinoFrom = 0;
    AtlSgTopFCNC_tZ_Base::Clear(option);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fNLeptons_min = 3;
    fNLeptons_max = 3;
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Base3L::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Check whether SingleTopAnalysis "triggered" on the event already.
    //
    // Do not perform this check when running the AtlBDecayGenTool,
    // because this check would spoil the event selection since the
    // required "trigger" information does not exist of course in this
    // case.
    AtlAnalysisTool *tool = (AtlAnalysisTool*)GetTool("AtlBDecayGenTool");
    if ( tool != 0 && !tool->IsOff() ) return kTRUE;
    return (fEvent->GetEventHeader()->TestPreselectionFlagsAny(AtlEventHeader::kElElEl |
							       AtlEventHeader::kElElMu |
							       AtlEventHeader::kElMuMu |
							       AtlEventHeader::kMuMuMu));
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    AtlSgTopFCNC_tZ_Base::Print(option);
}
    
//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::ReconstructZ0Top() {
    //
    // Reconstruct tZ final state
    //
    // (1) Reconstruct all Z->l+l- candidates and sort them w.r.t.
    // their proximity to the true Z mass. Accept only candidates
    // within the given Z mass window
    //
    // (2) Reconstruct the top-quark from Et_miss, the leading jet
    // and the lepton with the highest Pt value which is not used for
    // reconstruction of the best Z candidate
    //

    // Reconstruct all Z0 candidates;
    // require at least one Z0 candidate
    ReconstructZ0Decays();
    if ( !fReconstructTop ) return;
    if ( fEvent->GetN_Z0Decays() < 1 ) return;
    AtlJet * jet = static_cast<AtlJet *>(fJets->First());
    if ( !jet ) return;
    HepZ0Decay *Z0 = GetZ0DecayBest();
    HepParticle *Z0daughter1 = (HepParticle*)Z0->GetDaughter1();
    HepParticle *Z0daughter2 = (HepParticle*)Z0->GetDaughter2();
    
    // Reconstruct top candidate
    for ( Int_t i = 0; i < fLeptons->GetEntries(); i++ ) {
	HepParticle *lep = (HepParticle*)fLeptons->At(i);
	if ( lep != Z0daughter1 && lep != Z0daughter2 ) {
	    ReconstructTopDecay(lep, jet);
	    break;
	}
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::ReconstructTopDecay(HepParticle *lepton,
						 AtlJet *jet) {
    //
    // Reconstruct the t -> b+lnu candidate for Et_miss, the given
    // lepton and the given jet, and store the result if it's inside
    // the allowed top-mass window
    //
    assert( lepton );
    assert( jet );
    ReconstructWDecay(lepton);

    if ( fReconstructedTopFrom ) {
        assert( fReconstructedTopFrom == jet );
        return;
    }
    fReconstructedTopFrom = jet;

    if ( fEvent->GetN_WDecaysLNu() < 1 ) return;
    AtlWDecayLNu *W = (AtlWDecayLNu*)fEvent->GetWDecaysLNu()->At(0);
    TLorentzVector p_top;
    p_top.SetPxPyPzE(W->Px() + jet->Px(),
		     W->Py() + jet->Py(),
		     W->Pz() + jet->Pz(),
		     W->E()  + jet->E());
    Float_t m_top = p_top.M();
    if ( fVetoTop ) {
	if ( m_top > fMassTop_min && m_top < fMassTop_max ) return;
    } else {
	if ( m_top < fMassTop_min || m_top > fMassTop_max ) return;
    }
    fEvent->AddTopDecay(W->Px() + jet->Px(),
			W->Py() + jet->Py(),
			W->Pz() + jet->Pz(),
			W->E()  + jet->E(),
			W, jet, jet->Px(),
			jet->Py(), jet->Pz(), jet->E(),
			HepTopDecay::kOther);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::ReconstructWDecay(HepParticle *lep) {
    //
    // Reconstruct W->lnu decay for Et_miss and the given lepton
    //
    if ( fReconstructedWFrom ) {
        assert( lep == fReconstructedWFrom );
        return;
    }
    fReconstructedWFrom = lep;

    ReconstructNeutrino(lep);
    if ( fEvent->GetN_Neutrinos() < 1 ) return;
    HepParticle *nu = (HepParticle*)fEvent->GetNeutrinos()->At(0);    
    Double_t Px_W = lep->Px() + nu->Px();
    Double_t Py_W = lep->Py() + nu->Py();
    Double_t Pz_W = lep->Pz() + nu->Pz();
    Double_t E_W  = lep->E()  + nu->E();
    fEvent->AddWDecayLNu(Px_W, Py_W, Pz_W, E_W,
			 lep, lep->Px(), lep->Py(), lep->Pz(), lep->E(),
			 nu, HepWDecay::kOther);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::ReconstructNeutrino(HepParticle *lep) {
    //
    // Reconstruct neutrino 4-momentum from Et_miss and the given
    // lepton using the W mass constraint.
    //
    // See "Implementation of a kinematic fit of single top-quark production
    // in association with a W boson and its application in a neural-network-
    // based analysis in ATLAS" by Thomas Loddenkoetter for the derivation
    // of the formulae with a detailled discussion.
    //

    if ( fReconstructedNeutrinoFrom ) {
        assert( fReconstructedNeutrinoFrom == lep );
        return;
    }
    fReconstructedNeutrinoFrom = lep;

    TVector3 const & lepton = lep->P3();
    double PxMiss = fEvent->GetEnergySum()->GetMissingEt().Px();
    double PyMiss = fEvent->GetEnergySum()->GetMissingEt().Py();
    double EtMiss2 = pow(PxMiss, 2) + pow(PyMiss, 2);
    double PzMiss;

	{
	    double A0 = pow(80.399, 2) / 2.;
	    double A = A0 + lepton.Px() * PxMiss + lepton.Py() * PyMiss;
	    double Delta = lepton.Mag2() * (A * A - (EtMiss2 * lepton.Perp2()));
    
	    if (Delta >= 0.) {
		/* two real-valued solutions: ((A * Pz(lep) +/- sqrt(Delta)) / Pt(lep)**2) -- choose the smaller one */
		double AZ = A * lepton.Pz();
		PzMiss = (AZ - copysign(sqrt(Delta), AZ)) / lepton.Perp2();
	    }
	    else {
		/* complex solutions -- scale MET down such that Delta becomes 0, yielding a single real solution */
		double f = A0 / (sqrt(EtMiss2) * lepton.Perp() - lepton.Px() * PxMiss - lepton.Py() * PyMiss);
		PxMiss *= f;
		PyMiss *= f;
		EtMiss2 = pow(PxMiss, 2) + pow(PyMiss, 2);
		A = A0 + lepton.Px() * PxMiss + lepton.Py() * PyMiss;
		PzMiss = (A * lepton.Pz()) / lepton.Perp2();
	    }
	}

	Int_t pdg = 0;
	if ( lep->IsEPlus() ) {
	    pdg =  12;
	} else if ( lep->IsEMinus() ) {
	    pdg = -12;
	} else if ( lep->IsMuPlus() ) {
	    pdg =  14;
	} else if ( lep->IsMuMinus() ) {
	    pdg = -14;
	} else {
	    Error("ReconstructNeutrino", "Wrong lepton type. Abort!");
	    gSystem->Abort(1);
	}
	fEvent->AddNeutrino(PxMiss, PyMiss, PzMiss, sqrt(EtMiss2 + PzMiss * PzMiss), pdg);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::BookHistograms() {
    //
    // Book histograms
    //
    AtlSgTopFCNC_tZ_Base::BookHistograms();
    
    // Histograms w/o any cut
    BookHistogramsTop(fHistsNoCuts);

    // Histograms after pre-selection
    BookHistogramsTop(fHistsPreSel);

    // Histograms w/o b-tagging 
    BookHistogramsTop(fHistsPreTag);

    // Histograms w/ all cuts applied
    BookHistogramsTop(fHistsAllCuts);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::BookHistogramsTop(AtlHistogramTool *htool) {
    //
    // Book histograms for top reconstruction
    //
 
    // Invariant top-quark mass
    htool->Add("top/h_top_M", "Top Invariant Mass",
	       10, 0., 500., "M_{top} [GeV]", "Events");

    // Charge
    htool->Add("top/h_top_Chg", "Top-quark Charge",
	       2, -1., 1., "Chg(top)", "Entries");
   
    // Transverse momentum
    htool->Add("top/h_top_Pt", "Top transverse momentum",
	       10, 0., 500., "top p_{T} [GeV]", "Events");
    
    // Pseudo-rapidity
    htool->Add("top/h_top_Eta", "Top pseudo-rapidity",
	       8, -5., 5., "top #eta", "Events");
    
    // Azimuth
    htool->Add("top/h_top_Phi", "Top azimuth",
	       8, -M_PI, M_PI, "top #phi [rad]", "Events");

    // Invariant W mass
    htool->Add("top/h_W_M", "W Invariant Mass",
	       10, 0., 150., "M_{W} [GeV]", "Events");

    // Transverse W mass
    htool->Add("top/h_W_Mt", "W Transverse Mass",
	       10, 0., 250., "M_{T, W} [GeV]", "Events");

    // Neutrino Pz
    htool->Add("top/h_nu_Pz", "Neutrino P_{z}",
	       50, -200., 200., "p_{z}(#nu) [GeV]", "Events"); 
	
    //TH2 ZPt vs topM
    htool->Add("top/h_ZPt_topM", "Z transverse momentum vs Top Mass",
	       5, 0., 500., 5, 0., 400.,"M(top) [GeV]","Z candidate p_{T} [GeV]", "Events");
    //TH2 ZPt vs MtW
    htool->Add("top/h_ZPt_MtW", "Z transverse momentum vs W Transverse Mass",
	       5, 0., 250., 5, 0., 400.,"M_{T,W} [GeV]","Z candidate p_{T} [GeV]", "Events");
/*
// Pt_rel (MC only; for fake studies)
htool->Add("top/h_el_ptrel_photoconv", "Pt_rel, el from photo conversion",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_el_ptrel_bmeson", "Pt_rel, el from B meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_el_ptrel_cmeson", "Pt_rel, el from c meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_el_ptrel_bbaryon", "Pt_rel, el from B baryon decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_el_ptrel_top", "Pt_rel, el from top decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");

// DeltaR (MC only; for fake studies)
htool->Add("top/h_el_deltaR_photoconv", "DeltaR, el from photo conversion",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_el_deltaR_bmeson", "DeltaR, el from B meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_el_deltaR_cmeson", "DeltaR, el from c meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_el_deltaR_bbaryon", "DeltaR, el from B baryon decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_el_deltaR_top", "DeltaR, el from top decays",
50, 0., 10., "#Delta R", "Entries");

// Pt_rel (MC only; for fake studies)
htool->Add("top/h_mu_ptrel_photoconv", "Pt_rel, mu from photo conversion",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_mu_ptrel_bmeson", "Pt_rel, mu from B meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_mu_ptrel_cmeson", "Pt_rel, mu from c meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_mu_ptrel_bbaryon", "Pt_rel, mu from B baryon decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("top/h_mu_ptrel_top", "Pt_rel, mu from top decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");

// DeltaR (MC only; for fake studies)
htool->Add("top/h_mu_deltaR_photoconv", "DeltaR, mu from photo conversion",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_mu_deltaR_bmeson", "DeltaR, mu from B meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_mu_deltaR_cmeson", "DeltaR, mu from c meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_mu_deltaR_bbaryon", "DeltaR, mu from B baryon decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("top/h_mu_deltaR_top", "DeltaR, mu from top decays",
50, 0., 10., "#Delta R", "Entries");
*/

    // MC truth origin (for fake studies)
    TH1D *h_el_top_tru_orig = htool->Add("top/h_el_top_truth_origin", "MC Particle Origin, electrons from top candidates",
					 AtlMCTruthClassifier::kNumParticleOrigin,
					 0, AtlMCTruthClassifier::kNumParticleOrigin,
					 "Electron from top", "Entries");
    SetParticleOriginHistBinLabels(h_el_top_tru_orig);
    TH1D *h_mu_top_tru_orig = htool->Add("top/h_mu_top_truth_origin", "MC Particle Origin, muons from top candidates",
					 AtlMCTruthClassifier::kNumParticleOrigin,
					 0, AtlMCTruthClassifier::kNumParticleOrigin,
					 "Muon from top", "Entries");
    SetParticleOriginHistBinLabels(h_mu_top_tru_orig);
    
    // MC truth b-quarks (ttbar only)
    htool->Add("top/h_deltaR_b_untagged_bjet", "DeltaR, untagged b-quark to b-jet",
	       50, 0., 5., "#DeltaR", "Entries");
    htool->Add("top/h_deltaR_b_tagged_bjet", "DeltaR, untagged b-quark to b-jet",
	       50, 0., 5., "#DeltaR", "Entries");
    
    htool->Add("top/h_el_top_bquarks_Wboson_eta", "b-quarks from ttbar, e from W, eta",
	       20, -10., 10., "b-quark #eta", "Entries");
    htool->Add("top/h_el_top_bquarks_Bmeson_eta", "b-quarks from ttbar, e from B, eta",
	       20, -10., 10., "b-quark #eta", "Entries");
    htool->Add("top/h_mu_top_bquarks_Wboson_eta", "b-quarks from ttbar, mu from W, eta",
	       20, -10., 10., "b-quark #eta", "Entries");
    htool->Add("top/h_mu_top_bquarks_Bmeson_eta", "b-quarks from ttbar, mu from B, eta",
	       20, -10., 10., "b-quark #eta", "Entries");
    
    htool->Add("top/h_el_top_bquarks_Wboson_pt", "b-quarks from ttbar, e from W, pT",
	       100, 0., 200., "b-quark p_{T} [GeV]", "Entries");
    htool->Add("top/h_el_top_bquarks_Bmeson_pt", "b-quarks from ttbar, e from B, pT",
	       100, 0., 200., "b-quark p_{T} [GeV]", "Entries");
    htool->Add("top/h_mu_top_bquarks_Wboson_pt", "b-quarks from ttbar, mu from W, pT",
	       100, 0., 200., "b-quark p_{T} [GeV]", "Entries");
    htool->Add("top/h_mu_top_bquarks_Bmeson_pt", "b-quarks from ttbar, mu from B, pT",
	       100, 0., 200., "b-quark p_{T} [GeV]", "Entries");
    
    htool->Add("top/h_deltaR_el_Bmeson_b_untagged", "DeltaR, e from B-meson to untagged b-quark",
	       40, 0., 1., "#DeltaR", "Enetries");
    htool->Add("top/h_deltaR_mu_Bmeson_b_untagged", "DeltaR, mu from B-meson to untagged b-quark",
	       40, 0., 1., "#DeltaR", "Enetries");
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::FillHistogramsTop(AtlHistogramTool *htool,
					       Double_t w) {
    //
    // Fill all histograms with the given weight for the reconstructed
    // semi-leptonic top candidate
    //
    // The transverse W mass is comupte using the common function
    // AtlEvent::W_Mperp() which is independend from the neutrino
    // reconstruction
    //
    if ( fEvent->GetN_TopDecays() != 0 ){
	HepZ0Decay *Z0 = GetZ0DecayBest();
	TLorentzVector p_Z = Z0->P();	
	HepTopDecay *top = (HepTopDecay*)fEvent->GetTopDecays()->At(0);
	TLorentzVector p_top = top->P();
	AtlWDecayLNu *Wboson = (AtlWDecayLNu*)top->GetWDecay();
	TLorentzVector p_W = Wboson->P();
	HepParticle *nu = Wboson->GetNeutrino();
	HepParticle *lep = (HepParticle*)Wboson->GetChargedLeptonOrig();
	Float_t MtW = fEvent->W_Mperp(lep);
	htool->Fill("top/h_top_M",   p_top.M(),     w);
	htool->Fill("top/h_top_Chg", top->Charge(), w);
	htool->Fill("top/h_top_Pt",  p_top.Pt(),    w);
	htool->Fill("top/h_top_Eta", p_top.Eta(),   w);
	htool->Fill("top/h_top_Phi", p_top.Phi(),   w);
	htool->Fill("top/h_W_M",     p_W.M(),       w);
	htool->Fill("top/h_W_Mt",    MtW,           w);
	htool->Fill("top/h_nu_Pz",   nu->Pz(),      w);
	htool->Fill("top/h_ZPt_topM",p_top.M() ,p_Z.Pt(), w);
	htool->Fill("top/h_ZPt_MtW", MtW ,p_Z.Pt(), w);
	
	// Fill MC truth origin
	if ( fEvent->IsMC() && fEvent->GetN_MCParticles() > 1 ) {
	    HepMCParticle *prt_b1 = (HepMCParticle*)fEvent->GetMCParticles()->At(0);
	    HepMCParticle *prt_b2 = (HepMCParticle*)fEvent->GetMCParticles()->At(1);
	    AtlJet *bjet = (AtlJet*)fJets->First();
	    
	    // Find untagged b-quark
	    HepMCParticle *prt_but = prt_b1;
	    HepMCParticle *prt_bt  = prt_b2;
	    if ( prt_b2->DeltaR(bjet) > prt_b1->DeltaR(bjet) ) {
		prt_but = prt_b2;
		prt_bt  = prt_b1;
	    }
	    htool->Fill("top", "h_deltaR_b_untagged_bjet", prt_but->DeltaR(bjet), w);
	    htool->Fill("top", "h_deltaR_b_tagged_bjet",   prt_bt->DeltaR(bjet), w);
	    
	    if ( lep->IsElectron() ) {
		htool->Fill("top", "h_el_top_truth_origin",
			    (Int_t)(((AtlElectron*)lep)->GetMCTruthClassifier()->GetParticleOrigin()), w);
		/*	// Fill pt_rel histograms
			AtlMCTruthClassifier::EParticleOrigin origin =((AtlElectron*)lep)->GetMCTruthClassifier()->GetParticleOrigin();
			Double_t pt_rel = 0.; Double_t DeltaR = 0.;
			fEvent->ComputePtRel(lep, fJets, 10., pt_rel, DeltaR);
			if ( origin ==  AtlMCTruthClassifier::kPhotonConv ) {
			htool->Fill("top/h_el_ptrel_photoconv", pt_rel, w);
			htool->Fill("top/h_el_deltaR_photoconv", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kBottomMeson ) {
			htool->Fill("top/h_el_ptrel_bmeson", pt_rel, w);
			htool->Fill("top/h_el_deltaR_bmeson", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kCharmedMeson ) {
			htool->Fill("top/h_el_ptrel_cmeson", pt_rel, w);
			htool->Fill("top/h_el_deltaR_cmeson", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kBottomBaryon ) {
			htool->Fill("top/h_el_ptrel_bbaryon", pt_rel, w);
			htool->Fill("top/h_el_deltaR_bbaryon", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::ktop ) {
			htool->Fill("top/h_el_ptrel_top", pt_rel, w);
			htool->Fill("top/h_el_deltaR_top", DeltaR, w);
					}*/
		if ( ((AtlElectron*)lep)->GetMCTruthClassifier()->IsFromWBoson() ) {
		    htool->Fill("top", "h_el_top_bquarks_Wboson_eta", prt_b1->Eta(), w);
		    htool->Fill("top", "h_el_top_bquarks_Wboson_eta", prt_b2->Eta(), w);
		    htool->Fill("top", "h_el_top_bquarks_Wboson_pt", prt_b1->Pt(), w);
		    htool->Fill("top", "h_el_top_bquarks_Wboson_pt", prt_b2->Pt(), w);
		} else if ( ((AtlElectron*)lep)->GetMCTruthClassifier()->IsFromBottomMeson() ) {
		    htool->Fill("top", "h_el_top_bquarks_Bmeson_eta", prt_b1->Eta(), w);
		    htool->Fill("top", "h_el_top_bquarks_Bmeson_eta", prt_b2->Eta(), w);
		    htool->Fill("top", "h_el_top_bquarks_Bmeson_pt", prt_b1->Pt(), w);
		    htool->Fill("top", "h_el_top_bquarks_Bmeson_pt", prt_b2->Pt(), w);
		    htool->Fill("top", "h_deltaR_el_Bmeson_b_untagged", prt_but->DeltaR(lep), w);
		}
	    } else if ( lep->IsMuon() ) {
		htool->Fill("top", "h_mu_top_truth_origin",
			    (Int_t)(((AtlMuon*)lep)->GetMCTruthClassifier()->GetParticleOrigin()), w);
		/*	// Fill pt_rel histograms
			AtlMCTruthClassifier::EParticleOrigin origin =((AtlMuon*)lep)->GetMCTruthClassifier()->GetParticleOrigin();
			Double_t pt_rel = 0.; Double_t DeltaR = 0.;
			fEvent->ComputePtRel(lep, fJets, 10., pt_rel, DeltaR);
			if ( origin ==  AtlMCTruthClassifier::kPhotonConv ) {
			htool->Fill("top/h_mu_ptrel_photoconv", pt_rel, w);
			htool->Fill("top/h_mu_deltaR_photoconv", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kBottomMeson ) {
			htool->Fill("top/h_mu_ptrel_bmeson", pt_rel, w);
			htool->Fill("top/h_mu_deltaR_bmeson", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kCharmedMeson ) {
			htool->Fill("top/h_mu_ptrel_cmeson", pt_rel, w);
			htool->Fill("top/h_mu_deltaR_cmeson", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::kBottomBaryon ) {
			htool->Fill("top/h_mu_ptrel_bbaryon", pt_rel, w);
			htool->Fill("top/h_mu_deltaR_bbaryon", DeltaR, w);
			} else if ( origin == AtlMCTruthClassifier::ktop ) {
			htool->Fill("top/h_mu_ptrel_top", pt_rel, w);
			htool->Fill("top/h_mu_deltaR_top", DeltaR, w);
			}*/
		if ( ((AtlMuon*)lep)->GetMCTruthClassifier()->IsFromWBoson() ) {
		    htool->Fill("top", "h_mu_top_bquarks_Wboson_eta", prt_b1->Eta(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Wboson_eta", prt_b2->Eta(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Wboson_pt", prt_b1->Pt(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Wboson_pt", prt_b2->Pt(), w);
		} else if ( ((AtlMuon*)lep)->GetMCTruthClassifier()->IsFromBottomMeson() ) {
		    htool->Fill("top", "h_mu_top_bquarks_Bmeson_eta", prt_b1->Eta(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Bmeson_eta", prt_b2->Eta(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Bmeson_pt", prt_b1->Pt(), w);
		    htool->Fill("top", "h_mu_top_bquarks_Bmeson_pt", prt_b2->Pt(), w);
		    htool->Fill("top", "h_deltaR_mu_Bmeson_b_untagged", prt_but->DeltaR(lep), w);
		}
	    }
	}
    }	
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::CreatePieCharts() {
    //
    // Create pie charts from particle origin histograms
    //

    // Leptons from top decays
    CreateParticleOriginPie("/NoCuts/top/h_el_top_truth_origin");
    CreateParticleOriginPie("/NoCuts/top/h_mu_top_truth_origin");
    CreateParticleOriginPie("/PreSel/top/h_el_top_truth_origin");
    CreateParticleOriginPie("/PreSel/top/h_mu_top_truth_origin");
    CreateParticleOriginPie("/PreTag/top/h_el_top_truth_origin");
    CreateParticleOriginPie("/PreTag/top/h_mu_top_truth_origin");
    CreateParticleOriginPie("/AllCuts/top/h_el_top_truth_origin");
    CreateParticleOriginPie("/AllCuts/top/h_mu_top_truth_origin");
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base3L::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //
    CreatePieCharts();
    AtlSgTopFCNC_tZ_Base::Terminate();
}
