//____________________________________________________________________
//
// ===============
//  Atlas SCT Hit
// ===============
//
// Overview of the identifiers for the SCT detector elements.
// ----------------------------------------------------------
//
// Definition and the range of values for the levels of the
// identifiers are: 
//
// element           range             meaning
// -------           ------            -------
// fEndcapOrBarrel     0                barrel 
//                  -2  / 2         neg ec /  pos ec
// fLayerDisk        0 to 3            for barrel
//                   0 to 8            for ec
// fPhiModule        0 to <56      for barrel (depends upon layer)
//                   0 to <52      for ec     (depends upon wheel)
// fEtaModule       -6 to -1       for neg barrel, along z     
//                   1 to 6        for pos barrel, along z     
//                   0 to 2        for ec, along r 
// fSide             0 to 1        inner/outer of pairs of Si crystals
// fStrip            0 to 767      strip number
//
//

//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlSCT3DHit
#include "AtlSCT3DHit.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


#ifndef __CINT__
ClassImp(AtlSCT3DHit);
#endif

//____________________________________________________________________

AtlSCT3DHit::AtlSCT3DHit() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlSCT3DHit::AtlSCT3DHit(Int_t Id, Float_t X, Float_t Y, Float_t Z) : 
  AtlSilicon3DHit(Id, X, Y, Z), fSide(0), fStrip(0) {
  //
  // Normal constructor
  //
}

//____________________________________________________________________

AtlSCT3DHit::~AtlSCT3DHit() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlSCT3DHit::Clear(Option_t* option) {
  //
  // Clear this object
  //
  AtlSilicon3DHit::Clear(option);
  fSide  = 0;
  fStrip = 0;
}

//____________________________________________________________________

void AtlSCT3DHit::Print(Option_t *option) const {
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

void AtlSCT3DHit::PrintHeader() {
    //
    // Print information header
    //
}

//____________________________________________________________________

void AtlSCT3DHit::PrintFooter() {
    //
    // Print footer
    //
}
