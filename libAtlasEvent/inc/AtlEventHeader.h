//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEventHeader
#define ATLAS_AtlEventHeader
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ATLAS_AtlEvtTreeInfo
#include <AtlEvtTreeInfo.h>
#endif
#ifndef ATLAS_AtlScaleFactor
#include <AtlScaleFactor.h>
#endif

class TDatime;

class AtlEventHeader : public TObject {

 public:
	Int_t ejets2017;
	Int_t mujets2017;
	Int_t ejets2018;
	Int_t mujets2018;
	Int_t ejets2016;
	Int_t mujets2016;
	Int_t ejets2015;
	Int_t mujets2015;
  enum EType {
    kPhysics,
    kCalibration,
    kCosmics,
    kTestBeam
  };
    
    enum EPreselectionFlags {
        kElJets = BIT(0),

    	kMuJets = BIT(1),
        kElEl = BIT(2),
        kElMu = BIT(3),
        kMuMu = BIT(4),
        kElElEl = BIT(5),
        kElElMu = BIT(6),
        kElMuMu = BIT(7),
        kMuMuMu = BIT(8),
        kLeptonic = BIT(24),
        k2Lep = (kElEl | kElMu | kMuMu),
        k3Lep = (kElElEl | kElElMu | kElMuMu | kMuMuMu),
    };

private:
    UInt_t    fRunNr;                   // Run number
    ULong64_t fEventNr;                 // Event number
    UInt_t    fLumiBlock;               // Luminosity block identifier
    Float_t   fBeamEnergy;              // Beam energy (GeV)
    EType     fEventType;               // Type of event
    UInt_t    fChannelNr;               // Channel number
    UInt_t    fHFOR_type;               // HFOR type number
    Bool_t    fIsMC;                    // Is real data or simulation ?
    Double_t  fMCWeight;                // MC Generator weight
    Double_t  fPileupWeight;            // Pile-up weight
    Double_t  fZVtxWeight;              // Z-vertex weight
    Float_t   fPreTagEvtWeight;         // Total event weight w/o b-tagging (see the AtlObjRecoScaleFactorTool for details)
    Float_t   fTagEvtWeight;            // Total event weight w/ b-tagging (see the AtlObjRecoScaleFactorTool for details)
    Float_t   fMMWeight_nominal;        // MM weight
    Float_t   fMMWeight_fake_mc_up;     // MM weight
    Float_t   fMMWeight_fake_mc_down;   // MM weight
    Float_t   fMMWeight_fake_alternate; // MM weight
    Float_t   fMMWeight_real_alternate; // MM weight
    Double_t  fTruthWeightCorrection;   // generic truth reweighting (e.g. PDFs, Pt(top), ...)
    Float_t   fActualIntPerXing;        // Actual interactions per bunch-crossing
    Float_t   fAverageIntPerXing;       // Average interactions per bunch-crossing
    // PDF information for systematics
    Double_t  fmcevt_pdf1;
    Double_t  fmcevt_pdf2;
    Int_t     fmcevt_pdf_id1;
    Int_t     fmcevt_pdf_id2;
    Double_t  fmcevt_pdf_scale;
    Double_t  fmcevt_pdf_x1;
    Double_t  fmcevt_pdf_x2;

    ULong_t  fPreselectionFlags;
    Long_t   fTruthFlavor;
    Int_t    fMEPhotonFlag;
    
    AtlScaleFactor * fJetVertexWeight; //! weight correcting jet-vertex-fraction cut efficiencies
    AtlScaleFactor * fLeptonWeight; //! weight correcting lepton (trigger) efficiencies
    AtlScaleFactor * fBtagWeight_MV2c10_60; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_MV2c10_70; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_MV2c10_77; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_MV2c10_85; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_MV2c20_77; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_MV2c20_85; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_DL1r_85; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_DL1r_77; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_DL1r_70; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fBtagWeight_DL1r_60; //! weight correcting b-tagging efficiencies
    AtlScaleFactor * fForwardJVTWeight;
    AtlScaleFactor * fLeptonTriggerSF;

    static AtlEvtTreeInfo *fgTreeInfo; //! Pointer to the current A++ tree info object
    
public:
    AtlEventHeader();
    virtual ~AtlEventHeader();

    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    void PrintEventType() const;

    Float_t BeamEnergy() const { return fBeamEnergy; }
    
    inline UInt_t RunNr() const { return fRunNr; }
    inline ULong64_t EventNr() const { return fEventNr; }
    inline UInt_t LumiBlock() const { return fLumiBlock; }
    inline Bool_t IsMC() const { return fIsMC; }
    inline Bool_t IsDATA() const { return !fIsMC; }
    inline Bool_t IsPhysics() const { return fEventType == kPhysics; }
    inline Bool_t IsCosmics() const { return fEventType == kCosmics; }
    inline Bool_t IsTestBeam() const { return fEventType == kTestBeam; }
    inline Bool_t IsCalibration() const { return fEventType == kCalibration; }
    inline Float_t  GetPreTagEvtWeight() const { return fPreTagEvtWeight; }
    inline Float_t  GetTagEvtWeight() const { return fTagEvtWeight; }
    inline Double_t GetMCWeight() const { return fMCWeight; }
    inline Double_t GetPileupWeight() const { return fPileupWeight; }
    inline Double_t GetZVtxWeight() const { return fZVtxWeight; }
    inline Double_t GetMMWeightNominal() const { return fMMWeight_nominal; }
    inline Double_t GetMMWeightFake_MC_up() const { return fMMWeight_fake_mc_up; }
    inline Double_t GetMMWeightFake_MC_down() const { return fMMWeight_fake_mc_down; }
    inline Double_t GetMMWeightFake_alternate() const { return fMMWeight_fake_alternate; }
    inline Double_t GetMMWeightReal_alternate() const { return fMMWeight_real_alternate; }
    inline Double_t GetTruthWeightCorrection() const { return fTruthWeightCorrection; }
    inline Float_t  GetActualIntPerXing() const { return fActualIntPerXing; }
    inline UInt_t   GetChannelNumber() const { return fChannelNr; }
    inline Float_t  GetAverageIntPerXing() const { return fAverageIntPerXing; }
    inline UInt_t   GetHFORType() const { return fHFOR_type; }
    Long_t GetTruthFlavor() const { return fTruthFlavor; }

    inline Bool_t IsRun1() const {
	//
	// Is ATLAS Run-1 event ?
	//
	return ( fBeamEnergy == 4. ) ? kTRUE : kFALSE;
    }
    inline Bool_t IsRun2() const {
	//
	// Is ATLAS Run-2 event ?
	//
	return ( fBeamEnergy >= 6.5 ) ? kTRUE : kFALSE;
    }
    
    inline void SetRunNr(UInt_t RunNr) { fRunNr = RunNr; }
    inline void SetEventNr(ULong64_t EventNr) { fEventNr = EventNr; }
    inline void SetEventType(EType type) { fEventType = type; }
    inline void SetMC(Bool_t IsMC) { fIsMC = IsMC; }
    inline void SetLumiBlock(UInt_t LumiBlock) { fLumiBlock = LumiBlock; }
    inline void SetBeamEnergy(Float_t BeamEnergy) {
	fBeamEnergy = BeamEnergy; }
    inline void SetPreTagEvtWeight(Float_t Weight) {
	fPreTagEvtWeight = Weight;
    }
    inline void SetTagEvtWeight(Float_t Weight) {
	fTagEvtWeight = Weight;
    }
    inline void SetMCWeight( Double_t mc_weight ) {
	fMCWeight = mc_weight;
    }
    inline void SetPileupWeight( Double_t pileup_weight ) {
	fPileupWeight = pileup_weight;
    }
    inline void SetZVtxWeight( Double_t zvtx_weight ) {
	fZVtxWeight = zvtx_weight;
    }
    inline void SetMMWeightNominal( Double_t mm_weight ) {
	fMMWeight_nominal = mm_weight;
    }
    inline void SetMMWeightFakeMCup( Double_t mm_weight ) {
	fMMWeight_fake_mc_up = mm_weight;
    }
    inline void SetMMWeightFakeMCdown( Double_t mm_weight ) {
	fMMWeight_fake_mc_down = mm_weight;
    }
    inline void SetMMWeightFakeAlternate( Double_t mm_weight ) {
	fMMWeight_fake_alternate = mm_weight;
    }
    inline void SetMMWeightRealAlternate( Double_t mm_weight ) {
	fMMWeight_real_alternate = mm_weight;
    }
    inline void SetTruthWeightCorrection( Double_t TruthWeightCorrection ) {
	fTruthWeightCorrection = TruthWeightCorrection;
    }
    inline void SetAverageIntPerXing(Float_t intpercrossing) {
      fAverageIntPerXing = intpercrossing;
    }
    inline void SetActualIntPerXing(Float_t intpercrossing) {
      fActualIntPerXing = intpercrossing;
    }
    inline void SetChannelNumber(UInt_t channelnumber) {
      fChannelNr = channelnumber;
    }
    inline void SetHFORType(UInt_t hfor_type) {
        fHFOR_type = hfor_type;
    }
    void SetTruthFlavor(Long_t TruthFlavor);

    inline static AtlEvtTreeInfo* GetEvtTreeInfo()
    { return fgTreeInfo; }
    inline static void SetEvtTreeInfo(AtlEvtTreeInfo* TreeInfo)
    { fgTreeInfo = TreeInfo; }

    // PDF information
    inline void SetPDFInfo(Double_t mcevt_pdf1,
			   Double_t mcevt_pdf2,
			   Int_t mcevt_pdf_id1,
			   Int_t mcevt_pdf_id2,
			   Double_t mcevt_pdf_scale,
			   Double_t mcevt_pdf_x1,
			   Double_t mcevt_pdf_x2) {
	//
	// Store info for PDF systematics
	//
	fmcevt_pdf1 = mcevt_pdf1;
	fmcevt_pdf2 = mcevt_pdf2;
	fmcevt_pdf_id1 = mcevt_pdf_id1;
	fmcevt_pdf_id2 = mcevt_pdf_id2;
	fmcevt_pdf_scale = mcevt_pdf_scale;
	fmcevt_pdf_x1 = mcevt_pdf_x1;
	fmcevt_pdf_x2 = mcevt_pdf_x2;
    }

    Double_t Get_mcevt_pdf1() const { return fmcevt_pdf1; }
    Double_t Get_mcevt_pdf2() const { return fmcevt_pdf2; }
    Int_t    Get_mcevt_pdf_id1() const { return fmcevt_pdf_id1; }
    Int_t    Get_mcevt_pdf_id2() const { return fmcevt_pdf_id2; }
    Double_t Get_mcevt_pdf_scale() const { return fmcevt_pdf_scale; }
    Double_t Get_mcevt_pdf_x1() const { return fmcevt_pdf_x1; }
    Double_t Get_mcevt_pdf_x2() const { return fmcevt_pdf_x2; }
    
    void SetPreselectionFlags(ULong_t PreselectionFlags);
    ULong_t GetPreselectionFlags() const { return fPreselectionFlags; }
    Bool_t TestPreselectionFlagsAny(ULong_t PreselectionFlags) const { return (fPreselectionFlags & PreselectionFlags); }

    void SetMEPhoton(bool flag);
    Bool_t HasMEPhoton() const;

    AtlScaleFactor & JetVertexWeight() { return *fJetVertexWeight; }
    AtlScaleFactor & LeptonWeight() { return *fLeptonWeight; }
    AtlScaleFactor & BtagWeight_MV2c10_60() { return *fBtagWeight_MV2c10_60; }
    AtlScaleFactor & BtagWeight_MV2c10_70() { return *fBtagWeight_MV2c10_70; }
    AtlScaleFactor & BtagWeight_MV2c10_77() { return *fBtagWeight_MV2c10_77; }
    AtlScaleFactor & BtagWeight_MV2c10_85() { return *fBtagWeight_MV2c10_85; }
    AtlScaleFactor & BtagWeight_MV2c20_77() { return *fBtagWeight_MV2c20_77; }
    AtlScaleFactor & BtagWeight_MV2c20_85() { return *fBtagWeight_MV2c20_85; }
    AtlScaleFactor & BtagWeight_DL1r_85() { return *fBtagWeight_DL1r_85; }
    AtlScaleFactor & BtagWeight_DL1r_77() { return *fBtagWeight_DL1r_77; }
    AtlScaleFactor & BtagWeight_DL1r_70() { return *fBtagWeight_DL1r_70; }
    AtlScaleFactor & BtagWeight_DL1r_60() { return *fBtagWeight_DL1r_60; }
    AtlScaleFactor & ForwardJVT() { return *fForwardJVTWeight; }
    AtlScaleFactor & LeptonTriggerSF() { return *fLeptonTriggerSF; }



    ClassDef(AtlEventHeader,35) // ATLAS event header
};
#endif

