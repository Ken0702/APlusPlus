//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef HEP_HepNtuplePlotCmd
#define HEP_HepNtuplePlotCmd
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TString
#include <TString.h>
#endif

class HepNtuplePlotCmd : public TObject {
  
 private:
    TString  fTreeName;   // Name of the tree to use for plotting
    TString  fVarExp;     // Variable expression
    TString  fSelection;  // Selection
    TString  fPlotName;   // Plot name
    TString  fPlotTitle;  // Plot title
    Int_t    fNbins;      // Number of bins
    Double_t fXlow;       // Lower boundary
    Double_t fXup;        // Upper boundary
    TString  fXTitle;     // X axis title
    TString  fYTitle;     // Y axis title
  
 public:
    HepNtuplePlotCmd(const char* TreeName, const char* VarExp,
		     const char* Selction, const char* PlotName,
		     const char* PlotTitle, Int_t Nbins,
		     Double_t Xlow, Double_t Xup, const char* XTitle,
		     const char* YTitle);
    virtual ~HepNtuplePlotCmd();
  
    const char* GetTreeName()const { return fTreeName; }
    const char* GetVarExp() const { return fVarExp; }
    const char* GetSelection() const { return fSelection; }
    const char* GetPlotName() const { return fPlotName; }
    const char* GetPlotTitle() const { return fPlotTitle; }
    Int_t GetNbins() const { return fNbins; }
    Double_t GetXlow() const { return fXlow; }
    Double_t GetXup() const { return fXup; }
    const char* GetXTitle() const { return fXTitle; }
    const char* GetYTitle() const { return fYTitle; }

    ClassDef(HepNtuplePlotCmd,0) // Ntuple plot cmd used by HepDataMCPlotter
};
#endif
