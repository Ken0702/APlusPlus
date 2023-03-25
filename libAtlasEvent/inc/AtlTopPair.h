//
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Felix Thomas
//
#ifndef ATLAS_AtlTopPair
#define ATLAS_AtlTopPair
#ifndef HEP_HepTopDecay
#include <HepTopDecay.h>
#endif
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ATLAS_AtlWDecayLNu
#include <AtlWDecayLNu.h>
#endif
#ifndef ATLAS_AtlWDecayJJ
#include <AtlWDecayJJ.h>
#endif


#include <TMath.h>
#include <TMatrixD.h>

class AtlTopPair : public TObject {

  public:
      
  enum EType { kInvalid = 0, kDiLeptonic, kSemiLeptonic, kFullHadronic};

  private:
    TRef          *fTop1;                               // Top quark 1
    TRef          *fTop2;                               // Top quark 2
    Double_t       fChi2;                               // Chi2 of the candidate as computed by the KinFitter 
    Int_t          fNdof;                               // number of degrees of freedom
    Double_t       fChi2Prob;                           // Chi2-Probability of the candidate as computed by the KinFitter
    Int_t          fId;                                 // Id of the reconstructed top pair
    EType          fType;                               // Type of the top pair (see EType enumeration)
    
    TMatrixD fChargedLeptonPull;
    TMatrixD fNeutrinoPull;
    TMatrixD fLepBJetPull;
    TMatrixD fHadJet1Pull;
    TMatrixD fHadJet2Pull;
    TMatrixD fHadBJetPull;
    
  public:
    AtlTopPair();
    AtlTopPair(Int_t Id, HepTopDecay* top1, HepTopDecay* top2, 
        Double_t chi2, Int_t ndof, AtlTopPair::EType type);
    virtual ~AtlTopPair();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    void InitPulls();
    inline HepTopDecay*       GetTop1() const { return (HepTopDecay*)fTop1->GetObject(); }
    inline HepTopDecay*       GetTop2() const { return (HepTopDecay*)fTop2->GetObject(); }
    HepTopDecay*       GetLeptonicTop() const;
    HepTopDecay*       GetHadronicTop() const;

    inline Double_t           GetChi2() const { return fChi2; }
    inline Double_t           GetChi2Prob() const { return fChi2Prob; }
    inline Int_t              GetId() const { return fId; }
    inline AtlTopPair::EType  GetType() const { return fType; }
    inline TMatrixD GetLeptonPull()   { return fChargedLeptonPull; }
    inline TMatrixD GetNeutrinoPull() { return fNeutrinoPull; }
    inline TMatrixD GetLepBJetPull()  { return fLepBJetPull; }
    inline TMatrixD GetHadJet1Pull()  { return fHadJet1Pull; }
    inline TMatrixD GetHadJet2Pull()  { return fHadJet2Pull; }
    inline TMatrixD GetHadBJetPull()  { return fHadBJetPull; }
    void SetPullMatrices(TMatrixD leptonpull, TMatrixD neutrinopull, TMatrixD lepbjetpull,
                         TMatrixD hadjet1pull, TMatrixD hadjet2pull, TMatrixD hadbjetpull);    
    ClassDef(AtlTopPair,3)  // ATLAS Top Pair
};

#endif
