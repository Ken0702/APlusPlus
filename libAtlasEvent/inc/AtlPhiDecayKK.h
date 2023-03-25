//  
// Author: Sophia Schade <mailto: schade@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sophia Schade
//
#ifndef ATLAS_AtlPhiDecayKK
#define ATLAS_AtlPhiDecayKK

#include <AtlIDTrack.h>     // for AtlIDTrack
#include <Hep2BodyDecay.h>  // for Hep2BodyDecay
#include <Rtypes.h>         // for ClassDef
#include <RtypesCore.h>     // for Float_t, Option_t, Int_t
#include "HepParticle.h"    // for HepParticle
#include "HepVertex.h"      // for HepVertex, HEP_HepVertex
class TBuffer;
class TClass;
class TMemberInspector;

class AtlPhiDecayKK : public Hep2BodyDecay {
    
  private:
    
    Float_t fAngle;        // Angle between KPlus and KMinus

  public:
    AtlPhiDecayKK();
    AtlPhiDecayKK(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		  Float_t E, AtlIDTrack *KPlus, AtlIDTrack *KMinus,
		  HepVertex *DecayVtx,HepVertex *PrimaryVtx,
		  HepParticle ReFitKPlus, HepParticle ReFitKMinus);
    virtual ~AtlPhiDecayKK();
    virtual void Clear(Option_t *option = "") override;
    void Print(Option_t *option = "") override;
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlIDTrack* GetKPlus()         { return (AtlIDTrack*)GetDaughter1();           }
    inline AtlIDTrack* GetKMinus()        { return (AtlIDTrack*)GetDaughter2();           }
    inline Float_t     GetAngleKK() const { return  fAngle;                               }
    inline HepVertex*  GetVtx() override {
        return static_cast<HepVertex*>(Hep2BodyDecay::GetVtx());
    }
    inline HepParticle GetReFitKPlus()    { return GetReFitDaughter1();                   }
    inline HepParticle GetReFitKMinus()   { return GetReFitDaughter2();                   }
    
    ClassDefOverride(AtlPhiDecayKK,1) // Phi ->K-Plus + K-Minus  - decay
};
#endif
