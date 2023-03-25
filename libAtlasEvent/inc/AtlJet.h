//  
// author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlJet
#define ATLAS_AtlJet
#ifndef ROOT_HepJet
#include <HepJet.h>
#endif
#ifndef ROOT_TRef
#include <TRef.h>
#endif
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif
#ifndef ATLAS_AtlBTag
#include <AtlBTag.h>
#endif
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif
#ifndef ATLAS_AtlElectron
#include <AtlElectron.h>
#endif
#ifndef ATLAS_AtlMuon
#include <AtlMuon.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef ATLAS_AtlK0sDecayPiPi
#include <AtlK0sDecayPiPi.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#ifndef ATLAS_AtlMETWeights
#include <AtlMETWeights.h>
#endif



class AtlJet : public HepJet, public AtlTriggerMatch, public AtlMETWeights {

 public:
    enum EType {
	kInvalidType = -1,
	kCone4H1Tower, kCone7H1Tower,
	kCone4H1Topo, kCone7H1Topo,
	kMCCone4Hadron, kMCCone7Hadron, 
	kMCAntiKt4Hadron, kMCAntiKt6Hadron, kMCAntiKt4HadronPileup,
	kAntiKt4H1Topo, kAntiKt4H1Tower, kAntiKt6H1Tower,
	kAntiKt4TopoEM, kAntiKt4TopoEMJES, 
	kAntiKt4LCTopo, kAntiKt4Tower,  kAntiKt6TopoEM,
	kAntiKt4Topo, kAntiKt6Tower, kAntiKt6LCTopo,
	kAntiKt6Topo,
	kDL1r,
	kAtlFast, kNumTypes };
    
    enum EFlavour {
	kUnknownFlavour,
	kLightFlavoured,
	kCharm,
	kBeauty,
	kTau,
    // Insert new flavours above
	kNumFlavours
    };
    
    enum EJetQuality {
      kInvalid        = BIT(0),
      kIsUgly         = BIT(1),
      kIsBad          = BIT(2),
      kIsGood         = BIT(3),
      kIsLooseBad     = BIT(4),
      kIsMediumBad    = BIT(5),
      kIsTightBad     = BIT(6),
      kJetQuality = ( kInvalid | 
		      kIsUgly | 
		      kIsBad | 
		      kIsGood |
		      kIsLooseBad |
		      kIsMediumBad |
		      kIsTightBad )
		      };
  
 private:
    AtlBTag        fTaggers[AtlBTag::kNumTaggers]; // how this jet was tagged by various taggers
    EFlavour       fTruthFlavour;      // MC truth flavour set by Athena
    TRefArray     *fTruthParticles;    // Reference to HepMCTruth particles in jet cone sorted by Pt in descending order by Athena
    TRefArray     *fTracks;            // Reference to AtlIDTracks in the jet, set by Athena'sd TrackAssociation (filled with tracks inside a jetcone by ParticleToJetAssociator)
    TRefArray     *fK0sCanditates;     // Reference to AtlK0sDecayPiPi in the jet, set by K0sFinder
    TRef          *fFakeCandidate;     // Reference to a lepton/photon faking the jet
    EJetQuality    fJetQuality;        // Result of Invalid, Ugly, Bad or Good Jet Quality Criteria are described here: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HowToCleanJets
    TLorentzVector fP_EMSCALE;         // 4-momentum (Px, Py, Pz, E) in GeV of uncalibrated EMScale-jet
    TLorentzVector fP_JESCorrSCALE;    // 4-momentum (Px, Py, Pz, E) in GeV of corrected JES-jet

    //
    // Jet quality variables for computation of refined jet qualities
    //
    // NOTE:
    // These refined quality definitions are tested only for jets of type
    // AntiKt4Topo or AntiKt6Topo and ARE NOT SET for the "Tower" and "Cone" jets!
    Double_t fLArQuality;           //
    Double_t fHecQuality;           //
    Double_t fNegEnergy;            //
    Double_t fN90;                  //
    Double_t fEMFraction;           //
    Double_t fHecF;                 //
    Double_t fTiming;               //
    Int_t    fSamplingMax;          //
    Double_t fFracSamplingMax;      //
    Double_t fSumPtTrk;             //
    Double_t fEMJES_EtaCorr;        //
    Double_t fBCH_CORR_CELL;        // fraction of the jet energy coming from this correction 
    Double_t fBCH_CORR_JET;         // fraction of the jet energy coming from this correction
    Float_t  fEta_offsetJES;        // corrected eta (sgtop d3pds)

    Float_t  fTruthDeltaRminBeauty; // DeltaR to next true B hadron
    Float_t  fTruthDeltaRminCharm;  // DeltaR to next true charmed hadron
    Float_t  fTruthDeltaRminTau;    // DeltaR to next true tau

    Double_t fJet_SF_DL1R;               // jet scale factors with syst. variations (SgTopD3PDs)
    Double_t fJet_SF_btag_up_DL1R;       //
    Double_t fJet_SF_btag_down_DL1R;     //  DL1R
    Double_t fJet_SF_ctag_up_DL1R;       //
    Double_t fJet_SF_ctag_down_DL1R;     //
    //Double_t fJet_SF_mistag_up_MV1_70;     //
    //Double_t fJet_SF_mistag_down_MV1_70;   //


    Double_t fJet_SF_MV1_60;               // jet scale factors with syst. variations (SgTopD3PDs)
    Double_t fJet_SF_btag_up_MV1_60;       //
    Double_t fJet_SF_btag_down_MV1_60;     //  -> 60% efficiency working point
    Double_t fJet_SF_ctag_up_MV1_60;       //
    Double_t fJet_SF_ctag_down_MV1_60;     //
    Double_t fJet_SF_mistag_up_MV1_60;     //
    Double_t fJet_SF_mistag_down_MV1_60;   // 

    Double_t fJet_SF_MV1_70;               // jet scale factors with syst. variations (SgTopD3PDs)
    Double_t fJet_SF_btag_up_MV1_70;       //
    Double_t fJet_SF_btag_down_MV1_70;     //  -> 70% efficiency working point
    Double_t fJet_SF_ctag_up_MV1_70;       //
    Double_t fJet_SF_ctag_down_MV1_70;     //
    Double_t fJet_SF_mistag_up_MV1_70;     //
    Double_t fJet_SF_mistag_down_MV1_70;   //

    Double_t fJet_SF_MV1_80;               // jet scale factors with syst. variations (SgTopD3PDs)
    Double_t fJet_SF_btag_up_MV1_80;       //
    Double_t fJet_SF_btag_down_MV1_80;     //  -> 80% efficiency working point
    Double_t fJet_SF_ctag_up_MV1_80;       //
    Double_t fJet_SF_ctag_down_MV1_80;     //
    Double_t fJet_SF_mistag_up_MV1_80;     //
    Double_t fJet_SF_mistag_down_MV1_80;   //

    Double_t fJet_SF_MV1c_50;               // jet scale factors with syst. variations (SgTopD3PDs)
    Double_t fJet_SF_btag_up_MV1c_50;       //
    Double_t fJet_SF_btag_down_MV1c_50;     //  -> 50% efficiency working point
    Double_t fJet_SF_ctag_up_MV1c_50;       //
    Double_t fJet_SF_ctag_down_MV1c_50;     //
    Double_t fJet_SF_mistag_up_MV1c_50;     //
    Double_t fJet_SF_mistag_down_MV1c_50;   //

  public:
    AtlJet();
    AtlJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz,
	   EJetQuality jetquality,
	   TLorentzVector P_EMSCALE, TLorentzVector P_JESCorrSCALE,
	   Double_t EMJES_EtaCorr, Double_t BCH_CORR_CELL, Double_t BCH_CORR_JET,
	   Float_t eta_offsetJES);
    virtual ~AtlJet();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();
    const AtlBTag* GetTag(AtlBTag::ETagger tagger);
    AtlBTag* AddTag(AtlBTag::ETagger tagger, Double_t weight,
		    Bool_t IsValid);
    void AddTrack(AtlIDTrack *trk, Float_t deltaR_max); 
    static const char* NameOfType(EType type);
    void SetQualityVars(Double_t LArQuality,
			Double_t HecQuality,
			Double_t NegEnergy,
			Double_t N90,
			Double_t EMFraction,
			Double_t HecF,
			Double_t Timing,
			Int_t    SamplingMax,
			Double_t FracSamplingMax,
			Double_t SumPtTrk);
    void SetRefinedQualityFlag();
    EFlavour GetTruthFlavourByDeltaRmin(Float_t DeltaRmax) const;
	
    inline static Bool_t IsMCJet(EType type) {
	// Is detector level or parton level jet type ?
	return ( type == kMCCone4Hadron   ||
		 type == kMCCone7Hadron   ||
		 type == kMCAntiKt4Hadron ||
		 type == kMCAntiKt6Hadron ||
		 type == kMCAntiKt4HadronPileup );
    }

    inline void SetTruthFlavour(EFlavour TruthFlavour) {
	// Set MC truth flavour
	fTruthFlavour = TruthFlavour;
    } 
    inline EFlavour GetTruthFlavour() const {
	// Return MC true jet flavour
	return fTruthFlavour;
    }
    inline void AddTruthParticle(HepMCParticle *prt) {
	// Add particle to list of truth particles inside jet cone
	fTruthParticles->Add(prt);
    }
    inline const TRefArray* GetTruthParticles() const {
	// Return array of HepMCTruth particles in jet cone, sorted by Pt
        return fTruthParticles;
    }
    inline void AddTrack(AtlIDTrack * trk) {
	// Add track to list of tracks inside the jet
	fTracks->Add(trk);
    }
    inline const TRefArray* GetTracks() const {
	// Return array of AtlIDTrack's in the jet
	return fTracks;
    }
    inline void AddK0sCanditate(AtlK0sDecayPiPi * K0s) {
	// Add K0sCanditate to list of K0sCanditates inside the jet
	fK0sCanditates->Add(K0s);
    } 
    inline const TRefArray* GetK0sCanditates() const {
	// Return array of AtlK0sDecayPiPi's in the jet
	return fK0sCanditates;
    } 
    inline void SetFakeCandidate(HepParticle *prt) {
	// Fills the data member fFakeCandidate with lepton/photon identified as faking the jet
	*fFakeCandidate = (TObject*)prt;
    }
    inline HepParticle*  GetFakeCandidate() const {
	// Returns pointer to lepton/photon that fakes the jet
	return (HepParticle*)fFakeCandidate->GetObject();	
    }
    inline Bool_t IsFaked() const {
	// Is the jet faked by a lepton or a photon?
	return ( GetFakeCandidate() != 0 );
    }
    inline Bool_t IsInvalidJet() const { return fJetQuality & kInvalid; }
    inline Bool_t IsUglyJet() const { return fJetQuality & kIsUgly; }
    inline Bool_t IsBadJet() const { return fJetQuality & kIsBad; }
    inline Bool_t IsGoodJet() const { return fJetQuality & kIsGood; }
    inline Bool_t IsLooseBadJet() const { return fJetQuality & kIsLooseBad; }
    inline Bool_t IsMediumBadJet() const { return fJetQuality & kIsMediumBad; }
    inline Bool_t IsTightBadJet() const { return fJetQuality & kIsTightBad; }

    //inline Float_t GetEMFraction() const { return fEMFraction; }


    //
    // EMScale kinematics
    //
    inline const TLorentzVector& P_EMSCALE() const {
	// Return 4-momentum vector (GeV)
	return fP_EMSCALE;
    }
    
    inline Float_t EMScaleM() const {
	// Return invariant mass (GeV)
	return fP_EMSCALE.M();
    }
    inline Float_t EMScalePt() const {
	// Return transvere momentum (GeV)
	return fP_EMSCALE.Perp();
    }
    inline Float_t EMScalePx() const {
	// Return momentum x component (GeV)
	return fP_EMSCALE.Px();
    }
    inline Float_t EMScalePy() const {
	// Return momentum y component (GeV)
	return fP_EMSCALE.Py();
    }
    inline Float_t EMScalePz() const {
	// Return momentum z component (GeV)
	return fP_EMSCALE.Pz();
    }
    inline Float_t EMScaleEt() const {
	// Return transverse energy (GeV)
	return fP_EMSCALE.Et();
    }
    inline Float_t EMScaleEta() const {
	// Return pseudo-rapidity
	Float_t eta = 0.;
	if ( fP_EMSCALE.CosTheta()*fP_EMSCALE.CosTheta() < 1. ) {
	    eta = fP_EMSCALE.Eta();
	} else {
	    eta = ( fP_EMSCALE.Pz() > 0. ) ? 10e10 : -10e10;
	}
	return eta;
    }
    inline Float_t EMScaleTheta() const {
	// Return zenith (rad)
	return fP_EMSCALE.Theta();
    }    
    inline Float_t EMScalePhi() const {
	// Return azimuth (rad)
	return fP_EMSCALE.Phi();
    }

    Float_t EMScaleDeltaR(AtlElectron *el, const char* option = "");
    Float_t EMScaleDeltaR(AtlMuon *mu, const char* option = "");

    
    //
    // JESCorr kinematics
    //
    inline Float_t JES_Pt() const {
	// Return transvere momentum (GeV)
	return fP_JESCorrSCALE.Pt();
    }
    
    inline Float_t JES_Px() const {
	//Return momentum in x (GeV)
	return fP_JESCorrSCALE.Px();
    }
    
    inline Float_t JES_Py() const {
	//Return momentum in y (GeV)
	return fP_JESCorrSCALE.Py();
    }
    
    inline Float_t JES_Pz() const {
	//Return momentum in z (GeV)
	return fP_JESCorrSCALE.Pz();
    }

    inline Float_t JES_Eta() const {
	// Return pseudo-rapidity
	return fP_JESCorrSCALE.Eta();
    }
    
    inline Float_t JES_Phi() const {
	// Return azimuth (rad)
	return fP_JESCorrSCALE.Phi();
    }

    inline Float_t JES_E() const {
	// Return energy (GeV)
	return fP_JESCorrSCALE.E();
    }

    // EMJES Eta Correction
    inline Double_t EMJES_EtaCorr() const {
	return fEMJES_EtaCorr;
    }

    inline Double_t Get_BCH_CORR_JET() const {
      return fBCH_CORR_JET;
    }
    
    inline Double_t Get_BCH_CORR_CELL() const {
      return fBCH_CORR_CELL;
    }

    inline Float_t GetEta_offsetJES() {
	// corrected eta (sgtop d3pds)
	return fEta_offsetJES;
    }
    
    inline Float_t GetTruthDeltaRminBeauty() const {
	//
	// Get DeltaR to next true B hadron
	//
	return fTruthDeltaRminBeauty;
    }
    inline Float_t GetTruthDeltaRminCharm() const {
	//
	// Get DeltaR to next true charmed hadron
	//
	return fTruthDeltaRminCharm;
    }
    inline Float_t GetTruthDeltaRminTau() const {
	//
	// Get DeltaR to next true tau
	//
	return fTruthDeltaRminTau;
    }
    inline void SetTruthDeltaRminBeauty(Float_t  TruthDeltaRminBeauty) {
	//
	// Set DeltaR to next true B hadron
	//
	fTruthDeltaRminBeauty = TruthDeltaRminBeauty;
    }
    inline void SetTruthDeltaRminCharm(Float_t TruthDeltaRminCharm) {
	// 
	// Set DeltaR to next true charmed hadron
	//
	fTruthDeltaRminCharm = TruthDeltaRminCharm;
    }
    inline void SetTruthDeltaRminTau(Float_t TruthDeltaRminTau) {
	//
	// Set DeltaR to next true tau
    	fTruthDeltaRminTau = TruthDeltaRminTau;
    }
    inline Double_t GetJetScaleFactor_DL1R()           {std::cout<<"DL1R AtlJet"<<std::endl; return fJet_SF_DL1R;}
    inline Double_t GetJetScaleFactor_btag_up_DL1R()   { return fJet_SF_btag_up_DL1R;}
    inline Double_t GetJetScaleFactor_btag_down_DL1R() { return fJet_SF_btag_down_DL1R;}
    inline Double_t GetJetScaleFactor_ctag_up_DL1R()   { return fJet_SF_ctag_up_DL1R;}
    inline Double_t GetJetScaleFactor_ctag_down_DL1R() { return fJet_SF_ctag_down_DL1R;}
    //inline Double_t GetJetScaleFactor_mistag_up_DL1R()   { return fJet_SF_mistag_up_DL1R;}
    //inline Double_t GetJetScaleFactor_mistag_down_DL1R() { return fJet_SF_mistag_down_DL1R;}

    inline void SetJetScaleFactors_MV1_60(Double_t jet_SF,
					  Double_t jet_SF_btag_up,
					  Double_t jet_SF_btag_down,
					  Double_t jet_SF_ctag_up,
					  Double_t jet_SF_ctag_down,
					  Double_t jet_SF_mistag_up,
					  Double_t jet_SF_mistag_down) {
	// Set Jet scale factors (nominal, up, down) SgTop D3PD variables
	fJet_SF_MV1_60             = jet_SF;
	fJet_SF_btag_up_MV1_60     = jet_SF_btag_up;
	fJet_SF_btag_down_MV1_60   = jet_SF_btag_down;
	fJet_SF_ctag_up_MV1_60     = jet_SF_ctag_up;
	fJet_SF_ctag_down_MV1_60   = jet_SF_ctag_down;
	fJet_SF_mistag_up_MV1_60   = jet_SF_mistag_up;
	fJet_SF_mistag_down_MV1_60 = jet_SF_mistag_down;
    }
    inline Double_t GetJetScaleFactor_MV1_60()           { return fJet_SF_MV1_60;}
    inline Double_t GetJetScaleFactor_btag_up_MV1_60()   { return fJet_SF_btag_up_MV1_60;}
    inline Double_t GetJetScaleFactor_btag_down_MV1_60() { return fJet_SF_btag_down_MV1_60;}
    inline Double_t GetJetScaleFactor_ctag_up_MV1_60()   { return fJet_SF_ctag_up_MV1_60;}
    inline Double_t GetJetScaleFactor_ctag_down_MV1_60() { return fJet_SF_ctag_down_MV1_60;}
    inline Double_t GetJetScaleFactor_mistag_up_MV1_60()   { return fJet_SF_mistag_up_MV1_60;}
    inline Double_t GetJetScaleFactor_mistag_down_MV1_60() { return fJet_SF_mistag_down_MV1_60;}

    inline void SetJetScaleFactors_MV1_70(Double_t jet_SF,
					  Double_t jet_SF_btag_up,
					  Double_t jet_SF_btag_down,
					  Double_t jet_SF_ctag_up,
					  Double_t jet_SF_ctag_down,
					  Double_t jet_SF_mistag_up,
					  Double_t jet_SF_mistag_down) {
	// Set Jet scale factors (nominal, up, down) SgTop D3PD variables
	fJet_SF_MV1_70             = jet_SF;
	fJet_SF_btag_up_MV1_70     = jet_SF_btag_up;
	fJet_SF_btag_down_MV1_70   = jet_SF_btag_down;
	fJet_SF_ctag_up_MV1_70     = jet_SF_ctag_up;
	fJet_SF_ctag_down_MV1_70   = jet_SF_ctag_down;
	fJet_SF_mistag_up_MV1_70   = jet_SF_mistag_up;
	fJet_SF_mistag_down_MV1_70 = jet_SF_mistag_down;
    }
    inline Double_t GetJetScaleFactor_MV1_70()           { return fJet_SF_MV1_70;}
    inline Double_t GetJetScaleFactor_btag_up_MV1_70()   { return fJet_SF_btag_up_MV1_70;}
    inline Double_t GetJetScaleFactor_btag_down_MV1_70() { return fJet_SF_btag_down_MV1_70;}
    inline Double_t GetJetScaleFactor_ctag_up_MV1_70()   { return fJet_SF_ctag_up_MV1_70;}
    inline Double_t GetJetScaleFactor_ctag_down_MV1_70() { return fJet_SF_ctag_down_MV1_70;}
    inline Double_t GetJetScaleFactor_mistag_up_MV1_70()   { return fJet_SF_mistag_up_MV1_70;}
    inline Double_t GetJetScaleFactor_mistag_down_MV1_70() { return fJet_SF_mistag_down_MV1_70;}

    inline void SetJetScaleFactors_MV1_80(Double_t jet_SF,
					  Double_t jet_SF_btag_up,
					  Double_t jet_SF_btag_down,
					  Double_t jet_SF_ctag_up,
					  Double_t jet_SF_ctag_down,
					  Double_t jet_SF_mistag_up,
					  Double_t jet_SF_mistag_down) {
	// Set Jet scale factors (nominal, up, down) SgTop D3PD variables
	fJet_SF_MV1_80             = jet_SF;
	fJet_SF_btag_up_MV1_80     = jet_SF_btag_up;
	fJet_SF_btag_down_MV1_80   = jet_SF_btag_down;
	fJet_SF_ctag_up_MV1_80     = jet_SF_ctag_up;
	fJet_SF_ctag_down_MV1_80   = jet_SF_ctag_down;
	fJet_SF_mistag_up_MV1_80   = jet_SF_mistag_up;
	fJet_SF_mistag_down_MV1_80 = jet_SF_mistag_down;
    }
    inline Double_t GetJetScaleFactor_MV1_80()           { return fJet_SF_MV1_80;}
    inline Double_t GetJetScaleFactor_btag_up_MV1_80()   { return fJet_SF_btag_up_MV1_80;}
    inline Double_t GetJetScaleFactor_btag_down_MV1_80() { return fJet_SF_btag_down_MV1_80;}
    inline Double_t GetJetScaleFactor_ctag_up_MV1_80()   { return fJet_SF_ctag_up_MV1_80;}
    inline Double_t GetJetScaleFactor_ctag_down_MV1_80() { return fJet_SF_ctag_down_MV1_80;}
    inline Double_t GetJetScaleFactor_mistag_up_MV1_80()   { return fJet_SF_mistag_up_MV1_80;}
    inline Double_t GetJetScaleFactor_mistag_down_MV1_80() { return fJet_SF_mistag_down_MV1_80;}

    inline void SetJetScaleFactors_MV1c_50(Double_t jet_SF,
					   Double_t jet_SF_btag_up,
					   Double_t jet_SF_btag_down,
					   Double_t jet_SF_ctag_up,
					   Double_t jet_SF_ctag_down,
					   Double_t jet_SF_mistag_up,
					   Double_t jet_SF_mistag_down) {
	// Set Jet scale factors (nominal, up, down) SgTop D3PD variables
	fJet_SF_MV1c_50             = jet_SF;
	fJet_SF_btag_up_MV1c_50     = jet_SF_btag_up;
	fJet_SF_btag_down_MV1c_50   = jet_SF_btag_down;
	fJet_SF_ctag_up_MV1c_50     = jet_SF_ctag_up;
	fJet_SF_ctag_down_MV1c_50   = jet_SF_ctag_down;
	fJet_SF_mistag_up_MV1c_50   = jet_SF_mistag_up;
	fJet_SF_mistag_down_MV1c_50 = jet_SF_mistag_down;
    }
    inline Double_t GetJetScaleFactor_MV1c_50()           { return fJet_SF_MV1c_50;}
    inline Double_t GetJetScaleFactor_btag_up_MV1c_50()   { return fJet_SF_btag_up_MV1c_50;}
    inline Double_t GetJetScaleFactor_btag_down_MV1c_50() { return fJet_SF_btag_down_MV1c_50;}
    inline Double_t GetJetScaleFactor_ctag_up_MV1c_50()   { return fJet_SF_ctag_up_MV1c_50;}
    inline Double_t GetJetScaleFactor_ctag_down_MV1c_50() { return fJet_SF_ctag_down_MV1c_50;}
    inline Double_t GetJetScaleFactor_mistag_up_MV1c_50()   { return fJet_SF_mistag_up_MV1c_50;}
    inline Double_t GetJetScaleFactor_mistag_down_MV1c_50() { return fJet_SF_mistag_down_MV1c_50;}

    ClassDef(AtlJet,45) // Atlas detector level jet
};
#endif

