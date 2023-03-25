//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
//
#ifndef HEP_HepTopDecay
#define HEP_HepTopDecay
#ifndef ROOT_TClass
#include <TClass.h>
#endif
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif
#ifndef HEP_HepJet
#include <HepJet.h>
#endif
#ifndef HEP_HepWDecay
#include <HepWDecay.h>
#endif
#ifndef HEP_HepMCParticle
#include <HepMCParticle.h>
#endif
class HepTopDecay : public HepDecayParticle {

  public:
    enum ProductionMode { kTTBar,
			  kSgTop_tChannel,
			  kSgTop_sChannel,
			  kSgTop_WtChannel,
			  kOther };
    
  protected:
    TRef     *fWDecay;      // W decay object associated to this top decay
    TRef     *fBJetOrig;    // Outgoing original B-jet of this top decay before fit
    TRef     *fTruthTop;    //Truth Top of reconstructed Top
    TLorentzVector fP_BJet; // Improved 4-momentum of outgoing B-jet after fit
    ProductionMode fMode;   // Production process of this top quark 
    
  public:
    HepTopDecay();
    HepTopDecay(Int_t Id, Float_t Px_T, Float_t Py_T, Float_t Pz_T,
		Float_t E_T, HepWDecay *WDecay, HepJet *BJetOrig,
		Float_t Px_j, Float_t Py_j, Float_t Pz_j, Float_t E_j,
		ProductionMode mode);
    virtual ~HepTopDecay();
    virtual void Clear(Option_t *option = "");
    virtual Int_t Compare(const TObject *obj) const;
    virtual void Print(Option_t *option = "");

    inline virtual Bool_t IsSortable() const { return kTRUE; }
    inline Bool_t IsTTBar() {
	if ( fMode == kTTBar ) return kTRUE;
	return kFALSE;
    }
    inline Bool_t IsSgTop_tChannel() {
	if ( fMode == kSgTop_tChannel ) return kTRUE;
	return kFALSE;
    }
    inline Bool_t IsSgTop_sChannel() {
	if ( fMode == kSgTop_sChannel ) return kTRUE;
	return kFALSE;
    }
    inline Bool_t IsSgTop_WtChannel() {
	if ( fMode == kSgTop_WtChannel ) return kTRUE;
	return kFALSE;
    }
    inline HepWDecay* GetWDecay() const {
	// Return associated W decay object
	return (HepWDecay*)fWDecay->GetObject();
    }
    inline HepJet* GetBJetOrig() const {
	//
	// Return unchanged b-jet connected to this top decay
	//
	return (HepJet*)fBJetOrig->GetObject();
    }
    inline TLorentzVector GetP_BJet() const {
	//
	// Return improved 4-momentum of b-jet associated to this top decay 
	//
	return fP_BJet;
    }
    inline Bool_t IsHadronicDecay() const {
	// W has decayed hadronically ?
	return GetWDecay()->IsHadronicDecay();
    }
    inline Bool_t IsSemiLeptonicDecay() const {
	// W has decayed leptonically ?
	return GetWDecay()->IsLeptonicDecay();
    }
    inline HepMCParticle* GetTruthTop() const { 
	// Get truth top-quark of reconstructed top. Pointer is zero if no
	// truth top exists
	return (HepMCParticle*)fTruthTop->GetObject();
    } 
    inline void SetTruthTop(HepMCParticle *TruthTop)
    { *fTruthTop = (TObject*)TruthTop;}
    inline void SetMode(ProductionMode mode) { fMode = mode; }
    
    ClassDef(HepTopDecay,6) // Top quark decay class
};
#endif
