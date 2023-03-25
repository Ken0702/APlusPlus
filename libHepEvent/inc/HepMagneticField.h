//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMagneticField
#define HEP_HepMagneticField
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector3
#include <TVector3.h>
#endif

class HepMagneticField : public TObject {

private:
    Int_t    fType;    // Magnetic field type
    TVector3 fBField;  // Direction and strength of the magnetic field
		       // (in Tesla)
    
public:
    enum EType { kSolenoid, kToroid };
    HepMagneticField(Float_t Bx, Float_t By, Float_t Bz,
		     Int_t Type = kSolenoid);
    virtual ~HepMagneticField();
    virtual void Print(Option_t *option = "") const;

    inline Int_t GetType() const { return fType; }
    inline const TVector3& GetBField() const { return fBField; }
    inline Float_t GetBStrength() const { return fBField.Mag(); }
    
    ClassDef(HepMagneticField,1) // Magnetic field
};
#endif

