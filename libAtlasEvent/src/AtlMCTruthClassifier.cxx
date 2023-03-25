//____________________________________________________________________
//
// MC truth classifier class (see AnalysisTop)
// 
// For now only the truth particle origin information is contained,
// but the class can easily be extended.
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2016 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMCTruthClassifier
#include <AtlMCTruthClassifier.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMCTruthClassifier);
#endif

static const char* fgParticleOriginNames[AtlMCTruthClassifier::kNumParticleOrigin] = {
    "NonDefined",
    
    "SingleElec",
    "SingleMuon",
    "SinglePhot",
    "SingleTau",
	
    "PhotonConv",
    "DalitzDec",
    "ElMagProc",
    "Mu",
    "TauLep",
    "top",
    "QuarkWeakDec",
    "WBoson",
    "ZBoson",
    "Higgs",
    "HiggsMSSM",
    "HeavyBoson",
    "WBosonLRSM",
    "NuREle",
    "NuRMu",
    "NuRTau",
    "LQ",
    
    "SUSY",
    "LightMeson",
    "StrangeMeson",
    "CharmedMeson",
    "BottomMeson",
    "CCbarMeson",
    "JPsi",
    "BBbarMeson",
    "LightBaryon",
    "StrangeBaryon",
    "CharmedBaryon",
    "BottomBaryon",
    "PionDecay",
    "KaonDecay",
    "BremPhot",
    "PromptPhot",
    "UndrPhot",
    "ISRPhot",
    "FSRPhot",
    "NucReact",
    "PiZero",
    "DiBoson",
    "ZorHeavyBoson",
    "QCD"
};

//____________________________________________________________________

AtlMCTruthClassifier::AtlMCTruthClassifier() {
    //
    // Default constructor
    //
    fParticleOrigin = kNonDefined;
}

//____________________________________________________________________

AtlMCTruthClassifier::AtlMCTruthClassifier(EParticleOrigin ParticleOrigin) {
    //
    // Default constructor
    //
    fParticleOrigin = ParticleOrigin;
}

//____________________________________________________________________

AtlMCTruthClassifier::~AtlMCTruthClassifier() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlMCTruthClassifier::Clear(Option_t *option) {
    //
    // Clear this object
    //
    fParticleOrigin = kNonDefined;
}

//____________________________________________________________________

const char* AtlMCTruthClassifier::GetParticleOriginName(EParticleOrigin ParticleOrigin) {
    //
    // Get human-readable name of particle origin
    //
    return fgParticleOriginNames[ParticleOrigin];
}

//____________________________________________________________________

AtlMCTruthClassifier::EParticleOrigin AtlMCTruthClassifier::GetParticleOriginByName(const char *name) {
    //
    // Get enum value for full name for particle origin
    //
    // In case no origin was found for the given name the value
    // kNonDefined is returned
    //
    for ( Int_t i = 0; i < kNumParticleOrigin; i++) {
        if ( strcasecmp(fgParticleOriginNames[i], name ) == 0 )
            return (EParticleOrigin)i;
    }
    return kNonDefined;
}



