//____________________________________________________________________
//
//
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlWjetsScaleFactorTool
#include <AtlWjetsScaleFactorTool.h>
#endif
#include <iostream>

using namespace std;

const Double_t AtlWjetsScaleFactorTool::fgWjetsSF_el[41] = {
    1.27782, 1.31514, 1.3039,  1.2874,  1.27433,
    1.22216, 1.21848, 1.20525, 1.14273, 1.15883,
    1.11091, 1.19356, 1.14998, 1.08237, 1.06008,
    1.16418, 1.07582, 1.08043, 1.1444,  1.10589,
    0.98765, 1.02782, 1.09187, 0.975726, 1.00977,
    1.02593, 0.947341, 0.987128, 1.03481, 1.09191,
    0.918242, 1.02247, 1.02147, 0.890984, 0.972405,
    1.31816, 0.83809, 1.09929, 1.09481, 1.24821,
    1.03161 };

const Double_t AtlWjetsScaleFactorTool::fgWjetsSF_mu[41] = {
    1.24262,  1.3441,   1.21622,  1.25679,  1.14936,
    1.22594,  1.21616,  1.16556,  1.21355,  1.18106,
    1.20481,  1.17082,  1.16274,  1.06071,  1.06033,
    1.10203,  1.08698,  1.11646,  1.12294,  1.16231,
    0.932065, 1.05363,  1.02453,  1.00237,  0.898718,
    1.16953,  1.02912,  0.895753, 0.842689, 0.98445,
    1.02685,  1.04646,  1.11947,  1.01368,  0.877994,
    1.07017,  0.816822, 0.83959,  1.11853,  1.06517,
    0.917841};

#ifndef __CINT__
ClassImp(AtlWjetsScaleFactorTool);
#endif


//____________________________________________________________________
    
AtlWjetsScaleFactorTool::AtlWjetsScaleFactorTool(const char *name,
						     const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fLeptons = 0;
}

//____________________________________________________________________

AtlWjetsScaleFactorTool::~AtlWjetsScaleFactorTool() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlWjetsScaleFactorTool::Clear(Option_t *option) {
    //
    // Clear this tool at the beginning of each event
    //
}

//____________________________________________________________________

void AtlWjetsScaleFactorTool::SetBranchStatus() {
    //
    // Turn on all needed branches
    //
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
    fTree->SetBranchStatus("fElectrons*", kTRUE);
    fTree->SetBranchStatus("fMuons*", kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4LCTopoJets*",       kTRUE);
    fTree->SetBranchStatus("fAntiKt4LCTopoJets*",         kTRUE);
}
//____________________________________________________________________

void AtlWjetsScaleFactorTool::BookHistograms() {
    //
    // Book Histograms
    //
}

//____________________________________________________________________

void AtlWjetsScaleFactorTool::FillHistograms() {
    //
    // Fill Histograms
    //

}

//____________________________________________________________________

void AtlWjetsScaleFactorTool::Print() const {
    //
    // Print tool information
    //
    cout << endl
	 << "========================================================" << endl
	 << "  W+jets Scale Factor Tool " << endl
	 << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

Bool_t AtlWjetsScaleFactorTool::AnalyzeEvent() {
    //
    // Calculate the scale factor for an event
    //

    // Manipulate wjets weight based on pt(W)
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);

    TVector2 met = fEvent->GetEnergySum()->GetMissingEt();
    TVector2 lep = lepton->P3().XYvector();
    Double_t Pt_W = (met+lep).Mod();
    
    Int_t sfbin = TMath::Min(TMath::FloorNint(Pt_W/5.), 40);
    if ( lepton->IsElectron() ) {
	fEvent->SetTagEvtWeight(GetTagEvtWeight()*fgWjetsSF_el[sfbin]);
    } else {
    	fEvent->SetTagEvtWeight(GetTagEvtWeight()*fgWjetsSF_mu[sfbin]);
    }

    return kTRUE;
}

//___________________________________________________________

void AtlWjetsScaleFactorTool::Terminate() {
    //
    // Executed at the end of the tool
    //
}
