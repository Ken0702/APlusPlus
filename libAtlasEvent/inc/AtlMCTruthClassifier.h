//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2016 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMCTruthClassifier
#define ATLAS_AtlMCTruthClassifier
#ifndef ROOT_TObject
#include <TObject.h>
#endif

class AtlMCTruthClassifier : public TObject {

public:
    enum EParticleOrigin {
	kNonDefined        = 0,
	
	kSingleElec        = 1,
	kSingleMuon        = 2,
	kSinglePhot        = 3,
	kSingleTau         = 4,
	
	kPhotonConv        = 5,
	kDalitzDec         = 6,
	kElMagProc         = 7,
	kMu                = 8,
	kTauLep            = 9,
	ktop               = 10,
	kQuarkWeakDec      = 11,
	kWBoson            = 12,
	kZBoson            = 13,
	kHiggs             = 14,
	kHiggsMSSM         = 15,
	kHeavyBoson        = 16,
	kWBosonLRSM        = 17,
	kNuREle            = 18,
	kNuRMu             = 19,
	kNuRTau            = 20,
	kLQ                = 21,
	
	kSUSY              = 22,
	kLightMeson        = 23,
	kStrangeMeson      = 24,
	kCharmedMeson      = 25,
	kBottomMeson       = 26,
	kCCbarMeson        = 27,
	kJPsi              = 28,
	kBBbarMeson        = 29, 
	kLightBaryon       = 30,
	kStrangeBaryon     = 31,
	kCharmedBaryon     = 32, 
	kBottomBaryon      = 33,
	kPionDecay         = 34,
	kKaonDecay         = 35,
	kBremPhot          = 36,
	kPromptPhot        = 37,
	kUndrPhot          = 38,
	kISRPhot           = 39,
	kFSRPhot           = 40, 
	kNucReact          = 41,
	kPiZero            = 42,
	kDiBoson           = 43,
	kZorHeavyBoson     = 44,
	kQCD               = 45,
	kNumParticleOrigin = 46
    };
    
private:
    EParticleOrigin fParticleOrigin; // Particle origin
    
public:
    AtlMCTruthClassifier();
    AtlMCTruthClassifier(EParticleOrigin ParticleOrigin);
    virtual ~AtlMCTruthClassifier();
    virtual void Clear(Option_t *option = "");
    static const char* GetParticleOriginName(EParticleOrigin ParticleOrigin);
    static EParticleOrigin GetParticleOriginByName(const char *name);
    
    inline EParticleOrigin GetParticleOrigin() const { return fParticleOrigin; }
    inline const char* GetName() const {
	//
        // Get name of stored particle origin
	//
        return GetParticleOriginName(fParticleOrigin);
    }
    inline void Set(EParticleOrigin ParticleOrigin) {
	//
	// Set classifier
	//
	fParticleOrigin = ParticleOrigin;
    }

    inline Bool_t IsFromBottomMeson() const { return fParticleOrigin == kBottomMeson; }
    inline Bool_t IsFromWBoson()      const { return fParticleOrigin == kWBoson; }
    
    ClassDef(AtlMCTruthClassifier,1) // MC truth classifier class
};
#endif

