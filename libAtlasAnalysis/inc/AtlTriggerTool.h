//  
// Author: Charlotte Santer <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Charlotte Santer
//
#ifndef ATLAS_AtlTriggerTool
#define ATLAS_AtlTriggerTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ATLAS_AtlTriggerToolItem
#include <AtlTriggerToolItem.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif

class TPRegexp;
class TDirectory;
class TGraphAsymmErrors;

class AtlTriggerTool : public AtlAnalysisTool {

  private:
    TList   *fListOfTriggerItems;  // List of all selected trigger items
    TH1F    *fHistCombPt;          // Pt histogram for all item combinations
    TH1F    *fHistCombPhi;         // Phi histogram for all item combinations
    TH1F    *fHistCombEta;         // Eta histogram for all item combinations
    TH1F    *fHistCombTotPt;       // Total Pt histogram for all item combinations
    TH1F    *fHistCombTotPhi;      // Total Phi histogram for all item combinations
    TH1F    *fHistCombTotEta;      // Total Eta histogram for all item combinations
    
  public:
    TString *fTriggerSelection;   // Selection string for (combined) trigger items
    
  public:
    AtlTriggerTool(const char* name, const char* title);
    virtual ~AtlTriggerTool();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual void FillHistogramsNoEvtSel();
    virtual void Terminate();
    virtual void Print(Option_t *option = "") const;
    void CompileSelectionString();
    AtlTriggerToolItem::ETriggerType  GetTriggerType(TString item);
    AtlTriggerToolItem::ETriggerLevel GetTriggerLevel(TString item);
    
    ClassDef(AtlTriggerTool,0) // Trigger tool
};
#endif

