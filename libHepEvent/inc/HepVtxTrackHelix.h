//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepVtxTrackHelix
#define HEP_HepVtxTrackHelix
#ifndef ROOT_HepTrackHelix
#include <HepTrackHelix.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif
#ifndef ROOT_TRef
#include <TRef.h>
#endif
#ifndef ROOT_TVector3
#include <TVector3.h>
#endif

class HepVtxTrackHelix : public HepTrackHelix {

protected:
    TVector3  fP_Vtx;        // Momentum at vertex
    Float_t   fQovP_Vtx;     // Charge over momentum magnitude at vertex (e/GeV)
                             // Note that here the full magnitude of
                             // the momentum is used - not just Pt
    Float_t   fD0_Vtx;       // Distance of closest approach (cm) w.r.t. the reference
                             // point in the xy plane at the vtx z position.
                             // This value is 0 for all tracks used in the vertex fit.
                             // It is non-zero for tracks not used for the vertex fit but
                             // which supposed to origin from this vertex.
                             // The sign of D_0 is computef by
                             //   sign(d) = sign((p_vec x Bz_vec) . (pca_vec-ref_vec)
                             // cf ATL-SOFT-PUB-2006-004
    Float_t   fZ0_Vtx;       // Z coordinate of closest approach (cm) w.r.t. the reference.
                             // Should always be 0
    Float_t   fPhi0_Vtx;     // Azimuth (rad) of the helix' tangent at the
			     // point of closest approach in the xy plane
                             // at the z position of this vertex
    Float_t   fTheta0_Vtx;   // Polar angle (rad) of the track at the point of closest
                             // approach in the xy plane at the z position of this vertex
    Float_t   fChi2_Vtx;     // Chi2 of the track after vertex refit
    Int_t     fNDoF_Vtx;     // No. of degrees of freedom of track after vertex refit
    Float_t  fCovMat_Vtx[15];// Covariance matrix, contains covariance-elements of Helix Parameters
                             // w.r.t associated vertex -> See HepTrackHelix    
    TRef     *fVertex;       // Link to vertex
    TRef     *fIsClonedFrom; // Sometimes a track points to multiple
                             // vertices. In this case the track is cloned and given new vtx fit
                             // paramaters. This link stores the pointer to the track from which the
                             // clone had being made of. In case the track is not a clone (most of the
                             // time) the link is the zero pointer
    
public:
    HepVtxTrackHelix();
    HepVtxTrackHelix(Int_t Id, Float_t Chi2, Int_t NDoF,
		     Float_t Xref, Float_t Yref, Float_t Zref,  Float_t Phi0,
		     Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta0, 
		     const Float_t CovMat[15] = 0);
    virtual ~HepVtxTrackHelix();
    
    virtual void Clear(Option_t *option = "");
    virtual Bool_t IsPositive() const;
    virtual Bool_t IsNegative() const;
    virtual Int_t Charge() const;

    virtual const TVector3& P() const;
    virtual Float_t Pt() const;
    virtual Float_t Pmag() const;
    virtual Float_t Pz() const;
    
    virtual Float_t QovP() const;
    virtual Float_t D0() const;
    virtual Float_t Z0() const;
    virtual Float_t Phi0() const;
    virtual Float_t Theta0() const;
    virtual const Float_t* CovMatrix() const;
    
    virtual Float_t Chi2() const;
    virtual Int_t   NDoF() const;
    inline virtual Float_t Chi2ovNDoF() {return Chi2()/NDoF();}
        
    inline const TVector3& GetRef_Vtx() const {return this->GetVertex()->GetPos(); }

    inline Float_t GetQovP_Vtx()      const { return fQovP_Vtx; }
    inline Float_t GetD0_Vtx()        const { return fD0_Vtx; }
    inline Float_t GetZ0_Vtx()        const { return fZ0_Vtx; }
    inline Float_t GetPhi0_Vtx()      const { return fPhi0_Vtx; }
    inline Float_t GetTheta0_Vtx()    const { return fTheta0_Vtx; }
    inline const Float_t* GetCovMatrix_Vtx() const { return fCovMat_Vtx; }
    inline Float_t GetChi2_Vtx()      const { return fChi2_Vtx; }
    inline Float_t GetNDoF_Vtx()      const { return fNDoF_Vtx; }

    inline HepVertex* GetVertex() const
	{ return (HepVertex*)fVertex->GetObject(); }
    inline HepVtxTrackHelix* IsClonedFrom() const
        { return (HepVtxTrackHelix*)fIsClonedFrom->GetObject(); }
    inline Bool_t IsInvalidVtxTrack()   const { return ProducedAt()->IsInvalid(); }
    Bool_t IsVtxTrack() const;
    Bool_t IsPrimaryVtxTrack()   const;
    Bool_t IsPileUpVtxTrack()    const; 
    Bool_t IsSecondaryVtxTrack() const;
    inline HepVertex* ProducedAt() const { return GetVertex(); }
    inline void SetVertex(HepVertex *vtx) { *fVertex = (TObject*)vtx; }
    inline void SetIsClonedFrom(HepVtxTrackHelix* trk) { *fIsClonedFrom = (TObject*)trk; }

    inline void SetCovMatrix_Vtx(Float_t* CovMat_Vtx) {
	for(Int_t i=0; i<15; i++) fCovMat_Vtx[i]= CovMat_Vtx[i];
    }
    inline void SetPVtx(Float_t Px_Vtx, Float_t Py_Vtx, Float_t Pz_Vtx) {
	// Set momentum 3-vector from vtx-fit (in global frame)
	// will be filled while running the a++converter !!!
	fP_Vtx.SetXYZ(Px_Vtx, Py_Vtx, Pz_Vtx); 
    }
    inline void SetPtEtaPhiVtx(Float_t Pt_Vtx, Float_t Eta_Vtx, Float_t Phi_Vtx) {
	// Set momentum 3-vector from vtx-fit (in global frame)
	// will be filled while running the a++converter !!!
	fP_Vtx.SetPtEtaPhi(Pt_Vtx, Eta_Vtx, Phi_Vtx); 
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
    
    void SetVtxParameters(Float_t Chi2_Vtx, Int_t NDoF_Vtx,
			  Float_t Phi0_Vtx, Float_t QovP_Vtx, Float_t D0_Vtx,
			  Float_t Z0_Vtx, Float_t Theta0_Vtx, Float_t CovMat_Vtx[15]);
    
  protected:
    virtual void PrintVtxType() const;
    virtual void PrintClone() const;
    
    ClassDef(HepVtxTrackHelix,9) // Track helix attached to a vertex
};
#endif
