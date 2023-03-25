//____________________________________________________________________
//
// Class for fraction fit on a given HepDataMCPlot
// 
//
//  
// Author: Umberto Prosperi Porta <mailto: umberto.prosperi@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Umberto Prosperi Porta
//



#ifndef HEP_HepFractionFitterTask
#include <HepFractionFitterTask.h>
#endif
#include <TFractionFitter.h>
#include <HepDataMCPlot.h>
#include <TString.h>
#include <TFile.h>
#include <TList.h>
#include <TH1F.h>
#include <TObjArray.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepFractionFitterTask);
#endif

//____________________________________________________________________
HepFractionFitterTask::HepFractionFitterTask(const char* name,
					 const char* title) : TTask(name, title) {
    //
    // Default constructor


       
}

//____________________________________________________________________

HepFractionFitterTask::~HepFractionFitterTask() {
    //
    // Default destructor
    //

}
//____________________________________________________________________

void HepFractionFitterTask::SetInputFilename(const char* InputFilename) {

    //
    // Set name of input file containing HepDataMCPlot
    // with the histograms for the fit
    //       
       

    fInputFilename.Remove(0);
    fInputFilename.Append(InputFilename);
}
//____________________________________________________________________

void HepFractionFitterTask::SetHepDataMCPlotName(const char* HepDataMCPlotName) {
    //
    // Set name of HepDataMCPlot
    //
    fHepDataMCPlotName.Remove(0);
    fHepDataMCPlotName.Append(HepDataMCPlotName);
}


//____________________________________________________________________

void HepFractionFitterTask::Exec(Option_t *option) {
    //
    // Execute task
    //
   
   
    f_input = new TFile(fInputFilename,"READ");
    fHepDataMCPlot = (HepDataMCPlot*)f_input->Get(fHepDataMCPlotName);


    Nmc = fHepDataMCPlot->GetMCHistStack()->GetHists()->GetEntries();

    TObjArray *mc = new TObjArray(Nmc);

    for(int i=0; i<fHepDataMCPlot->GetMCHistStack()->GetHists()->GetEntries(); i++){

	mc->Add((TH1F*)fHepDataMCPlot->GetMCHistStack()->GetHists()->At(i));

    }

    fHistDATA = (TH1F*)fHepDataMCPlot->GetHistDATA();
    
    TFractionFitter *fit = new TFractionFitter(fHistDATA,mc);


    //  fit->Constrain(1,0.0,1.0);               // constrain fraction 1 to be between 0 and 1
    fit->SetRangeX(1,7);                         // use only the first 7 bins in the fit

    
    Int_t status = fit->Fit();                   // perform the fit
 
    TString currentworkingdir ="$LIBQCDANALYSIS/hist/";
    TString filename = "FractionFitter.root";
    new TFile(currentworkingdir+filename,"RECREATE");
    

    
    cout << "fit status: " << status << endl;
    if (status == 0) {                       // check on fit status

	TH1F* result = (TH1F*) fit->GetPlot();

	fHistDATA->Write();

	result->Write();
	
    }



    

}






























