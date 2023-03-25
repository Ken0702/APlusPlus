//
//    Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
//    Update: $$
//    Copyright: 2010 (C) Felix Thomas
//

#ifndef ATLAS_AtlObjectsDefinitionTool
#define ATLAS_AtlObjectsDefinitionTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#include <fstream>
#include <iostream>

class AtlEvent;
class TString;
class TROOT;

class AtlObjectsDefinitionTool : public AtlAnalysisTool {

 public:
    AtlObjectsDefinitionTool(const char* name, const char* title);
    virtual ~AtlObjectsDefinitionTool();

    //
    // Getters
    //

    // Objects of signal quality
    virtual TList*   GetLeptons() = 0;    
    virtual TList*   GetElectrons() = 0;
    virtual TList*   GetMuons() = 0;          
    virtual TList*   GetJets() = 0;           
    virtual TList*   GetBJets() = 0;

 protected:
    virtual void   BookHistograms();
    virtual void   FillHistograms();
    virtual void   SetBranchStatus();
    virtual void   SetCutDefaults();
    virtual void   Clear();
    virtual void   Terminate();
    virtual void   Print() const;
    virtual Bool_t AnalyzeEvent();
    
    ClassDef(AtlObjectsDefinitionTool, 0)  // Atlas Objects Definition Tool
};
#endif

