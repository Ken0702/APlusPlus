//____________________________________________________________________
//
// Hadronic W boson decay class
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef ATLAS_AtlWDecayJJ
#define ATLAS_AtlWDecayJJ
#ifndef HEP_HepWDecay
#include <HepWDecay.h>
#endif
#ifndef ATLAS_AtlJet
#include <AtlJet.h>
#endif

//____________________________________________________________________

class AtlWDecayJJ : public HepWDecay {
    
  protected:
    HepJet  *fJet1;  // Jet 1 containing the 4-momentum computed during
                     // the W reconstruction
    HepJet  *fJet2;  // Jet 2 containing the 4-momentum computed during
                     // the W reconstruction
    
  public:
    AtlWDecayJJ();
    AtlWDecayJJ(Int_t Id, Float_t Px_W, Float_t Py_W, Float_t Pz_W,
		Float_t E_W, AtlJet *jet1_orig, AtlJet *jet2_orig,
		Float_t Px_j1, Float_t Py_j1, Float_t Pz_j1, Float_t E_j1,
		Float_t Px_j2, Float_t Py_j2, Float_t Pz_j2, Float_t E_j2,
		ProductionMode mode);
    virtual ~AtlWDecayJJ();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    
    inline AtlJet* GetJet1_Orig() const {
	// Reference to the (original) jet 1
	//
	// Note that the 4-momentuum of this jet remains unchanged by
	// the W reconstruction. To get the improved momentum use GetJet1()
	//
	return (AtlJet*)GetDaughter1();
    }
    inline AtlJet* GetJet2_Orig() const {
	// Reference to the (original) jet 2
	//
	// Note that the 4-momentuum of this jet remains unchanged by
	// the W reconstruction. To get the improved momentum use GetJet1()
	//
	return (AtlJet*)GetDaughter2();
    }
    inline HepJet* GetJet1() const {
	//
	// Reference to jet 1
	//
	// Note that the momentum of this jet had been changed by
	// the W reconstruction. To get the original (unchanged)
	// jet use GetJet1_Orig()
	//
	return fJet1;
    }
    inline HepJet* GetJet2() const {
	//
	// Reference to jet 2
	//
	// Note that the momentum of this jet had been changed by
	// the W reconstruction. To get the original (unchanged)
	// jet use GetJet2_Orig()
	//
	return fJet2;
    }
    inline virtual Bool_t IsHadronicDecay() const
    { return kTRUE; }
    inline virtual Bool_t IsLeptonicDecay() const
    { return kFALSE; }

    ClassDef(AtlWDecayJJ,2) // Hadronic W decay
};
#endif
