//____________________________________________________________________
//
// Single-top Entry List Writer
// 
//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlSgTopEntryListWriter
#include <AtlSgTopEntryListWriter.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTopEntryListWriter);
#endif


const Int_t AtlSgTopEntryListWriter::fgNCuts = 2;

//____________________________________________________________________

AtlSgTopEntryListWriter::AtlSgTopEntryListWriter(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTopEntryListWriter::~AtlSgTopEntryListWriter() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopEntryListWriter::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);

    // Switch on only branches we are going to use
    // This will speed up the analysis process considerably
    fTree->SetBranchStatus("fEventHeader*",         kTRUE);
    fTree->SetBranchStatus("fTrigger*",             kTRUE);

}

//____________________________________________________________________

void AtlSgTopEntryListWriter::SetCutDefaults() {
    //
    // Selection cut defaults
    //
//    fWriteEntryList = kTRUE;
    
    fTriggerChoice = "EF_e20_medium";

}

//____________________________________________________________________

void AtlSgTopEntryListWriter::BookHistograms() {
    //
    // Book histograms
    //    

//     // Weighted
//     fHistCutFlow_weighted = new TH1F("h_cut_flow_kinfit_weighted", "Cut-flow KinFitter mode (weighted)",
// 				     fgNCuts, 0,
// 				     fgNCuts);
//     fHistCutFlow_weighted->SetYTitle("Number of Entries");
//     TAxis *axis = fHistCutFlow_weighted->GetXaxis();
//     axis->SetBinLabel(1, "No Cuts");
//     axis->SetBinLabel(2, Form("%s Trigger",fTriggerChoice.Data()));

    
//     // Unweighted    
//     fHistCutFlow_unweighted = new TH1F("h_cut_flow_kinfit_unweighted", "Cut-flow KinFitter mode (unweighted)",
// 				       fgNCuts, 0,
// 				       fgNCuts);
//     fHistCutFlow_unweighted->SetYTitle("Number of Entries");
//     axis = fHistCutFlow_unweighted->GetXaxis();
//     axis->SetBinLabel(1, "No Cuts");
//     axis->SetBinLabel(2, Form("%s Trigger",fTriggerChoice.Data()));

    
}

//____________________________________________________________________

Bool_t AtlSgTopEntryListWriter::ProcessCut() {
    //
    // Event selection
    //

//     // No cut (1)
//     fHistCutFlow_weighted->AddBinContent(1, fEvtWeight);
//     fHistCutFlow_unweighted->AddBinContent(1, TMath::Abs(fEvtWeight));

//     fHistCutFlow2_weighted->AddBinContent(1, fEvtWeight);
//     fHistCutFlow2_unweighted->AddBinContent(1, TMath::Abs(fEvtWeight));


    
    if ( fEvent->HasPassedHLT(fTriggerChoice) == kFALSE ) {

	return kFALSE;
    }


    cout << endl << endl
	 << " ***************************************************************" << endl
	 << " Event with number " << fEvent->EventNr()
	 << " and run number " << fEvent->RunNr()
	 << " added to entry list."
	 << " ***************************************************************" << endl
	 << endl << endl;
    
    // Trigger cut (2)
//     fHistCutFlow_weighted->AddBinContent(2, fEvtWeight);
//     fHistCutFlow_unweighted->AddBinContent(2, TMath::Abs(fEvtWeight));

//     fHistCutFlow2_weighted->AddBinContent(2, fEvtWeight);
//     fHistCutFlow2_unweighted->AddBinContent(2, TMath::Abs(fEvtWeight));
    
    return kTRUE;
}

//____________________________________________________________________


void AtlSgTopEntryListWriter::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //
    
}

//____________________________________________________________________

void AtlSgTopEntryListWriter::Print() const {
    //
    // Print user analysis configuration
    //
    cout
	<< endl
	<< "========================================================" << endl
	<< "  Atlas Single-Top Entry List Writer          " << endl
	<< "========================================================" << endl
	<< "  Chosen trigger item is fTriggerChoice = " << fTriggerChoice << endl
	<< "========================================================" << endl
	<< endl;

}

//____________________________________________________________________

void AtlSgTopEntryListWriter::Terminate() {
    //
    // Terminate routine
    //

    AtlSelector::Terminate();
}

//____________________________________________________________________
