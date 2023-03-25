//____________________________________________________________________
//
// Class with information on systematics
//
// This class holds all information for a systematic, i.e. the
// pointer to the nominal and the systematic histograms. In order to
// keep the class structure simple and efficient, the
// class only stores information for _one_ process at a time.
//
// Please use the ChangeProcess("processname") function to change processes.
//
// There are two operation modes:
// - This class can be used with a MCPlotter file to extract the histograms
// and save them in a template file.
// - The template file should then be used to perform shape tests, extract
// rate uncertainties, etc. 
//
// The derived class need to implement the functions:
// - Initialize:
//   Function for reading MCPlotter files and extracting a list of
//   histograms that are needed for ComputeUpDownVariation().
//   (The histograms can then be accessed for each process by calling
//    ChangeProcess() )
//
// - GetHistsFromFile:
//   Read systematic templates from template file for a given process.
//
// - ComputeUpDownVariation:
//   Function for computing the up and down variation for this systematic
//   given the histograms from any MCPlotter file.
//
//
// If this class is used with MCPlotter files:
// -------------------------------------------
// To prevent open and closing of files, the user should get all infos
// for all processes, before going to the next systematic.
// - see AtlHistFactoryTask as example
// 
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#include <TSystem.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TROOT.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLatex.h>

#ifndef __CINT__
ClassImp(AtlHistFactorySystematic);
#endif

//____________________________________________________________________

AtlHistFactorySystematic::AtlHistFactorySystematic() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlHistFactorySystematic::AtlHistFactorySystematic(const char *systname,
						   const char *systtitle,
						   Bool_t useShape) :
    TNamed(systname, systtitle) {
    //
    // Default constructor
    //
    fHistUp   = 0;
    fHistDown = 0;
    fHistNom  = 0;

    fHistShapeResidual = 0;
    fGraphShapeQQPlot = 0;

    fTemplateFile = 0;
    fDiscriminant = 0;

    fIsNominal = kFALSE;
    fUseShape  = useShape;
    fUseFullStats = kFALSE;

    fNPseudoExp   = 30000;
    fUsePseudoExp = kFALSE;
    fHistChi2 = 0;
}

//____________________________________________________________________

AtlHistFactorySystematic::~AtlHistFactorySystematic() {
    //
    // Default destructor
    //
    if ( fDiscriminant != 0 ) delete fDiscriminant;
    fTemplateFile = 0; // we do not own the template file!
}

//____________________________________________________________________

void AtlHistFactorySystematic::PerformShapeTest(Double_t& pval_min,
						Double_t& kstest_min) {
    //
    // Perform shape test, returns p-value for up and down variations.
    //
    // If a systematic does not have a up/down histogram for shape
    // comparisons this function can be overloaded for this particular
    // systematic.
    //

    // Method only works with a shape template file
    if ( fTemplateFile == 0 ) {
	Error("PerformShapeTest",
	      "You need to set a template file. Abort!");
	gSystem->Abort();
    }

    // Clone histos to leave data member unchanged
    TH1 *h_up   = (TH1*) fHistUp->Clone("syst_up");
    TH1 *h_down = (TH1*) fHistDown->Clone("syst_down");
    
    // Normalize the systematic histograms to the nominal event yield
    h_up->Scale(fHistNom->Integral()/h_up->Integral());
    h_down->Scale(fHistNom->Integral()/h_down->Integral());

    Int_t n_res = fHistNom->GetNbinsX();
    Double_t *res_up   = new Double_t[n_res];
    Double_t *res_down = new Double_t[n_res];
    Double_t *res;

    fPValue_up   = 0.;
    fPValue_down = 0.;

    if ( fUsePseudoExp ) {
	Double_t chi2_up   = fHistNom->Chi2Test(h_up,   "WWCHI2", res_up);
	Double_t chi2_down = fHistNom->Chi2Test(h_down, "WWCHI2", res_down);

	Int_t bin_chi2_up   = fHistChi2->FindBin(chi2_up);
	Int_t bin_chi2_down = fHistChi2->FindBin(chi2_down);

	fPValue_up   = fHistChi2->Integral(bin_chi2_up,
					fHistChi2->GetNbinsX()+1);
	fPValue_down = fHistChi2->Integral(bin_chi2_down,
					fHistChi2->GetNbinsX()+1);
    } else {
	fPValue_up   = fHistNom->Chi2Test(h_up, "WW", res_up);
	fPValue_down = fHistNom->Chi2Test(h_down, "WW", res_down);
    }

    pval_min  = TMath::Min(fPValue_up, fPValue_down);
    if ( fPValue_up < fPValue_down)
	res = res_up;
    else
	res = res_down;
    
    // Create chi2 residual plots
    fHistShapeResidual = (TH1*) fHistNom->Clone("residuals");
    for ( Int_t i = 0; i < n_res; i++ ) {
	fHistShapeResidual->SetBinContent(i+1, res[i]);
	// residuals do not have an associated error, thus set it to zero
	fHistShapeResidual->SetBinError(i+1, 0.);
    }
    
    // Create chi2 residual Q-Q plot
    // =============================
    // first approach use normal distribution
    // although the residuals might follow a student t distribution.
    fGraphShapeQQPlot = new TGraph(n_res);
    
    // Sort residuals
    // ---------------
    // Create index array
    TArrayI *index = new TArrayI(n_res);
    TMath::Sort(n_res, res, index->GetArray(), kFALSE);
    
    Double_t *sorted_res = new Double_t[n_res];
    // Fill sorted residual list
    for ( Int_t j = 0; j < n_res; j++){
	sorted_res[j] = res[index->At(j)];
    }
    
    for ( Int_t k = 0; k < n_res; k++ ) {
	// Evaluate qunatiles at (m-0.5)/n
	// m -> Number of residual (1, 2, ..., n_res)
	// n -> n_res
	// m-0.5 == k+0.5
	fGraphShapeQQPlot->SetPoint(k,
				    TMath::NormQuantile((k+0.5)/n_res),
				    sorted_res[k]);
    }
    
    Double_t kstest_up   = fHistNom->KolmogorovTest(h_up);
    Double_t kstest_down = fHistNom->KolmogorovTest(h_down);
    kstest_min  = TMath::Min(kstest_up, kstest_down);

    // clean-up
    delete h_up;
    delete h_down;
}

//____________________________________________________________________

void AtlHistFactorySystematic::SetDiscriminant(const char* discriminant) {
    //
    // Setter function for the histogram name and it's directory
    // within the MCPlotter file.
    //

    if ( fDiscriminant != 0 ) delete fDiscriminant;
    fDiscriminant = new TString(discriminant);
}

//____________________________________________________________________

void AtlHistFactorySystematic::GetRateUnc(Double_t& rate_up,
					  Double_t& rate_down) {
    //
    // Return the relative rate uncertainties
    //
    //   rate_up   = (up - nom)/nom;
    //   rate_down = (nom - down)/nom;
    //
    //   up/down/nom = Number of Events (up/down/nom)
    //

    if ( fHistNom == 0 || fHistUp == 0 || fHistDown == 0 ) {
	Error("AtlHistFactorySystematic",
	      "Please provide all three histograms: nominal, up-var. and down-variation. Abort!");
	gSystem->Abort();
    }

    Double_t nom  = fHistNom->Integral();
    Double_t up   = fHistUp->Integral();
    Double_t down = fHistDown->Integral();
    
    rate_up   = (up - nom)/nom;
    rate_down = (nom - down)/nom;
}

//____________________________________________________________________

void AtlHistFactorySystematic::ChangeProcess(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //

    // To check the histograms here might cause problems.
    //
    // 1. We assume that a systematic always has up/down variations
    //    - Is this true?
    //    - Do we need to treat a systematic always with up/down
    //      variations?
    //    - Can the histfactory deal with one sided systematics
    //      that are not symmetriezed?
    // 2. The systematic 'nominal' has by definition only one
    //    histogram...
    // 3. When creating the file holding all templates, we assume
    //    that we save always the up/down variation
    //    - What do we do with nominal or if we don't have both
    //      variations?
    //
    // --> Possible solution:
    //     - Make this function pure virtual, each systematic has
    //       to define its own way of retrieving histograms
    //     - GetRateUncertainty() has to be a virtual function
    //     - For saving templates, we return a list of templates
    //       to be saved.
    //
    
    if ( fTemplateFile != 0 ) {
	// we get the histos directly from the template file
	fHistNom = (TH1*) fTemplateFile->Get(Form("%s_%s",
						  process,
						  "nominal"));
	GetHistsFromFile(process);

	// For nominal systematic only nominal histogram exists
	if ( fIsNominal ) {
	    if ( fHistNom == 0 ) {
		Error("ChangeProcess",
		      "Could not find histogram '%s_nominal' in file %s\n. Abort!",
		      process, fTemplateFile->GetName());
		gSystem->Abort();
	    }
	} else if ( fHistUp == 0 || fHistDown == 0 || fHistNom == 0 ) {
	    // For all other sytematics, all three histograms must exist
	    Error("ChangeProcess",
		  "Could not find histograms for process '%s' in file '%s'.\n Abort!",
		  process,
		  fTemplateFile->GetName());
	    gSystem->Abort();
	}
    } else {
	// Compute the variation using MCPlotter files
	ComputeUpDownVariation(process);

	// Check if templates for systematic up/down have been created
	//
	// This is problematic if one chooses to have not a up down variation
	// or nominal. What happens then?
	if ( (fHistUp == 0 || fHistDown == 0) && fHistNom == 0) {
	    Error("ChangeProcess",
		  "Could not find histogram for process '%s'. Abort!",
		  process);
	    gSystem->Abort();
	}
    }
}

//____________________________________________________________________

void AtlHistFactorySystematic::SaveTemplates(TFile *fout) {
    //
    // Save the templates of the up/down variations in the
    // output file.
    //
    // Systematics that do not produce a up and down varied histogram
    // can overwrite this function, e.g. for the nominal case.
    //

    // Write() will save it to the current directory
    // therefore change the current directory
    fout->cd();

    // fHistUp->SetDirectory(fout);
    fHistUp->Write();
    
    // fHistDown->SetDirectory(fout);
    fHistDown->Write();
}

//____________________________________________________________________

void AtlHistFactorySystematic::SaveShapeCtrlPlots(TFile *fout,
						  const char *process) {
    //
    // Save the residual and Q-Q ctrl plots for this systematic
    // in given output file
    //
    // Systematics that do not produce a up and down varied histogram
    // can overwrite this function
    //

    // Write() will save it to the current directory
    // therefore change the current directory
    fout->cd();
    if ( fout->GetDirectory(GetName()) == 0 ) {
	fout->mkdir(GetName(), GetName());
    }
    fout->cd(GetName());


    fHistShapeResidual->SetName(Form("%s_%s_residuals",
				     process,
				     GetName()));
    fGraphShapeQQPlot->SetName(Form("%s_%s_QQplot",
				    process,
				    GetName()));
    fGraphShapeQQPlot->GetXaxis()->SetTitle("expected residuals");
    fGraphShapeQQPlot->GetYaxis()->SetTitle("sample residuals");
    
    fHistShapeResidual->Write();
    fGraphShapeQQPlot->Write();

    if ( fHistChi2 != 0 ) fHistChi2->Write();
    
    // clean up histograms
    delete fHistShapeResidual;
    delete fGraphShapeQQPlot;
}

//____________________________________________________________________

void AtlHistFactorySystematic::ComputeChi2Distribution() {
    // 
    // Compute the chi square distribution for the reference
    // histogram using pseudo experiments and assuming gaussian
    // errors in each bin.
    //
    // The resulting chi2 distribution can be used to compute p-values
    // without assuming a chi square distribution.
    //

    // Redirect all the info statements from Chi2Test during
    // pseudo experiments to trash bin
    gSystem->RedirectOutput("/dev/null");

    
    // Delete old chi2 histogram if it exists
    if ( fHistChi2 != 0 ) delete fHistChi2;
    fHistChi2 = new TH1F(Form("%s_Chi2Dist", fHistNom->GetName()),
			 "Chi2 Distriubtion",
			 200., 0., 80.);
    
    // The nominal histogram holds the reference histogram
    TH1F *hVaried_a = (TH1F*) fHistNom->Clone("Varied_A");
    TH1F *hVaried_b = (TH1F*) fHistNom->Clone("Varied_B");

    TRandom3 rnd1;
    rnd1.SetSeed();
    TRandom3 rnd2;
    rnd2.SetSeed();

    for(Int_t i = 0; i < fNPseudoExp; i++) {
	for( Int_t j = 1; j <= fHistNom->GetNbinsX(); j++) {
	    hVaried_a->SetBinContent(j,
				     rnd1.Gaus(fHistNom->GetBinContent(j),
					       fHistNom->GetBinError(j)));
	    hVaried_b->SetBinContent(j,
				     rnd2.Gaus(fHistNom->GetBinContent(j),
					       fHistNom->GetBinError(j)));
	    // keep the relative error of the bin fixed
	    // --> scale nominal error by the ratio of the bin contents
	    Double_t errVar_a = fHistNom->GetBinError(j);
	    errVar_a *= hVaried_a->GetBinContent(j)/fHistNom->GetBinContent(j);
	    hVaried_a->SetBinError(j, errVar_a);
	    
	    Double_t errVar_b = fHistNom->GetBinError(j);
	    errVar_b *= hVaried_b->GetBinContent(j)/fHistNom->GetBinContent(j);
	    hVaried_b->SetBinError(j, errVar_b);
	}
	fHistChi2->Fill(hVaried_a->Chi2Test(hVaried_b, "WWCHI2"));
    }
    // normalize distribution
    fHistChi2->Scale(1.0/fHistChi2->Integral());
    delete hVaried_a;
    delete hVaried_b;

    // restore std_out/std_err
    gSystem->RedirectOutput(0);
}

//____________________________________________________________________

TH1F* AtlHistFactorySystematic::GetChi2Distribution() {
    //
    // Return the chi2 distribution (compute dist. if necessary)
    //
    if ( fHistChi2 == 0 )
	ComputeChi2Distribution();

    // Check if it is the correct chi2 distriubtion for
    // the current histogram
    TString chi2name = fHistChi2->GetName();
    if ( !chi2name.Contains(fHistNom->GetName()) )
	ComputeChi2Distribution();
        
    return fHistChi2;
}

//____________________________________________________________________

void AtlHistFactorySystematic::ExportShapePlots(const char *process,
						const char *dir) {
    //
    // Export shape plots to pdf for given process in directory 'dir'
    //

    // set batch mode to suppress canvas
    Bool_t IsBatch = gROOT->IsBatch();
    if ( !IsBatch ) gROOT->SetBatch(kTRUE);
    
    // Setup Atlas Style if possible
    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    if ( AtlasStyle == 0 ) {
	Warning("ExportShapePlots",
		"Could not find ATLAS Style! Using current style.");
    } else {
	gROOT->SetStyle("ATLAS");
	gROOT->ForceStyle();
    }

    // Setup Main and Ratio Pad
    // - see HepDataMCPlot::SetupPad() for details
    TCanvas *Canvas = gROOT->MakeDefCanvas();

    Double_t RatioHeight = 0.4;
    Double_t addspace  = RatioHeight - gStyle->GetPadBottomMargin() + gStyle->GetLabelSize()*0.5;
    Double_t newheight = (1. + addspace);

    TPad *MainPad  = new TPad("MainPad", "", 0., 1.- 1./newheight, 1., 1.);
    TPad *RatioPad = new TPad("RatioPad", "", 0., 0., 1., RatioHeight/newheight);
    RatioPad->SetBottomMargin(gStyle->GetPadBottomMargin()/RatioHeight);

    Canvas->SetWindowSize(Canvas->GetWindowWidth(),
			  Canvas->GetWindowHeight()*(1 + addspace));

    // Main Pad
    // - Draw distributions normalized (-> to unity or nominal?)
    //
    // Ratio Pad
    // - Draw a) (up - nom)/nom
    //        b) (nom - down)/nom
    // - for a), and b): up/down are scaled to nominal
    //   (i.e. comparing shapes only)
    //
    // - Uncertainty band:
    //   relativ error of nominal histogram

    // Clone histos to leave data member unchanged
    TH1 *h_up   = (TH1*) fHistUp->Clone("syst_up");
    TH1 *h_down = (TH1*) fHistDown->Clone("syst_down");
    TH1 *h_nom  = (TH1*) fHistNom->Clone("nominal");
    
    // Normalize the systematic histograms to the nominal event yield
    h_up->Scale(h_nom->Integral()/h_up->Integral());
    h_down->Scale(h_nom->Integral()/h_down->Integral());

    // Histo Shapes
    Canvas->cd();
    MainPad->Draw();
    MainPad->cd();

    // Setup
    // - colors
    h_nom->SetLineColor(kBlack);
    h_up->SetLineColor(kBlue);
    h_down->SetLineColor(kRed);
    // - fill style
    h_nom->SetFillStyle(0);
    h_up->SetFillStyle(0);
    h_down->SetFillStyle(0);
    
    // - axis labels
    h_nom->SetLabelSize(0., "X");
    h_nom->SetTitleSize(0., "X");
    h_nom->GetXaxis()->SetDecimals(kTRUE);
    h_nom->GetYaxis()->SetDecimals(kTRUE);

    // - axis range
    h_nom->SetMaximum(h_nom->GetMaximum()*1.4);

    // Draw main
    h_nom->Draw("hist");
    h_up->Draw("same,hist");
    h_down->Draw("same,hist");

    h_nom->SetYTitle("Entries");
    h_nom->SetTitleOffset(1.2, "Y");

    // Draw legend
    TLegend *leg = new TLegend(0.45, 0.75, 0.8, 0.92);
    leg->SetHeader(process);
    leg->AddEntry((TObject*)0, Form("%s", GetTitle()), "");
    leg->AddEntry(h_nom, "Nominal", "l");
    leg->AddEntry(h_up,
		  Form("up, p-value = %3.2f", fPValue_up),
		  "l");
    leg->AddEntry(h_down,
		  Form("down, p-value = %3.2f", fPValue_down),
		  "l");
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetLineColor(0);
    leg->SetBorderSize(0);
    leg->Draw();
    
    // Draw Atlas label
    // -> see HepDataMCPlot::DrawAtlasLabel() for details
    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(kBlack);

    double delx = 0.115*696*gPad->GetWh()/(604*gPad->GetWw());

    Double_t x = 0.2;
    Double_t y = 0.87;
    
    l.DrawLatex(x,y,"ATLAS");

    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(kBlack);
    p.DrawLatex(x+delx,y, "Internal");

    
    // Shape and uncertainty band
    Canvas->cd();
    RatioPad->Draw();
    RatioPad->cd();

    TH1 *h_shape_up   = (TH1*) h_up->Clone("shape_up");
    h_shape_up->Add(fHistNom, -1.);
    h_shape_up->Divide(fHistNom);
    h_shape_up->SetYTitle("shape diff.");
    
    TH1 *h_shape_down = (TH1*) h_down->Clone("shape_down");
    h_shape_down->Add(fHistNom, -1.);
    h_shape_down->Divide(fHistNom);
    
    TH1 *h_err = (TH1*) fHistNom->Clone("err_band");
    for ( Int_t i = 1; i <= h_err->GetNbinsX(); i++ ) {
	h_err->SetBinError(i, h_err->GetBinError(i)/h_err->GetBinContent(i));
	h_err->SetBinContent(i, 0.);
    }

    // Setup
    // - colors
    h_shape_up->SetLineColor(kBlue);
    h_shape_down->SetLineColor(kRed);

    // - axis labels
    h_shape_up->SetTitleSize(1./RatioHeight*gStyle->GetTitleSize("X"), "X");
    h_shape_up->SetLabelSize(1./RatioHeight*gStyle->GetLabelSize("X"), "X");

    h_shape_up->SetTitleOffset(0.5, "Y");
    h_shape_up->GetYaxis()->SetTitle("shape diff.");
    
    h_shape_up->SetTitleSize(1./RatioHeight*gStyle->GetTitleSize("Y"), "Y");
    h_shape_up->SetLabelSize(1./RatioHeight*gStyle->GetLabelSize("Y"), "Y");

    h_shape_up->GetXaxis()->SetDecimals(kTRUE);
    h_shape_up->GetYaxis()->SetDecimals(kTRUE);

    h_shape_up->GetXaxis()->SetNdivisions(505);
    h_shape_up->GetYaxis()->SetNdivisions(505);

    Double_t min = h_shape_up->GetMinimum();
    Double_t max = h_shape_up->GetMaximum();

    if ( min > h_shape_down->GetMinimum() ) min = h_shape_down->GetMinimum();
    if ( max < h_shape_down->GetMaximum() ) max = h_shape_down->GetMaximum();

    h_shape_up->SetMaximum(max + 0.1);
    h_shape_up->SetMinimum(min - 0.1);

    // Draw ratio
    h_shape_up->SetYTitle("shape diff.");
    h_shape_down->SetYTitle("shape diff.");
    
    h_shape_up->Draw("hist");
    h_shape_up->GetYaxis()->SetTitle("shape diff.");
    h_shape_down->Draw("hist,same");

    TGraphAsymmErrors *h_errorband = new TGraphAsymmErrors(h_err);
    h_errorband->SetFillStyle(3254);
    h_errorband->SetFillColor(16);
    h_errorband->SetLineColor(16);
    h_errorband->SetMarkerSize(0.);
    h_errorband->Draw("5,same");

    Canvas->SaveAs(Form("%s/%s.pdf",
			dir, process));

    delete h_up;
    delete h_down;
    delete h_nom;
    delete leg;

    // Restore old batch mode
    gROOT->SetBatch(IsBatch);
}

