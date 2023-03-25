//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEMShower
#define ATLAS_AtlEMShower
#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifndef HEP_HepJet
#include <HepJet.h>
#endif
#include <TString.h>
#ifndef ATLAS_AtlMETWeights
#include <AtlMETWeights.h>
#endif

class TSystem;

class AtlEMShower: public AtlMETWeights {

  public:
    enum EIsEM {
	kInvalid           = BIT(0),  // Invalid selection
	kElectronLoose     = BIT(1),  // Loose electron selection
	kElectronFwdLoose  = BIT(2),  // Loose selection for forwrd electrons
	kElectronMedium    = BIT(3),  // Medium electron selection
	kElectronTight     = BIT(4),  // Tight electron selection
	kElectronFwdTight  = BIT(5),  // Tight selection for forwrd electrons
	kElectronTightIso  = BIT(6),  // Tight + isolation selection for electrons
	kPhotonLoose       = BIT(7),  // Loose photon selection
	kPhotonTight       = BIT(8),  // Tight photon selection
	kPhotonTightIso    = BIT(9),  // Tight + isolation selection for photons
	kElectronMediumIso = BIT(10), // Medium + isolation selection for electrons
	kElectronMediumPP  = BIT(11), // Medium++ electron selection
	kElectronTightPP   = BIT(12), // Tight++ electron selection
	kElectron90EtCone20   = BIT(13), // EtCone20@90% electron selection
	kElectron90PtCone30   = BIT(14), // PtCone30@90% electron selection
	
	kIsEM_Electron = ( kElectronLoose | kElectronFwdLoose |
			   kElectronMedium | kElectronTight |
			   kElectronFwdTight | kElectronTightIso |
			   kElectronMediumIso |
			   kElectronMediumPP | kElectronTightPP ),
	kIsEM_Photon = ( kPhotonLoose | kPhotonTight |
			 kPhotonTightIso )
    };
    static constexpr Int_t fgNIsEM = 13;
    static const char* fgIsEMNames[];

    //
    // Source code of IsEM flags: see files quoted here
    // http://alxr.usatlas.bnl.gov/lxr-stb5/source/atlas/Reconstruction/egamma/egammaEvent/egammaEvent/egammaPIDdefs.h

    enum EAuthor {
	kInvalidAuthor   = BIT(0), // Invalid or unknown author
	kHighPtElectron  = BIT(1), // Standard cluster-based electron algorithm
	kSoftElectron    = BIT(2), // Track-based electron algorithm
	kForwardElectron = BIT(3), // Forward cluster-based electron algorithm
	kPhoton          = BIT(4), // Standard cluster-based photon algorithm
	kConvPhoton      = BIT(5), // Recovered conversion photon
	kAuthor_Electron = ( kHighPtElectron | kSoftElectron |
			     kForwardElectron ),
	kAuthor_Photon = ( kPhoton | kConvPhoton )
    };

    //
    // Doxygen documentation of meaning of egamma authors
    // and all other egamma parameters:
    //
    // http://atlas-computing.web.cern.ch/atlas-computing/links/nightlyDocDirectory/egammaEvent/html/namespaceegammaParameters.html
    
    static constexpr Int_t fgNAuthors = 6;
    static const char* fgAuthorNames[];

    
    enum ECaloIsoCorrection {
        kUncorrected     = BIT(0),     // EtCone variables with no correction applied
	kPtCorrected     = BIT(1),     // EtCone variables with Pt leakage correction applied
	kEDCorrected     = BIT(2),     // EtCone variables with ambient energy density pileup correction applied
	kNPVCorrected    = BIT(3),     // EtCone variables with nPV pileup correction applied
	kPtEDCorrected   = BIT(4),     // EtCone variables with Pt leakage and ambient energy 
	                               // density pileup correction applied
	kPtNPVCorrected  = BIT(5)      // EtCone variables with Pt leakage and nPV pileup 
	                               // correction applied
	
    };

  protected:
    EAuthor fAuthor;       // Reconstruction algorithm(s) used
    Float_t fEtCone20;     // Transverse energy inside a cone of radius dR=0.2
                           // around the electron cluster (GeV) (Uncorrected)
    Float_t fEtCone30;     // Transverse energy inside a cone of radius dR=0.3
                           // around the electron cluster (GeV) (Uncorrected)
    Float_t fEtCone40;     // Transverse energy inside a cone of radius dR=0.4
                           // around the electron cluster (GeV) (Uncorrected)
    Float_t fEtCone45;     // Transverse energy inside a cone of radius dR=0.45
                           // around the electron cluster (GeV) (Uncorrected)
    Float_t fEtCone20_pt_corrected;            // EtCone variable with Pt leakage correction applied
    Float_t fEtCone30_pt_corrected;            // EtCone variable with Pt leakage correction applied
    Float_t fEtCone40_pt_corrected;            // EtCone variable with Pt leakage correction applied
    Float_t fEtCone20_pt_corrected_error;      // error on EtCone variable with Pt leakage correction applied
    Float_t fEtCone30_pt_corrected_error;      // error on EtCone variable with Pt leakage correction applied
    Float_t fEtCone40_pt_corrected_error;      // error on EtCone variable with Pt leakage correction applied
    Float_t fEtCone20_ED_corrected;            // EtCone variable with ambient energy density pileup correction applied
    Float_t fEtCone30_ED_corrected;            // EtCone variable with ambient energy density pileup correction applied
    Float_t fEtCone40_ED_corrected;            // EtCone variable with ambient energy density pileup correction applied
    Float_t fEtCone20_NPV_corrected;           // EtCone variable with nPV pileup correction applied
    Float_t fEtCone30_NPV_corrected;           // EtCone variable with nPV pileup correction applied
    Float_t fEtCone40_NPV_corrected;           // EtCone variable with nPV pileup correction applied
    Float_t fEtCone20_corrected;               // EtCone variable with Pt leakage and ambient energy 
	                                       // density pileup correction applied
    Float_t fEtCone30_corrected;               // EtCone variable with Pt leakage and ambient energy 
	                                       // density pileup correction applied
    Float_t fEtCone40_corrected;               // EtCone variable with Pt leakage and ambient energy 
	                                       // density pileup correction applied
    Float_t fEtCone20_PtNPV_corrected;         // EtCone variable with Pt leakage and nPV pileup 
                                               // correction applied
    Float_t fEtCone30_PtNPV_corrected;         // EtCone variable with Pt leakage and nPV pileup 
                                               // correction applied
    Float_t fEtCone40_PtNPV_corrected;         // EtCone variable with Pt leakage and nPV pileup 
                                               // correction applied
    UInt_t  fIsEMBitField; // Complete IsEM bit pattern provided by the e/gamma EDM
    EIsEM   fIsEM;         // Result of loose, medium, tight, ... selection(s)
    Float_t fEMWeight;     // Weight for EM (electron/photon) hypothesis
    Float_t fBkgWeight;    // Weight for background (pion) hypothesis
    UInt_t  fOQFlag;     // Object quality flag from Athena (bit-word)
    // for more information go to:
    // http://alxr.usatlas.bnl.gov/lxr/source/atlas/PhysicsAnalysis/SUSYPhys/SUSYTools/SUSYTools/egammaPIDdefs.h
    Float_t        fPtCone20;  // Summed Pt (GeV) of tracks in a cone with half-opening angle 0.2
    Float_t        fPtCone30;  // Summed Pt (GeV) of tracks in a cone with half-opening angle 0.3
    Float_t        fPtCone40;  // Summed Pt (GeV) of tracks in a cone with half-opening angle 0.4
    TLorentzVector fPCluster;  // 4-momentum (Px, Py, Pz, E) in GeV of Cluster

    //
    // Documentation on different calo isolation corrections:
    //
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/CaloIsolationCorrections
    
 public:
    AtlEMShower();
    AtlEMShower(EAuthor Author, 
		UInt_t IsEMBitField,
		EIsEM IsEM, Float_t EMWeight, Float_t BkgWeight,
		UInt_t OQFlag, 
		TLorentzVector PCluster);
    virtual ~AtlEMShower();
    void Clear(Option_t *option = "");
    static TString* NameOfAuthor(EAuthor eg_Author);
    static TString* NameOfIsEM(EIsEM eg_IsEM);
    TString* GetAuthorName();
    TString* GetIsEMName();

    virtual Bool_t IsEM_ClusterEtaRange()         const = 0;
    virtual Bool_t IsEM_ClusterHadronicLeakage()  const = 0;
    virtual Bool_t IsEM_ClusterMiddleEnergy()     const = 0;
    virtual Bool_t IsEM_ClusterMiddleEratio37()   const = 0;
    virtual Bool_t IsEM_ClusterMiddleEratio33()   const = 0;
    virtual Bool_t IsEM_ClusterMiddleWidth()      const = 0;
    virtual Bool_t IsEM_ClusterStripsEratio()     const = 0;
    virtual Bool_t IsEM_ClusterStripsDeltaEmax2() const = 0;
    virtual Bool_t IsEM_ClusterStripsDeltaE()     const = 0;
    virtual Bool_t IsEM_ClusterStripsWtot()       const = 0;
    virtual Bool_t IsEM_ClusterStripsFracm()      const = 0;
    virtual Bool_t IsEM_ClusterStripsWeta1c()     const = 0;
    virtual Bool_t IsEM_ClusterStripsDEmaxs1()    const = 0;
    virtual Bool_t IsEM_TrackMatchEoverP()        const = 0;
    virtual Bool_t IsEM_Isolation()               const = 0;
    virtual Bool_t IsEM_ClusterIsolation()        const = 0;
    virtual Bool_t IsEM_TrackIsolation()          const = 0;
    
    inline Bool_t IsEM(EIsEM IsEM) { return fIsEM & IsEM; }
    inline Bool_t HasInvalidIsEM() const { return fIsEM & kInvalid; }
    inline Bool_t HasInvalidAuthor() const { return fAuthor & kInvalidAuthor; }
    inline Bool_t HasAuthor(EAuthor author) const { return fAuthor & author; }
    inline EAuthor GetAuthor() const { return fAuthor; }
    inline EIsEM GetIsEM() const { return fIsEM; }
    inline UInt_t GetIsEMBitField() { return fIsEMBitField; }
    //inline Float_t GetEtCone20()  { return fEtCone20; }
    inline Float_t GetEtCone20(ECaloIsoCorrection CaloIsoCorr = kUncorrected) {
        if(CaloIsoCorr & kUncorrected) return fEtCone20;
	else if(CaloIsoCorr & kPtCorrected) return fEtCone20_pt_corrected;
	else if(CaloIsoCorr & kEDCorrected) return fEtCone20_ED_corrected;
	else if(CaloIsoCorr & kNPVCorrected) return fEtCone20_NPV_corrected;
	else if(CaloIsoCorr & kPtEDCorrected) return fEtCone20_corrected;
	else if(CaloIsoCorr & kPtNPVCorrected) return fEtCone20_PtNPV_corrected;
	else return 0;
    }
    inline Float_t GetEtCone30(ECaloIsoCorrection CaloIsoCorr = kUncorrected)  { 
        if(CaloIsoCorr & kUncorrected) return fEtCone30;
	else if(CaloIsoCorr & kPtCorrected) return fEtCone30_pt_corrected;
	else if(CaloIsoCorr & kEDCorrected) return fEtCone30_ED_corrected;
	else if(CaloIsoCorr & kNPVCorrected) return fEtCone30_NPV_corrected;
	else if(CaloIsoCorr & kPtEDCorrected) return fEtCone30_corrected;
	else if(CaloIsoCorr & kPtNPVCorrected) return fEtCone30_PtNPV_corrected;
	else return 0;
    }
    inline Float_t GetEtCone40(ECaloIsoCorrection CaloIsoCorr = kUncorrected)  { 
        if(CaloIsoCorr & kUncorrected) return fEtCone40;
	else if(CaloIsoCorr & kPtCorrected) return fEtCone40_pt_corrected;
	else if(CaloIsoCorr & kEDCorrected) return fEtCone40_ED_corrected;
	else if(CaloIsoCorr & kNPVCorrected) return fEtCone40_NPV_corrected;
	else if(CaloIsoCorr & kPtEDCorrected) return fEtCone40_corrected;
	else if(CaloIsoCorr & kPtNPVCorrected) return fEtCone40_PtNPV_corrected;
	else return 0;
    }
    inline Float_t GetEtCone45()  { return fEtCone45; }
    inline Float_t GetEtCone20_pt_corrected_error() { return fEtCone20_pt_corrected_error; }
    inline Float_t GetEtCone30_pt_corrected_error() { return fEtCone30_pt_corrected_error; }
    inline Float_t GetEtCone40_pt_corrected_error() { return fEtCone40_pt_corrected_error; }
    inline Float_t GetPtCone20() const { return fPtCone20; }
    inline Float_t GetPtCone30() const { return fPtCone30; }
    inline Float_t GetPtCone40() const { return fPtCone40; }
    inline Float_t GetEMWeight()  { return fEMWeight; }
    inline Float_t GetBkgWeight() { return fBkgWeight; }
    inline UInt_t  GetOQFlag()    { return fOQFlag; }
    inline Bool_t IsElectronTight() { return fIsEM & kElectronTight; }
    inline void SetPtConeVars(Float_t PtCone20, Float_t PtCone30, Float_t PtCone40) {
	fPtCone20 = PtCone20;
	fPtCone30 = PtCone30;
	fPtCone40 = PtCone40;
    }
    inline void SetEtConeVars( Float_t EtCone20, Float_t EtCone30, Float_t EtCone40, Float_t EtCone45, 
			       Float_t EtCone20_pt_corrected, Float_t EtCone30_pt_corrected, 
			       Float_t EtCone40_pt_corrected, 
			       Float_t EtCone20_pt_corrected_error, Float_t EtCone30_pt_corrected_error, 
			       Float_t EtCone40_pt_corrected_error, 
			       Float_t EtCone20_ED_corrected, Float_t EtCone30_ED_corrected, 
			       Float_t EtCone40_ED_corrected, 
			       Float_t EtCone20_NPV_corrected, Float_t EtCone30_NPV_corrected, 
			       Float_t EtCone40_NPV_corrected, 
			       Float_t EtCone20_corrected, Float_t EtCone30_corrected, 
			       Float_t EtCone40_corrected,
			       Float_t EtCone20_PtNPV_corrected, Float_t EtCone30_PtNPV_corrected, 
			       Float_t EtCone40_PtNPV_corrected) {
      fEtCone20 = EtCone20; fEtCone30 = EtCone30; fEtCone40 = EtCone40; fEtCone45 = EtCone45; 
      fEtCone20_pt_corrected = EtCone20_pt_corrected; fEtCone30_pt_corrected = EtCone30_pt_corrected;
      fEtCone40_pt_corrected = EtCone40_pt_corrected; 
      fEtCone20_pt_corrected_error = EtCone20_pt_corrected_error;
      fEtCone30_pt_corrected_error = EtCone30_pt_corrected_error; fEtCone40_pt_corrected_error = EtCone40_pt_corrected_error;
      fEtCone20_ED_corrected = EtCone20_ED_corrected; fEtCone30_ED_corrected = EtCone30_ED_corrected;
      fEtCone40_ED_corrected = EtCone40_ED_corrected; 
      fEtCone20_NPV_corrected = EtCone20_NPV_corrected; fEtCone30_NPV_corrected = EtCone30_NPV_corrected; 
      fEtCone40_NPV_corrected = EtCone40_NPV_corrected;
      fEtCone20_corrected = EtCone20_corrected; fEtCone30_corrected = EtCone30_corrected;
      fEtCone40_corrected = EtCone40_corrected;     
      fEtCone20_PtNPV_corrected = EtCone20_PtNPV_corrected; fEtCone30_PtNPV_corrected = EtCone30_PtNPV_corrected;
      fEtCone40_PtNPV_corrected = EtCone40_PtNPV_corrected;
    }
    //
    // Cluster kinematics
    //
    inline const TLorentzVector& ClusterP() const {
	// Return cluster 4-momentum vector (GeV)
	return fPCluster;
    }
    inline Float_t ClusterPt() {
	// Return transvere momentum (GeV)
	return fPCluster.Perp();
    }
    inline Float_t ClusterPx() const {
	// Return momentum x component (GeV)
	return fPCluster.Px();
    }
    inline Float_t ClusterPy() const {
	// Return momentum y component (GeV)
	return fPCluster.Py();
    }
    inline Float_t ClusterPz() const {
	// Return momentum z component (GeV)
	return fPCluster.Pz();
    }
    inline Float_t ClusterEt() const {
	// Return transverse energy (GeV)
	return fPCluster.Et();
    }
    inline Float_t ClusterE() const {
	// Return energy (GeV)
	return fPCluster.E();
    }
    inline Float_t ClusterEta() {
	// Return pseudo-rapidity
	Float_t eta = 0.;
	if ( fPCluster.CosTheta()*fPCluster.CosTheta() < 1. ) {
	    eta = fPCluster.Eta();
	} else {
	    eta = ( fPCluster.Pz() > 0. ) ? 10e10 : -10e10;
	}
	return eta;
    }
    
    inline Float_t ClusterPhi() {
	// Return azimuth (rad)
	return fPCluster.Phi();
    }
    Float_t ClusterDeltaR(HepJet *jet);
    
    inline void SetClusterE(Double_t E){
	// Set cluster energy (GeV)
	fPCluster.SetE(E);
    }
    
    ClassDef(AtlEMShower,25) // Atlas EM shower properties
};
#endif

