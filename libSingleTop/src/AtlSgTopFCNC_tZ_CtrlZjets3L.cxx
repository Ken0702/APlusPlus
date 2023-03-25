//____________________________________________________________________
//
// Single-top FCNC tZ analysis: Z+jets tri-lepton ctrl region
// 
//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlZjets3L
#include <AtlSgTopFCNC_tZ_CtrlZjets3L.h>
#endif

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_CtrlZjets3L);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlZjets3L::AtlSgTopFCNC_tZ_CtrlZjets3L(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base3L(OutputFileName) {
    //
    // Default constructor
    //
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlZjets3L::~AtlSgTopFCNC_tZ_CtrlZjets3L() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlZjets3L::SetCutDefaults() {
    //
    // Selection cut defaults
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlZjets3L::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top FCNC tZ Selector : Z+jets 3L CR      " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSgTopFCNC_tZ_Base3L::Print(option);
    cout << endl
	 << "========================================================"
	 << endl << endl;   
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_CtrlZjets3L::ProcessCut() {
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
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));
    if ( fMtW < fMtW_min || fMtW > fMtW_max ) return kFALSE;

    // Fill all pre-selection histograms
    FillHistogramsCommon(fHistsPreSel, fEvent->GetPreTagEvtWeight());
    
    // =================
    // tZ reconstruction
    // =================
    ReconstructZ0Top();

    // Require a Z candidate
    if ( fRequireZ && fEvent->GetN_Z0Decays() < 1 ) return kFALSE;

    // Fill all pre-tagged hisotgrams and count number of pre-tagged events
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

void AtlSgTopFCNC_tZ_CtrlZjets3L::BookHistograms() {
    //
    // Book histograms
    //
    AtlSgTopFCNC_tZ_Base3L::BookHistograms();
}
