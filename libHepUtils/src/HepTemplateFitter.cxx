//____________________________________________________________________
//
//    This Tool provides the functionality to define templates whose
//    weighted sum is then fitted to a reference. The fractions of 
//    the individual samples as well es the fitted templates themselves
//    are returned to the user.
//
//    In order to use the tool you have to write a task file in the 
//    fashion described below. If you have experience with the 
//    HepDataMCPlotter you should not have any problems.
//
//    The example is taken from a flavor ratio analysis in the
//    semileptonic ttbar channel
//
//    --------------------
//
//    {
//    // Define a new task
//    TTask *ttbar = new TTask("ttbar", "TTbar Analysis");
//
//    // Initialize HepTemplatefitter with name and title. The Name
//    // sets the name of the .root file that will be written.
//    HepTemplateFitter *pl_ttbar = new HepTemplateFitter("Template Fitter", "Template plots");
//
//    // AddMCSample adds samples to the list from which the templates will be
//    // generated. 
//    // Parameter 1 is the pathname of the sample
//    // Parameter 2 is the title of the MCSample
//    // Parameter 3 is the cross section of the sample since it is possible
//    // to generate templates from multiple files.
//    pl_ttbar->AddMCSample("templatetest.root", "MC t#bar{t} Np0 qq", 1.0);
//
//    // AddDatasample adds samples to the list from which the reference histogram
//    // will be generated. Since one may want to do template fits in MC also
//    // it is implemented similiar to AddMCSample
//    pl_ttbar->AddDataSample("FRTest.root", "MC t#bar{t} Np0 qq", 1.0);
//
//    // This generates the templates out of the MCSample files.
//    // Parameter 1 is the name of the histogram from which the template is derived
//    // Parameter 2 is the title of the parameter (for legends)
//    // Parameter 3 is the color of the template
//    pl_ttbar->GenerateTemplate("h_Masstemplate_uds", "light", kOrange);
//    pl_ttbar->GenerateTemplate("h_Masstemplate_c", "charm", kRed);
//    pl_ttbar->GenerateTemplate("h_Masstemplate_b", "bottom", kBlue);
//    pl_ttbar->GenerateTemplate("h_Masstemplate_x", "unknown", kGreen);
//
//    // Sets the reference. The parameter is the name of the histogram
//    // from which the reference is generated. 
//    pl_ttbar->GenerateReference("h_jets_mass_all");
//
//    // Add the Fitter to the task
//    ttbar->Add(pl_ttbar);
//
//    // Open browser
//    gROOT->GetListOfTasks()->Add(ttbar);
//    gROOT->GetListOfBrowsables()->Add(ttbar);
//    new TBrowser;
//    }
//
//    --------------------
//
//    Now execute the task and you will get the results of the fit.
//
//
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox-tu-dresden.de>
// Update: $$
// Copyright: 2010 (C) Felix Thomas
//
#ifndef HEP_HepTemplateFitter
#include <HepTemplateFitter.h>
#endif
#include <HepDataMCSample.h>
#include <HepTemplate.h>
#include <TFile.h>
#include <THStack.h>
#include <TKey.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TObjectTable.h> 
#include <TLegendEntry.h>
#include <TFractionFitter.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepTemplateFitter);
#endif

//____________________________________________________________________

HepTemplateFitter::HepTemplateFitter(const char* name, const char* title) :
    TTask(name, title) {
    //
    // Default constructor
    //
    
    TH1::AddDirectory(kFALSE);
    
    fDataSamples = new TList;
    fMCSamples = new TList;
    fTemplates = new TList;
    fReference = 0;
    fExportDir = new TString("./");
    fExportROOT = kTRUE;
    fOutputRootFile = 0;
}

//____________________________________________________________________

HepTemplateFitter::~HepTemplateFitter() {
    //
    // Default destructor
    //
    if ( fReference    != 0 ) delete fReference;
    fMCSamples->Delete(); delete fMCSamples;
    fDataSamples->Delete(); delete fDataSamples;
    delete fExportDir;
    if ( fOutputRootFile != 0 ) delete fOutputRootFile;
}



//____________________________________________________________________

HepDataMCSample* HepTemplateFitter::AddMCSample(const char* path,
					       const char* label,
					       Color_t color,
					       Style_t line_style,
					       Width_t line_width,
					       Style_t fill_style) {
    //
    // Add MC sample given by the path to its histogram file.
    // The label will be used in the corresponding legend entry.
    // The line and fill attributes are used for drawing the histograms
    // in stacked and overlay mode, resp.
    //
    HepDataMCSample *sample = new HepDataMCSample(path, label, color, line_style,
						  line_width, fill_style); 
    fMCSamples->Add(sample);
    return sample;
}

//____________________________________________________________________


HepDataMCSample* HepTemplateFitter::AddMCSample(const char* path,
					       const char* label,
					       Float_t xsec) {
    //
    // Same as above but line style and width, and fill style are set
    // to the defaults given by the current style
    //
    HepDataMCSample *sample = AddMCSample(path, label,
                      gStyle->GetHistFillColor(),
					  gStyle->GetHistLineStyle(),
					  gStyle->GetHistLineWidth(),
					  gStyle->GetHistFillStyle());
    sample->SetXsection(xsec);
    return sample;
}

//____________________________________________________________________

HepDataMCSample* HepTemplateFitter::AddDataSample(const char* path,
					       const char* label,
					       Color_t color,
					       Style_t line_style,
					       Width_t line_width,
					       Style_t fill_style) {
    //
    // Add Data sample given by the path to its histogram file.
    // The label will be used in the corresponding legend entry.
    // The line and fill attributes are used for drawing the histograms
    // in stacked and overlay mode, resp.
    //
    HepDataMCSample *sample = new HepDataMCSample(path, label, color, line_style,
						  line_width, fill_style); 
    fDataSamples->Add(sample);
    return sample;
}

//____________________________________________________________________

HepDataMCSample* HepTemplateFitter::AddDataSample(const char* path,
					       const char* label) {
    //
    // Same as above but line style and width, and fill style are set
    // to the defaults given by the current style
    //
    return AddDataSample(path, label, gStyle->GetHistFillColor(), gStyle->GetHistLineStyle(),
		       gStyle->GetHistLineWidth(),
		       gStyle->GetHistFillStyle());
}

//____________________________________________________________________

HepDataMCSample* HepTemplateFitter::AddDataSample(const char* path,
					       const char* label,
					       Float_t xsec) {
    //
    // Same as above but line style and width, and fill style are set
    // to the defaults given by the current style
    //
    HepDataMCSample *sample = AddDataSample(path, label, 
                      gStyle->GetHistFillColor(),
					  gStyle->GetHistLineStyle(),
					  gStyle->GetHistLineWidth(),
					  gStyle->GetHistFillStyle());
    sample->SetXsection(xsec);
    return sample;
}

//____________________________________________________________________

void HepTemplateFitter::Exec(Option_t *option) {
    //
    // Execute task
    //

    cout << endl;
    Info("Exec", "Begin of task \"%s\"", GetName());
    
    // Preparing Array with template-histograms
    TObjArray *mc = new TObjArray(fTemplates->GetEntries());

    for ( Int_t i = 0; i < fTemplates->GetEntries(); ++i){
        mc->Add(((HepTemplate*)fTemplates->At(i))->GetInputHistogram());
    }
    
    // Initialization and configuration of FractionFitter
    fFitter = new TFractionFitter(fReference, mc); 
    
    HepTemplate* templ;
    for ( Int_t i = 0; i < fTemplates->GetEntries(); ++i ){
        templ = (HepTemplate*)fTemplates->At(i);
        if ( templ->IsBound() ){
            fFitter->Constrain( i ,templ->GetLowerBound(), templ->GetUpperBound());
        }
    }

    // Performing Fit
    Int_t status = fFitter->Fit();               

    // Gathering results
    if (status == 0) {
        FillTemplates();
        THStack* hstack = BuildStackPlot();
        
        fReference->Draw("Ep");
        hstack->Draw("same");
        fReference->Draw("Epsame");
    
        // Open .root output file for writing
        if ( fExportROOT ) {
	        if ( fOutputRootFile != 0 ) delete fOutputRootFile;
	        fOutputRootFile = new TFile(Form("%s/%s.root",
				                        fExportDir->Data(),
					                    GetName()), "recreate");

            fReference->Write();
            ((TH1F*)fFitter->GetPlot())->Write();
            mc->Write();
            hstack->Write();
            for (Int_t i = 0; i < fTemplates->GetEntries(); ++i ) {
                templ = (HepTemplate*)fTemplates->At(i);
                templ->GetFittedHistogram()->Write();
            }
        }
    }

/*
    TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);
    leg->SetHeader("The Legend Title");

    leg->AddEntry(prediction, Form("%s, fraction: %f", templ->GetName(), templ->GetFitFraction()),"f");
*/

    Info("Exec", "End of task \"%s\"", GetName());
    cout << endl;

    // Close output .root file
    delete fOutputRootFile; fOutputRootFile = 0;

}


//____________________________________________________________________

void HepTemplateFitter::FillTemplates() {
    //
    // Fit Histograms and fit results are migrated to the 
    // HepTemplate-class objects
    //

    TH1F* prediction;
    HepTemplate* templ;
    Double_t fraction;
    Double_t error;

    for (Int_t i = 0; i < fTemplates->GetEntries(); ++i ) {
        templ = (HepTemplate*)fTemplates->At(i);

        fFitter->GetResult(i, fraction, error);
        
        prediction = (TH1F*)fFitter->GetMCPrediction(i);
        prediction->SetName(Form("%s_fitted", templ->GetInputHistogram()->GetName()));
        prediction->SetTitle(Form("%s (fitted), fraction = %f, uncertainty = %f",
                                    templ->GetInputHistogram()->GetTitle(),
                                    fraction, error));
        templ->SetFittedHistogram(prediction);

        templ->SetFitFraction(fraction);
        templ->SetFitFractionError(error);
    }
}

//____________________________________________________________________

THStack* HepTemplateFitter::BuildStackPlot() {
    //
    // Generation of stacked plot that shows the individual
    // contributions of the templates to the fit
    //

    TH1F* result = (TH1F*)fFitter->GetPlot();
    
    THStack* hstack = new THStack(Form("%s_stacked",result->GetName()),
                                  Form("%s (stacked)", result->GetTitle()));
    
    HepTemplate* templ;
    TH1F* histogram;
    
    for ( Int_t i = 0; i < fTemplates->GetEntries(); ++i){
        templ = (HepTemplate*)fTemplates->At(i);
        histogram = templ->GetFittedHistogram();
        histogram->SetFillColor(templ->GetColor());
        histogram->Scale(result->Integral()/histogram->Integral()*templ->GetFitFraction());
        hstack->Add(histogram);
    }
    
    return hstack;
}


//____________________________________________________________________

void HepTemplateFitter::GenerateTemplate(char* histname, char* templatename, Color_t color) {
    
    HepTemplate* temp = BuildTemplate(histname, templatename, color);
    fTemplates->Add(temp);
}

//____________________________________________________________________

void HepTemplateFitter::GenerateTemplate(char* histname, char* templatename, Double_t low, Double_t high, Color_t color) {
    
    HepTemplate* temp = BuildTemplate(histname, templatename, color);
    temp->SetBounds(low, high);
    fTemplates->Add(temp);
}


//____________________________________________________________________

HepTemplate* HepTemplateFitter::BuildTemplate(char* histname, char* templatename, Color_t color) {
    
    if ( fMCSamples == 0){
        Info("BuildTemplate", "Empty sample list, no template generated!");
        return 0;
    }

    HepDataMCSample* sample;
    HepTemplate* temp = new HepTemplate();
    temp->SetName(templatename);

    for ( Int_t i = 0; i < fMCSamples->GetEntries(); ++i ){
        sample = (HepDataMCSample*)fMCSamples->At(i);
        sample->Open();

        if ( i == 0 ){

            TH1F* histogram = new TH1F(*((TH1F*)sample->GetFile()->FindObjectAny(histname)));
            histogram->Scale(1.0/sample->GetXsection());
            temp->SetInputHistogram(histogram);
        }
        else {
            temp->GetInputHistogram()->Add((TH1F*)sample->GetFile()->FindObjectAny(histname), 1.0/sample->GetXsection());
        }
        sample->Close();
    }
    temp->SetColor(color);
    return temp;
}


//____________________________________________________________________

void HepTemplateFitter::GenerateReference(char* histname) {
    
    if ( fDataSamples == 0){
        Info("GenerateReference", "Empty sample list, no reference generated!");
        return;
    }
    
    if ( fReference != 0 ) delete fReference;
        
    HepDataMCSample* sample;

    for ( Int_t i = 0; i < fDataSamples->GetEntries(); ++i ){
        sample = (HepDataMCSample*)fDataSamples->At(i);
        sample->Open();

        if ( i == 0 ){

            fReference = new TH1F(*((TH1F*)sample->GetFile()->FindObjectAny(histname)));
            fReference->Scale(1.0/sample->GetXsection());
        }
        else {
            fReference->Add((TH1F*)sample->GetFile()->FindObjectAny(histname), 1.0/sample->GetXsection());
        }
        sample->Close();
    }
}

