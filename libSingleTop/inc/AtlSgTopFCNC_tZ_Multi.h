#ifndef SGTOP_AtlSgTopFCNC_tZ_Multi
#define SGTOP_AtlSgTopFCNC_tZ_Multi

#include <AtlSgTopFCNC_tZ_Base3L.h>


class AtlSgTopFCNC_tZ_Multi : public AtlSgTopFCNC_tZ_Base3L {

  public:

    class Subselection : public TNamed {

        friend class AtlSgTopFCNC_tZ_Multi;

    public:

        explicit Subselection(char const * name) :
            TNamed(name, name),
            fNLeptons_min(-1),
            fNLeptons_max(-1),
            fAbsSumLepQ(-1),
            fNJets_min(-1),
            fNJets_max(-1),
            fNBTags_min(-1),
            fNBTags_max(-1),
            fNLightJets_min(-1),
            fNLightJets_max(-1),
            fPt_Lep1_min(NAN),
            fPt_Lep2_min(NAN),
            fPt_Lep3_min(NAN),
            fMET_min(NAN),
            fMET_max(NAN),
            fMtW_min(NAN),
            fMtW_max(NAN),
            fPtZ_max(NAN),
            fReconstructTop(kFALSE),
            fRequireZ(kFALSE),
            fRequireTop(kFALSE),
            fVetoZbosonLate(kFALSE),
            fRecord(kTRUE),
            fHistsAllCuts(0)
        { }

        Int_t    fNLeptons_min;
        Int_t    fNLeptons_max;
        Int_t    fAbsSumLepQ;
        Int_t    fNJets_min;
        Int_t    fNJets_max;
        Int_t    fNBTags_min;
        Int_t    fNBTags_max;
        Int_t    fNLightJets_min;
        Int_t    fNLightJets_max;
        Double_t fPt_Lep1_min;
        Double_t fPt_Lep2_min;
        Double_t fPt_Lep3_min;
        Double_t fMET_min;
        Double_t fMET_max;
        Double_t fMtW_min;
        Double_t fMtW_max;
        Double_t fPtZ_max;
        Bool_t   fReconstructTop;
        Bool_t   fRequireZ;
        Bool_t   fRequireTop;
        Bool_t   fVetoZbosonLate;
        Bool_t   fRecord;

    private:

        AtlHistogramTool * fHistsAllCuts;

    };

    Int_t fNJets_min;
    Int_t fNJets_max;
    Bool_t fVetoZbosonLate;

    TList * fListOfSubselections;

    AtlSgTopFCNC_tZ_Multi(const char* OutputFileName);
    ~AtlSgTopFCNC_tZ_Multi() /* override */;

    void BookHistograms() /* override */;
    void FillHistograms() /* override */;
    Bool_t ProcessPreCut() /* override */;
    Bool_t ProcessCut() /* override */;
    void SlaveBegin(TTree *) /* override */;
    virtual Bool_t ProcessCurrentSubselection();
    void SetCutDefaults() /* override */;
    void Print(Option_t *option) const /* override */;

protected:

    Subselection * fSubselection;

    ClassDef(AtlSgTopFCNC_tZ_Multi, 0) // Analysis class for the single-top FCNC tZ analysis
};

#endif

