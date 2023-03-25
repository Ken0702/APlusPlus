//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlDataMCPlotterTask
#define ATLAS_AtlDataMCPlotterTask
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ATLAS_AtlSample
#include <AtlSample.h>
#endif
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif
#ifndef ATLAS_AtlPlotterFolder
#include <AtlPlotterFolder.h>
#endif

class AtlDataMCPlotterTask : public AtlTask {
    
  protected:
    TString *fRootScript;              // Path to Root script used 

    TList   *fListOfUserEnvs;          // List of user environment variables
    TList   *fListOfFolders;           // List of all folders 
    TList   *fNtuplePlotCmds;          // List of ntuple plot commands    

    Float_t  fDataLuminosity;          // Data luminosity for plotter
    
    TString *fLuminosityLabel;         // Luminosity Label for Plotter
    TString *fCenterOfMassEnergyLabel; // CenterOfMassEnergyLabel for plotter
    TString *fAtlasLabelStatus;        // Status label for plotter (default: "Work in Progress")
    Bool_t   fDrawDataZeroEntryErrors; // Flag for plotter (default = false)
    Bool_t   fUseOverflowUnderflow;    // Flag for plotter (default = false)
    Bool_t   fDrawMCError;             // Flag for plotter (default = ktrue)
    Bool_t   fExportPDF;               // Flag for plotter (default = kfalse)
    Bool_t   fDrawSignalOverlay;       // Flag for drawing the signal MC (assumed to be the least entry in the MC histogram stack) as overlay instead as part of the stack (default = false)
    Float_t  fScaleOverlay;            // Scale factor of the overlay histogram ( default = 1. )
    Bool_t   fDrawSignificance;        // Draw significance panel
    
  public:
    AtlDataMCPlotterTask(const char* name, const char* title);
    virtual ~AtlDataMCPlotterTask();
    
    void CreateRootScript(Option_t *option);
    virtual void CreateRunScript(Option_t *option = "");
    virtual void ExecInteractiveJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    void AddUserEnv(const char* env);
    
    virtual void ExecGridJob(const Option_t*);
    virtual void ExecNAFBatchJob(const Option_t*);
    virtual void CreateNAFBatchRunScript();
    virtual void CreateGridRunScript();

    void SetAtlasLabelStatus(const char* label); 
    void SetCenterOfMassEnergyLabel(const char* label);
    void SetLuminosityLabel(const char* label);
    void SetDrawDataZeroEntryErrors(Bool_t status);
    void SetDrawMCError(Bool_t status);
    void SetExportPDF(Bool_t status);
    void SetUseOverflowUnderflow(Bool_t status);
    void SetDrawSignalOverlay(Bool_t status);
    void SetScaleOverlay(Float_t scale);
    inline void SetDrawSignificance(Bool_t b = kTRUE) { fDrawSignificance = b;}
    
    inline void AddFolders(TList *folders) {
      fListOfFolders->AddAll(folders);
    }
    inline void AddNtuplePlotCmds(TList *NtuplePlotCmds) {
      fNtuplePlotCmds->AddAll(NtuplePlotCmds);
    }
    inline void SetDataLuminosity(Double_t luminosity) {
	fDataLuminosity = luminosity;
    }
    
    ClassDef(AtlDataMCPlotterTask,0) // Task for creating Data-MC stacked plots
};
#endif
