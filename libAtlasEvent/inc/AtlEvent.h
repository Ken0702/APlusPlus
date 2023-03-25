//
// author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvent
#define ATLAS_AtlEvent
#ifndef HEP_HepEvent
#include <HepEvent.h>
#endif
#ifndef HEP_HepVertex
#include <HepVertex.h>
#endif
#ifndef ATLAS_AtlEventHeader
#include <AtlEventHeader.h>
#endif
#ifndef ATLAS_AtlJet
#include <AtlJet.h>
#endif
#ifndef ATLAS_AtlMCJet
#include <AtlMCJet.h>
#endif
#ifndef ATLAS_AtlIDTrack
#include <AtlIDTrack.h>
#endif
#ifndef ATLAS_AtlTrigger
#include <AtlTrigger.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#ifndef ATLAS_AtlEnergySum
#include <AtlEnergySum.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif
#ifndef HEP_HepMCVertex
#include <HepMCVertex.h>
#endif
#ifndef ATLAS_AtlPixelHit
#include "AtlPixelHit.h"
#endif
#ifndef ATLAS_AtlSCT3DHit
#include "AtlSCT3DHit.h"
#endif
#ifndef ATLAS_AtlElectron
#include <AtlElectron.h>
#endif
#ifndef ATLAS_AtlMuon
#include <AtlMuon.h>
#endif
#ifndef ATLAS_AtlTau
#include <AtlTau.h>
#endif
#ifndef ATLAS_AtlPhoton
#include <AtlPhoton.h>
#endif
#ifndef ATLAS_AtlFastElectron
#include <AtlFastElectron.h>
#endif
#ifndef ATLAS_AtlFastMuon
#include <AtlFastMuon.h>
#endif
#ifndef ATLAS_AtlFastTau
#include <AtlFastTau.h>
#endif
#ifndef ATLAS_AtlFastPhoton
#include <AtlFastPhoton.h>
#endif
#ifndef ATLAS_AtlFastJet
#include <AtlFastJet.h>
#endif
#ifndef ATLAS_AtlTopPair
#include <AtlTopPair.h>
#endif
#ifndef HEP_HepTopDecay
#include <HepTopDecay.h>
#endif
#ifndef ATLAS_AtlWDecayLNu
#include <AtlWDecayLNu.h>
#endif
#ifndef ATLAS_AtlWDecayJJ
#include <AtlWDecayJJ.h>
#endif
#ifndef ATLAS_AtlZ0DecayEE
#include <AtlZ0DecayEE.h>
#endif
#ifndef ATLAS_AtlZ0DecayMuMu
#include <AtlZ0DecayMuMu.h>
#endif
#ifndef ATLAS_AtlZ0DecayTauTau
#include <AtlZ0DecayTauTau.h>
#endif
#ifndef ATLAS_AtlZ0DecayJJ
#include <AtlZ0DecayJJ.h>
#endif
#ifndef ATLAS_AtlK0sDecayPiPi
#include <AtlK0sDecayPiPi.h>
#endif
#ifndef ATLAS_AtlPhotonConv
#include <AtlPhotonConv.h>
#endif
#ifndef ATLAS_AtlLambdaDecayPPi
#include <AtlLambdaDecayPPi.h>
#endif
#ifndef ATLAS_AtlD0DecayKPi
#include <AtlD0DecayKPi.h>
#endif
#ifndef ATLAS_AtlDstarDecayDPi
#include <AtlDstarDecayDPi.h>
#endif
#ifndef ATLAS_AtlPhiDecayKK
#include <AtlPhiDecayKK.h>
#endif
#ifndef ATLAS_AtlTriggerItem
#include <AtlTriggerItem.h>
#endif
#ifndef HEP_HepDriftChamberDigit
#include <HepDriftChamberDigit.h>
#endif
#ifndef ROOT_TClonesArray
#include <TClonesArray.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TTree
#include <TTree.h>
#endif
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>

class HepDatabasePDG;
class TSystem;
class TArrayF;
class TArrayI;
class TDirectory;

class AtlEvent : public HepEvent {

private:
    AtlEventHeader  fEventHeader;         // Event header
    AtlTrigger     *fTrigger;             // Trigger decisions
    AtlEnergySum    fEnergySum;           // Energy sums (SumEt, missing Et)
    Int_t           fN_Cone4H1TowerJets;  // No. of cone4-H1-tower jets
    Int_t           fN_Cone7H1TowerJets;  // No. of cone7-H1-tower jets
    Int_t           fN_Cone4H1TopoJets;   // No. of cone4-H1-topo jets
    Int_t           fN_Cone7H1TopoJets;   // No. of cone7-H1-topo jets
    Int_t           fN_MCCone4HadronJets; // No. of cone4 hadron level jets
    Int_t           fN_MCCone7HadronJets; // No. of cone7 hadron level jets
    Int_t           fN_MCAntiKt4HadronJets;// No. of antikt4 hadron level jets
    Int_t           fN_MCAntiKt6HadronJets;// No. of antikt6 hadron level jets
    Int_t           fN_MCAntiKt4HadronPileupJets;// No. of antikt4 hadron level jets
    Int_t           fN_AntiKt4H1TopoJets; // No. of antikt4-H1-topo jets
    Int_t           fN_AntiKt4H1TowerJets;// No. of antikt4-H1-tower jets
    Int_t           fN_AntiKt6H1TowerJets;// No. of antikt6-H1-tower jets
    Int_t           fN_AntiKt4TopoEMJets;
    Int_t           fN_AntiKt4TopoEMJESJets;
    Int_t           fN_AntiKt4LCTopoJets;
    Int_t           fN_AntiKt4TowerJets;
    Int_t           fN_AntiKt6TopoEMJets;
    Int_t           fN_AntiKt4TopoJets;
    Int_t           fN_AntiKt6TowerJets;
    Int_t           fN_AntiKt6LCTopoJets;
    Int_t           fN_AntiKt6TopoJets;
    Int_t           fN_DL1rJets;
    Int_t           fN_AtlFastJets;       // No. of AtlFast jets
    Int_t           fN_MCParticles;       // No. of MC truth particles
    Int_t           fN_MCVertices;        // No. of MC truth vertices
    Int_t           fN_IDTracks;          // No. of inner detector tracks (including clones!)
    Int_t           fN_TRTHits;           // No. of TRT digitisations
    Int_t           fN_SCTHits;           // No. of SCT digitisations
    Int_t           fN_PixelHits;         // No. of Pixel digitisations
    Int_t           fN_Electrons;         // No. of electrons
    Int_t           fN_Muons;             // No. of muons (staco)
    Int_t           fN_Taus;              // No. of taus
    Int_t           fN_Photons;           // No. of photons
    Int_t           fN_AtlFastElectrons;  // No. of AtlFast electrons
    Int_t           fN_AtlFastMuons;      // No. of AtlFast muons
    Int_t           fN_AtlFastTaus;       // No. of AtlFast taus
    Int_t           fN_AtlFastPhotons;    // No. of AtlFast photons
    Int_t           fN_TopPairs;          // No. of reconstructed top pairs
    Int_t           fN_TopDecays;         // No. of reconstructed top decays
    Int_t           fN_WDecaysLNu;        // No. of reconstructed leptonic W decays
    Int_t           fN_WDecaysJJ;         // No. of reconstructed hadronic W decays
    Int_t           fN_Neutrinos;         // No. of reconstructed neutrinos
    Int_t           fN_Z0Decays;          // No. of reconstructed Z0 decays
    Int_t           fN_PhiDecaysKK;       // No. of reconstructed Phi decays
    Int_t           fN_K0sDecaysPiPi;     // No. of reconstructed K0s decays
    Int_t           fN_PhotonConv;         // No. of reconstructed photo conversions
    Int_t           fN_LambdaDecaysPiPi;  // No. of reconstructed Lambda decays
    Int_t           fN_D0DecaysKPi;       // No. of reconstructed D0 decays
    Int_t           fN_DstarDecaysDPi;    // No. of reconstructed Dstar decays
    Int_t           fN_Vertices;          // No. of vertices
    Int_t           fN_vpx;               // No. of primary vertices (stored in SgTopD3PDs)
    Float_t         fbdt_scale;
    TClonesArray *fCone4H1TowerJets;   //-> Cone4-H1-tower jets @detector level
    TClonesArray *fCone7H1TowerJets;   //-> Cone7-H1-tower jets @detector level
    TClonesArray *fCone4H1TopoJets;    //-> Cone4-H1-topo jets @detector level
    TClonesArray *fCone7H1TopoJets;    //-> Cone7-H1-topo jets @detector level
    TClonesArray *fMCCone4HadronJets;  //-> Cone4 hadron level jets
    TClonesArray *fMCCone7HadronJets;  //-> Cone7 hadron level jets
    TClonesArray *fMCAntiKt4HadronJets;//-> AntiKt4 hadron level jets
    TClonesArray *fMCAntiKt6HadronJets;//-> AntiKt6 hadron level jets
    TClonesArray *fMCAntiKt4HadronPileupJets;//-> AntiKt4 hadron level jets
    TClonesArray *fAntiKt4H1TopoJets;  //-> AntiKt4-H1-topo jets @detector level
    TClonesArray *fAntiKt4H1TowerJets; //-> AntiKt4-H1-tower jets @detector level
    TClonesArray *fAntiKt6H1TowerJets; //-> AntiKt6-H1-tower jets @detector level
    TClonesArray *fAntiKt4TopoEMJets;
    TClonesArray *fAntiKt4TopoEMJESJets;
    TClonesArray *fAntiKt4LCTopoJets;
    TClonesArray *fAntiKt4TowerJets;
    TClonesArray *fAntiKt6TopoEMJets;
    TClonesArray *fAntiKt4TopoJets;
    TClonesArray *fAntiKt6TowerJets;
    TClonesArray *fAntiKt6LCTopoJets;
    TClonesArray *fAntiKt6TopoJets;
    TClonesArray *fDL1rJets;
    TClonesArray *fAtlFastJets;       //-> AtlFast jets
    TClonesArray *fMCParticles;       //-> Array of MC truth particles
    TClonesArray *fMCVertices;        //-> Array of MC truth vertices
    TClonesArray *fIDTracks;          //-> Array of inner detector tracks
    TClonesArray *fTRTHits;           //-> Array of TRT digitisations
    TClonesArray *fSCTHits;           //-> Array of SCT hits
    TClonesArray *fPixelHits;         //-> Array of Pixel hits
    TClonesArray *fElectrons;         //-> Array of electrons
    TClonesArray *fMuons;             //-> Array of muons (staco)
    TClonesArray *fTaus;              //-> Array of taus
    TClonesArray *fPhotons;           //-> Array of photons
    TClonesArray *fAtlFastElectrons;  //-> Array of AtlFast electrons
    TClonesArray *fAtlFastMuons;      //-> Array of AtlFast muons
    TClonesArray *fAtlFastTaus;       //-> Array of AtlFast taus
    TClonesArray *fAtlFastPhotons;    //-> Array of AtlFast photons
    TClonesArray *fTopPairs;          //-> Array of reconstructed top pairs
    TClonesArray *fTopDecays;         //-> Array of reconstructed top decays
    TClonesArray *fWDecaysLNu;        //-> Array of reconstructed leptonic W decays
    TClonesArray *fWDecaysJJ;         //-> Array of reconstructed hadronic W decays
    TClonesArray *fNeutrinos;         //-> Array of reconstructed neutrinos (eg from W decays)
    TClonesArray *fZ0Decays;          //-> Array of reconstructed Z0 decays
    TClonesArray *fPhiDecaysKK;       //-> Array of reconstructed Phi decays
    TClonesArray *fK0sDecaysPiPi;     //-> Array of reconstructed K0s decays
    TClonesArray *fPhotonConv;        //-> Array of reconstructed photo conversions
    TClonesArray *fLambdaDecaysPiPi;  //-> Array of reconstructed Lambda decays
    TClonesArray *fD0DecaysKPi;       //-> Array of reconstructed D0 decays
    TClonesArray *fDstarDecaysDPi;    //-> Array of reconstructed Dstar decays
    TClonesArray *fVertices;          //-> Array of reconstructed vertices

    // SgTop D3PD variables
    Bool_t fIsEleMuOverlap;
    
public:
    AtlEvent();
    virtual ~AtlEvent();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "all") const;
    void Init();

    //
    // Event building functions
    //

    // Truth objects
    HepMCParticle* AddMCParticle(Int_t PdgCode, Float_t Px, Float_t Py,
				 Float_t Pz, Float_t E, Int_t MCStatus,
				 Bool_t IsGenerator, Bool_t IsGenNonInteracting,
				 Bool_t IsGenStable, Bool_t IsGenSimulStable,
				 Bool_t IsGenInteracting, Bool_t IsConversion,
				 Bool_t IsBremsstrahlung);
    HepMCVertex* AddMCVertex(Float_t x, Float_t y, Float_t z);

    // Reconstructed objects
    AtlTriggerItem* AddL1Match(AtlTriggerMatch *RecoObject, const char* L1ItemName);
    AtlTriggerItem* AddHLTMatch(AtlTriggerMatch *RecoObject, const char* HLTItemName);
    HepVertex* AddVertex(Float_t X, Float_t Y, Float_t Z, Float_t Chi2, Int_t NDoF, 
			 Float_t errX, Float_t errY, Float_t errZ, Int_t n_tracks, Int_t type);
    AtlIDTrack* AddIDTrack(Float_t Chi2, Int_t NDoF, Float_t Xref,
			   Float_t Yref, Float_t Zref, Float_t Phi0,
			   Float_t QovP, Float_t D0, Float_t Z0, Float_t Theta,
			   const Float_t CovMat[15] = 0);
    AtlIDTrack* AddIDTrack(AtlIDTrack *trk);
    AtlTRTDigit* AddTRTHit(Float_t DriftTime, Float_t DriftRadius, UInt_t Digit,
			   Int_t EndCapOrBarrel=-20, Int_t PhiSector=-1,
			   Int_t ModuleOrWheel=-1, Int_t StrawLayer=-1, Int_t Straw=-1);
    AtlPixelHit* AddPixelHit(Float_t X, Float_t Y, Float_t Z);
    AtlSCT3DHit* AddSCTHit(Float_t X, Float_t Y, Float_t Z);
    AtlElectron* AddElectron(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
			     Bool_t IsPositron, Float_t EMWeight,
			     Float_t BkgWeight, UInt_t OQFlag,
			     UInt_t IsEMBitField,
			     AtlEMShower::EIsEM IsEM,
			     AtlEMShower::EAuthor Author,
			     TLorentzVector PCluster);
    AtlMuon* AddMuon(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Bool_t IsMuPlus, 
		     Float_t EtCone10, Float_t EtCone20, Float_t EtCone30, Float_t EtCone40,
		     Int_t NtrkCone10, Int_t NtrkCone20, Int_t NtrkCone30, Int_t NtrkCone40,
		     Float_t PtCone10, Float_t PtCone20, Float_t PtCone30,
		     Float_t PtCone40, AtlMuon::EAuthor Author, AtlMuon::EQuality Quality,
		     Float_t MatchingChi2, Int_t MatchingNDoF,
		     Bool_t IsCombinedMuon,
		     TLorentzVector PMuonSpecExtrapol, 
		     Int_t MuonSpecExtrapolCharge);
    AtlTau* AddTau(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Bool_t IsTauPlus, 
		   AtlTau::EAuthor Author, AtlTau::ETauFlag TauFlag);
    AtlPhoton* AddPhoton(Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Float_t EMWeight,
			 Float_t BkgWeight, UInt_t OQFlag, 
			 UInt_t IsEMBitField,
			 AtlEMShower::EIsEM IsEM, AtlEMShower::EAuthor Author,
			 TLorentzVector PCluster);
    AtlJet* AddJet(AtlJet::EType type, Float_t E, Float_t Px, Float_t Py, Float_t Pz,
		   AtlJet::EJetQuality JetQuality,
		   TLorentzVector P_EMSCALE, TLorentzVector P_JESCorrSCALE,
		   Double_t EMJES_EtaCorr,
		   Double_t BCH_CORR_CELL, Double_t BCH_CORR_JET,
		   Float_t eta_offsetJES = 0.); 
    
    // ATLFAST objects
    AtlFastElectron* AddAtlFastElectron(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Bool_t IsPositron);
    AtlFastMuon* AddAtlFastMuon(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Bool_t IsMuPlus);
    AtlFastTau* AddAtlFastTau(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Bool_t IsTauPlus);
    AtlFastPhoton* AddAtlFastPhoton(Float_t Px, Float_t Py, Float_t Pz, Float_t E);
    
    // Objects reconstructed by finders
    AtlTopPair*  AddTopPair(HepTopDecay* top1, HepTopDecay* top2, Double_t chi2, Int_t ndof, AtlTopPair::EType type);
    HepTopDecay* AddTopDecay(Float_t Px_T, Float_t Py_T, Float_t Pz_T, Float_t E_T,
			     HepWDecay *WDecay, HepJet *BJetOrig,
			     Float_t Px_j, Float_t Py_j, Float_t Pz_j, Float_t E_j,
			     HepTopDecay::ProductionMode mode);
    AtlWDecayLNu* AddWDecayLNu(Float_t Px_W, Float_t Py_W, Float_t Pz_W, Float_t E_W,
			       HepParticle *lepton_orig, Float_t Px_lep,
			       Float_t Py_lep, Float_t Pz_lep, Float_t E_lep,
			       HepParticle* neutrino, HepWDecay::ProductionMode mode);
    AtlWDecayJJ* AddWDecayJJ(Float_t Px_W, Float_t Py_W, Float_t Pz_W,
			     Float_t E_W, AtlJet *jet1_orig, AtlJet *jet2_orig,
			     Float_t Px_j1, Float_t Py_j1, Float_t Pz_j1, Float_t E_j1,
			     Float_t Px_j2, Float_t Py_j2, Float_t Pz_j2, Float_t E_j2,
			     HepWDecay::ProductionMode mode);
    HepParticle* AddNeutrino(Float_t Px, Float_t Py, Float_t Pz, Float_t E, Int_t PdgCode);
    HepZ0Decay* AddZ0Decay(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
			   TObject* Daughter1, TObject* Daughter2,
	                   HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    HepZ0Decay* AddZ0Decay(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
			   TObject* Daughter1, TObject* Daughter2);
    AtlPhiDecayKK* AddPhiDecayKK(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				 AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
				 HepVertex* Vtx,
				 HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    AtlK0sDecayPiPi* AddK0sDecayPiPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				     AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
				     HepVertex* Vtx,
				     HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    AtlPhotonConv* AddPhotonConv(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
			       AtlIDTrack* Daughter1, AtlIDTrack* Daughter2,
			       HepVertex* Vtx,
			       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    AtlLambdaDecayPPi* AddLambdaDecayPiPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				      AtlIDTrack* Proton, AtlIDTrack* Pion, 
				      HepVertex* Vertex, HepVertex* PrimaryVtx,
				      HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);

    AtlD0DecayKPi* AddD0DecayKPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				 AtlIDTrack* Kaon, AtlIDTrack* Pion, 
				 HepVertex* Vertex, HepVertex* PrimaryVtx,
				 HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    AtlDstarDecayDPi* AddDstarDecayDPi(Float_t Px, Float_t Py, Float_t Pz, Float_t E,
				       AtlD0DecayKPi* D0, AtlIDTrack* Pion, 
				       HepVertex* Vertex, HepVertex* PrimaryVtx,
				       HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    //
    // Matching functions
    //
    TList* FindOverlaps_Particle_Jets(HepParticle *prt,
				      TCollection *search_list,
				      Float_t DeltaR = 0.1);
    TList* FindOverlaps_ElectronCluster_Jets(AtlElectron *el,
					     TCollection *search_list,
					     Float_t DeltaR = 0.1);
    TList* FindOverlaps_ElectronTrack_EMScaleJets(AtlElectron *el,
						  TCollection *search_list,
						  Float_t DeltaR = 0.1,
						  const char* option = "trkem");
    TList* FindOverlaps_MuonTrack_EMScaleJets(AtlMuon *mu,
					      TCollection *search_list,
					      Float_t DeltaR = 0.1);
    TList* FindMatchedParticles(HepParticle *prt,
				TCollection *search_list,
				Float_t DeltaR = 0.1,
				Float_t DeltaPtFrac = 0.1);
    TList* FindMatchedParticles(HepJet *jet,
				TCollection *search_list,
				Float_t DeltaR = 0.2,
				Float_t DeltaPtFrac = 0.2);
    HepMCParticle* FindMatchedMCParticle(HepParticle *prt,
					 Bool_t RemoveUnstable = kTRUE,
					 Float_t DeltaR = 0.1,
					 Float_t DeltaPtFrac = 0.1);
    HepMCParticle* FindMatchedMCParticle(HepJet *jet,
					 Bool_t RemoveUnstable = kTRUE,
					 Float_t DeltaR = 0.2,
					 Float_t DeltaPtFrac = 0.2);
    TList* FindTruthMatchedJets(AtlJet::EType type);
    TList* FindTruthMatchedJets(TCollection *search_list, AtlJet::EType type);
    TList* FindMatchedJets(HepParticle *prt,
			   TCollection *search_list,
			   Float_t DeltaR = 0.1,
			   Float_t DeltaPtFrac = 0.1,
			   Bool_t UseDeltaRonly = kFALSE);
    TList* FindMatchedJets(HepJet *jet,
			   TCollection *search_list,
			   Float_t DeltaR = 0.2,
			   Float_t DeltaEtFrac = 0.2);
    HepJet* FindMatchedMCJet(HepParticle *prt,
			     Float_t DeltaR = 0.2,
			     Float_t DeltaPtFrac = 0.2);
    HepJet* FindMatchedMCJet(HepJet *jet,
			     Float_t DeltaR = 0.2,
			     Float_t DeltaEtFrac = 0.2);
    
    AtlIDTrack* GetAssociatedIDTrack(HepMCParticle *prt) const;

    //
    // Print functions for truth information
    //
    void PrintMCParticles(const char* PrtType) const;
    void PrintMCDaughters(HepMCParticle *prt) const;
    void PrintMCMothers(HepMCParticle *prt) const;
    void PrintMCGenealogy(HepMCParticle *prt) const;
    void PrintMCGenealogyTree(Int_t First = 1, Int_t Last = 10000) const;
    inline void PrintMCDaughters(Int_t Id) const
	    { PrintMCDaughters(GetMCParticle_ById(Id)); }
    inline void PrintMCMothers(Int_t Id) const
	    { PrintMCMothers(GetMCParticle_ById(Id)); }
    inline void PrintMCGenealogy(Int_t Id) const
	    { PrintMCGenealogy(GetMCParticle_ById(Id)); }


    //
    // Generic event information
    //
    inline Bool_t    IsMC() const { return fEventHeader.IsMC(); }
    inline Bool_t    IsDATA() const { return fEventHeader.IsDATA(); }
    inline Float_t   BeamEnergy() const { return fEventHeader.BeamEnergy(); }
    inline Int_t     LumiBlock() const { return fEventHeader.LumiBlock(); }
    inline UInt_t    RunNr() const { return fEventHeader.RunNr(); }
    inline ULong64_t EventNr() const { return fEventHeader.EventNr(); }
    inline UInt_t    GetChannelNumber() const { return fEventHeader.GetChannelNumber(); }
    inline Float_t   GetHFORType() const { return fEventHeader.GetHFORType(); }
    inline Bool_t    IsRun1() const { return fEventHeader.IsRun1(); }
    inline Bool_t    IsRun2() const { return fEventHeader.IsRun2(); }
    
    //
    // Event shape
    //
    Float_t Sphericity(TList *Vectors);
    
    //
    // Event reconstruction
    //
    Float_t W_Mperp2(HepParticle *lepton, TVector2 MissingEt) const;
    void ComputePtRel(HepParticle *prt, TCollection *search_list,
		      Float_t DeltaRmax, Double_t &PtRel, Double_t &DeltaR);
    
    inline Float_t W_Mperp(HepParticle *lepton, TVector2 MissingEt) const {
	//
	// W boson transverse mass reconstruction for given
	// lepton and missing Et
	//
	return TMath::Sqrt(W_Mperp2(lepton, MissingEt));
    }
    inline Float_t W_Mperp(HepParticle *lepton) const {
	//
	// W boson transverse mass reconstruction for given
	// lepton and (commonly) reconstructed missing Et
	//
	return W_Mperp(lepton, fEnergySum.GetMissingEt());
    }
    inline Float_t W_Mperp2(HepParticle *lepton) const {
	//
	// W boson transverse mass reconstruction for given
	// lepton and (commonly) reconstructed missing Et
	//
	return W_Mperp2(lepton, fEnergySum.GetMissingEt());
    }

    
    //
    // Getter functions
    //

    inline Float_t GetPreTagEvtWeight() const { return fEventHeader.GetPreTagEvtWeight(); }
    inline Float_t GetTagEvtWeight() const { return fEventHeader.GetTagEvtWeight(); }
    inline Float_t GetMCWeight() const { return fEventHeader.GetMCWeight(); }
    inline Float_t GetPileupWeight() const { return fEventHeader.GetPileupWeight(); }
    inline Float_t GetZVtxWeight() const { return fEventHeader.GetZVtxWeight(); }
    inline Float_t GetMMWeightNominal() const { return fEventHeader.GetMMWeightNominal(); }
    inline const AtlEventHeader* GetEventHeader() const { return &fEventHeader; }
    AtlEventHeader * GetEventHeader() { return &fEventHeader; }
    inline const AtlEnergySum* GetEnergySum() const { return &fEnergySum; }
    AtlEnergySum * GetEnergySum() { return &fEnergySum; }

    // Truth information
    TList*         GetMCParticles(const char* PrtType) const;
    HepMCParticle* GetMCParticle_ById(Int_t Id) const;
    TList*         GetMCGenealogy(HepMCParticle *prt) const;
    TLorentzVector GetMCSumEnergy_NonInt() const;
    Float_t        GetMCSumEnergy_NonInt_Eta() const;
    Float_t        GetMCSumEnergy_NonInt_Phi() const;
    TList*         GetMCNeutrinos(Bool_t sort = kTRUE) const;
    HepMCParticle* GetLeadingMCNeutrino() const;
    TList*	   GetMCTops(Bool_t good = kTRUE) const;
    HepMCParticle* GetGoodMother(HepMCParticle* particle); // ***working?
    TList*         GetStableMCParticles() const;
    TList*         FindMCParticlesByName(const char* PdgName, Bool_t RemoveDouble = kFALSE);
    inline TClonesArray* GetMCParticles() const { return fMCParticles; }
    inline TClonesArray* GetMCVertices() const  { return fMCVertices; }
    inline Int_t GetN_MCParticles() const { return fN_MCParticles; }
    inline Int_t GetN_MCVertices() const { return fN_MCVertices; }
    
    inline Int_t GetN_MCCone4HadronJets() const { return fN_MCCone4HadronJets; }
    inline Int_t GetN_MCCone7HadronJets() const { return fN_MCCone7HadronJets; }
    inline Int_t GetN_MCAntiKt4HadronJets() const { return fN_MCAntiKt4HadronJets; }
    inline Int_t GetN_MCAntiKt6HadronJets() const { return fN_MCAntiKt6HadronJets; }
    inline Int_t GetN_MCAntiKt4HadronPileupJets() const { return fN_MCAntiKt4HadronPileupJets; }
    inline TClonesArray* GetMCCone4HadronJets() const { return fMCCone4HadronJets; }
    inline TClonesArray* GetMCCone7HadronJets() const { return fMCCone7HadronJets; }
    inline TClonesArray* GetMCAntiKt4HadronJets() const { return fMCAntiKt4HadronJets; }
    inline TClonesArray* GetMCAntiKt6HadronJets() const { return fMCAntiKt6HadronJets; }
    inline TClonesArray* GetMCAntiKt4HadronPileupJets() const { return fMCAntiKt4HadronPileupJets; }
    inline AtlJet* GetLeadingMCCone4HadronJet() const
    { return GetLeadingJet(AtlJet::kMCCone4Hadron); }

    // Reconstruction information
    AtlElectron*   GetElectron_ById(Int_t Id) const;
    AtlMuon*       GetMuon_ById(Int_t Id) const;
    AtlJet*        GetLeadingJet(AtlJet::EType type) const;
    Int_t          GetN_Jets(AtlJet::EType type) const;
    TClonesArray*  GetJets(AtlJet::EType type) const;
    TList* GetJets(AtlJet::EType type, Float_t Et_min,
		   Float_t Et_max = 10e10, Float_t Eta_min = -10e10,
		   Float_t Eta_max = 10e10, Bool_t is_good = kFALSE,
		   Bool_t sort = kTRUE, Bool_t remove_faked = kTRUE);

    TList* GetElectrons(AtlEMShower::EAuthor author, AtlEMShower::EIsEM IsEM,
			Float_t Pt_min = 0., Float_t Pt_max = 10e10,
			Float_t Eta_min = -10e10, Float_t Eta_max = 10e10,
			Float_t EtCone20_max = 10e10, Bool_t sort = kTRUE,
			Bool_t exclude_crack = kTRUE, Float_t EtCone20_IsoFactor = 0.,
			Bool_t use_cluster_eta = kFALSE,
			AtlEMShower::ECaloIsoCorrection CaloIsoCorrection = AtlEMShower::kUncorrected);

    TList* GetVtxTracks(Float_t Chi2ovNDoF_max = 1000,
			Float_t Pt_min = 0., Float_t Pt_max = 10e10,
			Float_t Eta_min = -10e10, Float_t Eta_max = 10e10, Bool_t sort=kTRUE);
    
    TList* GetSecVtxTracks(Float_t Chi2ovDoF_max = 1000,
			   Float_t Pt_min = 0., Float_t Pt_max = 10e10,
			   Float_t Eta_min = -10e10, Float_t Eta_max = 10e10, Bool_t sort=kTRUE);
    
    TList* GetPrimVtxTracks(Float_t Chi2ovDoF_max = 10e10,
			   Float_t Pt_min = 0., Float_t Pt_max = 10e10,
			   Float_t Eta_min = -10e10, Float_t Eta_max = 10e10, Bool_t sort=kTRUE);

    TList* GetPhotons(AtlEMShower::EAuthor author, AtlEMShower::EIsEM IsEM,
		      Float_t Pt_min = 0., Float_t Pt_max = 10e10,
		      Float_t Eta_min = -10e10, Float_t Eta_max = 10e10,
		      Float_t EtCone20_max = 10e10, Bool_t sort = kTRUE,
		      Bool_t exclude_crack = kTRUE,
		      AtlEMShower::ECaloIsoCorrection CaloIsoCorrection = AtlEMShower::kUncorrected);
    TList* GetMuons(AtlMuon::EAuthor author, Float_t Pt_min = 0.,
		    Float_t Pt_max = 10e10,	Float_t Eta_min = -10e10,
		    Float_t Eta_max = 10e10, Float_t Chi2_max = 10e10,
		    Float_t EtCone20_max = 10e10, Bool_t staco_combined = kFALSE,
		    Bool_t sort = kTRUE, AtlMuon::EQuality quality = AtlMuon::EQuality( AtlMuon::kLoose | AtlMuon::kMedium | AtlMuon::kTight ));
    TList* GetTaus(AtlTau::EAuthor author, AtlTau::ETauFlag flag,
		   Float_t Pt_min = 0., Float_t Pt_max = 10e10,
		   Float_t Eta_min = -10e10, Float_t Eta_max = 10e10,
		   Bool_t sort = kTRUE);
    TList* GetVertices(HepVertex::EType type) const;
    Int_t GetN_PrimaryVertices() const;
    HepVertex* GetPrimaryVtx() const;

    // bdt response 
    Float_t GetBDTResponse() const;

    inline Int_t GetN_Cone4H1TowerJets()  const { return fN_Cone4H1TowerJets;  }
    inline Int_t GetN_Cone7H1TowerJets()  const { return fN_Cone7H1TowerJets;  }
    inline Int_t GetN_Cone4H1TopoJets()   const { return fN_Cone4H1TopoJets;   }
    inline Int_t GetN_Cone7H1TopoJets()   const { return fN_Cone7H1TopoJets;   }  
    inline Int_t GetN_AntiKt4H1TopoJets() const { return fN_AntiKt4H1TopoJets; }
    inline Int_t GetN_AntiKt4H1TowerJets() const { return fN_AntiKt4H1TowerJets; }
    inline Int_t GetN_AntiKt6H1TowerJet() const { return fN_AntiKt6H1TowerJets; }
    inline Int_t GetN_AntiKt4TopoEMJets() const { return fN_AntiKt4TopoEMJets; }
    inline Int_t GetN_AntiKt4TopoEMJESJets() const { return fN_AntiKt4TopoEMJESJets; }
    inline Int_t GetN_AntiKt4LCTopoJets() const { return fN_AntiKt4LCTopoJets; }
    inline Int_t GetN_AntiKt4TowerJets() const { return fN_AntiKt4TowerJets; }
    inline Int_t GetN_AntiKt6TopoEMJets() const { return fN_AntiKt6TopoEMJets; }
    inline Int_t GetN_AntiKt4TopoJets() const { return fN_AntiKt6TowerJets; }
    inline Int_t GetN_AntiKt6TowerJets() const { return fN_AntiKt6TowerJets; }
    inline Int_t GetN_AntiKt6LCTopoJets() const { return fN_AntiKt6LCTopoJets; }
    inline Int_t GetN_AntiKt6TopoJets() const { return fN_AntiKt6TopoJets; }
    inline Int_t GetN_AtlFastJets()       const { return fN_AtlFastJets;       }
    inline Int_t GetN_IDTracks() const { return fN_IDTracks; }
    inline Int_t GetN_TRTHits() const { return fN_TRTHits; }
    inline Int_t GetN_PixelHits() const { return fN_PixelHits; }
    inline Int_t GetN_SCTHits() const { return fN_SCTHits; }
    inline Int_t GetN_SiliconHits() const {return (fN_PixelHits + fN_SCTHits); }
    inline Int_t GetN_Electrons() const { return fN_Electrons; }
    inline Int_t GetN_Muons() const { return fN_Muons; }
    inline Int_t GetN_Taus() const { return fN_Taus; }
    inline Int_t GetN_Photons() const { return fN_Photons; }
    inline Int_t GetN_AtlFastElectrons() const { return fN_AtlFastElectrons; }
    inline Int_t GetN_AtlFastMuons() const { return fN_AtlFastMuons; }
    inline Int_t GetN_AtlFastTaus() const { return fN_AtlFastTaus; }
    inline Int_t GetN_AtlFastPhotons() const { return fN_AtlFastPhotons; }
    inline Int_t GetN_TopPairs() const { return fN_TopPairs; }
    inline Int_t GetN_TopDecays() const { return fN_TopDecays; }
    inline Int_t GetN_WDecaysLNu() const { return fN_WDecaysLNu; }
    inline Int_t GetN_WDecaysJJ() const { return fN_WDecaysJJ; }
    inline Int_t GetN_Neutrinos() const { return fN_Neutrinos; }
    inline Int_t GetN_Z0Decays() const { return fN_Z0Decays; }
    inline Int_t GetN_PhiDecaysKK() const { return fN_PhiDecaysKK; }
    inline Int_t GetN_K0sDecaysPiPi() const { return fN_K0sDecaysPiPi; }
    inline Int_t GetN_PhotonConv() const { return fN_PhotonConv; }
    inline Int_t GetN_LambdaDecaysPiPi() const { return fN_LambdaDecaysPiPi; }
    inline Int_t GetN_D0DecaysKPi() const { return fN_D0DecaysKPi; }
    inline Int_t GetN_DstarDecaysDPi() const { return fN_DstarDecaysDPi; }
    inline Int_t GetN_Vertices() const { return fN_Vertices; }
    inline TClonesArray* GetCone4H1TowerJets()  const { return fCone4H1TowerJets;  }
    inline TClonesArray* GetCone7H1TowerJets()  const { return fCone7H1TowerJets;  }
    inline TClonesArray* GetCone4H1TopoJets()   const { return fCone4H1TopoJets;   }
    inline TClonesArray* GetCone7H1TopoJets()   const { return fCone7H1TopoJets;   }
    inline TClonesArray* GetAntiKt4H1TopoJets() const { return fAntiKt4H1TopoJets; }
    inline TClonesArray* GetAntiKt4H1TowerJets() const { return fAntiKt4H1TowerJets; }
    inline TClonesArray* GetAntiKt6H1TowerJets() const { return fAntiKt6H1TowerJets; }
    inline TClonesArray* GetAntiKt4TopoEMJets() const { return fAntiKt4TopoEMJets; }
    inline TClonesArray* GetAntiKt4TopoEMJESJets() const { return fAntiKt4TopoEMJESJets; }
    inline TClonesArray* GetAntiKt4LCTopoJets() const { return fAntiKt4LCTopoJets; }
    inline TClonesArray* GetAntiKt4TowerJets() const { return fAntiKt4TowerJets; }
    inline TClonesArray* GetAntiKt6TopoEMJets() const { return fAntiKt6TopoEMJets; }
    inline TClonesArray* GetAntiKt4TopoJets() const { return fAntiKt4TopoJets; }
    inline TClonesArray* GetAntiKt6TowerJets() const { return fAntiKt6TowerJets; }
    inline TClonesArray* GetAntiKt6LCTopoJets() const { return fAntiKt6LCTopoJets; }
    inline TClonesArray* GetAntiKt6TopoJets() const { return fAntiKt6TopoJets; }
    inline TClonesArray* GetDL1rJets() const { return fDL1rJets; }
    inline TClonesArray* GetAtlFastJets()       const { return fAtlFastJets;       }
    inline TClonesArray* GetIDTracks() const { return fIDTracks; }
    inline TClonesArray* GetTRTHits() const { return fTRTHits; }
    inline TClonesArray* GetElectrons() const { return fElectrons; }
    inline TClonesArray* GetMuons() const { return fMuons; }
    inline TClonesArray* GetTaus() const { return fTaus; }
    inline TClonesArray* GetPhotons() const { return fPhotons; }
    inline TClonesArray* GetAtlFastElectrons() const { return fAtlFastElectrons; }
    inline TClonesArray* GetAtlFastMuons() const { return fAtlFastMuons; }
    inline TClonesArray* GetAtlFastTaus() const { return fAtlFastTaus; }
    inline TClonesArray* GetAtlFastPhotons() const { return fAtlFastPhotons; }
    inline TClonesArray* GetTopPairs() const {return fTopPairs; }
    inline TClonesArray* GetTopDecays() const {return fTopDecays; }
    inline TClonesArray* GetWDecaysLNu() const {return fWDecaysLNu; }
    inline TClonesArray* GetWDecaysJJ() const {return fWDecaysJJ; }
    inline TClonesArray* GetNeutrinos() const {return fNeutrinos; }
    inline TClonesArray* GetZ0Decays() const {return fZ0Decays; }
    inline TClonesArray* GetPhiDecaysKK() const {return fPhiDecaysKK; }
    inline TClonesArray* GetK0sDecaysPiPi() const {return fK0sDecaysPiPi; }
    inline TClonesArray* GetPhotonConv() const {return fPhotonConv; }
    inline TClonesArray* GetLambdaDecaysPiPi() const { return fLambdaDecaysPiPi; }
    inline TClonesArray* GetD0DecaysKPi() const { return fD0DecaysKPi; }
    inline TClonesArray* GetDstarDecaysDPi() const { return fDstarDecaysDPi; }
    inline TClonesArray* GetVertices() const { return fVertices; }

    //
    // Setter functions
    //
    inline void SetBDTResponse ( Float_t bdt_scale ) { fbdt_scale = bdt_scale; }

    inline void SetN_PrimaryVertices( Int_t vpx_n ) { fN_vpx = vpx_n; }
    void SetN_Jets(AtlJet::EType type, Int_t NJets);
    inline void SetN_TRTHits(Int_t N_TRTHits) { fN_TRTHits = N_TRTHits; }
    inline void SetN_PixelHits(Int_t N_PixelHits) { fN_PixelHits = N_PixelHits; }
    inline void SetN_SCTHits(Int_t N_SCTHits) { fN_SCTHits = N_SCTHits; }
    inline void SetEventHeader(UInt_t RunNr, ULong64_t EventNr,
			       UInt_t LumiBlock, Float_t BeamEnergy,
			       Bool_t IsMC,
			       AtlEventHeader::EType EventType,
			       Float_t MCWeight, 
			       Float_t PileupWeight,
			       Float_t ZVertexWeight,
			       Float_t MMWeight_nominal,
			       Float_t MMWeight_fake_mc_up,
			       Float_t MMWeight_fake_mc_down,
			       Float_t MMWeight_fake_alternate,
			       Float_t MMWeight_real_alternate,
			       Double_t TruthWeightCorrection) {
	fEventHeader.SetRunNr(RunNr);
	fEventHeader.SetEventNr(EventNr);
	fEventHeader.SetLumiBlock(LumiBlock);
	fEventHeader.SetBeamEnergy(BeamEnergy);
	fEventHeader.SetMC(IsMC);
	fEventHeader.SetEventType(EventType);
	fEventHeader.SetPreTagEvtWeight(MCWeight*PileupWeight*ZVertexWeight*TruthWeightCorrection);
	fEventHeader.SetTagEvtWeight(MCWeight*PileupWeight*ZVertexWeight*TruthWeightCorrection);
	fEventHeader.SetMCWeight(MCWeight);
	fEventHeader.SetPileupWeight(PileupWeight);
	fEventHeader.SetZVtxWeight(ZVertexWeight);
	fEventHeader.SetMMWeightNominal(MMWeight_nominal);
	fEventHeader.SetMMWeightFakeMCup(MMWeight_fake_mc_up);
	fEventHeader.SetMMWeightFakeMCdown(MMWeight_fake_mc_down);
	fEventHeader.SetMMWeightFakeAlternate(MMWeight_fake_alternate);
	fEventHeader.SetMMWeightRealAlternate(MMWeight_real_alternate);
	fEventHeader.SetTruthWeightCorrection(TruthWeightCorrection);
    }
    inline void SetL1TriggerItem(Int_t Bit, Bool_t Passed,
				 Float_t Prescale, Float_t PassThrough) {
	// Set level 1 trigger item for given bit number
	fTrigger->SetL1Item(Bit, Passed, Prescale, PassThrough);
    }
    inline void SetHLTTriggerItem(Int_t Bit, Bool_t Passed,
				  Float_t Prescale, Float_t PassThrough) {
	// Set higher level trigger item for given bit number
	fTrigger->SetHLTItem(Bit, Passed, Prescale, PassThrough);
    }
    inline void SetPreTagEvtWeight(Float_t Weight)
    { fEventHeader.SetPreTagEvtWeight(Weight); }
    inline void SetTagEvtWeight(Float_t Weight)
    { fEventHeader.SetTagEvtWeight(Weight); }
    
    //
    // Trigger information
    //
    void PrintTriggerMatches(Option_t *option = "ALL") const;
    void PrintTriggerMatches(AtlJet::EType type) const;
    inline AtlTrigger* GetTrigger() const { return fTrigger; }
    inline Bool_t HasPassedL1(const char* L1ItemName) {
	// Has passed given 1st level trigger ?
	return GetTrigger()->HasPassedL1(L1ItemName, RunNr(), kTRUE);
    }
    inline Bool_t HasPassedHLT(const char* HLTItemName) {
	// Has passed given higher level trigger ?
	return GetTrigger()->HasPassedHLT(HLTItemName, RunNr(), kTRUE);
    }
    inline Bool_t HasMatchedL1(AtlTriggerMatch *RecoObject, const char* L1ItemName) {
	//
	// Test trigger matching of reconstructed object and given L1
	// trigger item.
	//
	// Allowed reconstructed objects are of type AtlElectron,
	// AtlPhoton, AtlMuon, AtlTau and AtlJet.
	//
	return RecoObject->HasMatchedL1(L1ItemName,
					RunNr(),
					GetTrigger());
    }
    inline Bool_t HasMatchedHLT(AtlTriggerMatch *RecoObject, const char* HLTItemName) {
	//
	// Test trigger matching of reconstructed object and given HLT
	// trigger item.
	//
	// Allowed reconstructed objects are of type AtlElectron,
	// AtlPhoton, AtlMuon, AtlTau and AtlJet.
	//
	return RecoObject->HasMatchedHLT(HLTItemName,
					 RunNr(),
					 GetTrigger());
    }

    // SgTop D3PD variables
    inline void SetIsEleMuOverlap(Bool_t isEleMuOverlap) {
	fIsEleMuOverlap = isEleMuOverlap;
    }

    inline Bool_t IsEleMuOverlap() const { return fIsEleMuOverlap; }
    
  private:
    
    void PrintMCGenealogyTree(HepMCParticle *prt, TString *padding,
                              TList *CheckList = 0) const;
    void PrintTriggerMatches(AtlTriggerMatch *obj) const;
    
    ClassDef(AtlEvent,67) // ATLAS event class
};

#endif
 
