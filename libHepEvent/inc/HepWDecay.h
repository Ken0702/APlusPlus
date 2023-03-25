//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef HEP_HepWDecay
#define HEP_HepWDecay
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif

class TClass;

class HepWDecay : public HepDecayParticle {

  public:
    enum ProductionMode { kTTBar,
			  kSgTop_tChannel,
			  kSgTop_sChannel,
			  kSgTop_WtChannel,
			  kOther };

  protected:
    TRef  *fDaughter1;  // Daughter particle 1
    TRef  *fDaughter2;  // Daughter particle 2
    TRef  *fTruthW;     //Truth W of reconstructed W
    ProductionMode fMode;   // Production process of this W-boson     

  public:
    HepWDecay();
    HepWDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
	      Float_t E, TObject *Daughter1, TObject *Daughter2,
	      ProductionMode mode);
    virtual ~HepWDecay();
    virtual void Clear(Option_t *option);
    virtual Bool_t IsHadronicDecay() const = 0;
    virtual Bool_t IsLeptonicDecay() const = 0;
    
    inline TObject* GetDaughter1() const { return fDaughter1->GetObject(); }
    inline TObject* GetDaughter2() const { return fDaughter2->GetObject(); }
    inline HepMCParticle* GetTruthW() const
    { return (HepMCParticle*)fTruthW->GetObject(); }
    inline void SetTruthW(HepMCParticle* TruthW) {*fTruthW = (TObject*)TruthW;}
    void SetMode( ProductionMode mode ){ fMode = mode; }

    Bool_t IsTTBar(){
	if ( fMode == kTTBar ) return kTRUE;
	return kFALSE;
    }
    Bool_t IsSgTop_tChannel(){
	if ( fMode == kSgTop_tChannel ) return kTRUE;
	return kFALSE;
    }
    Bool_t IsSgTop_sChannel(){
	if ( fMode == kSgTop_sChannel ) return kTRUE;
	return kFALSE;
    }
    Bool_t IsSgTop_WtChannel(){
	if ( fMode == kSgTop_WtChannel ) return kTRUE;
	return kFALSE;
    }

    ClassDef(HepWDecay,5) // Base W boson decay class
};
#endif
