//____________________________________________________________________
//
// ========================
//  Atlas TRT digitisation
// ========================
//
// Overview of the identifiers for the TRT detector elements.
// ----------------------------------------------------------
//
// Definition and the range of values for the levels of the
// identifiers are:
//
// element           range              meaning
// -------           -----              -------
//
// fEndCapOrBarrel   +/- 1            positive/negative barrel 
//                   +/- 2            positive/negative endcap 
//                   0 to 17          endcap wheel range 
//
// fPhiSector        0 to 31          for barrel modular geometry 
//                      0             for barrel "simple layer" geometry 
//                   0 to 31          for endcap 
//
// fModuleOrWheel    0 to 2           barrel module layer number 
//                   0 to 72          is straw layer for barrel "simple layer" geometry 
//
// fStrawLayer       0 to 18 (19)     for barrel module 0 for modular (layer) geometry 
//                   0 to 23          for barrel module 1       "        "      " 
//                   0 to 29 (28)     for barrel module 2       "        "      " 
//                       0            for barrel "simple layer" geometry 
//                   0 to 15 or 7     for ec (middle modules have 8 layers,  
//                                           224 total layers in ec) 
//
// fStraw            0 to <28         for barrel, depends upon layer 
//                   0 to <981        for barrel "simple layer" geometry 
//                   0 to <23         for ec (fewer for last 4 modules/wheels due 
//                                           to smaller inner radius) 
//
//  

//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTRTDigit
#include <AtlTRTDigit.h>
#endif
#include <iostream>
#ifndef ROOT_TString
#include <TString.h>
#endif


using namespace std;

#ifndef __CINT__
ClassImp(AtlTRTDigit);
#endif

//____________________________________________________________________

AtlTRTDigit::AtlTRTDigit() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlTRTDigit::AtlTRTDigit(Int_t Id, Float_t DriftTime, Float_t DriftRadius,
			 UInt_t Digit) :
    HepDriftChamberDigit(Id, DriftTime, DriftRadius), fDigit(Digit), 
    fEndCapOrBarrel(0), fPhiSector(), fModuleOrWheel(0), fStrawLayer(0),
    fStraw(0) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlTRTDigit::~AtlTRTDigit() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlTRTDigit::Clear(Option_t* option) {
    //
    // Clear this object
    //
    HepDriftChamberDigit::Clear(option);    
    fDigit          = 0;
    fEndCapOrBarrel = 0;
    fPhiSector      = 0;
    fModuleOrWheel  = 0;
    fStrawLayer     = 0;
    fStraw          = 0;
}

//____________________________________________________________________

void AtlTRTDigit::Print(Option_t* option) const {
    //
    // Print out TRT hit
    //
    // Options available:
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of TRT hits
    //
    TString opt = option;
    opt.ToLower();

    // Print header
    if( !opt.Contains("nohead") ) PrintHeader();
        
    // Print
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.precision(3);
    cout.width(4);  cout << fId;
    cout.width(14); cout << DriftTime();
    cout.width(15); cout << DriftRadius();
    cout.width(13); cout << FittedRadius();
    cout.width(4);  cout << "" ;
    
    // Print bit pattern
    for(Int_t bit=1; bit<28;++bit){
	if( bit == 1 || bit == 10 || bit == 19 ) cout << " ";
	cout << ( (fDigit >> (27-bit)) & 0x1 );
	if( bit == 1 || bit == 10 || bit == 19 ) cout << " ";
    }

    // Print HT flag
    if ( IsHighThrHit() ) cout << " HT    ";
    else cout << "       ";
    
    // Print Time-over-Threshold
    cout.width(6); cout << TimeOverThreshold("prsnt") << endl;
    
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
  
}

//____________________________________________________________________

void AtlTRTDigit::PrintHeader() {
    //
    // Print information header
    //
    cout << "--------------------------------------------------------------------------------------------------"
	 << endl
	 << "  Id  T_Drift (ns)  R_Drift (mum)  R_Fit (mum)  Digit: BC 1       BC 2       BC 3        ToT (ns)"
	 << endl
	 << "--------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void AtlTRTDigit::PrintFooter() {
    //
    // Print footer
    //
    cout << "--------------------------------------------------------------------------------------------------"
	 << endl;
}



