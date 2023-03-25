//___________________________________________________________
//
// A++ Systematics Tool
//
//
//
//
// Author: Soeren Stamm <mailto:Soeren.Stamm@physik.hu-berlin.de> 
// Copyright: 2013 (C) Soeren Stamm
//
//
//
#ifndef ATLAS_AtlSystematicsTool
#include <AtlSystematicsTool.h>
#endif
#include <AtlEvent.h>

#ifndef __CINT__
ClassImp(AtlSystematicsTool);
#endif


//___________________________________________________________

AtlSystematicsTool::AtlSystematicsTool(const char* name, const char* title) : 
AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
}

//___________________________________________________________

AtlSystematicsTool::~AtlSystematicsTool(){
    //
    // default destructor
    //
}

//___________________________________________________________

void AtlSystematicsTool::SetBranchStatus() {
    //
    // Switch on needed branches
    //
}

//___________________________________________________________

void AtlSystematicsTool::Clear(){
    //
    //  clear the dynamically allocated objects 
    //
}

//___________________________________________________________

void AtlSystematicsTool::BookHistograms(){
    //
    // Book histograms
    //
}

//___________________________________________________________

Bool_t AtlSystematicsTool::AnalyzeEvent() {
    //
    // Event analysis         
    //
    return kTRUE;
}
