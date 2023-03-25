//____________________________________________________________________
//
// Leptonic W boson decay class
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef ATLAS_AtlWDecayLNu
#define ATLAS_AtlWDecayLNu
#ifndef HEP_HepWDecay
#include <HepWDecay.h>
#endif
#ifndef ATLAS_HepParticle
#include <HepParticle.h>
#endif

//____________________________________________________________________

class AtlWDecayLNu : public HepWDecay {

 protected:
    HepParticle *fChargedLepton;  // Charged lepton containing the
                                  // momentum computed during the W
                                  // reconstruction. The PDG code is
                                  // taken from the original lepton
    
 public:
    AtlWDecayLNu();
    AtlWDecayLNu(Int_t Id, Float_t Px_W, Float_t Py_W,
		 Float_t Pz_W, Float_t E_W, HepParticle *lepton_orig,
		 Float_t Px_lep, Float_t Py_lep, Float_t Pz_lep,
		 Float_t E_lep, HepParticle *neutrino,
		 ProductionMode mode);
    virtual ~AtlWDecayLNu();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");

    inline virtual Bool_t IsHadronicDecay() const
    { return kFALSE; }
    inline virtual Bool_t IsLeptonicDecay() const
    { return kTRUE; }
    inline HepParticle* GetChargedLeptonOrig() const {
	//
	// Reference to the (original) charged lepton
	//
	// Note that the momentum of this lepton remains unchanged by
	// the W reconstruction. To get the re-computed momentum use
	// GetChargedLepton()
	//
	return (HepParticle*)GetDaughter1();
    }
    inline HepParticle* GetChargedLepton() const {
	//
	// Reference to the charged lepton
	//
	// Note that the momentum of this lepton has been changed by
	// the W reconstruction. To get the original (unchanged)
	// use lepton GetChargedLeptonOrig()
	//
	return fChargedLepton;
    }
    inline HepParticle* GetNeutrino() const {
	//
	// Get reference to the outgoing neutrino which has been
	// obtained by the W reconstruction
	//
	return (HepParticle*)GetDaughter2();
    }

    ClassDef(AtlWDecayLNu,2) // Leptonic W decay
};
#endif
