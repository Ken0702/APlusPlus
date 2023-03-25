//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlK0sDecayPiPi
#define ATLAS_AtlK0sDecayPiPi
#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif

class AtlK0sDecayPiPi : public Hep2BodyDecay {
    
  private:
    
  public:
    AtlK0sDecayPiPi();
    AtlK0sDecayPiPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		    Float_t E, AtlIDTrack *PiPlus, AtlIDTrack *PiMinus,
		    HepVertex *DecayVtx, HepVertex *PrimaryVtx,
		    HepParticle ReFitPiPlus, HepParticle ReFitPiMinus);
    virtual ~AtlK0sDecayPiPi();
    virtual void Clear(Option_t *option = "");
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlIDTrack* GetPiPlus()     { return (AtlIDTrack*)GetDaughter1();             }
    inline AtlIDTrack* GetPiMinus()    { return (AtlIDTrack*)GetDaughter2();             }
    inline HepVertex*  GetVtx()        { return (HepVertex* )Hep2BodyDecay::GetVtx(); }
    inline HepParticle GetReFitPiPlus() { return GetReFitDaughter1();                    }
    inline HepParticle GetReFitPiMinus(){ return GetReFitDaughter2();                    }

    
    ClassDef(AtlK0sDecayPiPi,1) // K0s ->pi+pi- decay
};
#endif
