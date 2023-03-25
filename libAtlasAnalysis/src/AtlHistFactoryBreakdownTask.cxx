//____________________________________________________________________
//
// DOCUMENT ME
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryBreakdownTask
#include <AtlHistFactoryBreakdownTask.h>
#endif
#include <AtlHistFactoryBreakdownEntry.h>
#include <AtlHistFactoryMeasurement.h>
#include <RooAbsData.h>
#include <RooWorkspace.h>
#include <RooStats/ProfileLikelihoodTestStat.h>
#include <TFile.h>
#include <TString.h>
#include <iostream>

using namespace std;
using namespace RooStats;
using namespace RooFit;


#ifndef __CINT__
ClassImp(AtlHistFactoryBreakdownTask);
#endif

//____________________________________________________________________

AtlHistFactoryBreakdownTask::AtlHistFactoryBreakdownTask(const char* name,
							 const char* title,
							 AtlHistFactoryMeasurement *meas)
    : TTask(name, title) {
    //
    // Default constructor
    //

    fMeasurement = meas;

    // Check if file for combined analysis exists
    TString *filename = fMeasurement->GetModelFileName();
    TFile *f = new TFile(filename->Data());

    if ( f->IsZombie() ) {
	Info("AtlHistFactoryBreakdownTask",
	     "Model file %s does not exist or is corrupted. Deactivate task.",
	     filename->Data());
	Info("AtlHistFactoryBreakdownTask",
	     "Please create a model file first and then recreate this task");
	SetActive(kFALSE);
    } else {
	Info("AtlHistFactoryBreakdownTask",
	     "Model file %s found. Activate task.",
	     filename->Data());
	SetActive(kTRUE);

	Initialize();
    }
}

//____________________________________________________________________

AtlHistFactoryBreakdownTask::~AtlHistFactoryBreakdownTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlHistFactoryBreakdownTask::Initialize() {
    //
    // Create breakdown entries
    //

    Info("Initialize",
	 "Create tasks for breakdown table.");

    TString *ws_filename = fMeasurement->GetModelFileName();
    
    // Open workspace file
    TFile *f_ws = TFile::Open(ws_filename->Data());
    if ( f_ws == 0 ) {
	Error("CreateSystBreakdown", "Couldn't find file with name: %s",
	      ws_filename->Data());
    }
    
    // Get infos for fit
    RooWorkspace *wSpace = (RooWorkspace*) f_ws->Get("combined");

    ModelConfig *model = (ModelConfig*) wSpace->obj("ModelConfig");

    RooRealVar *par = 0;
    
    // Nuisance parameters
    RooArgSet *nuis = (RooArgSet*)model->GetNuisanceParameters();
    TIterator *next_nuis = nuis->createIterator();


    //
    // Start creation of breakdown entries
    //

    TString jobhome = (fMeasurement->GetOutputDir())->Data();
    jobhome.Append("/breakdown_config/");
    jobhome.ReplaceAll("//", "/");

    TString outdir = (fMeasurement->GetOutputDir())->Data();
    outdir.Append("/breakdown/");
    outdir.ReplaceAll("//", "/");

    TString dataset = fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData";

    //
    // 1. All systematics turned on (aka nominal fit)
    //
    
    AtlHistFactoryBreakdownEntry *entry = new AtlHistFactoryBreakdownEntry("Total", "Total",
									   "TotalUncertainty");
    entry->SetMeasurement(fMeasurement);
    entry->SetJobHome(Form("%s/%s/%s", jobhome.Data(), "Total", dataset.Data()));
    entry->SetOutputFile(Form("%s/%s/breakdown_%s_%s.root",
			      outdir.Data(),
			      fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData",
			      "combined",
			      "TotalUncertainty"));
    Add(entry);

    //
    // 2. Only data statistics (keyword is 'data_stat')
    //
    entry = new AtlHistFactoryBreakdownEntry("Data_stat", "Data stat.",
					     "data_stat");
    entry->SetMeasurement(fMeasurement);
    entry->SetJobHome(Form("%s/%s/%s", jobhome.Data(), "data_stat", dataset.Data()));
    entry->SetOutputFile(Form("%s/%s/breakdown_%s_%s.root",
			      outdir.Data(),
			      fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData",
			      "combined",
			      "Datastat"));
    Add(entry);

    //
    // 3. MC statistics only
    //
    entry = new AtlHistFactoryBreakdownEntry("MC_stat", "MC stat.",
					     "gamma_stat");
    entry->SetMeasurement(fMeasurement);
    entry->SetJobHome(Form("%s/%s/%s", jobhome.Data(), "gamma_stat", dataset.Data()));
    entry->SetOutputFile(Form("%s/%s/breakdown_%s_%s.root",
			      outdir.Data(),
			      fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData",
			      "combined",
			      "MCstat"));
    Add(entry);

    //
    // 4. All other NPs
    //
    while ((par = (RooRealVar*)next_nuis->Next())) {
	TString parname = par->GetName();

	// Special config for MC parameters needed
	if ( parname.Contains("gamma_stat") )
	    continue;
	
	entry = new AtlHistFactoryBreakdownEntry(Form("Syst_%s", parname.Data()),
						 Form("Syst_%s", parname.Data()),
						 parname.Data());
	entry->SetMeasurement(fMeasurement);
	entry->SetJobHome(Form("%s/%s/%s", jobhome.Data(), parname.Data(), dataset.Data()));
	entry->SetOutputFile(Form("%s/%s/breakdown_%s_%s.root",
				  outdir.Data(),
				  fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData",
				  "combined",
				  parname.Data()));
				     
	Add(entry);
    }
}



