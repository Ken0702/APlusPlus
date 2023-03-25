//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlToolCut
#define ATLAS_AtlToolCut
#ifndef ATLAS_AtlCut
#include <AtlCut.h>
#endif

class TObjArray;
class TObjString;
class TString;

class AtlToolCut : public AtlCut {

  public:
    AtlToolCut(const char* tool, const char* var, const char* val);
    virtual ~AtlToolCut();
    TString GetTool();
    TString GetVariable() const;
    
    ClassDef(AtlToolCut,1) // Tool cut object for A++ analysis selectors
};
#endif
