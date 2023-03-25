//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepEnergySum
#define HEP_HepEnergySum
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector2
#include <TVector2.h>
#endif
#ifndef ROOT_TMath
#include <TMath.h>
#endif

class HepEnergySum : public TObject {

protected:
    Float_t  fSumEt;     // Scalar sum of all transverse energy
                         // deposits in the event (GeV)
    TVector2 fMissingEt; // Negative vectorial sum of all transverse
			 // energy deposits in the event (missing Et)
                         // in GeV
    
public:
    HepEnergySum();
    virtual ~HepEnergySum();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    
    inline Float_t GetSumEt() const { return fSumEt; }
    inline const TVector2& GetMissingEt() const { return fMissingEt; }
    inline Float_t MissingEt_Phi() const { return TVector2::Phi_mpi_pi(fMissingEt.Phi()); }
    inline Float_t MissingEt_Mag() const { return fMissingEt.Mod(); }
    inline void SetSumEt(Float_t SumEt) { fSumEt = SumEt; }
    inline void SetMissingEt(Float_t Ex, Float_t Ey)
	{ fMissingEt.Set(Ex, Ey); }
    inline void SetMissingEtMagPhi(Float_t Mag, Float_t Phi)
        { fMissingEt.SetMagPhi(Mag, Phi); }
    
    ClassDef(HepEnergySum,1) // Energy sums (missing Et etc.)
};
#endif

