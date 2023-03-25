//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlPhotonConv
#define ATLAS_AtlPhotonConv
#ifndef HEP_Hep2BodyDecay
#include <Hep2BodyDecay.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif

class AtlPhotonConv : public Hep2BodyDecay {
    
  private:
    
  public:
    AtlPhotonConv();
    AtlPhotonConv(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		    Float_t E, AtlIDTrack *ePlus, AtlIDTrack *eMinus,
		    HepVertex *DecayVtx, HepVertex *PrimaryVtx,
		    HepParticle ReFitePlus, HepParticle ReFiteMinus);
    virtual ~AtlPhotonConv();
    virtual void Clear(Option_t *option = "");
    void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    
    inline AtlIDTrack* GetePlus()     { return (AtlIDTrack*)GetDaughter1();             }
    inline AtlIDTrack* GeteMinus()    { return (AtlIDTrack*)GetDaughter2();             }
    inline HepVertex*  GetVtx()        { return (HepVertex* )Hep2BodyDecay::GetVtx(); }
    inline HepParticle GetReFitePlus() { return GetReFitDaughter1();                    }
    inline HepParticle GetReFiteMinus(){ return GetReFitDaughter2();                    }

    
    ClassDef(AtlPhotonConv,1) // Gamma -> e+e- conversion
};
#endif
