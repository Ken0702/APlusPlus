//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
//
#ifndef HEP_Hep3DHit
#define HEP_Hep3DHit
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector3
#include "TVector3.h"
#endif

class Hep3DHit : public TObject {

protected:
    Int_t    fId;   // Id number (for convenience only)
    TVector3 fPos;  // Position of the hit
public:
    Hep3DHit();
    Hep3DHit(Int_t Id, Float_t X, Float_t Y, Float_t Z);
    virtual ~Hep3DHit();
    
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();
    
    inline TVector3& Pos() { return fPos; }
    inline Float_t X() const { return fPos.X(); }
    inline Float_t Y() const { return fPos.Y(); }
    inline Float_t Z() const { return fPos.Z(); }
    inline Float_t Eta() const { if ( fPos.Pt() != 0. ) {
	return fPos.Eta();
    } else {
        return ( fPos.Pz() > 0. ) ? 10e10 : -10e10;}}
    inline Float_t Phi() const { return fPos.Phi(); }
    inline Float_t R() const { return fPos.Mag(); }
    inline Float_t R2() const { return fPos.Mag2(); }   // squared radius (spherical coordinates)
    inline Float_t Rt() const { return fPos.Perp(); }   // transverse distance to z-Axis
    inline Float_t Rt2() const { return fPos.Perp2(); } 

    inline Int_t GetId() const { return fId; }
    inline void SetId(Int_t Id) { fId = Id; }
    
    ClassDef(Hep3DHit,1) // Abstract 3D hit class
};
#endif

