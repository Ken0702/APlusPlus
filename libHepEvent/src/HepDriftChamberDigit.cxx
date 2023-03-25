//____________________________________________________________________
//
// Drift chamber digitisation object (hit)
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepDriftChamberDigit
#include <HepDriftChamberDigit.h>
#endif
#include <iostream>
#include <TString.h>

using namespace std;

#ifndef __CINT__
ClassImp(HepDriftChamberDigit);
#endif

//____________________________________________________________________

HepDriftChamberDigit::HepDriftChamberDigit() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

HepDriftChamberDigit::HepDriftChamberDigit(Int_t Id, Float_t DriftTime,
					   Float_t DriftRadius) : 
  HepDigit(Id), fDriftTime(DriftTime), fDriftRadius(DriftRadius), 
  fFittedRadius(0.), fT0(0.){
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepDriftChamberDigit::~HepDriftChamberDigit() {
    //
    // Default destructor
    //    
}

//____________________________________________________________________

void HepDriftChamberDigit::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fId = 0;
    fDriftTime = 0.;
    fDriftRadius = 0.;
    fFittedRadius = 0.;
    fT0 = 0.;
}

//____________________________________________________________________

void HepDriftChamberDigit::Print(Option_t *option) const {
    //
    // Print digitisation information
    //
    TString opt = option;
    opt.ToLower();
    
    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
    
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(14); cout << DriftTime();
    cout.width(15); cout << DriftRadius();
    cout.width(13); cout << FittedRadius();
    cout << endl;
    
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepDriftChamberDigit::PrintHeader() {
    //
    // Print information header
    //
    cout << "-----------------------------------------------------------------"
	 << endl
	 << "  Id  T_Drift (ns)  R_Drift (mum)  R_Fit (mum)"
	 << endl
	 << "-----------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void HepDriftChamberDigit::PrintFooter() {
    //
    // Print footer
    //
    cout << "-----------------------------------------------------------------"
	 << endl;
}

