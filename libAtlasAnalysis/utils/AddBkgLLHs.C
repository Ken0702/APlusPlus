#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include <cassert>
#include <iostream>

using namespace std;

void AddBkgLLHs(const char* finSignal,
				const char* finBkg) {
	//
	// Addition of bkg MEM likelihoods for the ATLAS 8 TeV s-channel analysis.
	// 
	// Desired signal likelihoods 'llh_sChannel2j' and 'llh_sChannel3j' contained
	// in 'finSignal'. Bkg likelihoods contained in 'finBkg'.
	// 'finSignal' does not contain branches of bkg likelihoods.
	// 
	// Adding bkg likelihoods to the 'physics' tree in 'finSignal'.
	//
	
	// Open files
	TFile *inFileSig = new TFile(finSignal, "UPDATE");
	assert( inFileSig );
	TFile *inFileBkg = new TFile(finBkg, "READ");
	assert( finBkg );
	
	// Get trees
	TTree *treeSig = (TTree*)inFileSig->Get("physics");
	TTree *treeBkg = (TTree*)inFileBkg->Get("physics");
	
	// Run and Event numbers
	UInt_t RunNumber_bkg   = 0;
	UInt_t EventNumber_bkg = 0;
	UInt_t RunNumber_sig   = 0;
	UInt_t EventNumber_sig = 0;
	
	treeBkg->SetBranchAddress("RunNumber", &RunNumber_bkg);
	treeBkg->SetBranchAddress("EventNumber", &EventNumber_bkg);
	
	treeSig->SetBranchAddress("RunNumber", &RunNumber_sig);
	treeSig->SetBranchAddress("EventNumber", &EventNumber_sig);
	
	// Bkg likleihoods and their errors
	Double_t llh_tChannel4FS    = 0.0;
	Double_t llherr_tChannel4FS = 0.0;
	Double_t llh_ttbarSL        = 0.0;
	Double_t llherr_ttbarSL     = 0.0;
	Double_t llh_ttbarDL        = 0.0;
	Double_t llherr_ttbarDL     = 0.0;
	Double_t llh_Wjj            = 0.0;
	Double_t llherr_Wjj         = 0.0;
	Double_t llh_Wbb            = 0.0;
	Double_t llherr_Wbb         = 0.0;
	Double_t llh_Wcj            = 0.0;
	Double_t llherr_Wcj         = 0.0;
	Double_t llh_WHbb           = 0.0;
	Double_t llherr_WHbb        = 0.0;
	Double_t llh_WHbbj          = 0.0;
	Double_t llherr_WHbbj       = 0.0;
	
	// Will take the bkg likelihoods from the bkg file tree
	treeBkg->SetBranchAddress("llh_tChannel4FS", &llh_tChannel4FS);
	treeBkg->SetBranchAddress("llherr_tChannel4FS", &llherr_tChannel4FS);
	treeBkg->SetBranchAddress("llh_ttbarSL", &llh_ttbarSL);
	treeBkg->SetBranchAddress("llherr_ttbarSL", &llherr_ttbarSL);
	treeBkg->SetBranchAddress("llh_ttbarDL", &llh_ttbarDL);
	treeBkg->SetBranchAddress("llherr_ttbarDL", &llherr_ttbarDL);
	treeBkg->SetBranchAddress("llh_Wjj", &llh_Wjj);
	treeBkg->SetBranchAddress("llherr_Wjj", &llherr_Wjj);
	treeBkg->SetBranchAddress("llh_Wbb", &llh_Wbb);
	treeBkg->SetBranchAddress("llherr_Wbb", &llherr_Wbb);
	treeBkg->SetBranchAddress("llh_Wcj", &llh_Wcj);
	treeBkg->SetBranchAddress("llherr_Wcj", &llherr_Wcj);
	treeBkg->SetBranchAddress("llh_WHbb", &llh_WHbb);
	treeBkg->SetBranchAddress("llherr_WHbb", &llherr_WHbb);
	treeBkg->SetBranchAddress("llh_WHbbj", &llh_WHbbj);
	treeBkg->SetBranchAddress("llherr_WHbbj", &llherr_WHbbj);
	
	// Will write bkg likelihoods to the signal file tree
	TBranch *br_llh_tChannel4FS    = treeSig->Branch("llh_tChannel4FS", &llh_tChannel4FS, "llh_tChannel4FS/D");
	TBranch *br_llherr_tChannel4FS = treeSig->Branch("llherr_tChannel4FS", &llherr_tChannel4FS, "llherr_tChannel4FS/D");
	TBranch *br_llh_ttbarSL        = treeSig->Branch("llh_ttbarSL", &llh_ttbarSL, "llh_ttbarSL/D");
	TBranch *br_llherr_ttbarSL     = treeSig->Branch("llherr_ttbarSL", &llherr_ttbarSL, "llherr_ttbarSL/D");
	TBranch *br_llh_ttbarDL        = treeSig->Branch("llh_ttbarDL", &llh_ttbarDL, "llh_ttbarDL/D");
	TBranch *br_llherr_ttbarDL     = treeSig->Branch("llherr_ttbarDL", &llherr_ttbarDL, "llherr_ttbarDL/D");
	TBranch *br_llh_Wjj            = treeSig->Branch("llh_Wjj", &llh_Wjj, "llh_Wjj/D");
	TBranch *br_llherr_Wjj         = treeSig->Branch("llherr_Wjj", &llherr_Wjj, "llherr_Wjj/D");
	TBranch *br_llh_Wbb            = treeSig->Branch("llh_Wbb", &llh_Wbb, "llh_Wbb/D");
	TBranch *br_llherr_Wbb         = treeSig->Branch("llherr_Wbb", &llherr_Wbb, "llherr_Wbb/D");
	TBranch *br_llh_Wcj            = treeSig->Branch("llh_Wcj", &llh_Wcj, "llh_Wcj/D");
	TBranch *br_llherr_Wcj         = treeSig->Branch("llherr_Wcj", &llherr_Wcj, "llherr_Wcj/D");
	TBranch *br_llh_WHbb           = treeSig->Branch("llh_WHbb", &llh_WHbb, "llh_WHbb/D");
	TBranch *br_llherr_WHbb        = treeSig->Branch("llherr_WHbb", &llherr_WHbb, "llherr_WHbb/D");
	TBranch *br_llh_WHbbj          = treeSig->Branch("llh_WHbbj", &llh_WHbbj, "llh_WHbbj/D");
	TBranch *br_llherr_WHbbj       = treeSig->Branch("llherr_WHbbj", &llherr_WHbbj, "llherr_WHbbj/D");
	
	Int_t nEntriesSig = treeSig->GetEntries();
	Int_t nEntriesBkg = treeBkg->GetEntries();
	if ( nEntriesSig != nEntriesBkg ) {
	    cerr << "AddBkgLLHs : Different number if entries in input trees 'physics'. Abort." << endl;
	    gSystem->Abort(1);
	}
	
	// First loop over tree entries - check if run and event numbers are equal for every tree entry
	for (Int_t iEntry = 0; iEntry < nEntriesSig; ++iEntry) {
		treeBkg->GetEntry(iEntry);
		treeSig->GetEntry(iEntry);
		if ( RunNumber_bkg != RunNumber_sig || EventNumber_bkg != EventNumber_sig ) {
			cerr << "AddBkgLLHs : Rund/Evt numbers not identical. Abort." << endl;
			cerr << "RunNumbers  : " << RunNumber_bkg << "\t" << RunNumber_sig << endl;
			cerr << "EventNumbers: " << EventNumber_bkg << "\t" << EventNumber_sig << endl;
			gSystem->Abort(1);
		}			
	}	
	
	// Loop over tree entries
	for (Int_t iEntry = 0; iEntry < nEntriesSig; ++iEntry) {
		treeBkg->GetEntry(iEntry);
		treeSig->GetEntry(iEntry);
		// Fill bkg likelihood trees in signal file
		br_llh_tChannel4FS->Fill();
		br_llherr_tChannel4FS->Fill();
		br_llh_ttbarSL->Fill();
		br_llherr_ttbarSL->Fill();
		br_llh_ttbarDL->Fill();
		br_llherr_ttbarDL->Fill();
		br_llh_Wjj->Fill();
		br_llherr_Wjj->Fill();
		br_llh_Wbb->Fill();
		br_llherr_Wbb->Fill();
		br_llh_Wcj->Fill();
		br_llherr_Wcj->Fill();
		br_llh_WHbb->Fill();
		br_llherr_WHbb->Fill();
		br_llh_WHbbj->Fill();
		br_llherr_WHbbj->Fill();
	}
	
	// Save likelihoods
	inFileSig->cd();
	treeSig->Write();
	inFileSig->Close();
	delete inFileSig;
	delete inFileBkg;
}
