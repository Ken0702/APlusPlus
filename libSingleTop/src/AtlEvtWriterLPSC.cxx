//____________________________________________________________________
//
// Event writer or the LPSC event format from the Grenoble group
// (SgTop-D3PD plus BDT input variables)
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2014 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterLPSC
#include <AtlEvtWriterLPSC.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtWriterLPSC);
#endif

//____________________________________________________________________

AtlEvtWriterLPSC::AtlEvtWriterLPSC() {
    //
    // Default constructor
    //
    fXsection = 0;
    fNEvents = 0;
}

//____________________________________________________________________

AtlEvtWriterLPSC::~AtlEvtWriterLPSC() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtWriterLPSC::BookTree(TTree *tree, AtlEvent *evt) {
    //
    // Book output tree
    //
    AtlEvtWriterD3PDSgTop::BookTree(tree, evt);

    if ( fCloneOnly ) {
        Info(__FUNCTION__, "Not booking bdt_* branches (clone only).");
        return;
    }

    // Book BDT variables
    fTree->Branch("bdt_number_looseleptons", &fN_looseleptons, "bdt_number_looseleptons/I");
    fTree->Branch("bdt_number_loosejets", &fN_loosejets, "bdt_number_loosejets/I");
    fTree->Branch("bdt_MET", &fMET, "bdt_MET/F");
    fTree->Branch("bdt_MtW", &fMtW, "bdt_MtW/F");
    fTree->Branch("bdt_Pt_Lep", &fPtLep, "bdt_Pt_Lep/F");
    fTree->Branch("bdt_Q_Lep", &fQLep, "bdt_Q_Lep/F");
    fTree->Branch("bdt_DeltaPhiTopBJet2_BJet1",
    		  &fDeltaPhiTopBJet2_BJet1,
    		  "bdt_DeltaPhiTopBJet2_BJet1/F");
    fTree->Branch("bdt_DeltaPhiTopBJet1_BJet2",
    		  &fDeltaPhiTopBJet1_BJet2,
    		  "bdt_DeltaPhiTopBJet1_BJet2/F");
    fTree->Branch("bdt_SumEtLep_MET", &fSumEtLep_MET,
    		  "bdt_SumEtLep_MET/F");
    fTree->Branch("bdt_SumEtLep_MET_BJets", &fSumEtLep_MET_BJets,
    		  "bdt_SumEtLep_MET_BJets/F");
    fTree->Branch("bdt_SumPt", &fSumPt, "bdt_SumPt/F");
    fTree->Branch("bdt_DeltaEtaLep_BJet1", &fDeltaEtaLep_BJet1,
    		  "bdt_DeltaEtaLep_BJet1/F");
    fTree->Branch("bdt_DeltaEtaLep_BJet2", &fDeltaEtaLep_BJet2,
    		  "bdt_DeltaEtaLep_BJet2/F");
    fTree->Branch("bdt_DeltaEtaTopBJet2_BJet1",
    		  &fDeltaEtaTopBJet2_BJet1,
    		  "bdt_DeltaEtaTopBJet2_BJet1/F");
    fTree->Branch("bdt_DeltaPhiBJet1_BJet2", &fDeltaPhiBJet1_BJet2,
    		  "bdt_DeltaPhiBJet1_BJet2/F");
    fTree->Branch("bdt_DeltaPhiLep_MET", &fDeltaPhiLep_MET,
    		  "bdt_DeltaPhiLep_MET/F");
    fTree->Branch("bdt_CosLepW_WTopBJet1", &fCosLepW_WTopBJet1,
    		  "bdt_CosLepW_WTopBJet1/F");
    fTree->Branch("bdt_CosLepTopBJet1_TopBJet1CMS",
    		  &fCosLepTopBJet1_TopBJet1CMS,
    		  "bdt_CosLepTopBJet1_TopBJet1CMS/F");
    fTree->Branch("bdt_CosLepTopBJet2_TopBJet2CMS",
    		  &fCosLepTopBJet2_TopBJet2CMS,
    		  "bdt_CosLepTopBJet2_TopBJet2CMS/F");
    fTree->Branch("bdt_Pt_BJetTop", &fPt_BJetTop,
    		  "bdt_Pt_BJetTop/F");
    fTree->Branch("bdt_DeltaEtaNu_BJetNonTop", &fDeltaEtaNu_BJetNonTop,
    		  "bdt_DeltaEtaNu_BJetNonTop/F");
    fTree->Branch("bdt_SumPtBJet1_BJet2", &fSumPtBJet1_BJet2,
    		  "bdt_SumPtBJet1_BJet2/F");
    fTree->Branch("bdt_CosMET_BJet2", &fCosMET_BJet2,
    		  "bdt_CosMET_BJet2/F");
    fTree->Branch("bdt_MLep_BJet2", &fMLep_BJet2,
    		  "bdt_MLep_BJet2/F");
    fTree->Branch("bdt_MTop", &fMTop, "bdt_MTop/F");
    fTree->Branch("bdt_MLep_BJetTop", &fMLep_BJetTop,
                  "bdt_MLep_BJetTop/F");
    fTree->Branch("bdt_MTop_NonTopBJet", &fMTop_NonTopBJet,
                  "bdt_MTop_NonTopBJet/F");
}

//____________________________________________________________________

TTree* AtlEvtWriterLPSC::CreateTree(const char* name,
                                         const char* title,
					 TTree *InputTree) {
    //
    // Create output tree
    //
    InputTree->SetBranchStatus("*", kTRUE);

    fTree = InputTree->CloneTree(0);
    if ( fTree == 0 ) {
	// TTree::CloneTree() cannot cope with empty trees. Bypass by
	// using TTree::CopyTree()
    	fTree = InputTree->CopyTree("","");
    }
    fTree->SetNameTitle(name, title);
    return fTree;
}

//____________________________________________________________________

void AtlEvtWriterLPSC::Terminate() {
    //
    // Function that is called from AtlSelector::Terminate().
    //
    // Currently used to add a branch to the output tree with scaled
    // event weights for BDT purposes. The sample is normalized to a
    // lumi of 1 pb^-1 (default xsec unit in A++).
    //

    // only do if there is an output tree
    if ( nullptr == fTree )
        return;

    if ( fCloneOnly ) {
        Info(__FUNCTION__, "Skip scaling evt weight for BDT (clone only).");
        return;
    }

    // only do this if the xsection and nevent have valid values
    if ( fXsection <= 0 || fNEvents <= 0 ) {
        Warning(__FUNCTION__, "cross section (%f) or number of events (%f) invalid",
                fXsection, fNEvents);
        return;
    } else {
        Info(__FUNCTION__, "Scaling bdt evt weights with cross section (%g) divided by number of events (%g): %g", fXsection, fNEvents, fXsection/fNEvents);
    }

    // original evt weights
    TBranch *b_weight = fTree->GetBranch("evt_weight");
    Float_t weight = 0;
    fTree->SetBranchAddress("evt_weight", &weight);

    // normalized with MC lumi
    Float_t weight_norm = 0;
    TBranch *b_weight_norm = fTree->Branch("bdt_evt_weight_norm", &weight_norm, "bdt_evt_weight_test/F");

    // loop to fill new branch
    Int_t nentries = (Int_t)b_weight->GetEntries();
    for ( Int_t i = 0; i < nentries; i++ ) {
        b_weight->GetEntry(i);
        weight_norm = weight * fXsection/fNEvents;
        b_weight_norm->Fill();
    }
}
