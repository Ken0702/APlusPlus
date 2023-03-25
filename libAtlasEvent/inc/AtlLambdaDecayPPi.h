//  
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlLambdaDecayPPi
#define ATLAS_AtlLambdaDecayPPi
#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif


class AtlLambdaDecayPPi : public Hep2BodyDecay {
    
  private:

    
  public:
    AtlLambdaDecayPPi();
    AtlLambdaDecayPPi(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		      Float_t E, AtlIDTrack *Proton, AtlIDTrack *Pion, 
		      HepVertex *Vtx, HepVertex *PrimaryVtx,
		      HepParticle ReFitDaughter1, HepParticle ReFitDaughter2);
    virtual ~AtlLambdaDecayPPi();
    virtual void Clear(Option_t *option = "");
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlIDTrack* GetProton()     { return (AtlIDTrack*)GetDaughter1();             }
    inline AtlIDTrack* GetPion()       { return (AtlIDTrack*)GetDaughter2();             }
    inline HepVertex*  GetVtx()        { return (HepVertex* )Hep2BodyDecay::GetVtx(); }
    inline HepParticle GetReFitProton() { return GetReFitDaughter1();                      }
    inline HepParticle GetReFitPion()   { return GetReFitDaughter2();                      }
    
    
    ClassDef(AtlLambdaDecayPPi,3) // Lambda -> Proton Pi decay
};
#endif
