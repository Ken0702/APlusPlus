//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlPixelHit
#define ATLAS_AtlPixelHit
#ifndef ATLAS_AtlSilicon3DHit
#include "AtlSilicon3DHit.h"
#endif

class AtlPixelHit : public AtlSilicon3DHit {
private:
    Float_t fTotalCharge;  // Total collected charge
    Int_t   fToT;          // Time-over-Treshold
    Int_t   fPhiIndex;     // Cluster row
    Int_t   fEtaIndex;     // Cluster column
public:
    AtlPixelHit();
    AtlPixelHit(Int_t Id, Float_t R, Float_t Eta, Float_t Phi);
    virtual ~AtlPixelHit();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();

    inline Int_t   ToT() const { return fToT; }
    inline Float_t TotalCharge() const { return fTotalCharge; }   
    inline Int_t   PhiIndex() const { return fPhiIndex; }
    inline Int_t   EtaIndex() const { return fEtaIndex; }

    inline void SetTotalCharge(Float_t TotalCharge) { 
      fTotalCharge = TotalCharge; 
    } 

    inline void SetToT(Int_t ToT) { fToT = ToT; } 
    inline void SetPhiIndex(Int_t PhiIndex) { fPhiIndex = PhiIndex; } 
    inline void SetEtaIndex(Int_t EtaIndex) { fEtaIndex = EtaIndex; } 
     
    ClassDef(AtlPixelHit,1) // ATLAS Pixel hit
};

#endif
