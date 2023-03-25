//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterBase
#define ATLAS_AtlEvtWriterBase
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#ifndef ROOT_TTree
#include <TTree.h>
#endif

class AtlEvtWriterBase : public TObject {

protected:
    TTree    *fTree;  // Output tree
    AtlEvent *fEvent; // Event object
    
public:
    AtlEvtWriterBase();
    virtual ~AtlEvtWriterBase();
    virtual void BookTree(TTree *tree, AtlEvent *evt) = 0;
    virtual void WriteEvent() = 0;
    virtual TTree* CreateTree(const char* name, const char* title,
			      TTree *InputTree);
    virtual const char* GetNameOfType() { return ""; }
    virtual void Terminate() {;}
    
    ClassDef(AtlEvtWriterBase,0) // Abstract base class for writing events
};
#endif

