//____________________________________________________________________
//
// =================
//  Atlas Pixel hit
// =================
//
// Overview of the identifiers for the Pixel detector elements.
// ----------------------------------------------------------
//
// Definition and the range of values for the levels of the
// identifiers are:
//
// element           range    bits          meaning
// -------           -----    ----          -------
// 
// fEndCapOrBarrel     0         2           barrel 
//                  -2  / 2               neg ec /  pos ec
// fLayerDisk        0 to 2      2          for barrel
//                   0 to 2      2          for ec
// fPhiModule        0 to <29    5       for barrel (depends upon layer)
//                   0 to <72    7       for ec     (depends upon disk)
// fEtaModule        0 to 12     4       for barrel, along z
//                   0 to 1      1       for ec, along r
// fPhiIndex         0 to 327    9       for barrel, pixel coordinate along r/phi
//                   0 to 192    8       for ec,      "
// fEtaIndex         0 to 205    8       for barrel, pixel coordinate along z
//                   0 to 164    8       for ec,       "       "        "   r
//
//

//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlPixelHit
#include "AtlPixelHit.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


#ifndef __CINT__
ClassImp(AtlPixelHit);
#endif

//____________________________________________________________________

AtlPixelHit::AtlPixelHit() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlPixelHit::AtlPixelHit(Int_t Id, Float_t X, Float_t Y, Float_t Z) : 
  AtlSilicon3DHit(Id, X, Y, Z), fTotalCharge(0.), fToT(0),
  fPhiIndex(0), fEtaIndex(0) {
  //
  // Normal constructor
  //
}

//____________________________________________________________________

AtlPixelHit::~AtlPixelHit() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlPixelHit::Clear(Option_t* option) {
  //
  // Clear this object
  //
  AtlSilicon3DHit::Clear(option);
  fTotalCharge = 0.;
  fToT         = 0;
  fEtaIndex    = 0;
  fPhiIndex    = 0;
}

//____________________________________________________________________

void AtlPixelHit::Print(Option_t *option) const {
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

void AtlPixelHit::PrintHeader() {
    //
    // Print information header
    //
}

//____________________________________________________________________

void AtlPixelHit::PrintFooter() {
    //
    // Print footer
    //
}
