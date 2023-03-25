//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryMeasurement
#define ATLAS_AtlHistFactoryMeasurement
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TStyle
#include <TStyle.h>
#endif

class TString;
class TList;
class AtlHistFactoryChannel;
class AtlHistFactorySystematic;

class AtlHistFactoryMeasurement : public TNamed {

protected:
    TString *fOutputDir;       // Output directory (workspace, xml files, etc.)
    TString *fOutPrefix;       // Output prefix for HistFactory (may include directories)
    TString *fScheme;          // Name of the plotter folder (default = "plots_datamc")

    TList *fListOfChannels;    // List of all channels

    Double_t fLumi;            // Set Lumi (see RooStats documentation for more details)
    Double_t fLumiRelError;    // Set relative lumi error

    Int_t fBinLow;             // Set lowest bin included in the measurement for all (!) observables
    Int_t fBinHigh;            // Set highest bin included in the measurement for all (!) observables

    TList *fPOIs;              // List of parameters of interest

    Bool_t fUseAsimovData;     // Use Asimov data? (default = kTRUE)
    Bool_t fUsePseudoExp;      // Use Pseudo exp. for shapes? (default = kFALSE)
    Bool_t fExportShapePlots;  // Export shape plots to pdf? (default = kFALSE)
    Bool_t fExportCorrMatrix;  // Export correlation matrix? (default = kFALSE)
    Bool_t fRunPulls;          // Run Pull Plots (default = kFALSE)
    Bool_t fNoSystematics;     // Skip all systematics? (default = kFALSE)
    
public:
    AtlHistFactoryMeasurement(const char* name, const char* title,
			      const char* outdir,
			      const char* scheme = "plots_datamc",
			      const char* out_prefix = "models/atlas");
    virtual ~AtlHistFactoryMeasurement();

    void AddChannel(AtlHistFactoryChannel *ch);
    void AddPOI(const char* poi);

    void SetDrawOptions(const char* sample_name,
			Color_t fill_color, Style_t fill_style,
			Color_t line_color, Style_t line_style = 1);

    TString* GetModelFileName(const char* channel = "combined");
    
    inline TString* GetOutputDir() const { return fOutputDir; }
    inline TString* GetOutPrefix() const { return fOutPrefix; }
    inline TString* GetScheme() const { return fScheme; }
    inline TList*   GetListOfChannels() const { return fListOfChannels; }
    inline Double_t GetLumi() const { return fLumi; }
    inline Double_t GetLumiRelError() const { return fLumiRelError; }
    inline TList* GetPOIs() const { return fPOIs; }
    inline Int_t GetBinLow() const { return fBinLow; }
    inline Int_t GetBinHigh() const { return fBinHigh; }
    inline Bool_t GetUseAsimovData() const { return fUseAsimovData; }
    inline Bool_t GetUsePseudoExp() const { return fUsePseudoExp; }
    inline Bool_t GetExportShapePlots() const { return fExportShapePlots; }
    inline Bool_t GetExportCorrMatrix() const { return fExportCorrMatrix; }
    inline Bool_t GetRunPullPlots() const { return fRunPulls; }
    inline Bool_t GetNoSystematics() const { return fNoSystematics; }
    
    inline void SetLumiAndRelError(Double_t lumi, Double_t relerr) {
	fLumi = lumi;
	fLumiRelError = relerr;
    }
    inline void SetBinLow(Int_t bin) { fBinLow = bin; }
    inline void SetBinHigh(Int_t bin) { fBinHigh = bin; }
    inline void SetUseAsimovData(Bool_t flag = kTRUE) { fUseAsimovData = flag; }
    inline void SetUsePseudoExp(Bool_t flag = kTRUE) { fUsePseudoExp = flag; }
    inline void SetExportShapePlots(Bool_t flag = kTRUE) { fExportShapePlots = flag; }
    inline void SetExportCorrMatrix(Bool_t flag = kTRUE) { fExportCorrMatrix = flag; }
    inline void SetRunPullPlots(Bool_t flag = kTRUE) { fRunPulls = flag; }
    inline void SetNoSystematics(Bool_t flag = kTRUE) { fNoSystematics = flag; }
    
    ClassDef(AtlHistFactoryMeasurement,0) // DOCUMENT ME
};
#endif

