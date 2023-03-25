//____________________________________________________________________
//
// Small helper class describing cuts used by A++ analysis tools
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlToolCut
#include <AtlToolCut.h>
#endif
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#ifndef __CINT__
ClassImp(AtlToolCut);
#endif

//____________________________________________________________________

AtlToolCut::AtlToolCut(const char* tool, const char* var,
		       const char* val) : AtlCut("", "") {
    //
    // Default constructor
    //
    TString name(tool);
    name.Append(";;");
    name.Append(var);
    SetNameTitle(name, val);
}

//____________________________________________________________________

AtlToolCut::~AtlToolCut() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

TString AtlToolCut::GetTool() {
    //
    // Return tool name
    //
    TObjArray *tokens = fName.Tokenize(";;");
    return  ((TObjString*)tokens->At(0))->GetString();
}

//____________________________________________________________________

TString AtlToolCut::GetVariable() const {
    //
    // Return variable name
    //
    TObjArray *tokens = fName.Tokenize(";;");
    return ((TObjString*)tokens->At(1))->GetString();
}
