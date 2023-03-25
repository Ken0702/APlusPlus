//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCut
#define ATLAS_AtlCut
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class AtlCut : public TNamed {

  public:
    AtlCut(const char* var, const char* val);
    virtual ~AtlCut();
    virtual const char* GetVar() const { return GetName(); }
    const char* GetVal() const { return GetTitle(); }
    void SetVal(const char* val) { SetTitle(val); }

    ClassDef(AtlCut,0) // Cut object for A++ analysis selectors
};
#endif

