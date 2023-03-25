//____________________________________________________________________
//
// Abstract base class for ATLAS analysis tools
// 
// The purpose of this class is to provide analysis tools consisting
// of analysis code which might be useful in many analyses (eg a
// particle decay finder).
//
// A tool is added to an analysis selector by using
// AtlSelector::AddTool(). The selector can include more than one tool
// (it is even possible to run the same tool twice with different
// parameters). Tools are named classes and can be accessed via their
// names from the list of tools held by the AtlSelector class.
//
// A tool can be plugged at different positions into the analysis. The
// selector allows for two different user-defnied analysis routines,
// one for a crude event preselection (AtlSelector::ProcessPreCut()),
// and the other for the main analysis and event selection
// (AtlSelector::ProcessCut()). The tool can be placed either before
// both of them, in between or at the very end. A decay finder for
// instance, would be placed best in between the preselection and the
// main event selection. This would allow to run the finder only over
// those events which are of special interest, while not loosing any
// time to analsye the rest of them.
// Additionally there might be the need for tools that either change
// or correct kinematic variables, alter the event weight or fulfill
// other tasks.
// For this a series of insertion points are available. In order to
// see clearly where the tools are invoked, have a look at the Process()
// routine of the AtlSelector class.
// Two process modes have to be mentionend in particular:
// The mode kIndividual gives the user the possibility to invoke the
// tool at a point where the user desires it. The tool is invoked via
// tool->Process(). 
// The mode kOff switches a tool of by removing it from the list of 
// registered tools. This makes it possible to keep code that handles
// the setup of tools while dynamically switching on and off the tool.
//
//
// The steering is done with the (public) datamember fProcessMode.
//
// Inter-tool communications can be performed easily in the following way:
//
//   insidde tool A {
//     ...
//     ClassToolB *toolb = fParent->GetTool("ClassToolB");
//     toolB->DoSomething();
//   }
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#include <AtlAnalysisTool.h>
#include <AtlSelector.h>        // for AtlSelector
#include <iostream>             // for operator<<, endl, basic_ostream, basi...

using namespace std;

static const char* fgProcessModeNames[AtlAnalysisTool::kOff + 1] = {
    "Systematics",
    "ObjectsDefinition",
    "ScaleFactor",
    "PreAnalysis",
    "MainAnalysis",
    "PostAnalysis",
    "Individual",
    "Off"
};

//____________________________________________________________________

AtlAnalysisTool::AtlAnalysisTool(const char* name, const char* title) :
    TNamed(name, title) {
    //
    // Default constructor
    //
    fEvent = 0;
    fTree = 0;
    fOutputFile = 0;
    fOutputDir = "/";
    fProcessMode = kMainAnalysis;
    fVerbosityLevel = 0;
}

//____________________________________________________________________

AtlAnalysisTool::~AtlAnalysisTool() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

const char* AtlAnalysisTool::GetProcessModeName(EProcessMode mode) {
    //
    // Get human-readable name of the process mode
    //
    return fgProcessModeNames[mode];
}

//____________________________________________________________________

Bool_t AtlAnalysisTool::Process() {
    //
    // Analyze current event
    //
    // The return value is only used for AtlObjectsDefinitionTools.
    //
    return AnalyzeEvent();
}

//____________________________________________________________________

AtlAnalysisTool* AtlAnalysisTool::GetTool(const char* ClassName,
					  const char* ToolName ,
					  Bool_t force) const {
    //
    // Find given (other) tool
    //
    return fParent->GetTool(ClassName, ToolName, force);
}

//____________________________________________________________________

void AtlAnalysisTool::Init() {
    //
    // User-defined init function which is executed in AtlSelector::SlaveBegin()
    // just before Print()
    //
}

//____________________________________________________________________

void AtlAnalysisTool::Print() const {
    //
    // Print configuration of base class. Can be integrated into the
    // print function of the derived class.
    //
    cout << "  Process mode    = " << GetProcessModeName(fProcessMode)
	 << endl
	 << "  Verbosity level = " << fVerbosityLevel << endl
         << "  Output dir      = " << fOutputDir << endl;
}
	
    
