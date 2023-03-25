//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $$
// Copyright: 2010 (C) Felix Thomas
//
#ifndef HEP_HepTemplateFitter
#define HEP_HepTemplateFitter
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_THSTACK
#include <THStack.h>
#endif
#ifndef ROOT_TDirectory
#include <TDirectory.h>
#endif

class TFile;
class TKey;
class TSystem;
class TFractionFitter;
class HepDataMCSample;
class HepTemplate;
class TObjString;
class TCanvas;
class TROOT;

class HepTemplateFitter : public TTask {

  private:
    TList   *fDataSamples;     // List of data samples (might be also MC)
    TList   *fMCSamples;       // List of MC samples

    TList   *fTemplates;       // List with templates
    TH1F    *fReference;       // Histogram to which the templates are fitted
    
    TString *fExportDir;       // Working directory
    Bool_t   fExportROOT;      // Export all plots to a ROOT file ?
    TFile   *fOutputRootFile;  // Output file containing all plots 

    TFractionFitter *fFitter;
    
  public:
    HepTemplateFitter(const char* name, const char* title);
    virtual ~HepTemplateFitter();
    virtual void Exec(Option_t *option = "");

    HepDataMCSample* AddMCSample(const char* path, const char* label, Float_t xsec); // *MENU*

    HepDataMCSample* AddDataSample(const char* path, const char* label); // *MENU*
    HepDataMCSample* AddDataSample(const char* path, const char* label, Float_t xsec); // *MENU*

    void GenerateTemplate(char* histname, char* templatename, Color_t color);
    void GenerateTemplate(char* histname, char* templatename, Double_t low, Double_t high, Color_t color);
    
    void GenerateReference(char* histname);

    inline void SetWorkingDir(const char* dir)
    { SetExportDir(dir); }
    inline void SetExportDir(const char* dir) {
	fExportDir->Remove(0);
	fExportDir->Append(dir);
    }
    Bool_t GetExportROOT() { return fExportROOT; }
    
  private:
    HepDataMCSample* AddMCSample(const char* path, const char* label, Color_t color,
			 Style_t line_style, Width_t line_width,
			 Style_t fill_style);
    HepDataMCSample* AddDataSample(const char* path, const char* label, Color_t color,
      		 Style_t line_style, Width_t line_width,
      		 Style_t fill_style);

    void         FillTemplates();
    HepTemplate* BuildTemplate(char* histname, char* templatename, Color_t color);
    THStack*     BuildStackPlot();
    ClassDef(HepTemplateFitter,0) // Template Fitter
};
#endif

