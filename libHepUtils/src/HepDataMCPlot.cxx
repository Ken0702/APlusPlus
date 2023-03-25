//____________________________________________________________________
//
// Plot overlayed or stacked MC histograms with DATA on top
//
// * The same binning for all histograms is required
// * Until now only 1d histograms are supported
//
// For drawing the histograms in grouped/ungrouped mode use
// SetGroupHistograms(). All MC stack histograms can be unified into a
// single-coloured histogram by SetUnifyMCStack().
//
// The total statistical uncertainty of the MC can be shown by using
// SetDrawMCError(). The errors are displayed as a rectangular hatched
// area on top of each bin.
//
// Usually the error per bin for the DATA histogram as well as for the
// total MC stack is computed as sqrt(sum of squares of weight) for
// each bin, which is ROOT's standard error handling for
// histograms. For verly low entry numbers inside a bin such kind of
// errors are not valid any more since the error function becomes more
// and more poissonic and the entry number is bound to be larger equal
// than zero which results in asymmetric error bars and in non-zero
// errors for bins without any entry. To obtain a better description,
// for entry numbers less then 33 the upper and lower confidence
// limits are estimated for a confidence level of 68% by the method of
// Feldman and Cousins (see the class TFeldmanCousins or the
// Statistics Review of the Particle Data Group). This method provides
// a smooth transition from the symmetric gaussian errors for large
// entry numbers to the completely asymmetric errors for zero-entry
// bins. This gives a realistic description of the uncertainties,
// however it slows down the speed of drawing the histograms. The
// limitt of 33 entries was chosen to have a fair trade-off between a
// good description and the time needed for drawing.
//
//
// Fitter:
// The plot gives the possibility to fit the MC distributions to the
// data distribution. This is done using the TFractionFitter class
// contained in ROOT.
// Starting from your plot object HepDataMCPlot *pl, the fit can be easily
// performed. First start with:
// pl->PrepareFit();
// This creates the needed HepTemplate objects. Folders are treated as one
// template per default. If you want to have all samples as single templates,
// use pl->PrepareFit(kTRUE);
// If you want you can specify start values for the template fractions
// pl->SetTemplateStartValue(template_name,initValue,initErr);
// and/or bounds:
// pl->SetTemplateBounds(template_name,low,up);
// The functions pl->ListFolders() and pl->ListSingleSamples() list all
// folders and single samples contained in the plot. Their names are used
// for the naming of the template objects.
// The fit is started by:
// pl->Fit();
// The function returns the value 0, if the fit converged.
// You can list all obtained fractions by
// pl->ListTemplateFractions();
// or get a fraction of a specific template:
// pl->GetTemplateFitFraction(template_name);
// Note that the fit uses only non-empty histograms. Templates for empty
// histograms are not created.
// Finally you can plot the data together with the fitted MC as stacked
// histogram by:
// pl->DrawFit();
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCPlot
#include <HepDataMCPlot.h>
#endif
#include <HepTemplate.h>
#include <TList.h>
#include <TObjString.h>
#include <HepDataMCSample.h>
#include <TStyle.h>
#include <TBrowser.h>
#include <TPad.h>
#include <iostream>
#include <TObjectTable.h>
#include <TLegendEntry.h>
#include <TFeldmanCousins.h>
#include <TFractionFitter.h>
#include <TVirtualFitter.h>
#include <Fit/Fitter.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>

#include <TLatex.h>
#include <TPave.h>
#include <cmath>
#include <TLine.h>
#include <TMarker.h>
#include <TH1.h>

#include "Math/Math.h"
#include "Math/SpecFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"


using namespace std;

#ifndef __CINT__
ClassImp(HepDataMCPlot);
#endif

static TFeldmanCousins fgFC(0.680);

static Double_t fgFCHigherCache[33] = {1.28,2.755,4.255,5.305,6.78,7.81,9.28,10.305,11.32,12.795,13.81,14.82,16.295,17.305,18.32,19.325,20.8,21.81,22.82,23.825,25.305,26.31,27.32,28.325,29.33,30.805,31.815,32.82,33.825,34.83,36.24,37.315,38.32};
static Double_t fgFCLowerCache[33] = {0,0.37,0.74,1.105,2.34,2.755,3.825,4.255,5.305,6.335,6.78,7.81,8.83,9.28,10.305,11.32,12.335,12.795,13.81,14.82,15.835,16.295,17.305,18.32,19.325,20.335,20.8,21.81,22.82,23.825,24.835,25.305,26.31};


//____________________________________________________________________

HepDataMCPlot::HepDataMCPlot() {
    //
    // Default constructor
    //
    Init();
}

//____________________________________________________________________

HepDataMCPlot::HepDataMCPlot(const char* name, const char* title) :
    TNamed(name, title) {
    //
    // Normal constructor
    //
    Init();
    fMCHistStack = new THStack(Form("hstack_%s", name), title);
    fLegend = new TLegend(0.59, 0.42, 0.90, 0.86);
    // fLegend = new TLegend(0.54, 0.42, 0.86, 0.86); // 0.59 .. 0.90
    fXTitle = new TString;
    fYTitle = new TString;

}

//____________________________________________________________________

HepDataMCPlot::~HepDataMCPlot() {
    //
    // Default destructor
    //
    if ( fMCHistStack != 0 ) {
	if ( fMCHistStack->GetHists() != 0 ) fMCHistStack->GetHists()->Delete();
	delete fMCHistStack;
    }
    if ( fLegend      != 0 ) delete fLegend;
    if ( fLegendLarge != 0 ) delete fLegendLarge;
    if ( fXTitle      != 0 ) delete fXTitle;
    if ( fYTitle      != 0 ) delete fYTitle;
    fHistDATA = 0;
    if ( fMCFolders != 0 ) {
//	fMCFolders->Delete();
	delete fMCFolders;
    }
}

//____________________________________________________________________

void HepDataMCPlot::Init() {
    //
    // Initialize variables
    //
    fCanvas    = nullptr;
    fFitter = nullptr;
    fGraphDATA = nullptr;
    fHistDATA = nullptr;
    fHistogramLastInStack = nullptr;
    fHistogramMainPad   = nullptr;
    fHistogramRatioPad1 = nullptr;
    fHistogramRatioPad2 = nullptr;
    fLegend = nullptr;
    fLegendLarge = nullptr;
    fMCErrors  = nullptr;
    fMCFitTemplates = nullptr;
    fMCFittedHistStack = nullptr;
    fMCFolders = nullptr;
    fMCHistStack = nullptr;
    fMCSingleSamples = nullptr;
    fMainPad   = nullptr;
    fRatioLine = nullptr;
    fRatioPad1 = nullptr;
    fRatioPad2 = nullptr;
    fXTitle = nullptr;
    fYTitle = nullptr;

    fDataMCRatioCenterOne  = kFALSE;
    fDrawCMSLumiAtlasLabel = kTRUE;
    fDrawData = kTRUE;
    fDrawDataErrorX = kTRUE;
    fDrawDataMCRatio  = kTRUE;
    fDrawDataZeroEntries = kTRUE;
    fDrawDataZeroEntryErrors = kTRUE;
    fDrawMCError  = kFALSE;
    fDrawNormError  = kFALSE;
    fDrawSignalOverlay = kTRUE;
    fDrawSignificance = kFALSE;
    fFirstDraw = kTRUE;
    fFirstLegend = kTRUE;
    fGroupHistograms = kTRUE;
    fUnifyMCStack = kFALSE;
    fUseAtlasStyle = kTRUE;
    fUseOverflowUnderflow = kTRUE;
    fUseShortCMSLumiLabel = kTRUE;
    fYRangeUser = kFALSE;

    fAddRange = 1.1;
    // fAtlasLabelPosX      = 0.51;
    fAtlasLabelPosX      = 0.56;
    fAtlasLabelPosY      = 0.88;
    fAtlasLabelStatus = "Internal";
    fCMSEnergyLabelPosX  = 0.20;
    fCMSEnergyLabelPosY  = 0.79;
    fCenterOfMassEnergyLabel = "7";
    fLuminosityLabel = "4.7 fb^{-1}";
    fLuminosityLabelPosX = 0.20;
    fLuminosityLabelPosY = 0.85;
    fRatioHeight = 0.4;
    fRatioLine = 0;
    fScaleOverlay = 1.;
    fN_MCHistograms = 0;
}

//____________________________________________________________________

void HepDataMCPlot::SetHistDATA(TH1F *h, const char* label = "Data",
				Float_t lumiData = 1.) {
    //
    // Set DATA histogram
    //

    fHistDATA = h;
    fHistDATA->SetName(Form("%s_data", h->GetName()));
    fHistDATA->SetTitle(label);

    fLumiDATA = lumiData;
}

//____________________________________________________________________

void HepDataMCPlot::AddHistMC(TH1F *h, HepDataMCSample *sample) {
    //
    // Add MC histogram to stack
    //
    fMCHistStack->Add(h);
    sample->SetHistogram(h);
    h->SetName(Form("%s_mc%d", h->GetName(), fN_MCHistograms++));
    h->SetTitle(sample->GetLabel());
    h->SetLineColor(gStyle->GetHistLineColor());
    h->SetLineStyle(sample->GetLineStyle());
    h->SetLineWidth(sample->GetLineWidth());
    h->SetFillColor(sample->GetColor());
    h->SetFillStyle(sample->GetFillStyle());
    fLegend->AddEntry(h, sample->GetLabel(), "F");
}

//____________________________________________________________________

void HepDataMCPlot::AddHistMC(TH1F *h, HepDataMCFolder *folder,
			      HepDataMCSample *sample) {
    //
    // Add grouped MC histogram to stack
    //
    AddHistMC(h, sample);
    // Grouped legend ?
}

//____________________________________________________________________

void HepDataMCPlot::Draw(Option_t *option) {
    //
    // Draw this object
    // - by default, two pads are drawn, one for the stack and one for
    //   the significance. Best results are obtained if drawn in a
    //   Canvas of specified height and width (with default config)
    //
    //               width      height
    //   No ratio:    700         500
    //    1 ratio:    700         633
    //    2 ratio:    700         765
    //
    //
    // Options available:
    //   "NODATA"  - don't draw data histogram (full blinding)
    //   "NOSTACK" - Overlay MC histograms rather than stack them
    //   "NOLEG"   - Do not draw a legend
    //   "NORM"    - Normalize the plots to Integral of 1 in case of "nostack" option. Use the option like hist->Draw("nostack,norm").
    //   !!!ATTENTION!!! The histograms are scaled after the option "norm". You have to close root and restart it to get again the unnormalized plots
    //
    // todo: add option partialblind, with some % as threshold to draw data point in bin

    // Convert option string
    TString opt = option;
    opt.ToLower();

    if ( gDebug > 0 )
        Info(__FUNCTION__, "Drawing with options \"%s\"", opt.Data());

    // Setup ATLAS style
    if ( fUseAtlasStyle ) SetAtlasStyle();

    // Option: Draw Data
    if ( opt.Contains("nodata") )
	fDrawData = kFALSE;
    else if ( fHistDATA == 0 ) {
        Warning(__FUNCTION__, "No Data histogram, turning off data.");
        fDrawData = kFALSE;
    }

    // Option: Unstacked?
    Bool_t nostack = opt.Contains("nostack");
    if ( nostack  || !fDrawData || fHistDATA == 0 ) {
	// No ratios in this case
	fDrawDataMCRatio  = kFALSE;
//	fDrawSignificance = kFALSE;
    }

    // Flag: Overflow/Underflow
    if ( fUseOverflowUnderflow ) {
	// DATA
	if ( fHistDATA != 0 )
	    AddOverflowUnderflowBins(fHistDATA);

	// MC
	TH1F *h_mc = 0;
	TIter next_hist(fMCHistStack->GetHists());
	while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) )
	    AddOverflowUnderflowBins(h_mc);

	// The stack needs modifications if it was already created
	// e.g. by rebinning or scaling the plot
	fMCHistStack->Modified();
    }


    // Flag: ATLAS, CMS, Lumi label
    // Add some more space in case of drawing ATLAS labels
    if ( fDrawCMSLumiAtlasLabel ) fAddRange = 1.3;

    // All options are set
    // --> now do the pad configuration
    SetupPad();

    // Clear all axis titles beside the one to draw
    TH1F *h_mc = 0;
    TIter next_hist(fMCHistStack->GetHists());

    // Remove all other titles
    while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
	   h_mc->SetXTitle("");
	   h_mc->SetYTitle("");
	   h_mc->SetLabelSize(0., "X");
    }

    // Stacked or unstacked
    if ( nostack ) {
        if ( gDebug > 0 )
            Info(__FUNCTION__, "Drawing unstacked");
	DrawNoStack(opt.Data());
	fDrawMCError = kFALSE; // no mc error needed
    } else {
        if (gDebug > 0 )
            Info(__FUNCTION__, "Drawing stacked");
	DrawStack(opt.Data());
    }

    // Draw ratio plot
    if ( fDrawDataMCRatio ) {
	DrawDataMCRatio();
    }

    // Draw significance plot
    if ( fDrawSignificance ) {
	DrawSignificance();
    }

    // Draw legend
    if ( !opt.Contains("noleg") ) {
	DrawLegend();
    }
    fCanvas->cd();

    // Draw Atlas label, CMS and Lumi
    if (fDrawCMSLumiAtlasLabel) {
	fMainPad->cd();
	DrawATLASLabel(fAtlasLabelPosX,
		       fAtlasLabelPosY,
		       fAtlasLabelStatus);
	if ( fUseShortCMSLumiLabel ) {
	    DrawTextLatex(fCMSEnergyLabelPosX,
			  fCMSEnergyLabelPosY,
			  1, "#sqrt{s} = "+fCenterOfMassEnergyLabel+" TeV, "+fLuminosityLabel);
	} else {
	    DrawTextLatex(fCMSEnergyLabelPosX,
			  fCMSEnergyLabelPosY,
			  1
			  ,"#sqrt{s} = "+fCenterOfMassEnergyLabel+" TeV");

	    DrawTextLatex(fLuminosityLabelPosX,
			  fLuminosityLabelPosY, 1,
			  "#int Ldt = "+fLuminosityLabel);
	}
    }

    // Setup Axis
    SetupAxis();

    // go back to the original pad / canvas
    fCanvas->cd();
    fCanvas->Update();

    if ( fFirstDraw ) fFirstDraw = kFALSE;
}

//____________________________________________________________________

TString HepDataMCPlot::GetPathInsideFile(TDirectory *dir) {
    //
    // Strip filesystem path from the full path of the given directory
    //
    TString path(dir->GetPath());
    if ( !path.Contains(":/") ) return "";
    TObjArray *tokens = path.Tokenize(":");
    if ( gDebug > 0 ) ::Info(__FUNCTION__,
                           "Reducing tokenized path %s", path.Data());
    TString PathInsideFile = dynamic_cast<TObjString*>(tokens->Last())->GetString().Copy();
    tokens->Delete();
    return PathInsideFile;
}

//____________________________________________________________________

void HepDataMCPlot::SetXTitle(const char* title) {
    //
    // Set title of x-axis
    //
    fXTitle->Remove(0);
    fXTitle->Append(title);
    if ( gPad != 0 ) {
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj = 0;
	while ( (obj = next()) ) {
	    TString objname(obj->GetName());
	    if ( objname.Contains(GetName()) ) {
		if ( obj->InheritsFrom("THStack") ) {
		    dynamic_cast<THStack*>(obj)->GetXaxis()->SetTitle(title);
		}
		if ( obj->InheritsFrom("TH1F") ) {
		    dynamic_cast<TH1F*>(obj)->SetXTitle(title);
		}
	    }
	}
	gPad->Modified();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetYTitle(const char* title) {
    //
    // Set title of y-axis
    //
    fYTitle->Remove(0);
    fYTitle->Append(title);
    if ( gPad != 0 ) {
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj = 0;
	while ( (obj = next()) ) {
	    TString objname(obj->GetName());
	    if ( objname.Contains(GetName()) ) {
		if ( obj->InheritsFrom("THStack") ) {
		    dynamic_cast<THStack*>(obj)->GetYaxis()->SetTitle(title);
		}
		if ( obj->InheritsFrom("TH1F") ) {
		    dynamic_cast<TH1F*>(obj)->SetYTitle(title);
		}
	    }
	}
	gPad->Modified();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetXRange(Double_t xmin, Double_t xmax) {
    //
    // Set range of x-axis
    //
    if ( fCanvas != 0 ) {
	if ( fMainPad != 0 )
	    SetXRange(fMainPad, xmin, xmax);
	if ( fRatioPad1 != 0 ) {
	    SetXRange(fRatioPad1, xmin, xmax);
	    // Update line in ratio
	    if ( fRatioLine != 0 ) {
		fRatioLine->SetX1(xmin);
		fRatioLine->SetX2(xmax);
	    }
	}
	if ( fRatioPad2 != 0 )
	    SetXRange(fRatioPad2, xmin, xmax);
	fCanvas->Modified();
	fCanvas->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetXRange(TPad *pad, Double_t xmin, Double_t xmax) {
    //
    // Set range of x-axis
    //
    TIter next(pad->GetListOfPrimitives());
    TObject *obj = 0;
    while ( (obj = next()) ) {
	if ( obj->InheritsFrom("THStack") ) {
	    dynamic_cast<THStack*>(obj)->GetXaxis()->SetRangeUser(xmin, xmax);
	}
	if ( obj->InheritsFrom("TH1") ) {
	    dynamic_cast<TH1*>(obj)->GetXaxis()->SetRangeUser(xmin, xmax);
	}
    }
    pad->Modified();
    pad->Update();
}

//____________________________________________________________________

void HepDataMCPlot::SetYRange(Double_t ymin, Double_t ymax) {
    //
    // Set range of y-axis
    //
    // Needs to be called before plot is drawn!
    //
    fYRangeUser = kTRUE;
    fYRangeUserMin = ymin;
    fYRangeUserMax = ymax;
}

//____________________________________________________________________

void HepDataMCPlot::SetYRangeRatio(Double_t ymin, Double_t ymax, Int_t ratio) {
    //
    // Set range of y-axis of ratio plot
    //
    // Needs to be called before plot is drawn!
    //
    if ( ratio == 1 && fRatioPad1 != 0
	 && (fDrawDataMCRatio || fDrawSignificance)) {
	fHistogramRatioPad1->GetYaxis()->SetRangeUser(ymin, ymax);
	fRatioPad1->Modified();
	fRatioPad1->Update();
    } else if ( ratio == 2 && fRatioPad2 != 0
		&& (fDrawDataMCRatio && fDrawSignificance)) {
	fHistogramRatioPad2->GetYaxis()->SetRangeUser(ymin, ymax);
	fRatioPad2->Modified();
	fRatioPad2->Update();
    } else {
	Error("SetYRangeRatio", "Ratio plot not available!");
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetNdivisionsX(Int_t n, Bool_t optim) {
    //
    // Set no. of X axis divisions
    //
    if ( gPad != 0 ) {
	fMCHistStack->GetXaxis()->SetNdivisions(n, optim);
	gPad->Modified();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetNdivisionsY(Int_t n, Bool_t optim) {
    //
    // Set no. of Y axis divisions
    //
    if ( gPad != 0 ) {
        TAxis *yaxis = fMCHistStack->GetYaxis();
        if ( yaxis == 0 ) {
            Warning(__FUNCTION__, "fMCHistStack has no Y-axis; probably wasn't drawn.");
            return;
        }
	yaxis->SetNdivisions(n, optim);
	gPad->Modified();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::Browse(TBrowser *b) {
    //
    // Browse plotter in TBrowser
    //
    Draw(b ? b->GetDrawOption() : "");
    gPad->Update();
}

//____________________________________________________________________

void HepDataMCPlot::DrawLegend() {
    //
    // Draw legend
    //
    // In case of gswitched on grouping of histograms only the group
    // entries but not the entries inside the groupe will be displayed
    //

    // Choose main pad to draw
    TPad* padsav = dynamic_cast<TPad*>(gPad);
    fMainPad->cd();

    // =============
    // Update legend
    // =============
    fLegend->Clear();

    // Adjust legend position
    // cout << fLegend->GetX1NDC() << endl;
    // cout << fLegend->GetX1NDC() + 0.05 - gStyle->GetPadRightMargin() << endl;
    // fLegend->SetX1( fLegend->GetX1NDC() + 0.05 - gStyle->GetPadRightMargin());
    // fLegend->SetX2( fLegend->GetX2NDC() + 0.05 - gStyle->GetPadRightMargin());

    fLegend->SetFillColor(0);
    fLegend->SetFillStyle(0);
    fLegend->SetLineColor(0);
    fLegend->SetBorderSize(0);

    // Add DATA
    if ( (fHistDATA != 0) && fDrawData ) {
	fLegend->AddEntry(fHistDATA, fHistDATA->GetTitle(), "P");
    }

    // In case of a unified MC stack do not specify the MC samples
    if ( fUnifyMCStack == kTRUE ) {
	fLegend->AddEntry(fHistMCTop, "MC", "F");
    } else {
	// Use this list to determine what the un-grouped MC histograms
	// are
	TList *hists_to_draw = new TList;
	hists_to_draw->AddAll(fMCHistStack->GetHists());

	// Add all grouped histograms
	TH1F *h_mc = 0;
	if ( fGroupHistograms == kTRUE ) {
	    TIter next_folder(fMCFolders, kIterBackward);
	    HepDataMCFolder *folder = 0;
            Bool_t is_first_folder = kTRUE;
	    while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
		HepDataMCSample *sample = 0;
		TIter next_sample(folder->GetMCSamples(), kIterBackward);
		while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
		    h_mc = sample->GetHistogram();
		    hists_to_draw->Remove(h_mc);
		    if ( sample == folder->GetMCSamples()->Last() ) {
			TString label = folder->GetTitle();
                        if ( fDrawSignalOverlay && is_first_folder
			     && fScaleOverlay != 1. ) {
			    // Change label in case of scaled overlay
			    label.Append(Form(" #times %-.0f", fScaleOverlay));
			}
                        // in case several legend entries have the same name,
                        // only add the first entry (here check only previous entry)
                        TLegendEntry *prev_entry = dynamic_cast<TLegendEntry*>(fLegend->GetListOfPrimitives()->Last());
                        if ( prev_entry != nullptr && label.EqualTo(prev_entry->GetLabel()) )  {
                            if ( gDebug > 0 ) {
                                Info(__FUNCTION__, "Not adding duplicate label entry with label %s", label.Data());
                            }
                        } else {
                            fLegend->AddEntry(h_mc, label, "F");
                        }
		    }
		}
                is_first_folder = kFALSE;
	    }
	}

	// Add the (remaining) un-grouped histograms
	TIter next_ungrouped_hist(hists_to_draw, kIterBackward);
	while ( (h_mc = dynamic_cast<TH1F*>(next_ungrouped_hist())) ) {
	    TString label = h_mc->GetTitle();
	    if ( fDrawSignalOverlay && h_mc == fHistogramLastInStack
		 && fScaleOverlay != 1. ) {
		// Change label in case of scaled overlay
	    	label.Append(Form(" #times %-.0f", fScaleOverlay));
	    }
	    fLegend->AddEntry(h_mc, label, "F");
	}
	delete hists_to_draw;
    }

    if ( fDrawMCError && fDrawNormError ) {
	fLegend->AddEntry(fMCErrors, "MC + Norm. Uncertainty", "f" );
    } else if ( fDrawMCError ) {
	fLegend->AddEntry(fMCErrors, "MC Uncertainty", "f" );
    }

    // Draw
    if ( fFirstLegend ) {
    	fFirstLegend = kFALSE;
    	fLegend->Paint();
    }
    fLegend->Draw();
    padsav->cd();
}

//____________________________________________________________________

void HepDataMCPlot::DrawLegendPad() {
    //
    // Draw enlarged legend clone into the current pad.
    // The pad is cleaned before
    //
    if ( fLegendLarge != 0 ) delete fLegendLarge;
    fLegendLarge = dynamic_cast<TLegend*>(fLegend->Clone());
    fLegendLarge->SetX1NDC(0.);
    fLegendLarge->SetX2NDC(1.);
    fLegendLarge->SetY1NDC(0.);
    fLegendLarge->SetY2NDC(1.);
    fLegendLarge->SetX1(0.);
    fLegendLarge->SetX2(1.);
    fLegendLarge->SetY1(0.);
    fLegendLarge->SetY2(1.);
    if ( gPad ) gPad->Clear();
    fLegendLarge->Draw();
    gPad->Modified();
    gPad->Update();
}

//____________________________________________________________________

void HepDataMCPlot::SetGroupHistograms(Bool_t GroupHistograms) {
    //
    // Switch grouping of histograms on or off
    //
    fGroupHistograms = GroupHistograms;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

Int_t HepDataMCPlot::DistancetoPrimitive(Int_t px, Int_t py) {
    //
    // Compute distance from point px,py
    //
    const Int_t kMaxDist = 10;
    Int_t dist = 99999;

    // Are we on the axis ?
    TH1 *h = fMCHistStack->GetHistogram();
    if ( h != 0 ) dist = h->DistancetoPrimitive(px, py);

    // Are we at the DATA histogram or the topmost MC histogram ?
    Int_t distt = 99999;
    if ( fHistDATA != 0 ) {
	distt = fHistDATA->DistancetoPrimitive(px, py);
	if ( distt < dist ) dist = distt;
    }
    h = dynamic_cast<TH1*>(fMCHistStack->GetStack()->Last());
    distt = h->DistancetoPrimitive(px, py);
    if ( distt < dist ) dist = distt;

    // Is this object being selected ?
    if ( dist < kMaxDist ) {
	gPad->SetSelected(this);
	gPad->SetCursor(kPointer);
    }

    return dist;
}

//____________________________________________________________________

void HepDataMCPlot::SetUnifyMCStack(Bool_t UnifyMCStack) {
    //
    // Draw all histograms of the MC stack
    //
    SetUnifyMCStack(UnifyMCStack, kTRUE);
}

//____________________________________________________________________

void HepDataMCPlot::SetUnifyMCStack(Bool_t UnifyMCStack, Bool_t DoRedraw) {
    //
    // Draw all histograms of the MC stack
    //
    fUnifyMCStack = UnifyMCStack;
    if ( DoRedraw == kTRUE ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawData(Bool_t DrawData) {
    //
    // Draw DATA histogram
    //
    fDrawData = DrawData;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawDataMCRatio(Bool_t DrawDataMCRatio) {
    //
    // Draw DATA MC ratio
    //
    fDrawDataMCRatio = DrawDataMCRatio;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawSignificance(Bool_t DrawSignificance) {
    //
    // Draw Significance
    //
    fDrawSignificance = DrawSignificance;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDataMCRatioCenterOne(Bool_t DataMCRatioCenterOne) {
    //
    // Draw Significance
    //
    fDataMCRatioCenterOne = DataMCRatioCenterOne;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawMCError(Bool_t DrawMCError) {
    //
    // Draw MC error band
    //
    SetDrawMCError(DrawMCError, kTRUE);
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawNormError(Bool_t DrawNormError) {
    //
    // Include normalisation uncertainties in error band
    //
    fDrawNormError = DrawNormError;
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawDataZeroEntryErrors(Bool_t DrawDataZeroEntryErrors) {
    //
    // Draw the Data error bars even for 0 data entry bins
    //
    fDrawDataZeroEntryErrors = DrawDataZeroEntryErrors;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawDataZeroEntries(Bool_t flag) {
    //
    // Draw the marker for bins with no data
    //
    fDrawDataZeroEntries = flag;
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawMCError(Bool_t DrawMCError, Bool_t DoRedraw) {
    //
    // Draw MC error band
    //
    fDrawMCError = DrawMCError;
    if ( DoRedraw == kTRUE ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetUseOverflowUnderflow(Bool_t UseOverflowUnderflow) {
    //
    // Add overflow to last bin and underflow to first bin
    //
    fUseOverflowUnderflow = UseOverflowUnderflow;
    if ( gPad ) {
     	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

TGraphAsymmErrors* HepDataMCPlot::SetErrors(TH1F *hist,
					    Bool_t IsData,
					    Option_t *option) {
    //
    // Internal helper function.
    //
    // Creates the graph for drawing (asymmetric) errors. For large
    // entries numbers the errors are taken from the given histogram
    // (as well as the xy-positions). For low numbers (<33) the errors
    // are estimated by the FeldmanCousins method.
    //
    TString opt = option;
    Double_t y = 0.;
    Double_t ey_lo = 0.;
    Double_t ey_up = 0.;

    Double_t stat_ey_lo = 0.;
    Double_t stat_ey_up = 0.;
    Double_t norm_ey_lo = 0.;
    Double_t norm_ey_up = 0.;

    // Create graph
    TGraphAsymmErrors *ErrorGraph = new TGraphAsymmErrors(hist->GetNbinsX());

    // Set graph positions and errors for each histogram bin
    // i   = number of the point of the graph (starts at 0)
    // bin = histogram bin numbers start at 1 ( --> i+1)
    for ( Int_t i = 0; i < hist->GetNbinsX(); i++ ) {
	Int_t bin = i+1;

	y = hist->GetBinContent(bin);
	if (opt.Contains("norm")) {
	    // the histogram is drawn normalized, but the histogram itself
	    // contains non-normalized entries
	    y = y/hist->GetSumOfWeights();
	}

	// Set each point: (bin-center, y)
	ErrorGraph->SetPoint(i, hist->GetBinCenter(bin), y);
	if ( IsData && !fDrawDataZeroEntries && y == 0 ) // move zero entries to -infinity
	    ErrorGraph->SetPoint(i, hist->GetBinCenter(bin), -9999.);


	// Compute the statistical uncertainty
	ComputeStatisticalUncertainty(hist, bin,
				      stat_ey_up,
				      stat_ey_lo);

	// Compute normalization uncertainty
	if ( !IsData && fDrawNormError ) { // MC only
	    ComputeNormUncertainty(bin,
				   norm_ey_up,
				   norm_ey_lo);
	}

	// sum of both errors:
	ey_up = TMath::Sqrt(stat_ey_up*stat_ey_up + norm_ey_up*norm_ey_up);
	ey_lo = TMath::Sqrt(stat_ey_lo*stat_ey_lo + norm_ey_lo*norm_ey_lo);

	// Only for data:
	// Draw error for empty bins?
	if (IsData && y == 0 && !fDrawDataZeroEntryErrors)
	    ey_up = 0;

	// scale the uncertainty of each point when
	// drawing normalized plots
	if ( opt.Contains("norm") ) {
	    ey_lo = ey_lo/hist->GetSumOfWeights();
	    ey_up = ey_up/hist->GetSumOfWeights();
	}

	// Draw x-error for data?
	if (IsData && !fDrawDataErrorX ) {
	    ErrorGraph->SetPointError(i,
				      0., 0.,
				      ey_lo, ey_up);
	} else {
	    ErrorGraph->SetPointError(i,
				      hist->GetBinWidth(bin)*0.5,
				      hist->GetBinWidth(bin)*0.5,
				      ey_lo, ey_up);
	}
    }
    return ErrorGraph;
}

//____________________________________________________________________

void HepDataMCPlot::ComputeStatisticalUncertainty(TH1F *h, Int_t bin,
						  Double_t &ey_up, Double_t &ey_lo) {
    //
    // Computes upper and lower errors for given number of events in
    // bin of histogram h.
    // For low numbers (<33) the errors
    // are estimated by the FeldmanCousins method.
    //
    Double_t y = h->GetBinContent(bin);

    if ( y < fgFeldmanCousinsMaxEntries ) {
	// Entries with QCDMM weights might be below zero
	// Take absolute value then...
	// Negative values should be avoided, rebin your histogram!
	if ( y < 0 )
	    y = TMath::Abs(y);

	// Use Feldman&Cousins for low entry numbers
    	ey_up = fgFCHigherCache[static_cast<Int_t>(y)] - y; if(ey_up<0) ey_up = y;
    	ey_lo = y - fgFCLowerCache[static_cast<Int_t>(y)];  if(ey_lo<0) ey_lo = y;
    } else {
	// Use Root's errors otherwise
	ey_up = h->GetBinError(bin);
	ey_lo = h->GetBinError(bin);
    }
}

//____________________________________________________________________

void HepDataMCPlot::ComputeNormUncertainty(Int_t bin,
					   Double_t &ey_up,
					   Double_t &ey_lo) {
    //
    // Compute the normalization uncertainty per bin
    //
    // For each grouped template an uncertainty must be provided
    //

    TList *hists_mc = GetListOfMCTemplates("err");
    Double_t rel_unc = 0.;
    HepDataMCFolder *folder;
    Double_t err2 = 0.;

    for ( Int_t i = 0; i < hists_mc->GetEntries(); i++ ) {
	// reset uncertainty to protect against single samples
	rel_unc = 0.;

	// Get number of events and compute uncertainty
	TH1F* h_mc = dynamic_cast<TH1F*>(hists_mc->At(i));

	if ( fMCFolders->GetEntries() > i ) {
	    // protect against single samples
	    // (the list of MC templates is synchronised
	    //  with the list of folders, single samples are last)
	    folder = dynamic_cast<HepDataMCFolder*>(fMCFolders->At(i));
	    rel_unc = folder->GetNormUncertainty();
	}
	err2 += TMath::Power(rel_unc*h_mc->GetBinContent(bin),2);
    }
    ey_up = TMath::Sqrt(err2);
    ey_lo = ey_up; // error is symmetric

    hists_mc->Delete();
    delete hists_mc;
}

//____________________________________________________________________

void HepDataMCPlot::PrepareFit(Bool_t single_samples, Bool_t prepare_effs) {
    //
    // Prepares fit using TFractionFitter
    //
    // HepTemplate objects are created from the list of MC samples.
    // Per default, sample groups are treated as one template.
    // Set single_samples to TRUE, if you want to fit each single
    // sample.
    // If prepare_effs is set to TRUE, cut efficiencies are calculated
    // and assigned to the template objects. For that, it is assumed
    // that the number of entries in the fit histogram is equal to the
    // number of events after all cuts applied. The denominator for the
    // efficiencies is taken from the event info histogram. In case of
    // folders a weighted sum is calculated. The efficiencies are probably
    // needed to calculate cross sections from the fit results.
    //

    //Check for data hist
    if(fHistDATA==0) {
	Error("PrepareFit", "Plot contains no data histogram. Fit is not possible.");
	gSystem->Abort(-1);
    }

    if(fMCFitTemplates==0) fMCFitTemplates = new TList();
    Float_t eff = 0;
    Float_t denom = 0;
    Float_t weight = 0;
    if (single_samples) {
	Info("PrepareFit", "Creating templates for single samples.");
	//Loop over folders and generate templates for each sample of them
	TIter next_folder(fMCFolders);
	HepDataMCFolder *folder = 0;
	while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	    TIter next_sample(folder->GetMCSamples());
	    HepDataMCSample *sample = 0;
	    while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
		HepTemplate* temp = new HepTemplate();
		temp->SetName(gSystem->BaseName(sample->GetName()));
		temp->SetInputHistogram(sample->GetHistogram());
		temp->SetColor(sample->GetColor());
		if(prepare_effs) {
		    if(sample->GetNEvents()>0)
			eff = sample->GetHistogram()->Integral()/(sample->GetNEvents()*fLumiDATA/sample->GetLumi());
		    else eff = 0;
		    temp->SetCutEfficiency(eff);
		}
		fMCFitTemplates->Add(temp);
		Info("PrepareFit", "Added template: %s", temp->GetName());
	    }
	}

	//Loop over all single samples, not belonging to a folder
	TIter next_singlesample(fMCSingleSamples);
	HepDataMCSample *single_sample = 0;
	while ( (single_sample = dynamic_cast<HepDataMCSample*>(next_singlesample())) ) {
	    HepTemplate* temp = new HepTemplate();
	    //eff = ((TH1F*)single_sample->GetHistogram())->Integral()/single_sample->GetNEvents();
	    temp->SetName(gSystem->BaseName(single_sample->GetName()));
	    temp->SetInputHistogram(single_sample->GetHistogram());
	    temp->SetColor(single_sample->GetColor());
	    if(prepare_effs) {
		if(single_sample->GetNEvents()>0)
		    eff = single_sample->GetHistogram()->Integral()/(single_sample->GetNEvents()*fLumiDATA/single_sample->GetLumi());
		else eff = 0;
		temp->SetCutEfficiency(eff);
	    }
	    fMCFitTemplates->Add(temp);
	    Info("PrepareFit", "Added template: %s", temp->GetName());
	}
    }
    else {
	Info("PrepareFit", "Creating templates using folder structure.");
	//Loop over folders and generate template for each folder
	TIter next_folder(fMCFolders);
	HepDataMCFolder *folder = 0;
	while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	    TIter next_sample(folder->GetMCSamples());
	    HepTemplate* temp = new HepTemplate();
	    HepDataMCSample *sample = 0;
	    sample = dynamic_cast<HepDataMCSample*>(next_sample());
	    if(sample==0) continue; //Next folder in case of empty folder
	    TH1F *h_folder = dynamic_cast<TH1F*>(sample->GetHistogram()->Clone());
	    temp->SetName(folder->GetName());
	    temp->SetColor(folder->GetColor());
	    if(prepare_effs) {
		eff = 0;
		denom = 0;
		weight = 0;
	    }
	    while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
		h_folder->Add(sample->GetHistogram());
		if(prepare_effs) {
		    denom = denom + sample->GetHistogram()->Integral()*fLumiDATA/sample->GetLumi();
		}
	    }
	    temp->SetInputHistogram(h_folder);
	    if(prepare_effs) {
		TIter next_sample2(folder->GetMCSamples());
		while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample2())) ) {
		    weight = sample->GetHistogram()->Integral()*fLumiDATA/(sample->GetLumi()*denom);
		    eff = eff + weight * sample->GetHistogram()->Integral()/(sample->GetNEvents()*fLumiDATA/sample->GetLumi());
		}
		temp->SetCutEfficiency(eff);
	    }
	    fMCFitTemplates->Add(temp);
	    Info("PrepareFit", "Added template: %s", temp->GetName());
	}

	//Loop over all single samples, not belonging to a folder
	TIter next_singlesample(fMCSingleSamples);
	HepDataMCSample *single_sample = 0;
	while ( (single_sample = dynamic_cast<HepDataMCSample*>(next_singlesample())) ) {
	    HepTemplate* temp = new HepTemplate();
	    temp->SetName(gSystem->BaseName(single_sample->GetName()));
	    temp->SetTitle(gSystem->BaseName(single_sample->GetName()));
	    temp->SetInputHistogram(single_sample->GetHistogram());
	    temp->SetColor(single_sample->GetColor());
	    if(prepare_effs) {
		if(single_sample->GetNEvents()>0)
		    eff = single_sample->GetHistogram()->Integral()/(single_sample->GetNEvents()*fLumiDATA/single_sample->GetLumi());
		else eff = 0;
		temp->SetCutEfficiency(eff);
	    }
	    fMCFitTemplates->Add(temp);
	    Info("PrepareFit", "Added template: %s",temp->GetName());
	}
    }

    Info("PrepareFit", "Created %d template objects.",fMCFitTemplates->GetEntries());
    cout << endl;
}

//____________________________________________________________________

Int_t HepDataMCPlot::Fit() {
    //
    // Perform the fit
    //
    // Create TFractionFitter instance. Start values and constraints
    // given to template objects are set and the fit is started.
    // Returns the return value from TFractionFitter.
    //

    // Preparing Array with template-histograms
    if(fMCFitTemplates==0) {
	Info("Fit", "Empty template list. Calling PrepareFit() function.");
	PrepareFit();
    }
    if(fMCFitTemplates->GetEntries()==0) {
	Info("Fit", "Template list has 0 entries. Empty plot? Calling PrepareFit() function.");
	PrepareFit();
	if(fMCFitTemplates->GetEntries()==0) {
	    Error("Fit", "Still no templates. Assuming that plot is empty. Abort !!!");
	    return -1;
	}
    }
    TObjArray *mc = new TObjArray(fMCFitTemplates->GetEntries());
    HepTemplate* templ = 0;
    TH1F *templ_hist = 0;
    //Temporary template list, containing all templates used for the fit:
    TList *templ_list = new TList();
    for ( Int_t i = 0; i < fMCFitTemplates->GetEntries(); i++){
	templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->At(i));
	templ_hist = templ->GetInputHistogram();
        if(templ_hist->GetEntries()>0) {
	    mc->Add(templ_hist);
	    templ_list->Add(templ);
	}
    }

    // Initialization and configuration of FractionFitter
    fFitter = new TFractionFitter(fHistDATA, mc);
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,99,0))
    TVirtualFitter * fVFitter = fFitter->GetFitter();
#endif

    //Set start values and constraints
    for ( Int_t i = 0; i < templ_list->GetEntries(); i++ ){
        templ = dynamic_cast<HepTemplate*>(templ_list->At(i));
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,99,0))
	if ( templ->HasStartValue() ) {
	    fVFitter->SetParameter(i,templ->GetName(),templ->GetStartValue(),templ->GetStartValueError(),
				   templ->GetLowerBound(),templ->GetUpperBound());
	}
        else if ( !templ->HasStartValue() && templ->IsBound() ){
            fFitter->Constrain( i ,templ->GetLowerBound(), templ->GetUpperBound());
        }
#else
        ROOT::Fit::ParameterSettings & par = fFitter->GetFitter()->Config().ParSettings(i);
        par.SetName(templ->GetName());
	if ( templ->HasStartValue() ) {
           par.SetValue(templ->GetStartValue());
           par.SetStepSize(templ->GetStartValueError());
        }
        if ( templ->IsBound() ) {
           par.SetLowerLimit(templ->GetLowerBound());
           par.SetUpperLimit(templ->GetUpperBound());
        }
#endif
    }

    // Performing Fit
    cout << endl;
    cout << "===============================================" << endl;
    Info("Fit", "Starting fit procedure !!!");
    cout << "===============================================" << endl;
    cout << endl;
    Int_t status = fFitter->Fit();
    cout << endl;

    // Gathering results
    if (status == 0) {
	cout << "===============================================" << endl;
	Info("Fit", "Fitter converged !!!");
	cout << "===============================================" << endl;
	cout << endl;
	//Store obtained fractions and histograms in template objects.
	//Create final stacked plot.
	Double_t fraction, error;
	TH1F* result = dynamic_cast<TH1F*>(fFitter->GetPlot());
	fMCFittedHistStack = new THStack(Form("%s_stacked",result->GetName()),
					 Form("%s (stacked)", result->GetTitle()));
	for (Int_t i = 0; i < templ_list->GetEntries(); ++i ) {
	    templ = dynamic_cast<HepTemplate*>(templ_list->At(i));
	    fFitter->GetResult(i, fraction, error);
	    templ->SetFitFraction(fraction);
	    templ->SetFitFractionError(error);
	    templ_hist = dynamic_cast<TH1F*>(fFitter->GetMCPrediction(i));
	    templ_hist->SetName(Form("%s_fitted", templ->GetInputHistogram()->GetName()));
	    templ_hist->SetTitle(Form("%s (fitted), fraction = %f, uncertainty = %f",
				      templ->GetInputHistogram()->GetTitle(),
				      fraction, error));
	    templ->SetFittedHistogram(templ_hist);

	    TH1F* stack_hist = dynamic_cast<TH1F*>(templ_hist->Clone());
	    stack_hist->SetFillColor(templ->GetColor());
	    stack_hist->Scale(result->Integral()/stack_hist->Integral()*fraction);
	    fMCFittedHistStack->Add(stack_hist);
	}

	//delete stack_hist;
    }
    else {
	cout << endl;
	Error("Fit", "Fitter failed with status code %d !!!", status);
	return status;
    }



    delete templ_list;
    //delete templ_hist;
    //delete templ;

    return 0;
}

//____________________________________________________________________

void HepDataMCPlot::DrawFit(Option_t *option) {
    //
    // Draw fitted MC histogram stack together with data
    //
    TString opt = option;
    opt.ToLower();

    fHistDATA->Draw("Ep");
    fMCFittedHistStack->Draw("hist,same");
    fHistDATA->Draw("Epsame");

    //Legend!!!
    if ( !opt.Contains("noleg") ) {
	DrawLegend();
    }
}

//____________________________________________________________________

void HepDataMCPlot::ListFolders() {
    cout << endl;
    Info("ListFolders", "Listing names of HepDataMCFolder objects:");
    for(Int_t i = 0; i < fMCFolders->GetEntries(); i++) {
	Info("ListFolders", "%s", gSystem->BaseName(fMCFolders->At(i)->GetName()));
    }
    cout << endl;
}

//____________________________________________________________________

void HepDataMCPlot::ListSingleSamples() {
    cout << endl;
    Info("ListSingleSamples", "Listing names of HepDataMCSample single sample objects:");
    for(Int_t i = 0; i < fMCSingleSamples->GetEntries(); i++) {
	Info("ListSingleSamples", "%s", gSystem->BaseName(fMCSingleSamples->At(i)->GetName()));
    }
    cout << endl;
}

//____________________________________________________________________

void HepDataMCPlot::ListTemplateFractions() {
    cout << endl;
    Info("ListTemplateFractions", "Listing fractions and errors of all templates:");
    cout << "Template name \t\t\t\t" << " Fraction \t " << " Fraction error" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    HepTemplate *templ = 0;
    TString name = "";
    TString tab = "";
    Float_t frac = 0;
    for(Int_t i = 0; i < fMCFitTemplates->GetEntries(); i++) {
	templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->At(i));
	name = templ->GetName();
	frac = templ->GetFitFraction();
	if(name.Length()<9) name = Form("%s\t\t\t\t",name.Data());
	else if(name.Length()>8 && name.Length()<17) name = Form("%s\t\t\t",name.Data());
	else if(name.Length()>16 && name.Length()<25) name = Form("%s\t\t",name.Data());
	else if(name.Length()>24 && name.Length()<33) name = Form("%s\t",name.Data());
	if(frac==0) tab = " \t\t  ";
	else tab = " \t  ";
	cout << name << " \t " << frac
	     << tab << templ->GetFitFractionError() << endl;
    }
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << endl;
}

//____________________________________________________________________

Double_t HepDataMCPlot::GetTemplateFitFraction(const char* template_name) {
    //
    // Get template fraction for specific template
    //
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) return templ->GetFitFraction();
    else {
	Error("GetTemplateFitFraction", "Could not find template with name %s.", template_name);
	return -1;
    }
}

//____________________________________________________________________

Double_t HepDataMCPlot::GetTemplateFitFractionError(const char* template_name) {
    //
    // Get template fraction error for specific template
    //
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) return templ->GetFitFractionError();
    else {
	Error("GetTemplateFitFractionError", "Could not find template with name %s.", template_name);
	return -1;
    }
}

//____________________________________________________________________

Float_t HepDataMCPlot::GetTemplateCutEfficiency(const char* template_name) {
    //
    // Get template cut efficiency for specific template
    //
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) return templ->GetCutEfficiency();
    else {
	Error("GetTemplateCutEfficiency", "Could not find template with name %s.", template_name);
	return -1;
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetTemplateBounds(const char* template_name, Double_t Lower, Double_t Upper) {
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) templ->SetBounds(Lower, Upper);
    else {
	Error("SetTemplateBounds", "Could not find template with name %s.", template_name);
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetTemplateStartValue(const char* template_name, Double_t frac, Double_t frac_err) {
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) templ->SetStartValue(frac, frac_err);
    else {
	Error("SetTemplateStartValue", "Could not find template with name %s.", template_name);
    }
}

//____________________________________________________________________

void HepDataMCPlot::FixTemplateFraction(const char* template_name, Bool_t fix) {
    HepTemplate *templ = dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(template_name));
    if(templ!=0) templ->FixFraction(fix);
    else {
	Error("FixTemplateFraction", "Could not find template with name %s.", template_name);
    }
}

//____________________________________________________________________

void HepDataMCPlot::Rebin(Int_t ngroup) {
    //
    // Performs a rebinning of the stacked plot.
    // Rebinning is applied to all histograms
    // (data and MC).
    //
    TH1F *h_mc = 0;

    // Rebin individual histograms
    TIter next_hist(fMCHistStack->GetHists());
    while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
	h_mc->Rebin(ngroup);
    }
    TObjArray *arr = 0;
    arr = fMCHistStack->GetStack();
    for ( Int_t i = 0; i < arr->GetEntries(); i++ ) {
	h_mc = dynamic_cast<TH1F*>(arr->At(i));
	h_mc->Rebin(ngroup);
    }
    fMCHistStack->Modified();
    if ( fHistogramLastInStack != 0 ) fHistogramLastInStack->Rebin(ngroup);

    // Rebin Data
    if( fHistDATA!=0 ) fHistDATA->Rebin(ngroup);

    // Redraw
    if ( gPad ) {
	Draw();
	gPad->Update();
    }
}

//____________________________________________________________________

void HepDataMCPlot::ScaleMCStack(Float_t scale) {
    //
    // Scales the complete MC hist stack by the given scale
    //

    TH1F *h_mc = 0;

    TIter next_hist(fMCHistStack->GetHists());
    while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
	h_mc->Scale(scale);
    }

    fHistMCTop->Scale(scale);

    TObjArray *arr = 0;
    arr = fMCHistStack->GetStack();
    for (Int_t i = 0; i < arr->GetEntries(); i++) {
	h_mc = dynamic_cast<TH1F*>(arr->At(i));
	h_mc->Scale(scale);
    }

    if ( gPad ) {
	Draw();
	gPad->Update();
    }

}

//____________________________________________________________________

void HepDataMCPlot::ScaleMCFolder(Float_t scale, const char* mc_folder) {
    //
    // Scales only a group of histograms by the given scale
    //

    TString FolderToScale = mc_folder;

    TH1F *h_mc = 0;

    // Loop over folders
    TIter next_folder(fMCFolders);
    HepDataMCFolder *folder = 0;
    while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {

	if ( !(FolderToScale.Contains(folder->GetName())) )
	    continue;

	TIter next_sample(folder->GetMCSamples());
	HepDataMCSample *sample = 0;

	while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
	    h_mc  = sample->GetHistogram();
            if ( h_mc == nullptr ) {
                Warning(__FUNCTION__, "Histogram for sample %s is NULL", sample->GetName());
                continue;
            }
	    h_mc->Scale(scale);
	}
    }
    fMCHistStack->Modified();
}

//____________________________________________________________________

void HepDataMCPlot::DrawATLASLabel(Double_t x,Double_t y,
				   TString text,Color_t color) {
    //
    // Draw ATLAS + label
    //
    // Functions taken from official ATLAS style
    //

    // Adjust position in case user adds more space to the right margin
    // x += 0.05 - gStyle->GetPadRightMargin();

    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize);
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);

    // for some reason it is not possible to get the
    // TLatex Xsize in NDC (although you ask for it)
    // --> Solution hardcode the numbers for the default Canvas
    //     (this was already done in the ATLAS Style for no ratios)
    double delx = 0.;
    if ( fDrawDataMCRatio && fDrawSignificance )
	delx = 0.115*696*gPad->GetWh()/(737*gPad->GetWw());
    else if ( fDrawDataMCRatio || fDrawSignificance )
	delx = 0.115*696*gPad->GetWh()/(604*gPad->GetWw());
    else
	delx = 0.115*696*gPad->GetWh()/(472*gPad->GetWw());

    l.DrawLatex(x,y,"ATLAS");
    if (text.Data()) {
	TLatex p;
	p.SetNDC();
	p.SetTextFont(42);
	p.SetTextColor(color);
	p.DrawLatex(x+delx,y,text.Data());
    }
}

//____________________________________________________________________

void HepDataMCPlot::DrawTextLatex(Double_t x,Double_t y,
				  Color_t color,TString text,
				  Double_t tsize) {
    //
    // Draw latex text on given position
    //
    // Functions taken from official ATLAS style
    //

    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize);
    l.SetNDC();
    l.SetTextColor(color);
    l.SetTextFont(42);
    l.SetTextSize(tsize);
    l.DrawLatex(x,y,text.Data());
}

//____________________________________________________________________

void HepDataMCPlot::SetAtlasStyle(){
    //
    // set current style to ATLASstyle
    //

    TStyle *AtlasStyle = gROOT->GetStyle("ATLAS");
    if ( AtlasStyle == 0 ) {
	Warning("SetAtlasStyle", "Could not find ATLAS Style! Using current style.\nPlease load the ATLAS Style in your .rootlogon.C!");
    } else {
	// Set Atlas Style
	gROOT->SetStyle("ATLAS");
	//
	gROOT->ForceStyle();
    }
}

//____________________________________________________________________

void HepDataMCPlot::Print(Option_t *option) const {
    //
    // Print event yield table
    //
    // Options available:
    //   "SPLIT" - Print table for all samples. If this option is
    //             not given, only the grouped sample will be
    //             listed (default).
    //
    //   "LATEX" - Print table as latex tabular. If this option is
    //             not given, a simple ASCII table is printed (default).
    //             Import of the latex table requires the siunitx package.
    //
    TString opt = option;
    opt.ToUpper();

    // "CUTFLOW" - Print yields after the cuts in the cutflow tool
    if(opt.Contains("CUTFLOW")){
		TIter next(GetMCHistStack()->GetHists());
		TH1F *h_mc = 0;
		while ( (h_mc = dynamic_cast<TH1F*>(next())) ) {
			cout<<h_mc->GetTitle()<<endl;
			float scale = 1;
			if(  ((TString) h_mc->GetTitle()).Contains("410658tchanlt")) scale = 0.00899393;
			if(  ((TString) h_mc->GetTitle()).Contains("410470ttbar")) scale = 0.000187816 ; // / 2.720128786 ;
			if(  ((TString) h_mc->GetTitle()).Contains("410646Wt")) scale = 0.00684453;
			for(int bin = 0; bin<h_mc->GetNbinsX();bin++ ){

				cout<<h_mc->GetXaxis()->GetBinLabel(bin)<<" ";
				cout<<h_mc->GetBinContent(bin) / scale <<endl; //tchan Scale: 0.00899393, ttbar : 0.000187816
			}



		}
    }
    TString hline, hdline;
    if ( opt.Contains("LATEX") ) {
        hline = "\\hline";
        hdline = "\\hline\\hline";
    } else {
        hline = "---------------------------";
        hdline = "===========================";
    }

    cout << endl;

    if ( opt.Contains("LATEX") )
        cout << "\\begin{tabular}{|l|c|}" << endl;

    cout << hdline.Data() << endl;
    if ( opt.Contains("LATEX") )
        cout << "Process & Events \\\\" << endl;
    else
        cout << "Process         Events     " << endl;
    cout << hdline.Data() << endl;

    // Signal events (signal == Samples from last folder in stack!)
    Float_t n_signal = 0.;
    Float_t err2_signal = 0.;

    // MC list
    Float_t n_totmc = 0.;
    Float_t totmcerr2 = 0.;
    Double_t err  = 0.;
    if ( opt.Contains("SPLIT") ) {
	TIter next(GetMCHistStack()->GetHists(), kIterBackward);
	TH1F *h_mc = 0;
	Float_t n_mc = 0.;
	while ( (h_mc = dynamic_cast<TH1F*>(next())) ) {
	    Int_t first = h_mc->GetXaxis()->GetFirst() - 1; // overflow underflow
	    Int_t last  = h_mc->GetXaxis()->GetLast()  + 1;
	    n_mc = h_mc->IntegralAndError(first, last, err);
	    if ( opt.Contains("LATEX") ) {
		printf("\\text{%-16s} & $\\num{%f} \\pm \\num{%f}$\\\\\n",
//		printf("%-16s & %-16f & %-16f\n",
		       h_mc->GetTitle(),
		       n_mc, err);
	    } else {
		printf("%-16s%-.f ± %-.f\n",
		       h_mc->GetTitle(),
		       n_mc, err);
	    }
	    n_totmc += n_mc;
	    totmcerr2 += err*err;
	}
    } else {
	TIter next_folder(GetMCFolders(), kIterBackward);
	HepDataMCFolder *folder = 0;
	TH1F *h_mc = 0;
	Bool_t is_first_folder = kTRUE; // actually last in GetMCFolders()
	while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	    Float_t err2 = 0.;
	    Float_t n_mc = 0.;
	    TIter next_sample(folder->GetMCSamples());
	    HepDataMCSample *sample = 0;
	    while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
		h_mc = sample->GetHistogram();
                if ( nullptr == h_mc ) {
                    Warning(__FUNCTION__, "No histogram found for sample %s",
                            sample->GetName());
                    continue;
                }
		Int_t first = h_mc->GetXaxis()->GetFirst() - 1;
		Int_t last  = h_mc->GetXaxis()->GetLast()  + 1;
		n_mc += h_mc->IntegralAndError(first, last, err);
		err2 += err*err;
		totmcerr2 += err*err;

		if ( is_first_folder ){
		    err2_signal = err2;
		    n_signal = n_mc;
		}
	    }
	    is_first_folder = kFALSE;
	    if ( opt.Contains("LATEX") ) {
		printf("\\text{%-16s} & $\\num{%f} \\pm \\num{%f}$\\\\\n",
		       folder->GetName(),
		       n_mc, TMath::Sqrt(err2));
	    } else {
		printf("%-16s%-.f +- %-.f\n",
		       folder->GetName(),
		       n_mc, TMath::Sqrt(err2));
	    }
	    n_totmc += n_mc;
	}
    }

    // MC total
    cout << hline.Data() << endl;
    if ( opt.Contains("LATEX") ) {
	printf("\\text{MC total}        & $\\num{%f} \\pm \\num{%f}$\\\\\n",
	       n_totmc, TMath::Sqrt(totmcerr2));
    } else {
 	printf("MC total        %-.f +- %-.f\n",
	       n_totmc, TMath::Sqrt(totmcerr2));
    }

    // Data
    if ( GetHistDATA() != 0 ) {
        cout << hline.Data() << endl;
	Int_t first = GetHistDATA()->GetXaxis()->GetFirst() - 1;
	Int_t last  = GetHistDATA()->GetXaxis()->GetLast()  + 1;
	Float_t n_data = GetHistDATA()->IntegralAndError(first, last, err);
	if ( opt.Contains("LATEX") ) {
	    printf("\\text{DATA}            & $\\num{%f} \\pm \\num{%f}$\\\\\n",
		   n_data, TMath::Sqrt(n_data));
	} else {
	    printf("DATA            %-.f +- %-.f\n",
		   n_data, TMath::Sqrt(n_data));
	}
    }
    cout << hdline.Data() << endl;

    if ( !opt.Contains("SPLIT") ) {
	// only available if not using split mode
	// use MC+QCD as estimate for background (B)
	// if used for optimizing cuts, the data should not be used
        if ( opt.Contains("LATEX") ) {
            printf("$S/B$         & %1.4f\\\\\n", n_signal/(n_totmc-n_signal));
            printf("$S/\\sqrt{B}$  & %1.4f\\\\\n", n_signal/TMath::Sqrt(totmcerr2-err2_signal));
        } else {
            printf("S/B             %1.4f\n", n_signal/(n_totmc-n_signal));
            printf("S/sqrt(B)       %1.4f\n", n_signal/TMath::Sqrt(totmcerr2-err2_signal));
        }
        cout << hdline.Data() << endl;
    }
    if ( opt.Contains("LATEX") ) {
        cout << "\\end{tabular}" << endl;
    }
    cout << endl;
}

//____________________________________________________________________

void HepDataMCPlot::AddOverflowUnderflowBins(TH1* hist) {
    //
    // Add overflow/underflow bin to first/last bin
    //

    Double_t underflow     = hist->GetBinContent(0);
    Double_t underflow_err = hist->GetBinError(0);

    Double_t first     = hist->GetBinContent(1);
    Double_t first_err = hist->GetBinError(1);

    // Set bin content and error for first/underflow bin
    hist->SetBinContent(0., 0.);
    hist->SetBinError(0., 0.);

    hist->SetBinContent(1, underflow + first);
    hist->SetBinError(1, TMath::Sqrt(underflow_err*underflow_err+
				     first_err*first_err));

    Int_t maxbin = hist->GetNbinsX();

    Double_t overflow     = hist->GetBinContent(maxbin+1);
    Double_t overflow_err = hist->GetBinError(maxbin+1);

    Double_t last     = hist->GetBinContent(maxbin);
    Double_t last_err = hist->GetBinError(maxbin);

    // Set bin content and error for first/underflow bin
    hist->SetBinContent(maxbin, last + overflow);
    hist->SetBinError(maxbin, TMath::Sqrt(last_err*last_err +
					  overflow_err*overflow_err));

    hist->SetBinContent(maxbin+1, 0.);
    hist->SetBinError(maxbin+1, 0.);
}

//____________________________________________________________________

void HepDataMCPlot::DrawNoStack(Option_t *option) {
    //
    // ==================================
    // Draw MC histograms in overlay mode
    // ==================================
    //
    // The fill colour is used as histogram line colour, but
    // the histogram itself is not filled.
    //
    // Options available:
    //   "NORM"    - normalize histograms to 1
    //

    // Convert option string
    TString opt = option;
    opt.ToLower();

    // Choose main pad to draw
    TPad *padsav = dynamic_cast<TPad*>(gPad);
    fMainPad->Draw();
    fMainPad->cd();

    // In overlay mode (non-stacked mode) do not draw data points
    fDrawData = kFALSE;

    // Collect all histograms that need to be drawn
    TList *hists_to_draw = new TList;
    hists_to_draw->AddAll(fMCHistStack->GetHists());

    // Setup colors
    SetColorsNoStack();

    TH1F *h_mc = 0;

    // Create grouped histograms if neccessary
    if ( fGroupHistograms ) {

	// Loop over folders
	TIter next_folder(fMCFolders);
	HepDataMCFolder *folder = 0;
	while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	    TIter next_sample(folder->GetMCSamples());
	    HepDataMCSample *sample = 0;

	    TH1F *h_mcgroup = 0;
	    while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
		h_mc  = sample->GetHistogram();
		hists_to_draw->Remove(h_mc);

		// Clone first histogram to be used for the group
		// otherwise add all samples to existing group histogram
		if ( h_mcgroup == 0 ) {
		    h_mcgroup = dynamic_cast<TH1F*>(h_mc->Clone());
		    h_mcgroup->SetLineColor(folder->GetAttFill().GetFillColor());
		} else {
		    h_mcgroup->Add(h_mc);
		}
	    }

	    // Add the grouped histogram to the list
	    hists_to_draw->Add(h_mcgroup);
	}
    }

    // Find maximum bin content
    TIter next_hist(hists_to_draw);
    Double_t max_mc = 0.;
    while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
	// Maximum is the same, also for normalized drawing
	// DrawNormalized() takes care of setting the correct maximum
	if( max_mc < h_mc->GetBinContent(h_mc->GetMaximumBin()) )
	    max_mc = h_mc->GetBinContent(h_mc->GetMaximumBin());
    }
    // Add extra space to maximum
    max_mc *= fAddRange;

    // Draw all histograms in the list
    next_hist.Reset();
    Bool_t first = kTRUE;
    while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
	// Set axis range
	if ( fYRangeUser ) {
	    max_mc = fYRangeUserMax;
	    h_mc->SetMinimum(fYRangeUserMin);
	}
	h_mc->SetMaximum(max_mc);

	if ( first ) {
	    first = kFALSE;
	    if ( opt.Contains("norm") ) {
		if ( h_mc->Integral() == 0 ) {
		    Warning("DrawNoStack",
			    "Integral is 0. Drawing of histogram %s not possible.",
			    h_mc->GetTitle());
		    first = kTRUE;
		} else {
                    h_mc->Scale(1./h_mc->Integral());
                    h_mc->Draw("hist");
                    fHistogramMainPad = h_mc;
		}
	    } else {
		h_mc->Draw("hist");
		fHistogramMainPad = h_mc;
	    }
	} else {
	    if ( opt.Contains("norm") )
		if ( h_mc->Integral() == 0 ) {
		    Warning("DrawNoStack",
			    "Integral is 0. Drawing of histogram %s not possible.",
			    h_mc->GetTitle());
		} else {
                    h_mc->Scale(1./h_mc->Integral());
                    h_mc->Draw("hist,same");
		}
	    else
		h_mc->Draw("hist,same");
	}

    }
        // apply user range to rescaled histograms
        if ( fYRangeUser ) {
            fHistogramMainPad->SetMinimum(fYRangeUserMin);
            fHistogramMainPad->SetMaximum(fYRangeUserMax);
        }

    delete hists_to_draw;
    padsav->cd();
}

//____________________________________________________________________

void HepDataMCPlot::SetColorsNoStack() {
    //
    // Setup color scheme for non-stacked mode
    //

    // Use this list to determine the un-grouped histograms
    TList *hists_to_draw = new TList;
    hists_to_draw->AddAll(fMCHistStack->GetHists());

    // Loop over folders
    TIter next_folder(fMCFolders);
    TH1F *h_mc = 0;
    HepDataMCFolder *folder = 0;
    while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
    	TIter next_sample(folder->GetMCSamples());
    	HepDataMCSample *sample = 0;

    	while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
    	    h_mc  = sample->GetHistogram();
    	    hists_to_draw->Remove(h_mc);

    	    h_mc->SetFillStyle(0);
    	    h_mc->SetLineColor(sample->GetAttFill().GetFillColor());
    	    h_mc->SetLineWidth(gStyle->GetHistLineWidth());
    	    h_mc->SetLineStyle(gStyle->GetHistLineStyle());
    	}
    }

    TIter next_ungrouped_hist(hists_to_draw);
    while ( (h_mc = dynamic_cast<TH1F*>(next_ungrouped_hist())) ) {
    	h_mc->SetFillStyle(0);
	// Soren: Assume that the line color is already set correctly?
    	// h_mc->SetLineColor(gStyle->GetHistLineColor());
	h_mc->SetLineWidth(gStyle->GetHistLineWidth());
	h_mc->SetLineStyle(gStyle->GetHistLineStyle());
    }
    delete hists_to_draw;
}

//____________________________________________________________________

void HepDataMCPlot::ExportTemplates(TFile *f, const char* suffix) {
    //
    // Saves group-histograms and single sample histograms in a
    // given file.
    //
    // The name of the histograms consists of the folder/sample name
    // and a suffix, e.g. "ttbar_jesup"
    //

    // Check if file is writable:
    if ( !f->IsWritable() ) {
	Error("SaveTemplates",
	      "File %s is not writable! Abort!",
	      f->GetName());
	gSystem->Abort(0);
    }
    f->cd();

    TList *hists_to_save = GetListOfMCTemplates(suffix);
    TH1F *h = 0;
    TIter next_hist(hists_to_save);
    while ( (h = dynamic_cast<TH1F*>(next_hist())) ) {
	h->SetDirectory(f);
    }

    // Export data histogram
    if ( fHistDATA ) {
	TH1F *h_data = dynamic_cast<TH1F*>(
            fHistDATA->Clone(Form("%s_%s", "data", suffix)));
	h_data->SetDirectory(f);
    }

    // Save templates
    f->Write();
    hists_to_save->Delete();
    delete hists_to_save;
}

//____________________________________________________________________

TList* HepDataMCPlot::GetListOfMCTemplates(const char* suffix) {
    //
    // Saves group-histograms and single sample histograms and
    // returns a list of histograms
    //
    // The name of the histograms consists of the folder/sample name
    // and a suffix, e.g. "ttbar_jesup"
    //

    // Use this list to determine the un-grouped histograms
    TList *hists_to_save = new TList;
    hists_to_save->AddAll(fMCHistStack->GetHists());
    TH1F *h_mc = 0;

    // Loop over folders
    TIter next_folder(fMCFolders);
    HepDataMCFolder *folder = 0;
    while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	TIter next_sample(folder->GetMCSamples());
	HepDataMCSample *sample = 0;

	TH1F *h_mcgroup = 0;
	while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
	    h_mc  = sample->GetHistogram();
            if ( nullptr == h_mc ) {
                Warning(__FUNCTION__,
                        "Sample %s has no histogram %s. Skip!",
                        sample->GetName(), GetName());
                continue;
            }
	    hists_to_save->Remove(h_mc);

	    // Clone first histogram to be used for the group
	    // otherwise add all samples to existing group histogram
	    if ( h_mcgroup == 0 )
		h_mcgroup = dynamic_cast<TH1F*>(
                    h_mc->Clone(Form("%s_%s", folder->GetName(), suffix)));
	    else
		h_mcgroup->Add(h_mc);
	}

	// Add the grouped histogram to the list
	hists_to_save->Add(h_mcgroup);
    }

    return hists_to_save;
}

//____________________________________________________________________

void HepDataMCPlot::PrintBinStatistics(Option_t *option) {
    //
    // Print S/B and S/sqrt(B) for all bins
    //

    TString opt = option;
    opt.ToUpper();

    TList *hists = GetListOfMCTemplates("Statistics");

    // It is assumed that the signal process is drawn as topmost histogram
    // --> therefore it is the last histogram in the list.

    TH1F *bkg = dynamic_cast<TH1F*>(hists->At(0)->Clone("bkg"));
    for ( Int_t i = 1; i < hists->GetEntries()-1; i++ ) {
	bkg->Add(dynamic_cast<TH1F*>(hists->At(i)));
    }
    TH1F *sig = dynamic_cast<TH1F*>(hists->At(hists->GetEntries()-1)->Clone("sig"));

    // Binning starts at 1, over and underflow bin are excluded here
    printf("========================================\n");
    printf(" Bin Low Edge |     S/B    |  S/sqrt(B) \n");
    printf("========================================\n");
    for ( Int_t bin = 1; bin <= sig->GetNbinsX(); bin++ ) {
	if ( opt.Contains("LATEX") ) {
	    printf(" %-4.3f & %-8f & %-8f\n",
		   sig->GetXaxis()->GetBinLowEdge(bin),
		   sig->GetBinContent(bin)/bkg->GetBinContent(bin),
		   sig->GetBinContent(bin)/TMath::Sqrt((bkg->GetSumw2())->At(bin)));
	} else {
	    printf("    %-4.3f     |  %-8f  |  %-8f\n",
		   sig->GetXaxis()->GetBinLowEdge(bin),
		   sig->GetBinContent(bin)/bkg->GetBinContent(bin),
		   sig->GetBinContent(bin)/TMath::Sqrt((bkg->GetSumw2())->At(bin)));
	}
    }
    printf("========================================\n");

    delete bkg;
    delete sig;
    hists->Delete();
    delete hists;
}

//____________________________________________________________________

void HepDataMCPlot::DrawStack(Option_t *option) {
    //
    // ==================================
    // Draw MC histograms in stacked mode
    // ==================================
    //
    // In case of stacked histogram mode draw the histograms in
    // filled mode with their fill color. Use the histogram line
    // colour of the current style for drawing the histogram
    // outline

    if ( gDebug ) {
        if ( fFirstDraw )
            Info(__FUNCTION__, "Drawing stack for first time");
        else
            Info(__FUNCTION__, "Drawing stack again");
    }
    // Convert option string
    TString opt = option;
    opt.ToLower();

    // Choose main pad to draw
    TPad *padsav = dynamic_cast<TPad*>(gPad);
    fMainPad->Draw();
    fMainPad->cd();

    // Remove signal MC from stack for SignalOverlay
    // Update: Take complete last folder, not only last hist on stack,
    // but can switch back to old with the flag:
    Bool_t use_last_hist = kFALSE;
    HepDataMCFolder *fsignal = dynamic_cast<HepDataMCFolder*>(GetMCFolders()->Last());
    THStack *st_signal =  new THStack(Form("hstack_%s", fsignal->GetName()),
                                      fsignal->GetTitle());
    if ( fDrawSignalOverlay ) {
        if ( use_last_hist ) {
            if ( fFirstDraw ) {
                fHistogramLastInStack = dynamic_cast<TH1*>(fMCHistStack->GetHists()->Last());
                fHistogramLastInStack->Scale(fScaleOverlay);
                fMCHistStack->GetHists()->RemoveLast();
            }
        } else {
            // move signal histograms to different stack
            TIter next_sample(fsignal->GetMCSamples());
            HepDataMCSample *sample = 0;
            while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
                TH1F* h_mc = sample->GetHistogram();
                st_signal->Add(h_mc);
                if ( fFirstDraw ) {
                    h_mc->Scale(fScaleOverlay);
                    fMCHistStack->GetHists()->Remove(h_mc);
                }
            }
        }
    }

    // Set colors
    SetColorsStack();

    // Get maximum bin content
    Double_t max_mc = fMCHistStack->GetMaximum();
    Double_t max_data = 0.;
    if ( fHistDATA != 0 && fDrawData )
	max_data = fHistDATA->GetMaximum();
    Double_t ymax = 0.;
    if ( max_mc > max_data )
	ymax = max_mc;
    else
	ymax = max_data;

    // Add extra space to maximum
    ymax *= fAddRange;
    if ( fYRangeUser ) {
	ymax = fYRangeUserMax;

	fMCHistStack->SetMinimum(fYRangeUserMin);
	if ( fHistDATA )
		fHistDATA->SetMinimum(fYRangeUserMin);
    }

    // Set y range
    fMCHistStack->SetMaximum(ymax);
    if ( fHistDATA != 0 )
      	fHistDATA->SetMaximum(ymax);

    // Draw MC histograms
    if ( fUnifyMCStack ) {
	// In case of unified MC histograms draw simply the top-most one
	fHistMCTop = dynamic_cast<TH1F*>(
            (fMCHistStack->GetStack()->Last())->Clone("top_last"));
	fHistMCTop->Draw("hist,same"); // do not use: DrawClone --> produces artefacts when saving plots!!!
	fHistMCTop->SetXTitle(fXTitle->Data());
	fHistMCTop->SetYTitle(fYTitle->Data());
	fHistMCTop->SetFillStyle(1001);
	fHistMCTop->SetFillColor(fgUnifyColor);
	fHistogramMainPad = fHistMCTop;
    } else {
	// In case of drawing all the different MCs draw the stack and the topmost one
	// again to show the histogram line
	fMCHistStack->Draw("hist");
	fMCHistStack->GetHistogram()->SetXTitle(fXTitle->Data());
	fMCHistStack->GetHistogram()->SetYTitle(fYTitle->Data());
	fHistogramMainPad = fMCHistStack->GetHistogram();
	fHistMCTop = dynamic_cast<TH1F*>(
            (fMCHistStack->GetStack()->Last())->Clone("top_last"));
	fHistMCTop->Draw("hist,same"); // do not use: DrawClone --> produces artefacts when saving plots!!!
	fHistMCTop->SetFillStyle(0);
    }

    // Draw overlay of signal histogram
    if ( fDrawSignalOverlay ) {
        if ( use_last_hist ) {
            fHistogramLastInStack->SetFillColorAlpha(kMagenta, 0.2);
            fHistogramLastInStack->SetLineColor(kMagenta);
            fHistogramLastInStack->SetLineWidth(2);
            fHistogramLastInStack->Draw("hist,same");
        } else {
            fsignal = dynamic_cast<HepDataMCFolder*>(GetMCFolders()->Last());
            TIter next_sample(fsignal->GetMCSamples());
            HepDataMCSample *sample = 0;
            while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
                TH1F* h_mc = sample->GetHistogram();
                h_mc->SetFillColorAlpha(kMagenta, 0.2);
                h_mc->SetLineColor(kMagenta);
                h_mc->SetLineWidth(2);
            }
            st_signal->Draw("hist,same");
        }
    }

    // Draw MC uncertainty band (if wished)
    if ( fDrawMCError ) {
    	fMCErrors = SetErrors(fHistMCTop, kFALSE);
    	fMCErrors->SetLineColor(16);   // before: 6 (pink)
    	fMCErrors->SetFillColor(16);   // before: 6 (pink)
    	fMCErrors->SetFillStyle(3254); // before: 3005
    	fMCErrors->Draw("2,same");
    } else {
    	Info("Draw",
    	     "MC error graph was not set. No MC errors are drawn.");
    }

    // Draw DATA histogram on top (if exist)
    // with black markers (boxes)
    if ( fDrawData && (fHistDATA != 0) ) {

    	fHistDATA->SetMarkerStyle(gStyle->GetMarkerStyle());
	fGraphDATA = SetErrors(fHistDATA,kTRUE,option);
	fGraphDATA->SetMarkerStyle(fHistDATA->GetMarkerStyle());
	fGraphDATA->SetLineColor(fHistDATA->GetLineColor());

	fGraphDATA->Draw("EP");

	// if ( fDrawDataZeroEntryErrors )
        //     fHistDATA->Draw("E0 X0 P same"); // or P0?
	// else
	//     fHistDATA->Draw("E X0 P same");
    }

    // Restore old gPad pointer
    padsav->cd();
}



//____________________________________________________________________

void HepDataMCPlot::SetColorsStack() {
    //
    // Setup colour scheme for stacked mode
    //
    // In case of grouped drawing mode set the line and fill
    // attributes of the grouped histograms first. The histograms
    // within one group get all the same colour for filling and the
    // histogram line so that they appear as a single histogram

    // Use this list to determine the un-grouped histograms
    TList *hists_to_draw = new TList;
    hists_to_draw->AddAll(fMCHistStack->GetHists());
    if ( gDebug )
        Info(__FUNCTION__, "Setting color of %d histograms", hists_to_draw->GetEntries());

    // Do not forget to change the clone histograms internally
    // used by the hist stack too
    TIter next_folder(fMCFolders);
    HepDataMCFolder *folder = 0;
    TH1F *h_mc = 0;
    TH1F *h_mc2 = 0;
    while ( (folder = dynamic_cast<HepDataMCFolder*>(next_folder())) ) {
	TIter next_sample(folder->GetMCSamples());
	HepDataMCSample *sample = 0;
	while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
	    // Change the sample and the cloned histogram from THStack
	    h_mc = sample->GetHistogram();
            if ( h_mc == nullptr ) {
                if ( gDebug > 0 )
                    Info(__FUNCTION__, "No histogram for sample %s available; skip",
                         sample->GetName());
                continue;
            }

	    h_mc ->SetFillStyle(1001);
	    h_mc ->SetFillColor(folder->GetAttFill().GetFillColor());
	    h_mc ->SetLineStyle(gStyle->GetHistLineStyle());
	    h_mc ->SetLineColor(folder->GetAttFill().GetFillColor());
	    h_mc ->SetLineWidth(0);

	    h_mc2  = dynamic_cast<TH1F*>(
                fMCHistStack->GetStack()->FindObject(h_mc->GetName()));
	    if ( h_mc2 != 0 ) {
		h_mc2->SetFillStyle(1001);
		h_mc2->SetFillColor(folder->GetAttFill().GetFillColor());
		h_mc2->SetLineStyle(gStyle->GetHistLineStyle());
		h_mc2->SetLineColor(folder->GetAttFill().GetFillColor());
		h_mc2->SetLineWidth(0);
	    }

	    if ( fGroupHistograms == kTRUE ) {
		hists_to_draw->Remove(h_mc);

		if ( folder->GetAttFill().GetFillColor() == kWhite ) {
		    h_mc ->SetLineColor(kBlack);
		    h_mc ->SetLineWidth(1);

		    if ( h_mc2 != 0 ) {
			h_mc2->SetLineColor(kBlack);
			h_mc2->SetLineWidth(1);
		    }
		}
	    } else {
		if ( h_mc2 != 0 ) {
		    h_mc2->SetLineWidth(1);
		}
	    }
	}
    }

    // Set line and fill attributes for the remaining un-grouped
    // MC histograms. In case of un-grouped drawing these are all
    // histograms of the MC stack
    if ( gDebug )
        Info(__FUNCTION__, "Setting color of %d ungrouped histograms left",
             hists_to_draw->GetEntries());
    TIter next_ungrouped_hist(hists_to_draw);
    while ( (h_mc = dynamic_cast<TH1F*>(next_ungrouped_hist())) ) {
	h_mc2 = dynamic_cast<TH1F*>(
            fMCHistStack->GetStack()->FindObject(h_mc->GetName()));

	h_mc ->SetFillStyle(1001);
	h_mc ->SetLineColor(gStyle->GetHistLineColor());
	h_mc ->SetLineStyle(gStyle->GetHistLineStyle());
	h_mc ->SetLineWidth(0);

	h_mc2->SetFillStyle(1001);
	h_mc2->SetLineColor(gStyle->GetHistLineColor());
	h_mc2->SetLineStyle(gStyle->GetHistLineStyle());
	h_mc2->SetLineWidth(0);
    }
    delete hists_to_draw;
}

//____________________________________________________________________

void HepDataMCPlot::SetupPad() {
    //
    // Setup Pads for drawing the stack and ratio-plots
    //

    // How many ratio plots?
    Int_t NRatios = 0;
    if ( fDrawDataMCRatio ) NRatios++;
    if ( fDrawSignificance ) NRatios++;

    // Clear current pad or create a new one
    if ( gPad ) {
	gPad->Clear();
	fCanvas = 0;
    } else {
	fCanvas = gROOT->MakeDefCanvas();
    }

    Double_t newheight  = 1.;
    Double_t newheight2 = 1.;
    Double_t addspace = fRatioHeight - gStyle->GetPadBottomMargin() + gStyle->GetLabelSize()*0.5;
    if ( NRatios == 1 ) {

	newheight = (1. + addspace);
	// Main Pad NDCs
	fMainPad = new TPad("MainPad", "", 0., 1.- 1./newheight, 1., 1.);

	// First Ratio Pad NDC (overlap with Main intended!)
	fRatioPad1 = new TPad("RatioPad1", "", 0., 0., 1., fRatioHeight/newheight);
	fRatioPad1->SetBottomMargin(gStyle->GetPadBottomMargin()/fRatioHeight);

	fRatioPad2 = 0;
    } else if ( NRatios == 2 ) {
	newheight  = (1. +    addspace);
	newheight2 = (1. + 2.*addspace);

	// Main Pad NDCs
	fMainPad = new TPad("MainPad", "", 0., 1.- 1./newheight2, 1., 1.);

	// First Ratio Pad NDC (overlap with Main intended!)
	fRatioPad1 = new TPad("RatioPad1", "", 0., 1.-newheight/newheight2, 1., 1.-(newheight-fRatioHeight)/newheight2);
	fRatioPad1->SetBottomMargin(gStyle->GetPadBottomMargin()/fRatioHeight);

	// Second Ratio Pad NDC (overlap with first ratio intended!)
	fRatioPad2 = new TPad("RatioPad1", "", 0., 0., 1., fRatioHeight/newheight2);
	fRatioPad2->SetBottomMargin(gStyle->GetPadBottomMargin()/fRatioHeight);
    } else {
	fMainPad = new TPad("MainPad", "", 0., 0., 1., 1.);
	fRatioPad1 = 0;
	fRatioPad2 = 0;
    }

    if ( fCanvas != 0 )
	fCanvas->SetWindowSize(fCanvas->GetWindowWidth(),
			       fCanvas->GetWindowHeight()*(1 + NRatios*addspace));

    // store the original pad (this is not always a canvas)
    fCanvas = dynamic_cast<TCanvas*>(gPad);

}

//____________________________________________________________________

void HepDataMCPlot::DrawDataMCRatio() {
    //
    // Draw Data/MC ratio
    //
    // The data/mc ratio is plotted with dotted markers and their
    // uncertainty corresponds to the relative uncertainty from data.
    // Furthermore the uncertainty on the prediction (i.e. MC/MC) is
    // shown as shaded uncertainty band.
    //

    // Choose pad for ratio
    // (Currently always the first one!)
    TPad *padsav = dynamic_cast<TPad*>(gPad);
    fRatioPad1->Draw();
    fRatioPad1->cd();
    fRatioPad1->SetGrid();

    if ( fHistDATA == 0 ) {
	Fatal("DrawDataMCRatio", "Data histogram does not exist. Abort!");
    } else if ( fHistMCTop == 0 ) {
	Fatal("DrawDataMCRatio", "Stack is not drawn. Draw stack first! Abort!");
    }
    // Data
    TH1F *h_ratio = dynamic_cast<TH1F*>(fHistDATA->Clone("ratio"));
    TH1F *h_error = dynamic_cast<TH1F*>(fHistMCTop->Clone("PredictionUncertainty"));

    // Copy the uncertainty from the MCerrors graph
    // those include any uncertainties, e.g. normalization uncertainties
    for ( Int_t bin = 1; bin <= fHistMCTop->GetNbinsX(); bin++ ) {
        // note that the error graph points start at 0, bins start at 1.
        // GetErrorY() returns the mean of upper and lower error
        h_error->SetBinError(bin, fMCErrors->GetErrorY(bin-1));

	// then compute relative error for each bin
	if ( h_error->GetBinContent(bin) == 0 )
	    h_error->SetBinError(bin, 0.);
	else
	    h_error->SetBinError(bin, h_error->GetBinError(bin)/h_error->GetBinContent(bin));
    }

    // Data, MC ratio:
    if ( fDataMCRatioCenterOne ) {
	// Data/MC
	h_ratio->Divide(fHistMCTop);
	// MC/MC (Prediction is 1)
	for ( Int_t bin = 1; bin <= h_error->GetNbinsX(); bin++ )
	    h_error->SetBinContent(bin, 1);

    } else {
	// (Data - MC)/MC
	h_ratio->Add(fHistMCTop, -1.);
	h_ratio->Divide(fHistMCTop);

	// (MC - MC)/MC (Prediction is 0)
	for ( Int_t bin = 1; bin <= h_error->GetNbinsX(); bin++ )
	    h_error->SetBinContent(bin, 0);
    }

    // Errors
    for (Int_t bin = 0; bin < h_error->GetNbinsX(); bin++) {
	// set relative data error
	if ( fHistMCTop->GetBinContent(bin) == 0 ) {
	    h_ratio->SetBinError(bin, 0.);
	} else {
	    h_ratio->SetBinError(bin, fHistDATA->GetBinError(bin)/fHistMCTop->GetBinContent(bin));
	}
    }

    // Setup style
    h_ratio->SetMarkerStyle(gStyle->GetMarkerStyle());
    h_ratio->SetMarkerSize(0.8*gStyle->GetMarkerSize());

    Double_t ymax = h_ratio->GetBinContent(h_ratio->GetMaximumBin());
    Double_t ymin = h_ratio->GetBinContent(h_ratio->GetMinimumBin());
    if ( fDataMCRatioCenterOne ) {
	ymax -= 1.;
	ymin -= 1.;
    }

    if ( ymax > 0. && ymin < 0. ) {
	ymin = TMath::Abs(ymin);
    } else if ( ymax < 0 && ymin < 0 ) {
	ymax = TMath::Abs(ymax);
	ymin = TMath::Abs(ymin);
    }
    Double_t yrange = ymax > ymin ? ymax*fAddRange/1.3 : ymin*fAddRange/1.3;

    if ( fDataMCRatioCenterOne ) {
	h_ratio->SetYTitle("Data / MC");
	h_ratio->SetMaximum(1+yrange);
	h_ratio->SetMinimum(1-yrange);
    } else {
	h_ratio->SetYTitle("Data/MC - 1"); // (Data - MC)/MC
	h_ratio->SetMaximum( yrange);
	h_ratio->SetMinimum(-yrange);
    }

    h_ratio->Draw("axis");

    if ( fDataMCRatioCenterOne ) {
	fRatioLine = new TLine(h_error->GetXaxis()->GetXmin(), 1.,
			 h_error->GetXaxis()->GetXmax(), 1.);
    } else {
	fRatioLine = new TLine(h_error->GetXaxis()->GetXmin(), 0.,
			 h_error->GetXaxis()->GetXmax(), 0.);
    }
    fRatioLine->SetHorizontal(kTRUE);
    fRatioLine->SetLineStyle(7);
    fRatioLine->SetLineColor(kBlack);
    fRatioLine->Draw();

    TGraphAsymmErrors *h_errorband = new TGraphAsymmErrors(h_error);
    h_errorband->SetFillStyle(3254);
    h_errorband->SetFillColor(16);
    h_errorband->SetLineColor(16);
    h_errorband->SetMarkerSize(0.);
    h_errorband->Draw("5,same");

    if ( fDrawDataErrorX )
	h_ratio->Draw("E0 P same");
    else
	h_ratio->Draw("E0 P X0 same");

    // Save pointer to histogram holding axis
    fHistogramRatioPad1 = h_ratio;

    // Redraw grid lines
    fRatioPad1->RedrawAxis("g");

    padsav->cd();
}

//____________________________________________________________________

void HepDataMCPlot::SetupAxis() {
    //
    // Setup axis labels and axis range
    //
    // Title size needs to be rescaled when using ratio plots. Size
    // depends whether axis is redrawn in the ratio pad or main pad.
    //

    // Scale for labels for subpads is given by 1./fRatioHeight
    if ( fDrawDataMCRatio && fDrawSignificance ) {
	// Setup second ratio plot if both ratios are drawn
	fHistogramRatioPad2->SetTitleSize(1./fRatioHeight*gStyle->GetTitleSize("X"), "X");
	fHistogramRatioPad2->SetLabelSize(1./fRatioHeight*gStyle->GetLabelSize("X"), "X");

	fHistogramRatioPad2->SetTitleSize(1./fRatioHeight*gStyle->GetTitleSize("Y"), "Y");
	fHistogramRatioPad2->SetLabelSize(1./fRatioHeight*gStyle->GetLabelSize("Y"), "Y");

	fHistogramRatioPad2->SetTitleFont(gStyle->GetTitleFont("X"), "X");
	fHistogramRatioPad2->SetTitleFont(gStyle->GetTitleFont("Y"), "Y");

	fHistogramRatioPad2->SetXTitle(fXTitle->Data());
	fHistogramRatioPad2->SetTitleOffset(gStyle->GetTitleOffset("X"),"X");
	fHistogramRatioPad2->SetTitleOffset(gStyle->GetTitleOffset("Y")*fRatioHeight,"Y");

	// reduce number of ticks on y-axis
	fHistogramRatioPad2->GetYaxis()->SetNdivisions(505);
	fHistogramRatioPad2->GetYaxis()->SetDecimals(kTRUE);
	fHistogramRatioPad2->GetXaxis()->SetDecimals(kTRUE);
    }

    if ( fDrawDataMCRatio || fDrawSignificance ) {
	// Setup first ratio
	if ( fDrawDataMCRatio && fDrawSignificance ) {
	    // if second ratio is present, don't draw titel and labels for the first one
	    fHistogramRatioPad1->SetTitleSize(0., "X");
	    fHistogramRatioPad1->SetLabelSize(0., "X");
	} else {
	    fHistogramRatioPad1->SetTitleSize(1./fRatioHeight*gStyle->GetTitleSize("X"), "X");
	    fHistogramRatioPad1->SetLabelSize(1./fRatioHeight*gStyle->GetLabelSize("X"), "X");
	}
	fHistogramRatioPad1->SetTitleSize(1./fRatioHeight*gStyle->GetTitleSize("Y"), "Y");
	fHistogramRatioPad1->SetLabelSize(1./fRatioHeight*gStyle->GetLabelSize("Y"), "Y");

	fHistogramRatioPad1->SetTitleFont(gStyle->GetTitleFont("X"), "X");
	fHistogramRatioPad1->SetTitleFont(gStyle->GetTitleFont("Y"), "Y");

	fHistogramRatioPad1->SetXTitle(fXTitle->Data());
	fHistogramRatioPad1->SetTitleOffset(gStyle->GetTitleOffset("X"),"X");
	fHistogramRatioPad1->SetTitleOffset(gStyle->GetTitleOffset("Y")*fRatioHeight,"Y");

	// reduce number of ticks on y-axis
	fHistogramRatioPad1->GetYaxis()->SetNdivisions(505);
	fHistogramRatioPad1->GetYaxis()->SetDecimals(kTRUE);
	fHistogramRatioPad1->GetXaxis()->SetDecimals(kTRUE);

	// Setup Main pad in case of ratios
	fHistogramMainPad->SetXTitle(fXTitle->Data());
	fHistogramMainPad->SetYTitle(fYTitle->Data());
	fHistogramMainPad->SetLabelSize(0., "X");
	fHistogramMainPad->SetTitleSize(0., "X");
    	fHistogramMainPad->GetXaxis()->SetDecimals(kTRUE);
	fHistogramMainPad->GetYaxis()->SetDecimals(kTRUE);
    }

    if ( !fDrawDataMCRatio && !fDrawSignificance ) {
	// If no ratio is drawn, restore the label size
	fHistogramMainPad->SetXTitle(fXTitle->Data());
	fHistogramMainPad->SetYTitle(fYTitle->Data());
	fHistogramMainPad->SetLabelSize(gStyle->GetLabelSize("X"), "X");
	fHistogramMainPad->SetTitleSize(gStyle->GetTitleSize("X"), "X");
    }
}

//____________________________________________________________________

void HepDataMCPlot::DrawSignificance() {
    //
    // Draw Significance
    //

    // Choose pad for ratio
    TPad *padsav = dynamic_cast<TPad*>(gPad);
    if ( fDrawDataMCRatio ) {
	// take the second pad
	fRatioPad2->Draw();
	fRatioPad2->cd();
	fRatioPad2->SetGrid();
    } else {
	// take the first
	fRatioPad1->Draw();
	fRatioPad1->cd();
	fRatioPad1->SetGrid();
    }

    if ( fHistDATA == 0 && fDrawData ) {
        Fatal(__FUNCTION__, "Data histogram does not exist. Abort!");
    } else if ( fHistMCTop == 0 ) {
        Fatal(__FUNCTION__, "Stack is not drawn. Draw stack first! Abort!");
    }

    // Get significance by comparing top of MC stack with data;
    // in case data is unavailable, compare sum of bkg with sum of MC
    TH1F *h_sigma;
    if ( fHistDATA )
        h_sigma = CompareHistograms(fHistDATA, fHistMCTop);
    else {
        TH1F *h_asimov = 0;
        TH1F *h_bkg = 0;
        TH1F *h_sig = 0;
        TH1F *h_mc = 0;
        // all hist in stack are bkg MC
        TIter next_hist(fMCHistStack->GetHists());
        while ( (h_mc = dynamic_cast<TH1F*>(next_hist())) ) {
            if ( h_bkg == 0 ) {
                h_bkg = dynamic_cast<TH1F*>(h_mc->Clone());
            } else {
                h_bkg->Add(h_mc);
            }
        }
        // asimov is bkg + sig
        HepDataMCFolder *fsignal = dynamic_cast<HepDataMCFolder*>(GetMCFolders()->Last());
        TIter next_sample(fsignal->GetMCSamples());
        HepDataMCSample *sample = 0;
        while ( (sample = dynamic_cast<HepDataMCSample*>(next_sample())) ) {
            h_mc = sample->GetHistogram();
            if ( h_sig == 0 ) {
                h_sig = dynamic_cast<TH1F*>(h_mc->Clone());
            } else {
                h_sig->Add(h_mc);
            }
        }
        if ( fDrawSignalOverlay ) {
            h_sig->Scale(1./fScaleOverlay); // undo scaling in DrawStack()
        }
        h_asimov = dynamic_cast<TH1F*>(h_bkg->Clone());
        h_asimov->Add(h_sig);

        h_sigma = CompareHistograms(h_asimov, h_bkg);

        if ( gDebug > 0 ) {
            Info(__FUNCTION__, "Integral of top histogram in stack:  %10.1f", fHistMCTop->Integral());
            Info(__FUNCTION__, "Integral of last histogram in stack: %10.1f",
                 dynamic_cast<TH1F*>(fMCHistStack->GetHists()->Last())->Integral());
            Info(__FUNCTION__, "Integral of asimov histogram:        %10.1f", h_asimov->Integral());
            Info(__FUNCTION__, "Integral of bkg histogram:           %10.1f", h_bkg->Integral());
            Info(__FUNCTION__, "Integral of sig histogram:           %10.1f", h_sig->Integral());
        }
    }
    h_sigma->SetYTitle("Significance");
    h_sigma->SetFillColor(kGray);

    Double_t ymax = h_sigma->GetBinContent(h_sigma->GetMaximumBin());
    Double_t ymin = h_sigma->GetBinContent(h_sigma->GetMinimumBin());

    if ( ymax > 0. && ymin < 0. ) {
	ymin = TMath::Abs(ymin);
    } else if ( ymax < 0 && ymin < 0 ) {
	ymax = TMath::Abs(ymax);
	ymin = TMath::Abs(ymin);
    }
    Double_t yrange = ymax > ymin ? ymax*fAddRange : ymin*fAddRange;
    h_sigma->SetMaximum( yrange);
    h_sigma->SetMinimum(-yrange);

    h_sigma->Draw("hist");

    if ( fDrawDataMCRatio && fDrawSignificance )
	fHistogramRatioPad2 = h_sigma;
    else
	fHistogramRatioPad1 = h_sigma;

    padsav->cd();
}

//____________________________________________________________________

TH1F* HepDataMCPlot::CompareHistograms(TH1* hObs, TH1* hExp,
				       bool neglectUncertainty,
				       bool variableBinning,
				       TH1* hPull) {
    //
    // Given two ROOT histograms (with the same binning!) containing the
    // observed and expected counts, create and return a histogram showing
    // the significance of their bin-to-bin discrepancies.
    //
    // If the histogram representing the expectation (second input
    // parameter) has non-zero bin "errors", these are considered the
    // standard deviations representing the full uncertainty and the
    // significance is computed accordingly, unless this is disabled (third
    // parameter).
    //
    // The last input pointer is the pull histogram, which is filled with
    // all z-values, without discarding the bins for which the p-value is
    // larger than 0.5: in case of ideal match, the pulls follow a standard
    // normal distribution.  A typical binnin for the pulls is from -5 to
    // +5 with 20 bins.
    //
    //
    //
    // Code from
    // "Plotting the Differences Between Data and Expectation"
    // by Georgios Choudalakis and Diego Casadei
    // Eur. Phys. J. Plus 127 (2012) 25
    // http://dx.doi.org/10.1140/epjp/i2012-12025-y
    // (http://arxiv.org/abs/1111.2062)
    //
    // -----------------------------------------------------------------
    // This code is covered by the GNU General Public License:
    // http://www.gnu.org/licenses/gpl.html
    // -----------------------------------------------------------------
    // Diego Casadei <casadei@cern.ch> 7 Dec 2011
    //

    if (hObs==0 || hExp==0) {
	cerr << "ERROR in CompareHistograms(): invalid input" << endl;
	return 0;
    }
    TString name=hObs->GetName();
    name+="_cmp_";
    name+=hExp->GetName();
    int Nbins = hObs->GetNbinsX();
    if (Nbins != hExp->GetNbinsX()) {
	cerr << "ERROR in CompareHistograms(): different binning" << endl;
	return 0;
    }
    TH1F* hOut = 0;
    if (variableBinning) {
	hOut = new TH1F(name, "",
			hObs->GetXaxis()->GetNbins(),
			hObs->GetXaxis()->GetXbins()->GetArray());
    } else {
	hOut = new TH1F(name, "",
			Nbins,
			hObs->GetXaxis()->GetXmin(),
			hObs->GetXaxis()->GetXmax());
    }
    hOut->GetXaxis()->SetTitle( hObs->GetXaxis()->GetTitle() );
    hOut->GetYaxis()->SetTitle("significance");
    hOut->SetFillColor(2);

    for (int i=1; i<=Nbins; ++i) { // SKIP UNDER- AND OVER-FLOWS
	// if (hObs->GetBinContent(i)<=0) continue;

	unsigned nObs = (int) hObs->GetBinContent(i);
	float nExp = hExp->GetBinContent(i);
	float vrnc = hExp->GetBinError(i);
	vrnc *= vrnc; // variance
	float zValue = 0;
	float pValue = 1;
	if (vrnc>0 && !neglectUncertainty) {
	    // account for systematic uncertainty
	    pValue = pValuePoissonError(nObs, nExp, vrnc);
	} else {
	    // assume perfect knowledge of Poisson parameter
	    pValue = pValuePoisson(nObs,nExp);
	}
	zValue = pValueToSignificance(pValue, (nObs>nExp));
	if (pValue<0.5) hOut->SetBinContent(i, zValue);
	if (hPull) hPull->Fill(zValue);
    }

    return hOut;
}

//____________________________________________________________________

Double_t HepDataMCPlot::pja_normal_quantile(double p) {
    //
    //
    // Normal quantile computed following Peter John Acklam's
    // pseudo-code algorithm for rational approximation
    // (pjacklam@online.no)
    // http://home.online.no/~pjacklam/notes/invnorm/
    //
    // The algorithm below assumes p is the input and x is the output.
    //
    //  Coefficients in rational approximations.
    //  a(1) <- -3.969683028665376e+01
    //  a(2) <-  2.209460984245205e+02
    //  a(3) <- -2.759285104469687e+02
    //  a(4) <-  1.383577518672690e+02
    //  a(5) <- -3.066479806614716e+01
    //  a(6) <-  2.506628277459239e+00
    //
    //  b(1) <- -5.447609879822406e+01
    //  b(2) <-  1.615858368580409e+02
    //  b(3) <- -1.556989798598866e+02
    //  b(4) <-  6.680131188771972e+01
    //  b(5) <- -1.328068155288572e+01
    //
    //  c(1) <- -7.784894002430293e-03
    //  c(2) <- -3.223964580411365e-01
    //  c(3) <- -2.400758277161838e+00
    //  c(4) <- -2.549732539343734e+00
    //  c(5) <-  4.374664141464968e+00
    //  c(6) <-  2.938163982698783e+00
    //
    //  d(1) <-  7.784695709041462e-03
    //  d(2) <-  3.224671290700398e-01
    //  d(3) <-  2.445134137142996e+00
    //  d(4) <-  3.754408661907416e+00
    //
    //  Define break-points.
    //
    //  p_low  <- 0.02425
    //  p_high <- 1 - p_low
    //
    //  Rational approximation for lower region.
    //
    //  if 0 < p < p_low
    //     q <- sqrt(-2*log(p))
    //     x <- (((((c(1)*q+c(2))*q+c(3))*q+c(4))*q+c(5))*q+c(6)) /
    //           ((((d(1)*q+d(2))*q+d(3))*q+d(4))*q+1)
    //  endif
    //
    //  Rational approximation for central region.
    //
    //  if p_low <= p <= p_high
    //     q <- p - 0.5
    //     r <- q*q
    //     x <- (((((a(1)*r+a(2))*r+a(3))*r+a(4))*r+a(5))*r+a(6))*q /
    //          (((((b(1)*r+b(2))*r+b(3))*r+b(4))*r+b(5))*r+1)
    //  endif
    //
    //  Rational approximation for upper region.
    //
    //  if p_high < p < 1
    //     q <- sqrt(-2*log(1-p))
    //     x <- -(((((c(1)*q+c(2))*q+c(3))*q+c(4))*q+c(5))*q+c(6)) /
    //            ((((d(1)*q+d(2))*q+d(3))*q+d(4))*q+1)
    //  endif
    //
    // The absolute value of the relative error (x_approx - x)/x is
    // <= 1.15e-9 for all input values p which can be represented in
    // IEEE double precision arithmetic.
    //
    //
    //
    //
    // Code from
    // "Plotting the Differences Between Data and Expectation"
    // by Georgios Choudalakis and Diego Casadei
    // Eur. Phys. J. Plus 127 (2012) 25
    // http://dx.doi.org/10.1140/epjp/i2012-12025-y
    // (http://arxiv.org/abs/1111.2062)
    //
    // -----------------------------------------------------------------
    // This code is covered by the GNU General Public License:
    // http://www.gnu.org/licenses/gpl.html
    // -----------------------------------------------------------------
    //


    double a[6] = {
	-3.969683028665376e+01, // a(1) -> a[0]
	 2.209460984245205e+02, // a(2)
	-2.759285104469687e+02, // a(3)
	 1.383577518672690e+02, // a(4)
	-3.066479806614716e+01, // a(5)
	 2.506628277459239e+00, // a(6) -> a[5]
    };

    double b[5] = {
	-5.447609879822406e+01, // b(1) -> b[0]
	 1.615858368580409e+02, // b(2)
	-1.556989798598866e+02, // b(3)
	 6.680131188771972e+01, // b(4)
	-1.328068155288572e+01, // b(5) -> b[4]
    };

    double c[6] = {
	-7.784894002430293e-03, // c(1) -> c[0]
	-3.223964580411365e-01, // c(2)
	-2.400758277161838e+00, // c(3)
	-2.549732539343734e+00, // c(4)
	 4.374664141464968e+00, // c(5)
	 2.938163982698783e+00, // c(6) -> c[5]
    };

    double d[4] = {
	7.784695709041462e-03, // d(1) -> d[0]
	3.224671290700398e-01, // d(2)
	2.445134137142996e+00, // d(3)
	3.754408661907416e+00, // d(4) -> d[3]
    };

    // Define break-points.
    double p_low  = 0.02425;
    double p_high = 1 - p_low;

    // output value
    double x=0;

    // Rational approximation for lower region.
    if (0 < p && p < p_low) {
	double q = sqrt(-2*log(p));
	x = (((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
	    ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
    }
    // Rational approximation for central region.
    else if (p_low <= p && p <= p_high) {
	double q = p - 0.5;
	double r = q*q;
	x = (((((a[0]*r+a[1])*r+a[2])*r+a[3])*r+a[4])*r+a[5])*q /
	    (((((b[0]*r+b[1])*r+b[2])*r+b[3])*r+b[4])*r+1);
    }
    // Rational approximation for upper region.
    else if (p_high < p && p < 1) {
	double q = sqrt(-2*log(1-p));
	x = -(((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
	    ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
    }

    return x;
}

//____________________________________________________________________

Double_t HepDataMCPlot::pValuePoisson(unsigned nObs, double nExp) {
    //
    // p-value for Poisson distribution, no uncertainty on the parameter
    // -----------------------------------------------------------------
    //
    // Diego Casadei <casadei@cern.ch>   Oct 2011
    // Last update: 4 Nov 2011 (using incomplete Gamma from ROOT)
    // -----------------------------------------------------------------
    //
    // Consider Poi(k|nExp) and compute the p-value which corresponds to
    // the observation of nObs counts.
    //
    // When nObs > nExp there is an excess of observed events and
    //
    //   p-value = P(n>=nObs|nExp) = \sum_{n=nObs}^{\infty} Poi(n|nExp)
    //           = 1 - \sum_{n=0}^{nObs-1} Poi(n|nExp)
    //           = 1 - e^{-nExp} \sum_{n=0}^{nObs-1} nExp^n / n!
    //
    // Otherwise (nObs <= nExp) there is a deficit and
    //
    //   p-value = P(n<=nObs|nExp) = \sum_{n=0}^{nObs} Poi(n|nExp)
    //           = e^{-nExp} \sum_{n=0}^{nObs} nExp^n / n!
    //
    //
    //
    // Code from
    // "Plotting the Differences Between Data and Expectation"
    // by Georgios Choudalakis and Diego Casadei
    // Eur. Phys. J. Plus 127 (2012) 25
    // http://dx.doi.org/10.1140/epjp/i2012-12025-y
    // (http://arxiv.org/abs/1111.2062)
    //
    // -----------------------------------------------------------------
    // This code is covered by the GNU General Public License:
    // http://www.gnu.org/licenses/gpl.html
    // -----------------------------------------------------------------

    if (nExp==0) return 0.5;
    if (nExp<0) {
	cerr << "ERROR in pValuePoisson(): invalid expectation = " << nExp
	     << " returning 0.5" << endl;
	return 0.5;
    }

    // ROOT provides everything:
    if (nObs>nExp) // excess
	return ROOT::Math::inc_gamma(nObs,nExp);
    else // deficit
	return ROOT::Math::inc_gamma_c(nObs+1,nExp);
}

//____________________________________________________________________

Double_t HepDataMCPlot::pValuePoissonError(unsigned nObs,
					   double E,
					   double V) {
    //
    //  p-value for Poisson distribution when there is uncertainty on the
    // parameter
    //
    // -----------------------------------------------------------------
    //
    // Diego Casadei <casadei@cern.ch>  6 Nov 2011
    // Last update: 3 Mar 2012 (logarithms used only for big numbers)
    //
    // -----------------------------------------------------------------
    //
    // Consider Poi(k|nExp) and compute the p-value which corresponds to
    // the observation of nObs counts, in the case of uncertain nExp whose
    // variance is provided.
    //
    // The prior for nExp is a Gamma density which matches the expectation
    // and variance provided as input.  The marginal model is provided by
    // the Poisson-Gamma mixture, which is used to compute the p-value.
    //
    // Gamma density: the parameters are
    //  * a = shape param  [dimensionless]
    //  * b = rate param   [dimension: inverse of x]
    //
    //   nExp ~ Ga(x|a,b) = [b^a/Gamma(a)] x^{a-1} exp(-bx)
    //
    // One has E[x] = a/b and V[x] = a/b^2 hence
    //  * b = E/V
    //  * a = E*b
    //
    // The integral of Poi(n|x) Ga(x|a,b) over x gives the (marginal)
    // probability of observing n counts as
    //
    //               b^a [Gamma(n+a) / Gamma(a)]
    //   P(n|a,b) = -----------------------------
    //                      n! (1+b)^{n+a}
    //
    // When nObs > nExp there is an excess of observed events and
    //
    //   p-value = P(n>=nObs) = \sum_{n=nObs}^{\infty} P(n)
    //           = 1 - \sum_{n=0}^{nObs-1} P(n)
    //
    // Otherwise (nObs <= nExp) there is a deficit and
    //
    //   p-value = P(n<=nObs) = \sum_{n=0}^{nObs} P(n)
    //
    // To compute the sum, we use the following recurrent relation:
    //
    //   P(n=0) = [b/(1+b)]^a
    //   P(n=1) = [b/(1+b)]^a a/(1+b) = P(n=0) a/(1+b)
    //   P(n=2) = [b/(1+b)]^a a/(1+b) (a+1)/[2(1+b)] = P(n=1) (a+1)/[2(1+b)]
    //   ...        ...
    //   P(n=k) = P(n=k-1) (a+k-1) / [k(1+b)]
    //
    // and to avoid rounding errors, we work with logarithms.
    //
    //
    // Code from
    // "Plotting the Differences Between Data and Expectation"
    // by Georgios Choudalakis and Diego Casadei
    // Eur. Phys. J. Plus 127 (2012) 25
    // http://dx.doi.org/10.1140/epjp/i2012-12025-y
    // (http://arxiv.org/abs/1111.2062)
    //
    // -----------------------------------------------------------------
    // This code is covered by the GNU General Public License:
    // http://www.gnu.org/licenses/gpl.html
    // -----------------------------------------------------------------
    if (E<=0 || V<=0) {
	cerr << "ERROR in pValuePoissonError(): expectation and variance must be positive. "
	     << "Returning 0.5" << endl;
	return 0.5;
    }
    double B = E/V;
    double A = E*B;

    // relative syst = sqrt(V)/E = 1/sqrt(A)
    // relative stat = 1/sqrt(nObs)
    // if syst < 0.1*stat there is no need for syst:
    // save a bit of CPU time :-)
    // if (A>100*nObs) return pValuePoisson(nObs,E); // UNCOMMENT TO SPEED-UP
    if (A>100) { // need to use logarithms

	unsigned stop=nObs;
	if (nObs>E) --stop;

	/// NB: must work in log-scale otherwise troubles!
	double logProb = A*log(B/(1+B));
	double sum=exp(logProb); // P(n=0)
	for (unsigned u=1; u<=stop; ++u) {
	    logProb += log((A+u-1)/(u*(1+B)));
	    sum += exp(logProb);
	}
	if (nObs>E)  // excess
	    return 1-sum;
	else  // deficit
	    return sum;

    } else {

	// Recursive formula: P(n;A,B) = P(n-1;A,B) (A+n-1)/(n*(1+B))
	double p0 = pow(B/(1+B),A); // P(0;A,B)
	double nExp = A/B;
	if (nObs>nExp) {// excess
	    double pLast = p0;
	    double sum = p0;
	    for (unsigned k=1; k<=nObs-1; ++k) {
		double p = pLast * (A+k-1) / (k*(1+B));
		// cout << Form("Excess: P(%d;%8.5g) = %8.5g and sum = %8.5g",k-1,nExp,pLast,sum) << " -> ";
		sum += p;
		pLast = p;
		// cout << Form("P(%d;%8.5g) = %8.5g and sum = %8.5g",k,nExp,pLast,sum) << endl;
	    }
	    return 1-sum;
	} else {// deficit
	    double pLast = p0;
	    double sum = p0;
	    for (unsigned k=1; k<=nObs; ++k) {
		// cout << Form("Deficit: P(%d;%8.5g) = %8.5g and sum = %8.5g",k-1,nExp,pLast,sum) << " -> ";
		double p = pLast * (A+k-1) / (k*(1+B));
		sum += p;
		pLast = p;
		// cout << Form("P(%d;%8.5g) = %8.5g and sum = %8.5g",k,nExp,pLast,sum) << endl;
	    }
	    return sum;
	}
    }
}


//____________________________________________________________________

Double_t HepDataMCPlot::pValueToSignificance(double p, bool excess) {
    //
    // Convert a p-value into a right-tail normal significance, i.e. into
    // the number of Gaussian standard deviations which correspond to it.
    // -----------------------------------------------------------------
    // Code from
    // "Plotting the Differences Between Data and Expectation"
    // by Georgios Choudalakis and Diego Casadei
    // Eur. Phys. J. Plus 127 (2012) 25
    // http://dx.doi.org/10.1140/epjp/i2012-12025-y
    // (http://arxiv.org/abs/1111.2062)
    //
    // -----------------------------------------------------------------
    // This code is covered by the GNU General Public License:
    // http://www.gnu.org/licenses/gpl.html
    // -----------------------------------------------------------------

    if (p<0 || p>1) {
	cerr << "ERROR: p-value must belong to [0,1] but input value is " << p << endl;
	return 0;
    }
    /*
      if (excess)
      return TMath::normal_quantile(1-p,1);
      else
      return TMath::normal_quantile(p,1);
    */

    if (excess)
	return pja_normal_quantile(1-p);
    else
	return pja_normal_quantile(p);
}


//____________________________________________________________________

void HepDataMCPlot::OptimizeBinning(Double_t zs, Double_t zb,
				    Bool_t DrawStack) {
    //
    // Optimize binning for given HepDataMCPlot
    // (this plot should have many bin (~100 - ~1000)
    //
    // To optimize the binning, the quantity Z_ij
    //
    // BEGIN_LATEX
    // Z(i,j) = z_{S}*#frac{n_{S}(i,j)}{N_S} + z_{B}*#frac{n_{B}(i,j)}{N_B}
    // END_LATEX
    //
    // with
    //
    // BEGIN_LATEX(separator='=', align='rl')
    // n_{S(B)}(i,j) = #text{Number of signal (bkg) events with indices between i and j}
    // N_{S(B)} = #text{Number of signal (bkg) events}
    // END_LATEX
    //
    // is computed. BEGIN_LATEX z_{S} END_LATEX and BEGIN_LATEX z_{B} END_LATEX are parameters for adjusting the number of bins.
    //
    // The binning is determined in a iterative procedure. Starting with
    // BEGIN_LATEX i = j = k, #text{with} k_{last bin} END_LATEX , then
    //  1) Compute BEGIN_LATEX Z(i,j) END_LATEX
    //  2) Check if BEGIN_LATEX Z(i,j) > 1 && #sqrt{n_B(i,j)} < 0.1 END_LATEX
    //  2.a) Condition is true  -> Merge all bins from i to j
    //                             Set j = i-1 and go back to 1)
    //  2.b) Condition is false -> Reduce i by one and go back to 1).
    //
    //  Repeat this until i == 1.
    //

    const Int_t maxbins = 51; // 50 bins (bin numbering starts with 1)

    TList *hists_mc = GetListOfMCTemplates("binning");
    // Assume that signal is last histogram in the stack
    TH1F *h_sig = dynamic_cast<TH1F*>(hists_mc->Last());
    TH1F *h_bkg = dynamic_cast<TH1F*>(hists_mc->At(0)->Clone("bkg"));
    for ( Int_t i = 1; i < hists_mc->GetEntries() - 1; i++ ) {
	// ToDo: Find smart way to skip samples
	// TString name = hists_mc->At(i)->GetName();
	// if ( name.Contains("wjetsLF") ) {
	//     cout << name.Data() << endl;
	//     continue;
	// }
	h_bkg->Add(dynamic_cast<TH1F*>(hists_mc->At(i)));
    }

    Double_t Ns = h_sig->Integral();
    Double_t Nb = h_bkg->Integral();

    // sum of weights for sqrt(B) computation
    // TArrayD *sig_sw2 = h_sig->GetSumw2();
    TArrayD *bkg_sw2 = h_bkg->GetSumw2();

    // Assume that 50 is the maximum number of bins after rebinning
    // Store array of low binedges (in reversed order)
    Double_t lowbinedges[maxbins];
    Int_t binnumbers[maxbins];

    for ( Int_t a = 0; a < maxbins; a++ ) { lowbinedges[a] = 0.; }

    cout << "Bin |  Binrange | S/B | S/sqrt(B) |" << endl;

    Int_t nbins = 1;
    for ( Int_t j = h_sig->GetNbinsX(); j > 0; j-- ) {
	Double_t Zij = 0;
	Double_t ns_ij = 0;
	Double_t nb_ij = 0;
	Double_t bkg_err = 0;

	Int_t i = j;
	for (; i > 0; i-- ) {
	    ns_ij += h_sig->GetBinContent(i);
	    nb_ij += h_bkg->GetBinContent(i);
	    bkg_err += bkg_sw2->At(i);

	    Zij = zs*ns_ij/Ns + zb*nb_ij/Nb;
	    if ( (Zij > 1. && sqrt(bkg_err)/Nb < 0.1)
		 || i == 1 ) {
		lowbinedges[nbins] = h_sig->GetXaxis()->GetBinLowEdge(i);
		binnumbers[nbins] = j;

		std::cout.precision(8);
		// printf("%-3d | %-4d-%-4d | %-3.3f |    %-3.3f    |\n",
		//        nbins, i, j,
		//        ns_ij/nb_ij, ns_ij/sqrt(bkg_err));
		nbins++;
		if ( nbins == maxbins ) {
		    Error("OptimizeBinning",
			  "Maximum number of allowed bins reached. Stop.");
		    return;
		}
		break;
	    }
	}
	j = i;
    }

    // set last bin's low edge
    // --> it's the low edge of the overflow bin
    //
    // the first bin's low edge is already set
    // nbins is here already the overflow bin, no '+1' needed!
    Int_t lastbin = h_sig->GetNbinsX();
    lowbinedges[0] = h_sig->GetXaxis()->GetBinLowEdge(lastbin+1);

    // Array stores the bin numbers of the upper edges
    // Thus this is the underflow bin's upper edge, which is zero.
    binnumbers[nbins] = 0.;

    // Print Binarray
    cout << "Array[" << nbins << "] = {" ;
    for ( Int_t bin = nbins-1; bin >= 0; bin-- ) {
	printf("%8.8f", lowbinedges[bin]);
	if ( bin > 0 ) cout << ", ";
	else cout << "}" << endl;
    }

    // Create empty histograms for filling rebinned histogram
    // Assume that 50 is the maximum number of bins after rebining
    if ( DrawStack ) {
	THStack *stack = new THStack;

	Bool_t rebinData = kTRUE;
	TH1F *hdata = new TH1F("hist_data", "hist_data", nbins-1, 0., 1.);

	for ( Int_t l = 0; l < hists_mc->GetEntries(); l++ ) {

	    // Create Histogram with correct number of bins
	    TH1F *hist = new TH1F(Form("hist_%d", l), Form("hist_%d", l),
				  nbins-1, 0., 1.);

	    // Rebin each Histogram
	    TH1F *h_org = dynamic_cast<TH1F*>(hists_mc->At(l));

	    // We need to iterate backwards through binnumbers!
	    Int_t bin = 1;
	    for ( Int_t m = nbins; m > 1; m-- ) {

		if ( rebinData ) {
		    Double_t val2 = fHistDATA->Integral(binnumbers[m] + 1, binnumbers[m-1]);
		    hdata->SetBinContent(bin, val2);
		}

		Double_t val = h_org->Integral(binnumbers[m] + 1, binnumbers[m-1]);
		hist->SetBinContent(bin, val);
		bin++;
	    }

	    rebinData = kFALSE;

	    // Adjust colors
	    hist->SetFillColor(h_org->GetFillColor());
	    hist->SetLineColor(h_org->GetFillColor());
	    hist->SetLineWidth(1);
	    hist->SetFillStyle(1001);

	    cout << hist->Integral() << endl;

	    // Add hist to Stack
	    stack->Add(hist);
	}
	stack->Draw();
	hdata->Draw("same");
    }
}

//____________________________________________________________________

void HepDataMCPlot::SetScaleOverlay(Float_t ScaleOverlay) {
    //
    // Scale factor of the overlay histogram (see
    // SetDrawSignalOverlay())
    //
    fScaleOverlay = ScaleOverlay;
    if ( gPad ) Draw();
}

//____________________________________________________________________

void HepDataMCPlot::SetDrawSignalOverlay(Bool_t DrawSignalOverlay) {
    //
    // Flag for drawing the signal MC (assumed to be the least entry
    // in the MC histogram stack) as overlay instead as part of the
    // stack
    //
    fDrawSignalOverlay = DrawSignalOverlay;
    if ( gPad ) Draw();
}
