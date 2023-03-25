//___________________________________________________________
//
// A++ Object Definition and Correction Tool for Run 2
//
// Select objects (currenly leptons and jets) according to
// user-defined criteria, and apply corrections to the
// simulation.
//

#include "AtlObjectsDefinitionToolR2.h"

#include <cassert>
#include <cmath>

#include <TCollection.h>
#include <TList.h>
#include <TLorentzVector.h>

#include "AtlElectron.h"
#include "AtlEvent.h"
#include "AtlEventHeader.h"
#include "AtlJet.h"
#include "AtlMuon.h"
#include "AtlScaleFactor.h"


namespace {

template<typename _SpecialT, typename _GenericT>
_SpecialT polymorphic_downcast(_GenericT * x) {
    assert(dynamic_cast<_SpecialT>(x) == x);
    return static_cast<_SpecialT>(x);
}

}

//___________________________________________________________

AtlObjectsDefinitionToolR2::AtlObjectsDefinitionToolR2(char const * name, char const * title)
    : AtlObjectsDefinitionTool(name, title)
    , fElectronPt_min(-INFINITY)
    , fMuonPt_min(-INFINITY)
    , fJetEta_max(INFINITY)
    , fJetPt_min(-INFINITY)
    , fJetsAuthor(AtlJet::kInvalidType)
    , fBtagger(AtlBTag::kInvalidTagger)
    , fBtaggingWorkingPoint(NAN)
    , fBtaggingNominalEfficiency(-1)
    , fWeights(kAllSF)

    , fLeptons(new TList())
    , fElectrons(new TList())
    , fMuons(new TList())
    , fJets(new TList())
    , fBJets(new TList())

    , fClean(kTRUE)
{
}

//___________________________________________________________

AtlObjectsDefinitionToolR2::~AtlObjectsDefinitionToolR2() {
    delete fBJets;
    delete fJets;
    delete fMuons;
    delete fElectrons;
    delete fLeptons;
}

//___________________________________________________________

void AtlObjectsDefinitionToolR2::Clear() {
    //
    // Clear the event (but not the configuration!).
    //
    fLeptons->Clear();
    fElectrons->Clear();
    fMuons->Clear();
    fJets->Clear();
    fBJets->Clear();
    fClean = kTRUE;
}

//___________________________________________________________

Bool_t AtlObjectsDefinitionToolR2::AnalyzeEvent() {

/* check pre-conditions */
    assert(fClean);
    fClean = kFALSE;

    /* standard electron collection */
    for ( TIter next(fEvent->GetElectrons()); AtlElectron * lep = polymorphic_downcast<AtlElectron *>(next()); ) {
        if ( lep->P().Pt() >= fElectronPt_min ) {
            if ( lep->IsTight() ) {
                fElectrons->Add(lep);
                fLeptons->Add(lep);
            }
        }
    }

    /* standard lepton collection */
    for ( TIter next(fEvent->GetMuons()); AtlMuon * lep = polymorphic_downcast<AtlMuon *>(next()); ) {
        if ( lep->P().Pt() >= fMuonPt_min ) {
            if ( lep->IsTight() ) {
                fMuons->Add(lep);
                fLeptons->Add(lep);
            }
        }
    }

    /* standard (b-)jet collections */
    for ( TIter next(fEvent->GetJets(fJetsAuthor)); AtlJet * jet = polymorphic_downcast<AtlJet *>(next()); ) {
        TLorentzVector const & p = jet->P();
        if ( p.Pt() >= fJetPt_min && fabs(p.Eta()) < fJetEta_max ) {
            fJets->Add(jet);
            AtlBTag const * tag = (fBtagger != AtlBTag::kInvalidTagger ? jet->GetTag(fBtagger) : 0);
            if ( tag ) {
                if ( tag->GetWeight() > fBtaggingWorkingPoint ) {
                    fBJets->Add(jet);
                }
            }
        }
    }

    /* sort by transverse momentum */
    fMuons->Sort(kSortDescending);
    fElectrons->Sort(kSortDescending);
    fLeptons->Sort(kSortDescending);
    fJets->Sort(kSortDescending);
    fBJets->Sort(kSortDescending);

    /* do scale factors */
    AtlEventHeader & eventHeader = *fEvent->GetEventHeader();
    if ( eventHeader.IsMC() ) {
        double eventWeight = eventHeader.GetMCWeight();
        if ( (fWeights & kPileupSF) ) {
            eventWeight *= eventHeader.GetPileupWeight();
        }
        if ( (fWeights & kZVertexSF) ) {
            eventWeight *= eventHeader.GetZVtxWeight();
        }
        if ( (fWeights & kJetVertexSF) ) {
            eventWeight *= eventHeader.JetVertexWeight().GetValue();
        }
        if ( (fWeights & kLeptonSF) ) {
            if ( (fWeights & kLeptonSF) == kLeptonSF ) {
                eventWeight *= eventHeader.LeptonWeight().GetValue();
            }
            else {
                Fatal(__FUNCTION__, "Individual electron, muon and trigger weights are not supported -- use EWeights::kLepton.");
            }
        }
        eventHeader.SetPreTagEvtWeight(eventWeight);
        if ( (fWeights & kBtaggingSF) ) {
            eventWeight *= GetBtaggingSF().GetValue();
        }
        eventHeader.SetTagEvtWeight(eventWeight);
    }
    else {
        double eventWeight = 1.;
        eventHeader.SetPreTagEvtWeight(eventWeight);
        eventHeader.SetTagEvtWeight(eventWeight);
    }

    return kTRUE;
}

//___________________________________________________________

AtlScaleFactor const & AtlObjectsDefinitionToolR2::GetBtaggingSF() const {
    AtlEventHeader & eventHeader = *fEvent->GetEventHeader();
    switch ( fBtagger ) {

    case AtlBTag::kDL1r_85:
    	return eventHeader.BtagWeight_DL1r_85();
    	break;

    case AtlBTag::kMV2c10_60:
        if ( fJetsAuthor == AtlJet::kAntiKt4TopoEM )
            return eventHeader.BtagWeight_MV2c10_60();
        break;

    case AtlBTag::kMV2c10_70:
        if ( fJetsAuthor == AtlJet::kAntiKt4TopoEM )
            return eventHeader.BtagWeight_MV2c10_70();
        break;

    case AtlBTag::kMV2c10_77:
        if ( fJetsAuthor == AtlJet::kAntiKt4TopoEM )
            return eventHeader.BtagWeight_MV2c10_77();
        break;

    case AtlBTag::kMV2c10_85:
        if ( fJetsAuthor == AtlJet::kAntiKt4TopoEM )
            return eventHeader.BtagWeight_MV2c10_85();
        break;

    case AtlBTag::kMV2c20:
        switch ( fJetsAuthor ) {

        case AtlJet::kAntiKt4TopoEM:
            switch ( fBtaggingNominalEfficiency ) {
            case 77: return eventHeader.BtagWeight_MV2c20_77();
            case 85: return eventHeader.BtagWeight_MV2c20_85();
            }
            break;

        }
        break;

    }
    Fatal(__FUNCTION__, "b-tagging calibration for the chosen tagger and working point not available.");
    return AtlScaleFactor::nil;
}
