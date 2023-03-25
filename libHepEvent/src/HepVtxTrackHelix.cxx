//____________________________________________________________________
//
// Vertex fitted particle track
//
// A HepVtxTrackHelix-object contains two sets of helix-parametrisation.
// The inherited one {D0, Z0, ...} and one w.r.t the vertex the Track is
// linked to {D0_Vtx, Z0_Vtx, ...}, whereby the coordinates of the Reference
// Point are given by the coordinates of the Vertex.
//
// !!! Note: Although a track can point to multiple vertices, here   !!! 
// !!! only the relation and track parameters of the vertex with the !!!
// !!! lowest chi2/ndof are stored.                                  !!!
//
// (The lost connection to the vertices might be restored however by
// walking the relationships from the vertices in direction of the
// tracks.)
//
//  Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//  Update: $Id$
//  Copyright: 2008 (C) Oliver Maria Kind
//

#ifndef HEP_HepVtxTrackHelix
#include <HepVtxTrackHelix.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepVtxTrackHelix);
#endif

//____________________________________________________________________

HepVtxTrackHelix::HepVtxTrackHelix() {
    //
    // Default constructor
    //
    fVertex = new TRef;
    fIsClonedFrom = new TRef;
}

//____________________________________________________________________

HepVtxTrackHelix::HepVtxTrackHelix(Int_t Id, Float_t Chi2, Int_t NDoF,
				   Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
				   Float_t QovP, Float_t D0,
				   Float_t Z0, Float_t Theta0,
				   const Float_t CovMat[15]) :
    HepTrackHelix(Id, Chi2, NDoF, Xref, Yref, Zref, Phi0, QovP, D0, Z0,
		  Theta0, CovMat),
    
    fNDoF_Vtx(0) {
  
  //
  // Normal constructor
  //
  fVertex = new TRef;
  fIsClonedFrom = new TRef;
  fP_Vtx.SetXYZ(0., 0., 1.);
}

//____________________________________________________________________

HepVtxTrackHelix::~HepVtxTrackHelix() {
    //
    // Default destructor
    //
    delete fVertex; fVertex = 0;
    delete fIsClonedFrom; fIsClonedFrom = 0;
}

//____________________________________________________________________

void HepVtxTrackHelix::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepTrackHelix::Clear(option);

    fChi2_Vtx   = 0.;
    fNDoF_Vtx   = 0;
    fQovP_Vtx   = 0.;
    fQovP_Vtx   = 0.;
    fD0_Vtx     = 0.;
    fZ0_Vtx     = 0.;
    fPhi0_Vtx   = 0.;
    fTheta0_Vtx = 0.;
    delete fVertex; fVertex = 0;
    delete fIsClonedFrom; fIsClonedFrom = 0;

    //Reset Vectors
    fP_Vtx.SetXYZ(0., 0., 1.);
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsPositive() const { 
    //
    // Positively charged ?
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //

    if ( IsVtxTrack() ) {
	return ( fQovP_Vtx > 0. ) ? kTRUE : kFALSE;
    } else {
	return ( fQovP > 0. ) ? kTRUE : kFALSE;
    }
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsNegative() const {
    //
    // Negatively charged ?
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return !IsPositive();
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsVtxTrack() const {

    if (ProducedAt() == 0) return kFALSE;
    else                   return kTRUE;
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsPrimaryVtxTrack() const {

    if (ProducedAt() == 0) return kFALSE;
    else                   return ProducedAt()->IsPrimary(); 
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsSecondaryVtxTrack() const {

    if (ProducedAt() == 0) return kFALSE;
    else                   return ProducedAt()->IsSecondary();  
}

//____________________________________________________________________

Bool_t HepVtxTrackHelix::IsPileUpVtxTrack() const {

    if (ProducedAt() == 0) return kFALSE;
    else                   return ProducedAt()->IsPileUp();
}

//____________________________________________________________________


Int_t HepVtxTrackHelix::Charge() const {
    //
    // Get particle's charge (in units of e)
    //
    Float_t QovP = 0.;
    Int_t charge = 0;
    if ( IsVtxTrack() ) {
	QovP = fQovP_Vtx;
    } else {
	QovP = fQovP;
    }
    if ( QovP > 0. ) {
	charge = 1;
    } else if ( QovP < 0. ) {
	charge = -1;
    }
    return charge;
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::Pt() const {
    //
    // Transverse momentum
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //

    return P().Perp();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::Pmag() const {
    //
    // Momentum magnitude
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return P().Mag();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::Pz() const {
    //
    // Momentum Z component
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return P().Z();
}

//____________________________________________________________________

const TVector3& HepVtxTrackHelix::P() const {

    // Momentum 3-vector
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fP_Vtx : GetP();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::QovP() const {
    //
    // Q/P
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fQovP_Vtx : GetQovP();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::D0() const {
    //
    // D0
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fD0_Vtx : GetD0();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::Z0() const {
    //
    // Z0
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fZ0_Vtx : GetZ0();
}

//____________________________________________________________________

Float_t HepVtxTrackHelix::Phi0() const {
    //
    // Phi0
    // Either for the vertex fitted helix parameters (default)
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fPhi0_Vtx : GetPhi0();
}

//____________________________________________________________________


Float_t HepVtxTrackHelix::Theta0() const {
    //
    // Theta0
    // Either for the vertex fitted helix parameters 
    // or the non-vertex fitted ones (if exist).
    //
    return ( IsVtxTrack() ) ? fTheta0_Vtx : GetTheta0();
}

//____________________________________________________________________


const Float_t* HepVtxTrackHelix::CovMatrix() const {
    // Covariance Matrix in helix parameter expression
    // either for the vertex fitted or the
    // non vertex fitted ones.

    return ( IsVtxTrack() ) ? fCovMat_Vtx : GetCovMatrix();
}


//____________________________________________________________________


Float_t HepVtxTrackHelix::Chi2() const {
    //
    // Chi2
    // Either for the vertex fitted tracks
    // or the non-vertex fitted ones.
    //
    return ( IsVtxTrack() ) ? fChi2_Vtx : GetChi2();
}

//____________________________________________________________________


Int_t HepVtxTrackHelix::NDoF() const {
    //
    // NDoF
    // Either for the vertex fitted helix parameters
    // or the non-vertex fitted ones.
    //
    return ( IsVtxTrack() ) ? fNDoF_Vtx : GetNDoF();
}

//____________________________________________________________________


void HepVtxTrackHelix::PrintVtxType() const {
    //
    // Print type of assigned vertex
    //
    if ( GetVertex() != 0 ) {
	cout.width(17); cout << GetVertex()->GetVtxTypeStr();
    } else {
	cout << "          ---    ";
    }
}

//____________________________________________________________________

void HepVtxTrackHelix::PrintClone() const {
    //
    // Print Id of clone track (if any)
    //
    if ( IsClonedFrom() != 0 ) {
	cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
	cout.width(5); cout << IsClonedFrom()->GetId();
    } else {
	cout << "  -- ";
    }
}

//____________________________________________________________________

void HepVtxTrackHelix::SetVtxParameters(Float_t Chi2_Vtx, Int_t NDoF_Vtx,
					Float_t Phi0_Vtx, Float_t QovP_Vtx, Float_t D0_Vtx,
					Float_t Z0_Vtx, Float_t Theta0_Vtx,
					Float_t CovMat_Vtx[15]) {
    //
    // Set helix parameters w.r.t. the vertex as reference point
    //
    fChi2_Vtx   = Chi2_Vtx;
    fNDoF_Vtx   = NDoF_Vtx;
    fPhi0_Vtx   = Phi0_Vtx;
    fQovP_Vtx   = QovP_Vtx;
    fD0_Vtx     = D0_Vtx;
    fZ0_Vtx     = Z0_Vtx;
    fTheta0_Vtx = Theta0_Vtx;
    for(Int_t i=0; i<15; i++) fCovMat_Vtx[i] = CovMat_Vtx[i];

}
