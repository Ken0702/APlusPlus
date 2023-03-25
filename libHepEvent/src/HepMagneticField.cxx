//____________________________________________________________________
//
// Small helper class describing magnetic fields used for track
// parametrisations
// 
// The field can either be solenoidal or toroidal.
//
// (1) Solenoid field:
//
// The direction of the solenoid field lines is defined by the vector
// (Bx,By,Bz). Its length gives the strengts of the magnetic field (in
// Tesla).
//
// (2) Toroidal field:
//
// The symmetry axis of the toroid is given by the (Bx,By,Bz)
// vector. Its length defines the strength of the toroidal field (in
// Tesla).
//

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMagneticField
#include <HepMagneticField.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepMagneticField);
#endif

//____________________________________________________________________

HepMagneticField::HepMagneticField(Float_t Bx, Float_t By, Float_t Bz,
				   Int_t Type) : fType(Type) {
    //
    // Default constructor
    //
    fBField.SetXYZ(Bx, By, Bz);
}

//____________________________________________________________________

HepMagneticField::~HepMagneticField() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepMagneticField::Print(Option_t *option) const {
    //
    // Print object information
    //
    switch ( fType ) {
      case kSolenoid:
	cout << "Solenoidal magnetic field";
	break;
      case kToroid:
	cout << "Toroidal magnetic field";
	break;
    }
    cout << " :   strength = " << fBField.Mag() << " T   ";
    TVector3 u = fBField.Unit();
    cout << "direction = (" << u.X() << ", " << u.Y()
	 << ", " << u.Z() << ")" << endl;
}

	

