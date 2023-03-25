//____________________________________________________________________
//
// Single-top FCNC tZ analysis
// 
//  
#include "AtlSgTopFCNC_tZ_Multi.h"

#include <cassert>
#include <cmath>

#include <Rtypes.h>
#include <TList.h>

#include "AtlHistogramTool.h"
#include "AtlEvtWriterD3PDSgTopThinned.h"

using namespace std;

//____________________________________________________________________

AtlSgTopFCNC_tZ_Multi::AtlSgTopFCNC_tZ_Multi(const char* OutputFileName) :
    AtlSgTopFCNC_tZ_Base3L(OutputFileName) {
    //
    // Default constructor
    //
    SetCutDefaults();
    fListOfSubselections = new TList();
    fSubselection = 0;
}

//____________________________________________________________________

AtlSgTopFCNC_tZ_Multi::~AtlSgTopFCNC_tZ_Multi() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Multi::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    AtlSgTopFCNC_tZ_Base3L::SetCutDefaults();
    fNLeptons_min = -1;
    fNLeptons_max = -1;
    fNJets_min = -1;
    fNJets_max = -1;
    fNBTags_min = -1;
    fNBTags_max = -1;
    fPt_Lep1_min = NAN;
    fPt_Lep2_min = NAN;
    fPt_Lep3_min = NAN;
    fMET_min = NAN;
    fMET_max = NAN;
    fMtW_min = NAN;
    fMtW_max = NAN;
    fMassZ_min = NAN; // disabling the Z-mass window in the Z reconstruction
    fMassZ_max = NAN;
    fMassTop_min = NAN; // disabling the top-mass window in the top reconstruction
    fMassTop_max = NAN;
    fReconstructTop = kFALSE;
    fRequireZ = kFALSE;
    fRequireTop = kFALSE;
    fVetoZboson = kFALSE; // not supported; use the proper Z veto (fVetoZbosonLate) instead
    fVetoTop = kFALSE; // not supported
    fVetoZbosonLate = kFALSE;
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Multi::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top FCNC tZ Selector                     " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSgTopFCNC_tZ_Base3L::Print(option);
    cout << endl
	 << "========================================================"
	 << endl << endl;   
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Multi::ProcessPreCut() {
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Multi::ProcessCut() {
    //
    // Event selection
    //

    bool selected = kFALSE;

    for ( TIter next(fListOfSubselections); Subselection * subsel = static_cast<Subselection *>(next()); ) {
        assert( !fSubselection );
        fSubselection = subsel;

        SetCutDefaults();
        fNLeptons_min = subsel->fNLeptons_min;
        fNLeptons_max = subsel->fNLeptons_max;
        fNJets_min = subsel->fNJets_min;
        fNJets_max = subsel->fNJets_max;
        fNBTags_min = subsel->fNBTags_min;
        fNBTags_max = subsel->fNBTags_max;
        fPt_Lep1_min = subsel->fPt_Lep1_min;
        fPt_Lep2_min = subsel->fPt_Lep2_min;
        fPt_Lep3_min = subsel->fPt_Lep3_min;
        fMET_min = subsel->fMET_min;
        fMET_max = subsel->fMET_max;
        fMtW_min = subsel->fMtW_min;
        fMtW_max = subsel->fMtW_max;
        fReconstructTop = subsel->fReconstructTop;
        fRequireZ = subsel->fRequireZ;
        fRequireTop = subsel->fRequireTop;
        fVetoZboson = kFALSE;
        fVetoTop = kFALSE;
        fVetoZbosonLate = subsel->fVetoZbosonLate;
        assert( fReconstructTop || !fRequireTop );
        assert( fRequireZ || !fRequireTop );

        fHistsAllCuts = subsel->fHistsAllCuts;

        Bool_t subselected = ProcessCurrentSubselection();
        if ( subselected ) {
            assert( fEvent->GetEventHeader()->TestPreselectionFlagsAny(AtlEventHeader::kLeptonic) );
        }
        if ( subsel->fRecord ) {
            if ( subselected ) {
               selected = kTRUE;
            }
            if ( AtlEvtWriterD3PDSgTopThinned * evtWriter = dynamic_cast<AtlEvtWriterD3PDSgTopThinned *>(fEvtWriter) ) {
                evtWriter->SetSubselectionFlag(subsel->GetName(), subselected);
            }
        }

        fHistsAllCuts = 0;

        fSubselection = 0;
    }

    SetCutDefaults();

    return selected;
}

//____________________________________________________________________

Bool_t AtlSgTopFCNC_tZ_Multi::ProcessCurrentSubselection() {

    // Leptons

    double Pt_Lep[3] = { 0., 0., 0. };
    for ( int i = 0; i != min(fLeptons->GetEntries(), 3); ++i ) {
        Pt_Lep[i] = static_cast<HepParticle *>(fLeptons->At(i))->Pt();
    }
    if ( !(Pt_Lep[0] >= fPt_Lep1_min) )
        return kFALSE;
    if ( !(Pt_Lep[1] >= fPt_Lep2_min) )
        return kFALSE;
    if ( !(Pt_Lep[2] >= fPt_Lep3_min) )
        return kFALSE;

    if ( !(fLeptons->GetEntries() >= fNLeptons_min ) )
        return kFALSE;
    if ( !(fLeptons->GetEntries() <= fNLeptons_max ) )
        return kFALSE;

    if ( fSubselection->fAbsSumLepQ >= 0 ) {
        int SumLepQ = 0;
        for ( int i = 0; i != fLeptons->GetEntries(); ++i ) {
            HepParticle * lep = static_cast<HepParticle *>(fLeptons->At(i));
            if (lep->Charge() > 0)
                ++SumLepQ;
            else if (lep->Charge() < 0)
                --SumLepQ;
        }
        if ( fSubselection->fAbsSumLepQ != abs(SumLepQ) )
            return kFALSE;
    }

    // Jets

    if ( !(fJets->GetEntries() >= fNJets_min) )
        return kFALSE;

    if ( !(fJets->GetEntries() <= fNJets_max) )
        return kFALSE;

    // Missing Et

    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( !(fMET >= fMET_min) )
        return kFALSE;
    if ( !(fMET < fMET_max) )
        return kFALSE;

    // B-tagging requirement

    if ( !(fBJets->GetEntries() >= fNBTags_min) )
        return kFALSE;
    if ( !(fBJets->GetEntries() <= fNBTags_max) )
        return kFALSE;

    int NLightJets = fJets->GetEntries() - fBJets->GetEntries();
    if ( fSubselection->fNLightJets_min >= 0 && !(NLightJets >= fSubselection->fNLightJets_min) )
        return kFALSE;
    if ( fSubselection->fNLightJets_max >= 0 && !(NLightJets <= fSubselection->fNLightJets_max) )
        return kFALSE;

    // Transverse W mass (leading lepton !)
    //
    // Do not cut on this MtW definition but on the proper MtW using
    // the lepton from the top decay (see below)
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));

    // =================
    // tZ reconstruction
    // =================
    ReconstructZ0Top();

    // Require a Z candidate
    // Note: in order for this to work properly, we must use the same notion of "distance" as
    //       HepZ0Decay::Compare does -> use a symmetric cut about the Z mass.
    {
        int NumberOfGoodCandidates = 0;
        for ( TIter next(fEvent->GetZ0Decays()); HepZ0Decay * z = static_cast<HepZ0Decay *>(next()); ) {
            double m = z->P().M() - 91.187; /* use same number as in HepZ0Decay::Compare */
            if (fabs(m) <= 10.) {
                NumberOfGoodCandidates += 1;
            }
        }
        if ( fRequireZ && NumberOfGoodCandidates < 1 )
            return kFALSE;
        if ( fVetoZbosonLate && NumberOfGoodCandidates > 0 )
            return kFALSE;
    }
    
    // Require a top-quark candidate
    if ( fRequireTop && fEvent->GetN_TopDecays() < 1 )
        return kFALSE;

    // Cut on transverse W mass (based on the lepton originating from
    // the reconstrcuted top decay)
    if ( fMtW_min > 0. || fMtW_max < INFINITY ) {
        HepParticle * theLepton = 0;
        if ( fEvent->GetN_TopDecays() > 0 ) {
            // use lepton associated with the top-quark candidate
            HepTopDecay *top = (HepTopDecay*)fEvent->GetTopDecays()->At(0);
            AtlWDecayLNu *Wboson = (AtlWDecayLNu*)top->GetWDecay();
            theLepton = Wboson->GetChargedLeptonOrig();
        }
        else {
            // use hardest lepton not associated with the best Z0 candidate
            HepZ0Decay * Z0 = GetZ0DecayBest();
            TObject * lep1 = (Z0 ? Z0->GetDaughter1() : 0);
            TObject * lep2 = (Z0 ? Z0->GetDaughter2() : 0);
            for (TIter next(fLeptons); TObject * lep = next(); ) {
                if ( lep != lep1 && lep != lep2 ) {
                   theLepton = static_cast<HepParticle *>(lep);
                   break;
                }
            }
        }
        assert( theLepton );
        Float_t MtW = fEvent->W_Mperp(theLepton);
        if ( !(MtW >= fMtW_min && MtW < fMtW_max) )
            return kFALSE;
    }

    // Fill histograms for with all cuts applied
    FillHistogramsCommon(fHistsAllCuts, fEvent->GetTagEvtWeight());
    if ( fSubselection->fPtZ_max < INFINITY ) {
        HepZ0Decay * Z0 = GetZ0DecayBest();
        if ( !Z0 )
            return kFALSE;
        if ( !(Z0->Pt() < fSubselection->fPtZ_max) )
            return kFALSE;
    }
    if ( GetZ0DecayBest() ) {
        FillHistogramsZ0(fHistsAllCuts, fEvent->GetTagEvtWeight(), "Z0_best", GetZ0DecayBest());
    }
    if ( fReconstructTop ) {
        FillHistogramsTop(fHistsAllCuts, fEvent->GetTagEvtWeight());
    }

    // Accept event
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Multi::BookHistograms() {
    //
    // Book histograms
    //
    assert( !fHistsNoCuts );
    assert( !fHistsPreSel );
    assert( !fHistsPreTag );
    assert( !fHistsAllCuts );

    for ( TIter next(fListOfSubselections); Subselection * subsel = static_cast<Subselection *>(next()); ) {
        assert( !fSubselection );
        fSubselection = subsel;
        assert( !subsel->fHistsAllCuts );
        assert( *subsel->GetName() );
        AtlHistogramTool * tool = new AtlHistogramTool(subsel->GetName(), "Histograms with all cuts");
        subsel->fHistsAllCuts = tool;
        AddTool(tool);
        BookHistogramsCommon(tool);
        BookHistogramsZ0(tool, "Z0_best");
        if ( subsel->fReconstructTop ) {
            BookHistogramsTop(tool);
        }
        fSubselection = 0;
    }
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Multi::FillHistograms() {
}

//____________________________________________________________________

void AtlSgTopFCNC_tZ_Multi::SlaveBegin(TTree * tree) {
    AtlSgTopFCNC_tZ_Base3L::SlaveBegin(tree);
    if ( AtlEvtWriterD3PDSgTopThinned * evtWriter = dynamic_cast<AtlEvtWriterD3PDSgTopThinned *>(fEvtWriter) ) {
        for ( TIter next(fListOfSubselections); Subselection * subsel = static_cast<Subselection *>(next()); ) {
            if ( subsel->fRecord ) {
                evtWriter->BookSubselectionFlag(subsel->GetName());
            }
        }
    }
}
