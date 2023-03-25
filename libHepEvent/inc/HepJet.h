//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepJet
#define HEP_HepJet
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TLorentzVector
#include <TLorentzVector.h>
#endif

class HepParticle;

class HepJet : public TObject {
    
  protected:
    Int_t           fId;  // Id number (for convenience only)
    TLorentzVector  fP;   // Jet 4-momentum (GeV)
    
  private:
    Bool_t          fCompute;  //! Compute the transient variables at the next possibility
    Float_t         fEt;       //! Transverse energy (GeV)
    Float_t         fPt;       //! Transverse momentum (GeV)
    Float_t         fTheta;    //! Polar angle of the 3-momentum (rad)
    Float_t         fCosTheta; //! Cosine of the polar angle
    Float_t         fPhi;      //! Azimuth of the 3-momentum (rad)
    Float_t         fEta;      //! Pseudo-rapidity of the 3-momentum
 
  public:
    HepJet();
    HepJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz);
    virtual ~HepJet();
    virtual void Clear(Option_t *option = "");
    virtual Int_t Compare(const TObject *obj) const;
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    Float_t DeltaPhi(HepParticle *prt) const;
    Float_t DeltaPhi(HepJet *jet) const;
    Float_t DeltaEta(HepParticle *prt);
    Float_t DeltaEta(HepJet *jet);
    Float_t DeltaR(HepParticle *prt);
    Float_t DeltaR(HepJet *jet);
    Float_t DeltaPtFrac(HepParticle *prt);
    Float_t DeltaPtFrac(HepJet *jet);
    Float_t DeltaEtFrac(HepParticle *prt);
    Float_t DeltaEtFrac(HepJet *jet);
    Float_t InvDeltaPtFrac(HepParticle *prt);
    Double_t PtRel(HepParticle* prt);
    
    inline virtual Bool_t IsSortable() const { return kTRUE; }
    inline Int_t GetId() const { return fId; }
    inline void SetId(Int_t Id) { fId = Id; }
    inline const TLorentzVector& P() const {
	// Return momentum 4 vector (GeV)
	return fP;
    }
    inline TVector3 P3() const {
	// Return momentum 3 vector (GeV)
	return fP.Vect();
    }
    inline Float_t Et() {
	// Return transvere energy (GeV)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.Et()" instead.          !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fEt;
    }
    inline Float_t Pt() {
	// Return transvere momentum (GeV)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.Perp()" instead.        !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fPt;
    }
    inline Float_t Eta() {
	// Return pseudo-rapidity
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.Eta()" instead.         !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fEta;
    }
    inline Float_t Phi() {
	// Return azimuth (rad)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.Phi()" instead.         !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fPhi;
    }
    inline Float_t Theta() {
	// Return polar angle (rad)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.Theta()" instead.       !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fTheta;
    }
    inline Float_t CosTheta() {
	// Return cosine of the polar angle
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fCone4H1TowerJets.fP.CosTheta()" instead.    !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fCosTheta;
    }

    inline Float_t M() const {
	// Return invariant mass (GeV)
	return fP.M();
    }
    inline Float_t Px() const {
	// Return momentum x component
	return fP.Px();
    }
    inline Float_t Py() const {
	// Return momentum y component
	return fP.Py();
    }
    inline Float_t Pz() const {
	// Return momentum z component
	return fP.Pz();
    }
    inline Float_t E() const {
	// Return energy component
	return fP.E();
    }

    inline void SetPtEtaPhiE(Float_t Pt, Float_t Eta, Float_t Phi,
			     Float_t E) {
        fP.SetPtEtaPhiE(Pt, Eta, Phi, E);
	ComputeTransientVars();
    }
    void SetP(TLorentzVector const & p) {
        fP = p;
        ComputeTransientVars();
    }
    
  private:
    void ComputeTransientVars();
    
    ClassDef(HepJet,3) // Basic jet class
};
#endif

