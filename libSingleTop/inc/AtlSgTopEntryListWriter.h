//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlSgTopEntryListWriter
#define SINGLETOP_AtlSgTopEntryListWriter
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif

class AtlSgTopEntryListWriter : public AtlSelector {
    
  private:
    
/*   // Cut-flow histogram */
/*   TH1F *fHistCutFlow_weighted; // Weighted cut-flow histogram: how many events survive each cut? */
/*   TH1F *fHistCutFlow_unweighted; // Unweighted cut-flow histogram: how many events survive each cut?  */
    
  public:
    
    // Trigger item selection
    TString fTriggerChoice;
    
  public:
    AtlSgTopEntryListWriter(const char* OutputFilename);
    virtual ~AtlSgTopEntryListWriter();
    
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print() const;
    virtual void Terminate();
    static const Int_t fgNCuts;
    
    
    ClassDef(AtlSgTopEntryListWriter,0) // Single-top t-Channel analysis
};
#endif

