//  
// Author: Felix Thomas <mailto: FelixThomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2010 (C) Felix Thomas 
//

#ifndef ATLAS_AtlSystematicsTool
#define ATLAS_AtlSystematicsTool
#ifndef ATLAS_AnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ROOT_AtlSelector
#include <AtlSelector.h>
#endif
#include <TRandom3.h>
#include <TFile.h>

class TH1F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

class AtlSystematicsTool : public AtlAnalysisTool {

  public:

    AtlSystematicsTool(const char* name, const char* title);
    virtual ~AtlSystematicsTool();

    virtual void   SetBranchStatus();
    virtual void   BookHistograms();
    virtual void   FillHistograms() {;}
    virtual Bool_t AnalyzeEvent();
    virtual void   Clear();

    ClassDef(AtlSystematicsTool,0) // A++ Tool for calculating Scale-Factors
};
#endif
