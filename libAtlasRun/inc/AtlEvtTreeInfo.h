//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtTreeInfo
#define ATLAS_AtlEvtTreeInfo
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class TSystem;
class TObjArray;
class TObjString;
class TString;

class AtlEvtTreeInfo : public TNamed {

private:
    Int_t   fAthenaVersion[5];          // ATHENA version used for the A++ conversion process
    TString fAthenaPhysicsAnalysisName; // Name of the ATHENA physics analysis (if any),
                                        // this is often referred to as the name of the patch
    Float_t fBeamEnergy;                // LHC beam energy (GeV)
    
public:
    AtlEvtTreeInfo();
    AtlEvtTreeInfo(const char* name, const char* title);
    virtual ~AtlEvtTreeInfo();
    void ReadAthenaVersion();

    inline Int_t AthenaVersionMajor() const {
	// Major ATHENA version number (1st token)
	return fAthenaVersion[0];
    }
    inline Int_t AthenaVersionMinor() const {
	// Minor ATHENA version number (2nd token)
	return fAthenaVersion[1];
    }
    inline Int_t AthenaVersionRevision() const {
	// ATHENA development revision number (3rd token)
	return fAthenaVersion[2];
    }
    inline Int_t AthenaVersionPatch() const {
	// ATHENA development revision number (4th token)
	return fAthenaVersion[3];
    }
    inline Int_t AthenaVersionPhysicsAnalysis() const {
	// ATHENA development revision number (5th token)
	return fAthenaVersion[4];
    }
    inline const char* AthenaPhysicsAnalysisName() const {
	// ATHENA physics analysis name
	return fAthenaPhysicsAnalysisName.Data();
    }
    inline Float_t GetBeamEnergy() const {
	// LHC beam energy (GeV)
	return fBeamEnergy;
    }
    inline void SetBeamEnergy(Float_t BeamEnergy) {
	// Set LHC beam energy
	fBeamEnergy = BeamEnergy;
    }
    
    ClassDef(AtlEvtTreeInfo,4) // A++ event tree info
};
#endif

