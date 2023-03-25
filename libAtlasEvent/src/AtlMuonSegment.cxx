//____________________________________________________________________
//
// Track segment in outer muon chambers
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMuonSegment
#include <AtlMuonSegment.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMuonSegment);
#endif

//____________________________________________________________________

AtlMuonSegment::AtlMuonSegment() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlMuonSegment::AtlMuonSegment(Int_t Id, Float_t Chi2, Int_t NDoF,
			       Float_t Xref, Float_t Yref, Float_t Zref,
			       Float_t Phi0, Float_t QovR, Float_t QxD0,
			       Float_t Z0, Float_t TDip, Float_t CovMat[15]) :
    HepTrackHelix(Id, Chi2, NDoF, Xref, Yref, Zref, Phi0, QovR, QxD0, Z0, TDip, CovMat) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlMuonSegment::~AtlMuonSegment() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

const HepMagneticField* AtlMuonSegment::GetMagneticField() const {
    //
    // Return magnetic field
    //
    static HepMagneticField *b_field = 0;
    if ( b_field == 0 ) {
	b_field = new HepMagneticField(0., 0., 2.3,
				       HepMagneticField::kToroid);
    }
    return b_field;
}


