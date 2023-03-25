//
// Author: Andreas Hartmetz <mailto: hartmetz@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Andreas Hartmetz
//
#ifndef ATLAS_AtlBTag
#define ATLAS_AtlBTag

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#include <math.h>

class TString;

class AtlBTag : public TObject {

  public:
    enum ETagger {
	// taggers Athena release 17
	kIP2d,
	kIP3d,
	kSVInfoPlus,
	kSV1,
	kSV2,
	kSV0,
	kSV0InfoPlus,
	kSoftMuonTag,
	kJetProb,
	kJetFitterTagNN,
	kJetFitterCOMBNN,
	kBaseline,
	kMV1,
	kMV1c,
	kMV2c20,
	kMV2c10_60,
	kMV2c10_70,
	kMV2c10_77,
	kMV2c10_85,
	kDL1r_85,
	kDL1r_77,
	kDL1r_70,
	kDL1r_60,
	// Insert new taggers above
	kNumTaggers,
	kInvalidTagger
    };

  private:
    ETagger  fTagger;  // Type of the used B-tag algorithm
    Double_t fWeight;  // B-tag weight
    Bool_t   fValid;   // Valid Tagger ?

    Int_t    fNTrk;           // Number of tracks used for tagging in the jet (IP2d, IP3d), total number of vertex tracks (SVInfoPlus, SV0InfoPlus) , total number of tracks at vertices with more than one track (JetFitterTag)
    Double_t fPb;             // b likelihood (IP2d, IP3d, SV1, SV2, JetFitterTag only)
    Double_t fPu;             // u likelihood (IP2d, IP3d, SV1, SV2, JetFitterTag only)
    Double_t fSecVtxMass;     // Mass of secondary vertex (SVInfoPlus, SV0InfoPlus, JetFitterTag only)
    Int_t    fN1TrkSecVtx;    // Number of single-track vertices (JetFitterTag only)
    Int_t    fN2TrkSecVtx;    // Number of two-track vertices (SVInfoPlus, SV0InfoPlus only)
    Int_t    fNManyTrkSecVtx; // Number of >1 track vertices (JetFitterTag only)
    Bool_t   fGoodSecVtx;     // Flag for good vertices (SVInfoPlus, SV0InfoPlus only)
    Int_t    fNGoodVtxTrk;    // Number of good tracks in vertex (SVInfoPlus, SV0InfoPlus only)
    Int_t    fNGoodJetTrk;    // Number of good tracks in jet (SVInfoPlus, SV0InfoPlus only)
    Double_t fEFrac;          // Energy fraction sec. vtx/jet (SVInfoPlus, SV0InfoPlus, JetFitterTag only)
    Double_t fSignificance3d; // 3d significance (JetFitterTag only)
    Double_t fDeltaPhi;       // delta(phi) between sum of all momenta at vertices and the fitted B-meson flight direction (JetFitterTag only)
    Double_t fDeltaEta;       // delta(eta) between sum of all momenta at vertices and the fitted B-meson flight direction (JetFitterTag only)
    
  public:
    AtlBTag();
    AtlBTag(ETagger tagger, Double_t weight, Bool_t Valid);
    virtual ~AtlBTag();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();
    static const char* GetTaggerName(ETagger tagger);
    static ETagger GetTaggerByName(const char *name);
    
    inline ETagger GetTagger() const { return fTagger; }
    inline Double_t GetWeight() const {
        if (fTagger == kJetProb) return -log10(fWeight);
        else return fWeight; }
    inline Bool_t IsValid() const { return fValid; }
    inline const char* GetName() const {
	// Get name of current tagger
	return GetTaggerName(fTagger);
    }
    inline Int_t GetNTrk() const {
	// Number of tracks used for tagging
	return fNTrk;
    }
    inline Double_t GetPb() const {
	// b likelihood
	return fPb;
    }
    inline Double_t GetPu() const {
	// u likelihood
	return fPu;
    }
    inline Double_t GetSecVtxMass() const {
        // SV0, mass of secondary vertex
	return fSecVtxMass;
    }
    inline Int_t GetN1TrkSecVtx() const {
	// SV0, number of single-track vertices
	return fN1TrkSecVtx;
    }
    inline Int_t GetNManyTrkSecVtx() const {
	// SV0, number of many-track vertices
	return fNManyTrkSecVtx;
    }
    inline Int_t GetN2TrkSecVtx() const {
	// SV0, number of two-track vertices
	return fN2TrkSecVtx;
    }
    inline Bool_t IsGoodSecVtx() const {
        // SV0, flag for good vertices
	return fGoodSecVtx;
    }
    inline Int_t GetNGoodVtxTrk() const {
	// SV0, number of good tracks in vertex
	return fNGoodVtxTrk;
    }
    inline Int_t GetNGoodJetTrk() const {
	// SV0, number of good tracks in jet
	return fNGoodJetTrk;
    }
    inline Double_t GetEFrac() const {
	// SV0, energy fraction sec. vtx/jet
	return fEFrac;
    }
    inline Double_t GetSignificance3d() const {
	// 3d significance (JetFitterTag only)
	return fSignificance3d;
    }
    inline Double_t GetDeltaPhi() const {
	// delta(phi) between sum of all momenta at vertices and the
	// fitted B-meson flight direction (JetFitter only)
	return fDeltaPhi;
    }
    inline Double_t GetDeltaEta() const {
	// delta(eta) between sum of all momenta at vertices and the
	// fitted B-meson flight direction (JetFitter only)
	return fDeltaEta;
    }
    inline void SetNTrk(Int_t NTrk)
    { fNTrk = NTrk; }
    inline void SetPbPu(Double_t Pb, Double_t Pu)
    { fPb = Pb; fPu = Pu; }
    inline void SetSecVtxMass(Double_t SecVtxMass)
    { fSecVtxMass = SecVtxMass; }
    inline void SetN1TrkSecVtx(Int_t N1TrkSecVtx)
    { fN1TrkSecVtx = N1TrkSecVtx; }
    inline void SetN2TrkSecVtx(Int_t N2TrkSecVtx)
    { fN2TrkSecVtx = N2TrkSecVtx; }
    inline void SetNManyTrkSecVtx(Int_t NManyTrkSecVtx)
    { fNManyTrkSecVtx = NManyTrkSecVtx; }
    inline void SetGoodSecVtx(Bool_t GoodSecVtx)
    { fGoodSecVtx = GoodSecVtx; }
    inline void SetNGoodVtxTrk(Int_t NGoodVtxTrk)
    { fNGoodVtxTrk = NGoodVtxTrk; }
    inline void SetNGoodJetTrk(Int_t NGoodJetTrk)
    { fNGoodJetTrk = NGoodJetTrk; }
    inline void SetEFrac(Double_t EFrac)
    { fEFrac = EFrac; }
    inline void SetSignificance3d(Double_t Significance3d)
    { fSignificance3d = Significance3d; }
    inline void SetDeltaPhiEta(Double_t DeltaPhi, Double_t DeltaEta) {
	fDeltaPhi = DeltaPhi;
	fDeltaEta = DeltaEta;
    }
    
    ClassDef(AtlBTag,14)  // ATLAS B-tagger result
};

#endif
