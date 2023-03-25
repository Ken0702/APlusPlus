//____________________________________________________________________
//
// MC folder containing AtlSamples used for an A++ plotting task
// 
//
#ifndef ATLAS_AtlPlotterFolder
#include <AtlPlotterFolder.h>
#include <TList.h>
#endif

using namespace std;

#ifndef __CINT__
ClassImp(AtlPlotterFolder);
#endif

//____________________________________________________________________

AtlPlotterFolder::AtlPlotterFolder(const char* Name, const char* Title,
				   const char* Color) : 
    TNamed(Name, Title), fColor(Color) {
    //
    // Default constructor
    //

    fListOfSamples  = new TList;    
}

//____________________________________________________________________

AtlPlotterFolder::~AtlPlotterFolder() {
    //
    // Default destructor
    //
    fListOfSamples->Delete(); delete fListOfSamples;
}

//____________________________________________________________________

void AtlPlotterFolder::AddSample(AtlSample* sample) {
    //
    // Add sample
    //
    fListOfSamples->Add(sample);
}
