//  
// author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlMuon
#define ATLAS_AtlMuon
#ifndef HEP_HepMuon
#include <HepMuon.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#ifndef ATLAS_AtlMETWeights
#include <AtlMETWeights.h>
#endif
#ifndef ATLAS_AtlMCTruthClassifier
#include <AtlMCTruthClassifier.h>
#endif


class TSystem;

class AtlMuon : public HepMuon, public AtlTriggerMatch, public AtlMETWeights {

  public:
    enum EAuthor {
	kInvalidAuthor       = BIT(0),
	kHighPt              = BIT(1),
	kLowPt               = BIT(2),
	kMediumPt            = BIT(3),
	kMuonBoySP           = BIT(4),
	kMuonBoy             = BIT(5),
	kSTACO               = BIT(6),
	kMuTag               = BIT(7), // CAUTION! 8 and 9 do not exist!!!
	kMOORE               = BIT(10),
	kMuidSA              = BIT(11),
	kMuidCo              = BIT(12),
	kMuGirl              = BIT(13),
	kCaloMuonId          = BIT(14),
	kMuGirlLowBeta       = BIT(15),
	kCaloTag             = BIT(16),
	kCaloLikelihood      = BIT(17),
	kMuTagIMO            = BIT(18),
	kMuonCombinedRefit   = BIT(19),
	kExtrapolateMuonToIP = BIT(20)
    };
    static constexpr Int_t fgNAuthors = 21; // NO! There are actually only 19 now!
    static const char* fgAuthorNames[];
    enum EQuality {
	kInvalid = BIT(0),
	kLoose   = BIT(1),
	kMedium  = BIT(2),
	kTight   = BIT(3)
    };
    static constexpr Int_t fgNQuality = 4;
    static const char* fgQualityNames[];
    
  private:
    EAuthor  fAuthor;         // Reconstruction algorithm(s) used
    EQuality fQuality;        // Loose, medium, tight quality flags
    Float_t  fEtCone10;       // Transverse energy inside a cone dR=0.1
                              // around the muon direction (GeV).
                              // Looks like this one ist not filled in the SingleTop D3PDs
    Float_t  fEtCone20;       // Transverse energy inside a cone dR=0.2
                              // around the muon direction (GeV)
    Float_t  fEtCone30;       // Transverse energy inside a cone dR=0.3
                              // around the muon direction (GeV)
    Float_t  fEtCone40;       // Transverse energy inside a cone dR=0.4
                              // around the muon direction (GeV)
    Int_t    fNtrkCone10;     // No. of charged tracks inside a cone dR=0.1
    Int_t    fNtrkCone20;     // No. of charged tracks inside a cone dR=0.2
    Int_t    fNtrkCone30;     // No. of charged tracks inside a cone dR=0.3
    Int_t    fNtrkCone40;     // No. of charged tracks inside a cone dR=0.4
    Float_t  fPtCone10;       // Summed Pt of charged tracks inside a cone dR=0.1 (GeV)
    Float_t  fPtCone20;       // Summed Pt of charged tracks inside a cone dR=0.2 (GeV)
    Float_t  fPtCone30;       // Summed Pt of charged tracks inside a cone dR=0.3 (GeV)
    Float_t  fPtCone40;       // Summed Pt of charged tracks inside a cone dR=0.4 (GeV)
    Float_t  fMatchingChi2;   // Chi2 for combined Muons
    Int_t    fMatchingNDoF;   // No. of degrees of freedom for combined Muons
                              // Attention: Dafault Value is chi2=1000, NDoF=1;
                              // Value will be set for combinedMuons during the conversion
                              // for not combined muons, please look for the chi2 and NDoF values
                              // from original IDTrack if it exists
    Bool_t   fIsCombinedMuon; // Bool for good combined Muon
    HepTrackHelix *fTrackRefit;        // Track parameters (global track) after refit with muon hypothesis
    TRef    *fIDTrack;                 // Link to original inner detector Track
    TRef    *fMETrack;                 // Link to original MS extrapolated Track
    TLorentzVector fPMuonSpecExtrapol; // 4-momentum (Px, Py, Pz, E) in GeV
    Int_t fMuonSpecExtrapolCharge;     // Charge of te Track in MuonSpectrometer
    UInt_t         fMuon_inTrigger;  // Muon_inTrigger SgTop D3PD variable
    Bool_t         fIsTriggerMatched;  //WARNING: Only filled in D3PDTopSlim mode
    Double_t       fMu_SF;            // muon scale factors with syst. variations (SgTopD3PDs)
    Double_t       fMu_SF_reco_up;
    Double_t       fMu_SF_reco_down;
    Double_t       fMu_SF_Id_up;
    Double_t       fMu_SF_Id_down;
    Double_t       fMu_trigger_SF;
    Double_t       fMu_trigger_SF_up;
    Double_t       fMu_trigger_SF_down;

    AtlMCTruthClassifier fMCTruthClassifier; // MC truth classifier
 
public:
    AtlMuon();
    AtlMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	    Bool_t IsMuPlus, Float_t EtCone10, Float_t EtCone20,
	    Float_t EtCone30, Float_t EtCone40, Int_t NtrkCone10,
	    Int_t NtrkCone20, Int_t NtrkCone30, Int_t NtrkCone40,
	    Float_t PtCone10, Float_t PtCone20, Float_t PtCone30,
	    Float_t PtCone40, EAuthor Author, EQuality Quality,
	    Float_t MatchingChi2, Int_t MatchingNDoF,
	    Bool_t IsCombinedMuon,
	    TLorentzVector PMuonSpecExtrapol, Int_t MuonSpecExtrapolCharge);
    virtual ~AtlMuon();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    void PrintAuthor() const;
    TString* NameOfAuthor();
    TString* NameOfQuality();
    static TString* NameOfAuthor(AtlMuon::EAuthor author);
    static TString* NameOfQuality(AtlMuon::EQuality quality);
    
    inline EAuthor GetAuthor() const { return fAuthor; }
    inline EQuality GetQuality() const { return fQuality; }
    inline Float_t GetEtCone10() const { return fEtCone10; }
    inline Float_t GetEtCone20() const { return fEtCone20; }
    inline Float_t GetEtCone30() const { return fEtCone30; }
    inline Float_t GetEtCone40() const { return fEtCone40; }
    inline Int_t   GetNtrkCone10() const { return fNtrkCone10; }
    inline Int_t   GetNtrkCone20() const { return fNtrkCone20; }
    inline Int_t   GetNtrkCone30() const { return fNtrkCone30; }
    inline Int_t   GetNtrkCone40() const { return fNtrkCone40; }
    inline Float_t GetPtCone10() const { return fPtCone10; }
    inline Float_t GetPtCone20() const { return fPtCone20; }
    inline Float_t GetPtCone30() const { return fPtCone30; }
    inline Float_t GetPtCone40() const { return fPtCone40; }
    inline Float_t GetMatchingChi2() const { return fMatchingChi2; }
    inline Int_t   GetMatchingNDoF() const { return fMatchingNDoF; }
    inline Float_t GetMatchingChi2overNDoF() const { 
      if ( fMatchingNDoF == 0 )
	  // Beware of muons w/o inner track
	  return -1;
      else 
	return fMatchingChi2/fMatchingNDoF; 
    }

    inline Bool_t IsCombinedMuon() { return fIsCombinedMuon; }
    
    inline Bool_t HasInvalidAuthor() const { return fAuthor & kInvalidAuthor; }
    inline Bool_t IsHighPt()              const { return fAuthor & kHighPt; }
    inline Bool_t IsLowPt()               const { return fAuthor & kLowPt; }
    inline Bool_t IsMediumPt()            const { return fAuthor & kMediumPt; }
    inline Bool_t IsMuonBoySP()           const { return fAuthor & kMuonBoySP; }
    inline Bool_t IsMuonBoy()             const { return fAuthor & kMuonBoy; }
    inline Bool_t IsSTACO()               const { return fAuthor & kSTACO; }
    inline Bool_t IsMuTag()               const { return fAuthor & kMuTag; }
    inline Bool_t IsMOORE()               const { return fAuthor & kMOORE; }
    inline Bool_t IsMuidSA()              const { return fAuthor & kMuidSA; }
    inline Bool_t IsMuidCo()              const { return fAuthor & kMuidCo; }
    inline Bool_t IsMuGirl()              const { return fAuthor & kMuGirl; }
    inline Bool_t IsCaloMuonId()          const { return fAuthor & kCaloMuonId; }
    inline Bool_t IsMuGirlLowBeta()       const { return fAuthor & kMuGirlLowBeta; }
    inline Bool_t IsCaloTag()             const { return fAuthor & kCaloTag; }
    inline Bool_t IsCaloLikelihood()      const { return fAuthor & kCaloLikelihood; }
    inline Bool_t IsMuTagIMO()            const { return fAuthor & kMuTagIMO; }
    inline Bool_t IsMuonCombinedRefit()   const { return fAuthor & kMuonCombinedRefit; }
    inline Bool_t IsExtrapolateMuonToIP() const { return fAuthor & kExtrapolateMuonToIP; }

    inline Bool_t HasInvalidQuality() const { return fQuality & kInvalid; }
    inline Bool_t IsLoose()  const { return fQuality & kLoose; }
    inline Bool_t IsMedium() const { return fQuality & kMedium; }
    inline Bool_t IsTight()  const { return fQuality & kTight; }

    // Getters and Setter for the original IDTrack
    inline void SetIDTrack(AtlIDTrack *trk) { *fIDTrack = (TObject*)trk; }
    inline void SetMETrack(AtlIDTrack *trk) { *fMETrack = (TObject*)trk; }

    inline AtlIDTrack* GetIDTrack() const{
	// Returns original associated IDTrack
	return (AtlIDTrack*)fIDTrack->GetObject(); 
    }
    inline AtlIDTrack* GetMETrack() const{
	// Returns original associated IDTrack
	return (AtlIDTrack*)fMETrack->GetObject(); 
    }
    
    inline void SetTrackRefit(Float_t Chi2, Int_t NDoF,
			      Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
			      Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta0, 
			      Float_t CovMat[15]) {
	// Set track refit parameters, Id always 1
	fTrackRefit->SetParameters(1, Chi2, NDoF, Xref, Yref, Zref, Phi0,
				   QovP, D0, Z0, Theta0, CovMat);
    }
    inline HepTrackHelix* GetTrackRefit() const {
	// Returns track helix with track parameters of 
	// associated track (global) after refit with muon hypothesis
	return fTrackRefit;
    }
    inline virtual void GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const {
	// Gives the matrix from track-refit with muon mass hypothesis
	// after coordinate transformation
	//fTrackRefit->GetCovMatrixPtEtaPhi(CovMatPtEtaPhi);
	GetIDTrack()->GetCovMatrixPtEtaPhi(CovMatPtEtaPhi);
    } 

    inline Int_t GetMuonSpecExtrapolCharge() const {return fMuonSpecExtrapolCharge; }
    
    inline const TLorentzVector& PMuonSpecExtrapol() const {
      // Return 4-momentum vector (GeV)
      return fPMuonSpecExtrapol;
    }
    
    inline Float_t MuonSpecExtrapolPt() {
      // Return transvere momentum (GeV)
      return fPMuonSpecExtrapol.Perp();
    }
    
    inline Float_t MuonSpecExtrapolPx() const {
      // Return momentum x component (GeV)
      return fPMuonSpecExtrapol.Px();
    }
    
    inline Float_t MuonSpecExtrapolPy() const {
      // Return momentum y component (GeV)
      return fPMuonSpecExtrapol.Py();
    }
    
    inline Float_t MuonSpecExtrapolPz() const {
      // Return momentum z component (GeV)
      return fPMuonSpecExtrapol.Pz();
    }
    
    inline Float_t MuonSpecExtrapolEt() const {
      // Return transverse energy (GeV)
      return fPMuonSpecExtrapol.Et();
    }
    
    inline Float_t MuonSpecExtrapolEta() const {
      // Return pseudo-rapidity
      Float_t eta = 0.;
      if ( fPMuonSpecExtrapol.CosTheta()*fPMuonSpecExtrapol.CosTheta() < 1. ) {
	eta = fPMuonSpecExtrapol.Eta();
      } else {
	eta = ( fPMuonSpecExtrapol.Pz() > 0. ) ? 10e10 : -10e10;
      }
      return eta;
    }
    
    inline Float_t MuonSpecExtrapolPhi() const {
      // Return azimuth (rad)
      return fPMuonSpecExtrapol.Phi();
    }

    inline Float_t DeltaPhi0(AtlMuon *muon) {
	return  TVector2::Phi_mpi_pi(fTrackRefit->Phi0() - muon->GetTrackRefit()->Phi0());
    }


    //
    // Muon trigger match from SgTop D3PDs
    //
    inline void SetMuon_inTrigger(UInt_t Muon_inTrigger) {
	// Set Muon_inTrigger SgTop D3PD variable 
	fMuon_inTrigger = Muon_inTrigger;
    }
    inline UInt_t GetMuon_inTrigger() const {
	// Returns Muon_inTrigger SgTop D3PD variable 
	return fMuon_inTrigger;
    }
    inline Bool_t IsTriggerMatched() const { return fIsTriggerMatched; }
    inline void SetIsTriggerMatched(Bool_t is_matched) {
	fIsTriggerMatched = is_matched;
    }

    inline void SetMuonScaleFactors(Double_t mu_SF, Double_t mu_SF_reco_up, Double_t mu_SF_reco_down, Double_t mu_SF_Id_up, Double_t mu_SF_Id_down, Double_t mu_trigger_SF, Double_t mu_trigger_SF_down, Double_t mu_trigger_SF_up) {
	// Set Muon scale factors (nominal, up, down) SgTop D3PD variables
	fMu_SF      = mu_SF;
	fMu_SF_reco_up   = mu_SF_reco_up;
	fMu_SF_reco_down = mu_SF_reco_down;
	fMu_SF_Id_up   = mu_SF_Id_up;
	fMu_SF_Id_down = mu_SF_Id_down;
	fMu_trigger_SF = mu_trigger_SF;
	fMu_trigger_SF_up   = mu_trigger_SF_up;
	fMu_trigger_SF_down = mu_trigger_SF_down;
    }
    inline Double_t GetMuonScaleFactor()      { return fMu_SF;}
    inline Double_t GetMuonScaleFactor_reco_up()   { return fMu_SF_reco_up;}
    inline Double_t GetMuonScaleFactor_reco_down() { return fMu_SF_reco_down;}
    inline Double_t GetMuonScaleFactor_Id_up()   { return fMu_SF_Id_up;}
    inline Double_t GetMuonScaleFactor_Id_down() { return fMu_SF_Id_down;}
    inline Double_t GetMuonScaleFactor_trigger()   { return fMu_trigger_SF;}
    inline Double_t GetMuonScaleFactor_trigger_up()   { return fMu_trigger_SF_up;}
    inline Double_t GetMuonScaleFactor_trigger_down() { return fMu_trigger_SF_down;}

    inline const AtlMCTruthClassifier* GetMCTruthClassifier() const {
	return &fMCTruthClassifier;
    }
    inline void SetMCTruthClassifier(AtlMCTruthClassifier::EParticleOrigin
				     ParticleOrigin) {
	// Set MC truth classifier
	fMCTruthClassifier.Set(ParticleOrigin);
    }
    
    ClassDef(AtlMuon,21) // Atlas muon class
};
#endif
