//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlAnalysisTool
#define ATLAS_AtlAnalysisTool

#include <AtlEvent.h>    // for AtlEvent
#include <Rtypes.h>      // for kFALSE, kTRUE, ClassDefOverride
#include <RtypesCore.h>  // for Bool_t, Float_t, Int_t, Option_t
#include <TNamed.h>      // for TNamed
#include <TString.h>     // for TString
class AtlSelector;
class TBuffer;
class TClass;
class TFile;
class TMemberInspector;
class TTree;

class AtlAnalysisTool : public TNamed {

  public:
    enum EProcessMode {
	kSystematics,           // tool is the first to run after loading the event
	kObjectsDefinition,     // tool that is used for objects definitions
	kScaleFactor,           // tool calculates pretag and tag event weifht (scale factors)
	kPreAnalysis,           // tool runs before the preselection
	kMainAnalysis,          // tool runs after the preselection and before the main user analysis
	kPostAnalysis,          // tool runs after the main analysis
	kIndividual,            // tool has to be invoked by the user
	kOff                    // tool is switched off (should always be last item in this enumeration)
    };

    EProcessMode fProcessMode;    // Defines when exactly the tool is run (see AtlAnalysisTool class description for details)
    Int_t        fVerbosityLevel; // Indicates the degree of verbosity of tool (0 = no info/debug output, 1 = info, 2 = info+debug)
    TString      fOutputDir;      // Directory in root output file, where histogram dir is created
    
  protected:
    AtlEvent    *fEvent;      // Pointer to current event
    TTree       *fTree;       // Event tree
    TFile       *fOutputFile; // Output file
    TString     *fOption;     // Option string
    AtlSelector *fParent;     // Pointer to parent selector
    
  public:
    AtlAnalysisTool(const char* name, const char* title);
    virtual ~AtlAnalysisTool();
    virtual void SetBranchStatus() = 0;
    virtual void BookHistograms()  = 0;
    virtual void FillHistograms()  = 0;
    virtual void Init();
    virtual void FillHistogramsNoEvtSel() {;}
    virtual Bool_t AnalyzeEvent() { return kTRUE; }
    virtual void SetCutDefaults() {;}
    // void Clear(Option_t* /*option*/) override {
    //     Fatal(__FUNCTION__, "This should not be called, call Clear() instead.");}
    virtual void Clear() {;}
    virtual Bool_t Notify() override { return kTRUE; }
    virtual void Terminate() {;}
    virtual void Print() const;  // hides TNamed::Print(Option_t* option="")
    Bool_t Process();
    AtlAnalysisTool* GetTool(const char* ClassName,
			     const char* ToolName = "",
			     Bool_t force = kFALSE) const;
    static const char* GetProcessModeName(EProcessMode mode);

    inline Float_t GetPreTagEvtWeight() const { return fEvent->GetPreTagEvtWeight(); }
    inline Float_t GetTagEvtWeight() const { return fEvent->GetTagEvtWeight(); }
    inline Bool_t IsObjectsDefinition() const
    { return fProcessMode == kObjectsDefinition; }
    inline Bool_t IsScaleFactor() const
    { return fProcessMode == kScaleFactor; }
    inline Bool_t IsPreAnalysis() const
    { return fProcessMode == kPreAnalysis; }
    inline Bool_t IsMainAnalysis() const
    { return fProcessMode == kMainAnalysis; }
    inline Bool_t IsPostAnalysis() const
    { return fProcessMode == kPostAnalysis; }
    inline Bool_t IsSystematics() const
    { return fProcessMode == kSystematics; }
    inline Bool_t IsIndividual() const
    { return fProcessMode == kIndividual; }
    inline Bool_t IsOff() const
    { return fProcessMode == kOff; }
	
    inline void SetParent(AtlSelector *parent)
    { fParent = parent; }
    inline void SetEvent(AtlEvent *evt)
    { fEvent = evt; }
    inline void SetTree(TTree *t)
    { fTree = t; }
    inline void SetOutputFile(TFile *f)
    { fOutputFile = f; }

    inline void SetOutputDir(const char* dir)
    { fOutputDir = dir; }
    inline const char* GetOutputDir() const
    { return fOutputDir.Data(); }
    
    ClassDefOverride(AtlAnalysisTool,0) // Abstract base class for ATLAS analysis tools
};
#endif

