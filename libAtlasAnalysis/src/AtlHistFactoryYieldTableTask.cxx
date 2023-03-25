//____________________________________________________________________
//
// DOCUMENT ME
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryYieldTableTask
#include <AtlHistFactoryYieldTableTask.h>
#endif
#include <AtlHistFactoryMeasurement.h>
#include <AtlHistFactoryChannel.h>
#include <AtlHistFactorySample.h>
#include <RooAbsData.h>
#include <RooCategory.h>
#include <RooCatType.h>
#include <RooFitResult.h>
#include <RooProduct.h>
#include <RooAbsRealLValue.h>
#include <RooRealSumPdf.h>
#include <RooRealVar.h>
#include <RooSimultaneous.h>
#include <RooWorkspace.h>
#include <TMath.h>
#include <TROOT.h>
#include <TSystem.h>

using namespace std;
using namespace RooStats;
using namespace HistFactory;
using namespace RooFit;

#ifndef __CINT__
ClassImp(AtlHistFactoryYieldTableTask);
#endif

//____________________________________________________________________

AtlHistFactoryYieldTableTask::AtlHistFactoryYieldTableTask(const char* name,
							   const char* title)
    : AtlHistFactoryTask(name, title) {
    //
    // Default constructor
    //
    SetInteractiveJob(kTRUE); // these tasks are always interactive
}

//____________________________________________________________________

AtlHistFactoryYieldTableTask::~AtlHistFactoryYieldTableTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlHistFactoryYieldTableTask::Initialize() {
    //
    // Default destructor
    //
    
    AtlHistFactoryTask::Initialize();
    
    // get combined model
    TString *filename = fMeasurement->GetModelFileName();
    
    fModelFile = new TFile(filename->Data());
    if ( fModelFile->IsZombie() ) {
	Info("Initialize",
	     "No model file with name '%s' found. Set Task inactive.",
	     filename->Data());
	Info("Initialize",
	     "Create a model first, then recreate this task.");
	SetActive(kFALSE);
    } else {
	Info("Initialize",
	     "Found model input file '%s.",
	     filename->Data());
    }

    delete filename;
}

//____________________________________________________________________

void AtlHistFactoryYieldTableTask::ExecInteractiveJob(Option_t* option) {
    //
    // Exec interactive job
    //

    // Convert option string
    TString opt = option;
    opt.ToLower();
    
    PrintYieldTable();
}

//____________________________________________________________________

void AtlHistFactoryYieldTableTask::PrintYieldTable() {
    //
    // Print Yield table
    //
    
    TString *table_dir = GetDirectoryName("yields");

    // Get workspace, model and data
    RooWorkspace  *w = (RooWorkspace*) fModelFile->Get("combined");
    ModelConfig   *m = (ModelConfig*) w->obj("ModelConfig");
    const char* data_name = fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData";
    RooAbsData *data = w->data(data_name);

    // Get the combined pdf of all channels
    RooSimultaneous *simpdf = (RooSimultaneous*) m->GetPdf(); 

    RooFitResult *fitResult = simpdf->fitTo(*data,
					    RooFit::SumW2Error(kFALSE),
					    RooFit::Offset(true),
					    RooFit::Extended(true), // overall number of events is fit parameter
					    RooFit::Save(true));    // a non-zero fit result pointer is returned
    fitResult->Print();

    // Get the list of channels
    RooCategory* channels = (RooCategory*) (&simpdf->indexCat());

    // Loop over all channels
    TIterator* iter = channels->typeIterator();
    RooCatType* chan;

    while((chan = (RooCatType*) iter->Next())) {

	// Create yield table for this channel
	ofstream tab;
	tab.open(Form("%s/yields_%s_%s.tex",
		      table_dir->Data(),
		      chan->GetName(),
		      data_name));
	
	// Table header
	tab << "\\begin{tabular}{l|c|r@{\\,}@{$\\pm$}@{\\,}l|c}" << endl
	    << "\\hline" << endl
	    << "Process & Prefit & \\multicolumn{2}{|c|}{Postfit} & Postfit / Prefit \\\\" << endl
	    << "\\hline" << endl;
	
	// Pdf for this channel
	RooAbsPdf* chan_pdf = (RooAbsPdf*) simpdf->getPdf(chan->GetName());
	
	// Need to reduce the data for each channel
	RooAbsData* chan_data = (RooAbsData*) data->reduce(Form("%s==%s::%s",
								channels->GetName(),
								channels->GetName(),
								chan->GetName()));
	
	// Observable for this channel
	RooArgSet *chan_obs_set = chan_pdf->getObservables(*m->GetObservables());
	RooRealVar *chan_obs    = (RooRealVar*)chan_obs_set->first();
	
	// Get 'RooRealSumPdf' for this channel
	// - this is unfortunatly hardcoded
	// - assume that this is valid for all HistFactory models...
	RooRealSumPdf *chan_sumpdf = (RooRealSumPdf*) chan_pdf->getComponents()->find(Form("%s_model",
											   chan->GetName()));

	// Yields for individual samples:
	// ------------------------------
	// The 'RooRealSumPdf' has a list of coef. and pdfs for each sample
	// The sum of Sum [coef * pdf] is the channel pdf
	// The combination of [coef_i * pdf_i] is the pdf for sample i
	//
	RooArgList sample_func = chan_sumpdf->funcList();
	RooArgList sample_coef = chan_sumpdf->coefList();
	
	// Loop over all samples
	TIterator *s_iter = sample_func.createIterator(kIterForward);
	RooProduct *sample = 0;

	// List of RooRealSumPdfs
	TList *single_samples = new TList;

	// Syncronize loop with channel / sample in fMeasurement
	AtlHistFactoryChannel *atl_chan =
	    (AtlHistFactoryChannel*) fMeasurement->GetListOfChannels()->At(chan->getVal());
	TIter next_atlsample(atl_chan->GetListOfSamples());
	AtlHistFactorySample *atl_sample = 0;

	Double_t TotalExp_prefit = 0.;
	Double_t TotalExp_postfit = 0.;
	Double_t TotalExp_postfit_err = 0.;
	
	// Create sum pdf used for all bkg processes
	RooArgList bkgsum_f;
	RooArgList bkgsum_c;

	while ( (sample = (RooProduct*) s_iter->Next()) &&
		(atl_sample = (AtlHistFactorySample*) next_atlsample()) ) {
	    
	    RooArgList sample_f;
	    RooArgList sample_c;
	    
	    sample_f.add(*sample);
	    sample_c.add(*sample_coef.at(sample_func.index(sample->GetName())));

	    // Skip signal sample
	    if ( !atl_sample->IsSignal() ) {
		bkgsum_f.add(*sample);
		bkgsum_c.add(*sample_coef.at(sample_func.index(sample->GetName())));
	    }

	    RooRealSumPdf *single_sample = new RooRealSumPdf(Form("%s_postfit_pdf", atl_sample->GetName()),
							     Form("%s_postfit_pdf", atl_sample->GetName()),
							     sample_f, sample_c);

	    const char *hist_name = atl_sample->GetName();
	    TH1 *h = (TH1*) fModelFile->Get(Form("%s_hists/%s/%s_nominal",
						 chan->GetName(),
						 hist_name, hist_name));
	    // Prefit yield
	    // (Prefit error is misleading, since it only includes the 
	    //  MC statistics and does not include the norm. uncertainty 
	    //  or any rate uncertainties. Thus, it will be smaller than
	    //  the postfit uncertainty)
	    Double_t prefit_int = h->Integral();
	    TotalExp_prefit += prefit_int;
	    
	    // Normalization of this pdf
	    RooAbsReal *sample_var = (RooAbsReal*) single_sample->createIntegral(RooArgSet(*chan_obs));
	    Double_t sample_evts = sample_var->getVal();
	    Double_t sample_err  = sample_var->getPropagatedError(*fitResult);

	    // Print table header
	    TString title = atl_sample->GetTitle();
	    title.ReplaceAll("Norm. ","");
	    // convert from root-latex to latex
	    title.ReplaceAll("#it","\\textit");
	    title.ReplaceAll("t#bar{t}", "$\\text{t}\\bar{\\text{t}}$");
	    
	    tab << title.Data()
		<< " & " << prefit_int 
		<< " & " << sample_evts 
		<< " & " << sample_err
		<< " & " << sample_evts/prefit_int
		<< " \\\\" << endl;
	    
	    single_samples->Add(single_sample);
	}

	// Bkg sum
	RooRealSumPdf *bkg_sum = new RooRealSumPdf("sumpdf_bkg",
						   "sumpdf_bkg",
						   bkgsum_f, bkgsum_c);
	RooAbsReal *bkgsum_var = (RooAbsReal*) bkg_sum->createIntegral(RooArgSet(*chan_obs));
	Double_t bkgsum_evts = bkgsum_var->getVal();
	Double_t bkgsum_err  = bkgsum_var->getPropagatedError(*fitResult);

	// Total Bkg Sum
	tab << "\\hline" << endl
	    << "\\hline" << endl
	    << "Total exp." 
	    << " & " << 0 // ToDo
	    << " & " << bkgsum_evts
	    << " & " << bkgsum_err
	    << " & " << 0 // ToDo
	    << "\\\\" << endl;

	// Total expectation
	RooAbsReal *Postfit_sum = (RooAbsReal*) chan_sumpdf->createIntegral(RooArgSet(*chan_obs));
	TotalExp_postfit = Postfit_sum->getVal();
	TotalExp_postfit_err = Postfit_sum->getPropagatedError(*fitResult);

	// Total Exp and Data
	tab << "\\hline" << endl
	    << "\\hline" << endl
	    << "Total exp." 
	    << " & " << TotalExp_prefit
	    << " & " << TotalExp_postfit
	    << " & " << TotalExp_postfit_err
	    << " & " << TotalExp_postfit/TotalExp_prefit
	    << "\\\\" << endl;
	tab << "Data " 
	    << " & " << chan_data->sumEntries()
	    << " & \\multicolumn{1}{r}{" << chan_data->sumEntries() << "\\,}"
	    << " & " // no error for data
	    << " & " // no postfit/prefit for data
	    << " \\\\" << endl;	    

	// Table footer
	tab << "\\hline" << endl
	    << "\\end{tabular}" << endl;
	tab.close();
    }
    
    delete table_dir;
}
