//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryBreakdownEntry
#define ATLAS_AtlHistFactoryBreakdownEntry
#ifndef ATLAS_AtlHistFactoryTask
#include <AtlHistFactoryTask.h>
#endif

class RooRealVar;

class AtlHistFactoryBreakdownEntry : public AtlHistFactoryTask {

private:
    TString fParameter;
    
public:
    AtlHistFactoryBreakdownEntry(const char *name, const char* title,
				 const char *par);
    virtual ~AtlHistFactoryBreakdownEntry();

    virtual void CreateRunScript(Option_t *option = "");
    virtual Bool_t ExecBatchJob(Option_t *option);
    
    static void RunEntry(const char* inputfile,
			 const char* outputfile,
			 const char* dataset,
			 const char* param,
			 Double_t precision = 0.05);

 private:
	static Bool_t IsJesComponent(const RooRealVar* par);
	static Bool_t IsLepComponent(const RooRealVar* par);
    
    ClassDef(AtlHistFactoryBreakdownEntry,0) // DOCUMENT ME
};
#endif

