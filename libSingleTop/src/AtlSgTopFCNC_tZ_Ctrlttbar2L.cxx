//____________________________________________________________________
//
// Single-top FCNC tZ analysis: ttbar ctrl region
//
// ttbar is enriched by applying a veto for Z bosons
//
//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2016 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_Ctrlttbar2L
#include <AtlSgTopFCNC_tZ_Ctrlttbar2L.h>
#endif

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_Ctrlttbar2L);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_Ctrlttbar2L::AtlSgTopFCNC_tZ_Ctrlttbar2L(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base(OutputFileName) {
    //
    // Default constructor
    //
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_Ctrlttbar2L::~AtlSgTopFCNC_tZ_Ctrlttbar2L() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Ctrlttbar2L::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fNLeptons_max = 2;
	fVetoZboson = kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Ctrlttbar2L::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Check whether SingleTopAnalysis "triggered" on the event.
    return (fEvent->GetEventHeader()
     ->TestPreselectionFlagsAny(AtlEventHeader::kElEl | AtlEventHeader::kElMu | AtlEventHeader::kMuMu));
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Ctrlttbar2L::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top FCNC tZ Selector : ttbar Ctrl Region     " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSgTopFCNC_tZ_Base::Print(option);
    cout << endl
	 << "========================================================"
	 << endl << endl;   
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Ctrlttbar2L::ProcessCut() {
    //
    // Event selection
    //
    // Select di-lepton events with the given kinematic, while
    // accepting all di-lepton combinations allowed in Zt events
    // (permutations like e+e-mu+, e+e-mu- will be sorted out later by
    // the ME method).
    // 

    // Fill histograms without any cuts applied
    FillHistogramsCommon(fHistsNoCuts, fEvent->GetPreTagEvtWeight());
	
    // ==========================
    // Quick lepton pre-selection
    // ==========================
    
    // Minimum/maximum  no. of leptons (default = 2)
    if ( (fLeptons->GetEntries() < fNLeptons_min) 
	 || (fLeptons->GetEntries() > fNLeptons_max) ) return kFALSE;

    // Fetch leptons and cut on individual lepton Pt
    HepParticle *lep = 0;
    for ( Int_t i = 0; i < fLeptons->GetEntries(); i++ ) {
	lep = (HepParticle*)fLeptons->At(i);
	if ( i == 0 && lep->Pt() < fPt_Lep1_min ) return kFALSE;
	if ( i == 1 && lep->Pt() < fPt_Lep2_min ) return kFALSE;
    }

    // ===================
    // Kinematic selection
    // ===================
    
    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( fMET < fMET_min || fMET > fMET_max ) return kFALSE;
    
    // Transverse W mass (leading lepton !)
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));
    if ( fMtW < fMtW_min || fMtW > fMtW_max ) return kFALSE;

    // Fill all pre-selection histograms
    FillHistogramsCommon(fHistsPreSel, fEvent->GetPreTagEvtWeight());

    // =================
    // tZ reconstruction
    // =================
    ReconstructZ0Decays();

    // Require a Z candidate
    if ( fEvent->GetN_Z0Decays() < 1 ) return kFALSE;
    
    // Require a top-quark candidate
    //if ( fEvent->GetN_TopDecays() < 1 ) return kFALSE;
    
    // Fill all pre-tagged histograms and count number of pre-tagged events
    FillHistogramsCommon(fHistsPreTag, fEvent->GetPreTagEvtWeight());
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
    FillHistogramsZ0(fHistsAllCuts, fEvent->GetTagEvtWeight(),
    		     "Z0_best", GetZ0DecayBest());
 
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Ctrlttbar2L::BookHistograms() {
    //
    // Book histograms
    //
    AtlSgTopFCNC_tZ_Base::BookHistograms();
}
