//  
// author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepTrackHelix
#define HEP_HepTrackHelix
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TVector3
#include <TVector3.h>
#endif
#ifndef ROOT_TMath
#include <TMath.h>
#endif
#ifndef HEP_MagneticField
#include <HepMagneticField.h>
#endif
#ifndef ROOT_TMath
#include "TMath.h"
#endif
#ifndef ROOT_TMatrixD
#include "TMatrixD.h"
#endif


class HepTrackHelix : public TObject {
protected:
    Int_t        fId;         // Id number (for convenience only)
    TVector3     fRef;        // Reference point (default is (0,0,0))
    TVector3     fP;          // Momentum at point of closest approach, pca (GeV)
    Float_t      fQovP;       // Q/P, charge over momentum (e/GeV)
                              // Note that here the full magnitude of
                              // the momentum is used - not just Pt
    Float_t      fD0;         // D_0, distance of closest approach perpendicular to
			      // B-field direction (cm) w.r.t. the reference point fRef
                              // The sign of D_0 is computef by
                              //   sign(d) = sign((p_vec x Bz_vec) . (pca_vec-ref_vec)
                              // cf ATL-SOFT-PUB-2006-004
    Float_t      fZ0;         // Z_0, distance of closest approach
			      // along B-field direction (cm)
    Float_t      fPhi0;       // Azimuth of the helix' tangent at the
			      // point of closest approach in a plane
			      // perpendicular to B-field direction (rad)
    Float_t      fTheta0;     // Polar angle of the track at the point of closest
                              // approach w.r.t. the B-field direction (rad)
    Float_t      fChi2;       // Chi2 of the track fit
    Int_t        fNDoF;       // No. of degrees of freedom of track fit
    Float_t      fCovMat[15]; // Covariance matrix, contains covariance-elements of
                              // [ D0-D0,D0-Z0, D0-Phi0,   D0-Theta0,     D0-QovP
                              //         Z0-Z0, Z0-Phi0,   Z0-Theta0,     Z0-QovP
                              //                Phi0-Phi0, Phi0-Theta0,   Phi0-QovP
                              //                           Theta0-Theta0, Theta0-QovP
                              //                                          QovP-QovP    ]
                              // The order is [D0-D0,D0-Z0,...,Z0-Z0,Z0-Phi0,...]
    Float_t      fD0sig;      // significance of D_0
    Float_t      fZ0sig;      // significance of Z_0
    
public:
    HepTrackHelix();
    HepTrackHelix(Int_t Id, Float_t Chi2, Int_t NDoF,
		  Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
		  Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta0, 
		  const Float_t CovMat[15] = 0);
    virtual ~HepTrackHelix();
   
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();
    virtual Int_t Compare(const TObject *obj) const;
    virtual const HepMagneticField* GetMagneticField() const { return 0; }
    void SetP(Float_t P);
    void GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const; 
    static void MomentumToTrkParameters(Float_t Px, Float_t Py, Float_t Pz,
					Float_t Charge, Float_t PCAx, Float_t PCAy,
					Float_t PCAz, Float_t Xref, Float_t Yref,Float_t Zref,
					Float_t& Phi0, Float_t& QoverP,
					Float_t& D0, Float_t& Z0, Float_t& Theta0);

    
    inline Float_t GetPhi0() const { return fPhi0; }
    inline Float_t GetQovP() const { return fQovP; }
    inline Float_t GetD0() const { return fD0; }
    inline Float_t GetTheta0() const { return fTheta0; }
    inline Float_t GetZ0() const { return fZ0; }

    inline Float_t GetPt() const { return fP.Perp(); }
    inline Float_t GetPmag() const { return fP.Mag(); }
    inline Float_t GetPz() const { return fP.Z(); }

    inline const TVector3& GetRef() const { return fRef; }
    inline const TVector3& GetP() const { return fP; }

    Float_t GetD0Sig() const { return fD0sig; }
    Float_t GetZ0Sig() const { return fZ0sig; }

    void SetD0sig(Float_t d0sig);
    void SetZ0sig(Float_t z0sig);

    inline virtual Float_t Phi0() const {
	return fPhi0;
    }
    inline virtual Float_t QovP() const {
	return fQovP;
    }
    inline virtual Float_t D0() const {
	return fD0;
    }
    inline virtual Float_t Z0() const {
	return fZ0;
    }
    inline virtual Float_t Pt() const {
	// Transverse momentum
	return GetPt();
    }
    inline virtual Float_t Pmag() const {
	// Momentum magnitude
	return GetPmag();
    }
    inline virtual Float_t Pz() const {
	// Momentum z component
	return GetPz();
    }
    inline virtual const TVector3& P() const {
	// Momentum 3-vector
	return GetP();
    }
    inline virtual Float_t Theta() const {
	// Polar angle of momentum vector
	return P().Theta();
    }
    inline virtual Float_t CosTheta() const {
	// Cosine of the momentum polar angle
	return P().CosTheta();
    }
    inline virtual Float_t Eta() const {
	// Pseudo-rapidity of the momentum vector
	Float_t cos_theta = CosTheta();
	if ( cos_theta*cos_theta < 1. ) {
	    return P().Eta();
	} else {
	    return ( Pz() > 0. ) ? 10e10 : -10e10;
	}
    }    
    inline virtual Float_t Phi() const {
	// Momentum vector azimuth
	return P().Phi();
    }

    inline Int_t GetId() const { return fId; }
    inline Int_t GetNDoF() const { return fNDoF; }
    inline Float_t GetChi2() const { return fChi2; }

    inline virtual Int_t NDoF() const { return fNDoF; }
    inline virtual Float_t Chi2() const { return fChi2; }

    inline virtual Float_t Chi2ovNDoF() {return Chi2()/NDoF();}
    
    inline virtual Bool_t IsPositive() const {
	// Track is positively charged ?
	return ( fQovP > 0. ) ? kTRUE : kFALSE;
    }
    inline virtual Bool_t IsNegative() const {
	// Track is negatively charged ?
	return !IsPositive();
    }
    inline virtual Bool_t IsSortable() const { return kTRUE; }
    inline void SetP(Float_t Px, Float_t Py, Float_t Pz) {
	// Set momentum 3-vector (in global frame)
	fP.SetXYZ(Px, Py, Pz);
    }
    inline void SetPtEtaPhi(Float_t Pt, Float_t Eta, Float_t Phi) {
	// Set momentum 3-vector (in global frame)
	fP.SetPtEtaPhi(Pt, Eta, Phi);
    }
    inline const Float_t* GetCovMatrix() const {
	return fCovMat;
    }
    inline virtual const Float_t* CovMatrix() const {
	// Covariance matrix in helix parameter expression
	return fCovMat;
    }
    inline void SetCovMatrix(Float_t* CovMat) {
	for(Int_t i=0; i<15; i++) fCovMat[i]= CovMat[i];    }
    inline void SetParameters(Int_t Id, Float_t Chi2, Int_t NDoF,
			      Float_t Xref, Float_t Yref, Float_t Zref,
			      Float_t Phi0, Float_t QovP, Float_t D0,
			      Float_t Z0, Float_t Theta0, Float_t CovMat[15]) {
        fId = Id; fChi2 = Chi2; fNDoF = NDoF; fRef.SetXYZ(Xref, Yref, Zref);
	fPhi0 = Phi0; fQovP = QovP; fD0 = D0; fZ0 = Z0; fTheta0 = Theta0;
	for(Int_t i=0; i<15; i++) fCovMat[i] = CovMat[i];
    }
    
  protected:
    void PrintCovMatrix() const;
    void PrintHelixParameters() const;
    virtual void PrintVtxType() const ;
    virtual void PrintClone() const;
    
    ClassDef(HepTrackHelix,6) // General track helix class
};
#endif

