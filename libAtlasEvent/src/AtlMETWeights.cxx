//____________________________________________________________________
//
// Matching of trigger objects to offline reconstructed objects
// 

//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Feli Thomas
//
#ifndef ATLAS_AtlMETWeights
#include <AtlMETWeights.h>
#endif

#ifndef __CINT__
ClassImp(AtlMETWeights);
#endif

//____________________________________________________________________

AtlMETWeights::AtlMETWeights() {
    //
    // Default constructor
    //

    fMETw_tight_Px = 0.;
    fMETw_tight_Py = 0.;
    fMETw_tight_Et = 0.;
    fMETw_medium_Px = 0.;
    fMETw_medium_Py = 0.;
    fMETw_medium_Et = 0.;

}

//____________________________________________________________________

AtlMETWeights::~AtlMETWeights() {
    //
    // Default destructor
    //
}

//____________________________________________________________________


void AtlMETWeights::Clear(Option_t *option) {
    //
    // Clear this object
    //

    fMETw_tight_Px = 0.;
    fMETw_tight_Py = 0.;
    fMETw_tight_Et = 0.;
    fMETw_medium_Px = 0.;
    fMETw_medium_Py = 0.;
    fMETw_medium_Et = 0.;
    
}

