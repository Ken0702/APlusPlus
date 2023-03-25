//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlRunEvtSelectionTool
#define ATLAS_AtlRunEvtSelectionTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#include <vector>

class TList;
class TFile;
class TTree;
class TString;
class TSystem;
class TROOT;

class AtlRunItem : public TObject {
    // Small helper class
public:
    AtlRunItem(Int_t run) {
	fRunNr = run;
	fEvents = new std::vector<int>;
    }
    virtual ~AtlRunItem() {
	delete fEvents;
    }
    virtual Bool_t IsSortable() const { return kTRUE; }
    virtual Int_t Compare(const TObject *obj) const {
	AtlRunItem *item = (AtlRunItem*)obj;
	if ( item->fRunNr < fRunNr ) return 1;
	if ( item->fRunNr > fRunNr ) return -1;
	return 0;
    }
    Int_t             fRunNr;  // Run number
    std::vector<int> *fEvents; // List of events for this run
};

class AtlRunEvtSelectionTool : public AtlAnalysisTool {

  private:
    TList      *fListOfRuns;  // Ordered list of selected runs
    AtlRunItem *fLastRun;     // Run number cache
    Int_t       fNRuns;       // Total number of included runs
    Int_t       fNEvents;     // Total number of added events
    
  public:
    TString  fInputFilename;   // Name of the input file (text or Root file)
    TString  fInputFormat;     // Format string for text input files (default is %d%d)
    TString  fInputTreename;   // Name of the inpu tree
    TString  fBranchnameRun;   // Name of the run branch
    TString  fBranchnameEvent; // Name of the event branch
    
  public:
    AtlRunEvtSelectionTool(const char* name, const char* title);
    virtual ~AtlRunEvtSelectionTool();
    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual void SetBranchStatus();
    virtual void Print() const;
    void ExportAsText();
    void Reset();
    
    inline Bool_t IsValidEvent() {
	//
	// Check if the current event is contained in the given run/evt list.
	// !!! Important !!! if the event is found it will be removed from
	// list for performance reasons.
	//
	return Contains((Int_t)fEvent->RunNr(), (Int_t)fEvent->EventNr(),
			kTRUE);
    }
    
  private:
    Bool_t Contains(Int_t run, Int_t evt, Bool_t remove = kFALSE);
    void Add(Int_t run, Int_t evt);
    AtlRunItem* AddRun(Int_t xrun);
    AtlRunItem* FindRunItem(Int_t run);
    void ParseInputFile();
    void ParseRootFile();
    void ParseTextFile();
    
    ClassDef(AtlRunEvtSelectionTool,0) // Run/Event selection tool
};
#endif

