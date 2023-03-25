//____________________________________________________________________
//
// Class for storing HistFactory sample information
//
// The normfactor for each sample will be used to scale the
// expected number of events of this sample. The norm uncertainty
// (e.g. theory unc.) can be used to constrain the normfactor.
// Currently the distribution is fixed in the AtlHistFactoryTask.
//
// Each sample can hold its own list of systematics.
// - see AtlHistFactoryTask for more details
//
// If 'isSignal' is true, the normfactor will be unconstrained,
// even if a norm unc. is provided.
//
// The sample can also be used to save the results of the shape
// tests.
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySample
#include <AtlHistFactorySample.h>
#endif
#include <AtlHistFactorySystematic.h>
#include <TList.h>
#include <TH1F.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHistFactorySample);
#endif

//____________________________________________________________________

AtlHistFactorySample::AtlHistFactorySample(const char* name,
					   const char* title,
					   const char* normfactor,
					   Double_t norm_unc,
					   Bool_t isSignal) :
    TNamed(name, title), fNormFactor(normfactor) {
    //
    // Default constructor
    //
    fNormUncertainty = norm_unc;

    fIsSignal = isSignal;
    fIsData = kFALSE; // assume this is a MC sample (default)
    
    fListOfSystematics = new TList;

    fHist_ShapeTest_pValue = 0;
    fHist_ShapeTest_KStest = 0;

    // Default signal range
    // Assuming histograms are always normalized to Lumi
    fSignalLow  = 0.;
    fSignalHigh = 5.;

    fUseLogNormal = kFALSE;
    fUseStatError = kTRUE;

    // Default draw style
    fFillColor = kWhite;
    fFillStyle = 0;
    fLineColor = kBlack;
    fLineStyle = kSolid;
}

//____________________________________________________________________

AtlHistFactorySample::~AtlHistFactorySample() {
    //
    // Default constructor
    //
    delete fListOfSystematics;

    if ( fHist_ShapeTest_pValue != 0 )
	delete fHist_ShapeTest_pValue;
    if ( fHist_ShapeTest_KStest != 0 )
	delete fHist_ShapeTest_KStest;
}

//____________________________________________________________________

void AtlHistFactorySample::AddSystematic(AtlHistFactorySystematic *syst) {
    //
    // Add Systematic to channel
    //
    
    fListOfSystematics->Add(syst);
}

//____________________________________________________________________

void AtlHistFactorySample::AddSystematics(TList *list) {
    //
    // Add Systematic to channel
    //
    
    fListOfSystematics->AddAll(list);
}

//____________________________________________________________________

void AtlHistFactorySample::BookHistograms(const char* channel) {
    //
    // Book histograms for this channel
    // They have only one bin, because TH1::Fill(name, weight)
    // will be used to add more bins as needed.
    
    // Delete any existing histograms
    if ( fHist_ShapeTest_pValue != 0 ) {
	delete fHist_ShapeTest_pValue;
	fHist_ShapeTest_pValue = 0;
    }
    
    fHist_ShapeTest_pValue = new TH1F(Form("%s_%s_pValue",
					   channel,
					   fName.Data()),
				      Form("p-value of Chi2 test for channel '%s'", channel),
				      1, 0., 1.);

    if ( fHist_ShapeTest_KStest != 0 ) {
	delete fHist_ShapeTest_KStest;
	fHist_ShapeTest_KStest = 0;
    }
    fHist_ShapeTest_KStest = new TH1F(Form("%s_%s_KStest",
					   channel,
					   fName.Data()),
				      Form("p-value of Chi2 test for channel '%s'", channel),
				      1, 0., 1.);
}

//____________________________________________________________________

void AtlHistFactorySample::FillHistograms(const char* syst,
					  Double_t pval,
					  Double_t kstest) {
    //
    // Fill shape test histograms
    //
    
    fHist_ShapeTest_pValue->Fill(syst, pval);
    fHist_ShapeTest_KStest->Fill(syst, kstest);
}

//____________________________________________________________________

void AtlHistFactorySample::WriteHistograms(TFile *fout) {
    //
    // Save histograms to file
    //

    // Change current directory to file in order to use TH1::Write()
    fout->cd();

    // Save an unsorted version
    TH1F *hist_pValue_unsort = (TH1F*) fHist_ShapeTest_pValue->Clone(Form("%s_unsrt", fHist_ShapeTest_pValue->GetName()));
    hist_pValue_unsort->LabelsDeflate();
    hist_pValue_unsort->SetMarkerStyle(20);
    hist_pValue_unsort->SetMarkerColor(kBlue);
    hist_pValue_unsort->LabelsOption("v");
    hist_pValue_unsort->Write();
    
    // Set some basic options
    fHist_ShapeTest_pValue->LabelsDeflate();
    fHist_ShapeTest_pValue->LabelsOption("<v");
    fHist_ShapeTest_pValue->SetMarkerStyle(20);
    fHist_ShapeTest_pValue->SetMarkerColor(kBlue);
    
    fHist_ShapeTest_pValue->Write();

    TH1F *hist_KStest_unsort = (TH1F*) fHist_ShapeTest_KStest->Clone(Form("%s_unsrt", fHist_ShapeTest_KStest->GetName()));
    hist_KStest_unsort->LabelsDeflate();
    hist_KStest_unsort->SetMarkerStyle(20);
    hist_KStest_unsort->SetMarkerColor(kBlue);
    hist_KStest_unsort->LabelsOption("v");
    hist_KStest_unsort->Write();

    // Set some basic options
    fHist_ShapeTest_KStest->LabelsDeflate();
    fHist_ShapeTest_KStest->LabelsOption("<v");
    fHist_ShapeTest_KStest->SetMarkerStyle(20);
    fHist_ShapeTest_KStest->SetMarkerColor(kBlue);
    
    fHist_ShapeTest_KStest->Write();
}

//____________________________________________________________________

void AtlHistFactorySample::SetSignalRange(Double_t low, Double_t high) {
    //
    // Defines the range for the NormFactor for this sample. Values
    // set here, only apply for signal samples.
    //
    // Background samples are constrained by the norm uncertainty,
    // which is provided during construction of this object.
    //

    if ( low > high ) {
	Error("SetSignalRange",
	      "Lower bound is greater than upper bound. Abort!");
	gSystem->Abort();
    }	      

    fSignalLow  = low;
    fSignalHigh = high;
}

//____________________________________________________________________

void AtlHistFactorySample::AddShapeSystematic(const char* name,
					      TList *systs) {
    //
    // Add shape systematic to this sample
    //
    // The systematic is taken from the provided list, cloned and then
    // added to the list of systematics.
    //
    
    AtlHistFactorySystematic *syst_org   = (AtlHistFactorySystematic*) systs->FindObject(name);
    if ( syst_org == 0 ) {
	Error("AddShapeSystematic",
	      "Could not find any sytematic with name %s. Abort!",
	      name);
	gSystem->Abort();
    }
    AtlHistFactorySystematic *syst_shape = (AtlHistFactorySystematic*) syst_org->Clone();
    syst_shape->SetUseShape(kTRUE);

    fListOfSystematics->Add(syst_shape);
}
