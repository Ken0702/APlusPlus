//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMCParticle
#define HEP_HepMCParticle
#ifndef HEP_Particle
#include <HepParticle.h>
#endif
#ifndef ROOT_TRef
#include <TRef.h>
#endif
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif
#ifndef HEP_HepMCVertex
#include <HepMCVertex.h>
#endif

class HepMCVertex;

class HepMCParticle : public HepParticle {

protected:
    TRefArray      *fMothers;     // List of mother particles
    TRefArray      *fDaughters;   // List of daughters (if any)
    TRef           *fStartVertex; // Production vertex
    TRef           *fDecayVertex; // Decay vertex (if any)
    Int_t           fMCStatus;    // MC Generator Status Code
                                  // Only the status 1 are used by simulation.
                                  // http://atlas-physics.phyast.pitt.edu/~atlas/documents/simulation_physics/HERWIG_status_codes.pdf
    Bool_t fIsGenerator;          // Returns true on particles that were part of the event that came from the event
                                  // generator. Partilces produced by Geant are returned false
    Bool_t fIsGenNonInteracting;  // Returns true on neutrinos and other non interacting particles such as those from supersymmetry.
                                  // It returns a complete list including any geant secondaries. Is has more partilces than results
                                  // from the combination of IsGenStable and a subseqent selection
                                  // on the IsGenNonInteracting PDG ID codes.
    Bool_t fIsGenStable;          // Returns true on all stable particles. In the case of events from GenzModule these  are all the
                                  // partilces that are passed as input to Geant as either non-interacting or as particles
                                  // that are subsequently interact and decay in Geant.
    Bool_t fIsGenSimulStable;     // Returns true on all stable particles during generation and simulation.
    Bool_t fIsGenInteracting;     // Is a subset of IsGenStable that excludes non interacting particles. It includes electrons, photons,
                                  // muons, and hadrons that are considered as imput to geant such as pions, kaons and KL.
    Bool_t fIsConversion;         // Returns true for the particles that convert into the e+e- pair.
    Bool_t fIsBremsstrahlung;     // Returns true for the input particles in the Bremsstrahlung process.
    
    
public:
    HepMCParticle();
    HepMCParticle(Int_t Id, Int_t PdgCode, Float_t Px, Float_t Py, Float_t Pz,
		  Float_t E, Int_t MCStatus,
		  Bool_t IsGenerator,
		  Bool_t IsGenNonInteracting,
		  Bool_t IsGenStable,
		  Bool_t IsGenSimulStable,
		  Bool_t IsGenInteracting,
		  Bool_t IsConversion,
		  Bool_t IsBremsstrahlung);
    virtual ~HepMCParticle();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    Bool_t IsGoodMother();
    Double_t GetBremsLoss();

    inline Bool_t IsGenerator() { return fIsGenerator; }
    inline Bool_t IsGenNonInteracting() { return fIsGenNonInteracting; }
    inline Bool_t IsGenStable() { return fIsGenStable; }
    inline Bool_t IsGenSimulStable() { return fIsGenSimulStable; }
    inline Bool_t IsGenInteracting() { return fIsGenInteracting; }
    inline Bool_t IsConversion() { return fIsConversion; }
    inline Bool_t IsBremsstrahlung() { return fIsBremsstrahlung; }
        
    inline Int_t GetMCStatus() const { return fMCStatus; }
    inline HepMCParticle* DaughterOf(Int_t i = 0) const {
	//
	// Return the mother particle with index i. Most
	// useful for normal particles that have just one
	// mother (default).
	//
	return GetMother(i); 
    }
    inline HepMCParticle* GetMother(Int_t i = 0) const {
	//
	// Return the mother particle with index i. Most
	// useful for normal particles that have just one
	// mother (default).
	//
	return (HepMCParticle*)fMothers->At(i);
    }
    inline HepMCVertex* GetStartVertex() const
            { return (HepMCVertex*)fStartVertex->GetObject(); }
    inline HepMCVertex* ProducedAt() const
	    { return GetStartVertex(); }
    inline HepMCVertex* GetDecayVertex() const
	    { return (HepMCVertex*)fDecayVertex->GetObject(); }
    inline HepMCVertex* DecayedAt() const
	    { return GetDecayVertex(); }
    inline TRefArray* GetDaughters() const { return fDaughters; }
    inline TRefArray* GetMothers() const { return fMothers; }
    inline void AddDaughter(HepMCParticle *Daughter)
	    { fDaughters->Add((TObject*)Daughter); }
    inline void AddMother(HepMCParticle *Mother)
	    { fMothers->Add((TObject*)Mother); }
    inline void SetStartVertex(HepMCVertex *vtx)
	    { *fStartVertex = (TObject*)vtx; }
    inline void SetDecayVertex(HepMCVertex *vtx)
	    { *fDecayVertex = (TObject*)vtx; }
    inline Int_t GetN_Daughters() const
            { return fDaughters->GetEntries(); }
    inline Int_t GetN_Mothers() const
            { return fMothers->GetEntries(); }

    ClassDef(HepMCParticle,6) // HEP MC particle
};
#endif
