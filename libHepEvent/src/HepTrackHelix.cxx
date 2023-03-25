//____________________________________________________________________
//
// A basic track helix class
//
// The track helices are parametrised w.r.t. the point of closest
// approach to a Reference Point within the global atlas reference
// frame. So, the distance d0 for instance is
// d0=sqrt((PCA_x - Ref_x)^2 + (PCA_y - Ref_y)^2).
// The Reference Point the Tracks are expressed at is stored
// in the datamember fRef (usually fRef=(0,0,0)). This Point
// is allways the Reference Point where the helix-parametrisation w.r.t
// the Point of Closest Approach is expressed at. Possible other
// parametrisation sets like {D0_Vtx, Z0_Vtx, ...} from VtxTrackHelix class
// can be expressed to another Reference Point. See therefor the
// HepVtxTrackHelix.cxx description.

// The momentum information is stored in addition to the
// track parameters since it might differ from the simple calculation
// using the curvature only and neglecting material effects. The
// momentum vector is pinned at the point of closest approach, its
// direction is that of the helix tangent.
//
// The magnetic field can either be solenoidal or toroidal. It is
// represented by the static data member of type HepMagneticField.
//
// It is intended that this rather abstract class is inherited to some
// real track classes (eg. inner detector tracks or muon segments)
// with realistic values for the magnetic field given there by the
// (abstract) static member function GetMagneticField().
//

//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepTrackHelix
#include <HepTrackHelix.h>
#endif
#include <cmath>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepTrackHelix);
#endif

//____________________________________________________________________

HepTrackHelix::HepTrackHelix() {
    //
    // Default constructor
    //
    Clear();
}

//____________________________________________________________________

HepTrackHelix::HepTrackHelix(Int_t Id, Float_t Chi2, Int_t NDoF,
			     Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
			     Float_t QovP, Float_t D0,
			     Float_t Z0, Float_t Theta0, const Float_t CovMat[15]) :
    fId(Id), fQovP(QovP), fD0(D0), fZ0(Z0), fPhi0(Phi0), fTheta0(Theta0),
    fChi2(Chi2), fNDoF(NDoF),
    fD0sig(NAN), fZ0sig(NAN) {
    //
    // Normal constructor
    //
    // Note that the track momentum has to be given in addition via
    // the member function SetP() after the ctor was called. This is
    // because C++ cannot call abstract functions from inside the
    // ctor, neither directly or indirectly.
    //
    fRef.SetXYZ(Xref, Yref, Zref);

    // Compute (a first estimate of the) momentum vector from given
    // track paramters
    fP.SetMagThetaPhi(TMath::Abs(1./QovP), Theta0, Phi0);
    
    // Set covariance matrix
    if ( CovMat != 0 )
	for ( Int_t i = 0; i < 15; i++ ) fCovMat[i] = CovMat[i];
}

//____________________________________________________________________

HepTrackHelix::~HepTrackHelix() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepTrackHelix::Clear(Option_t *option) {
    //
    // Clear this object
    //
    
    // Reset parameters
    fId   = 0;
    fChi2 = 0;
    fNDoF = 0;
    fQovP = 0.;
    fD0   = 0.;
    fZ0   = 0.;
    fD0sig = NAN;
    fZ0sig = NAN;
    
    // Reset vectors
    fP.SetXYZ(0., 0., 1.);
    fRef.SetXYZ(0., 0., 0.);
    
    // Reset matrix
    for ( Int_t i=0; i < 15; i++ ) fCovMat[i] = 0.;
}

//____________________________________________________________________

void HepTrackHelix::Print(Option_t *option) const {
    //
    // Print object content
    //
    // Options available:
    //   "covmat" - Print content of covariance matrix
    //   "nohead" - No header containing the variable names is
    //              displayed. Useful when printing a whole table
    //              for a list of particles
    //
    TString opt = option;
    opt.ToLower();
    
    // Header
    if ( !opt.Contains("nohead") ) PrintHeader();

    // Main
    PrintHelixParameters();
    PrintVtxType();
    PrintClone();
    cout << endl;
    
    // Covariance matrix
    if ( opt.Contains("covmat") ) PrintCovMatrix();
    
    // Footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void HepTrackHelix::PrintHeader() {
    //
    // Print information header
    //
    cout << "------------------------------------------------------------------------------------------------------------"
	 << endl
         << " Id   P (GeV)  Pt (GeV)  Pz (GeV)    Theta      Phi    Eta  Q D0 (cm) Z_0 (cm) chi^2/DoF     Vtx type  Clone "
	 << endl
	 << "------------------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void HepTrackHelix::PrintFooter() {
    //
    // Print footer
    //
    cout << "------------------------------------------------------------------------------------------------------------"
	 << endl;
}

//____________________________________________________________________

void HepTrackHelix::PrintHelixParameters() const {
    //
    // Print helix parameters
    //
    cout.setf(ios::showpoint | ios::fixed, ios::floatfield);
    cout.width(3); cout << fId;
    cout.precision(3);
    cout.width(10); cout << Pmag();
    cout.width(10); cout << Pt();
    cout.width(10); cout << Pz();
    cout.width(9);  cout << Theta()*180/TMath::Pi();
    cout.width(9);  cout << Phi()*180/TMath::Pi();
    cout.width(7);  cout << Eta();
    if ( QovP() > 0. ) {
	cout.width(3); cout << "  +";
    } else {
	cout.width(3); cout << " -";
    }
    cout.width(8);  cout << D0();
    cout.width(9);  cout << Z0();
    cout.width(9);  cout << Chi2() << "/";
    cout.width(2);  cout << NDoF();
}

//____________________________________________________________________

void HepTrackHelix::PrintCovMatrix() const {
    //
    // Print covariance matrix
    //
    Int_t add = 0;
//    const Int_t w = 5;
    
	cout << endl;
	for ( Int_t i = 0; i < 5; i++ ) {  //rows
	    if ( i == 2 ) {
		cout << "     Cov = ";
	    } else {
		cout << "           ";
	    }
	    // cout << "1 1 1 1 1" << endl;
	    for( Int_t k = 0; k < i; k++ )   cout << "           ";
	    for( Int_t j = 0; j < 5-i; j++ ){
		//cout.width(w);
		cout << scientific <<"  " << CovMatrix()[j+add];
	    }
	    add+=(5-i);
	    cout << endl;
			    
	    /*for ( Int_t j = 0; j < i; j++ ) {
		for ( Int_t k = 0; k < w; k++ ) cout << " ";
	    }
	    for ( Int_t l = 0; l < 5-i; l++ ) {
		cout.width(w); cout << CovMatrix()[(i*5)+l];
		}*/
	}
}

//____________________________________________________________________

void HepTrackHelix::PrintVtxType() const {
    //
    // Print vtx type (there is none)
    //
    cout << "          ---  ";
}

//____________________________________________________________________

void HepTrackHelix::PrintClone() const {
    //
    // Print Id of clone track (there is none)
    //
    cout << "  -- ";
}

//____________________________________________________________________

Int_t HepTrackHelix::Compare(const TObject *obj) const {
    //
    // Comparison with another track by means of momentum (used for
    // sorting)
    //
    if ( Pt() <  ((HepTrackHelix*)obj)->Pt() ) return -1;
    if ( Pt() == ((HepTrackHelix*)obj)->Pt() ) return  0;
    if ( Pt() >  ((HepTrackHelix*)obj)->Pt() ) return  1;
    return 0;
}

//____________________________________________________________________

void HepTrackHelix::SetP(Float_t P) {
    //
    // Set track momentum
    // The momentum vector stored is measured in the global
    // reference frame
    //
    const Int_t BFieldType = GetMagneticField()->GetType();
    switch ( BFieldType ) {
	case HepMagneticField::kSolenoid:
	    fP.SetMagThetaPhi(P, fTheta0, fPhi0);
	    break;
	case HepMagneticField::kToroid:
	    fP.SetMagThetaPhi(P, fPhi0, fTheta0);
	    break;
    }
}

//____________________________________________________________________

void HepTrackHelix::MomentumToTrkParameters(Float_t Px, Float_t Py, Float_t Pz,
					    Float_t Charge, Float_t PCAx, Float_t PCAy,
					    Float_t PCAz, Float_t Xref, Float_t Yref,Float_t Zref,
					    Float_t& Phi0, Float_t& QoverP,
					    Float_t& D0, Float_t& Z0, Float_t& Theta0) {
    //
    // Convert given momentum into helix parameters
    //
    // d_vec = pca_vec - ref_vec
    // sign(d) = sign((p_vec x Bz_vec) . d_vec)
    // sign(d) cf ATL-SOFT-PUB-2006-004
    //
    TVector3 p(Px, Py, Pz);
    TVector3 l(0., 0., 1.); // B-field direction
    TVector3 d(PCAx - Xref, PCAy - Yref, 0.);
    TVector3 a = p.Cross(l);
    Float_t s = -1.*TMath::Sign(1., a.Dot(d));
    
    Phi0   = TMath::ATan2(Py, Px);
    QoverP = Charge / p.Mag();
    D0     = s*d.Perp();
    Z0     = PCAz;
    Theta0 = p.Theta();
}

//____________________________________________________________________

void HepTrackHelix::GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const {
    //
    // Get the covariance matrix of this track in Pt, eta, phi
    // coordinates
    //
    // For the coordinate transformation of (Pt,eta,phi) = T*(QovP,Theta,phi) with the initial
    // covariance matrix C, the transformed cov. matrix will be computed by
    // Cov_new = T*C*T_transposed
    //
    Double_t QovP  = this->QovP();
    Double_t Theta = this->Theta();
    Double_t pinv  = TMath::Abs(QovP);
    const Float_t *CovMat_ini = this->CovMatrix();
    
    // Build the initial covariance matrix
    // The way used here is clumsy and should be changed !!!
    TMatrixD C(3,3);
    C.Zero();
    C[0][0] = CovMat_ini[14];    // ( pt  , pt  )
    C[0][1] = CovMat_ini[13];    // ( pt  , eta )
    C[0][2] = CovMat_ini[11];    // ( pt  , phi )
    C[1][0] = C[0][1];           // ( eta , pt  )
    C[1][1] = CovMat_ini[12];    // ( eta , eta )
    C[1][2] = CovMat_ini[10];    // ( eta , phi )
    C[2][0] = C[0][2];           // ( phi , pt  )
    C[2][1] = C[1][2];           // ( phi , eta )
    C[2][2] = CovMat_ini[9];     // ( phi , phi )
    
    TMatrixD T(3,3);
    T.Zero();
    T[0][0] = -TMath::Sin(Theta)/(pinv*QovP);
    T[0][1] =  TMath::Cos(Theta)/pinv;
    T[1][1] =  -1./TMath::Sin(Theta);
    T[2][2] = 1.;

    TMatrixD T_trans(TMatrixD::kTransposed, T);
    TMatrixD CT_trans(C, TMatrixD::kMult, T_trans );
    TMatrixD TCT_trans(T, TMatrixD::kMult, CT_trans);

    CovMatPtEtaPhi = TCT_trans;
}

//____________________________________________________________________

void HepTrackHelix::SetD0sig(Float_t d0sig) {
    fD0sig = d0sig;
}

//____________________________________________________________________

void HepTrackHelix::SetZ0sig(Float_t z0sig) {
    fZ0sig = z0sig;
}
