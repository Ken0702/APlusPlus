
//
// Template inside HepTemplateFitter
// Helper class for creating the template fits
//
//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $$
// Copyright: 2010 (C) Felix Thomas
//
#ifndef HEP_HepTemplate
#include <HepTemplate.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepTemplate);
#endif

//____________________________________________________________________

HepTemplate::HepTemplate() {
    //
    // Default constructor
    //
    fInputHistogram     = 0;
    fFittedHistogram    = 0;
    fBoundsSet          = kFALSE;
    fUpperBound         = 1;
    fLowerBound         = 0;
    fFitFraction        = 0;
    fFitFractionError   = 0;
    fStartValue         = 0.5;
    fStartValueError    = 0.25;
    fStartValueSet      = kFALSE;
    fFixFraction        = kFALSE;
    fCutEfficiency      = 0;
}

//____________________________________________________________________

HepTemplate::~HepTemplate() {
    //
    // Default destructor
    //

    fInputHistogram     = 0;
    fFittedHistogram    = 0;
}

//____________________________________________________________________
