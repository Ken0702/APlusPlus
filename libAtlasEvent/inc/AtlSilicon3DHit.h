//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlSilicon3DHit
#define ATLAS_AtlSilicon3DHit
#ifndef ROOT_HepSiliconDigit
#include "Hep3DHit.h"
#endif

class AtlSilicon3DHit : public Hep3DHit {
private:
    Int_t fEndCapOrBarrel; // Hit is in barrel or end cap
    Int_t fEtaModule;      // Eta module of the cluster
    Int_t fPhiModule;      // Phi module of the cluster
    Int_t fLayerDisk;      // Layer of the cluster 
public:
    AtlSilicon3DHit();
    AtlSilicon3DHit(Int_t Id, Float_t X, Float_t Y, Float_t Z);
    virtual ~AtlSilicon3DHit();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();

    inline Int_t EndCapOrBarrel() const { return fEndCapOrBarrel; }
    inline Int_t PhiModule() const { return fPhiModule; }
    inline Int_t EtaModule() const { return fEtaModule; }
    inline Int_t LayerDisk() const { return fLayerDisk; }
    inline void  SetEndCapOrBarrel(Int_t EndCapOrBarrel) { fEndCapOrBarrel = EndCapOrBarrel; }
    inline void  SetPhiModule(Int_t PhiModule) { fPhiModule = PhiModule; }
    inline void  SetEtaModule(Int_t EtaModule) { fEtaModule = EtaModule; }
    inline void  SetLayerDisk(Int_t LayerDisk) { fLayerDisk = LayerDisk; }

    ClassDef(AtlSilicon3DHit,1) // ATLAS silicon hit
};

#endif
