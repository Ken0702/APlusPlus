//____________________________________________________________________
//
// Single-top FCNC tZ analysis: Z+jets di-lepton ctrl region
// 
//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind 
//
#ifndef SGTOP_AtlSgTopFCNC_tZ_CtrlZjets2L
#include <AtlSgTopFCNC_tZ_CtrlZjets2L.h>
#endif

#ifndef __CINT__
ClassImp(AtlSgTopFCNC_tZ_CtrlZjets2L);
#endif

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlZjets2L::AtlSgTopFCNC_tZ_CtrlZjets2L(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base(OutputFileName) {
    //
    // Default constructor
    //
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_CtrlZjets2L::~AtlSgTopFCNC_tZ_CtrlZjets2L() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlZjets2L::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fNLeptons_max=2;
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_CtrlZjets2L::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Check whether SingleTopAnalysis "triggered" on the event.
    return (fEvent->GetEventHeader()
     ->TestPreselectionFlagsAny(AtlEventHeader::kElEl | AtlEventHeader::kElMu | AtlEventHeader::kMuMu));
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlZjets2L::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top FCNC tZ Selector : Z+jets 2L CR      " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSgTopFCNC_tZ_Base::Print(option);
    cout << endl
	 << "========================================================"
	 << endl << endl;   
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_CtrlZjets2L::ProcessCut() {
    //
    // Event selection
    //

    // Fill histograms w/o any cuts applied
    FillHistogramsCommon(fHistsNoCuts, fEvent->GetPreTagEvtWeight());
    
    // ==========================
    // Quick lepton pre-selection
    // ==========================
    
    // Minimum/maximum  no. of leptons (default = 2)
    if ( fLeptons->GetEntries() < fNLeptons_min
	 || fLeptons->GetEntries() > fNLeptons_max ) return kFALSE;

    // ====================
    // Kinematic selection
    // ====================
    
    // Leading lepton Pt
    HepParticle *lep1 = (HepParticle*)fLeptons->At(0);
    HepParticle *lep2 = (HepParticle*)fLeptons->At(1);
    if ( lep1->Pt() < fPt_Lep1_min ) return kFALSE;
    if ( lep2->Pt() < fPt_Lep2_min ) return kFALSE;

    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( fMET < fMET_min || fMET > fMET_max ) return kFALSE;
    
    // Transverse W mass (leading lepton !)
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));
    if ( fMtW < fMtW_min || fMtW > fMtW_max ) return kFALSE;

    //Fill all pre-selection histograms
    FillHistogramsCommon(fHistsPreSel, fEvent->GetPreTagEvtWeight());
    
    // =================
    // Z reconstruction
    // =================
    ReconstructZ0Decays();
    
    // Require a Z candidate;
    if ( fRequireZ && fEvent->GetN_Z0Decays() < 1 ) return kFALSE;
    
    // // Veto a top candidate
    // if (fEvent->GetN_TopDecays() > 0) return kFALSE;
    //
    //OMK Comment this veto for now. It would require a top-quark
    //OMK reconstrution using one of the two leptons (or both?)
    
    // Fill all pre-tagged hisotgrams and count number of pre-tagged events
    FillHistogramsCommon(fHistsPreTag, fEvent->GetPreTagEvtWeight());
    FillHistogramsZ0(fHistsPreTag, fEvent->GetPreTagEvtWeight(),
    		     "Z0_best", GetZ0DecayBest());
    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();
    
    // =====================
    // B-tagging requirement
    // =====================
    if ( fBJets->GetEntries() < fNBTags_min
         || fBJets->GetEntries() > fNBTags_max) return kFALSE;

    // Fill histograms for with all cuts applied
    FillHistogramsCommon(fHistsAllCuts, fEvent->GetTagEvtWeight());
    FillHistogramsZ0(fHistsAllCuts, fEvent->GetTagEvtWeight(),
    		     "Z0_best", GetZ0DecayBest());

    return kTRUE;
}
    
//____________________________________________________________________

void AtlSgTopFCNC_tZ_CtrlZjets2L::BookHistograms() {
    //
    // Book histograms
    //
    AtlSgTopFCNC_tZ_Base::BookHistograms();
}
