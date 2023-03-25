//____________________________________________________________________
//
// A++ event tree info object
//
// The event tree info is usually stored as static data member in the
// AtlEventHeader class (in the AOD2A++ converter as well as in the
// A++ analysis selector).
//
//
// ATHENA version
// ==============
//
// The version number(s) of the ATHENA version used for the A++ event
// tree creation can be retrieved with the member functions
// AthenaVersionMajor(), AthenaVersionMinor(),
// AthenaVersionRevision(). The verion numbers are taken from the
// environment variables $AtlasVersion, $AtlasPatchVersion and $AtlasPatch
//
// Examples:
//     AtlEvtTreeInfo *info = AtlEventHeader::GetEvtTreeInfo();
//     cout << "ATHENA VERSION = "
//	<< info->AthenaVersionMajor() << "."
// 	<< info->AthenaVersionMinor() << "."
//	<< info->AthenaVersionRevision() << endl;
//
//     if ( inf->AthenaVersionMajor() >= 15 ) {
//         // ATHENA release 15 and later ...
//     }
//
//     if ( inf->AthenaVersionMajor()    == 14 &&
//          inf->AthenaVersionMinor()    == 5  &&
//          inf->AthenaVersionRevision() == 0 ) {
//          // ATHENA release 14.5.0 section ...
//     }
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtTreeInfo
#include <AtlEvtTreeInfo.h>
#endif
#include <TSystem.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#ifndef __CINT__
ClassImp(AtlEvtTreeInfo);
#endif

//____________________________________________________________________

AtlEvtTreeInfo::AtlEvtTreeInfo() {
    //
    // Default constructor
    //
    for ( Int_t i = 0; i < 5; i++ ) fAthenaVersion[i] = 0;
}

//____________________________________________________________________

AtlEvtTreeInfo::AtlEvtTreeInfo(const char* name, const char* title) :
    TNamed(name, title) {
    //
    // Normal constructor
    //
    ReadAthenaVersion();
    fBeamEnergy = 0.;
}

//____________________________________________________________________

AtlEvtTreeInfo::~AtlEvtTreeInfo() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtTreeInfo::ReadAthenaVersion() {
    //
    // Read the currently used ATHENA version from the environment
    // variables $AthenaVersion, $AthenaPatchVersion and $AthenaPatch
    //
    TString AtlasVersion(gSystem->Getenv("AtlasPatchVersion"));
    if ( AtlasVersion.Length() == 0 )
	AtlasVersion.Append(gSystem->Getenv("AtlasVersion"));
    if ( AtlasVersion.Length() == 0 ) {
	Error("ReadAthenaVersion",
	      "Environment variable AtlasVersion not set. Abort!");
	gSystem->Abort(0);
    }
    TObjArray *arr = AtlasVersion.Tokenize(".");
    for ( Int_t i = 0; i < arr->GetEntries(); i++ ) {
	fAthenaVersion[i] = ((TObjString*)arr->At(i))->GetString().Atoi();
    }
    arr->Delete(); delete arr;
    fAthenaPhysicsAnalysisName.Remove(0);
    fAthenaPhysicsAnalysisName.Append(gSystem->Getenv("AtlasPatch"));
}

