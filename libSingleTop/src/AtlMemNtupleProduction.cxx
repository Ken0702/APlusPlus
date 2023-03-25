//____________________________________________________________________
//
// MEM Ntuple production
// 
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlMemNtupleProduction
#include <AtlMemNtupleProduction.h>
#endif
#include <AtlEvtWriterMem.h>
#include <TList.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMemNtupleProduction);
#endif

//____________________________________________________________________

AtlMemNtupleProduction::AtlMemNtupleProduction(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    fElectrons = 0;
    fMuons     = 0;
    fJets      = 0;
    fBJets     = 0;

    fObjTool   = 0;
    fSFTool    = 0;
    
    // Cut defaults
    fMET_min    = 0.;
    fMtW_min    = 0.;
    fNBTags_min = 0;
    fNBTags_max = 999;
    fLeadingJetIsBTagged = kFALSE;
}

//____________________________________________________________________

AtlMemNtupleProduction::~AtlMemNtupleProduction() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlMemNtupleProduction::Clear(Option_t *option) {
    //
    // Clears the dynamically allocated objects
    //
}

//____________________________________________________________________

void AtlMemNtupleProduction::BookHistograms() {
    //
    // Get analysis object from tool and set pointers to lists of
    // selected objects and select b-tagger.
    //
	
    // Get ObjectSelection Tool + pointers to analysis objects
    fObjTool = 
	(AtlObjectsToolD3PDSgTop*)GetTool("AtlObjectsToolD3PDSgTop"
					  ,"",kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Provide tools with pointers to analysis objects
    fSFTool = 
	(AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool"
					    ,"",kTRUE);
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);

    // Set pointers to MemNtuple event writer
    if ( fOutputMode == kMemNtuple ) {
	((AtlEvtWriterMem*)fEvtWriter)->SetElectrons(fElectrons);
	((AtlEvtWriterMem*)fEvtWriter)->SetMuons(fMuons);
	((AtlEvtWriterMem*)fEvtWriter)->SetJets(fJets, fBJets);
	((AtlEvtWriterMem*)fEvtWriter)->SetBTagger(fObjTool->GetBTagger());
    }
}

//____________________________________________________________________

void AtlMemNtupleProduction::FillHistograms() {
    //
    // Fill histograms
    //    
}

//____________________________________________________________________

Bool_t AtlMemNtupleProduction::ProcessCut() {
    //
    // Event selection
    //

    // Missing Et
    if ( fEvent->GetEnergySum()->MissingEt_Mag() < fMET_min )
	return kFALSE;

    // Transverse W mass
    if ( fEvent->W_Mperp((HepParticle*)fLeptons->At(0)) < fMtW_min )
	return kFALSE;

    // Count no. of pre-tagged events
    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();
    
    // No. of b-tagged jets
    if ( fBJets->GetEntries() < fNBTags_min
	 || fBJets->GetEntries() > fNBTags_max) return kFALSE;

    // Require the leading jet to be b-tagged ?
    if ( fLeadingJetIsBTagged == kTRUE
	 && fBJets->At(0) != fJets->At(0) ) return kFALSE;    

    // Accept event
    return kTRUE;
}

//____________________________________________________________________

void AtlMemNtupleProduction::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Mem Ntuple Production                                 " << endl
	 << "========================================================" << endl
	 << endl
	 << "  fMET_min             = " << fMET_min << " GeV" << endl
	 << "  fMtW_min             = " << fMtW_min << " GeV" << endl
	 << "  fNBTags_min          = " << fNBTags_min << endl
	 << "  fNBTags_max          = " << fNBTags_max << endl
	 << "  fLeadingJetIsBTagged = "
	 << ( (fLeadingJetIsBTagged) ? "true" : "false" ) << endl << endl
	 << "========================================================" << endl
	 << endl;
}
