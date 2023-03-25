//
//Class for fraction fit on a given HepDataMCPlot
//
//
//
// Author: Umberto Prosperi Porta <mailto: umberto.prosperi@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Umberto Prosperi Porta
//
#ifndef HEP_HepFractionFitterTask
#define HEP_HepFractionFitterTask
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#include <iostream>
#include <fstream>


class TFractionFitter;
class HepDataMCPlot;
class TString;
class TFile;
class TList;
class TH1F;
class TObjArray;


class HepFractionFitterTask : public TTask {



  public: 
    TString         fInputFilename;         // name of input file (incouding path)
    TString         fHepDataMCPlotName;     // name of HepDataMCPlot (incouding path)
    
  private:

 
    TFile            *f_input;        // input file;
    HepDataMCPlot    *fHepDataMCPlot; // input HepDataMCPlot
    TH1F             *fHistDATA;      // histogram with data
    Int_t            Nmc;
  
	
    

  public:
    HepFractionFitterTask(const char* name, const char* title);
    virtual ~HepFractionFitterTask();

    virtual void Exec(Option_t *option);
    void SetInputFilename(const char* InputFilename);  

    void SetHepDataMCPlotName(const char* HepDataMCPlotName);




        ClassDef(HepFractionFitterTask,0) // fraction fitter

};
#endif
