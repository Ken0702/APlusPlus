//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCPlotter
#define HEP_HepDataMCPlotter
#ifndef HEP_HepDataMCPlot
#include <HepDataMCPlot.h>
#endif
#ifndef HEP_HepDataMCFolder
#include <HepDataMCFolder.h>
#endif
#ifndef HEP_HepNtuplePlotCmd
#include <HepNtuplePlotCmd.h>
#endif
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TDirectory
#include <TDirectory.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef TROOT
#include <TROOT.h>
#endif
#ifndef TStyle
#include <TStyle.h>
#endif

class TFile;
class TKey;
class TSystem;
class HepDataMCSample;
class TObjString;
class TCanvas;
class TROOT;
class TObjArray;

class HepDataMCPlotter : public TTask {

  private:
    TFile   *fOutputRootFile;      // Output file containing all plots 

    TList   *fListOfDataFileNames; // List of data file names
    TList   *fListOfDataFiles;     // List of data files pointers
    TList   *fListOfPlots;         // List of DataMC plots
    TList   *fListOfInFiles;       // List of all input files
    TList   *fNtuplePlotCmds;      // List of ntuple draw commands used by the plotter tasks
    TList   *fMCSamples;           // List of MC samples (input filenames and drawing attributes)
    TList   *fMCSingleSamples;     // List of MC single samples, not belonging to folder (input filenames and drawing attributes)
    TList   *fMCFolders;           // List of folders to group MC samples

    Float_t  fLumiDATA;                // Integrated DATA luminosity (used for over-all normalisation)
    TString *fExportDir;               // Working directory
    TString  fAtlasLabelStatus;        // label in plot after "ATLAS"
    TString  fCenterOfMassEnergyLabel; // Center of mass energy label
    TString  fLuminosityLabel;         // Luminosity label
    Bool_t   fExportROOT;              // Export all plots to a ROOT file 
    Bool_t   fExportEPS;               // Export all plots to EPS files 
    Bool_t   fExportPS;                // Export all plots to PS files 
    Bool_t   fExportPDF;               // Export all plots to PDF  files 
    Bool_t   fExportPNG;               // Export all plots to PNG files 
    Bool_t   fExportC;                 // Export all plots to C files 
    Bool_t   fDrawLegend;              // Draw a legend inside all plots
    Bool_t   fGroupHistograms;         // Group histograms when drawing and in the legend
    Bool_t   fUnifyMCStack;            // Draw MC stack histograms as a single histogram
    Bool_t   fDrawMCError;             // Draw the total MC uncertainty on top
    Bool_t   fDrawDataZeroEntryErrors; // Draw the DATA error bars even for 0 data entry bins
    Bool_t   fLogyaxis;                // Plot y axis in logarithmic scale
    Bool_t   fDrawCMSLumiAtlasLabel;   // Draw labels
    Bool_t   fUseAtlasStyle;	       // Use ATLAS style
    Bool_t   fUseOverflowUnderflow;    // Flag for plotter (default = false)
    Bool_t   fDrawSignalOverlay;       // Flag for drawing the signal MC (assumed to be the least entry in the MC histogram stack) as overlay instead as part of the stack (default = false)
    Float_t  fScaleOverlay;            // Scale factor of the overlay histogram ( default = 1. )
    Bool_t   fDrawSignificance;        // Draw significance panel
    
  public:
    HepDataMCPlotter(const char* name, const char* title);
    virtual ~HepDataMCPlotter();
    void InitInput();
    virtual void  Exec(Option_t *option = "");
    HepDataMCPlot* BuildNtuplePlot(HepNtuplePlotCmd *cmd) const;

    void AddDataFile(const char* path, const char* label); // *MENU*
    HepDataMCSample* AddMCSample(const char* path, const char* label, Color_t color,
				 Style_t line_style, Width_t line_width,
				 Style_t fill_style);
    HepDataMCSample* AddMCSample(const char* path, const char* label, Color_t color); // *MENU*
    HepDataMCSample* AddMCSample(const char* path, const char* label, Float_t xsec,
				 Color_t color); // *MENU*
    HepDataMCSample* AddMCSample(const char* folder_name, const char* path, const char* label,
				 Float_t xsec, Color_t color); // *MENU*
    HepDataMCSample* AddMCSample(HepDataMCFolder *folder, const char* path, const char* label,
				 Float_t xsec, Color_t color);
    void AddMCFolder(const char* name, const char* title, Color_t color);
    void AddNtuplePlotCmd(const char* TreeName, const char* VarExp,
			  const char* Selection, const char* PlotName,
			  const char* PlotTitle, Int_t Nbins,
			  Double_t Xlow, Double_t Xup,
			  const char* XTitle, const char* YTitle);
    void Export(HepDataMCPlot *Plot, TNamed *PlotName);
    
    inline TList* GetListOfPlots() { return fListOfPlots; }
    inline void SetLumiDATA(Float_t lumi)
    { fLumiDATA = lumi; }
    inline void SetWorkingDir(const char* dir)
    { SetExportDir(dir); }
    inline void SetExportDir(const char* dir) {
	fExportDir->Remove(0);
	fExportDir->Append(dir);
    }
    Bool_t GetExportROOT() { return fExportROOT; }
    Bool_t GetExportEPS()  { return fExportEPS; }
    Bool_t GetExportPS()   { return fExportPS; }
    Bool_t GetExportPDF()  { return fExportPDF; }
    Bool_t GetExportPNG()  { return fExportPNG; }
    Bool_t GetExportC()    { return fExportC; }
    Bool_t GetDrawLegend() { return fDrawLegend; }
    Bool_t GetGroupHistograms() { return fGroupHistograms; }
    Bool_t GetUnifyMCStack() { return fUnifyMCStack; }
    Bool_t GetDrawMCError() { return fDrawMCError; }
    Bool_t GetDrawDataZeroEntryErrors() { return fDrawDataZeroEntryErrors; }
    Bool_t GetLogyaxis() { return fLogyaxis; }
    void SetExportROOT(Bool_t ExportROOT) {
	// Switch export to .root format on or off
	fExportROOT = ExportROOT;
    } // *TOGGLE*
    void SetExportEPS(Bool_t ExportEPS) {
	// Switch export to EPS format on or off
	fExportEPS = ExportEPS;
    }   // *TOGGLE*
    void SetExportPS(Bool_t ExportPS) {
	// Switch export to PS format on or off
	fExportPS = ExportPS;
    }     // *TOGGLE*
    void SetExportPDF(Bool_t ExportPDF) {
	// Switch export to PDF format on or off
	fExportPDF = ExportPDF;
    }   // *TOGGLE*
    void SetExportPNG(Bool_t ExportPNG) {
	// Switch export to PNG format on or off
	fExportPNG = ExportPNG;
    }   // *TOGGLE*
    void SetExportC(Bool_t ExportC) {
	// Switch export to .C on or off
	fExportC = ExportC;
    }       // *TOGGLE*
    void SetDrawLegend(Bool_t DrawLegend) {
	// Switch drawing of the legend on or off
	fDrawLegend = DrawLegend;
    } // *TOGGLE*
    void SetGroupHistograms(Bool_t GroupHistograms); // *TOGGLE*
    void SetUnifyMCStack(Bool_t UnifyMCStack); // *TOGGLE*
    void SetDrawMCError(Bool_t DrawMCError); // *TOGGLE*
    void SetDrawDataZeroEntryErrors(Bool_t DrawDataZeroEntryErrors = kTRUE); // *TOGGLE*
    void SetLogyaxis(Bool_t Logyaxis); // *TOGGLE*
    void SetUseOverflowUnderflow(Bool_t status);
    void SetDrawSignalOverlay(Bool_t status);
    void SetScaleOverlay(Float_t scale);
    inline void SetDrawSignificance(Bool_t b = kTRUE) { fDrawSignificance = b;}
    void SetUseAtlasStyle(Bool_t UseAtlasStyle = kTRUE) { 
	//
	// Draw plot in ATLAS style
	//
	fUseAtlasStyle = UseAtlasStyle;
    }
    Bool_t GetUseAtlasStyle(){ return fUseAtlasStyle;}
    inline void SetDrawCMSLumiAtlasLabel(Bool_t DrawCMSLumiAtlasLabel = kTRUE){
	fDrawCMSLumiAtlasLabel = DrawCMSLumiAtlasLabel;
    }
    inline Bool_t GetDrawCMSLumiAtlasLabel(){return fDrawCMSLumiAtlasLabel;}
    inline void SetAtlasLabelStatus(TString AtlasLabelStatus){
	fAtlasLabelStatus = AtlasLabelStatus;
    }
    inline TString  GetAtlasLabelStatus(){ return fAtlasLabelStatus;}
    inline void SetCenterOfMassEnergyLabel(TString CenterOfMassEnergy){
	fCenterOfMassEnergyLabel = CenterOfMassEnergy;
    }
    inline TString  GetCenterOfMassEnergyLabel(){ return fCenterOfMassEnergyLabel;}
    inline void SetLuminosityLabel(TString Luminosity){
	fLuminosityLabel = Luminosity;
    }
    inline TString  GetLuminosityLabel(){ return fLuminosityLabel;}
        
  private:
    void ReadHistogramsFromDir(TDirectory *dir);
    void BuildListOfPlots();
    HepDataMCPlot* BuildPlot(TNamed *PlotName);
    TDirectory* MkDirWithParents(TFile *f, const char* dir);
    void Normalize(TH1F &h, HepDataMCSample *sample, Float_t LumiScale = 1.) const;
    TH1F* GetHistFromFile(TFile *f, TNamed *PlotName);
    HepDataMCPlot* CreatePlotFromHist(TH1F* h) const;
    void AddHistMC(HepDataMCPlot *plot, TH1F *h,
		   HepDataMCSample *mc_sample,
		   HepDataMCFolder *folder = 0,
		   Float_t LumiScale = 1.) const;

    ClassDef(HepDataMCPlotter,3) // DATA/MC plotter
};
#endif

