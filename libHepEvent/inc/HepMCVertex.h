//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMCVertex
#define HEP_HepMCVertex
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector3
#include <TVector3.h>
#endif
#ifndef ROOT_TRef
#include <TRef.h>
#endif
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif

class HepMCParticle;

class HepMCVertex : public TObject {

protected:
    Int_t      fId;        // Id number (for convenience only)
    TVector3   fPos;       // Vertex position (cm)
    TRef      *fMother;    // Incoming particle
    TRefArray *fDaughters; // Outgoing particles
    
public:
    HepMCVertex();
    HepMCVertex(Int_t Id, Float_t x, Float_t y, Float_t z);
    virtual ~HepMCVertex();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();
    void AddDaughter(HepMCParticle *Daughter);
    
    inline Float_t X() const { return fPos.X(); }
    inline Float_t Y() const { return fPos.Y(); }
    inline Float_t Z() const { return fPos.Z(); }
    inline Int_t GetId() { return fId; }
    inline void SetId(Int_t Id) { fId = Id; }
    inline HepMCParticle* GetMother() const
	{ return (HepMCParticle*)fMother->GetObject(); }
    inline TRefArray* GetDaughters() const { return fDaughters; }
    inline void SetMother(HepMCParticle *Mother)
	{ *fMother = (TObject*)Mother; }
    
    ClassDef(HepMCVertex,2) // HEP MC vertex class
};
#endif

