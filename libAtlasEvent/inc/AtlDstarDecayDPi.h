//  
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlDstarDecayDPi
#define ATLAS_AtlDstarDecayDPi
#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef ATLAS_AtlD0DecayKPi
#include <AtlD0DecayKPi.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif


class AtlDstarDecayDPi : public Hep2BodyDecay {
    
  private:

    
  public:
    AtlDstarDecayDPi();
    AtlDstarDecayDPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		      Float_t E, AtlD0DecayKPi *D0, AtlIDTrack *Pion, 
		      HepVertex *Vtx, HepVertex *PrimaryVtx,
		      HepParticle ReFitDaughter1, HepParticle ReFitDaughter2);
    virtual ~AtlDstarDecayDPi();
    virtual void Clear(Option_t *option = "");
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlD0DecayKPi* GetD0()      { return (AtlD0DecayKPi*)GetDaughter1();          }
    inline AtlIDTrack* GetPion()       { return (AtlIDTrack*)GetDaughter2();             }
    inline HepVertex*  GetVtx()        { return (HepVertex* )Hep2BodyDecay::GetVtx();    }
// WARNING! Make sure, that the refitted d0 -> k pi tracks are saved somewhere!
//    inline HepParticle GetReFitD0()    { return GetReFitDaughter1();                     }
    inline HepParticle GetReFitPion()  { return GetReFitDaughter2();                     }
    
    
    ClassDef(AtlDstarDecayDPi,1) // D0 -> Kaon Pi decay
};
#endif
