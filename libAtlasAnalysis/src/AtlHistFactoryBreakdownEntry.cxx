//____________________________________________________________________
//
// DOCUMENT ME
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryBreakdownEntry
#include <AtlHistFactoryBreakdownEntry.h>
#endif
#include <AtlHistFactoryMeasurement.h>
#include <RooAbsData.h>
#include <RooAbsPdf.h>
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <RooWorkspace.h>
#include <RooStats/ModelConfig.h>
#include <RooStats/ProfileLikelihoodTestStat.h>
#include <Math/ProbFunc.h>
#include <TFile.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TSystem.h>
#include <iostream>

using namespace std;
using namespace RooStats;
using namespace HistFactory;
using namespace RooFit;

#ifndef __CINT__
ClassImp(AtlHistFactoryBreakdownEntry);
#endif

//____________________________________________________________________

AtlHistFactoryBreakdownEntry::AtlHistFactoryBreakdownEntry(const char* name,
							   const char* title,
							   const char* par)
    : AtlHistFactoryTask(name, title), fParameter(par) {
    //
    // Default constructor
    //

    fBatchNodeAll = kTRUE;
}

//____________________________________________________________________

AtlHistFactoryBreakdownEntry::~AtlHistFactoryBreakdownEntry() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlHistFactoryBreakdownEntry::RunEntry(const char* inputfile,
					    const char* outputfile,
					    const char* dataset,
					    const char* param,
					    Double_t precision) {
    //
    // Static function to compute the breakdown entry for
    // given parameter(s).
    //
    // All NPs, which name contains 'par', will be set constant
    // at their unconstrained MLE value
    //
    
    // Create directory
    TString breakdownfile = TString(outputfile);

    TString *ws_filename = new TString(inputfile);
    
    // Open workspace file
    TFile *f_ws = TFile::Open(ws_filename->Data());
    
    // Get infos for fit
    RooWorkspace *wSpace = (RooWorkspace*) f_ws->Get("combined");
    RooAbsData   *data   = wSpace->data(dataset);

    ModelConfig *model = (ModelConfig*) wSpace->obj("ModelConfig");
    RooAbsPdf   *pdf   = model->GetPdf();

    RooRealVar  *par   = 0;

    // do a fit first to get unconstrained MLE
	const RooArgSet *poiSet = model->GetParametersOfInterest();
	RooRealVar      *rrvPOI = (RooRealVar*)poiSet->first();
	cout << "Initial POI value = " << rrvPOI->getVal() << endl;
    RooFitResult *fitResult = pdf->fitTo(*data,
										 RooFit::SumW2Error(kFALSE),
										 RooFit::Minimizer("Minuit", "Migrad"),
										 RooFit::Offset(true),
										 RooFit::Extended(true),
										 RooFit::Save(true));
    fitResult->Print();
	
	// Save best fit value of the poi as a snapshot of the signal model
	model->SetSnapshot(*model->GetParametersOfInterest());
	cout << "Snapshot of S+B Model " << model->GetName() 
		 << " is set to the best fit value of the poi = " << rrvPOI->getVal() << endl;
	
    // Book Histograms for breakdown Info
    TFile *fout = new TFile(breakdownfile.Data(),"RECREATE");
    
    TH1F *h_breakdown      = new TH1F("h_breakdown", "Central value for breakdown", 1., 0., 1.);
    TH1F *h_breakdown_up   = new TH1F("h_breakdown_up", "Up value for breakdown", 1., 0., 1.);
    TH1F *h_breakdown_down = new TH1F("h_breakdown_down", "Down value for breakdown", 1., 0., 1.);
    
    // Nuisance parameters
    RooArgSet *nuis = (RooArgSet*)model->GetNuisanceParameters();
    TIterator *next_nuis = nuis->createIterator();

    // Disable all parameters, which contain the string 'param'
    while ((par = (RooRealVar*)next_nuis->Next())) {
	TString par_name = par->GetName();
	if ( par_name.Contains(param) ) {
	    cout << "Set parameter " << par_name.Data()
		 << " constant at val = " << par->getVal() << endl;
	     par->setConstant(kTRUE);
	}
	
	// Data stats
	TString par_check = param;
	if ( par_check.Contains("data_stat") ) {
	    cout << "Set parameter " << par_name.Data()
		 << " constant at val = " << par->getVal() << endl;
	     par->setConstant(kTRUE);
	}
	
	// All JES components
	if ( par_check == "JES" && IsJesComponent(par) ) {
	    cout << "Set parameter " << par_name.Data()
		 << " constant at val = " << par->getVal() << endl;
	     par->setConstant(kTRUE);
	}
	
	// All Lepton components
	if ( par_check == "LEP" && IsLepComponent(par) ) {
	    cout << "Set parameter " << par_name.Data()
		 << " constant at val = " << par->getVal() << endl;
	     par->setConstant(kTRUE);
	}
	
    }
    
    // First case:
    // -----------
    TStopwatch w;
    w.Start();

    //
    // Scan mu interval
    //
    
    // Setup test statistic
    ProfileLikelihoodTestStat ProfllTestStat(*pdf);
    
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit", "Migrad");
    std::string minimizerType = ROOT::Math::MinimizerOptions::DefaultMinimizerType();
    ProfllTestStat.SetMinimizer(minimizerType.c_str());
    ProfllTestStat.SetPrintLevel(0);
    ProfllTestStat.SetReuseNLL(true);
    ProfllTestStat.SetStrategy(0);
    ProfllTestStat.SetLOffset(true);
    ProfllTestStat.SetOneSided(false);

    Double_t mu_min = 0.;
    Double_t mu_max = 2.;

    Int_t Neval = TMath::CeilNint((mu_max - mu_min)/precision);
    if ( Neval % 2 == 0 ) // avoid even numbers
	Neval += 1;       // (causes problems if mu == 1 if used
                          //  with likelihood offset)
    
    Double_t mu_int  = (mu_max - mu_min)/Neval;
    
    Double_t p_value = 0.;
    Double_t q_mu    = 0.;

    // Current point of graph
    Int_t i = 0;

    // Store p-value can results
    TGraph *g = new TGraph(Neval);
    
    for ( Double_t mu = mu_min; mu <= mu_max; mu += mu_int ) {
	
	RooRealVar *MuModel = (RooRealVar*) model->GetParametersOfInterest()->first();
	MuModel->setVal(mu);
	
	RooArgSet MuModelTmp(*MuModel);

	// -2.*ln(llh-ratio)
	q_mu = ProfllTestStat.Evaluate(*data, MuModelTmp);
	q_mu = 2.*q_mu;

	// Asymptotic formulae for p-value, given that
	// q_mu follows a chi2 distr. with ndf = 1
	// http://arxiv.org/abs/1007.1727
	p_value = 2.*ROOT::Math::normal_cdf_c(TMath::Sqrt(q_mu), 1.);
	
	g->SetPoint(i, mu, p_value);
	i++;
    }

    w.Stop();
    w.Print();

    // Find x-values close to y = 0.317
    Double_t pvalue = (1.-ROOT::Math::gaussian_cdf(1))*2; // 68.3% CL --> pvalue ~ 0.317
    Double_t x1 = -1.;
    Double_t x2 = -1.;
    Double_t y1, y2;
    Int_t p1 = -1, p2 = -1;
    
    for ( Int_t i = 0; i < g->GetN() - 1; i++ ) {
	g->GetPoint(i, x1, y1);
	g->GetPoint(i+1, x2, y2);

	if ( y1 < pvalue && pvalue < y2 )
	    p1 = i;
	
	if ( y1 > pvalue && pvalue > y2 )
	    p2 = i;
    }

    if ( p1 < 0 || p2 < 0 ) {
	cout << "x1 = " << x1 << "   x2 = " << x2 << endl;
	cout << "Error: x1 or x2 below 0. Abort." << endl;
	gSystem->Abort();
    }

    // Linear interpolation close to 0.16
    // y = a*x + b

    Double_t xmin, xmax, ymin, ymax;
    Double_t a, b;

    // lower part
    g->GetPoint(p1, xmin, ymin);
    g->GetPoint(p1+1, xmax, ymax);

    a = (ymax - ymin)/(xmax - xmin);
    b = ymin - a*xmin;

    Double_t mu_low = (pvalue - b)/a;

    // upper part
    g->GetPoint(p2, xmin, ymin);
    g->GetPoint(p2+1, xmax, ymax);

    a = (ymax - ymin)/(xmax - xmin);
    b = ymin - a*xmin;

    Double_t mu_high = (pvalue - b)/a;

    cout << "mu high = " << mu_high << endl;
    cout << "mu low  = " << mu_low  << endl;
    
    // Store result:
    // ToDo: Divide by initial best fit value for poi
    RooRealVar *poi = (RooRealVar*) model->GetParametersOfInterest()->first();
    Double_t v_poi = ((RooRealVar*) fitResult->floatParsFinal().find(poi->GetName()))->getVal();
    cout << "v_poi = " << v_poi << endl;
    
    h_breakdown_up->Fill(param, (mu_high - v_poi)/v_poi);
    h_breakdown_down->Fill(param, (v_poi - mu_low)/v_poi);
    h_breakdown->Fill(param, (mu_high - mu_low)/(2.*v_poi));

    g->SetName(Form("g_%s", param));
    g->Write();
    
    fout->Write();
}

//____________________________________________________________________

void AtlHistFactoryBreakdownEntry::CreateRunScript(Option_t *option) {
    //
    // Create Run Script
    //

    ofstream out;
    out.open(fRunScript->Data());
    out << "#!/bin/sh" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ analysis job submission" << endl
	<< "#" << endl << endl
	<< "# Set environment" << endl;

    // Platform
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript", "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }

    if ( !WriteEnvSetup(out) ) {

    // ROOT
    out << "export ROOTSYS=" << gSystem->Getenv("ROOTSYS") << endl
	<< "export PATH=${ROOTSYS}/bin:${PATH}" << endl
	<< "export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}" << endl;

    // A++ and user-defined libraries
    TList *UserEnvs = new TList();
    UserEnvs->Add(new TNamed("LIBHEPEVENT",""));
    UserEnvs->Add(new TNamed("LIBATLASRUN",""));
    UserEnvs->Add(new TNamed("LIBATLASEVENT",""));
    UserEnvs->Add(new TNamed("LIBATLASANALYSIS",""));
    
    TIter next_lib(UserEnvs);
    TNamed *env = 0;
    while ( (env = (TNamed*)next_lib()) ) {
	if ( gSystem->Getenv(env->GetName()) != 0 ) {
	    out << "export " << env->GetName() << "="
		<< gSystem->Getenv(env->GetName()) << endl
		<< "export LD_LIBRARY_PATH=${"
		<< env->GetName() << "}/lib/${ARCH_TYPE}:${LD_LIBRARY_PATH}"
		<< endl;
	} else {
	    Error("CreateRunScript", "Environment variable %s not set. Abort!",
		  env->GetName());
	    gSystem->Abort(0);
	}
    }

    }
	
	out	<< endl << endl
		<< "# Job execution" << endl
		<< "JOBHOME=" << fJobHome->Data() << endl
		<< "cd $JOBHOME" << endl	
		<< "root -q -l -b run_entry.C > " << fLogFilePath->Data()
		<< " 2>&1" << endl;
	out.close();
}

//____________________________________________________________________

Bool_t AtlHistFactoryBreakdownEntry::ExecBatchJob(Option_t*) {
    //
    // Exec batch job
    //

    // cout << "JobHome: " << fJobHome->Data() << endl;
    // cout << "RunScript: " << fRunScript->Data() << endl;
    // cout << "Outputfile: " << fOutputFileName->Data() << endl;
    // cout << "Logfile: " << fLogFilePath->Data() << endl;

    //
    // Create short C macro
    //
    TString root_macro = TString(fJobHome->Data());
    root_macro.Append("/run_entry.C");
    root_macro.ReplaceAll("//","/");
    
    ofstream out;
    out.open(root_macro.Data());
    out << "{" << endl
	<< "AtlHistFactoryBreakdownEntry::RunEntry("
	<< "\"" << fMeasurement->GetModelFileName()->Data() << "\"," << endl
	<< "\"" << fOutputFileName->Data() << "\"," << endl
	<< "\"" << (fMeasurement->GetUseAsimovData() ? "asimovData" : "obsData") << "\"," << endl
	<< "\"" << fParameter.Data() << "\"," << endl
	<< 0.01 << ");" << endl
	<< "}" << endl;
    out.close();    

    
    return SubmitBatchJob();
}

//____________________________________________________________________

Bool_t AtlHistFactoryBreakdownEntry::IsJesComponent(const RooRealVar* par) {
    //
    // Decide wether the given parameter is a jet energy scale
	// uncertainty component
    //
	TString pName = par->GetName();
	return ( pName.Contains("Jes") ||
			 pName.Contains("EtaIntercalibration") ||
			 pName.Contains("Pileup") ||
			 pName.Contains("PunchThrough") ||
			 pName.Contains("flavor_comp") ||
			 pName.Contains("flavor_response") ||
			 pName.Contains("SinglePart") );
}

//____________________________________________________________________

Bool_t AtlHistFactoryBreakdownEntry::IsLepComponent(const RooRealVar* par) {
    //
    // Decide wether the given parameter is a lepton
	// uncertainty component. This includes electrons and muons.
    //
	TString pName = par->GetName();
	return ( pName.Contains("eer") ||
			 pName.Contains("ees") ||
			 pName.Contains("lepIdSF") ||
			 pName.Contains("lepRecoSF") ||
			 pName.Contains("lepTrigSF") ||
			 pName.Contains("muid_res") ||
			 pName.Contains("mums_res") ||
			 pName.Contains("musc") );
}
