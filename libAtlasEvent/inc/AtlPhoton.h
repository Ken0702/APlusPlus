//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlPhoton
#define ATLAS_AtlPhoton
#ifndef HEP_HepPhoton
#include <HepPhoton.h>
#endif
#ifndef ATLAS_AtlEMShower
#include <AtlEMShower.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif

class AtlPhoton : public HepPhoton, public AtlEMShower,
    public AtlTriggerMatch {
  
 private:
  
 public:
    AtlPhoton();
    AtlPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	      Float_t EMWeight, Float_t BkgWeight,
	      UInt_t OQFlag,
	      UInt_t IsEMBitField, EIsEM IsEM, EAuthor Author,
	      TLorentzVector PCluster);
    virtual ~AtlPhoton();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    inline Bool_t IsPhoton()     const { return fAuthor & kPhoton;}  
    inline Bool_t IsConvPhoton() const { return fAuthor & kConvPhoton;}  

    inline Bool_t IsLoose() const {
	// Loose photon selection
	return fIsEM & kPhotonLoose;
    }
    inline Bool_t IsTight() const {
	// Loose photon selection
	return fIsEM & kPhotonTight;
    }


    // Get OQ decision
    UInt_t  IsGoodOQ(); // 0: is good OQ, 1: is bad OQ
    
    
    inline virtual Bool_t IsEM_ClusterEtaRange() const {
	// Cluster eta range
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<1);
    }
    inline virtual Bool_t IsEM_ClusterHadronicLeakage_Loose() const {
	// Cluster leakage into the hadronic calorimeter
	// for loose selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<2);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEnergy_Loose() const {
	// Energy in 2nd sampling (e277)
	// for loose selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<3);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio37_Loose() const {
	// Energy ratio in 2nd sampling
	// for loose selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<4);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio33_Loose() const {
	// nergy ratio in 2nd sampling
	// for loose selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<5);
    }
    inline virtual Bool_t IsEM_ClusterMiddleWidth_Loose() const {
	// Width in the second sampling
	// for loose selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<6);
    }
    inline virtual Bool_t IsEM_ClusterHadronicLeakage() const {
	// Cluster leakage into the hadronic calorimeter
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<10);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEnergy() const {
	// Energy in 2nd sampling (e277)
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<11);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio37() const {
	// Energy ratio in 2nd sampling
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<12);
    }
    inline virtual Bool_t IsEM_ClusterMiddleEratio33() const {
	// Energy ratio in 2nd sampling for photons
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<13);
    }
    inline virtual Bool_t IsEM_ClusterMiddleWidth() const {
	// Width in the second sampling
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<14);
    }
    inline virtual Bool_t IsEM_ClusterStripsEratio() const {
	// Fraction of energy found in 1st sampling 
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<15);
    }
    inline virtual Bool_t IsEM_ClusterStripsDeltaEmax2() const {
	// Energy of 2nd maximum in 1st sampling ~e2tsts1/(1000+const_lumi*et)
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<16);
    }
    inline virtual Bool_t IsEM_ClusterStripsDeltaE() const {
	// Difference between 2nd maximum and 1st minimum in
	// strips (e2tsts1-emins1)
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<17);
    }
    inline virtual Bool_t IsEM_ClusterStripsWtot() const {
	// Shower width in 1st sampling
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<18);
    }
    inline virtual Bool_t IsEM_ClusterStripsFracm() const {
	// Shower shape in shower core 1st sampling
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<19);
    }
    inline virtual Bool_t IsEM_ClusterStripsWeta1c() const {
	// Shower width weighted by distance from the maximum one
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<20);
    }
    inline virtual Bool_t IsEM_ClusterStripsDEmaxs1() const {
	// Difference between max and 2nd max in strips
	// for tight selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<21);
    }
    inline virtual Bool_t IsEM_TrackMatchEoverP() const {
	// Energy-momentum match for photon selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<22);
    }
    inline virtual Bool_t IsEM_Isolation() const {
	// Isolation
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<29);
    }
    inline virtual Bool_t IsEM_ClusterIsolation() const {
	// Calorimetric isolation for photon selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<30);
    }
    inline virtual Bool_t IsEM_TrackIsolation() const {
	// Tracker isolation for photon selection
	// (valid for >= Athena rel15.2.2)
	return fIsEMBitField & (0x1<<31);
    }

    ClassDef(AtlPhoton,13) // Atlas photon class
};
#endif
