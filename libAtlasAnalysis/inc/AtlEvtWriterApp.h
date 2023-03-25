//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterApp
#define ATLAS_AtlEvtWriterApp
#ifndef ATLAS_AtlEvtWriterBase
#include <AtlEvtWriterBase.h>
#endif

class AtlEvtWriterApp : public AtlEvtWriterBase {

private:

public:
    AtlEvtWriterApp();
    virtual ~AtlEvtWriterApp();
    virtual void BookTree(TTree *tree, AtlEvent *evt);
    virtual void WriteEvent();
    
    ClassDef(AtlEvtWriterApp,0) // Event writer for A++ events
};
#endif

