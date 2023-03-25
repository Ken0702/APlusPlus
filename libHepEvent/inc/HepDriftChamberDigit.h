//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepDriftChamberDigit
#define HEP_HepDriftChamberDigit
#ifndef HEP_HepDigit
#include <HepDigit.h>
#endif

class TString;

class HepDriftChamberDigit : public HepDigit {

protected:
    Float_t fDriftTime;    // Drift time (ns)
    Float_t fDriftRadius;  // Radius of drift circle (mum)
    Float_t fFittedRadius; // Radius determined by tracking (mum)
    Float_t fT0;           // Individual timing constant (ns)
    
public:
    HepDriftChamberDigit();
    HepDriftChamberDigit(Int_t Id, Float_t DriftTime,
			 Float_t DriftRadius);
    virtual ~HepDriftChamberDigit();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();

    inline void SetFittedRadius(Float_t FittedRadius) { fFittedRadius = FittedRadius; }
    inline void SetT0(Float_t T0) { fT0 = T0; }
    inline Float_t DriftTime() const { return fDriftTime; }
    inline Float_t DriftRadius() const { return fDriftRadius; }
    inline Float_t FittedRadius() const { return fFittedRadius; }
    inline Float_t T0() const { return fT0; }

    ClassDef(HepDriftChamberDigit,3) // Drift chamber digit
};
#endif

