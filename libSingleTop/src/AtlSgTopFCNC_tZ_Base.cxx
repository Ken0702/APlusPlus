//____________________________________________________________________
//
// Selector base class for the single-top FCNC tZ analysis
// 
//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maia Kind
//
#ifndef SINGLETOP_AtlSgTopFCNC_tZ_Base
#include <AtlSgTopFCNC_tZ_Base.h>
#endif

#include <cmath>

#include <TH1.h>
#include <AtlEventHeader.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TList.h>
#include <TVector2.h>
#include <TVector3.h>
#include <AtlEvtWriterD3PDSgTopThinned.h>

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_Base);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_Base::AtlSgTopFCNC_tZ_Base(const char* OutputFileName) :
    AtlSelector(OutputFileName),
    fHistsNoCuts(0),
    fHistsPreSel(0),
    fHistsPreTag(0),
    fHistsAllCuts(0)
{
    //
    // Default constructor
    //
    fLeptons     = 0;
    fElectrons   = 0;
    fMuons       = 0;
    fJets        = 0;
    fBJets       = 0;
    fObjTool     = 0;
    fSFTool      = 0;
    fReconstructedZ = kFALSE;
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_Base::~AtlSgTopFCNC_tZ_Base() {
    //
    // Default destructor
    //
    delete fHistsNoCuts;
    delete fHistsPreSel;
    delete fHistsPreTag;
    delete fHistsAllCuts;
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fNLeptons_min = 2;
    fNLeptons_max = 3;
    fNBTags_min   = 1;
    fNBTags_max   = 1;
    fPt_Lep1_min  = 15.;
    fPt_Lep2_min  = 15.;
    fPt_Lep3_min  = 15.;
    fMET_min      = 0.;
    fMET_max      = INFINITY;
    fMtW_min      = 0.;
    fMtW_max      = INFINITY;
    fMassZ_min    = 60.;
    fMassZ_max    = 120.;
    fMassTop_min  = 0.;
    fMassTop_max  = INFINITY;
    fReconstructTop = kTRUE;
    fRequireZ = kTRUE;
    fRequireTop = kTRUE;
    fVetoZboson   = kFALSE;
    fVetoTop      = kFALSE;

}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    fTree->SetBranchStatus("*", kTRUE);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::Clear(Option_t * option) {
    //
    // Clear variables and lists
    //
    fReconstructedZ = kFALSE;
    AtlSelector::Clear(option);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::Begin(TTree * tree) {
    //
    // Begin analysis job
    //
    fEvtWriter = new AtlEvtWriterD3PDSgTopThinned();
    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::SlaveBegin(TTree * tree) {
    //
    // Initialise before the first event.
    //
    AtlSelector::SlaveBegin(tree);

    // Get ObjectSelectionTool + pointers to analysis objects
    fObjTool = (AtlObjectsToolD3PDSgTop*)
	GetTool("AtlObjectsToolD3PDSgTop", "", kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Get ScaleFactorTool
    fSFTool = (AtlObjRecoScaleFactorTool*)
	GetTool("AtlObjRecoScaleFactorTool", "", kTRUE);
    // Provide SF tool with pointers to analysis objects
    if ( fSFTool ) {
	fSFTool->SetObjDefinitionTool(fObjTool);
        fSFTool->SetLeptons(fLeptons);
        fSFTool->SetJets(fJets);
    }

    // Initialise event writer
    if ( AtlEvtWriterD3PDSgTopThinned *evtWriter
	 = dynamic_cast<AtlEvtWriterD3PDSgTopThinned*>(fEvtWriter) ) {
        evtWriter->SetLeptons(fLeptons);
        evtWriter->SetJets(fJets, fBJets);
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    AtlSelector::Print();
    cout << endl
	 << "  fNLeptons_min  = " << fNLeptons_min << endl
	 << "  fNLeptons_max  = " << fNLeptons_max << endl
	 << "  fNBTags_min    = " << fNBTags_min << endl
	 << "  fNBTags_max    = " << fNBTags_max << endl
	 << "  fPt_Lep1_min   = " << fPt_Lep1_min << " GeV" << endl
	 << "  fPt_Lep2_min   = " << fPt_Lep2_min << " GeV" << endl
	 << "  fPt_Lep3_min   = " << fPt_Lep3_min << " GeV" << endl
	 << "  fMET_min       = " << fMET_min << " GeV" << endl
	 << "  fMET_max       = " << fMET_max << " GeV" << endl
	 << "  fMtW_min       = " << fMtW_min << " GeV" << endl
	 << "  fMtW_max       = " << fMtW_max << " GeV" << endl
	 << "  fMassZ_min     = " << fMassZ_min << " GeV" << endl
	 << "  fMassZ_max     = " << fMassZ_max << " GeV" << endl
	 << "  fMassTop_min   = " << fMassTop_min << " GeV" << endl
	 << "  fMassTop_max   = " << fMassTop_max << " GeV" << endl;
    cout << "  fVetoZboson    = ";
    if ( fVetoZboson ) {
    	cout << "true" << endl;
    } else {
    	cout << "false" << endl;
    }
    cout << "  fVetoTop       = ";
    if ( fVetoTop ) {
    	cout << "true" << endl;
    } else {
    	cout << "false" << endl;
    }

}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //
    // Always call AtlSelector::Terminate() at the very end, since
    // there the output file is written.
    //
    AtlSgTopFCNC_tZ_Base::CreatePieCharts();
    AtlSelector::Terminate();
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::ReconstructZ0Decays() {
    //
    // Reconstruct all Z -> l+l- candidates
    //
    // Loop over all ee, mumu combinations and store all Z candidates
    // inside the mass window (outside in case of the veto) in
    // descending order w.r.t. their proxomity to the true Z mass (ie.
    // 1st candidate = worst; last candidate = best)
    //
    if ( fReconstructedZ )
        return;
    fReconstructedZ = kTRUE;
    HepParticle *lep1 = 0;
    HepParticle *lep2 = 0;
    for ( Int_t i = 0; i < fLeptons->GetEntries()-1; i++ ) {
	for ( Int_t j = i+1; j < fLeptons->GetEntries(); j++ ) {
	    if ( i == j ) continue;
	    lep1 = (HepParticle*)fLeptons->At(i);
	    lep2 = (HepParticle*)fLeptons->At(j);
	    if ( lep1->IsElectron() && lep2->IsMuon() ) continue;
	    if ( lep1->IsMuon() && lep2->IsElectron() ) continue;
	    if ( lep1->Charge() * lep2->Charge() != -1. ) continue;

	    TLorentzVector p_Z = lep1->P() + lep2->P();
	    Float_t m_Z = p_Z.M();
	    if ( fVetoZboson ) {
		if ( m_Z > fMassZ_min && m_Z < fMassZ_max ) continue;
	    } else {
		if ( m_Z < fMassZ_min || m_Z > fMassZ_max ) continue;
	    }
	    fEvent->AddZ0Decay(p_Z.X(), p_Z.Y(), p_Z.Z(), p_Z.E(),
			       lep1, lep2);
	}
    }
    // Sort candidates by their difference from the true Z
    // mass in descending order
    fEvent->GetZ0Decays()->Sort();
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistograms() {
    //
    // Book histograms
    //

    // Histograms w/o any cut
    fHistsNoCuts = new AtlHistogramTool("NoCuts", "Histograms without cuts");
    AddTool(fHistsNoCuts);
    BookHistogramsCommon(fHistsNoCuts);
    BookHistogramsZ0(fHistsNoCuts, "Z0_best");

    // Histograms after pre-selection
    fHistsPreSel = new AtlHistogramTool("PreSel", "Pre-selection histograms");     
    AddTool(fHistsPreSel);
    BookHistogramsCommon(fHistsPreSel);
    BookHistogramsZ0(fHistsPreSel, "Z0_best");

    // Histograms w/o b-tagging 
    fHistsPreTag = new AtlHistogramTool("PreTag", "Pre-tagged histograms");     
    AddTool(fHistsPreTag);
    BookHistogramsCommon(fHistsPreTag);
    BookHistogramsZ0(fHistsPreTag, "Z0_best");

    // Histograms w/ all cuts applied
    fHistsAllCuts = new AtlHistogramTool("AllCuts", "Histograms with all cuts");
    AddTool(fHistsAllCuts);
    BookHistogramsCommon(fHistsAllCuts);
    BookHistogramsZ0(fHistsAllCuts, "Z0_best");
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsCommon(AtlHistogramTool *htool) {
    //
    // Book common histograms for leptons, jets and the enrgy sums and
    // angles
    //
    BookHistogramsLeptons(htool);
    BookHistogramsJets(htool);
    BookHistogramsMET(htool);
}
    
//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsLeptons(AtlHistogramTool *htool) {
    //
    // Book common histograms for leptons
    //
    
    // =======
    // Leptons
    // =======

    // Leading lepton Pt
    htool->Add("leptons/h_lep1_Pt", "Leading Lepton Pt", 10, 0., 300.,
	       "Leading lepton p_{T} [GeV]", "Events");

    // Leading lepton eta
    htool->Add("leptons/h_lep1_Eta", "Leading Lepton Eta",
	       18, -4.5, 4.5, "Leading lepton #eta", "Events");

    // Leading lepton phi
    htool->Add("leptons/h_lep1_Phi", "Leading Lepton Azimuth",
	       8, -M_PI, M_PI, "Leading lepton #phi [rad]",
	       "Events");

    // Sub-leading lepton Pt
    htool->Add("leptons/h_lep2_Pt", "Sub-leading Lepton Pt",
	       10, 0., 250., "Sub-leading lepton p_{T} [GeV]",
	       "Events");
    
    // Sub-leading lepton eta
    htool->Add("leptons/h_lep2_Eta", "Sub-leading Lepton Eta",
	       18, -4.5, 4.5, "Sub-leading lepton #eta", "Events");
    
    // Sub-leading lepton phi
    htool->Add("leptons/h_lep2_Phi", "Sub-leading Lepton Azimuth",
	       8, -M_PI, M_PI, "Sub-leading lepton #phi [rad]",
	       "Events");

    // Third lepton Pt
    htool->Add("leptons/h_lep3_Pt", "Third Lepton Pt",
	       10, 0., 200., "Third lepton p_{T} [GeV]",
	       "Events");
 
    // Sub-leading lepton eta
    htool->Add("leptons/h_lep3_Eta", "Third Lepton Eta",
	       18, -4.5, 4.5, "Third lepton #eta", "Events");
    
    // Sub-leading lepton phi
    htool->Add("leptons/h_lep3_Phi", "Third Lepton Azimuth",
	       8, -M_PI, M_PI, "Third lepton #phi [rad]",
	       "Events");
    // Lepton charge
    htool->Add("leptons/h_lep_Chg", "Lepton Charge", 3, -1.5, 1.5,
	       "Lepton charge", "Entries");

    // No. of leptons per event
    htool->Add("leptons/h_lep_N", "No. of Leptons per Event",
	       5, -0.5, 4.5, "Number of leptons / event", "Events");

    // Pileup weigth per event
    htool->Add("leptons/h_pileup_weight", "Pileup Weight",
	       10, -0.5, 5.5, "Pileup weight", "Events");

    // Monte Carlo weigth per event
    htool->Add("leptons/h_mc_weight", "Monte Carlo Weight",
	       10, -0.5, 3.5, "log(Monte Carlo weight)", "Events");

    // Lepton weigth per event
    htool->Add("leptons/h_lepton_weight", "Lepton Weight",
	       10, -0.5, 2.5, "Lepton weight", "Events");
  
    // b Tagging weigth per event
    htool->Add("leptons/h_btag_weight", "b Tagging Weight",
	       10, -0.5, 2.5, "b tagging weight", "Events");

    // Total weigth per event
    htool->Add("leptons/h_total_weight", "Total Weight",
	       10, -0.5, 3.5, "Total weight", "Events");

    // =========
    // Electrons
    // =========

    // Leading electron Pt
    htool->Add("leptons/el/h_el1_Pt", "Leading Electron Pt", 10, 0., 300.,
	       "Leading electron p_{T} [GeV]", "Events");

    // Leading electron eta
    htool->Add("leptons/el/h_el1_Eta", "Leading Electron Eta",
	       8, -2.5, 2.5, "Leading electron #eta", "Events");

    // Leading electron phi
    htool->Add("leptons/el/h_el1_Phi", "Leading Electron Azimuth",
	       8, -M_PI, M_PI, "Leading electron #phi [rad]",
	       "Events");

    // Sub-leading electron Pt
    htool->Add("leptons/el/h_el2_Pt", "Sub-leading Electron Pt",
	       10, 0., 250., "Sub-leading electron p_{T} [GeV]",
	       "Events");
    
    // Sub-leading electron eta
    htool->Add("leptons/el/h_el2_Eta", "Sub-leading Electron Eta",
	       8, -2.5, 2.5, "Sub-leading electron #eta", "Events");
    
    // Sub-leading electron phi
    htool->Add("leptons/el/h_el2_Phi", "Sub-leading Electron Azimuth",
	       8, -M_PI, M_PI, "Sub-leading electron #phi [rad]",
	       "Events");

    // Electron charge
    htool->Add("leptons/el/h_el_Chg", "Electron Charge", 3, -1.5, 1.5,
	       "Electron charge", "Entries");
    
    // No. of electrons per event
    htool->Add("leptons/el/h_el_N", "No. of Electrons per Event",
	       5, -0.5, 4.5, "Number of electrons / event", "Events");
/*
// Pt_rel (MC only; for fake studies)
htool->Add("leptons/el/h_el_ptrel_photoconv", "Pt_rel, e from photo conversion",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/el/h_el_ptrel_bmeson", "Pt_rel, e from B meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/el/h_el_ptrel_zboson", "Pt_rel, e from Z decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/el/h_el_ptrel_top", "Pt_rel, e from top decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");

// DeltaR (MC only; for fake studies)
htool->Add("leptons/el/h_el_deltaR_photoconv", "DeltaR, e from photo conversion",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/el/h_el_deltaR_bmeson", "DeltaR, e from B meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/el/h_el_deltaR_zboson", "DeltaR, e from Z decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/el/h_el_deltaR_top", "DeltaR, e from top decays",
50, 0., 10., "#Delta R", "Entries");
*/
    // MC truth origin (for fake studies)
    TH1D *h_el_tru_orig = htool->Add("leptons/el/h_el_truth_origin", "MC Particle Origin for all Electrons",
				     AtlMCTruthClassifier::kNumParticleOrigin,
				     0, AtlMCTruthClassifier::kNumParticleOrigin,
				     "", "Entries");
    SetParticleOriginHistBinLabels(h_el_tru_orig);
    
    // =====
    // Muons
    // =====

    // Leading muon Pt
    htool->Add("leptons/mu/h_mu1_Pt", "Leading Muon Pt", 10, 0., 300.,
	       "Leading muon p_{T} [GeV]", "Events");

    // Leading muon eta
    htool->Add("leptons/mu/h_mu1_Eta", "Leading Muon Eta",
	       8, -2.5, 2.5, "Leading muon #eta", "Events");

    // Leading muon phi
    htool->Add("leptons/mu/h_mu1_Phi", "Leading Muon Azimuth",
	       8, -M_PI, M_PI, "Leading muon #phi [rad]",
	       "Events");

    // Sub-leading muon Pt
    htool->Add("leptons/mu/h_mu2_Pt", "Sub-leading Muon Pt",
	       10, 0., 250., "Sub-leading muon p_{T} [GeV]",
	       "Events");
    
    // Sub-leading muon eta
    htool->Add("leptons/mu/h_mu2_Eta", "Sub-leading Muon Eta",
	       8, -2.5, 2.5, "Sub-leading muon #eta", "Events");
    
    // Sub-leading muon phi
    htool->Add("leptons/mu/h_mu2_Phi", "Sub-leading Muon Azimuth",
	       8, -M_PI, M_PI, "Sub-leading muon #phi [rad]",
	       "Events");

    // Muon charge
    htool->Add("leptons/mu/h_mu_Chg", "Muon Charge", 3, -1.5, 1.5,
	       "Muon charge", "Entries");

    // No. of muons per event
    htool->Add("leptons/mu/h_mu_N", "No. of Muons per Event",
	       5, -0.5, 4.5, "Number of muons / event", "Events");
/*
// Pt_rel (MC only; for fake studies)
htool->Add("leptons/mu/h_mu_ptrel_photoconv", "Pt_rel, mu from photo conversion",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/mu/h_mu_ptrel_bmeson", "Pt_rel, mu from B meson decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/mu/h_mu_ptrel_zboson", "Pt_rel, mu from Z decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
htool->Add("leptons/mu/h_mu_ptrel_top", "Pt_rel, mu from top decays",
50, 0., 200., "p_{T}^{rel} [GeV]", "Entries");
    
// DeltaR (MC only; for fake studies)
htool->Add("leptons/mu/h_mu_deltaR_photoconv", "DeltaR, mu from photo conversion",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/mu/h_mu_deltaR_bmeson", "DeltaR, mu from B meson decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/mu/h_mu_deltaR_zboson", "DeltaR, mu from Z decays",
50, 0., 10., "#Delta R", "Entries");
htool->Add("leptons/mu/h_mu_deltaR_top", "DeltaR, mu from top decays",
50, 0., 10., "#Delta R", "Entries");
*/
    // MC truth origin (for fake studies)
    TH1D *h_mu_tru_orig = htool->Add("leptons/mu/h_mu_truth_origin", "MC Particle Origin for all Muons",
				     AtlMCTruthClassifier::kNumParticleOrigin,
				     0, AtlMCTruthClassifier::kNumParticleOrigin,
				     "", "Entries");
    SetParticleOriginHistBinLabels(h_mu_tru_orig);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsJets(AtlHistogramTool *htool) {
    //
    // Book common histograms for jets
    //

    // Leading jet Pt
    htool->Add("jets/h_jet1_Pt", "Leading Jet Pt",
	       10, 0., 300., "Leading jet p_{T} [GeV]", "Events");
    
    // Leading jet eta
    htool->Add("jets/h_jet1_Eta", "Leading Jet Eta",
	       18, -4.5, 4.5, "Leading jet #eta", "Events");
    
    // Leading jet phi
    htool->Add("jets/h_jet1_Phi", "Leading Jet Azimuth",
	       8, -M_PI, M_PI, "Leading jet #phi [rad]",
	       "Events");

    // Sub-leading jet Pt
    htool->Add("jets/h_jet2_Pt", "Sub-leading Jet Pt",
	       10, 0., 250., "Sub-leading jet p_{T} [GeV]", "Events");
    
    // Sub-leading jet eta
    htool->Add("jets/h_jet2_Eta", "Sub-leading Jet Eta",
	       18, -4.5, 4.5, "Sub-leading jet #eta", "Events");
    
    // Sub-leading jet phi
    htool->Add("jets/h_jet2_Phi", "Sub-leading Jet Azimuth",
	       8, -M_PI, M_PI, "Sub-leading jet #phi [rad]",
	       "Events");

    // Third jet Pt
    htool->Add("jets/h_jet3_Pt", "Third Jet Pt",
	       10, 0., 200., "Third jet p_{T} [GeV]", "Events");
    
    // Third jet eta
    htool->Add("jets/h_jet3_Eta", "Third Jet Eta",
	       18, -4.5, 4.5, "Third jet #eta", "Events");
    
    // Third jet phi
    htool->Add("jets/h_jet3_Phi", "Third Jet Azimuth",
	       8, -M_PI, M_PI, "Third jet #phi [rad]",
	       "Events");

    // Leading b-jet Pt
    htool->Add("jets/h_bjet1_Pt", "Leading b-Jet Pt",
	       10, 0., 300., "Leading b-jet p_{T} [GeV]", "Events");
    
    // Leading b-jet eta
    htool->Add("jets/h_bjet1_Eta", "Leading b-Jet Eta",
	       18, -4.5, 4.5, "Leading b-jet #eta", "Events");
    
    // Leading b-jet phi
    htool->Add("jets/h_bjet1_Phi", "Leading b-Jet Azimuth",
	       8, -M_PI, M_PI, "Leading b-jet #phi [rad]",
	       "Events");
    
    // No. of jets per event
    htool->Add("jets/h_jet_N", "", 7, -0.5, 6.5,
	       "Number of jets / event", "Events");
	
    // No. of b-jets per event
    htool->Add("jets/h_bjet_N", "", 6, -0.5, 5.5,
	       "Number of bjets / event", "Events");

}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsMET(AtlHistogramTool *htool) {
    //
    // Book common histograms for energy sums
    //

    // MET magnitude
    htool->Add("met/h_MET_mag", "Missing Transverse Momentum",
	       10, 0., 300., "E_{T}^{miss} [GeV]", "Events");

    // MET azimuth
    htool->Add("met/h_MET_phi", "Missing Transverse Momentum",
	       8, -M_PI, M_PI, "E_{T}^{miss} #phi [rad]", "Events");
    
    // Transverse W mass (leading lepton !)
    htool->Add("met/h_MtW", "Transverse W Boson Mass (leading lepton)",
	       10, 0., 400, "M_{T, W}(lep 1) [GeV]", "Events");
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsZ0(AtlHistogramTool *htool,
					    const char* path) {
    //
    // Book histograms for a Z boson candidate at the given path
    // in three different subfolders for "elel", "mumu" and "all"
    //
    BookHistogramsZ0Subdir(htool, Form("%s/elel", path));
    BookHistogramsZ0Subdir(htool, Form("%s/mumu", path));
    BookHistogramsZ0Subdir(htool, Form("%s/all",  path));
}
    
//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::BookHistogramsZ0Subdir(AtlHistogramTool *htool,
						  const char* path) {
    //
    // Book histograms for a Z boson candidate at the given path
    //

    // Delta phi
    htool->Add(Form("%s/h_Z_DeltaPhi", path), "Z Azimuth Difference",
	       8, -M_PI, M_PI, "#Delta#phi(lep,lep) [rad]", "Events");
    
    // Invariant mass
    htool->Add(Form("%s/h_Z_M", path), "Z Invariant Mass",
	       22, 80., 102., "M(lep,lep) [GeV]", "Events");

    // Charge
    htool->Add(Form("%s/h_Z_Chg", path), "Dilepton Charge",
	       3, -1.5, 1.5, "#sum chg(lep1, lep2)", "Entries");

    // Transverse momentum
    htool->Add(Form("%s/h_Z_Pt", path), "Z transverse momentum",
	       10, 0., 400., "Z candidate p_{T} [GeV]", "Events");
   
    // Pseudo-rapidity
    htool->Add(Form("%s/h_Z_Eta", path), "Z pseudo-rapidity",
	       8, -5., 5., "Z candidate #eta", "Events");
    
    // Azimuth
    htool->Add(Form("%s/h_Z_Phi", path), "Z azimuth",
	       8, -M_PI, M_PI, "Z #phi [rad]", "Events");

    // MC truth origin (for fake studies)
    TH1D *h_el_Z1_tru_orig = htool->Add(Form("%s/h_el_Z1_truth_origin", path), "MC Particle Origin, first electron from Z candidates",
					AtlMCTruthClassifier::kNumParticleOrigin,
					0, AtlMCTruthClassifier::kNumParticleOrigin,
					"First e from Z", "Entries");
    SetParticleOriginHistBinLabels(h_el_Z1_tru_orig);
    TH1D *h_el_Z2_tru_orig = htool->Add(Form("%s/h_el_Z2_truth_origin", path), "MC Particle Origin, second electron from Z candidates",
					AtlMCTruthClassifier::kNumParticleOrigin,
					0, AtlMCTruthClassifier::kNumParticleOrigin,
					"Second e from Z", "Entries");
    SetParticleOriginHistBinLabels(h_el_Z2_tru_orig);

    TH1D *h_mu_Z1_tru_orig = htool->Add(Form("%s/h_mu_Z1_truth_origin", path), "MC Particle Origin, first muon from Z candidates",
					AtlMCTruthClassifier::kNumParticleOrigin,
					0, AtlMCTruthClassifier::kNumParticleOrigin,
					"First #mu from Z", "Entries");
    SetParticleOriginHistBinLabels(h_mu_Z1_tru_orig);
    TH1D *h_mu_Z2_tru_orig = htool->Add(Form("%s/h_mu_Z2_truth_origin", path), "MC Particle Origin, second muon from Z candidates",
					AtlMCTruthClassifier::kNumParticleOrigin,
					0, AtlMCTruthClassifier::kNumParticleOrigin,
					"Second #mu from Z", "Entries");
    SetParticleOriginHistBinLabels(h_mu_Z2_tru_orig);

}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsCommon(AtlHistogramTool *htool,
						Double_t w) {
    //
    // Fill common histograms  for leptons, jets and the enrgy sums
    //
    FillHistogramsLeptons(htool, w);
    FillHistogramsJets(htool, w);
    FillHistogramsMET(htool, w);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsLeptons(AtlHistogramTool *htool,
						 Double_t w) {
    //
    // Fill common histograms  for leptons  with given weight
    //

    // =======
    // Leptons
    // =======
    HepParticle *lep1 = (HepParticle*)fLeptons->At(0);
    HepParticle *lep2 = (HepParticle*)fLeptons->At(1);
    HepParticle *lep3 = (HepParticle*)fLeptons->At(2);
    htool->Fill("leptons/h_lep_N", fLeptons->GetEntries(), w);   
    htool->Fill("leptons/h_lep1_Pt",  lep1->Pt(), w);
    htool->Fill("leptons/h_lep1_Eta", lep1->Eta(), w);
    htool->Fill("leptons/h_lep1_Phi", lep1->Phi(), w);
    htool->Fill("leptons/h_lep_Chg",  lep1->Charge(), w);
    if ( lep2 != 0 ) { // In case of events with more than 1 lepton
	htool->Fill("leptons/h_lep2_Pt",  lep2->Pt(), w);
	htool->Fill("leptons/h_lep2_Eta", lep2->Eta(), w);
	htool->Fill("leptons/h_lep2_Phi", lep2->Phi(), w);
	htool->Fill("leptons/h_lep_Chg",  lep2->Charge(), w);
    }
    if ( lep3 != 0) { 
	htool->Fill("leptons/h_lep3_Pt",  lep3->Pt(), w);
	htool->Fill("leptons/h_lep3_Eta", lep3->Eta(), w);
	htool->Fill("leptons/h_lep3_Phi", lep3->Phi(), w);
    }

    // Fill weights
    htool->Fill("leptons/h_pileup_weight", fEvent->GetPileupWeight(), 1);
    htool->Fill("leptons/h_mc_weight", log(fEvent->GetMCWeight()), 1);
    htool->Fill("leptons/h_lepton_weight", fEvent->GetEventHeader()->LeptonWeight().GetValue(), 1);
    htool->Fill("leptons/h_btag_weight", fEvent->GetEventHeader()->BtagWeight_MV2c10_77().GetValue(), 1);
    htool->Fill("leptons/h_total_weight", log(fEvent->GetTagEvtWeight()), 1);

    
    // =========
    // Electrons
    // =========
    if ( lep1->IsElectron() ) {
	htool->Fill("leptons/el/h_el_N", fElectrons->GetEntries(), w);
	htool->Fill("leptons/el/h_el1_Pt",  lep1->Pt(), w);
	htool->Fill("leptons/el/h_el1_Eta", lep1->Eta(), w);
	htool->Fill("leptons/el/h_el1_Phi", lep1->Phi(), w);
	htool->Fill("leptons/el/h_el_Chg",  lep1->Charge(), w);
    }
    if ( lep2 != 0 && lep2->IsElectron() ) {
	htool->Fill("leptons/el/h_el2_Pt",  lep2->Pt(), w);
	htool->Fill("leptons/el/h_el2_Eta", lep2->Eta(), w);
	htool->Fill("leptons/el/h_el2_Phi", lep2->Phi(), w);
	htool->Fill("leptons/el/h_el_Chg",  lep2->Charge(), w);
    }

    // MC
    if ( fEvent->IsMC() ) {
	AtlElectron *el = 0;
	for ( Int_t i = 0; i < fElectrons->GetEntries(); i++ ) {
	    el = (AtlElectron*)fElectrons->At(i);
	    AtlMCTruthClassifier::EParticleOrigin origin = el->GetMCTruthClassifier()
		->GetParticleOrigin();
/*
// Fill pt_rel histograms
Double_t pt_rel = 0.; Double_t DeltaR = 0.;
fEvent->ComputePtRel(el, fJets, 10., pt_rel, DeltaR);
if ( origin ==  AtlMCTruthClassifier::kPhotonConv ) {
htool->Fill("leptons/el/h_el_ptrel_photoconv", pt_rel, w);
htool->Fill("leptons/el/h_el_deltaR_photoconv", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::kBottomMeson ) {
htool->Fill("leptons/el/h_el_ptrel_bmeson", pt_rel, w);
htool->Fill("leptons/el/h_el_deltaR_bmeson", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::kZBoson ) {
htool->Fill("leptons/el/h_el_ptrel_zboson", pt_rel, w);
htool->Fill("leptons/el/h_el_deltaR_zboson", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::ktop ) {
htool->Fill("leptons/el/h_el_ptrel_top", pt_rel, w);
htool->Fill("leptons/el/h_el_deltaR_top", DeltaR, w);
}
*/	    
	    // Fill MC truth origin
	    htool->Fill("leptons/el/h_el_truth_origin", (Int_t)origin, w);
	}
    }
	
    // =====
    // Muons
    // =====
    if ( lep1->IsMuon() ) {
	htool->Fill("leptons/mu/h_mu_N", fMuons->GetEntries(), w);
	htool->Fill("leptons/mu/h_mu1_Pt",  lep1->Pt(), w);
	htool->Fill("leptons/mu/h_mu1_Eta", lep1->Eta(), w);
	htool->Fill("leptons/mu/h_mu1_Phi", lep1->Phi(), w);
	htool->Fill("leptons/mu/h_mu_Chg",  lep1->Charge(), w);
    }
    if ( lep2 != 0 && lep2->IsMuon() ) {
	htool->Fill("leptons/mu/h_mu2_Pt",  lep2->Pt(), w);
	htool->Fill("leptons/mu/h_mu2_Eta", lep2->Eta(), w);
	htool->Fill("leptons/mu/h_mu2_Phi", lep2->Phi(), w);
	htool->Fill("leptons/mu/h_mu_Chg",  lep2->Charge(), w);
    }    

    // MC
    if ( fEvent->IsMC() ) {
	AtlMuon *mu = 0;
	for ( Int_t i = 0; i < fMuons->GetEntries(); i++ ) {
	    mu = (AtlMuon*)fMuons->At(i);
	    AtlMCTruthClassifier::EParticleOrigin origin = mu->GetMCTruthClassifier()
		->GetParticleOrigin();
/*
// Fill pt_rel and deltaR histograms
Double_t pt_rel = 0.; Double_t DeltaR = 0.;
fEvent->ComputePtRel(mu, fJets, 10., pt_rel, DeltaR);
if ( origin ==  AtlMCTruthClassifier::kPhotonConv ) {
htool->Fill("leptons/mu/h_mu_ptrel_photoconv", pt_rel, w);
htool->Fill("leptons/mu/h_mu_deltaR_photoconv", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::kBottomMeson ) {
htool->Fill("leptons/mu/h_mu_ptrel_bmeson", pt_rel, w);
htool->Fill("leptons/mu/h_mu_deltaR_bmeson", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::kZBoson ) {
htool->Fill("leptons/mu/h_mu_ptrel_zboson", pt_rel, w);
htool->Fill("leptons/mu/h_mu_deltaR_zboson", DeltaR, w);
} else if ( origin == AtlMCTruthClassifier::ktop ) {
htool->Fill("leptons/mu/h_mu_ptrel_top", pt_rel, w);
htool->Fill("leptons/mu/h_mu_deltaR_top", DeltaR, w);
}
*/	    
	    // Fill MC truth origin
	    htool->Fill("leptons/mu/h_mu_truth_origin", (Int_t)origin, w);
	}
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsJets(AtlHistogramTool *htool,
					      Double_t w) {
    //
    // Fill common histograms for jets with given weight
    //
    AtlJet *jet1 = (AtlJet*)fJets->At(0);
    AtlJet *jet2 = (AtlJet*)fJets->At(1);
    AtlJet *jet3 = (AtlJet*)fJets->At(2);
    if ( jet1 != 0 ) {
    htool->Fill("jets/h_jet1_Pt",  jet1->Pt(), w);
    htool->Fill("jets/h_jet1_Eta", jet1->Eta(), w);
    htool->Fill("jets/h_jet1_Phi", jet1->Phi(), w);
    }
    if ( jet2 != 0 ) {
	htool->Fill("jets/h_jet2_Pt",  jet2->Pt(), w);
	htool->Fill("jets/h_jet2_Eta", jet2->Eta(), w);
	htool->Fill("jets/h_jet2_Phi", jet2->Phi(), w);
    }
    if ( jet3 != 0 ) {
	htool->Fill("jets/h_jet3_Pt",  jet3->Pt(), w);
	htool->Fill("jets/h_jet3_Eta", jet3->Eta(), w);
	htool->Fill("jets/h_jet3_Phi", jet3->Phi(), w);
    }
    AtlJet *bjet1 = (AtlJet*)fBJets->First();
    if ( bjet1 != 0 ) {
    htool->Fill("jets/h_bjet1_Pt",  bjet1->Pt(), w);
    htool->Fill("jets/h_bjet1_Eta", bjet1->Eta(), w);
    htool->Fill("jets/h_bjet1_Phi", bjet1->Phi(), w);
    }

    htool->Fill("jets/h_jet_N",    fJets->GetEntries(), w);
    htool->Fill("jets/h_bjet_N",   fBJets->GetEntries(), w);
    
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsMET(AtlHistogramTool *htool,
					     Double_t w) {
    //
    // Fill common histograms for energy sums with given weight
    //
    htool->Fill("met/h_MET_mag", fMET, w);
    htool->Fill("met/h_MET_phi",
		fEvent->GetEnergySum()->MissingEt_Phi(), w);
    htool->Fill("met/h_MtW", fMtW, w); // <-- leading lepton
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsZ0(AtlHistogramTool *htool,
					    Double_t w,
					    const char* path,
					    HepZ0Decay *Z0cand) {
    //
    // Fill all histograms for the given Z0 candidate at the given
    // path
    //
    if ( Z0cand->IsEE() ) {
	FillHistogramsZ0Subdir(htool, w, path, "elel", Z0cand);
    } else if ( Z0cand->IsMuMu() ) {
	FillHistogramsZ0Subdir(htool, w, path, "mumu", Z0cand);
    }
    FillHistogramsZ0Subdir(htool, w, path, "all", Z0cand);
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::FillHistogramsZ0Subdir(AtlHistogramTool *htool,
						  Double_t w,
						  const char* path,
						  const char* subdir,
						  HepZ0Decay *Z0cand) {
    //
    // Fill the histograms in the given subdirectory for the given Z0
    // candidate at the given path
    //
    TString dir = path;
    dir.Append("/");
    dir.Append(subdir);
    TLorentzVector p_Z = Z0cand->P();
    HepParticle *lep1 = (HepParticle*)Z0cand->GetDaughter1();
    HepParticle *lep2 = (HepParticle*)Z0cand->GetDaughter2();
    htool->Fill(dir.Data(), "h_Z_DeltaPhi", lep1->DeltaPhi(lep2),          w);
    htool->Fill(dir.Data(), "h_Z_M",        p_Z.M(),                       w);
    htool->Fill(dir.Data(), "h_Z_Chg",      lep1->Charge()+lep2->Charge(), w);
    htool->Fill(dir.Data(), "h_Z_Pt",       p_Z.Pt(),                      w);
    htool->Fill(dir.Data(), "h_Z_Eta",      p_Z.Eta(),                     w);
    htool->Fill(dir.Data(), "h_Z_Phi",      p_Z.Phi(),                     w);

    // Fill MC truth origin
    if ( fEvent->IsMC() ) {
	if ( Z0cand->IsEE() ) {
	    htool->Fill(dir.Data(), "h_el_Z1_truth_origin",
			(Int_t)(((AtlElectron*)lep1)->GetMCTruthClassifier()->GetParticleOrigin()), w);
	    htool->Fill(dir.Data(), "h_el_Z2_truth_origin",
			(Int_t)(((AtlElectron*)lep2)->GetMCTruthClassifier()->GetParticleOrigin()), w);
	} else if ( Z0cand->IsMuMu() ) {
	    htool->Fill(dir.Data(), "h_mu_Z1_truth_origin",
			(Int_t)(((AtlMuon*)lep1)->GetMCTruthClassifier()->GetParticleOrigin()), w);
	    htool->Fill(dir.Data(), "h_mu_Z2_truth_origin",
			(Int_t)(((AtlMuon*)lep2)->GetMCTruthClassifier()->GetParticleOrigin()), w);
	}
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::CreatePieCharts() {
    //
    // Create pie charts from particle origin histograms
    //

    // All leptons
    CreateParticleOriginPie("/NoCuts/leptons/el/h_el_truth_origin");
    CreateParticleOriginPie("/NoCuts/leptons/mu/h_mu_truth_origin");
    CreateParticleOriginPie("/PreSel/leptons/el/h_el_truth_origin");
    CreateParticleOriginPie("/PreSel/leptons/mu/h_mu_truth_origin");
    CreateParticleOriginPie("/PreTag/leptons/el/h_el_truth_origin");
    CreateParticleOriginPie("/PreTag/leptons/mu/h_mu_truth_origin");
    CreateParticleOriginPie("/AllCuts/leptons/el/h_el_truth_origin");
    CreateParticleOriginPie("/AllCuts/leptons/mu/h_mu_truth_origin");

    // Leptons from Z candidates
    CreateParticleOriginPie("/NoCuts/Z0_best/elel/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/elel/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/mumu/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/mumu/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/all/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/all/h_mu_Z1_truth_origin");

    CreateParticleOriginPie("/NoCuts/Z0_best/elel/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/elel/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/mumu/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/mumu/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/all/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/NoCuts/Z0_best/all/h_mu_Z2_truth_origin");
	
    CreateParticleOriginPie("/PreSel/Z0_best/elel/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/elel/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/mumu/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/mumu/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/all/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/all/h_mu_Z1_truth_origin");

    CreateParticleOriginPie("/PreSel/Z0_best/elel/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/elel/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/mumu/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/mumu/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/all/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreSel/Z0_best/all/h_mu_Z2_truth_origin");
    
    CreateParticleOriginPie("/PreTag/Z0_best/elel/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/elel/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/mumu/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/mumu/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/all/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/all/h_mu_Z1_truth_origin");

    CreateParticleOriginPie("/PreTag/Z0_best/elel/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/elel/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/mumu/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/mumu/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/all/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/PreTag/Z0_best/all/h_mu_Z2_truth_origin");
    
    CreateParticleOriginPie("/AllCuts/Z0_best/elel/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/elel/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/mumu/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/mumu/h_mu_Z1_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/all/h_el_Z1_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/all/h_mu_Z1_truth_origin");

    CreateParticleOriginPie("/AllCuts/Z0_best/elel/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/elel/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/mumu/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/mumu/h_mu_Z2_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/all/h_el_Z2_truth_origin");
    CreateParticleOriginPie("/AllCuts/Z0_best/all/h_mu_Z2_truth_origin");
}

//____________________________________________________________________

TPie* AtlSgTopFCNC_tZ_Base::CreateParticleOriginPie(const char *HistName,
						    Int_t MaxSlices) {
    //
    // Create pi chart for the given MC truth particle origin
    // histogram for leptons (full path). The pie is stored in the
    // same folder as the histogram. If the number of non-zero bin
    // entries is larger than the given maximum number of slices for
    // the pie chart, the excess bins are collected in a slice called
    // "other".
    //

    // Get histogram
    TH1D *h = (TH1D*)fOutputFile->Get(HistName);
    if ( h == 0 ) {
	Error("CreateParticleOriginPie", "Histogram %s not found. Abort!",
	      HistName);
	gSystem->Abort(1);
    }

    // Save present working directory and cd to the given path
    TDirectory *savdir = gDirectory;
    fOutputFile->cd(gSystem->DirName(HistName));

    // Get the fractional contents for all bins and sort them 
    Double_t frac[100];
    Int_t nbins = h->GetNbinsX();
    Double_t tot = h->Integral();
    for ( Int_t i = 0; i < nbins; i++ ) {
	frac[i] = h->GetBinContent(i+1)/tot;
    }
    Int_t frac_idx[100];
    TMath::Sort(nbins, frac, frac_idx);

    // Count the non-zero bin entries
    Int_t nonzero = 0;
    for ( Int_t i = 0; i < nbins; i++ ) {
	if ( frac[frac_idx[i]]  == 0. ) {
	    nonzero = i;
	    break;
	}
    }

    // Set the no. of slices
    Int_t nslices = nonzero;
    Bool_t other = kFALSE;
    if ( nonzero > MaxSlices ) {
	nslices = MaxSlices + 1;
	other = kTRUE;
    }

    // Create and fill pie chart
    Double_t frac_other = 0.;
    TString *pname = new TString(h->GetName());
    pname->ReplaceAll("h_", "p_");
    pname->ReplaceAll("tp_", "th_");
    TPie *pie = new TPie(pname->Data(), h->GetTitle(), nslices);
    for ( Int_t i = 0; i < nonzero; i++ ) {
	if ( i < MaxSlices ) {
	    pie->SetEntryVal(i, frac[frac_idx[i]]);
	    pie->SetEntryLabel(i, h->GetXaxis()->GetBinLabel(frac_idx[i]+1));
	    pie->SetEntryFillColor(i, 2+i);
	    pie->SetEntryLineColor(i, 2+i);
	} else {
	    frac_other += frac[frac_idx[i]];
	}
    }
    if ( other ) {
	pie->SetEntryVal(nslices-1, frac_other);
	pie->SetEntryLabel(nslices-1, "Other");
	pie->SetEntryFillColor(nslices-1, 1+nslices);
	pie->SetEntryLineColor(nslices-1, 1+nslices);
    }
    pie->SetRadius(0.25);
    pie->SetTextFont(42);
    pie->SetTextSize(0.03);
    pie->SetLabelFormat("%txt %perc");

    // Save pie chart and restore pwd
    pie->Write();
    savdir->cd();

    return pie;
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::SetParticleOriginHistBinLabels(TH1D *h) {
    //
    // Set alphanumeric bin labels for the x axis of the given
    // particle origin histogram
    //
    for ( Int_t i = 0; i < AtlMCTruthClassifier::kNumParticleOrigin; i++ ) {
	h->GetXaxis()->SetBinLabel(i+1,
				   AtlMCTruthClassifier::GetParticleOriginName((AtlMCTruthClassifier::EParticleOrigin)i));
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Base::ComputeDecayAngleE(HepParticle *mother,
					      HepParticle *daughter,
					      Double_t &DecayAngle,
					      Double_t &E) {
    //
    // Compute the decay angle of the daughter particle in the parent
    // rest frame w.r.t. the direction of the parent particle in the
    // lab frame. Compute also the energy of the daughter in the
    // mother's rest frame.
    //
    TLorentzVector p_mother = mother->P();
    TVector3 boost = -p_mother.BoostVector();
    TLorentzVector p_daughter_rf = daughter->P();
    p_daughter_rf.Boost(boost);
    DecayAngle = p_mother.Angle(p_daughter_rf.Vect());
    E = p_daughter_rf.E();
}
