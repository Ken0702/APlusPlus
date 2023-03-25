//____________________________________________________________________
//
// DOCUMENT ME
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryPlotterTask
#include <AtlHistFactoryPlotterTask.h>
#endif
#include <AtlHistFactoryMeasurement.h>
#include <AtlHistFactoryChannel.h>
#include <AtlHistFactorySample.h>
#include <RooAbsData.h>
#include <RooCategory.h>
#include <RooCatType.h>
#include <RooConstVar.h>
#include <RooCurve.h>
#include <RooFitResult.h>
#include <RooHist.h>
#include <RooPlot.h>
#include <RooProduct.h>
#include <RooAbsRealLValue.h>
#include <RooRealSumPdf.h>
#include <RooRealVar.h>
#include <RooSimultaneous.h>
#include <RooWorkspace.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGaxis.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TPad.h>
#include <TROOT.h>
#include <TSystem.h>

using namespace std;
using namespace RooStats;
using namespace HistFactory;
using namespace RooFit;

#ifndef __CINT__
ClassImp(AtlHistFactoryPlotterTask);
#endif

//____________________________________________________________________

AtlHistFactoryPlotterTask::AtlHistFactoryPlotterTask(const char* name,
						     const char* title) :
    AtlHistFactoryTask(name, title) {
    //
    // Default constructor
    //

    SetInteractiveJob(kTRUE); // these tasks are always interactive
    fRatioHeight = 0.4;

    // ToDo: Configurable positions, etc.
    // values taken from HepDataMCPlot
    //
    fCMSEnergyLabel = new TLatex(0.2, 0.77, "#sqrt{s} = 8 TeV");
    fCMSEnergyLabel->SetNDC();

    fLumiLabel = new TLatex(0.2, 0.85, "#int Ldt = 20.3 fb^{-1}");
    fLumiLabel->SetNDC();

    fRemoveDataErrorX = kTRUE;
}

//____________________________________________________________________

AtlHistFactoryPlotterTask::~AtlHistFactoryPlotterTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlHistFactoryPlotterTask::Initialize() {
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

void AtlHistFactoryPlotterTask::ExecInteractiveJob(Option_t* option) {
    //
    // Exec interactive job
    //

    // Convert option string
    TString opt = option;
    opt.ToLower();
    
    CreatePlot();
}

//____________________________________________________________________
void AtlHistFactoryPlotterTask::CreatePlot() {
    //
    // Create Plot
    //

    // Turn off all info messages
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
    
    // Get workspace, model and data
    RooWorkspace  *w = (RooWorkspace*) fModelFile->Get("combined");
    ModelConfig   *m = (ModelConfig*) w->obj("ModelConfig");
    RooAbsData *data = w->data(fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData");

    // Get the combined pdf of all channels
    RooAbsPdf *pdf = m->GetPdf(); 
    RooSimultaneous *simpdf = (RooSimultaneous*) m->GetPdf(); 

    RooFitResult *fitResult = simpdf->fitTo(*data,
					    RooFit::SumW2Error(kFALSE),
					    RooFit::Offset(true),
					    RooFit::Extended(true), // overall number of events is fit parameter
					    RooFit::Save(true));    // a non-zero fit result pointer is returned
    
    fitResult->Print();

    /*
    // TFile *f = new TFile("/rdsk/dats2/atlas/stamm/HistFactory/postfitplots/WCR_mtw_nosysts/fit/obsData_fitresult.root");
    // RooFitResult *fitResult_mtw = (RooFitResult*) f->Get("fitresult_simPdf_obsData");
    RooFitResult *fitResult_mtw = 0;
    
    // exchange parameter values like this?
    RooArgSet *nuis = (RooArgSet*)m->GetNuisanceParameters();
    TIterator *next_nuis = nuis->createIterator();
    RooRealVar* parmodel = 0;

    while ((parmodel = (RooRealVar*)next_nuis->Next())) {
	if ( fitResult_mtw == 0 ) break;
	break;

	// original fit
	RooRealVar* parfit = (RooRealVar*) fitResult->floatParsFinal().find(parmodel->GetName());

	TString name = parmodel->GetName();
	if ( name.Contains("gamma_stat") ) {
	    // reset gamma stat parameters since no information from fit is available
	    // if we copy values from the mtw fit to a different distribution

	    Double_t err = GetGammaUncertainty(name.Data(), w);
	    Double_t fit_val   = 1.;
	    Double_t fit_err   = err;

	    cout << Form("%s: %3.3f +- %3.3f",
	    		 name.Data(),
	    		 fit_val,
	    		 fit_err) << endl;

	    parmodel->setVal(fit_val); // reset
	    parmodel->setError(fit_err); // reset

	} else {
	    RooRealVar* parfit_mtw =  (RooRealVar*) fitResult_mtw->floatParsFinal().find(parmodel->GetName());
	    
	    // Set model parameters
	    // cout << name.Data() << endl;
	    parmodel->setVal(parfit_mtw->getVal());
	    parmodel->setError(parfit_mtw->getError());
	    
	    // manipulate fit result
	    parfit->setVal(parfit_mtw->getVal());
	    parfit->setError(parfit_mtw->getError());

	}
    }
    */
    // Get the list of channels
    RooCategory* channels = (RooCategory*) (&simpdf->indexCat());

    // Loop over all channels
    TIterator* iter = channels->typeIterator();
    RooCatType* chan;

    TString *plot_dir = GetDirectoryName("plot");
    TFile *fout = new TFile(Form("%s/Postfit_plots.root", plot_dir->Data()), "RECREATE");

    // TCanvas needed for legend
    TCanvas *c = new TCanvas;
    
    while((chan = (RooCatType*) iter->Next())) {

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
	// - this has to be changed to ..._model_newSimPdf (if lognormals are used)
	RooRealSumPdf *chan_sumpdf = (RooRealSumPdf*) chan_pdf->getComponents()->find(Form("%s_model",
											   chan->GetName()));
	
	// Plot individual samples:
	// ------------------------
	// The 'RooRealSumPdf' has a list of coef. and pdfs for each sample
	// The sum of Sum [coef * pdf] is the channel pdf
	// The combination of [coef_i * pdf_i] is the pdf for sample i
	//
	RooArgList sample_func = chan_sumpdf->funcList();
	RooArgList sample_coef = chan_sumpdf->coefList();
	
	// Loop over all samples
	TIterator *s_iter = sample_func.createIterator(kIterBackward);
	RooProduct *sample = 0;

	// Create sum pdf used for stack plot
	RooArgList stack_f;
	RooArgList stack_c;
	
	// List of RooRealSumPdfs
	TList *stack = new TList;
	TList *single_samples = new TList;

	Int_t i = 0;
	while ((sample = (RooProduct*) s_iter->Next())) {

	    // Add each sample to the stack
	    // - the pdf that needs to be added is the sum
	    //   of all sample pdfs in this loop
	    // - the last pdf is equal to the sum of all pdfs
	    stack_f.add(*sample);
	    stack_c.add(*sample_coef.at(sample_func.index(sample->GetName())));

	    RooArgList sample_f;
	    RooArgList sample_c;
	    
	    sample_f.add(*sample);
	    sample_c.add(*sample_coef.at(sample_func.index(sample->GetName())));

	    RooRealSumPdf *stack_sum = new RooRealSumPdf(Form("sumpdf_%d", i),
							 Form("sumpdf_%d", i),
							 stack_f, stack_c);
	    RooRealSumPdf *single_sample = new RooRealSumPdf(Form("single_sample_pdf_%d", i),
							     Form("single_sample_pdf_%d", i),
							     sample_f, sample_c);
	    i++; // just a number for naming each sumpdf differently
	    
	    stack->Add(stack_sum);
	    single_samples->Add(single_sample);
	}
	
	// Create stack plot
	// -----------------
	RooPlot *plot = chan_obs->frame();
	plot->SetNameTitle(Form("Postfit_stack_%s", chan->GetName()),
			   Form("Postfit_stack_%s", chan->GetName()));
	
	// Draw now all RooRealSumPdfs in reverse order with the correct normalization
	TIter next_sample(stack, kIterBackward);
	RooRealSumPdf *stack_sample = 0;

	// Syncronize loop with channel / sample in fMeasurement
	AtlHistFactoryChannel *atl_chan =
	    (AtlHistFactoryChannel*) fMeasurement->GetListOfChannels()->At(chan->getVal());
	// we need to forward iterate through this one, since the pdf with
	// most entries will be drawn first
	TIter next_atlsample(atl_chan->GetListOfSamples());
	AtlHistFactorySample *atl_sample = 0;

	// Create Legend
	TLegend *legend = new TLegend(0.59, 0.5, 0.90, 0.86);
	legend->SetName(Form("Legend_%s", chan->GetName()));
	legend->SetFillColor(0);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	// add data
	TLegendEntry *entry = legend->AddEntry("",
					       fMeasurement->GetUseAsimovData() ? "Asimov Data" : "Data",
					       "p");
	entry->SetMarkerColor(kBlack);
	entry->SetMarkerStyle(20);

	while ( (stack_sample = (RooRealSumPdf*) next_sample()) ) {
	    // Syncronize loops
	    atl_sample = (AtlHistFactorySample*) next_atlsample();
	    if ( atl_sample == 0) {
		Error("CreatePlot", "Loops out of sync. Check your configuration, workspace, etc. Abort!");
		gSystem->Abort();
	    }
	    
	    // Normalization of this pdf
	    Double_t sample_evts = ((RooAbsReal*) stack_sample->createIntegral(RooArgSet(*chan_obs)))->getVal();
	    
	    stack_sample->plotOn(plot,
	     			 Normalization(sample_evts,RooAbsReal::NumEvent),
				 LineColor(atl_sample->GetLineColor()),
				 LineStyle(atl_sample->GetLineStyle()),
				 FillColor(atl_sample->GetFillColor()),
				 FillStyle(atl_sample->GetFillStyle()),
				 DrawOption("F"));
	    // Add corresponding legend entry
	    TString title = atl_sample->GetTitle();
	    title.ReplaceAll("Norm. ","");
	    entry = legend->AddEntry(stack_sample,
				     title.Data(),
				     "f");
	    entry->SetFillStyle(atl_sample->GetFillStyle());
	    entry->SetFillColor(atl_sample->GetFillColor());
	}
	
	// Sum of all model pdfs
	// Draw the channel pdf, this should be identical to the last pdf line drawn
	chan_pdf->plotOn(plot, Normalization(1., RooAbsReal::RelativeExpected),
	 		 FillColor(kYellow), LineColor(kBlack),
			 // LineStyle(kDashed));
			 Invisible());

	// Get the last pdf added to the plot (i.e. sum of all pdfs)
	RooCurve *TopMC = plot->getCurve();

	
	// error band
	chan_pdf->plotOn(plot,
			 Normalization(1., RooAbsReal::RelativeExpected),
			 FillColor(14),
			 FillStyle(3254),
			 VisualizeError(*fitResult, *m->GetNuisanceParameters()));
	RooCurve *c_errorband = plot->getCurve();

	/*
	// error band from mtw fit
	// Copy list of parameters and remove gamma_stat parameters
	RooArgSet *set = (RooArgSet*) (m->GetNuisanceParameters())->Clone("newset");
	TIterator *next_nuis = set->createIterator();
	RooRealVar *par = 0;
	while ((par = (RooRealVar*)next_nuis->Next())) {
	    TString name = par->GetName();
	    if ( name.Contains("gamma_stat") )
		set->remove(*par,false,true);
	}
	chan_pdf->plotOn(plot,
			 Normalization(1., RooAbsReal::RelativeExpected),
			 FillColor(6),
			 FillStyle(3001),
			 VisualizeError(*fitResult_mtw, *set));
	RooCurve *c_errorband_syst = plot->getCurve();
	*/
	
	entry = legend->AddEntry("", "Fit uncertainty", "f");
	entry->SetFillStyle(3254);
	entry->SetLineColor(14);
	entry->SetFillColor(14);

	// Add legend
	plot->addObject(legend);

	// data
	chan_data->plotOn(plot,
			  RooFit::DataError(RooAbsData::Poisson),
			  MarkerColor(kBlack));

	// Remove data points' x-error
	RooHist *h_data = plot->getHist();
	Int_t np      = h_data->GetN();
	Double_t *exl = h_data->GetEXlow();
	Double_t *exh = h_data->GetEXhigh();
	
	// Loop over all 'np' points p of RooHist
	// (n.b. histos start at p+1)
	for ( Int_t p = 0; p < np; p++ ) {
	    if ( fRemoveDataErrorX ) {
		exl[p] = 0.;
		exh[p] = 0.;
	    }
	}

	// Do the usual axis label stuff etc.
	//
	// Get the nominal histogram of the first process in each channel
	atl_sample = (AtlHistFactorySample *) atl_chan->GetListOfSamples()->At(0);
	const char *hist_name = atl_sample->GetName();
	TH1 *h = (TH1*) fModelFile->Get(Form("%s_hists/%s/%s_nominal",
					     chan->GetName(),
					     hist_name, hist_name));
	plot->GetXaxis()->ImportAttributes(h->GetXaxis());
	plot->GetYaxis()->ImportAttributes(h->GetYaxis());
	plot->SetNdivisions(505,"Y");

	// Add ATLAS, Lumi, sqrt(s)
	plot->addObject(GetAtlasLabel(0.56, 0.88, "Internal"));
	plot->addObject(fCMSEnergyLabel->Clone(Form("cms_%s", chan->GetName())));
	plot->addObject(fLumiLabel->Clone(Form("lumi_%s", chan->GetName())));

	plot->Draw();

	// Create ratio plot
	// -----------------
	// (Data/MC - 1)
	//
	// Data points : Data/MC - 1
	RooHist *h_datamc = (RooHist*) h_data->Clone(Form("ratio_datamc_%s", chan->GetName()));

	TH1F *h_mc = (TH1F*) chan_pdf->createHistogram(Form("mc_stack_%s", chan->GetName()),
						       *((RooAbsRealLValue*) chan_obs),
						       IntrinsicBinning());
	// Normalize with stack
	stack_sample = (RooRealSumPdf*) stack->Last();
	Double_t Nevts_mcstack = ((RooAbsReal*) stack_sample->createIntegral(RooArgSet(*chan_obs)))->getVal();
	h_mc->Scale(Nevts_mcstack/h_mc->Integral());

	// Divide Points by MC expectation
	// Scale error from data accordingly

	Double_t *y = h_datamc->GetY();
	exl = h_datamc->GetEXlow();
	exh = h_datamc->GetEXhigh();
	Double_t *eyl = h_datamc->GetEYlow();
	Double_t *eyh = h_datamc->GetEYhigh();
	
	for ( Int_t p = 0; p < np; p++ ) {
	    y[p] = y[p]/h_mc->GetBinContent(p+1) - 1;
	    eyl[p] = eyl[p]/h_mc->GetBinContent(p+1);
	    eyh[p] = eyh[p]/h_mc->GetBinContent(p+1);
	    if ( fRemoveDataErrorX ) {
		exl[p] = 0.;
		exh[p] = 0.;
	    }
	}

	// Uncertainty band for MC
	RooCurve *c_mcerror = (RooCurve*) c_errorband->Clone(Form("mc_error_%s", chan->GetName()));
	//xxx RooCurve *c_mcerror_syst = (RooCurve*) c_errorband_syst->Clone(Form("mc_error_syst_%s", chan->GetName()));
	// The uncertainty band consists of a set of 2N points, where N is the number of points
	// of the nominal RooCurve (TopMC)
	// The first N points are used for the upper errors (+1 sigma) and the other points are used
	// for the lower errors (-1 sigma) but in reverse order w.r.t. the +1 sigma points.
	Int_t j = 0;
	for ( Int_t i = 0; i < c_mcerror->GetN(); i++ ) {

	    if ( i < TopMC->GetN() )
		j = i;
	    else
		j = 2*TopMC->GetN() - i - 1;
	    
	    // Shift all points by central value (MC' - MC)
	    // Convert to relative error: divide by central value (MC' - MC)/MC
	    Double_t *mcy = TopMC->GetY();     // MC central value
	    Double_t *ery = c_mcerror->GetY(); // upper or lower error
	    //xxx Double_t *ery_s = c_mcerror_syst->GetY(); // upper or lower error
	    if ( mcy[j] != 0 ) {
		ery[i] = (ery[i] - mcy[j])/mcy[j];
		//xxx ery_s[i] = (ery_s[i] - mcy[j])/mcy[j];
	    }
	}
	c_mcerror->SetFillColor(14);
	c_mcerror->SetFillStyle(3254);
	c_mcerror->SetMarkerSize(0);

	//xxx c_mcerror_syst->SetFillColor(6);
	//xxx c_mcerror_syst->SetFillStyle(3001);
	//xxx c_mcerror_syst->SetMarkerSize(0);
	
	RooPlot *ratio = chan_obs->frame();
	ratio->SetNameTitle(Form("Ratio_Postfit_%s", chan->GetName()),
			    Form("Ratio_Postfit_%s", chan->GetName()));
	
	ratio->addObject(c_mcerror,"F");
	//xxx ratio->addObject(c_mcerror_syst,"F");
	ratio->addObject(h_datamc, "PE");

	ratio->GetXaxis()->ImportAttributes(h->GetXaxis());
	ratio->GetYaxis()->ImportAttributes(h->GetYaxis());
	ratio->GetYaxis()->SetTitle("Data/SM - 1");
	ratio->SetNdivisions(505,"Y");

	ratio->Draw();

	
	//
	// Signal pdf and (data - background pdf)
	// --------------------------------------

	RooPlot *plot2 = chan_obs->frame();
	plot2->SetNameTitle(Form("SigMinusBkg_%s", chan->GetName()),
			    Form("SigMinusBkg_%s", chan->GetName()));

	// Histogram for signal pdf
	// ------------------------
	RooRealSumPdf *single_smpl = (RooRealSumPdf*) single_samples->Last();
	TH1 *h_sig = single_smpl->createHistogram("Signal_pdf",
						  *((RooAbsRealLValue*) chan_obs),
						  IntrinsicBinning());
	Double_t Nevts_sig = ((RooAbsReal*) single_smpl->createIntegral(RooArgSet(*chan_obs)))->getVal();
	h_sig->Scale(Nevts_sig/h_sig->Integral());

	// the last in single samples is the first in atl_samples
	atl_sample = (AtlHistFactorySample *) atl_chan->GetListOfSamples()->At(0);

	// Draw signal pdf + a black line a contour
	single_smpl->plotOn(plot2,
			    Normalization(Nevts_sig,RooAbsReal::NumEvent),
			    FillColor(atl_sample->GetFillColor()),
			    FillStyle(atl_sample->GetFillStyle()),
			    DrawOption("F"));
	RooCurve *sig_pdf = plot2->getCurve();
	single_smpl->plotOn(plot2,
			    Normalization(Nevts_sig,RooAbsReal::NumEvent),
			    LineColor(kBlack),
			    LineWidth(1),
			    DrawOption("L"));
	// Draw uncertainty band for signal pdf
	// single_smpl->plotOn(plot2,
	// 		    Normalization(Nevts_sig,RooAbsReal::NumEvent),
	// 		    FillColor(14),
	// 		    FillStyle(3254),
	// 		    VisualizeError(*fitResult, *m->GetNuisanceParameters()));

	// Histogram for background pdf
	// ----------------------------
	stack_sample = (RooRealSumPdf*) stack->At(stack->GetEntries()-2);
	TH1 *h_bkg = stack_sample->createHistogram("Background_pdf",
						   *((RooAbsRealLValue*) chan_obs),
						   IntrinsicBinning());
	Double_t Nevts_mc = ((RooAbsReal*) stack_sample->createIntegral(RooArgSet(*chan_obs)))->getVal();
	h_bkg->Scale(Nevts_mc/h_bkg->Integral());

	// Draw bkg pdf and uncertainty band, but set option to invisible for bkg pdf
	stack_sample->plotOn(plot2,
			     Normalization(Nevts_mc, RooAbsReal::NumEvent),
			     Invisible());
	RooCurve *bkg_pdf = plot2->getCurve();

	stack_sample->plotOn(plot2,
			     Normalization(Nevts_mc, RooAbsReal::NumEvent),
			     FillColor(14),
			     FillStyle(3254),
			     VisualizeError(*fitResult, *m->GetNuisanceParameters()));
	RooCurve *bkg_err = plot2->getCurve();

	// Data
	RooHist *h_datamc2 = (RooHist*) h_data->Clone(Form("DataMinusBkg_%s", chan->GetName()));
	Double_t *x = h_datamc2->GetX();
	y = h_datamc2->GetY();
	exl = h_datamc2->GetEXlow();
	exh = h_datamc2->GetEXhigh();
	eyl = h_datamc2->GetEYlow();
	eyh = h_datamc2->GetEYhigh();

	// Loop with two tasks:
	// a) Shift data points by bkg exp.
	// b) Draw uncertainty band with abs. uncertainty for bkg pdf
	//    (i.e. shift uncertainty band to signal pdf values)
	//
	j = 0;
	for ( Int_t i = 0; i < bkg_err->GetN(); i++ ) {

	    if ( i < bkg_pdf->GetN() )
		j = i;
	    else
		j = 2*bkg_pdf->GetN() - i - 1;

	    // Shift all points by central value (MC_bkg' - MC_bkg) + MC_sig
	    Double_t *bkgx = bkg_pdf->GetX(); // Bkg pdf central value
	    Double_t *bkgy = bkg_pdf->GetY(); // Bkg pdf central value
	    Double_t *sigy = sig_pdf->GetY(); // Sig pdf central value
	    Double_t *ery  = bkg_err->GetY(); // upper or lower error
	    if ( bkgy[j] != 0 ) {
		ery[i] = (ery[i] - bkgy[j] + sigy[j]);
		// ery[i] = (ery[i] - bkgy[j]);
	    }
	    
	    // Set Central Values of RooCurve to signal pdf
	    for ( Int_t p = 0; p < np; p++ ) {
		if ( fRemoveDataErrorX ) {
		    exl[p] = 0.;
		    exh[p] = 0.;
		}
		
		if ( i < bkg_pdf->GetN() && // +1 sigma
		     bkgx[i] < x[p] && x[p] < bkgx[i+1] )
		    y[p] = y[p] - bkgy[i];
	    }
	}

	// Create a new graph, otherwise y-range is set to original
	// data values and modification is difficult
	TGraphAsymmErrors *g = new TGraphAsymmErrors(np, x, y,
						     exl, exh, eyl, eyh);
	// add data - bkg to plot
	plot2->addObject(g, "P");

	// Setup lables, axis, etc.
	plot2->GetXaxis()->ImportAttributes(h->GetXaxis());
	plot2->GetYaxis()->ImportAttributes(h->GetYaxis());
	plot2->SetNdivisions(505, "Y");

	// Add ATLAS, Lumi, sqrt(s)
	plot2->addObject(GetAtlasLabel(0.20, 0.87, "Internal"));

	TLatex *CMSLabel = (TLatex*) fCMSEnergyLabel->Clone(Form("cms2_%s", chan->GetName()));
	CMSLabel->SetX(0.67);
	CMSLabel->SetY(0.74);
	TLatex *LumiLabel  = (TLatex*) fLumiLabel->Clone(Form("lumi2_%s", chan->GetName()));
	LumiLabel->SetX(0.67);
	LumiLabel->SetY(0.84);
	plot2->addObject(CMSLabel);
	plot2->addObject(LumiLabel);
	
	// Add Legend
	TLegend *legend2 = new TLegend(0.2, 0.60, 0.50, 0.85);
	legend2->SetName(Form("Legend_%s", chan->GetName()));
	legend2->SetFillColor(0);
	legend2->SetFillStyle(0);
	legend2->SetBorderSize(0);

	entry = legend2->AddEntry("",
				  fMeasurement->GetUseAsimovData() ? "Asimov Data - background" : "Data - background",
				  "p");
	entry->SetMarkerColor(kBlack);
	entry->SetMarkerStyle(20);

	// Signal pdf legend entry
	entry = legend2->AddEntry("", atl_sample->GetTitle(), "f");
	entry->SetFillColor(atl_sample->GetFillColor());
	entry->SetFillStyle(atl_sample->GetFillStyle());

	// Fit uncertainty band
	entry = legend2->AddEntry("", "Fit uncertainty", "f");
	entry->SetFillStyle(3254);
	entry->SetLineColor(14);
	entry->SetFillColor(14);

	plot2->addObject(legend2);

	plot2->Draw();

	
	// Save all plots to file
	plot->Write();
	ratio->Write();
	plot2->Write();
    }

    c->Close();
    fout->Close();
}


//____________________________________________________________________

TLatex* AtlHistFactoryPlotterTask::GetAtlasLabel(Double_t x, Double_t y,
						 const char* text) {
    //
    // Get TLatex object with ATLAS + label
    //

    TString label = "ATLAS";
    if ( text )
	label.Append(Form(" #font[42]{%s}", text));
    
    TLatex *l = new TLatex(x, y, label.Data());
    l->SetNDC();
    l->SetTextFont(72);
    l->SetTextColor(kBlack);

    return l;
}

//____________________________________________________________________

Double_t AtlHistFactoryPlotterTask::GetGammaUncertainty(const char* name,
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
