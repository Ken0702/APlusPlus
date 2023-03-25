//____________________________________________________________________
//
// A++ Example Analysis Task
//
// This example demonstrates how to setup an A++ analysis job by
// inheritance from the AtlSelector class. It is shown how the
// routines SetBranchStatus(), BookHistograms(), FillHistograms() and
// ProcessCut(), which have to be defined by the user, should look.
//
// Note that the ProcessCut() routine, which is used for an event
// selection, is optional. The default, which is defined already in
// AtlSelector::ProcessCut(), is to take every event.
//
// For more details see the AtlSelector class.
//
//
// Input: some A++ tree or chain
//
// Ouput: .root file containing some example histograms
//
// Usage:
// 
// {
//   TChain *ch = new TChain("MyInputTree");
//   ch->Add("MyInputFiles*.root");
//   AtlExampleAnalysis *sel = new AtlExampleAnalysis("MyOutputFile.root");
//   ch->Process(sel, "CTRL");
// }
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlExampleAnalysis
#include <AtlExampleAnalysis.h>
#endif
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlExampleAnalysis);
#endif

//____________________________________________________________________

AtlExampleAnalysis::AtlExampleAnalysis(const char* OutputFile) :
  AtlSelector(OutputFile) {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlExampleAnalysis::~AtlExampleAnalysis() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlExampleAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    
    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
    // Switch on only branches we are going to use
    // This will speed up the analysis process considerably
    fTree->SetBranchStatus("fEventHeader*",   kTRUE);
    fTree->SetBranchStatus("fEnergySum*",     kTRUE);
    fTree->SetBranchStatus("fN_AtlFastJets*", kTRUE);
    fTree->SetBranchStatus("fAtlFastJets*",   kTRUE);
    fTree->SetBranchStatus("fN_MCParticles*", kTRUE);
    fTree->SetBranchStatus("fMCParticles*",   kTRUE);
    fTree->SetBranchStatus("fN_Muons*",       kTRUE);
    fTree->SetBranchStatus("fMuons*",         kTRUE);
}

//____________________________________________________________________

void AtlExampleAnalysis::BookHistograms() {
    //
    // Book histograms
    //
    // This example shows also how to create sub-folders inside a
    // .root file in order to group the large amount of histograms one
    // usually has
    //
    
    // Save present working directory
    // (only relevant when creating sub-directories)
    TDirectory *savdir = gDirectory;
    
    // No. of AtlFast jets
    fHistAtlFastNJets = new TH1F("h_atlfast_njets", "No. of AtlFast Jets",
				 10, 0, 10);
    fHistAtlFastNJets->SetXTitle("N_{Jets}");
    fHistAtlFastNJets->SetYTitle("Number of Entries");
    
    // Create folder for MC truth information
    fOutputFile->cd();
    gDirectory->mkdir("mc_truth", "Monte Carlo Truth info");
    gDirectory->cd("mc_truth");
    
    // MC particle's Pt
    fHistMCPrtPt = new TH1F("h_mcprt_pt", "MC Particle Transverse Momentum",
			    200, 0, 200);
    fHistMCPrtPt->SetXTitle("P_{t} (GeV)");
    fHistMCPrtPt->SetYTitle("Number of Entries");
    
    // MC particle's Eta-Phi plane
    fHistMCPrtEtaPhi = new TH2F("h_mcprt_etaphi", "MC Particle Eta-Phi",
				100, -5, 5, 128, -3.2, 3.2);
    fHistMCPrtEtaPhi->SetXTitle("#eta");
    fHistMCPrtEtaPhi->SetYTitle("#phi (rad)");
    fHistMCPrtEtaPhi->SetZTitle("Number of Entries");
    
    // Create folder for muon information
    fOutputFile->cd();
    gDirectory->mkdir("muon", "Staco Muons");
    gDirectory->cd("muon");

    // No. of muons
    fHistNMuons = new TH1F("h_nmuons", "No. of Muons per Event", 10, 0, 10);
    fHistNMuons->SetXTitle("N_{#mu}");
    fHistNMuons->SetYTitle("Number of Entries");

    // Muon Pt
    fHistMuonPt = new TH1F("h_muon_pt", "Muon Transverse Momentum",
			   100, 0, 100);
    fHistMuonPt->SetXTitle("P_{t} (GeV)");
    fHistMuonPt->SetYTitle("Number of Entries");
    
    // Restore pwd
    savdir->cd();
}

//____________________________________________________________________

void AtlExampleAnalysis::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    // No. of AtlFast jets
    fHistAtlFastNJets->Fill(fEvent->GetN_AtlFastJets());
    
    // Loop over all MC particles
    HepMCParticle *prt = 0;
    for ( Int_t i = 0; i < fEvent->GetN_MCParticles(); i++ ) {
	prt = (HepMCParticle*)fEvent->GetMCParticles()->At(i);
	Float_t Pt = prt->Pt();
	fHistMCPrtPt->Fill(Pt, GetPreTagEvtWeight());
	
	// Fill eta-phi plane only for particles with Pt>0 to prevent from
	// annoying Root warnings when computing eta
	if ( Pt != 0. )
	    fHistMCPrtEtaPhi->Fill(prt->Eta(), prt->Phi(), GetPreTagEvtWeight());
    }

    // Fill muon histograms
    fHistNMuons->Fill(fEvent->GetN_Muons());
    for ( Int_t i = 0; i < fEvent->GetN_Muons(); i++ ) {
	fHistMuonPt->Fill(((AtlMuon*)fEvent->GetMuons()->At(i))->Pt(),
			  GetPreTagEvtWeight());
    }
}

//____________________________________________________________________

Bool_t AtlExampleAnalysis::ProcessCut() {
    //
    // Event selection
    //
    // All events are chosen
    //

    return kTRUE;
}

    

