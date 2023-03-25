//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepVertex
#define HEP_HepVertex
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector3
#include <TVector3.h>
#endif
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif

class HepVtxTrackHelix;
class TSystem;

class HepVertex : public TObject {

  public:
    enum EType { kInvalid      = BIT(0),
		 kPrimary      = BIT(1),
		 kPileUp       = BIT(2),
		 kSecondary    = BIT(3),
		 kTrkKink      = BIT(4),
		 kConversion   = BIT(5),
		 kV0Vtx        = BIT(6),
		 kV0K0s        = BIT(7),
		 kV0Lambda     = BIT(8),
		 kV0LambdaBar  = BIT(9),
		 kV0Rho        = BIT(10),
		 kDummy        = BIT(11),
		 kNotSpecified = BIT(12)
		 
    };

private:
    Int_t      fId;         // Id number (for convenience only)
    TVector3   fPos;        // Vertex position (cm)
    TRefArray *fDaughters;  // Outgoing tracks
    Float_t    fChi2;       // Chi2 of the vertex fit
    Int_t      fNDoF;       // Degrees of freedom
    EType      fType;       // Vertex type
    Int_t      fNDaughters; // No. of daughter tracks (used as replacement in case fDaughters could not be filled, for instance when reading from D3PDs
    Float_t fErrX; // errors of components
    Float_t fErrY;
    Float_t fErrZ;
    
public:
    HepVertex();
    HepVertex(Int_t Id, Float_t X, Float_t Y, Float_t Z,
	      Float_t Chi2, Int_t NDoF);
    virtual ~HepVertex();
    virtual void Clear(Option_t *option = "");
    virtual Int_t Compare(const TObject *obj) const;
    void AddDaughter(HepVtxTrackHelix *trk);
    TString* GetVtxTypeStr() const;
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();

    inline const TVector3& GetPos() const { return fPos; }
    inline TRefArray* GetDaughters() const { return fDaughters; }
    inline Int_t GetNDaughters() const {
	//
	// Return no. of daughter tracks
	// In case of a filled daughter array simply the no. of
	// entries is returned. In some case (e.g. when reading from DPDs) those
	// arrays are not filled. Here, the data member fNDaughters is used.
	//
	return ( fDaughters->GetEntries() != 0 )
	    ? fDaughters->GetEntries() : fNDaughters;
    }
    inline Float_t GetChi2() const { return fChi2; }
    inline Int_t GetNDoF() const { return fNDoF; }
    inline Float_t GetChi2overNDoF() const { return fChi2/fNDoF; }
    inline Int_t GetId() { return fId; }
    inline EType GetType() const { return fType; }
    
    inline Bool_t IsInvalid()      const { return fType & kInvalid; }
    inline Bool_t IsDummy()        const { return fType & kDummy; }
    inline Bool_t IsNotSpecified() const { return fType & kNotSpecified; }
    inline Bool_t IsPrimary()      const { return fType & kPrimary; }
    inline Bool_t IsPileUp()       const { return fType & kPileUp; }
    inline Bool_t IsSecondary()    const { return fType & kSecondary; }
    inline Bool_t IsTrkKink()      const { return fType & kTrkKink; }
    inline Bool_t IsConversion()   const { return fType & kConversion; }
    inline Bool_t IsV0Vtx()        const { return fType & kV0Vtx; }
    inline Bool_t IsV0K0s()        const { return fType & kV0K0s; }
    inline Bool_t IsV0Lambda()     const { return fType & kV0Lambda; }
    inline Bool_t IsV0LambdaBar()  const { return fType & kV0LambdaBar; }
    inline Bool_t IsV0Rho()        const { return fType & kV0Rho; }
    inline virtual Bool_t IsSortable() const { return kTRUE; }
    inline void SetDummy()        { fType = kDummy; }
    inline void SetNotSpecified() { fType = kNotSpecified; }
    inline void SetPrimary()      { fType = kPrimary; }
    inline void SetPileUp()       { fType = kPileUp; }
    inline void SetSecondary()    { fType = kSecondary; }
    inline void SetTrkKink()      { fType = EType(kTrkKink | kSecondary); }
    inline void SetConversion()   { fType = EType(kConversion | kSecondary); }
    inline void SetV0Vtx()        { fType = EType(kV0Vtx | kSecondary); }
    inline void SetV0K0s()        { fType = EType(kV0K0s | kV0Vtx | kSecondary); }
    inline void SetV0Lambda()     { fType = EType(kV0Lambda | kV0Vtx | kSecondary); }
    inline void SetV0LambdaBar()  { fType = EType(kV0LambdaBar | kV0Vtx | kSecondary); }
    inline void SetV0Rho()        { fType = EType(kV0Rho | kV0Vtx | kSecondary); }
    inline void SetNDaughters(Int_t Ntrk) { fNDaughters = Ntrk; }
    inline void SetErrors(Float_t ErrX, Float_t ErrY, Float_t ErrZ) { 
      fErrX = ErrX; 
      fErrY = ErrY; 
      fErrZ = ErrZ; 
    }
    inline Float_t X() const { return fPos.X(); }
    inline Float_t Y() const { return fPos.Y(); }
    inline Float_t Z() const { return fPos.Z(); }
    inline Float_t ErrX() const { return fErrX; }
    inline Float_t ErrY() const { return fErrY; }
    inline Float_t ErrZ() const { return fErrZ; }
    ClassDef(HepVertex,5) // Reconstructed vertex
};
#endif

