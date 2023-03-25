//____________________________________________________________________
//
// Single-top FCNC tZ analysis: Standard Model trilepton ctrl region
// 
// Author: Ana Peixoto <mailto: ana.paula.pereira.peixoto@cern.ch>
// Update: $Id$
// Copyright: 2015 (C) Ana Peixoto
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlSM
#include <AtlSgTopFCNC_tZ_CtrlSM.h>
#endif

#include <cmath>

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_CtrlSM);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlSM::AtlSgTopFCNC_tZ_CtrlSM(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base3L(OutputFileName) {
    //
    // Default constructor
    //
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlSM::~AtlSgTopFCNC_tZ_CtrlSM() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlSM::SetCutDefaults() {
    //
    // Selection cut defaults
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlSM::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top FCNC tZ Selector : SM 3L CR          " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSgTopFCNC_tZ_Base::Print(option);
    cout << endl
	 << "========================================================"
	 << endl << endl;   
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_CtrlSM::ProcessCut() {
    //
    // Event selection
    //
    // Select tri-lepton events with the given kinematic, while
    // accepting all tri-lepton combinations allowed in Zt events
    // (permutations like e+e-mu+, e+e-mu- will be sorted out later by
    // the ME method).
    // 
 
    // Fill histograms without any cuts applied
    FillHistogramsCommon(fHistsNoCuts, fEvent->GetPreTagEvtWeight());
    
    // ==========================
    // Quick lepton pre-selection
    // ==========================
    
    // Minimum/maximum  no. of leptons (default = 3)
    if ( (fLeptons->GetEntries() < fNLeptons_min) 
	 || (fLeptons->GetEntries() > fNLeptons_max) ) return kFALSE;
    
    // Fetch leptons and cut on individual lepton Pt
    Float_t tot_chg = 0.;
    HepParticle *lep = 0;
    for ( Int_t i = 0; i < fLeptons->GetEntries(); i++ ) {
	lep = (HepParticle*)fLeptons->At(i);
	if ( i == 0 && lep->Pt() < fPt_Lep1_min ) return kFALSE;
	if ( i == 1 && lep->Pt() < fPt_Lep2_min ) return kFALSE;
	if ( i == 2 && lep->Pt() < fPt_Lep3_min ) return kFALSE;
	tot_chg += lep->Charge();
    }

    // Remove all +++ and --- combinations (trilepton case only)
    if ( fLeptons->GetEntries() == 3 ) {
	if ( TMath::Abs(tot_chg) > 2. ) return kFALSE;
    }

    // ===================
    // Kinematic selection
    // ===================

    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( fMET < fMET_min || fMET > fMET_max ) return kFALSE;
    
    // Transverse W mass (leading lepton !)
	//
    // Do not cut on this MtW definition but on the proper MtW using
    // the lepton from the top decay (see below)
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));

    // Fill all pre-selection histograms
    FillHistogramsCommon(fHistsPreSel, fEvent->GetPreTagEvtWeight());
    
    // =================
    // tZ reconstruction
    // =================
    ReconstructZ0Top();

    // Require a Z candidate
    if ( fRequireZ && fEvent->GetN_Z0Decays() < 1 ) return kFALSE;
    
    // Require a top-quark candidate
    if ( fRequireTop && fEvent->GetN_TopDecays() < 1 ) return kFALSE;
   
	//Cut on the transverse momentum of the Z boson candidate
	HepZ0Decay *Z0 = GetZ0DecayBest();
    TLorentzVector p_Z = Z0->P();
    Double_t pT_Z = p_Z.Pt();
	if ( pT_Z > 250. ) return kFALSE;

	// Cut on transverse W mass (based on the lepton originating from
    // the reconstrcuted top decay)
    if ( fMtW_min > 0. || fMtW_max < INFINITY ) {
    HepTopDecay *top = (HepTopDecay*)fEvent->GetTopDecays()->At(0);
    AtlWDecayLNu *Wboson = (AtlWDecayLNu*)top->GetWDecay();
    Float_t MtW = fEvent->W_Mperp(Wboson->GetChargedLeptonOrig());
    if ( MtW < fMtW_min || MtW > fMtW_max ) return kFALSE;
    }

	// Fill all pre-tagged histograms and count number of pre-tagged events
    FillHistogramsCommon(fHistsPreTag, fEvent->GetPreTagEvtWeight());
    FillHistogramsTop(fHistsPreTag, fEvent->GetPreTagEvtWeight());
    FillHistogramsZ0(fHistsPreTag, fEvent->GetPreTagEvtWeight(),
    		     "Z0_best", GetZ0DecayBest());
    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();

    // =====================
    // B-tagging requirement
    // =====================
    if ( fBJets->GetEntries() < fNBTags_min
         || fBJets->GetEntries() > fNBTags_max ) return kFALSE;
    
    // Fill histograms for with all cuts applied
    FillHistogramsCommon(fHistsAllCuts, fEvent->GetTagEvtWeight());
    FillHistogramsTop(fHistsAllCuts, fEvent->GetTagEvtWeight());
    FillHistogramsZ0(fHistsAllCuts, fEvent->GetTagEvtWeight(),
    		     "Z0_best", GetZ0DecayBest());

	// Accept event
    return kTRUE;
}
    
//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlSM::BookHistograms() {
    //
    // Book histograms
    //
   AtlSgTopFCNC_tZ_Base3L::BookHistograms();
}
