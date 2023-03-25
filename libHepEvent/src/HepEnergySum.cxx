//____________________________________________________________________
//
// Energy sums in an collision event
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepEnergySum
#include <HepEnergySum.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepEnergySum);
#endif

//____________________________________________________________________

HepEnergySum::HepEnergySum() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

HepEnergySum::~HepEnergySum() {
    //
    // Default ddestructor
    //
}

//____________________________________________________________________

void HepEnergySum::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fSumEt = 0.;
    fMissingEt.Set(0., 0.);
}

//____________________________________________________________________

void HepEnergySum::Print(Option_t *option) const {
    //
    // Print object information
    //
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout << endl
	 <<"Sum(Et) = " << fSumEt << " GeV" << endl;
    cout << "Missing Energy : |Et| = " << MissingEt_Mag()
	 << " GeV,   phi = " << MissingEt_Phi()*TMath::RadToDeg()
	 << " deg" << endl;
}

