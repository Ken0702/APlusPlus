//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlElectron
#define ATLAS_AtlElectron
#ifndef HEP_HepElectron
#include <HepElectron.h>
#endif
#ifndef ATLAS_AtlEMShower
#include <AtlEMShower.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#ifndef ATLAS_AtlMCTruthClassifier
#include <AtlMCTruthClassifier.h>
#endif

class TSystem;

class AtlElectron : public HepElectron, public AtlEMShower,
    public AtlTriggerMatch {
    
  private:
    AtlMCTruthClassifier fMCTruthClassifier; // MC truth classifier
    HepTrackHelix *fTrackEMRefit; // Track parameters after refit with electron hypothesis
    TRef          *fIDTrack;      // Link to original Track
    UInt_t         fElectron_trackBlayer; // Electron_trackBlayer SgTop D3PD variable
    Bool_t         fIsTriggerMatched;  //WARNING: Only filled in D3PDTopSlim mode
    Double_t       fEl_SF;            // electron scale factors with syst. variations (SgTopD3PDs)
    Double_t       fEl_SF_reco_up;
    Double_t       fEl_SF_reco_down;
    Double_t       fEl_SF_Id_up;
    Double_t       fEl_SF_Id_down;
    Double_t       fEl_trigger_SF;
    Double_t       fEl_trigger_SF_up;
    Double_t       fEl_trigger_SF_down;
    
  public:
    AtlElectron();
    AtlElectron(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
		Bool_t IsPositron, 
		Float_t EMWeight,
		Float_t BkgWeight, UInt_t OQFlag,
		UInt_t IsEMBitField, EIsEM IsEM, EAuthor Author,
		TLorentzVector PCluster);
    virtual ~AtlElectron();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    
    static void PrintHeader();
    static void PrintFooter();

    inline const AtlMCTruthClassifier* GetMCTruthClassifier() const {
	return &fMCTruthClassifier;
    }
    inline void SetMCTruthClassifier(AtlMCTruthClassifier::EParticleOrigin
				     ParticleOrigin) {
	// Set MC truth classifier
	fMCTruthClassifier.Set(ParticleOrigin);
    }
    
    inline Bool_t IsHighPtElectron()  const { return fAuthor & kHighPtElectron; }
    inline Bool_t IsSoftElectron()    const { return fAuthor & kSoftElectron;}
    inline Bool_t IsForwardElectron() const { return fAuthor & kForwardElectron;}
    inline Bool_t IsPhotoConversion() const { return fAuthor & kPhoton;}
    
    inline void SetTrackEMRefit(Float_t Chi2, Int_t NDoF,
				Float_t Xref, Float_t Yref, Float_t Zref, Float_t Phi0,
				Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta0, 
				Float_t CovMat[15]) {
	// Set track refit parameters, Id always 1
	fTrackEMRefit->SetParameters(1, Chi2, NDoF, Xref, Yref, Zref, Phi0,
				     QovP, D0, Z0, Theta0, CovMat);
    }
 

    // Test hardness of cuts
    inline Bool_t IsLoose() const {
	// Loose electron selection
	return fIsEM & kElectronLoose;
    }
    inline Bool_t IsFwdLoose() const {
	// Loose selection for forwrd electrons
	return fIsEM & kElectronFwdLoose;
    }
    inline Bool_t IsMedium() const {
	// Medium electron selection
	return fIsEM & kElectronMedium;
    }
    inline Bool_t IsTight() const {
	// Tight electron selection
	return fIsEM & kElectronTight;
    }
    inline Bool_t IsTightIso() const {
	// Tight + isolation selection for electrons
	return fIsEM & kElectronTightIso;
    }
    inline Bool_t IsMediumIso() const {
	// Medium + isolation selection for electrons
	return fIsEM & kElectronMediumIso;
    }
    inline Bool_t IsFwdTight() const {
	// Tight selection for forwrd electrons
	return fIsEM & kElectronFwdTight;
    }
    inline Bool_t IsMediumPP() const {
	// Medium electron selection
	return fIsEM & kElectronMediumPP;
    }
    inline Bool_t IsTightPP() const {
	// Tight electron selection
	return fIsEM & kElectronTightPP;
    }

    inline Bool_t Is90EtCone20() const {
	// Tight electron selection
	return fIsEM & kElectron90EtCone20;
    }

    inline Bool_t Is90PtCone30() const {
	// Tight electron selection
	return fIsEM & kElectron90PtCone30;
    }
    
    // Get OQ decision
    UInt_t  IsGoodOQ(); // 0: is good OQ, 1: is bad OQ
    
    // Test individual IsEM flags
    inline virtual Bool_t IsEM_ClusterEtaRange() const {
      // Cut eta range
      return (fIsEMBitField & (0x1<<0))==0;
    }
    inline virtual Bool_t IsEM_ClusterHadronicLeakage() const {
      // Cluster leakage into the hadronic calorimeter
      return (fIsEMBitField & (0x1<<2))==0;
    }
    inline virtual Bool_t IsEM_ClusterMiddleEnergy() const {
      // Energy in 2nd sampling (e277)
      return (fIsEMBitField & (0x1<<3))==0;
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio37() const {
      // Energy ratio in 2nd sampling
      return (fIsEMBitField & (0x1<<4))==0;
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio33() const {
      // Energy ratio in 2nd sampling
      return (fIsEMBitField & (0x1<<5))==0;
    }
    inline virtual Bool_t IsEM_ClusterMiddleWidth() const {
      // Width in the second sampling */
      return (fIsEMBitField & (0x1<<6))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsEratio() const {
      // Fraction of energy found in 1st sampling
      return (fIsEMBitField & (0x1<<8))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsDeltaEmax2() const {
      // Energy of 2nd maximum in 1st sampling
      // ~e2tsts1/(1000+const_lumi*et)
      return (fIsEMBitField & (0x1<<9))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsDeltaE() const {
      // Difference between 2nd maximum and 1st minimum in
      // strips (e2tsts1-emins1)
      return (fIsEMBitField & (0x1<<10))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsWtot() const {
      // Shower width in 1st sampling
      return (fIsEMBitField & (0x1<<11))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsFracm() const {
      // Shower shape in shower core 1st sampling
      return (fIsEMBitField & (0x1<<12))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsWeta1c() const {
      // Shower width weighted by distance from the maximum one
      return (fIsEMBitField & (0x1<<13))==0;
    }
    inline virtual Bool_t IsEM_ClusterStripsDEmaxs1() const {
      // Difference between max and 2nd max in strips
      return (fIsEMBitField & (0x1<<15))==0;
    }
    inline virtual Bool_t IsEM_TrackBlayer() const {
      // B layer hit
      return (fIsEMBitField & (0x1<<16))==0;
    }
    inline virtual Bool_t IsEM_TrackPixel() const {
      // Number of Pixel hits
      return (fIsEMBitField & (0x1<<17))==0;
    }
    inline virtual Bool_t IsEM_TrackSi() const {
      // Number of Pixel and SCT hits
      return (fIsEMBitField & (0x1<<18))==0;
    }
    inline virtual Bool_t IsEM_TrackA0() const {
      // Distance of closet approach
      return (fIsEMBitField & (0x1<<19))==0;
    }
    inline virtual Bool_t IsEM_TrackMatchEta() const {
      // Eta difference between cluster and
      // extrapolated track in the 1st sampling
	return (fIsEMBitField & (0x1<<20))==0;
    }
    inline virtual Bool_t IsEM_TrackMatchPhi() const {
      // Phi difference between cluster and
      // extrapolated track in the 2nd sampling
      return (fIsEMBitField & (0x1<<21))==0;
    }
    inline virtual Bool_t IsEM_TrackMatchEoverP() const {
      // Energy-momentum match
      return (fIsEMBitField & (0x1<<22))==0;
    }
    inline virtual Bool_t IsEM_TrackTRThits() const {
      // Number of TRT hits
      return (fIsEMBitField & (0x1<<24))==0;
    }
    inline virtual Bool_t IsEM_TrackTRTratio() const {
      // Ratio of high to all TRT hits for
      // isolated electrons
      return (fIsEMBitField & (0x1<<25))==0;
    }
    inline virtual Bool_t IsEM_TrackTRTratio90() const {
      // Ratio of high to all TRT hits for
      // non-isolated electrons
      return (fIsEMBitField & (0x1<<26))==0;
    }
    inline virtual Bool_t IsEM_Isolation() const {
      // Isolation
      return (fIsEMBitField & (0x1<<29))==0;
    }
    inline virtual Bool_t IsEM_ClusterIsolation() const {
      // Calorimetric isolation
      return (fIsEMBitField & (0x1<<30))==0;
    }
    inline virtual Bool_t IsEM_TrackIsolation() const {
      // Tracker isolation
      return (fIsEMBitField & (0x1<<31))==0;
    }
    
    // Tracking
    inline HepTrackHelix* GetTrackEMRefit() const {
      // Returns track helix with track parameters of 
      // associated track after refit with electron hypothesis
      return fTrackEMRefit;
    }
    inline void SetIDTrack(AtlIDTrack *trk) { *fIDTrack = (TObject*)trk; }
    inline AtlIDTrack* GetIDTrack() const{
      // Returns original associated track
      return (AtlIDTrack*)fIDTrack->GetObject(); 
    }
    inline virtual void GetCovMatrixPtEtaPhi(TMatrixD& CovMatPtEtaPhi) const {
	// Returns Covariance Matrix from track fit after coordinate transformation
	GetIDTrack()->GetCovMatrixPtEtaPhi(CovMatPtEtaPhi);
    }

    // SgTop D3PD variables
    inline void SetElectron_trackBlayer(UInt_t Electron_trackBlayer) {
	// Set Electron_trackBlayer SgTop D3PD variable 
	fElectron_trackBlayer = Electron_trackBlayer;
    }
    inline UInt_t GetElectron_trackBlayer() const {
	// Returns Electron_trackBlayer SgTop D3PD variable 
	return fElectron_trackBlayer;
    }
    inline Bool_t IsTriggerMatched() const { return fIsTriggerMatched; }
    inline void SetIsTriggerMatched(Bool_t is_matched) {
	fIsTriggerMatched = is_matched;
    }
    inline void SetElectronScaleFactors(Double_t el_SF, Double_t el_SF_reco_up, Double_t el_SF_reco_down, Double_t el_SF_Id_up, Double_t el_SF_Id_down, Double_t el_trigger_SF,Double_t el_trigger_SF_up, Double_t el_trigger_SF_down) {
	// Set Electron scale factors (nominal, up, down) SgTop D3PD variables
	fEl_SF      = el_SF;
	fEl_SF_reco_up   = el_SF_reco_up;
	fEl_SF_reco_down = el_SF_reco_down;
	fEl_SF_Id_up   = el_SF_Id_up;
	fEl_SF_Id_down = el_SF_Id_down;
	fEl_trigger_SF   = el_trigger_SF;
	fEl_trigger_SF_up   = el_trigger_SF_up;
	fEl_trigger_SF_down = el_trigger_SF_down;
    }
    inline Double_t GetElectronScaleFactor()      { return fEl_SF;}
    inline Double_t GetElectronScaleFactor_reco_up()   { return fEl_SF_reco_up;}
    inline Double_t GetElectronScaleFactor_reco_down() { return fEl_SF_reco_down;}
    inline Double_t GetElectronScaleFactor_Id_up()   { return fEl_SF_Id_up;}
    inline Double_t GetElectronScaleFactor_Id_down() { return fEl_SF_Id_down;}
    inline Double_t GetElectronScaleFactor_trigger()   { return fEl_trigger_SF;}
    inline Double_t GetElectronScaleFactor_trigger_up()   { return fEl_trigger_SF_up;}
    inline Double_t GetElectronScaleFactor_trigger_down() { return fEl_trigger_SF_down;}
 

    ClassDef(AtlElectron,29) // Atlas electron class
};
#endif
      
