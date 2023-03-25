#include "AtlEvtWriterD3PDSgTopThinned.h"

#include <algorithm>
#include <cassert>
#include <cstddef>

#include <TList.h>
#include <TMap.h>
#include <TTree.h>

#include <AtlEvent.h>


AtlEvtWriterD3PDSgTopThinned::AtlEvtWriterD3PDSgTopThinned()
    : AtlEvtWriterBase()
    , fJets(0)
    , fBJets(0)
    , fLeptons(0)
    , fSubselectionFlags(new TMap)
    , v_jet_pt(new Float_t(kMaxJets))
    , v_jet_eta(new Float_t(kMaxJets))
    , v_jet_phi(new Float_t(kMaxJets))
    , v_jet_btag(new Int_t(kMaxJets))
    , v_lep_pt(new Float_t(kMaxLeptons))
    , v_lep_eta(new Float_t(kMaxLeptons))
    , v_lep_phi(new Float_t(kMaxLeptons))
    , v_lep_pdgId(new Int_t(kMaxLeptons))
{
    fSubselectionFlags->SetOwnerKeyValue();
}

AtlEvtWriterD3PDSgTopThinned::~AtlEvtWriterD3PDSgTopThinned() {
    delete v_jet_pt;
    delete v_jet_eta;
    delete v_jet_phi;
    delete v_jet_btag;
    delete v_lep_pt;
    delete v_lep_eta;
    delete v_lep_phi;
    delete v_lep_pdgId;
    delete fSubselectionFlags;
}

void AtlEvtWriterD3PDSgTopThinned::SetJets(TList * jets, TList * bjets) {
    fJets = jets;
    fBJets = bjets;
}

void AtlEvtWriterD3PDSgTopThinned::SetLeptons(TList * leptons) {
    fLeptons = leptons;
}

void AtlEvtWriterD3PDSgTopThinned::BookTree(TTree * tree, AtlEvent * evt) {
    fEvent = evt;
    fTree->Branch("eventNumber", &v_eventNumber, "eventNumber/L");
    fTree->Branch("channelNumber", &v_channelNumber, "channelNumber/I");
    fTree->Branch("evt_weight_mc", &v_evt_weight_mc, "evt_weight_mc/F");
    fTree->Branch("evt_weight_pretag", &v_evt_weight_pretag, "PreTagFullWeight/F");
    fTree->Branch("evt_weight_tag", &v_evt_weight_tag, "FullWeight/F");
    fTree->Branch("jet_n", &v_jet_n, "jet_n/I");
    fTree->Branch("jet_pt", v_jet_pt, "jet_pt[jet_n]/F");
    fTree->Branch("jet_eta", v_jet_eta, "jet_eta[jet_n]/F");
    fTree->Branch("jet_phi", v_jet_phi, "jet_phi[jet_n]/F");
    fTree->Branch("jet_btag", v_jet_btag, "jet_btag[jet_n]/I");
    fTree->Branch("lep_n", &v_lep_n, "lep_n/I");
    fTree->Branch("lep_pt", v_lep_pt, "lep_pt[lep_n]/F");
    fTree->Branch("lep_eta", v_lep_eta, "lep_eta[lep_n]/F");
    fTree->Branch("lep_phi", v_lep_phi, "lep_phi[lep_n]/F");
    fTree->Branch("lep_pdgId", v_lep_pdgId, "lep_pdgId[lep_n]/I");
    fTree->Branch("met_pt", &v_met_pt, "met_pt/F");
    fTree->Branch("met_phi", &v_met_phi, "met_phi/F");
}

namespace {
class TObjBool : public TObject {
public:
    Bool_t fValue;
};
}

void AtlEvtWriterD3PDSgTopThinned::BookSubselectionFlag(char const * name) {
    if ( !fTree )
        return;
    TObjBool * obj = new TObjBool;
    obj->fValue = kFALSE;
    fSubselectionFlags->Add(new TObjString(name), obj);
    fTree->Branch(Form("sel_%s", name), &obj->fValue, Form("sel_%s/O", name));
}

void AtlEvtWriterD3PDSgTopThinned::SetSubselectionFlag(char const * name, Bool_t value) {
    if ( !fTree )
        return;
    TObjBool * obj = static_cast<TObjBool *>(fSubselectionFlags->GetValue(name));
    assert( obj );
    obj->fValue = value;
}

TTree * AtlEvtWriterD3PDSgTopThinned::CreateTree(char const * name, char const * title, TTree * inputTree) {
    fTree = new TTree(name, title);
    fTree->SetAutoSave(0);
    return fTree;
}

template<typename _IterT, typename _ValueT>
static int index_of(_IterT begin, _IterT end, _ValueT value) {
    _IterT it = std::find(begin, end, value);
    return ( it != end ? it - begin : -1 );
}

void AtlEvtWriterD3PDSgTopThinned::WriteEvent() {

    /* event weight */
    AtlEventHeader const & eventHeader = *fEvent->GetEventHeader();
    if ( eventHeader.GetPileupWeight() == 0. )
        return;
    v_evt_weight_mc = eventHeader.GetMCWeight();
    v_evt_weight_pretag = eventHeader.GetPreTagEvtWeight();
    v_evt_weight_tag = eventHeader.GetTagEvtWeight();
    v_eventNumber = eventHeader.EventNr();
    v_channelNumber = eventHeader.GetChannelNumber();

    /* jets */
    size_t jet_i = 0;
    for ( TIter next(fJets); HepJet * jet = static_cast<HepJet *>(next()); ) {
        if ( !(jet_i < kMaxJets) )
            break;
        v_jet_pt[jet_i] = jet->Pt();
        v_jet_eta[jet_i] = jet->Eta();
        v_jet_phi[jet_i] = jet->Phi();
        v_jet_btag[jet_i] = ( fBJets ? ( fBJets->Contains(jet) ? 1 : 0 ) : -1 );
        ++jet_i;
    }
    v_jet_n = jet_i;

    /* leptons */
    size_t lep_i = 0;
    for ( TIter next(fLeptons); HepParticle * lep = static_cast<HepParticle *>(next()); ) {
        if ( !(lep_i < kMaxLeptons) )
            break;
        v_lep_pt[lep_i] = lep->Pt();
        v_lep_eta[lep_i] = lep->Eta();
        v_lep_phi[lep_i] = lep->Phi();
        v_lep_pdgId[lep_i] = lep->GetPdgCode();
        ++lep_i;
    }
    v_lep_n = lep_i;

    /* save energy sums */
    v_met_pt = fEvent->GetEnergySum()->MissingEt_Mag();
    v_met_phi = fEvent->GetEnergySum()->MissingEt_Phi();

    /* save! */
    fTree->Fill();
}
