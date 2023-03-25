//____________________________________________________________________
//
//  Base class of an ATLAS silicon cluster

//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlSilicon3DHit
#include "AtlSilicon3DHit.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


#ifndef __CINT__
ClassImp(AtlSilicon3DHit);
#endif

//____________________________________________________________________

AtlSilicon3DHit::AtlSilicon3DHit() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlSilicon3DHit::AtlSilicon3DHit(Int_t Id, Float_t X, Float_t Y,
				 Float_t Z) : 
  Hep3DHit(Id, X, Y, Z), fEndCapOrBarrel(0), fEtaModule(0), 
  fPhiModule(0), fLayerDisk(0) {
  //
  // Normal constructor
  //
}

//____________________________________________________________________

AtlSilicon3DHit::~AtlSilicon3DHit() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlSilicon3DHit::Clear(Option_t* option) {
  //
  // Clear this object
  //
  Hep3DHit::Clear(option);
  fEndCapOrBarrel = 0;
  fPhiModule      = 0;
  fEtaModule      = 0;
  fLayerDisk      = 0;
}

//____________________________________________________________________

void AtlSilicon3DHit::Print(Option_t *option) const {
    //
    // Print digitisation information
    //
    TString opt = option;
    opt.ToLower();
    
    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
        
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void AtlSilicon3DHit::PrintHeader() {
    //
    // Print information header
    //
}

//____________________________________________________________________

void AtlSilicon3DHit::PrintFooter() {
    //
    // Print footer
    //
}
