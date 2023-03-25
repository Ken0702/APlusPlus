//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2014 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterLPSC
#define ATLAS_AtlEvtWriterLPSC
#ifndef ATLAS_AtlEvtWriterD3PDSgTop
#include <AtlEvtWriterD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlSgTop_sChannelAnalysis
#include <AtlSgTop_sChannelAnalysis.h>
#endif

class AtlEvtWriterLPSC : public AtlEvtWriterD3PDSgTop {

private:
    Float_t   fXsection;                   // sample xsec to compute MC lumi for BDT
    Float_t   fNEvents;                    // sample initial number of events to -"-

public:
    Int_t     fN_looseleptons;             // Number of loose leptons
    Int_t     fN_loosejets;                // Number of loose jets
    Float_t   fMET;                        // MissingEt of current event
    Float_t   fMtW;                        // Transverse W mass (GeV)
    Float_t   fPtLep;                      // Transverse momentum (GeV) of leading lepton
    Float_t   fQLep;                       // Charge of leading lepton
    Float_t   fDeltaPhiTopBJet2_BJet1;     // Delta phi between top-quark candidate reconstructed using the sub-leading b-jet and the leading b-jet
    Float_t   fDeltaPhiTopBJet1_BJet2;     // Delta phi between top-quark candidate reconstructed using the leading b-jet and the sub-leading b-jet
    Float_t   fSumEtLep_MET;               // Sum Et of lepton and MET
    Float_t   fSumEtLep_MET_BJets;         // Sum Et of lepton, MET and all b-jets
    Float_t   fSumPt;                      // Vectoriell Pt sum of the lepton, MET and all b-jets
    Float_t   fDeltaEtaLep_BJet1;          // Delta eta between the lepton and the leading b-jet
    Float_t   fDeltaEtaLep_BJet2;          // Delta eta between the lepton and the sub-leading b-jet
    Float_t   fDeltaEtaTopBJet2_BJet1;     // Delta eta between top decay reconstructed using the sub-leading b-jet and the leading b-jet
    Float_t   fDeltaPhiBJet1_BJet2;        // Delta phi between leading and sub-leading b-jet
    Float_t   fDeltaPhiLep_MET;            // Delta phi between lepton and MET
    Float_t   fCosLepW_WTopBJet1;          // Cos of angle between lepton momentum in W rest-frame and the W momentum in the rest-frame of the top-quark candidate with the leading b-jet
    Float_t   fCosLepTopBJet1_TopBJet1CMS; // Cos of angle between the lepton momentum in the top-quark rest-frame and the top-quark momentum in the c.m.s. The used top-quark is the candidate reconstructed using the leading b-jet. The c.m.s vector is the sum of the top momentum and the sub-leading b-jet momentum
    Float_t   fCosLepTopBJet2_TopBJet2CMS; // Cos of angle between the lepton momentum in the top-quark rest-frame and the top-quark momentum in the c.m.s. The used top-quark is the candidate reconstructed using the sub-leading b-jet. The c.m.s vector is the sum of the top momentum and the leading b-jet momentum
    Float_t   fPt_BJetTop;                 // Pt of b-jet from best recosntructed yop decay (GeV)
    Float_t   fDeltaEtaNu_BJetNonTop;      // Delta eta between the rec. neutrino and the b-jet from the non-top candidate
    Float_t   fSumPtBJet1_BJet2;           // Vectoriell Pt sum of the leading and the sub-leading b-jet (GeV)
    Float_t   fCosMET_BJet2;               // Cos of angle between MET and sub-leading b-jet
    Float_t   fMLep_BJet2;                 // Invariant mass of lepton and sub-leading b-jet (GeV)
    Float_t   fMTop;                       // Inv mass of best reconstructed top
    Float_t   fMLep_BJetTop;               // Inv mass of lepton and b-jet from top
    Float_t   fMTop_NonTopBJet;            // Inv mass of top and b-jet not from top

public:
    AtlEvtWriterLPSC();
    virtual ~AtlEvtWriterLPSC();
    virtual void BookTree(TTree *tree, AtlEvent *evt) override;
    TTree* CreateTree(const char* name, const char* title,
			      TTree *InputTree) override;
    virtual const char* GetNameOfType() override { return "kLPSC"; }
    void Terminate() override;
    inline void SetXsection(Float_t xsec) { fXsection = xsec; }
    inline void SetNEvents(Float_t nev) { fNEvents = nev; }

    ClassDefOverride(AtlEvtWriterLPSC,0) // Event writer class for SgTop-D3PD events in the LPSC Grenoble format
};
#endif
