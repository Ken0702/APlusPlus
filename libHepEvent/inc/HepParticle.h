//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepParticle
#define HEP_HepParticle
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TLorentzVector
#include <TLorentzVector.h>
#endif
#ifndef HEP_HepDatabasePDG
#include <HepDatabasePDG.h>
#endif
#ifndef ROOT_TMath
#include <TMath.h>
#endif
#ifndef ROOT_TMatrixD
#include <TMatrixD.h>
#endif

class HepJet;
class TSystem;

class HepParticle : public TObject {

  protected:
    Int_t                  fId;       // Id number (for convenience only)
    TLorentzVector         fP;        // 4-momentum (Px, Py, Pz, E) in GeV
    Int_t                  fPdgCode;  // PDG code
    static HepDatabasePDG *fDbasePDG; //! PDG database

  private:
    Bool_t                 fCompute;  //! Compute the transient variables
                                      //  at the next possibility
    Float_t                fPmag;     //! Magnitude of the 3-mom. vector (GeV)
    Float_t                fPt;       //! Transverse momentum (GeV)
    Float_t                fTheta;    //! Polar angle of the 3-momentum (rad)
    Float_t                fCosTheta; //! Cosine of the polar angle
    Float_t                fPhi;      //! Azimuth of the 3-momentum (rad)
    Float_t                fEta;      //! Pseudo-rapidity of the 3-momentum
    
  public:
    HepParticle();
    HepParticle(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
		Int_t PdgCode);
    virtual ~HepParticle();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    Double_t Mass(Option_t *option = "PDG") const;
    virtual Int_t Compare(const TObject *obj) const;
    const char* GetPdgName() const;
    Bool_t HasInvalidPdgCode() const;
    Float_t Charge() const;
    Float_t DeltaPhi(HepParticle *prt) const;
    Float_t DeltaPhi(HepJet *jet) const;
    Float_t DeltaEta(HepParticle *prt);
    Float_t DeltaEta(HepJet *jet);
    Float_t DeltaR(HepParticle *prt);
    Float_t DeltaR(HepJet *jet);
    Float_t Mperp2() const;
    Float_t DeltaPtFrac(HepParticle *prt);
    Float_t DeltaPtFrac(HepJet *jet);
    Float_t InvDeltaPtFrac(HepParticle *prt);
    virtual void GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const;
    
    inline Int_t GetId() const { return fId; }
    inline Int_t GetPdgCode() const { return fPdgCode; }
    inline Bool_t IsPositive() const
	    { return ( Charge() > 0. ) ? kTRUE : kFALSE; }
    inline Bool_t IsNegative() const
	    { return ( Charge() < 0. ) ? kTRUE : kFALSE; }
    inline Bool_t IsNeutral() const
	    { return ( Charge() == 0. ) ? kTRUE : kFALSE; }
    inline virtual Bool_t IsSortable() const { return kTRUE; }

    inline Bool_t IsChargedLepton() const {
	// Particle is electron, mu or tau
	return ( IsElectron() || IsMuon() || IsTau() ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsNeutrino() const {
	// Particle is neutrino or antineutrino
	return ( fPdgCode == 12 || fPdgCode == -12 || fPdgCode == 14 || fPdgCode == -14  || fPdgCode == 16 || fPdgCode == -16) ? kTRUE : kFALSE;
	// Particle is a neutrino
	return ( IsElectronNeutrino() || IsMuonNeutrino()
		 || IsTauNeutrino() ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsPhoton() const {
	// Particle is Gamma
	return ( fPdgCode == 22 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsElectron() const {
	// Particle is e+ or e-
	return ( fPdgCode == 11 || fPdgCode == -11 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsEMinus() const {
	// Particle is e-
	return ( fPdgCode ==  11 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsEPlus() const {
	// Particle is e+
	return ( fPdgCode == -11 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsElectronNeutrino() const {
	// Particle is electron neutrino
	return ( fPdgCode == 12 || fPdgCode == -12 ) ? kTRUE : kFALSE;
    }


    inline Bool_t IsMuon() const {
	// Particle is mu+ or mu-
	return ( fPdgCode ==  13 || fPdgCode == -13 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsMuPlus() const {
	// Particle is mu+-
	return ( fPdgCode == -13 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsMuMinus() const {
	// Particle is mu-
	return ( fPdgCode ==  13 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsMuonNeutrino() const {
	// Particle is muon neutrino
	return ( fPdgCode == 14 || fPdgCode == -14 ) ? kTRUE : kFALSE;
    }

    
    inline Bool_t IsTau() const {
      // Particle is tau+ or tau-
      return ( fPdgCode == 15 || fPdgCode == -15 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTauMinus() const {
      // Particle is tau-
      return ( fPdgCode ==  15 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTauPlus() const {
      // Particle is tau+
      return ( fPdgCode == - 15 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTauNeutrino() const {
      // Particle is tau neutrino
      return ( fPdgCode == 16 || fPdgCode == -16 ) ? kTRUE : kFALSE;
    }

    
    inline Bool_t IsPion() const {
	// Particle is Pi+ or Pi-
	return ( fPdgCode == 211 || fPdgCode == -211 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsPiMinus() const {
	// Particle is Pi-
	return ( fPdgCode ==  -211 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsPiPlus() const {
	// Particle is Pi+
	return ( fPdgCode == 211 ) ? kTRUE : kFALSE;
    }

    
    inline Bool_t IsKaon() const {
	// Particle is K+ or K-
	return ( fPdgCode == 321 || fPdgCode == -321 ) ? kTRUE : kFALSE;
    }

    inline Bool_t IsProton() const {
	// Particle is p or antiproton
	return ( fPdgCode == 2212 || fPdgCode == -2212 ) ? kTRUE : kFALSE;
    }
    
    inline Bool_t IsKMinus() const {
	// Particle is K-
	return ( fPdgCode ==  -321 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsKPlus() const {
	// Particle is Ki+
	return ( fPdgCode == 321 ) ? kTRUE : kFALSE;
    }

    inline Bool_t IsStringOrCluster() const {
	// Particle is cluster (91) or string (92) on hadronisation level
	return ( fPdgCode == 91 || fPdgCode == 92 ) ? kTRUE : kFALSE;
    }

    inline Bool_t IsCharmQuark() const {
	// Particle is c or c_bar
	return ( fPdgCode == 4 || fPdgCode == -4 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsCharm() const {
	// Particle is c
	return ( fPdgCode ==  4 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsCharmBar() const {
	// Particle is c_bar
	return ( fPdgCode == -4 ) ? kTRUE : kFALSE;
    }

    inline Bool_t IsBeautyQuark() const {
	// Particle is b or b_bar
	return ( fPdgCode == 5 || fPdgCode == -5 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsBeauty() const {
	// Particle is b
	return ( fPdgCode ==  5 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsBeautyBar() const {
	// Particle is b_bar
	return ( fPdgCode == -5 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsDown() const {
	// Particle is d
	return ( fPdgCode == 1 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsDownBar() const {
	// Particle is d_bar
	return ( fPdgCode == -1 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsDownQuark() const {
	// Particle is d or d_bar quark
	return IsDown() || IsDownBar();
    } 
    inline Bool_t IsUp() const {
	// Particle is u
	return ( fPdgCode == 2 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsUpBar() const {
	// Particle is u_bar
	return ( fPdgCode == -2 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsUpQuark() const {
	// Particle is u or u_bar quark
	return IsUp() || IsUpBar();
    } 
    inline Bool_t IsStrange() const {
	// Particle is s
	return ( fPdgCode == 3 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsStrangeBar() const {
	// Particle is s_bar
	return ( fPdgCode == -3 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsStrangeQuark() const {
	// Particle is s or s_bar quark
	return IsStrange() || IsStrangeBar();
    } 
    inline Bool_t IsTopQuark() const {
	// Particle is t or t_bar
	return ( fPdgCode == 6 || fPdgCode == -6 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTop() const {
	// Particle is t
	return ( fPdgCode ==  6 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTopBar() const {
	// Particle is t_bar
	return ( fPdgCode == -6 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsLightQuark() const {
	// Is any of u, u_bar, d, d_bar or s, s_bar
	return IsUpQuark() || IsDownQuark() || IsStrangeQuark();
    }
    inline Bool_t IsQuark() const {
	// Is any of u, u_bar, d, d_bar, s, s_bar, c, c_bar, b, b_bar, t, t_bar 
	return IsLightQuark() || IsCharmQuark() || IsBeautyQuark() || IsTopQuark();
    }
    inline Bool_t IsBeautyPrimeQuark() const {
	// Particle is b' or b'_bar
	return ( fPdgCode == 7 || fPdgCode == -7 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsBeautyPrime() const {
	// Particle is b'
	return ( fPdgCode ==  7 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsBeautyPrimeBar() const {
	// Particle is b'_bar
	return ( fPdgCode == -7 ) ? kTRUE : kFALSE;
    }


    inline Bool_t IsTopPrimeQuark() const {
	// Particle is t' or t'_bar
	return ( fPdgCode == 8 || fPdgCode == -8 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTopPrime() const {
	// Particle is t'
	return ( fPdgCode ==  8 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsTopPrimeBar() const {
	// Particle is t'_bar
	return ( fPdgCode == -8 ) ? kTRUE : kFALSE;
    }


    inline Bool_t IsWBoson() const {
	// Particle is W+ or W-
	return ( fPdgCode == 24 || fPdgCode == -24 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsWPlus() const {
	// Particle is W+
	return ( fPdgCode == 24 ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsWMinus() const {
	// Particle is W-
	return ( fPdgCode == -24 ) ? kTRUE : kFALSE;
    }

    inline Bool_t IsZ0Boson() const {
	// Particle is Z0
	return ( fPdgCode == 23 ) ? kTRUE : kFALSE;
    }
    
    inline Double_t M(Option_t *option = "PDG") const {
	// Alias for Mass()
	return Mass(option);
    }
    inline Float_t Mt2() const {
	//
	//     Mt^2 = E^2 - Pz^2 = M^2 + Pt^2    (GeV^2)
	//
	// !!! Warning: Do not use this function for   !!!
	// !!! the reconstruction of heavy particles   !!!
	// !!! like the W-boson with the Jacobian-peak !!!
	// !!! method. Use Mperp() instead             !!!
	//
	return fP.Mt2();
    }
    inline Float_t Mt() const {
	//
	//     Mt = Sqrt(E^2 - Pz^2 = M^2 + Pt^2)   (GeV)
	//
	// !!! Warning: Do not use this function for   !!!
	// !!! the reconstruction of heavy particles   !!!
	// !!! like the W-boson with the Jacobian-peak !!!
	// !!! method. Use Mperp() instead             !!!
	//
	return fP.Mt();
    }
    inline Float_t Mperp() const {
	//
	// Mt = Sqrt(Et^2 - Pt^2)     (GeV)
	// Use this function for the Jacobian-peak method
	//
	// Note that the mass is signed
	Float_t mperp2 = Mperp2();
	return ( mperp2 > 0 ) ? TMath::Sqrt(mperp2)
	: -TMath::Sqrt(-mperp2);
    }
    inline const TLorentzVector& P() const {
	// Return 4-momentum vector (GeV)
	return fP;
    }
    inline TVector3 P3() const {
	// Return 3-momentum vector (GeV)
	return fP.Vect();
    }
    inline Float_t Pmag() {
	// Return magnitude of 3-momentum (GeV)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.Mag()" instead.                !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fPmag;
    }
    inline Float_t Pt() {
	// Return transvere momentum (GeV)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.Perp()" instead.               !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fPt;
    }
    inline Float_t Px() const {
	// Return momentum x component (GeV)
	return fP.Px();
    }
    inline Float_t Py() const {
	// Return momentum y component (GeV)
	return fP.Py();
    }
    inline Float_t Pz() const {
	// Return momentum z component (GeV)
	return fP.Pz();
    }
    inline Float_t E() const {
	// Return energy (GeV)
	return fP.E();
    }
    inline Float_t Et() const {
	// Return transverse energy (GeV)
	return fP.Et();
    }
    inline Float_t Eta() {
	// Return pseudo-rapidity
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.Eta()" instead.                !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fEta;
    }
    inline Float_t Phi() {
	// Return azimuth (rad)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.Phi()" instead.                !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fPhi;
    }
    inline Float_t Theta() {
	// Return polar angle (rad)
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.Theta()" instead.              !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fTheta;
    }
    inline Float_t CosTheta() {
	// Return cosine of the polar angle
	//
	// !!! The return value is cached. Do not use this function !!!
	// !!! inside TTree::Draw() or TTree::Scan().               !!!
	// !!! Use eg "fElectrons.fP.CosTheta()" instead.           !!!
	//
	if ( fCompute ) ComputeTransientVars();
	return fCosTheta;
    }
    inline void SetId(Int_t Id) { fId = Id; }
    inline void SetPdgCode(Int_t pdg) { fPdgCode = pdg; }
    inline void SetPxPyPzE(Float_t Px, Float_t Py, Float_t Pz,
			     Float_t E) {
        fP.SetPxPyPzE(Px, Py, Pz, E);
	ComputeTransientVars();
    }
    inline void SetPtEtaPhiE(Float_t Pt, Float_t Eta, Float_t Phi,
			     Float_t E) {
        fP.SetPtEtaPhiE(Pt, Eta, Phi, E);
	ComputeTransientVars();
    }
    inline void SetPtEtaPhiM(Float_t Pt, Float_t Eta, Float_t Phi,
			     Float_t M) {
        fP.SetPtEtaPhiM(Pt, Eta, Phi, M);
	ComputeTransientVars();
    }
    void SetP(TLorentzVector const & p) {
        fP = p;
        ComputeTransientVars();
    }

    Bool_t operator==(HepParticle Partner);

    static void PrintLorentzVector(TLorentzVector vec); // Print Cartesian and (pt,eta,phi) coordinates of a TLorentzVector object

  private:
    void ComputeTransientVars();
    
    ClassDef(HepParticle,2) // Abstract HEP particle class
};
#endif

