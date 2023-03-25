//  
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlD0DecayKPi
#define ATLAS_AtlD0DecayKPi
#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif


class AtlD0DecayKPi : public Hep2BodyDecay {
    
  private:

    
  public:
    AtlD0DecayKPi();
    AtlD0DecayKPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		      Float_t E, AtlIDTrack *Kaon, AtlIDTrack *Pion, 
		      HepVertex *Vtx, HepVertex *PrimaryVtx,
		      HepParticle ReFitDaughter1, HepParticle ReFitDaughter2);
    virtual ~AtlD0DecayKPi();
    virtual void Clear(Option_t *option = "");
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlIDTrack* GetKaon()       { return (AtlIDTrack*)GetDaughter1();             }
    inline AtlIDTrack* GetPion()       { return (AtlIDTrack*)GetDaughter2();             }
    inline HepVertex*  GetVtx()        { return (HepVertex* )Hep2BodyDecay::GetVtx();    }
    inline HepParticle GetReFitKaon()  { return GetReFitDaughter1();                     }
    inline HepParticle GetReFitPion()  { return GetReFitDaughter2();                     }
    
    
    ClassDef(AtlD0DecayKPi,1) // D0 -> Kaon Pi decay
};
#endif
