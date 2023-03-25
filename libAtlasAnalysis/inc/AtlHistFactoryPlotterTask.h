//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryPlotterTask
#define ATLAS_AtlHistFactoryPlotterTask
#ifndef ATLAS_AtlHistFactoryTask
#include <AtlHistFactoryTask.h>
#endif
#include <RooStats/ModelConfig.h>

class RooAbsPdf;
class RooCurve;
class RooRealVar;
class RooSimultaneous;
class RooWorkspace;
class TCanvas;
class TPad;
class TLatex;

class AtlHistFactoryPlotterTask : public AtlHistFactoryTask {

  private:
    TFile *fModelFile;  //! Pointer to Model File

    TCanvas *fCanvas;      // Pointer to current canvas
    TPad *fMainPad;        // Pad for stack plot
    TPad *fRatioPad;       // Pad for ratio plot
    Double_t fRatioHeight; // Relative height of the ratio w.r.t. main pad
    
    TLatex *fCMSEnergyLabel;
    TLatex *fLumiLabel;
    
    Bool_t fRemoveDataErrorX;
    
  public:
    AtlHistFactoryPlotterTask(const char* name, const char* title);
    virtual ~AtlHistFactoryPlotterTask();
    virtual void ExecInteractiveJob(Option_t *option);
    virtual void Initialize();
    
  private:
    void CreatePlot();
    TLatex* GetAtlasLabel(Double_t x, Double_t y,
			  const char* text);
    Double_t GetGammaUncertainty(const char* name,
				 RooWorkspace *wSpace);

    ClassDef(AtlHistFactoryPlotterTask, 0) // DOCUMENT ME
};
#endif
