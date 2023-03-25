//____________________________________________________________________
//
// Single-top D3PD conversion task
// 
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlD3PDConversionTask
#include <AtlD3PDConversionTask.h>
#endif
#include <AtlSingleTopD3PDConverter.h>
#include <TString.h>
#include <TChain.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlD3PDConversionTask);
#endif

//____________________________________________________________________

AtlD3PDConversionTask::AtlD3PDConversionTask(const char* name,
					     const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    SetInputTreeName("RecoTree");
    SetCtrlPlots(kFALSE);
}

//____________________________________________________________________

AtlD3PDConversionTask::~AtlD3PDConversionTask() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

void AtlD3PDConversionTask::InitSelector() {
    //
    // Initialise the D3PD conversion selector
    //
    Info("InitSelector", "Set output file to %s", fOutputFileName->Data());
    fSelector = new AtlSingleTopD3PDConverter(fOutputFileName->Data());
}
