//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef HEP_Hep2BodyDecay
#define HEP_Hep2BodyDecay
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif

class HepVertex;

class Hep2BodyDecay : public HepDecayParticle {

protected:
    TRef    *fDaughter1;   // Daughter particle 1
    TRef    *fDaughter2;   // Daughter particle 2
    TRef    *fVtx;         // Associated Vertex
    Float_t  fDecayLength; // Distance from primary vtx (cm)
    Float_t  fAngleToPrim; // Angular divergence between lambda origin and primary vtx
    Float_t  fDecayAngle;  // Angle between outgoing tracks in lab frame
    Float_t  fOpeningAngle;      // Angle between outgoing tracks in rest frame
    Float_t  fTransvDecayLength; // Transverse decay length
    HepParticle fReFitDaughter1; // in case of using the kinematic fitter,
    HepParticle fReFitDaughter2; // store new momenta and PDGCode
    Bool_t   fHasSharedTracks;   // true, if one of the tracks also belongs to another decay
    
public:
    Hep2BodyDecay();
    Hep2BodyDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E, TObject *Daughter1, TObject *Daughter2,
		     HepVertex *DecayVtx, HepVertex* PrimaryVtx,
		     HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    virtual ~Hep2BodyDecay();
    virtual void Clear(Option_t *option);
    Float_t GetLifeTime();
    inline void SetSharedTracks(Bool_t sharedTracks) {
	fHasSharedTracks = sharedTracks;
    }
    
    inline Float_t GetOpeningAngle() const { return fOpeningAngle;     }
    inline Float_t GetDecayAngle() const   { return fDecayAngle;       } // Simple Angle, Not Cosine !!!
    inline Float_t GetDecayLength() const  { return fDecayLength;      }
    inline Float_t GetAngleToPrimary() const    { return fAngleToPrim;       }
    inline Float_t GetTransvDecayLength() const { return fTransvDecayLength; }
    inline TObject* GetDaughter1()   { return fDaughter1->GetObject(); }
    inline TObject* GetDaughter2()   { return fDaughter2->GetObject(); }
    virtual inline TObject* GetVtx() { return fVtx->GetObject();       }
    inline HepParticle GetReFitDaughter1() { return fReFitDaughter1;   }
    inline HepParticle GetReFitDaughter2() { return fReFitDaughter2;   }
    inline Bool_t   HasSharedTracks()      { return fHasSharedTracks;  }
    
    ClassDef(Hep2BodyDecay,4); // Base two body decay class
};
#endif

