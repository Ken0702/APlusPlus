//____________________________________________________________________
//
// Class for managing tasks associated with HistFactory
//
// Currently thre functions are implemented:
//
// - CreateTemplates()
//   Used for creating all templates for fitting and for shape tests,
//   respectively. All templates are saved in a file, one file
//   per channel.
//   For the fit templates, the channel discriminant will be used and the
//   corresponding histograms are saved.
//   For shape analysis, the DiscrimantRef and DiscriminantShape will be
//   used. These have to be set defined for each channel separatly.
//
// - PerformShapeTests()
//   Runs Chi2 and KS tests and save results in a dedicated file, one
//   file per channel.
//
// - CreateWorkSpace()
//   Creates the histfactory workspace and saves it to disk.
//
// A detailed example for the configuration can be found in
// $LIBSINGLETOP/tasks/task_sChannelAnalysis_HistFactory.C
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryTask
#include <AtlHistFactoryTask.h>
#endif
#include <AtlHistFactoryMeasurement.h>
#include <AtlHistFactoryChannel.h>
#include <AtlHistFactorySample.h>
#include <AtlHistFactorySystematic.h>
#include <HepDataMCPlot.h>
#include <RooAbsData.h>
#include <RooCategory.h>
#include <RooCatType.h>
#include <RooConstVar.h>
#include <RooFitResult.h>
#include <RooProduct.h>
#include <RooRealSumPdf.h>
#include <RooRealVar.h>
#include <RooSimultaneous.h>
#include <RooStats/ProfileLikelihoodTestStat.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TSystem.h>
#include <iostream>

using namespace std;
using namespace RooStats;
using namespace HistFactory;
using namespace RooFit;

#ifndef __CINT__
ClassImp(AtlHistFactoryTask);
#endif

//____________________________________________________________________

AtlHistFactoryTask::AtlHistFactoryTask(const char* name,
				       const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    fMeasurement = 0;
    fRunMode = kUnknown;
    fTemplateDir = 0;
    fHistFactoryDir = 0;
    fScheme = 0;

    // Pull plot configuration
    fNPullsPerPad    = 14;
    fPullScaleFactor = 10.;
}

//____________________________________________________________________

AtlHistFactoryTask::~AtlHistFactoryTask() {
    //
    // Default destructor
    //
    delete fTemplateDir;
}

//____________________________________________________________________

void AtlHistFactoryTask::ExecInteractiveJob(Option_t* option) {
    //
    // Exec interactive job
    //

    // Convert option string
    TString opt = option;
    opt.ToLower();
    
    if ( fRunMode == kCreateFitTemplates ||
	 fRunMode == kCreateShapeTemplates ) {
	CreateTemplates();
    } else if ( fRunMode == kPerformShapeTests ) {
	PerformShapeTests();
    } else if ( fRunMode == kCreateWorkspace ) {
	CreateWorkspace();
    } else if ( fRunMode == kFitModel ) {
	FitModel();
    } else if ( fRunMode == kRateTable ) {
	ExportRateUncertainties();
    } else if ( fRunMode == kHypoTestLimit ) {
	RunLimit();
    } else {
	Error("ExecInteractiveJob", "Unknown run mode! Try again.");
    }
}

//____________________________________________________________________

void AtlHistFactoryTask::ExecGridJob(Option_t*) {
    //
    // Exec grid job
    //
    Error("ExecGridJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

Bool_t AtlHistFactoryTask::ExecBatchJob(Option_t*) {
    //
    // Exec batch job
    //
    Error("ExecBatchJob", "Not supported! Abort!");
    gSystem->Abort(0);

    return kFALSE;
}

//____________________________________________________________________

void AtlHistFactoryTask::CreateRunScript(Option_t *option) {
    //
    // Create Run Script
    //
    Error("ExecGridRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHistFactoryTask::ExecNAFBatchJob(Option_t*) {
    //
    // Exec NAF batch job
    //
    Error("ExecNAFBatchJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHistFactoryTask::CreateNAFBatchRunScript() {
    //
    // Create NAF Batch Run Script
    //
    Error("ExecNAFBatchRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHistFactoryTask::CreateGridRunScript() {
    //
    // Create Grid Run Script
    //
    Error("ExecGridRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHistFactoryTask::Initialize() {
    //
    // Initialize HistFactory Task
    // - check if measurement is set
    // - set directories used by AtlTask
    //

    if ( fMeasurement == 0 ) {
	Error("Initialize",
	      "No measurement provided. Abort");
	gSystem->Abort();
    }

    TString outdir(gSystem->ExpandPathName(fMeasurement->GetOutputDir()->Data()));
    if ( !outdir.BeginsWith("/") ) {
	Error("Initialize",
	      "Output directory has to start with '/'. Abort!");
	gSystem->Abort();
    }
    
    // dummy outputfile needed for AtlTask
    SetOutputFile(Form("%s/HistFactory.root",
		       outdir.Data()));

    // Set pointer to output directory
    if ( fHistFactoryDir == 0 ) 
	fHistFactoryDir = GetDirectoryName("");

    // Set pointer to templates directory
    if ( fTemplateDir == 0 )
 	fTemplateDir = GetDirectoryName("templates");
}

//____________________________________________________________________

void AtlHistFactoryTask::CreateTemplates() {
    //
    // Create templates for given measurement
    //

    // Several problems need to be solved:
    // 1. We need to loop over all systematics to reduce
    //    IO operations
    //    --> However, not all samples have information for all
    //        systematics?
    //        E.g. a) QCD has no systematics or special systematics
    //             b) Generator comparison only effects one specific sample
    //             c) nominal has only one sample
    //
    // 2. Sample specific configuration for systematics:
    //    --> Shape info only for subset of all samples
    //    --> Other special configuration for systematics?
    //
    // Solution for 2.:
    // --> Save a list of systematics with special configurations
    //     These overwrite the general channel list
    // x> Here: not important because we don't need this information
    //          when creating the template file
    //
    // Solution for 1.:
    // --> b) At then end of this loop, loop over the list of systematics
    //        of each sample to get systematics that are only valid
    //        for a specific sample.
    // --> a) Samples that don't have infos on systematics will have the
    //        nominal histo saved as up and down variation (e.g. QCD)
    //        The nominal histo is used in the stack plot for the systematic!
    //        This problem is already solved during plotting!
    // --> c) Let the systematic save it's own template! Just provide the file pointer!

    fScheme = fMeasurement->GetScheme();
    if ( fScheme == 0 ) {
	Error("CreateTemplates", "Plotting folder not set. Abort!");
	gSystem->Abort();
    }
    
    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	
	// Create one outputfile per channel
	TFile *f = 0;

	// Create root file
	// (store shape and fit templates in different root files)
	TString filename;
	if ( fRunMode == kCreateShapeTemplates ) {
	    GetShapeTemplatesFileName(ch->GetName(), filename);
	    f = new TFile(filename.Data(), "RECREATE");
	    Info("CreateTemplates",
		 "Store shape templates in file %s",
		 f->GetName());
	} else {
	    GetTemplatesFileName(ch->GetName(), filename);
	    f = new TFile(filename.Data(), "RECREATE");
	    Info("CreateTemplates",
		 "Store fit templates in file %s",
		 f->GetName());
	}

	// Get the name of the discriminant
	// - fit templates:   GetDiscriminant()
	// - shape templates: GetShapeDiscriminantRef()  for nominal
	//                    GetShapeDiscriminantSyst() for all systematics

	TString *DiscriminantRef  = ch->GetShapeDiscriminantRef();
	TString *DiscriminantSyst = ch->GetShapeDiscriminantSyst();
	TString *DiscriminantFit  = new TString(ch->GetDiscriminant());

	if ( fRunMode == kCreateShapeTemplates &&
	     (DiscriminantRef == 0 || DiscriminantSyst == 0) ) {
	    
	    Error("CreateTemplates", "Shape histogram 'ref' or 'syst' is not set. Abort!");
	    gSystem->Abort();
	}

	if ( fRunMode == kCreateFitTemplates &&
	     DiscriminantFit == 0 ) {

	    Error("CreateTemplates", "Discriminant histogram 'fit' is not set. Abort!");
	    gSystem->Abort();
	}
	
	// Loop over all systematics for this channel
	TIter next_syst(ch->GetListOfSystematics());
	AtlHistFactorySystematic *syst = 0;
	while ( (syst = (AtlHistFactorySystematic*) next_syst()) ) {

	    // Skip systematics?
	    if ( fMeasurement->GetNoSystematics() && !syst->IsNominal() ) 
		continue;
	    
	    // Initialize systematic for this channel
	    // - Set discriminant for this channel
	    // - Set Base dir of MCPlotter files

	    // Get the name of the discriminant
	    // - fit templates:   GetDiscriminant()
	    // - shape templates: GetShapeDiscriminantRef()  for nominal
	    //                    GetShapeDiscriminantSyst() for all systematics
	    
	    if ( fRunMode == kCreateShapeTemplates ) {
		// Choose the reference shape template for nominal
		if ( syst->IsNominal() )
		    syst->SetDiscriminant(DiscriminantRef->Data());
		else
		    syst->SetDiscriminant(DiscriminantSyst->Data());
		
	    } else {
		// Choose the channel discriminant
		syst->SetDiscriminant(DiscriminantFit->Data());
	    }
	    syst->Initialize(ch->GetMCPlotterBaseDir(),
			     fScheme->Data());
	    
	    // Loop over all processes (samples) in one channel
	    TIter next_sample(ch->GetListOfSamples());
	    AtlHistFactorySample *sample = 0;
	    while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
		
		// Initialize systematic for this sample/process
		syst->ChangeProcess(sample->GetName());

		// For each process and systematic, write histograms
		// to output file
		cout << "Save templates for process: " << sample->GetTitle() << endl;
		cout << "Systematic: " << syst->GetTitle() << endl;
		syst->SaveTemplates(f);
	    } // end of sample loop

	    // Clear systematic (close files, reset internal histograms, etc.)
	    syst->Clear();
	    
	} // end of syst loop
	
	// Loop for sample specific systematics
	TIter next_sample(ch->GetListOfSamples());
	AtlHistFactorySample *sample = 0;
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {

	    // Loop over all systematics for this sample
	    TIter next_sample_syst(sample->GetListOfSystematics());
	    AtlHistFactorySystematic *sample_syst = 0;
	    while ( (sample_syst = (AtlHistFactorySystematic*) next_sample_syst()) ) {

		// Skip systematics?
		if ( fMeasurement->GetNoSystematics() && !sample_syst->IsNominal() ) 
		    continue;
		
		// Check if templates for this systematic already exist
		TList *chan_systs = ch->GetListOfSystematics();
		TObject *syst2 = chan_systs->FindObject(sample_syst->GetTitle());
		if ( syst2 != 0 ) // systematic exists and was already produced
		    continue;     // therefore skip template generation
		
		// Initialize systematic for this sample
		// - Set discriminant for this channel
		// - Set Base dir of MCPlotter files
		if ( fRunMode == kCreateShapeTemplates ) {
		    // Choose the reference shape template for nominal
		    if ( sample_syst->IsNominal() )
			sample_syst->SetDiscriminant(DiscriminantRef->Data());
		    else
			sample_syst->SetDiscriminant(DiscriminantSyst->Data());
		} else {
		    // Choose the channel discriminant
		    sample_syst->SetDiscriminant(DiscriminantFit->Data());
		}
		sample_syst->Initialize(ch->GetMCPlotterBaseDir(),
					fScheme->Data());

		// Initialize systematic for this sample/process
		sample_syst->ChangeProcess(sample->GetName());
		
		// For each process and systematic, write histograms
		// to output file
		cout << "Save templates for process: " << sample->GetTitle() << endl;
		cout << "Systematic: " << sample_syst->GetTitle() << endl;
		sample_syst->SaveTemplates(f);
	    } // end of sample systematic loop
	} // end of sample loop
	
	// Export data once per channel (if needed)
	if ( fRunMode == kCreateFitTemplates &&
	     fMeasurement->GetUseAsimovData() == kFALSE ) {
	    
	    cout << "Save DATA template." << endl;

	    // ToDo: Unify if "nominal" systematic is not used as nominal reference
	    TFile *fPlotterFile = TFile::Open(Form("%s/nominal/%s/MCPlotter.root",
						   ch->GetMCPlotterBaseDir(),
						   fScheme->Data()));
	    HepDataMCPlot *Hep = (HepDataMCPlot*) fPlotterFile->Get(DiscriminantFit->Data());

	    TH1 *h_data = Hep->GetHistDATA();
	    if ( h_data == 0 ) {
		Error(__FUNCTION__, "Could not find data histogram. Abort!");
		gSystem->Abort();
	    }
	    h_data->SetName("data");

	    f->cd();
	    h_data->Write();
	    
	    fPlotterFile->Close();
	    delete fPlotterFile;
	}

	// Histograms already written to file, just close it here
	// f->Write();
	cout << "Closing file" << endl;
	f->Close();
	// Free memory
	delete f;
    } // end of channel loop
		
}

//____________________________________________________________________

void AtlHistFactoryTask::SetRunMode(ERunMode mode) {
    //
    // Turn on/off creation of templates
    //

    fRunMode = mode;
    cout << GetName() << " - RunMode = " << fRunMode << endl;
    
    if ( mode == kCreateFitTemplates ||
	 mode == kCreateShapeTemplates ||
	 mode == kPerformShapeTests ||
	 mode == kCreateWorkspace ||
	 mode == kFitModel ||
	 mode == kRateTable ||
	 mode == kHypoTestLimit )
	SetInteractiveJob(kTRUE); // these tasks are always interactive
}

//____________________________________________________________________

void AtlHistFactoryTask::PerformShapeTests() {
    //
    // Create templates for given measurement
    //

    // Get directories (create them if necessary)
    TString *shape_dir    = GetDirectoryName("shape_tests");

    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	
	// Create one outputfile per channel
	TFile *f = new TFile(Form("%s/%s_shape_tests.root",
				  shape_dir->Data(),
				  ch->GetName()),
			     "RECREATE");
	Info("PerformShapeTests",
	     "Store shape test results in file:\n%s",
	     f->GetName());

	// Open shape template file
	TString filename;
	GetShapeTemplatesFileName(ch->GetName(), filename);
	
	TFile *f_shape_templates = TFile::Open(filename.Data());
	if ( f_shape_templates == 0 ) {
	    Error("PerformShapeTests",
		  "Could not find shape template file. Abort!");
	    gSystem->Abort();
	}

	// Open fit template file (use for systematics
	// with disjoint set of events only!)
	GetTemplatesFileName(ch->GetName(), filename);
	
	TFile *f_templates = TFile::Open(filename.Data());
	if ( f_templates == 0 ) {
	    Warning("PerformShapeTests",
		    "Could not find fit template file. It might not be needed.");
	}
	
	// Book Histograms for each sample once
	TIter next_sample(ch->GetListOfSamples());
	AtlHistFactorySample *sample = 0;
	// change directory to output file
	// otherwise the histograms will be associated with the
	// shape template file which gets closed before writing the histos.
	f->cd();
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
	    sample->BookHistograms(ch->GetName());
	}

	
	// Loop over all processes (samples) in one channel
	next_sample.Reset();
	sample = 0;
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {

	    // Open pdf file (no plot is added to pdf)
	    TString *export_dir = GetDirectoryName(Form("shape_tests/%s",
							ch->GetName()));
	    TCanvas *c = new TCanvas("canvas", "canvas");
	    c->SaveAs(Form("%s/%s.pdf[", export_dir->Data(), sample->GetName()));
	    
	    // Loop over all systematics for this channel
	    // append here all systematics for this sample
	    // -> works since the list will be updated for each sample!
	    TList *systematics = MergeListOfSystematics(ch->GetListOfSystematics(),
							sample->GetListOfSystematics());
	    
	    TIter next_syst(systematics);
	    AtlHistFactorySystematic *syst = 0;
	    TH1F *Chi2Dist = 0;
	    while ( (syst = (AtlHistFactorySystematic*) next_syst()) ) {
		
		if ( syst->IsNominal() )
		    continue;
		
		// Set template file
		if ( syst->GetUseFullStats() ) {
		    // if nominal and the systematic use different set of events
		    // use the full statistics for the shape comparison
		    // -> In that case use the fit templates!
		    if ( f_templates == 0 ) {
			Error("PerformShapeTests",
			      "Could not find fit template file. It's needed now. Abort!");
			gSystem->Abort();
		    }
		    syst->SetTemplateFile(f_templates);
		} else {
		    // if nominal and the systematic share the same events, use
		    // histograms with disjoint set of events
		    // -> In that case use the shape templates
		    // (the user is responsible that these histograms are not
		    //  correlated!)
		    syst->SetTemplateFile(f_shape_templates);
		}
		
		// Initialize systematic for this sample/process
		syst->ChangeProcess(sample->GetName());

		// Use pseudo exp. for shape tests?
		if ( fMeasurement->GetUsePseudoExp() ) {
		    syst->SetUsePseudoExp();

		    if ( Chi2Dist == 0 ) {
			Chi2Dist = syst->GetChi2Distribution();
			// keep it in root memory, even if the
			// shape file is closed. Otherwise it will
			// cause a seg. fault if it is accessed in the
			// GetChi2Distribution function
			Chi2Dist->SetDirectory(0);
		    } else {
			syst->SetChi2Distribution(Chi2Dist);
		    }
		}
		
		// Perform shape tests
		Double_t pval_min;  // minimum of up and down p-values
		Double_t ks_min;    // minimum of up and down KS-values
		syst->PerformShapeTest(pval_min,
				       ks_min);
		
		sample->FillHistograms(syst->GetTitle(),
				       pval_min, ks_min);

		// save ctrl plots for shapes
		syst->SaveShapeCtrlPlots(f, sample->GetName());
		if ( fMeasurement->GetExportShapePlots() ) {
		    syst->ExportShapePlots(sample->GetName(),
					   export_dir->Data());
		}
		
		// Clear systematic (close files, reset internal histograms, etc.)
		syst->Clear();
		
	    } // end of syst loop for this sample
	    
	    delete systematics; // clean up systematic list

	    // Close pdf for this sample
	    c->SaveAs(Form("%s/%s.pdf]", export_dir->Data(), sample->GetName()));
	} // end of sample loop for this channel
	
	// Close shape template file, once all systematics are done
	f_shape_templates->Close();

	
	// Loop over all processes (samples) in one channel
	// in order to save the histograms
	next_sample.Reset();
	sample = 0;
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
	    sample->WriteHistograms(f);
	}
	
	// Histograms already written to file, just close it here
	// f->Write();
	f->Close();
	// Free memory
	delete f;

    } // end of channel loop

    // Clean up directory strings
    delete shape_dir;
}

//____________________________________________________________________

void AtlHistFactoryTask::CreateWorkspace() {
    //
    // Create RooStats workspace using HistFactory
    //

    // ToDo:
    // Add new function:
    // - CreateWorkspace(const char* out_prefix) {
    //    fMeasurement->SetOutPrefix(out_prefix);
    //    CreateWorkspace();
    //   }
    // (if needed)
    //
    
    // Create the measurement
    Measurement meas = Measurement(fMeasurement->GetName(),
				   fMeasurement->GetTitle());
    
    // Misc. options
    TString *prefix = fMeasurement->GetOutPrefix();
    meas.SetOutputFilePrefix(prefix->Data());
    meas.SetExportOnly(kTRUE); // export here only, do fitting separatly
    
    // Set Lumi
    meas.SetLumi(fMeasurement->GetLumi());
    meas.SetLumiRelErr(fMeasurement->GetLumiRelError());

    // Set Parameters of interest
    TList *pois = fMeasurement->GetPOIs();
    if ( pois->GetEntries() == 0 ) {
	Error("CreateWorkspace",
	      "Histfactory needs a parameter of interest (POI). Abort!");
	gSystem->Abort();
    }
    
    TIter next_poi(pois);
    TObjString *poi = 0;
    while ( (poi = (TObjString*) next_poi()) ) {
	meas.AddPOI(poi->GetString().Data());
    }
    next_poi.Reset();

    // Set Range for all observables
    // - not very useful if one has more than one observable
    // - choose final binning when creating discriminant histogram to
    //   avoid this option!
    // if ( fMeasurement->GetBinLow() != -1 )
    // 	meas.SetBinLow(fMeasurement->GetBinLow());
    // if ( fMeasurement->GetBinHigh() != -1 )
    // 	meas.SetBinHigh(fMeasurement->GetBinHigh());
    
    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	
	// Open template file (one per channel)
	TString filename;
	GetTemplatesFileName(ch->GetName(), filename);
	
	TFile *f_templates = TFile::Open(filename.Data());
	
	if ( f_templates == 0 ) {
	    Error("CreateWorkspace",
		  "Could not find template file. Abort!");
	}

	// Add Channel
	Channel chan(ch->GetName());
	chan.SetStatErrorConfig( 0.0, "Gaussian" ); // ToDo: Check this
	
	// Add data if needed
	if ( fMeasurement->GetUseAsimovData() == kFALSE )
	    chan.SetData("data", f_templates->GetName());
	
	// Loop over all processes (samples) in one channel
	TIter next_sample(ch->GetListOfSamples());
	AtlHistFactorySample *sample = 0;
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
	    
	    Sample HFSample(sample->GetName());
	    if ( sample->UseStatError() ) {
		HFSample.ActivateStatError();
	    }
	    HFSample.SetInputFile(f_templates->GetName());

	    // Loop over all systematics
	    // if systematics with the same name exist,
	    // the sample systematic configuration is used.
	    AtlHistFactorySystematic *syst = 0;
	    TList *systematics = MergeListOfSystematics(ch->GetListOfSystematics(),
							sample->GetListOfSystematics());
	    
	    TIter next_syst(systematics);
	    while ( (syst = (AtlHistFactorySystematic*) next_syst()) ) {

		// Only nominal?
		if ( fMeasurement->GetNoSystematics() && !syst->IsNominal() )
		    continue;		    
		
		// Set template file
		syst->SetTemplateFile(f_templates);

		// Load process histogram
		syst->ChangeProcess(sample->GetName());
		
		if ( syst->IsNominal() ) {
		    // Setup nominal templates and nominal parameters
		    // such as norm uncertainty

		    // Use strings since shape systematics rely on strings
		    HFSample.SetHistoName(syst->GetHistNominal()->GetName());
		    HFSample.SetHistoPath("");

		    if ( sample->IsSignal() ) {
			// Define signal parameter --> AddNormFactor()
			HFSample.AddNormFactor(sample->GetNormFactorName(),
					       1.,
					       sample->GetSignalLow(),
					       sample->GetSignalHigh());

			Bool_t found = kFALSE;
			while ( (poi = (TObjString*) next_poi()) ) {
			    TString poi_str = poi->GetString();
			    found = found || poi_str.Contains(sample->GetNormFactorName());
			}
			next_poi.Reset();
			if ( !found ) {
			    Error("CreateWorkspace",
				  "Signal Normfactor does not match any POI. Please check! Abort.");
			    gSystem->Abort();
			}
		    } else {
			// Bkg processes are constrained --> AddOverallSys()
			HFSample.AddOverallSys(sample->GetNormFactorName(),
					       1. - sample->GetNormUncertainty(),
					       1. + sample->GetNormUncertainty());
			if ( sample->UseLogNormal() )
			    meas.AddLogNormSyst(sample->GetNormFactorName(),
						sample->GetNormUncertainty());
		    }
		} else {
		    // Skip all data driven samples here
		    if ( sample->IsData() ) continue;
		    
		    if ( syst->GetUseShape() ) {
			// Add shape uncertainty
			// sadly we need to give strings... how insane
			HFSample.AddHistoSys(syst->GetName(),
					     syst->GetHistDown()->GetName(),
					     f_templates->GetName(), "",
					     syst->GetHistUp()->GetName(),
					     f_templates->GetName(), "");
		    } else {
			// Add only rate uncertainty
			Double_t rate_up, rate_down;
			syst->GetRateUnc(rate_up, rate_down);
			HFSample.AddOverallSys(syst->GetName(),
					       1. - rate_down, // Low
					       1. + rate_up);  // High
		    }
		}
		
		// Clear systematic (close files, reset internal histograms, etc.)
		syst->Clear();
	    } // end of syst loop

	    // Add each sample to the channel
	    chan.AddSample(HFSample);

	} // end of sample loop
	
	// Add each channel to the measurement
	meas.AddChannel(chan);

	// Close shape template file, once all systematics are done
	// f_templates->Close();
	// do not close the file here, the histogram is then not accesible anymore
    } // end of channel loop

    gSystem->cd(fHistFactoryDir->Data());

    // Redirect all output from next function(s) to a logfile.
    // a) Create logfile directory
    TString *log_dir = GetDirectoryName("logs");
    
    // b) Remove old logs
    TString logfile = Form("%s/MakeModelAndMeasurementFast.log",
			   log_dir->Data());
    gSystem->Exec(Form("rm -f %s", logfile.Data()));
    Info("CreateWorkspace", "rm -f %s", logfile.Data());

    delete log_dir;

    // c) redirect
    gSystem->RedirectOutput( logfile.Data() );

    // Collect histograms from shape systematics
    meas.CollectHistograms();

    // Save Measurement and XML
    meas.PrintTree();

    // Create model first (creates directory)
    MakeModelAndMeasurementFast(meas);

    // Print xml
    meas.PrintXML(Form("%s/xml",
		       gSystem->DirName(fMeasurement->GetOutPrefix()->Data())));

    // d) restore std out and std err
    gSystem->RedirectOutput(0);
}

//____________________________________________________________________

void AtlHistFactoryTask::FitModel() {
    // 
    // Fit Model
    //

    // Fit all channels individually
    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	TString *filename = fMeasurement->GetModelFileName(ch->GetName());
	
	// Run Fit for each channel individually
	RunFit(filename->Data(),
	       ch->GetName(),
	       fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData");
	delete filename;
    }
    
    // Run combined fit
    TString *filename = fMeasurement->GetModelFileName();
    RunFit(filename->Data(),
	   "combined",
	   fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData");

    // Run pulls for combined only (for now)
    if ( fMeasurement->GetRunPullPlots() )
	CreatePullPlots(filename->Data(),
			"combined",
			fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData");
    
    delete filename;
}

//____________________________________________________________________

void AtlHistFactoryTask::RunFit(const char* ws_filename,
				const char* ws_name,
				const char* data_name) {
    //
    // Fit model saved in workspace
    //

    // Create log directory
    TString *logdir = GetDirectoryName("fit");

    TString logfile = logdir->Data();
    logfile.Append(Form("fit_%s_%s.log",
			ws_name,
			data_name));
    gSystem->Exec(Form("rm -f %s", logfile.Data()));
    
    // Open workspace file
    TFile *f_ws = TFile::Open(ws_filename);
    if ( f_ws == 0 ) {
	Error("RunFit", "Coulnd't find file with name: %s",
	      ws_filename);
    }
    
    // Get infos for fit
    RooWorkspace *wSpace = (RooWorkspace*) f_ws->Get(ws_name);
    RooAbsData   *data   = wSpace->data(data_name);
    
    ModelConfig *model = (ModelConfig*) wSpace->obj("ModelConfig");
    RooAbsPdf   *pdf   = model->GetPdf();

    // Redirect output
    gSystem->RedirectOutput(logfile.Data());

    RooFitResult *fitResult = pdf->fitTo(*data,
					 // RooFit::Minos(kTRUE),
    					 RooFit::SumW2Error(kFALSE),
    					 RooFit::Offset(true),
    					 RooFit::Extended(true), // overall number of events is fit parameter
    					 RooFit::Save(true));    // a non-zero fit result pointer is returned
    fitResult->Print();
    
    // Restore std output
    gSystem->RedirectOutput(0);

    fitResult->Print();

    TString Name_ws = ws_name;
    if ( Name_ws.Contains("combined") ) {
	if ( fMeasurement->GetExportCorrMatrix() ) {
	    // save correlation matrix histogram
	    TFile *f = new TFile(Form("%s/%s_corr.root",
				      logdir->Data(),
				      data_name), "RECREATE");
	    TH2* hCorr = fitResult->correlationHist();
	    hCorr->SetDirectory(f);
	    hCorr->Write();
	    f->Close();
	}

	TFile *fresult = new TFile(Form("%s/%s_fitresult.root",
					logdir->Data(),
					data_name), "RECREATE");
	fitResult->Write();
	fresult->Close();
	delete fresult;
    }
    
    // Close file
    f_ws->Close();
    delete f_ws;
    delete logdir;
}

//____________________________________________________________________

void AtlHistFactoryTask::CreatePullPlots(const char* ws_filename,
					 const char* ws_name,
					 const char* data_name) {
    //
    // Create pull plots for given workspace
    //

    // Create log directory
    TString *pulldir = GetDirectoryName("pulls");
    
    TString pullfile = pulldir->Data();
    pullfile.Append(Form("pull_%s_%s.root",
			 ws_name,
			 fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));
    gSystem->Exec(Form("rm -f %s", pullfile.Data()));

    // Open workspace file
    TFile *f_ws = TFile::Open(ws_filename);
    if ( f_ws == 0 ) {
	Error("CreatePullPlots", "Coulnd't find file with name: %s",
	      ws_filename);
    }

    // Get infos for fit
    RooWorkspace *wSpace = (RooWorkspace*) f_ws->Get(ws_name);
    RooAbsData   *data   = wSpace->data(data_name);

    ModelConfig *model = (ModelConfig*) wSpace->obj("ModelConfig");
    RooAbsPdf   *pdf   = model->GetPdf();

    RooFitResult *fitResult  = 0;
    RooRealVar   *par        = 0;
    
    // Book Histograms for Pull Info
    TFile *fout = new TFile(pullfile.Data(),"RECREATE");
    
    TH1F *h_pull = new TH1F("h_pulls", "Pull fit value", 1., 0., 1.);
    TH1F *h_pull_err_up   = new TH1F("h_pulls_err_up", "Upper Error of Pulls", 1., 0., 1.);
    TH1F *h_pull_err_down = new TH1F("h_pulls_err_down", "Lower Error of Pulls", 1., 0., 1.);

    // Get Pulls for all NP
    // ====================

    // Nuisance parameters
    RooArgSet *nuis = (RooArgSet*)model->GetNuisanceParameters();
    TIterator *next_nuis = nuis->createIterator();

    // Fit model
    // ---------
    // Redirect output
    gSystem->Exec(Form("rm -f %s/postfit_%s.log",
		       pulldir->Data(),
		       fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));
    gSystem->RedirectOutput(Form("%s/postfit_%s.log",
				 pulldir->Data(),
				 fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));
    
    fitResult = pdf->fitTo(*data,
 			   RooFit::SumW2Error(kFALSE),
			   RooFit::Offset(true),
			   RooFit::Extended(true),
			   RooFit::Save(true));
    fitResult->Print();

    // Redirect output
    gSystem->RedirectOutput(0);

    // Save postfit snapshot with different names
    // ToDo: Use only one snapshot
    // snapshot name is used to decide on prefit and postfit initial parameters
    wSpace->saveSnapshot("postfit_snapshot", *(pdf->getParameters(data)));
    wSpace->saveSnapshot("prefit_snapshot", *(pdf->getParameters(data)));

    // Fill Pull histogram
    //
    // Pull = (theta_hat - theta_zero)/delta_theta
    //
    while ((par = (RooRealVar*)next_nuis->Next())) {
	TString name = par->GetName();

	if ( name.Contains("Lumi") ) {
	    // Normalize Lumi pull correctly
	    Double_t lumi_err = fMeasurement->GetLumiRelError();

	    h_pull->Fill(par->GetName(), (par->getVal() - 1.)/lumi_err); // theta_0 = 1
	    if ( par->hasAsymError() ) { // Run Minos for this
		// Normalize the error +-1
		h_pull_err_up->Fill(par->GetName(), par->getErrorHi()/lumi_err);
		h_pull_err_down->Fill(par->GetName(), par->getErrorLo()/lumi_err);
	    } else if ( par->hasError() ) {
		h_pull_err_up->Fill(par->GetName(), par->getError()/lumi_err);
		h_pull_err_down->Fill(par->GetName(), par->getError()/lumi_err);
	    }
	} else if ( name.Contains("beta") ) {

	    TString *label = GetSystematicName(par->GetName());
	    if ( label == 0 ) label = new TString(par->GetName());
	    
	    // normalize norm uncertainties correctly
	    Double_t norm_err = GetNormUncertainty(name.Data());

	    h_pull->Fill(label->Data(), (par->getVal() - 1.)/norm_err); // theta_0 = 1
	    if ( par->hasAsymError() ) { // Run Minos for this
	     	// Normalize the error +-1
	     	h_pull_err_up->Fill(label->Data(), par->getErrorHi()/norm_err);
	     	h_pull_err_down->Fill(label->Data(), par->getErrorLo()/norm_err);
	    } else if ( par->hasError() ) {
	     	h_pull_err_up->Fill(label->Data(), par->getError()/norm_err);
	     	h_pull_err_down->Fill(label->Data(), par->getError()/norm_err);
	    }
	} else if ( name.Contains("gamma") ) {
	    Double_t err = GetGammaUncertainty(par->GetName(), wSpace);
	    
	    TString label = par->GetName();
	    label.ReplaceAll("_"," ");
	    label.ReplaceAll("gamma stat", "#gamma_{stat.}");
	    
	    h_pull->Fill(label.Data(), (par->getVal() - 1.)/err); // theta_0 = 1
	    if ( par->hasAsymError() ) { // Run Minos for this
	     	// Normalize the error +-1
	     	h_pull_err_up->Fill(label.Data(), par->getErrorHi()/err);
	     	h_pull_err_down->Fill(label.Data(), par->getErrorLo()/err);
	    } else if ( par->hasError() ) {
	     	h_pull_err_up->Fill(label.Data(), par->getError()/err);
	     	h_pull_err_down->Fill(label.Data(), par->getError()/err);
	    }
	} else {
	    // if norm uncertainties are gaussian, they are named alpha
	    // and then they behave as all other NPs

	    TString *label = GetSystematicName(par->GetName());
	    if ( label == 0 ) label = new TString(par->GetName());

	    h_pull->Fill(label->Data(), par->getVal());
	    if ( par->hasAsymError() ) { // Run Minos for this
		h_pull_err_up->Fill(label->Data(), par->getErrorHi());
		h_pull_err_down->Fill(label->Data(), par->getErrorLo());
	    } else if ( par->hasError() ) {
		h_pull_err_up->Fill(label->Data(), par->getError());
		h_pull_err_down->Fill(label->Data(), par->getError());
	    }
	}
    }
    next_nuis->Reset();
    // Deflate labels on x-axis (removes empty bins)
    h_pull->LabelsDeflate("X");
    h_pull_err_up->LabelsDeflate("X");
    h_pull_err_down->LabelsDeflate("X");
    
    // Pre-fit and post-fit impact on signal parameter
    // ===============================================
    // Pre-fit impact:
    // Loop over all nuisance parameters (NP), each time fix the NP
    // at +1 and -1 and redo the fit.
    //
    // Post-fit impact:
    // Loop over all NP, each time fix the NP at +1 sigma and -1 sigma
    // from the post-fit value and redo the fit.
    //
    // The impact on the parameter of interest (POI) is the difference between
    // the fit result for with the NP fixed and the POI pre-fit or post-fit
    // value.
    // 

    // Remove old logfile
    gSystem->Exec(Form("rm -f %s/pull_%s_%s.log",
		       pulldir->Data(),
		       wSpace->GetName(),
		       fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));

    TList *pois = fMeasurement->GetPOIs();
    TIter next_poi(pois);
    TObjString *obj_poi = 0;
    
    while ( (obj_poi = (TObjString*) next_poi()) ) {
	TString poi = obj_poi->GetString();

	TH1F *h_prefit_impact_up   = new TH1F(Form("%s_prefit_impact_up", poi.Data()),
					      Form("Prefit impact on '%s' (up)", poi.Data()),
					      1., 0., 1.);
	TH1F *h_prefit_impact_down = new TH1F(Form("%s_prefit_impact_down", poi.Data()),
					      Form("Prefit impact on '%s' (down)", poi.Data()),
					      1., 0., 1.);
	TH1F *h_postfit_impact_up   = new TH1F(Form("%s_postfit_impact_up", poi.Data()),
					      Form("Postfit impact on '%s' (up)", poi.Data()),
					      1., 0., 1.);
	TH1F *h_postfit_impact_down = new TH1F(Form("%s_postfit_impact_down", poi.Data()),
					      Form("Postfit impact on '%s' (down)", poi.Data()),
					      1., 0., 1.);
	
	while ((par = (RooRealVar*)next_nuis->Next())) {
	    
	    TString name = par->GetName();
	    
	    GetImpactOnPOI(wSpace, data_name, "prefit_snapshot",
			   par, poi.Data(), +1., h_prefit_impact_up,
			   pulldir->Data());
	    GetImpactOnPOI(wSpace, data_name, "prefit_snapshot",
			   par, poi.Data(), -1., h_prefit_impact_down,
			   pulldir->Data());
	    
	    GetImpactOnPOI(wSpace, data_name, "postfit_snapshot",
			   par, poi.Data(), +1., h_postfit_impact_up,
			   pulldir->Data());
	    GetImpactOnPOI(wSpace, data_name, "postfit_snapshot",
			   par, poi.Data(), -1., h_postfit_impact_down,
			   pulldir->Data());
	}
	next_nuis->Reset();
	
	// Adjust x-axis
	h_prefit_impact_up->LabelsDeflate("X");
	h_prefit_impact_down->LabelsDeflate("X");
	h_postfit_impact_up->LabelsDeflate("X");
	h_postfit_impact_down->LabelsDeflate("X");

	// Create pdf with all pulls and prefit / postfit impact on POI
	TString pullpdf = pullfile.Data();
	pullpdf.ReplaceAll(".root",
			   Form("_%s.pdf", poi.Data()));
	ExportPullPlots(pullpdf.Data(),
			h_pull, h_pull_err_up, h_pull_err_down,
			h_prefit_impact_up, h_prefit_impact_down,
			h_postfit_impact_up, h_postfit_impact_down);
    }
    fout->Write();
    
    // Close output file
    fout->Close();

    delete pulldir;
}

//____________________________________________________________________

void AtlHistFactoryTask::GetImpactOnPOI(RooWorkspace *wSpace,
					const char* data_name,
					const char* snapshot,
					RooRealVar *par,
					const char* poi_name,
					Double_t variation,
					TH1F *hresult,
					const char *logdir) {
    //
    // Get Impact on POI for given NP and store the result in the
    // given histogram.
    //

    // ToDo:
    // - Load snapshot beforehand and just provide pdf and parameter?
    //   maybe model?
    // - Asymmetric errors

    RooFitResult *fitResult  = 0;
    RooRealVar   *par_result = 0;

    TString name = par->GetName();

    // Load prefit snapshot
    TString Snapshot = snapshot;
    wSpace->loadSnapshot(snapshot);

    RooAbsData  *data  = wSpace->data(data_name);
    ModelConfig *model = (ModelConfig*) wSpace->obj("ModelConfig");
    RooAbsPdf   *pdf   = model->GetPdf();

    // Save fit result values for info statement below
    Double_t fit_val = 0.; // default values for gaussian NP (prefit)
    Double_t fit_err = 1.; // default values for gaussian NP (prefit)
    // Double_t fit_errHi = +1.;
    // Double_t fit_errLo = -1.;

    if ( Snapshot.Contains("prefit") ) {
	// Special treatment of lumi and normalization uncertainties
	// (if norms are lognormal)
	// Only needed for prefit values!
	// For postfit values, one can use the fit results directly
	
	if ( name.Contains("Lumi") ) {
	    // Option A:
	    // variation = 1. + variation*fMeasurement->GetLumiRelError();
	    // fit_val   = 1.;
	    // fit_err   = fMeasurement->GetLumiRelError();

	    // Option B:
	    variation = par->getVal() + variation*fMeasurement->GetLumiRelError();
	    fit_val   = par->getVal();
	    fit_err   = fMeasurement->GetLumiRelError();
	} else if ( name.Contains("beta_") ) {
	    // Norm parameters have log-normal constraints
	    // In that case they are called "beta_"
	    // Assume that no other NPs have log-normal constraints
	    //
	    // If the norm uncertainties have gaussian constraints
	    // they are called "alpha_" and can be treated in the same way
	    // as all other NPs

	    // Option A:
	    // Double_t norm_err = GetNormUncertainty(name.Data());
	    // variation = 1. + variation*norm_err;
	    
	    // fit_val   = 1.;
	    // fit_err   = norm_err;

	    // Option B:
	    Double_t norm_err = GetNormUncertainty(name.Data());
	    variation = par->getVal() + variation*norm_err;
	    
	    fit_val   = par->getVal();
	    fit_err   = norm_err;
	} else if ( name.Contains("gamma_") ) {
	    // Gamma parameters for statistical uncertainties for each bin

	    // Option A:
	    // Double_t err = GetGammaUncertainty(name.Data(), wSpace);
	    // variation = 1. + variation*err;
	    
	    // fit_val   = 1.;
	    // fit_err   = err;

	    // Option B:
	    Double_t err = GetGammaUncertainty(name.Data(), wSpace);
	    variation = par->getVal() + variation*err;
	    
	    fit_val   = par->getVal();
	    fit_err   = err;
	} else {
	    // Option A:
	    // variation = variation;
	    // fit_val = 0.;
	    // fit_err = 1.;

	    // Option B:
	    fit_err = variation;
	    
	    variation = par->getVal() + variation*1.;
	    fit_val = par->getVal();
	}	    
	
	par->setVal(variation);
	par->setConstant(true);
    } else {
	fit_val = par->getVal();
	fit_err = par->getError();

	par->setVal(par->getVal() + variation*par->getError());
	par->setConstant(true);
    }
    
    // Redirect output
    TString var = variation > 0 ? "up" : "down";
    gSystem->Exec(Form("rm -f %s/%s_%s_%s_%s.log",
		       logdir,
		       snapshot,
		       name.Data(),
		       var.Data(),
		       fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));
    gSystem->RedirectOutput(Form("%s/%s_%s_%s_%s.log",
				 logdir,
				 snapshot,
				 name.Data(),
				 var.Data(),
				 fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData"));
    
    // If you want the errors to reflect the information contained in the provided dataset, choose kTRUE. 
    // If you want the errors to reflect the precision you would be able to obtain with an unweighted dataset
    // with 'sum-of-weights' events, choose kFALSE.
    fitResult = pdf->fitTo(*data,
			   RooFit::SumW2Error(kFALSE),
			   RooFit::Offset(true),
			   RooFit::Extended(true),
			   RooFit::Save(true));

    fitResult->Print();
    
    par_result = (RooRealVar*) fitResult->floatParsFinal().find(poi_name); 

    // Store result with correct impact on mu
    RooRealVar *poi_init = (RooRealVar*) fitResult->floatParsInit().find(poi_name);

    Info("GetImpactOnPOI",
	 "NP: %s at %4.4f (%s)",
	 name.Data(), par->getVal(), snapshot);

    Info("GetImpactOnPOI", "mu_hat = %4.4f", poi_init->getVal());
    Info("GetImpactOnPOI", "Nuisance parameter %s, fit result: %4.4f +- %4.4f", name.Data(), fit_val, fit_err);
    Info("GetImpactOnPOI", "Result at %s = %3.3f -> mu = %4.4f +- %4.4f\n", name.Data(),
	 par->getVal(),
	 par_result->getVal(),
	 par_result->getError());

    gSystem->RedirectOutput(0);

    Info("GetImpactOnPOI",
	 "NP: %s at %4.4f (%s)",
	 name.Data(), par->getVal(), snapshot);

    Info("GetImpactOnPOI", "mu_hat = %4.4f", poi_init->getVal());
    Info("GetImpactOnPOI", "Nuisance parameter %s, fit result: %4.4f +- %4.4f", name.Data(), fit_val, fit_err);
    Info("GetImpactOnPOI", "Result at %s = %3.3f -> mu = %4.4f +- %4.4f\n", name.Data(),
	 par->getVal(),
	 par_result->getVal(),
	 par_result->getError());

    // Get x-axis label from list of systematic
    // ToDo: Is it needed? Is renaming bin labels for pull histogram enough?
    TString *label = GetSystematicName(par->GetName());
    if ( label == 0 ) label = new TString(par->GetName());
    Info("GetImpactOnPOI", "Label name: %s ", label->Data());
    hresult->Fill(par->GetName(), par_result->getVal() - poi_init->getVal());
    delete label;
}

//____________________________________________________________________

void AtlHistFactoryTask::GetTemplatesFileName(const char* channel,
					      TString &filename) {
    //
    // Get the name of the file holding all templates for this channel
    //
    
    filename = Form("%s/%s_templates.root",
		    fTemplateDir->Data(),
		    channel);
}

//____________________________________________________________________

void AtlHistFactoryTask::GetShapeTemplatesFileName(const char* channel,
						   TString &filename) {
    //
    // The name of the file holding all shape templates for this channel
    //
    
    filename = Form("%s/%s_shape_templates.root",
		    fTemplateDir->Data(),
		    channel);
}

//____________________________________________________________________

TList* AtlHistFactoryTask::MergeListOfSystematics(TList *ch_systs, TList *sample_systs) {
    //
    // Helper function to merge channel and sample systematics
    //
    // First create a new list from the ch_systs.
    // If two systematics with the same name exist in both lists:
    // - remove the systematic from the newly created list
    // - then append all systematics from the second list to the newly
    //   created list
    //
    // The user has to take care of deleting the new list.
    //
    
    // For each systematic:
    // Sample systematic config overwrites channel systematic config
    // -> Remove channel systematics with the same name
    // -> Then add all sample systematics

    TList *systematics = new TList;
    systematics->AddAll(ch_systs);
    
    TIter next_syst(sample_systs);
    AtlHistFactorySystematic *syst = 0;
    while ( (syst = (AtlHistFactorySystematic*) next_syst()) ) {
	TObject *obj = systematics->FindObject(syst->GetName());
	if ( obj != 0 )
	    systematics->Remove(obj);
    }
    systematics->AddAll(sample_systs);

    return systematics;
}

//____________________________________________________________________

void AtlHistFactoryTask::ExportRateUncertainties() {
    //
    // Export latex table with list of all rate uncertainties
    //

    TString *table_dir = GetDirectoryName("rate_tables");

    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	
	// Open template file (one per channel)
	TString filename;
	GetTemplatesFileName(ch->GetName(), filename);
	
	TFile *f_templates = TFile::Open(filename.Data());
	
	if ( f_templates == 0 ) {
	    Error("ExportRateUncertainties",
		  "Could not find template file. Abort!");
	}

	// Create Rate tab for this channel
	ofstream tab;
	tab.open(Form("%s/tab_rateuncert_%s.tex",
		      table_dir->Data(),
		      ch->GetName()));

	// Write tab body
	tab << "\\begin{tabular}{|r|";
	Int_t Nchans = ch->GetListOfSamples()->GetEntries();
	for ( Int_t i = 0; i < Nchans; i++ )
	    tab << "rr|";
	tab << "}" << endl
	      << "\\hline" << endl;

	// Write tab header
	// Header for processes
	tab << "Rate uncertainty [\\%] ";
	TIter next_proc(ch->GetListOfSamples());
	AtlHistFactorySample *proc = 0;
	while ( (proc = (AtlHistFactorySample*) next_proc()) ) {
	    // todo: replace this by proc->GetTitle()
	    tab << "& \\multicolumn{2}{c|}{" << proc->GetName() << "} ";
	}
	tab << "\\\\" << endl
	      << "\\hline" << endl;

	// Header for systematics
	tab << "Systematic ";
	for ( Int_t i = 0; i < Nchans; i++ )
	    tab << "& up & down ";
	tab << "\\\\" << endl
	      << "\\hline\\hline" << endl;

	
	// Loop over all systematics
	// ----------------------------------------------------------------
	// Add all systematics of all samples. Configuration
	// is not important, since only rates are computed,
	// we just need all systematics
	next_proc.Reset();
	TList *all_systs = new TList;
	all_systs->AddAll(ch->GetListOfSystematics());
	
	while ( (proc = (AtlHistFactorySample*) next_proc()) ) {
	    TList *old_systs = all_systs; // save pointer to list of systematics
	    all_systs = MergeListOfSystematics(all_systs,
					       proc->GetListOfSystematics());
	    delete old_systs;
	}	

	TList *ch_systs = ch->GetListOfSystematics();
	
	TIter next_syst(all_systs);
	AtlHistFactorySystematic *syst = 0;
	while ( (syst = (AtlHistFactorySystematic*) next_syst()) ) {

	    // skip nominal, no rate uncerainty...
	    if ( syst->IsNominal() ) continue;

	    // Set template file
	    syst->SetTemplateFile(f_templates);

	    // Is Sample Systematic?
	    // (-> it is not contained in the channel's list of systematics)
	    Bool_t IsSampleSyst = kFALSE;
	    TObject *obj = ch_systs->FindObject(syst->GetName());
	    if ( obj == 0 )
		IsSampleSyst = kTRUE;

	    // Write systematic title
	    tab << syst->GetTitle() << " ";
	    
	    // Loop over all processes (samples) in one channel
	    TIter next_sample(ch->GetListOfSamples());
	    AtlHistFactorySample *sample = 0;
	    while ( (sample = (AtlHistFactorySample*) next_sample()) ) {

		Double_t rate_up, rate_down;

		// If this is a sample systematic:
		// -> does it belong to this sample?
		if ( IsSampleSyst ) {
		    TList *sample_syst = sample->GetListOfSystematics();
		    obj = sample_syst->FindObject(syst->GetName());
		}

		// cout << "IsSampleSyst " << IsSampleSyst << endl;
		// cout << "Obj = " << obj << endl;
		// cout << "Syst: " << syst->GetName() << endl;
		
		if ( IsSampleSyst && obj == 0 ) {
		    // This systematic does not belong to this sample
		    // -> rate is zero
		    rate_up = 0;
		    rate_down = 0;
		} else {
		    // Load process histogram
		    syst->ChangeProcess(sample->GetName());
		    
		    // Compute rate uncertainty
		    syst->GetRateUnc(rate_up, rate_down);

		    // Clear systematic (close files, reset internal histograms, etc.)
		    syst->Clear();
		}
		
		// write rate uncertainty
		tab << "& " << Form("%3.1f", rate_up*100.) << " "
		      << "& " << Form("%3.1f", -rate_down*100.) << " ";
		
	    } // end of sample loop

	    // Add new line
	    tab << "\\\\" << endl;
	    
	} // end of sample loop

	f_templates->Close();

	// Write tab bottom
	tab << "\\hline" << endl
	      << "\\end{tabular}" << endl;
	tab.close();
    } // end of channel loop

    delete table_dir;
}

//____________________________________________________________________

Double_t AtlHistFactoryTask::GetNormUncertainty(const char* name) {
    //
    // Get the normalization uncertainty associated to this parameter
    //

    // Search for the normalization parameter in the first channel
    // of the measurement
    //
    // This does not work in special cases where the channels do have
    // different components. In that case, issue a error and abort
    // This needs to be implemented, e.g. looping over all channels

    TString par_name = name;

    // Get norm uncertainty from the first channel
    // Should be valid for all channels, no?
    TList *ch_list = fMeasurement->GetListOfChannels();
    AtlHistFactoryChannel *ch = (AtlHistFactoryChannel*) ch_list->At(0);
    
    TList *s_list = ch->GetListOfSamples();
    AtlHistFactorySample *sample = 0;
    TIter next_sample(s_list);
    while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
	TString norm_name = sample->GetNormFactorName();
	// this name is part of the parameter name
	if ( par_name.Contains(norm_name.Data()) )
	    return sample->GetNormUncertainty();
    }

    Error("GetNormUncertainty",
	  "Could not find a sample with norm parameter name %s",
	  name);
    gSystem->Abort();

    return -1; // suppress compiler warnings
}

//____________________________________________________________________

TString* AtlHistFactoryTask::GetSystematicName(const char* par_name) {
    //
    // Get the systematic name for the given parameter name
    //
    
    TString name = par_name;
    // Remove 'alpha_', 'beta_', 'gamma_' from parameter name
    if ( name.Contains("alpha_") ) name.ReplaceAll("alpha_", "");
    if ( name.Contains("beta_") ) name.ReplaceAll("beta_", "");
    if ( name.Contains("gamma_") ) name.ReplaceAll("gamma_", "");

    TIter next_ch(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ((ch = (AtlHistFactoryChannel*) next_ch())) {
	TList *syst = ch->GetListOfSystematics();
	TObject *obj = syst->FindObject(name.Data());
	if ( obj != 0 ) return (new TString(obj->GetTitle()));

	// If nothing was not found in the channel's list of systematics
	// Scan for sample systematics
	TIter next_sample(ch->GetListOfSamples());
	AtlHistFactorySample *sample = 0;
	while ((sample = (AtlHistFactorySample *) next_sample())) {
	    TList *s_syst = sample->GetListOfSystematics();
	    TObject *obj = s_syst->FindObject(name.Data());
	    if ( obj != 0 ) return (new TString(obj->GetTitle()));

	    TString s_name = sample->GetNormFactorName();
	    if ( s_name.Contains(name.Data()) )
		return (new TString(sample->GetTitle()));
	}
	next_sample.Reset();
    }
    return 0;
}

//____________________________________________________________________

void AtlHistFactoryTask::ExportPullPlots(const char* outfile,
					 TH1* h_pull,
					 TH1* h_pull_err_up,
					 TH1* h_pull_err_down,
					 TH1* h_prefit_impact_up,
					 TH1* h_prefit_impact_down,
					 TH1* h_postfit_impact_up,
					 TH1* h_postfit_impact_down) {
    //
    // Export pull plots to pdf
    //

    // Compute number of Pads needed for all pulls
    Int_t Nmax_pulls = h_pull->GetNbinsX();
    Int_t Npads = TMath::Ceil(Nmax_pulls/((Double_t)fNPullsPerPad));

    // Sort all pulls by post fit impact on mu
    // ---------------------------------------
    // Sort such that the pull with the highest postfit
    // impact (sum of postfit impact up + down) is at the
    // top of the graph
    
    TArrayI *index = new TArrayI(Nmax_pulls);
    Double_t Postfit_impact[300]; // max. number of supported pulls

    Double_t i_up;
    Double_t i_down;
    for ( Int_t k = 0; k < 300; k++ ) {
	// Array starts at 0, histograms at 1
	if ( k < Nmax_pulls ) {
	    i_up   = TMath::Abs(h_postfit_impact_up->GetBinContent(k+1));
	    i_down = TMath::Abs(h_postfit_impact_down->GetBinContent(k+1));
	    Postfit_impact[k] = i_up + i_down;
	} else {
	    Postfit_impact[k] = -1.; // minimum impact is 0.
	}
    }
    TMath::Sort(Nmax_pulls,
    		Postfit_impact,
    		index->GetArray(), kTRUE);
    // From now on, use the index array to access the pull info
    // Keep in mind that the number of the pull stored in the
    // index array is off by 1 (because the array starts at 0
    // and histogram bin numbers start at 1)
        
    for ( Int_t j = 0; j < Npads; j++ ) {

	// Compute the number of pulls for the current pad
	// as well as the first and last pull that should be drawn
	Int_t n_pulls    = fNPullsPerPad;
	Int_t pull_start = j*fNPullsPerPad;
	Int_t pull_end   = (j+1)*fNPullsPerPad;

	// In case less pulls than fNPullsPerPad are left
	// adjust the end point accordingly
	if ( pull_end > Nmax_pulls ) {
	    pull_end = Nmax_pulls;
	    n_pulls  = Nmax_pulls % fNPullsPerPad;
	}

	// Create canvas for pulls
	TCanvas *c = new TCanvas(Form("Pulls_%d_to_%d", pull_start, pull_end),
				 Form("Pulls_%d_to_%d", pull_start, pull_end),
				 500., 700.);
	c->SetLeftMargin(0.35);
	c->SetTopMargin(0.1);
	c->SetBottomMargin(0.15);
	
	// Collect all graphs in one multigraph
	TMultiGraph *mg = new TMultiGraph();

	TGraphAsymmErrors *g = new TGraphAsymmErrors(n_pulls); // pulls
	TGraphAsymmErrors *n = new TGraphAsymmErrors(n_pulls); // prefit impact
	TGraphAsymmErrors *u = new TGraphAsymmErrors(n_pulls); // postfit impact
	
	// Two graphs for background lines with different colors
	TGraphErrors *bkg1 = new TGraphErrors(n_pulls);
	TGraphErrors *bkg2 = new TGraphErrors(n_pulls);

	// Add some extra space for the legend
	TGraphErrors *bkg3 = new TGraphErrors(3);

	// Add all graphs and adjust drawing style
	mg->Add(bkg1, "E2");
	mg->Add(bkg2, "E20");
	mg->Add(bkg3, "E20");
	mg->Add(n, "E2");
	mg->Add(u, "E2");
	mg->Add(g, "PE");

	
	// Set Points and Errors
	// ---------------------

	// Counter for the y-axis of the pull graph
	// Historically, I increased this counter by +1 after adding a point to the
	// pull graph. After adding the sorting of the pulls, the easiest solution
	// was to have a graph with negative numbers from 0 to -fNPullsPerPad
	// such that the pull with the highest impact on poi is at the top
	Int_t i_pull = 0;

	// Scaling of the impact on poi errors
	// All graphs of the multigraph have to use the same x-axis
	// Since the impact on the poi will have a different x-axis range then the one
	// of the pulls, scale everything by a scale factor
	// The scale factor also defines then the new x-axis range which is drawn on top
	// of the plot	
	// N.B.: Another solution would be to have two multigraphs, but since the Multigraph
	//       does not support the drawing option X+, the scaling of ther errors was easier
	//       to implement
	
	for ( Int_t i = pull_start; i < pull_end; i++ ) {
	    // Remember:
	    // Histogram bin numbers start with 1
	    // Graph point numbers start with 0

	    // Pulls
	    g->SetPoint(i_pull, h_pull->GetBinContent(index->At(i)+1), -(i_pull+1));
	    
	    g->SetPointEYhigh(i_pull, 0.);
	    g->SetPointEYlow(i_pull, 0.);
	    
	    g->SetPointEXhigh(i_pull, h_pull_err_up->GetBinContent(index->At(i)+1));
	    g->SetPointEXlow(i_pull, h_pull_err_down->GetBinContent(index->At(i)+1));

	    // Prefit impact on mu
	    n->SetPoint(i_pull, 0., -(i_pull+1));
	    
	    n->SetPointEYhigh(i_pull, 0.4);
	    n->SetPointEYlow(i_pull, 0.4);
	    
	    i_up   = TMath::Abs(h_prefit_impact_up->GetBinContent(index->At(i)+1));
	    i_down = TMath::Abs(h_prefit_impact_down->GetBinContent(index->At(i)+1));
	    
	    n->SetPointEXhigh(i_pull, i_up*fPullScaleFactor);
	    n->SetPointEXlow(i_pull,  i_down*fPullScaleFactor);
	    
	    // Postfit impact on mu
	    u->SetPoint(i_pull, 0., -(i_pull+1));
	    
	    u->SetPointEYhigh(i_pull, 0.35);
	    u->SetPointEYlow(i_pull, 0.35);
	    
	    i_up   = TMath::Abs(h_postfit_impact_up->GetBinContent(index->At(i)+1));
	    i_down = TMath::Abs(h_postfit_impact_down->GetBinContent(index->At(i)+1));

	    u->SetPointEXhigh(i_pull, i_up*fPullScaleFactor);
	    u->SetPointEXlow(i_pull,  i_down*fPullScaleFactor);
	    
	    // Iterating colors for backgrounds
	    bkg1->SetPoint(i_pull, 0., -(i_pull+1));
	    bkg2->SetPoint(i_pull, 0., -(i_pull+1));
	    if ( i_pull % 2 == 0 ) {
		bkg1->SetPointError(i_pull, 0., 0.);
		bkg2->SetPointError(i_pull, 3., 0.5);
	    } else {
	    	bkg1->SetPointError(i_pull, 3., 0.5);
	    	bkg2->SetPointError(i_pull, 0., 0.);
	    }
	
	    // next pull
	    i_pull++;
	}

	// Add some space for legend at the top of the plot
	bkg3->SetPoint(1., 0., +1.);
	bkg3->SetPoint(2., 0., +2.);
	bkg3->SetPoint(3., 0., +3.);
	bkg3->SetPointError(1., 3., 0.5);
	bkg3->SetPointError(2., 3., 0.5);
	bkg3->SetPointError(3., 3., 0.5);
	bkg3->SetFillColor(0);
	
	g->SetLineWidth(2);
	
	n->SetFillStyle(1001);
	n->SetFillColor(kYellow);
	n->SetLineColor(kYellow);
	
	u->SetFillStyle(3354);
	u->SetFillColor(kBlue);
	u->SetLineColor(kBlue);

	bkg1->SetFillStyle(1001);
	bkg1->SetFillColor(kWhite);

	bkg2->SetFillStyle(3001);
	bkg2->SetFillColor(kGray);
		
	// Axis range
	mg->Draw("A");
	mg->GetXaxis()->SetLimits(-3., 3.);
	mg->GetXaxis()->SetTitle("(#hat{#theta} - #theta_{0})/#Delta#theta");
	mg->GetXaxis()->SetTitleOffset(1.);
	mg->GetYaxis()->SetTickLength(0.);
	mg->GetYaxis()->SetLabelSize(0.);

	Double_t xmin = mg->GetXaxis()->GetXmin();
	Double_t xmax = mg->GetXaxis()->GetXmax();
	Double_t ymax = mg->GetYaxis()->GetXmax();
	
	TGaxis *x2 = new TGaxis(xmin, ymax, // Position
				xmax, ymax, //
				xmin/fPullScaleFactor, xmax/fPullScaleFactor, // x-axis Range
				505, "-"); // ndivisions
	x2->ImportAxisAttributes(g->GetXaxis());
	x2->SetTitle("#Delta#mu");
	x2->SetTitleOffset(.8);
	x2->Draw();

	// Add lines for +1, 0., -1 (for the pulls)
	TLine *plus1  = new TLine(+1., 0., +1., -(n_pulls+1.));
	plus1->SetLineStyle(2);
	plus1->Draw();

	TLine *minus1 = new TLine(-1., 0., -1., -(n_pulls+1.));
	minus1->SetLineStyle(2);
	minus1->Draw();

	TLine *zero   = new TLine( 0., 0., 0., -(n_pulls+1.));
	zero->SetLineStyle(2);
	zero->Draw();

	TLegend *leg = new TLegend(0.35, 0.745, 0.8, 0.89);
	leg->AddEntry(g, "Pull", "PL");
	leg->AddEntry(n, "Prefit impact", "F");
	leg->AddEntry(u, "Postfit impact", "F");
	leg->SetFillColor(0);
	leg->SetFillStyle(0);
	leg->SetLineColor(0);
	leg->SetBorderSize(0);
	leg->Draw();
	
	// Atlas Label
	TLatex t1;
	t1.SetTextFont(72);
	t1.SetTextColor(kBlack);
	t1.SetTextAlign(31);
	t1.DrawLatexNDC(0.78, 0.85, "ATLAS");
	
	// Internal label
	TLatex t2; 
	t2.SetTextFont(42);
	t2.SetTextColor(kBlack);
	t2.SetTextAlign(11);
	t2.DrawLatexNDC(0.79, 0.85, "Internal");
	
	// Reset the counter for the pulls of this pad
	i_pull = 0;
	// 'i' points to the pulls used for this pad
	// (the pulls need to be accessed through the index array)
	for ( Int_t i = pull_start; i < pull_end; i++ ) {

	    // Add NP label for each pull
	    TLatex *l = new TLatex;
	    l->SetTextFont(42);
	    l->SetTextSize(0.03);
	    l->SetTextAlign(32);
	    
	    TAxis *hx = h_pull->GetXaxis();
	    l->DrawLatex(-3.1, -(i_pull + 1), hx->GetBinLabel(index->At(i)+1));

	    i_pull++;
	}

	// no ticks on the upper x-axis for the Multigraph (!)
	c->SetTicks(0, 0);
	// Redraw the axis to be on top of everything else that was added
	c->RedrawAxis();
	// Update the canvas
	c->Modified();
	c->Update();

	// Save all pulls in one pdf
	// (Check special case if there is only one pad)
	if ( j == 0 && Npads != 1 ) 	// First pad
	    c->SaveAs(Form("%s(", outfile));
	else if ( j+1 == Npads && Npads != 1 ) // last pad
	    c->SaveAs(Form("%s)", outfile));
	else
	    c->SaveAs(Form("%s", outfile));
    }
}

//____________________________________________________________________

void AtlHistFactoryTask::RunLimit() {
    //
    // Run Hypothesis Test:
    //  - Signal Hypothesis vs. Background only Hypothesis
    //
    // Modified version of $ROOTSYS/tutorials/roostats/StandardHypoTestDemo.C
    //

}

//____________________________________________________________________

Double_t AtlHistFactoryTask::GetGammaUncertainty(const char* name,
						 RooWorkspace *wSpace) {
    //
    // Get the uncertainty associated to this parameter
    //

    // Assuming gamma has a gaussian constraint
    
    // That's a nasty way of getting the sigma parameter
    // of the gamma parameters, is there any other way?
    
    TString pdfname = name;
    pdfname.Append("_constraint");
    RooAbsPdf *pdf = wSpace->pdf(pdfname.Data());
    
    TString sigma_name = name;
    sigma_name.Append("_sigma");
    RooConstVar *sigma = (RooConstVar*) pdf->findServer(sigma_name.Data());
    if ( sigma == 0 ) {
	Error("GetGammaUncertainty",
	      "Could not find parameter %s. Abort.",
	      name);
	gSystem->Abort();
    }
    
    return sigma->getVal();
}

//____________________________________________________________________

TString* AtlHistFactoryTask::GetDirectoryName(const char* name) {
    //
    // Return the name to a directory within the common output
    // directory.
    // If a directory with this name does not yet exist, it will
    // be created.
    //
    // The user is responsible for deleting the returned pointer.
    //
    
    TString *dir = new TString(gSystem->DirName(fOutputFileName->Data()));
    dir->Append(Form("/%s/", name));
    dir->ReplaceAll("//","/");
    
    // Create template directory (if it does not yet exist)
    Int_t val = gSystem->Exec(Form("test -d %s", dir->Data()));
    if ( val != 0 ) {
	val = gSystem->Exec(Form("mkdir -p %s", dir->Data()));
	if ( val == 0 ) {
	    Info("GetDirectoryName",
		 "mkdir -p %s", dir->Data());
	} else {
	    Error("GetDirectoryName",
		  "Creation of directory '%s' not possible. Abort!",
		  dir->Data());
	    gSystem->Abort();
	}
    }
    return dir;
}

//____________________________________________________________________

Bool_t AtlHistFactoryTask::IsNormParameter(const char* name) {
    //
    // Is norm parameter
    //

    TString par_name = name;
    // Loop over all samples

    // Loop over all channels
    TIter next_channel(fMeasurement->GetListOfChannels());
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	// Loop over all processes (samples) in one channel
	TIter next_sample(ch->GetListOfSamples());
	AtlHistFactorySample *sample = 0;
	while ( (sample = (AtlHistFactorySample*) next_sample()) ) {
	    if ( par_name.Contains(sample->GetNormFactorName()) )
		return kTRUE;
	}
    }
    return kFALSE;
}
