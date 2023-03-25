//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterD3PDSgTop
#define ATLAS_AtlEvtWriterD3PDSgTop
#ifndef ATLAS_AtlEvtWriterBase
#include <AtlEvtWriterBase.h>
#endif

class AtlEvtWriterD3PDSgTop : public AtlEvtWriterBase {

private:
    Float_t fEventWeight; // Total event weight

protected:
    Bool_t  fCloneOnly;   // Don't add branches to output

public:
    AtlEvtWriterD3PDSgTop();
    virtual ~AtlEvtWriterD3PDSgTop();
    virtual void BookTree(TTree *tree, AtlEvent *evt);
    virtual void WriteEvent();
    virtual TTree* CreateTree(const char* name, const char* title,
			      TTree *InputTree);
    virtual const char* GetNameOfType() /*override*/ { return "kD3PDSgTop"; }
    inline void SetCloneOnly(Bool_t co) { fCloneOnly = co; }

    ClassDef(AtlEvtWriterD3PDSgTop,0) // Event writer for SgTop-D3PD format
};
#endif

