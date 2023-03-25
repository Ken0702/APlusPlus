//____________________________________________________________________
//
// Single-top t-Channel Analysis
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_combinationAnalysis
#include <AtlSgTop_combinationAnalysis.h>
#endif
#include <iostream>
#include <AtlEvtWriterLPSC.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_combinationAnalysis);
#endif

//____________________________________________________________________

AtlSgTop_combinationAnalysis::AtlSgTop_combinationAnalysis(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
	fLepton    = 0;
    fLeptons   = 0;
    fElectrons = 0;
    fMuons     = 0;
    fLooseLeptons = 0;
    fJets      = 0;
    fBJets     = 0;
    fObjTool   = 0;
    fSFTool    = 0;
    //fWjetsSFTool = 0;
    fCutFlowTool   = 0;
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTop_combinationAnalysis::~AtlSgTop_combinationAnalysis() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::Clear( Option_t *option ) {
    //
    //  Clears the dynamically allocated objects
    //
	fLepton    = 0;
}


//____________________________________________________________________

void AtlSgTop_combinationAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
    /*fTree->SetBranchStatus("fEventHeader*", kTRUE);
    fTree->SetBranchStatus("fTopDecays*",   kTRUE);
    fTree->SetBranchStatus("fN_TopDecays*", kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",  kTRUE);
    fTree->SetBranchStatus("fVertices*",    kTRUE); 
    fTree->SetBranchStatus("fN_Electrons*",           kTRUE);
    fTree->SetBranchStatus("fElectrons*",             kTRUE);
    fTree->SetBranchStatus("fN_Muons*",               kTRUE);
    fTree->SetBranchStatus("fMuons*",                 kTRUE);
    fTree->SetBranchStatus("fEnergySum*",             kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4LCTopoJets*",  kTRUE);
    fTree->SetBranchStatus("fAntiKt4LCTopoJets*",  kTRUE);*/
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    fEvtWriter = new AtlEvtWriterLPSC;

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;
    fInputMode  = kD3PDSgTop_v2;
    fCounter = 0; //TEST
    // Cut defaults
    fMET_min    = 0.;
    fMtW_minPre = 0.;
    fMtW_min    = 0.;
    fPt_Jet_min = 0.;
    fPt_Lepton_min = 0.;
    fPt_ForwardJet_min = 0.;
    fEta_Jet_max = 1.e10;
    fDeltaEta_Jets_max = 1.e10;
    fNBTags_min = 0;
    fNBTags_max = 999;
    fNJets = -1;

    fLeadingJetIsBTagged = kFALSE;
    fAdditionalJetVeto = kFALSE;
    fVetoBTagger = AtlBTag::kMV1c;
    fVetoBTagWeight_min = 0.9195; // veto for t-channel SR
    fApplyDileptonVeto = kFALSE;
    fApplyBJetVeto = kFALSE;
    fApplySchannelVeto = kFALSE;
    fLeptonFlavour = kLepton;

    //BEY
    fMljb_min = 0.;      // efficient
    fMljb_max = 1.e10;   // very efficient
    fMjlfjb_min = 0.;    // efficient
    fMtW_max = 1.e10;    // medium efficient
    fHt_min = 0.;        // efficient
    // fHt_max = 1.e10;  // not efficient at all
    fPtJetlead_min = 0.; // efficient
    fMjlfjbHt_cut = kFALSE;
    //BEY

    fQCDVeto = kFALSE;
    Old = 0;
    New = 0;
    fMCTruthStudy = kFALSE;
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistograms() {
    //
    // Book histograms
    //    

    BookHistogramsPretag();
    BookHistogramsTag();
    if ( fMCTruthStudy )
	BookHistogramsMCTruthStudy();
    

    // BTag
    fHistsBTag = new AtlHistogramTool("bTagged", "B-tagged histograms");
    AddTool(fHistsBTag);
    BookHistogramsPreSel(fHistsBTag);
    BookHistogramsSel(fHistsBTag);

    // split up in muon and electron
    fHistsBTagElectron = new AtlHistogramTool("bTagged_el",
					      "B-tagged histograms in e channel");
    AddTool(fHistsBTagElectron);
    BookHistogramsPreSel(fHistsBTagElectron);
    BookHistogramsSel(fHistsBTagElectron);
    fHistsBTagMuon = new AtlHistogramTool("bTagged_mu",
					  "B-tagged histograms in mu channel");
    AddTool(fHistsBTagMuon);
    BookHistogramsPreSel(fHistsBTagMuon);
    BookHistogramsSel(fHistsBTagMuon);


    // Get ObjectSelection Tool + pointers to analysis objects
    fObjTool = 
	(AtlObjectsToolD3PDSgTop*)GetTool("AtlObjectsToolD3PDSgTop"
					  ,"",kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fLooseLeptons = fObjTool->GetLooseLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Provide tools with pointers to analysis objects
    fSFTool = 
	(AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool"
					    ,"",kTRUE);
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);

    // Provide tools with pointers to analysis objects
    /*fWjetsSFTool =
	(AtlWjetsScaleFactorTool*)GetTool("AtlWjetsScaleFactorTool"
					  ,"", kFALSE);
    if ( fWjetsSFTool != 0 )
	fWjetsSFTool->SetLeptons(fLeptons);*/


    // Get pointer to cut-flow tool
    fCutFlowTool = (AtlCutFlowTool*)GetTool("AtlCutFlowTool", "", kTRUE);

}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistogramsPreSel(AtlHistogramTool *htool) {
    //
    // Book event pre-selection histograms
    //

    // No. of events
    htool->Add("PreSel/h_presel_nevt", "No. of Pre-Selected Events", 1, 0, 1,
	       "Number of Entries", "PreSel");
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistogramsSel(AtlHistogramTool *htool){
    //
    // Book final event selection histograms
    //

    // No. of events
    htool->Add("Sel/h_sel_nevt", "No. of Selected Events", 1, 0, 1,
	       "Selection", "Number of Entries");

    //htool->Add("Sel/h_sel_njets", "No. of jets", 1, -0.5, 4.5,
    //	       "No. of Jets", "Number of Entries");

    htool->Add("Sel/h_sel_Ht", "Ht", 40, 0., 400.,
               "H_{T} [GeV]", "Number of Entries");

    // MET
    htool->Add("Sel/h_sel_met", "Missing E_T", 40, 0., 200.,
	       "E_{T, miss} [GeV]", "Number of Entries");

    // Mt_W
    htool->Add("Sel/h_sel_MtW", "Transverse W-Boson mass", 42, 0., 210.,
	       "M_{T, W} [GeV]", "Number of Entries");

    // 1st jet
    htool->Add("Sel/h_sel_LeadingJet_pt", "Leading Jet p_{T}", 60, 0., 300.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_even", "Leading Jet p_{T}, even events",
	       60, 0., 300., "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_odd", "Leading Jet p_{T}, odd events",
	       60, 0., 300., "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_eta", "Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_phi", "Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // 2nd jet
    htool->Add("Sel/h_sel_2ndLeadingJet_pt", "2nd Leading Jet p_{T}", 60, 0., 300.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingJet_eta", "2nd Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingJet_phi", "2nd Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // Special histograms for ttbar ctrl region
    // Bjets
    htool->Add("Sel/h_sel_LeadingBJet_pt", "Leading B-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingBJet_eta", "Leading B-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingBJet_phi", "Leading B-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_pt", "2nd Leading B-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_eta", "2nd Leading B-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_phi", "2nd Leading B-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    // Ljets
    htool->Add("Sel/h_sel_LeadingLJet_pt", "Leading L-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingLJet_eta", "Leading L-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingLJet_phi", "Leading L-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_pt", "2nd Leading L-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_eta", "2nd Leading L-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_phi", "2nd Leading L-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // Lepton
    htool->Add("Sel/h_sel_Lepton_pt", "Lepton p_{T}", 30., 0., 150.,
	       "Lepton p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_eta", "Lepton #eta", 20., -4., 4.,
	       "Lepton #eta", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_phi", "Lepton #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "Lepton #phi", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge", "Lepton Charge", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge_even", "Lepton Charge, even events", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge_odd", "Lepton Charge, odd events", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");

    // Leading jet pt for 5 eta bins
    htool->Add("Sel/h_sel_LeadingJet_pt_eta1", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta2", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta3", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta4", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta5", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");

    // Delta R
    htool->Add("Sel/h_sel_DeltaR_Lepton_LeadingJet", "#DeltaR(lep, leading jet)", 36., 0.4, 4.,
	       "#DeltaR(lepton, leading jet)", "Number of Entries");
    htool->Add("Sel/h_sel_DeltaR_Lepton_2ndLeadingJet","#DeltaR(lep, 2nd leading jet)",36., 0.4, 4.,
	       "#DeltaR(lepton, 2nd leading jet)", "Number of Entries");
    htool->Add("Sel/h_sel_DeltaR_LeadingJet_2ndLeadingJet", "#DeltaR(leading jet, 2nd leading jet)",
	       36., 0.4, 4., "#DeltaR(leading jet, 2nd leading jet)", "Number of Entries");

    //Inv. mass
    htool->Add("Sel/h_sel_InvM_Lepton_LeadingJet", "Inv. mass (lepton, leading jet)", 50., 0., 400.,
	       "Inv. mass (lepton, j_{1}) [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_InvM_LeadingJet_2ndLeadingJet", "Inv. mass (leading jet, 2nd leading jet)",
	       50., 0., 400., "Inv. mass (j_{1}, j_{2}) [GeV]", "Number of Enrties");

    // Pt_lepton * Et_miss
    htool->Add("Sel/h_sel_ScalarProd_Lepton_Etmiss","P_{Lepton} * E_{T, miss} [GeV^{2}]", 120., -5000., 10000.,
	      "#vec{p}_{lepton}#vec{E}_{T, miss}", "Number of Entries");

    // QCD Veto
    htool->Add("Sel/h_sel_QCDVeto", "QCD Veto", 20., -TMath::Pi(), TMath::Pi(), 20., 0., 100., "#Delta#phi(j_{1},l)", "Lepton p_{T} (GeV)", "Events");
}

//____________________________________________________________________



Bool_t AtlSgTop_combinationAnalysis::DiLeptonVeto() {
    //
    // Dilepton veto (against ttbar dilepton bkg)
    //
    // A return value of 'true' means the veto is fulfilled, ie. the
    // event ought to be discarded.
    //
    // Note: for run-1 events, the veto uses loose lepton information,
    // which is at present not available for run-2 events. Therefore
    // here the tight leptons are used for the moment.
    //
    if ( fEvent->IsRun1() ) { //was run1
        Int_t Ntight = fLeptons->GetEntries();
        Int_t Nloose = fLooseLeptons->GetEntries();
        if ( Ntight + Nloose > 1 ) return kTRUE;
    } else  if ( fEvent->IsRun2() ) {
	Int_t N_Electron = fEvent->GetN_Electrons();
	Int_t N_Muon = fEvent->GetN_Muons();
	if ( N_Electron + N_Muon > 1 ) return kTRUE;
    } else {
	Fatal(__FUNCTION__, "Unknown run period. Abort!");
	gSystem->Abort(1);
    }
    return kFALSE;
}


Bool_t AtlSgTop_combinationAnalysis::ProcessPreCut() {
    //
    // Event pre-selection
    //
	//std::cout<<" PreCuts "<<std::endl;
    /*fLeptons->ls();
	if ( fLeptons->GetEntries() != 1 ) {
		    return kFALSE;
		}
	else {
		    fLepton = (HepParticle*)fLeptons->At(0);
		}

	 fCutFlowTool->Fill("1Lepton", GetTagEvtWeight());
*/

	// Apply second Pt cut and eta cut
	    TIter next_jet(fJets);
	    AtlJet *jet = 0;
	    while ( (jet = (AtlJet*)next_jet()) ) {
	     	if ( jet->Pt() < 0 ) return kFALSE;

	     	if ( TMath::Abs(jet->Eta()) > 1000000000) return kFALSE;

	     	if(TMath::Abs(jet->Eta()) > 2.3) { // Stronger Cut on forward Jets
	     	    if(jet->Pt() < fPt_ForwardJet_min){

	     	    	return kFALSE;
	     	    }
	     	}
	    }
	fCutFlowTool->Fill("Forward Jet Pt", GetTagEvtWeight());

    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( fMET < fMET_min ) return kFALSE;
    fCutFlowTool->Fill("MET_Min", GetTagEvtWeight());


    // Transverse W mass
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));

    if ( fMtW < fMtW_minPre ) return kFALSE;
    fLepton = (HepParticle*)fLeptons->At(0);
    fCutFlowTool->Fill("MtW_Min", GetTagEvtWeight());


    // Require lepton flavour. Needed for AntiMuon sample which includes electrons.
    if ( fLeptonFlavour != kLepton) {
            if (( fLepton->IsElectron() && fLeptonFlavour != kElectron) ||
                ( fLepton->IsMuon() && fLeptonFlavour != kMuon )) {
                return kFALSE;
            }
        }

    fCutFlowTool->Fill("LepFlav", GetTagEvtWeight());


    // Accept event

    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_combinationAnalysis::ProcessCut() {
    //
    // Event selection
    //
	//std::cout<<"TChannel Cuts"<<std::endl;
	//std::cout<<"WEIGHTCUT"<<fSFTool->GetBtagSF()<<std::endl;
	/*if(fSFTool->GetBtagSF()<0.94){
		New++;
		std::cout<<"HELLO"<<New<<std::endl;
		return kFALSE;
	}*/


    // ===========
    // No. of jets
    // ===========

    fCutFlowTool->Fill("ProcessCut", GetTagEvtWeight());
    // Don't allow additional jets outside the given pt-eta range
    if ( fAdditionalJetVeto ) {
    	if ( fEvent->GetN_Jets(fObjTool->fJetsAuthor) > fJets->GetEntries() )
    	    return kFALSE;
    }
    fCutFlowTool->Fill("AddJetVeto", GetTagEvtWeight());

    if(fJets->GetEntries() != fNJets && fNJets > 0) return kFALSE;
    fCutFlowTool->Fill("NJets", GetTagEvtWeight());
    //tchan 1% here

    // Apply second Pt cut and eta cut
    TIter next_jet(fJets);
    AtlJet *jet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
     	if ( jet->Pt() < fPt_Jet_min ) return kFALSE;

     	if ( TMath::Abs(jet->Eta()) > fEta_Jet_max) return kFALSE;

     	if(TMath::Abs(jet->Eta()) > 2.3) { // Stronger Cut on forward Jets
     	    if(jet->Pt() < fPt_ForwardJet_min){

     	    	return kFALSE;
     	    }
     	}
    }

    fCutFlowTool->Fill("EtaPt", GetTagEvtWeight());

    // Compute Ht
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    fHt = fMET + lepton->Pt();
    next_jet.Reset();
    while ( (jet = (AtlJet*)next_jet()) ) {
	fHt += jet->Pt();
    }


    fCutFlowTool->Fill("Ht", GetTagEvtWeight());

    // Fill Pretag-Histograms
   // FillHistogramsPretag();

    // Count no. of pre-tagged events

    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();

    // =========
    // B-tagging
    // =========
    //fBJets->Print();


    // No. of b-tagged jets
    if ( fBJets->GetEntries() < fNBTags_min
	 || fBJets->GetEntries() > fNBTags_max) return kFALSE;
    fCutFlowTool->Fill("Num bTags", GetTagEvtWeight());

    // Require the leading jet to be b-tagged ?
    if ( fLeadingJetIsBTagged == kTRUE
    	 && fBJets->At(0) != fJets->At(0) ) return kFALSE;

    fCutFlowTool->Fill("Leading Jet BTag", GetTagEvtWeight());
    // B-tag veto (for wjets ctrl region)

    /*if ( fApplyBJetVeto ) {

     	// All events in wjets ctrl region contain 2 jets
	// If exactly one of the jets is tagged with the
	// SR tagger (i.e. MV1c@50%) the event is discarded
     	AtlJet *jet = 0;
     	Int_t NTagsMV1c = 0;
	
	next_jet.Reset();
	while ( (jet = (AtlJet*)next_jet()) ) {
	    if ( jet->GetTag(fVetoBTagger)->GetWeight() > fVetoBTagWeight_min &&
		 TMath::Abs(jet->Eta()) < 2.5 ) {
		NTagsMV1c++;
	    }
	}
	if ( NTagsMV1c == 1 ) return kFALSE;
    }*/
    if ( fApplyBJetVeto ) {
		TIter next_bjet(fBJets);
		AtlJet *bjet = 0;
		Bool_t IsVeto = ( fBJets->GetEntries() == fJets->GetEntries() );
		while ( (bjet = (AtlJet*)next_bjet()) ) {
		IsVeto &= ( bjet->GetTag(fVetoBTagger)->GetWeight()
				> fVetoBTagWeight_min );
		   }
		   if ( IsVeto ){

			   return kFALSE;
		   }
    }
    fCutFlowTool->Fill("BTagVeto", GetTagEvtWeight());


    if ( fApplySchannelVeto ) {
        // s|t-channel combination
	AtlBTag::ETagger schan_tagger = AtlBTag::kMV1;
	Double_t schan_wp = 0.7892;
	
	AtlJet *jet1 = (AtlJet*) fJets->At(0);
	AtlJet *jet2 = (AtlJet*) fJets->At(1);
	if ( jet1->GetTag(schan_tagger)->GetWeight() > schan_wp &&
	     jet2->GetTag(schan_tagger)->GetWeight() > schan_wp )
	    return kFALSE;
    }
	
    // ===============
    // Final selection
    // ===============

    //BEY - Comute some variables for cuts

    // Compute m(jlfjb)
    AtlJet *bJet = (AtlJet*)fBJets->At(0);

    AtlJet *lJet = 0;

    next_jet.Reset();

    while ( (jet = (AtlJet*)next_jet()) ) {

	if (jet != bJet) {

	    // Taking highest Pt light Jet
	    lJet = jet;
	    break;
	}
    }

    if(! bJet) bJet = lJet; //Fix for no Bjet cut.
    //cout<<bJet->P().M()<<endl;

     fMjlfjb = (bJet->P() + lJet->P()).M();


    // Compute Eta(jfj)Q(l)
    fEtajlf = (TMath::Abs( lJet->Eta()));

    // Compute Eta(jlf)Q(l)
    fEtajlfQl = (TMath::Abs(lJet->Eta()))*lepton->Charge();

    // Compute m(ljb) 
    fMljb = (bJet->P() + lepton->P()).M();

    // Compute DeltaEta(ljlf)
    fDeltaEtaljlf = lepton->DeltaEta(lJet);

    // Compute DeltaPhi(ljlf)
    fDeltaPhiljlf = lepton->DeltaPhi(lJet);

    // Compute DeltaR(ljlf)
    fDeltaRljlf = lepton->DeltaR(lJet);

    // Compute DeltaR(ljb)
    fDeltaRljb = lepton->DeltaR(bJet);

    // Compute DeltaR(jlfjb)
    fDeltaRjlfjb = lJet->DeltaR(bJet);

    // Compute CosTheta(ljlf)
    TVector3 lepp3 = lepton->P3();
    TVector3 ljetp3 = lJet->P3();
    fCosThetaljlf = lepp3.Dot(ljetp3)/(lepp3.Mag()*ljetp3.Mag());

    // BEY - cut on variables

    // cut on transverse momentum pt of leading jet
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    
    if (Jetlead->Pt() < fPtJetlead_min ) return kFALSE;

    // cut on invariant mass of the lepton (l) and the b-tagged jet (jb)
    if ( fMljb < fMljb_min ) return kFALSE; 
    if ( fMljb > fMljb_max ) return kFALSE;

    // cut on Ht
    if ( fHt < fHt_min ) return kFALSE; // 

    // cut on invariant mass of the light quark jet (jlf) and the b-tagged jet (jb)
    if ( fMjlfjb < fMjlfjb_min ) return kFALSE; 

    // 2-dim cuts on Mjlfjb vs Ht
    if ( fMjlfjbHt_cut) {
	if ( fMjlfjb < ((800./760.*fHt) - (240.*800./760.)))
	    return kFALSE;
    }
    

    // Final transverse W mass cut
    if ( fMtW < fMtW_min ) return kFALSE;
    fCutFlowTool->Fill("MtW min", GetTagEvtWeight());
    if ( fMtW > fMtW_max ) return kFALSE;
    		/*
    		//Old = 0;
    		//New = 0;
    		int Diff = 0;
            Int_t Ntight = fLeptons->GetEntries();
            Int_t Nloose = fLooseLeptons->GetEntries();
            if ( Ntight + Nloose > 1 ) {

            	//Old++;
            }

        	Int_t N_Electron = fEvent->GetN_Electrons();
        	Int_t N_Muon = fEvent->GetN_Muons();
        	if ( N_Electron + N_Muon > 1 ) {

        		//New++;
        	}
        	Diff = Old - New;
        	Diff = 0;
        	if(Diff !=0 ){
        		std::cout<<Ntight<<"  "<<Nloose<<"  "<<N_Electron<<"  "<<N_Muon<<"  "<<std::endl;
        		Diff = 0;
        		std::cout<<"DIFF"<<std::endl;
        		fEvent->Print("ALL");
        		if(fEvent->IsRun2()) std::cout<<"RUN2"<<std::endl;
        		if(fEvent->IsRun1()) std::cout<<"RUN1"<<std::endl;

        	}
        	*/

    // Dilepton cut
    if ( fApplyDileptonVeto ) {
    	if ( DiLeptonVeto() ) return kFALSE;

    }
    fCutFlowTool->Fill("DiLepVeto", GetTagEvtWeight());


    if ( fQCDVeto ) {
		Double_t DeltaPhi = TMath::Abs(lepton->DeltaPhi(Jetlead));
		//Double_t qcdvetoold  = 40.*(1. - (TMath::Pi() - DeltaPhi)/(TMath::Pi() - 1));
		Double_t qcdveto  = 40. * DeltaPhi/TMath::Pi() ;
		//std::cout<<qcdvetoold<<"ccc"<<qcdveto<<"AAAAAAAAAA"<<DeltaPhi<<"AAAAAAAAAAAAAAA"<<lepton->DeltaPhi(Jetlead)<<std::endl;
	if ( lepton->Pt() < qcdveto ){

	    return kFALSE;
	}


     }

    fCutFlowTool->Fill("QCDVeto", GetTagEvtWeight());

    Double_t DeltaEta = TMath::Abs(bJet->Eta() - lJet->Eta());
    if ( DeltaEta > fDeltaEta_Jets_max )
    	return kFALSE;
	



    fCutFlowTool->Fill("Final", GetTagEvtWeight());





    FillHistogramsSel(fHistsBTag, GetTagEvtWeight());
    if ( fLepton->IsElectron() ) {
    	FillHistogramsSel(fHistsBTagElectron, GetTagEvtWeight());
    }
    else {
    	FillHistogramsSel(fHistsBTagMuon, GetTagEvtWeight());
    }

    /*cout<<"ANALYSISCOUNTER"<<GetTagEvtWeight()<<endl;
    cout<<"ANALYSISCOUNTER"<<GetPreTagEvtWeight()<<endl;
    //fEvent->Print();
    cout<<"RunNr: "<<fEvent->GetEventHeader()->RunNr()<<endl;
    cout<<"EventNr: "<<fEvent->GetEventHeader()->EventNr()<<endl;
    cout<<"MC weight: "<<fEvent->GetMCWeight()<<endl;
    cout<<"weight_pileup: "<<fEvent->GetPileupWeight()<<endl;
    cout<<"weight_leptonSF: "<<fEvent->GetEventHeader()->LeptonWeight().GetValue()<<endl;
    cout<<"weight_bTagSF: "<<fEvent->GetEventHeader()->BtagWeight_MV2c10_60().GetValue()<<endl;
    cout<<"weight_jvt: "<<fEvent->GetEventHeader()->JetVertexWeight().GetValue()<<endl;

    cout<<"weight_ZVertex: "<<fEvent->GetZVtxWeight()<<endl;
    cout<<"MULTIPLY"<<fEvent->GetMCWeight()*fEvent->GetPileupWeight()*fEvent->GetEventHeader()->LeptonWeight().GetValue()*fEvent->GetEventHeader()->BtagWeight_MV2c10_60().GetValue()*
    		fEvent->GetEventHeader()->JetVertexWeight().GetValue()<<endl;*/

    return kTRUE;

}

//____________________________________________________________________



void AtlSgTop_combinationAnalysis::FillHistogramsSel(AtlHistogramTool *htool, Double_t W){
    //
    // Fill histograms for final event selection
    //

    HepParticle *lepton= fLepton;

    AtlJet *Jetlead = (AtlJet*)fJets->At(0);
    AtlJet *Jet2ndlead = (AtlJet*)fJets->At(1);

    htool->Fill("Sel/h_sel_nevt", 0.5, W);
    //htool->Fill("Sel/h_sel_njets", fJets->GetEntries(), W);
    htool->Fill("Sel/h_sel_Lepton_pt",  lepton->Pt(), W);
    htool->Fill("Sel/h_sel_Lepton_eta", lepton->Eta(), W);
    htool->Fill("Sel/h_sel_Lepton_phi", lepton->Phi(), W);
    htool->Fill("Sel/h_sel_Lepton_charge", lepton->Charge(), W);
    htool->Fill("Sel/h_sel_Ht", fHt, W);
    htool->Fill("Sel/h_sel_met", fMET, W);
    htool->Fill("Sel/h_sel_MtW", fMtW, W);
    htool->Fill("Sel/h_sel_LeadingJet_pt",Jetlead->Pt(), W);
    htool->Fill("Sel/h_sel_LeadingJet_eta",Jetlead->Eta(), W);
    htool->Fill("Sel/h_sel_LeadingJet_phi",Jetlead->Phi(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_pt",Jet2ndlead->Pt(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_eta",Jet2ndlead->Eta(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_phi",Jet2ndlead->Phi(), W);

    // even/odd events
    if ( fEvent->EventNr() % 2 == 0 ) {
	htool->Fill("Sel/h_sel_LeadingJet_pt_even", Jetlead->Pt(), W);
	htool->Fill("Sel/h_sel_Lepton_charge_even", lepton->Charge(), W);
    } else {
	htool->Fill("Sel/h_sel_LeadingJet_pt_odd", Jetlead->Pt(), W);
	htool->Fill("Sel/h_sel_Lepton_charge_odd", lepton->Charge(), W);
    }

    // Special histograms for ttbar ctrl region
    // make sure there are exactly 2 btags (counter example QCDJetLepton)
    if ( fJets->GetEntries() == 4 && fNBTags_min == 2 && fNBTags_max == 2) {
	AtlJet *BJetlead = (AtlJet*) fBJets->At(0);
	AtlJet *BJet2ndlead = (AtlJet*) fBJets->At(1);
	TIter next_ljet(fJets);
	AtlJet *jet = 0;
	AtlJet *LJetlead = 0;
	AtlJet *LJet2ndlead = 0;
	while ( (jet = (AtlJet*) next_ljet()) ) {
	    if ( jet != BJetlead && jet != BJet2ndlead ) {
		if ( LJetlead == 0 )
		    LJetlead = jet;
		else
		    LJet2ndlead = jet;
	    }
	}
	htool->Fill("Sel/h_sel_LeadingBJet_pt",BJetlead->Pt(), W);
	htool->Fill("Sel/h_sel_LeadingBJet_eta",BJetlead->Eta(), W);
	htool->Fill("Sel/h_sel_LeadingBJet_phi",BJetlead->Phi(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_pt",BJet2ndlead->Pt(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_eta",BJet2ndlead->Eta(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_phi",BJet2ndlead->Phi(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_pt",LJetlead->Pt(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_eta",LJetlead->Eta(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_phi",LJetlead->Phi(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_pt",LJet2ndlead->Pt(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_eta",LJet2ndlead->Eta(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_phi",LJet2ndlead->Phi(), W);
    }

    // Eta Bins:
    // [-2.5, -2.], ]-2., -1.], ]-1., 1], ]1., 2.], ]2., 2.5]
    if ( -2.5 <= Jetlead->Eta() && Jetlead->Eta() <= -2. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta1",Jetlead->Pt(), W);
    else if (  -2. <  Jetlead->Eta() && Jetlead->Eta() <= -1. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta2",Jetlead->Pt(), W);
    else if (  -1. <  Jetlead->Eta() && Jetlead->Eta() <= +1. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta3",Jetlead->Pt(), W);
    else if (  +1. <  Jetlead->Eta() && Jetlead->Eta() <= +2. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta4",Jetlead->Pt(), W);
    else if (  +2. <  Jetlead->Eta() && Jetlead->Eta() <= +2.5 )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta5",Jetlead->Pt(), W);

    // Delta R
    htool->Fill("Sel/h_sel_DeltaR_Lepton_LeadingJet", lepton->DeltaR(Jetlead), W);
    htool->Fill("Sel/h_sel_DeltaR_Lepton_2ndLeadingJet", lepton->DeltaR(Jet2ndlead), W);
    htool->Fill("Sel/h_sel_DeltaR_LeadingJet_2ndLeadingJet", Jetlead->DeltaR(Jet2ndlead), W);

    // Inv. mass
    TLorentzVector p_lep_j1 = lepton->P() + Jetlead->P();
    htool->Fill("Sel/h_sel_InvM_Lepton_LeadingJet",p_lep_j1.M(), W);

    TLorentzVector p_j1_j2 = Jetlead->P() + Jet2ndlead->P();
    htool->Fill("Sel/h_sel_InvM_LeadingJet_2ndLeadingJet",p_j1_j2.M(), W);

    // Pt_lepton * Et_miss
    TVector2 vec_met = fEvent->GetEnergySum()->GetMissingEt();
    TVector3 vec_lep = lepton->P3();
    htool->Fill("Sel/h_sel_ScalarProd_Lepton_Etmiss",vec_met*(vec_lep.XYvector()), W );

   //QCD Veto
   htool->Fill("Sel/h_sel_QCDVeto", lepton->DeltaPhi(Jetlead), lepton->Pt(), W);

}

//____________________________________________________________________


void AtlSgTop_combinationAnalysis::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    //FillHistogramsTag();

    if ( fMCTruthStudy ) {
	FillHistogramsMCTruthStudy();
    }
    
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::InitEvent() {
    //
    // Prepare event analysis
    //

    // Provided by tools or 
    // set in ProcessPreCut()
    // ------------------------------
    //  - fElectrons
    //  - fMuons
    //  - fLeptons
    //  - fBJets
    //  - fJets
    //  - fMtW

    // ============================
    // Send Objects to finder tools
    // ============================

    // Single-top t-channel Finder
    // fSgTop_tFinderTool->SetEtmiss(fMET);
    // fSgTop_tFinderTool->SetLeptons(fLeptons);
    // fSgTop_tFinderTool->SetJets(fJets);
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top t-Channel Analysis Selector          " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << "  fMET_min      = " << fMET_min << " GeV" << endl
	 << "  fMtW_min      = " << fMtW_min << " GeV" << endl
	 << endl
	 << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
	//dynamic_cast<AtlEvtWriterLPSC*>(fEvtWriter)->SetXsection(fXsection);
	//dynamic_cast<AtlEvtWriterLPSC*>(fEvtWriter)->SetNEvents(GetInitialNumberOfEvents());
    AtlSelector::Terminate();
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::FillHistogramsPretag() {
    //
    // Fill pretag Histograms
    //
	
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);

    //BEYS 1-D histograms
    
    fHist_Pretag_Ht->Fill(fHt, GetPreTagEvtWeight());
    
    //BEYE 1-D histograms

    
    fHist_Pretag_met->Fill(fEvent->GetEnergySum()->MissingEt_Mag(), GetPreTagEvtWeight());
    fHist_Pretag_MtW->Fill(fMtW, GetPreTagEvtWeight());

    fHist_Pretag_LeadingJet_pt->Fill(Jetlead->Pt(), GetPreTagEvtWeight());
    fHist_Pretag_LeadingJet_eta->Fill(Jetlead->Eta(), GetPreTagEvtWeight());
    fHist_Pretag_LeadingJet_phi->Fill(Jetlead->Phi(), GetPreTagEvtWeight());

    fHist_Pretag_2ndLeadingJet_pt->Fill( Jet2ndlead->Pt(), GetPreTagEvtWeight());
    fHist_Pretag_2ndLeadingJet_eta->Fill(Jet2ndlead->Eta(), GetPreTagEvtWeight());
    fHist_Pretag_2ndLeadingJet_phi->Fill(Jet2ndlead->Phi(), GetPreTagEvtWeight());
    
    fHist_Pretag_Lepton_pt->Fill(lepton->Pt(), GetPreTagEvtWeight());
    fHist_Pretag_Lepton_eta->Fill(lepton->Eta(), GetPreTagEvtWeight());
    fHist_Pretag_Lepton_phi->Fill(lepton->Phi(), GetPreTagEvtWeight());

    // QCD Multijet veto
    fHist_Pretag_QCDVeto->Fill(lepton->DeltaPhi(Jetlead),
			       lepton->Pt(),
			       GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::FillHistogramsTag() {
    //
    // Fill tag Histograms
    //

    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);

    TVector2 met = fEvent->GetEnergySum()->GetMissingEt();
    TVector2 lep = lepton->P3().XYvector();
    Double_t Pt_W = (met+lep).Mod();

    // BEYS 1-D histograms
    fHist_Tag_UwHt->Fill(fHt, 1); // unweighted histrogram in order to count the unweighted number of events 
    fHist_Tag_Ht->Fill(fHt, GetTagEvtWeight());
    fHist_Tag_Etajlf->Fill(fEtajlf, GetTagEvtWeight());
    fHist_Tag_EtajlfQl->Fill(fEtajlfQl, GetTagEvtWeight());
    fHist_Tag_Mjlfjb->Fill(fMjlfjb, GetTagEvtWeight());
    fHist_Tag_Mljb->Fill(fMljb, GetTagEvtWeight());
    fHist_Tag_DeltaEtaljlf->Fill(fDeltaEtaljlf, GetTagEvtWeight());
    fHist_Tag_DeltaPhiljlf->Fill(fDeltaPhiljlf, GetTagEvtWeight());
    fHist_Tag_DeltaRljlf->Fill(fDeltaRljlf, GetTagEvtWeight());
    fHist_Tag_DeltaRljb->Fill(fDeltaRljb, GetTagEvtWeight());
    fHist_Tag_DeltaRjlfjb->Fill(fDeltaRjlfjb, GetTagEvtWeight());
    fHist_Tag_CosThetaljlf->Fill(fCosThetaljlf, GetTagEvtWeight());
    //BEYE 1-D histograms

    fHist_Tag_met->Fill(fEvent->GetEnergySum()->MissingEt_Mag(), GetTagEvtWeight());
    fHist_Tag_MtW->Fill(fMtW, GetTagEvtWeight());

    fHist_Tag_PtW->Fill(Pt_W, GetTagEvtWeight());

    fHist_Tag_LeadingJet_pt->Fill(Jetlead->Pt(), GetTagEvtWeight());
    fHist_Tag_LeadingJet_eta->Fill(Jetlead->Eta(), GetTagEvtWeight());
    fHist_Tag_LeadingJet_phi->Fill(Jetlead->Phi(), GetTagEvtWeight());

    fHist_Tag_2ndLeadingJet_pt->Fill( Jet2ndlead->Pt(), GetTagEvtWeight());
    fHist_Tag_2ndLeadingJet_eta->Fill(Jet2ndlead->Eta(), GetTagEvtWeight());
    fHist_Tag_2ndLeadingJet_phi->Fill(Jet2ndlead->Phi(), GetTagEvtWeight());

    AtlJet *bJet = (AtlJet*)fBJets->At(0);
    AtlJet *lJet = 0;
    AtlJet *jet = 0;
    TIter next_jet(fJets);
    while ( (jet = (AtlJet*)next_jet()) ) {
	if (jet != bJet) {
	    // Taking highest Pt light Jet
	    // for ttbar selection, this can be a tagged jet as well
	    lJet = jet;
	    break;
	}
    }

    fHist_Tag_BJet_pt->Fill(bJet->Pt(), GetTagEvtWeight());
    fHist_Tag_BJet_eta->Fill(bJet->Eta(), GetTagEvtWeight());
    fHist_Tag_BJet_phi->Fill(bJet->Phi(), GetTagEvtWeight());

    fHist_Tag_LJet_pt->Fill( lJet->Pt(), GetTagEvtWeight());
    fHist_Tag_LJet_eta->Fill(lJet->Eta(), GetTagEvtWeight());
    fHist_Tag_LJet_phi->Fill(lJet->Phi(), GetTagEvtWeight());

    fHist_Tag_Lepton_pt->Fill(lepton->Pt(), GetTagEvtWeight());
    fHist_Tag_Lepton_eta->Fill(lepton->Eta(), GetTagEvtWeight());
    fHist_Tag_Lepton_phi->Fill(lepton->Phi(), GetTagEvtWeight());

    //BEYS 2-D histograms
    fHist_Tag_MjlfjbHt->Fill(fHt, fMjlfjb, GetTagEvtWeight());
    fHist_Tag_MljbHt->Fill(fHt, fMljb, GetTagEvtWeight());
    fHist_Tag_MjlfjbMljb->Fill(fMljb, fMjlfjb, GetTagEvtWeight());
    fHist_Tag_MjlfjbMtW->Fill(fMtW, fMjlfjb, GetTagEvtWeight());
    fHist_Tag_MljbMtW->Fill(fMtW, fMljb, GetTagEvtWeight());
    fHist_Tag_MjlfjbEtajlf->Fill(fEtajlf, fMjlfjb, GetTagEvtWeight());
    fHist_Tag_MljbEtajlf->Fill(fEtajlf, fMljb, GetTagEvtWeight());
    //BEYE 2-D histogram
    
    // QCD Multijet veto
    fHist_Tag_QCDVeto->Fill(lepton->DeltaPhi(Jetlead),
			    lepton->Pt(),
			    GetTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistogramsPretag() {
    //
    // Book pretag histograms
    //    

    gDirectory->mkdir("Pretag");
    gDirectory->cd("Pretag");

    // BEYS 1-D histograms
    //  Bins?
   fHist_Pretag_Ht = new TH1F("h_pretag_Ht", "H_{T}, pretag",
			       40, 0., 400.);
    
    fHist_Pretag_Ht->SetXTitle("H_{T} (GeV)");
    fHist_Pretag_Ht->SetYTitle("Events");

    // BEYE 1-D histograms
    
    fHist_Pretag_met = new TH1F("h_pretag_met", "Missing E_{T}, pretag",
				40, 0., 200.);
    fHist_Pretag_met->SetXTitle("E_{T, miss} (GeV)");
    fHist_Pretag_met->SetYTitle("Events");
    
    fHist_Pretag_MtW = new TH1F("h_pretag_MtW", "Transverse W-Boson mass, pretag",
				30, 0., 150);
    fHist_Pretag_MtW->SetXTitle("M_{T, W} (GeV)");
    fHist_Pretag_MtW->SetYTitle("Events");

    fHist_Pretag_LeadingJet_pt  = new TH1F("h_pretag_LeadingJet_pt", "Leading Jet p_{T}, pretag",
					  40, 0., 200.);
    fHist_Pretag_LeadingJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Pretag_LeadingJet_pt->SetYTitle("Events");

    fHist_Pretag_LeadingJet_eta = new TH1F("h_pretag_LeadingJet_eta", "Leading Jet #eta, pretag",
					  20, -5., 5.);
    fHist_Pretag_LeadingJet_eta->SetXTitle("#eta");
    fHist_Pretag_LeadingJet_eta->SetYTitle("Events");

    fHist_Pretag_LeadingJet_phi = new TH1F("h_pretag_LeadingJet_phi", "Leading Jet #phi, pretag",
					  20, -TMath::Pi(), TMath::Pi());
    fHist_Pretag_LeadingJet_phi->SetXTitle("#phi");
    fHist_Pretag_LeadingJet_phi->SetYTitle("Events");

    fHist_Pretag_2ndLeadingJet_pt  = new TH1F("h_pretag_2ndLeadingJet_pt", "2nd Leading Jet p_{T}, pretag",
					     40., 0., 200.);
    fHist_Pretag_2ndLeadingJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Pretag_2ndLeadingJet_pt->SetYTitle("Events");

    fHist_Pretag_2ndLeadingJet_eta = new TH1F("h_pretag_2ndLeadingJet_eta", "2nd Leading Jet #eta, pretag",
					     20., -5., 5.);
    fHist_Pretag_2ndLeadingJet_eta->SetXTitle("#eta");
    fHist_Pretag_2ndLeadingJet_eta->SetYTitle("Events");

    fHist_Pretag_2ndLeadingJet_phi = new TH1F("h_pretag_2ndLeadingJet_phi", "2nd Leading Jet #phi, pretag",
					     20., -TMath::Pi(), TMath::Pi());
    fHist_Pretag_2ndLeadingJet_phi->SetXTitle("#phi");
    fHist_Pretag_2ndLeadingJet_phi->SetYTitle("Events");

    fHist_Pretag_Lepton_pt  = new TH1F("h_pretag_Lepton_pt", "Lepton p_{T}, pretag",
				      40., 0., 200.);
    fHist_Pretag_Lepton_pt->SetXTitle("p_{T} (GeV)");
    fHist_Pretag_Lepton_pt->SetYTitle("Events");

    fHist_Pretag_Lepton_eta = new TH1F("h_pretag_Lepton_eta", "Lepton #eta, pretag",
				      20., -2.5, 2.5);
    fHist_Pretag_Lepton_eta->SetXTitle("#eta");
    fHist_Pretag_Lepton_eta->SetYTitle("Events");

    fHist_Pretag_Lepton_phi = new TH1F("h_pretag_Lepton_phi", "Lepton #phi, pretag",
				      20., -TMath::Pi(), TMath::Pi());
    fHist_Pretag_Lepton_phi->SetXTitle("#phi");
    fHist_Pretag_Lepton_phi->SetYTitle("Events");

    fHist_Pretag_QCDVeto = new TH2F("h_pretag_QCDVeto", "QCD Veto",
				20., -TMath::Pi(), TMath::Pi(),
				20., 0., 100.);
    fHist_Pretag_QCDVeto->SetXTitle("#Delta#phi(j_{1},l)");
    fHist_Pretag_QCDVeto->SetYTitle("Lepton p_{T} (GeV)");
    fHist_Pretag_QCDVeto->SetZTitle("Events");

    gDirectory->cd("..");
}

//____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistogramsTag() {
    //
    // Book tag histograms
    //    

    gDirectory->mkdir("Tag");
    gDirectory->cd("Tag");

    // BEYS 1-D histograms

    // This is going to be an unweighted histogram in order to count the unweighted number of events
    fHist_Tag_UwHt = new TH1F("h_tag_UwHt", "H_{T}, tag",
			      100, 0., 1000.);
    fHist_Tag_UwHt->SetXTitle("H_{T} (GeV)");
    fHist_Tag_UwHt->SetYTitle("Events");

    
    // Bins?
    fHist_Tag_Ht = new TH1F("h_tag_Ht", "H_{T}, tag",
			    100, 0., 1000.);
    fHist_Tag_Ht->SetXTitle("H_{T} (GeV)");
    fHist_Tag_Ht->SetYTitle("Events");

    // Bins?
    fHist_Tag_Etajlf = new TH1F("h_tag_Etajlf", "|#eta(j_{lf})|, tag",
				18, 0., 4.5);
    fHist_Tag_Etajlf->SetXTitle("|#eta(j_{lf})|");
    fHist_Tag_Etajlf->SetYTitle("Events");

    // Bins?
    fHist_Tag_EtajlfQl = new TH1F("h_tag_EtajlfQl", "|#eta(j_{lf})|*Q(l), tag",
				  36, -4.5, 4.5);
    fHist_Tag_EtajlfQl->SetXTitle("|#eta(j_{lf})|*Q(l)");
    fHist_Tag_EtajlfQl->SetYTitle("Events");

    
    // Bins?
    fHist_Tag_Mjlfjb = new TH1F("h_tag_Mjlfjb", "m(j_{lf}j_{b}), tag",
				60, 0., 600.);
    fHist_Tag_Mjlfjb->SetXTitle("m(j_{lf}j_{b}) (GeV)");
    fHist_Tag_Mjlfjb->SetYTitle("Events");

    // Bins?
    fHist_Tag_Mljb = new TH1F("h_tag_Mljb", "m(lj_{b}), tag",
			      40, 0., 400.);
    fHist_Tag_Mljb->SetXTitle("m(lj_{b}) (GeV)");
    fHist_Tag_Mljb->SetYTitle("Events");

    // Bins?
    fHist_Tag_DeltaEtaljlf = new TH1F("h_tag_DeltaEtaljlf", "#Delta#eta(lj_{lj}), tag",
				      28, -7., 7.);
    fHist_Tag_DeltaEtaljlf->SetXTitle("#Delta#eta(lj_{lf})");
    fHist_Tag_DeltaEtaljlf->SetYTitle("Events");

    // Bins?
    fHist_Tag_DeltaPhiljlf = new TH1F("h_tag_DeltaPhiljlf", "#Delta#phi(lj_{lj}), tag",
				      20, -TMath::Pi(), TMath::Pi());
    fHist_Tag_DeltaPhiljlf->SetXTitle("#Delta#phi(lj_{lf})");
    fHist_Tag_DeltaPhiljlf->SetYTitle("Events");

    // Bins?
    fHist_Tag_DeltaRljlf = new TH1F("h_tag_DeltaRljlf", "#DeltaR(lj_{lf}), tag",
				    30, 0., 6.);
    fHist_Tag_DeltaRljlf->SetXTitle("#DeltaR(lj_{lf})");
    fHist_Tag_DeltaRljlf->SetYTitle("Events");

    // Bins?
    fHist_Tag_DeltaRljb = new TH1F("h_tag_DeltaRljb", "#DeltaR(lj_{b}), tag",
				   30, 0., 6.);
    fHist_Tag_DeltaRljb->SetXTitle("#DeltaR(lj_{b})");
    fHist_Tag_DeltaRljb->SetYTitle("Events");

      // Bins?
    fHist_Tag_DeltaRjlfjb = new TH1F("h_tag_DeltaRjlfjb", "#DeltaR(j_{lf}j_{b}), tag",
				     30, 0., 6.);
    fHist_Tag_DeltaRjlfjb->SetXTitle("#DeltaR(j_{lf}j_{b})");
    fHist_Tag_DeltaRjlfjb->SetYTitle("Events");

    // Bins?
    fHist_Tag_CosThetaljlf = new TH1F("h_tag_CosThetaljlf", "Cos#Theta (lj_{lf}), tag",
				      50, -1., +1.);
    fHist_Tag_CosThetaljlf->SetXTitle("Cos#Theta (lj_{lf})");
    fHist_Tag_CosThetaljlf->SetYTitle("Events");
    //BEYE

    
    fHist_Tag_met = new TH1F("h_tag_met", "Missing E_{T}, tag",
			     24, 0., 120.);
    fHist_Tag_met->SetXTitle("E_{T, miss} (GeV)");
    fHist_Tag_met->SetYTitle("Events");

    fHist_Tag_MtW = new TH1F("h_tag_MtW", "Transverse W-Boson mass, tag",
			     30, 0., 150);
    fHist_Tag_MtW->SetXTitle("M_{T, W} (GeV)");
    fHist_Tag_MtW->SetYTitle("Events");

    fHist_Tag_PtW = new TH1F("h_tag_PtW", "Transverse W-Boson mass, tag",
			     40, 0., 200);
    fHist_Tag_PtW->SetXTitle("P_{T, W} (GeV)");
    fHist_Tag_PtW->SetYTitle("Events");

    fHist_Tag_LeadingJet_pt  = new TH1F("h_tag_LeadingJet_pt", "Leading Jet p_{T}, tag",
					40, 0., 200.);
    fHist_Tag_LeadingJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Tag_LeadingJet_pt->SetYTitle("Events");

    fHist_Tag_LeadingJet_eta = new TH1F("h_tag_LeadingJet_eta", "Leading Jet #eta, tag",
					20, -5., 5.);
    fHist_Tag_LeadingJet_eta->SetXTitle("#eta");
    fHist_Tag_LeadingJet_eta->SetYTitle("Events");

    fHist_Tag_LeadingJet_phi = new TH1F("h_tag_LeadingJet_phi", "Leading Jet #phi, tag",
					20, -TMath::Pi(), TMath::Pi());
    fHist_Tag_LeadingJet_phi->SetXTitle("#phi");
    fHist_Tag_LeadingJet_phi->SetYTitle("Events");
    
    fHist_Tag_2ndLeadingJet_pt  = new TH1F("h_tag_2ndLeadingJet_pt", "2nd Leading Jet p_{T}, tag",
					   40., 0., 200.);
    fHist_Tag_2ndLeadingJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Tag_2ndLeadingJet_pt->SetYTitle("Events");

    fHist_Tag_2ndLeadingJet_eta = new TH1F("h_tag_2ndLeadingJet_eta", "2nd Leading Jet #eta, tag",
					  20., -5., 5.);
    fHist_Tag_2ndLeadingJet_eta->SetXTitle("#eta");
    fHist_Tag_2ndLeadingJet_eta->SetYTitle("Events");

    fHist_Tag_2ndLeadingJet_phi = new TH1F("h_tag_2ndLeadingJet_phi", "2nd Leading Jet #phi, tag",
					  20., -TMath::Pi(), TMath::Pi());
    fHist_Tag_2ndLeadingJet_phi->SetXTitle("#phi");
    fHist_Tag_2ndLeadingJet_phi->SetYTitle("Events");

    fHist_Tag_BJet_pt  = new TH1F("h_tag_BJet_pt", "Leading Jet p_{T}, tag",
    					40, 0., 200.);
    fHist_Tag_BJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Tag_BJet_pt->SetYTitle("Events");

    fHist_Tag_BJet_eta = new TH1F("h_tag_BJet_eta", "Leading Jet #eta, tag",
    				  20, -2.5, 2.5);
    fHist_Tag_BJet_eta->SetXTitle("#eta");
    fHist_Tag_BJet_eta->SetYTitle("Events");

    fHist_Tag_BJet_phi = new TH1F("h_tag_BJet_phi", "Leading Jet #phi, tag",
    					20, -TMath::Pi(), TMath::Pi());
    fHist_Tag_BJet_phi->SetXTitle("#phi");
    fHist_Tag_BJet_phi->SetYTitle("Events");
    
    fHist_Tag_LJet_pt  = new TH1F("h_tag_LJet_pt", "2nd Leading Jet p_{T}, tag",
    					   40., 0., 200.);
    fHist_Tag_LJet_pt->SetXTitle("p_{T} (GeV)");
    fHist_Tag_LJet_pt->SetYTitle("Events");

    fHist_Tag_LJet_eta = new TH1F("h_tag_LJet_eta", "2nd Leading Jet #eta, tag",
    					  20., -5., 5.);
    fHist_Tag_LJet_eta->SetXTitle("#eta");
    fHist_Tag_LJet_eta->SetYTitle("Events");

    fHist_Tag_LJet_phi = new TH1F("h_tag_LJet_phi", "2nd Leading Jet #phi, tag",
    					  20., -TMath::Pi(), TMath::Pi());
    fHist_Tag_LJet_phi->SetXTitle("#phi");
    fHist_Tag_LJet_phi->SetYTitle("Events");
    
    fHist_Tag_Lepton_pt  = new TH1F("h_tag_Lepton_pt", "Lepton p_{T}, tag",
				    28, 0., 140.);
    fHist_Tag_Lepton_pt->SetXTitle("p_{T} (GeV)");
    fHist_Tag_Lepton_pt->SetYTitle("Events");
    
    fHist_Tag_Lepton_eta = new TH1F("h_tag_Lepton_eta", "Lepton #eta, tag",
				   20., -2.5, 2.5);
    fHist_Tag_Lepton_eta->SetXTitle("#eta");
    fHist_Tag_Lepton_eta->SetYTitle("Events");

    fHist_Tag_Lepton_phi = new TH1F("h_tag_Lepton_phi", "Lepton #phi, tag",
				   20., -TMath::Pi(), TMath::Pi());
    fHist_Tag_Lepton_phi->SetXTitle("#phi");
    fHist_Tag_Lepton_phi->SetYTitle("Events");

    fHist_Tag_QCDVeto = new TH2F("h_tag_QCDVeto", "QCD Veto",
				20., -TMath::Pi(), TMath::Pi(),
				20., 0., 100.);
    fHist_Tag_QCDVeto->SetXTitle("#Delta#phi(j_{1},l)");
    fHist_Tag_QCDVeto->SetYTitle("Lepton p_{T} (GeV)");
    fHist_Tag_QCDVeto->SetZTitle("Events");

    //BEYS 2-D histograms
    // Bins?
    fHist_Tag_MjlfjbHt = new TH2F("h_tag_MjlfjbHt", "m(j_{lf}j_{b}) H_{T}",
				35., 0., 1000.,
				35., 0., 1000.);
    fHist_Tag_MjlfjbHt->SetXTitle("H_{T} (GeV)");
    fHist_Tag_MjlfjbHt->SetYTitle("m(j_{lf}j_{b}) (GeV)");
    fHist_Tag_MjlfjbHt->SetZTitle("Events");

    // Bins?
    fHist_Tag_MljbHt = new TH2F("h_tag_MljbHt", "m(lj_{b}) H_{T}",
				35., 0., 1000.,
				35., 0., 1000.);
    fHist_Tag_MljbHt->SetXTitle("H_{T} (GeV)");
    fHist_Tag_MljbHt->SetYTitle("m(lj_{b}) (GeV)");
    fHist_Tag_MljbHt->SetZTitle("Events");

    // Bins?
    fHist_Tag_MjlfjbMljb = new TH2F("h_tag_MjlfjbMljb", "m(j_{lf}j_{b}) m(lj_{b})",
				35., 0., 1000.,
				35., 0., 1000.);
    fHist_Tag_MjlfjbMljb->SetXTitle("m(lj_{b}) (GeV)");
    fHist_Tag_MjlfjbMljb->SetYTitle("m(j_{lf}j_{b}) (GeV)");
    fHist_Tag_MjlfjbMljb->SetZTitle("Events");
    
    fHist_Tag_MjlfjbMtW = new TH2F("h_tag_MjlfjbMtW", "m(j_{lf}j_{b}) m_{T}(W)",
				35., 0., 1000.,
				35., 0., 1000.);
    fHist_Tag_MjlfjbMtW->SetXTitle("m_{T}(W) (GeV)");
    fHist_Tag_MjlfjbMtW->SetYTitle("m(j_{lf}j_{b}) (GeV)");
    fHist_Tag_MjlfjbMtW->SetZTitle("Events");

    fHist_Tag_MljbMtW = new TH2F("h_tag_MljbMtW", "m(lj_{b}) m_{T}(W)",
				35., 0., 1000.,
				35., 0., 1000.);
    fHist_Tag_MljbMtW->SetXTitle("m_{T}(W) (GeV)");
    fHist_Tag_MljbMtW->SetYTitle("m(lj_{b}) (GeV)");
    fHist_Tag_MljbMtW->SetZTitle("Events");

    fHist_Tag_MjlfjbEtajlf = new TH2F("h_tag_MjlfjbEtajlf", "m(j_{lf}j_{b}) |#eta(j_{lf})|",
			        18., 0., 4.5,
			        35., 0., 1000.);
    fHist_Tag_MjlfjbEtajlf->SetXTitle("|#eta(j_{lf})|");
    fHist_Tag_MjlfjbEtajlf->SetYTitle("m(j_{lf}j_{b}) (GeV)");
    fHist_Tag_MjlfjbEtajlf->SetZTitle("Events");

    fHist_Tag_MljbEtajlf = new TH2F("h_tag_MljbEtajlf", "m(lj_{b}) |#eta(j_{lf})|",
				18., 0., 4.5,
		                35., 0., 1000.);
    fHist_Tag_MljbEtajlf->SetXTitle("|#eta(j_{lf})|");
    fHist_Tag_MljbEtajlf->SetYTitle("m(lj_{b}) (GeV)");
    fHist_Tag_MljbEtajlf->SetZTitle("Events");

    //BEYE 2-D histogram
    
    gDirectory->cd("..");
}

// ____________________________________________________________________

void AtlSgTop_combinationAnalysis::FillHistogramsMCTruthStudy() {
    //
    // Fill histograms for mc truth study
    //

    // Perform truth matching
    DoTruthMatching();
    
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);
    
    // Highest pt leptons
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    HepParticle *loose_lepton = (HepParticle*)fLooseLeptons->At(0);

    // MC truth level jets
    TClonesArray *mcjets = fEvent->GetJets(AtlJet::kMCAntiKt4Hadron);

    // Fill Jet truth info:
    fHist_mc_jet_truth_label->Fill(Jetlead->GetTruthFlavour(), GetTagEvtWeight());
    fHist_mc_jet_truth_label->Fill(Jet2ndlead->GetTruthFlavour(), GetTagEvtWeight());
    
    // Fill Decay Channel
    fHist_mc_DecayChannels->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    fHist_mc_MET->Fill(fTtbarDecayMode, fMET, GetTagEvtWeight());

    // Get Number of tight and loose leptons
    Int_t Ntight = fLeptons->GetEntries();
    Int_t Nloose = fLooseLeptons->GetEntries();
    if ( Ntight + Nloose > 1 ) {
    // if ( fEvent->GetElectrons()->GetEntries() + fEvent->GetMuons()->GetEntries() > 1 ) {
	// Which type of events are vetoed?
	fHist_mc_DecayChannels_Veto->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    } else {
	fHist_mc_MET_nonVeto->Fill(fTtbarDecayMode, fMET, GetTagEvtWeight());
    }
    
    // Does a true lepton matches the trigger lepton?
    // (within DeltaR < 0.4)
    // (truth matching was already performed, check if a true particle was found)
    if ( fMCTightLepton != 0 ) {
	fHist_mc_TriggerMatch->Fill(fTtbarDecayMode, 1., GetTagEvtWeight());
    } else {
	fHist_mc_TriggerMatch->Fill(fTtbarDecayMode, 2., GetTagEvtWeight());
    }
    
    // Check reconstruction efficiencies
    // --> Is the ttbar event fully reconstructed using loose leptons?
    // 1: both leptons are reconstructed
    // 2: only tight lepton reconstructed
    // 3: only loose lepton reconstructed
    // 4: no true lepton reconstructed
    if ( fMCTightLepton != 0 && fMCLooseLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 1., GetTagEvtWeight());
    } else if ( fMCTightLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 2., GetTagEvtWeight());
    } else if ( fMCLooseLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 3., GetTagEvtWeight());
    } else {
    	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 4., GetTagEvtWeight());
    }

    // Fill DeltaR between reco lepton (for loose and tight)
    // and _closest_ (!) mc truth lepton
    // take closest, since loose/tight might not be matched!
    Double_t DeltaR_min_lepton = 99.;
    Double_t DeltaR_min_looselepton = 99.;

    if ( fMCLeptonPlus != 0 )
	DeltaR_min_lepton = fMCLeptonPlus->DeltaR(lepton);
    if ( fMCLeptonMinus != 0 )
	if ( fMCLeptonMinus->DeltaR(lepton) < DeltaR_min_lepton )
	    DeltaR_min_lepton = fMCLeptonMinus->DeltaR(lepton);

    if ( loose_lepton != 0 ) {
	if ( fMCLeptonPlus != 0 )
	    DeltaR_min_looselepton = fMCLeptonPlus->DeltaR(loose_lepton);
	if ( fMCLeptonMinus != 0 )
	    if ( fMCLeptonMinus->DeltaR(loose_lepton) < DeltaR_min_looselepton )
		DeltaR_min_looselepton = fMCLeptonMinus->DeltaR(loose_lepton);
    }
    
    if ( DeltaR_min_lepton != 99. )
	fHist_mc_DeltaR_tight_lepton->Fill(fTtbarDecayMode, DeltaR_min_lepton, GetTagEvtWeight());
    if ( DeltaR_min_looselepton != 99. )
	fHist_mc_DeltaR_loose_lepton->Fill(fTtbarDecayMode, DeltaR_min_looselepton, GetTagEvtWeight());


    // Di-lepton kinematics:
    // =====================
    // High / Low pt lepton
    // DeltaR between truth particles (di-lepton events only)
    // ------------------------------------------------------
    HepMCParticle *MCLeadLepton = 0;
    HepMCParticle *MC2ndLeadLepton = 0;
    HepMCParticle *MCLeadLep_bQuark = 0;
    HepMCParticle *MC2ndLeadLep_bQuark = 0;
    switch (fTtbarDecayMode) {
	// Di-Lepton events
	// -----------------
	case kElEl:
	case kMuMu:
	case kElMu:
	case kElTau:
	case kMuTau:
	case kTauTau:
	    // Find high pt lepton
	    if ( fMCLeptonPlus->Pt() > fMCLeptonMinus->Pt() ) {
		MCLeadLepton = fMCLeptonPlus;
		MC2ndLeadLepton = fMCLeptonMinus;
	    } else {
		MCLeadLepton = fMCLeptonMinus;
		MC2ndLeadLepton = fMCLeptonPlus;
	    }
	    
	    if ( MCLeadLepton->Charge() > 0 ) {
		// Q+ lepton --> b-quark
		MCLeadLep_bQuark = fMCbQuark;
		MC2ndLeadLep_bQuark = fMCAntibQuark;
	    } else {
		// Q+ lepton --> b-quark
		MCLeadLep_bQuark = fMCAntibQuark;
		MC2ndLeadLep_bQuark = fMCbQuark;
	    }
	    
	    // Fill pt histograms (truth level)
	    fHist_mc_truth_LeadLepton_Pt->Fill(fTtbarDecayMode, MCLeadLepton->Pt(), GetTagEvtWeight());
	    if ( fMCLooseLepton != 0 )
		fHist_mc_truth_2ndLeadLepton_Pt_matched->Fill(fTtbarDecayMode, MC2ndLeadLepton->Pt(), GetTagEvtWeight());
	    else
		fHist_mc_truth_2ndLeadLepton_Pt_unmatched->Fill(fTtbarDecayMode, MC2ndLeadLepton->Pt(), GetTagEvtWeight());

	    // Fill pt histograms (reco level)
	    fHist_mc_reco_LeadLepton_Pt->Fill(fTtbarDecayMode, lepton->Pt(), GetTagEvtWeight());
	    if ( loose_lepton != 0 ) {
		fHist_mc_reco_2ndLeadLepton_Pt->Fill(fTtbarDecayMode, loose_lepton->Pt(), GetTagEvtWeight());
	    }
	    
	    // Fill delta R histograms
	    // -----------------------
	    // Find opposite sign (OS) b-Quark and same sign b-Quark (SS)
	    // OS b-Quark should be closer (?) to lepton since it's from the same
	    // top quark decay...
	    
	    // Leading lepton and OS b-quark
	    fHist_mc_DeltaR_LeadLepton_bQuarkOS->Fill(fTtbarDecayMode,
						      MCLeadLepton->DeltaR(MCLeadLep_bQuark),
						      GetTagEvtWeight());
	    
	    // Leading lepton and SS b-quark
	    fHist_mc_DeltaR_LeadLepton_bQuarkSS->Fill(fTtbarDecayMode,
						      MCLeadLepton->DeltaR(MC2ndLeadLep_bQuark),
						      GetTagEvtWeight());
	    
	    // Leading lepton and OS b-quark
	    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->Fill(fTtbarDecayMode,
							 MC2ndLeadLepton->DeltaR(MC2ndLeadLep_bQuark),
							 GetTagEvtWeight());
	    
	    // Leading lepton and SS b-quark
	    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->Fill(fTtbarDecayMode,
							 MC2ndLeadLepton->DeltaR(MCLeadLep_bQuark),
							 GetTagEvtWeight());

	    // Leading lepton and 2nd leading lepton
	    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->Fill(fTtbarDecayMode,
							   MCLeadLepton->DeltaR(MC2ndLeadLepton),
							   GetTagEvtWeight());

	    // Compare unmatched taus to mc truth jets
	    if ( fMCLooseLepton != 0 ) {
		AtlJet *mcjet = 0;
		AtlJet *closest_mcjet = 0;
		Double_t deltaR = 99.;
		Double_t deltaR_min = 99.;
		TIter next_jet(mcjets);
		while( (mcjet = (AtlJet*)next_jet()) ) {
		    deltaR = fMCLooseLepton->DeltaR(mcjet);
		    if ( deltaR < deltaR_min ) {
			deltaR_min = deltaR;
			closest_mcjet = mcjet;
		    }
		}
		fHist_mc_DeltaR_unmatchedMCLepton_truthjet->Fill(fTtbarDecayMode,
								 deltaR_min,
								 GetTagEvtWeight());
		fHist_mc_unmatchedMCLepton_truthjet_pt->Fill(fTtbarDecayMode,
							     closest_mcjet->Pt(),
							     GetTagEvtWeight());
	    }
		
	default:
	    ;// Do nothing
    }
}

// ____________________________________________________________________

void AtlSgTop_combinationAnalysis::BookHistogramsMCTruthStudy() {
    //
    // Book histograms for mc truth study
    //

    gDirectory->mkdir("MCTruthInfo");
    gDirectory->cd("MCTruthInfo");

    fHist_mc_DecayChannels = new TH1F("h_mc_DecayChannels", "Decay channels of ttbar events",
				      11, 0.5, 11.5);
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels->SetYTitle("Events");

    fHist_mc_DecayChannels_Veto = new TH1F("h_mc_DecayChannels_Veto", "Decay channels of ttbar events",
				      11, 0.5, 11.5);
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels_Veto->SetYTitle("Events");

    fHist_mc_jet_truth_label = new TH1F("h_mc_jet_truth_label", "Truth label of jets",
					5, -0.5, 4.5);
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(1,  "Unknown");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(2,  "Light");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(3,  "C");    
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(4,  "B");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(5,  "#tau");
    fHist_mc_jet_truth_label->SetYTitle("Events");

    fHist_mc_TriggerMatch = new TH2F("h_mc_TriggerMatch", "Reconstructed Lepton matches Trigger",
				     11, 0.5, 11.5, 2, 0.5, 2.5);
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_TriggerMatch->GetYaxis()->SetBinLabel(1, "Matched");
    fHist_mc_TriggerMatch->GetYaxis()->SetBinLabel(2, "Unmatched");
    fHist_mc_TriggerMatch->SetZTitle("Events");
    
    fHist_mc_MET = new TH2F("h_mc_MET", "MET distribution per decay channel",
			    11, 0.5, 11.5, 50, 0., 200.);
    fHist_mc_MET->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_MET->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_MET->GetYaxis()->SetTitle("MET");
    fHist_mc_MET->SetZTitle("Events");

    fHist_mc_MET_nonVeto = new TH2F("h_mc_MET_nonVeto", "MET distribution per decay channel after veto",
			    11, 0.5, 11.5, 50, 0., 200.);
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_MET_nonVeto->GetYaxis()->SetTitle("MET");
    fHist_mc_MET_nonVeto->SetZTitle("Events");

    // Reconstructed Efficiencies
    fHist_mc_RecoEff = new TH2F("h_mc_RecoEff", "Reconstructed Efficiency",
				11, 0.5, 11.5, 4, 0.5, 4.5);
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(1, "2 lep");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(2, "only tight");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(3, "only loose");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(4, "no lep");

    
    // DeltaR per channel (reco, truth)
    fHist_mc_DeltaR_tight_lepton = new TH2F("h_mc_DeltaR_tight_lepton", "DeltaR(reco, truth)",
					    11, 0.5, 11.5, 50, 0., 3.);
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DeltaR_tight_lepton->GetYaxis()->SetTitle("#Delta R");

    // DeltaR per channel (reco, truth) 
    fHist_mc_DeltaR_loose_lepton = new TH2F("h_mc_DeltaR_loose_lepton", "DeltaR(reco, truth)",
					    11, 0.5, 11.5, 50, 0., 3.);
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DeltaR_loose_lepton->GetYaxis()->SetTitle("#Delta R");

    gDirectory->mkdir("Dilepton_kinematics");
    gDirectory->cd("Dilepton_kinematics");

    gDirectory->mkdir("truth_level");
    gDirectory->cd("truth_level");
    
    // DeltaR between truth particles
    fHist_mc_DeltaR_LeadLepton_bQuarkOS = new TH2F("h_mc_DeltaR_LeadLepton_bQuarkOS", "DeltaR(lep, b-quark)",
						   6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetYaxis()->SetTitle("#Delta R");
    
    fHist_mc_DeltaR_LeadLepton_bQuarkSS = new TH2F("h_mc_DeltaR_LeadLepton_bQuarkSS", "DeltaR(lep, b-quark)",
						   6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS = new TH2F("h_mc_DeltaR_2ndLeadLepton_bQuarkOS", "DeltaR(lep, b-quark)",
						      6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS = new TH2F("h_mc_DeltaR_2ndLeadLepton_bQuarkSS", "DeltaR(lep, b-quark)",
						      6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton = new TH2F("h_mc_DeltaR_LeadLepton_2ndLeadLepton", "DeltaR(lep1, lep2)",
							6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_truth_LeadLepton_Pt = new TH2F("h_mc_truth_LeadLepton_Pt", "Leading Lepton Pt (MC truth info)",
					    6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetYaxis()->SetTitle("Leading lepton p_{T} (MC truth info)");

    fHist_mc_truth_2ndLeadLepton_Pt_matched = new TH2F("h_mc_truth_2ndLeadLepton_Pt_matched", "Leading Lepton Pt (MC truth info), reco matched",
						  6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (MC truth info)");

    fHist_mc_truth_2ndLeadLepton_Pt_unmatched = new TH2F("h_mc_truth_2ndLeadLepton_Pt_unmatched", "Leading Lepton Pt (MC truth info), unmatched",
							 6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (MC truth info)");


    // Truth level jets
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet = new TH2F("h_mc_DeltaR_unmatchedMCLepton_truthjet", "DeltaR(unmatched MC lepton, truth jet)",
							  6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetYaxis()->SetTitle("#DeltaR");

    fHist_mc_unmatchedMCLepton_truthjet_pt = new TH2F("h_mc_unmatchedMCLepton_truthjet_pt", "Pt of closest mcjet to unmatched MC lepton",
						      6, 0.5, 6.5, 50., 0., 100.);
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetYaxis()->SetTitle("jet p_{T}");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("reco_level");
    gDirectory->cd("reco_level");

    fHist_mc_reco_LeadLepton_Pt = new TH2F("h_mc_reco_LeadLepton_Pt", "Leading Lepton Pt (reco)",
					    6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetYaxis()->SetTitle("Leading lepton p_{T} (reco)");
    
    fHist_mc_reco_2ndLeadLepton_Pt = new TH2F("h_mc_reco_2ndLeadLepton_Pt_matched", "Leading Lepton Pt (reco)",
						  6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (reco)");

    gDirectory->cd("..");
    gDirectory->cd("..");
}

// ____________________________________________________________________

void AtlSgTop_combinationAnalysis::DoTruthMatching() {
    //
    // - Assign true particles to data members
    // - Determine decay channel of top pair based on MC truth info
    //
    
    fMCbQuark = 0;
    fMCAntibQuark = 0;
    
    fMCLeptonPlus  = 0;
    fMCLeptonMinus = 0;

    fMCTightLepton = 0;
    fMCLooseLepton = 0;

    TIter next_MCprt(fEvent->GetMCParticles());
    HepMCParticle *mcprt = 0;

    // Count number of leptons and quarks
    Int_t N_El  = 0;
    Int_t N_Mu  = 0;
    Int_t N_Tau = 0;
    Int_t N_Quarks = 0;

    while ( (mcprt = (HepMCParticle*) next_MCprt()) ) {
	// skip b-quarks
	if ( mcprt->IsBeauty() ) {
	    fMCbQuark = mcprt;
	    continue;
	}
	if ( mcprt->IsBeautyBar() ) {
	    fMCAntibQuark = mcprt;
	    continue;
	}
	
	// Count leptons
	if ( mcprt->IsElectron() ) N_El++;
	if ( mcprt->IsMuon() ) N_Mu++;
	if ( mcprt->IsTau() ) N_Tau++;

	// Assign Leptons
	if ( mcprt->IsChargedLepton() ) {
	    if ( mcprt->Charge() > 0 )
		fMCLeptonPlus = mcprt;
	    else 
		fMCLeptonMinus = mcprt;
	}
	
	// Count Quarks
	if ( mcprt->IsQuark() ) { N_Quarks++; }
    }

    // Find the MC truth lepton matching the tight reco lepton
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    // Find the MC truth lepton matching the loose reco lepton
    HepParticle *loose_lepton = (HepParticle*)fLooseLeptons->At(0);

    // Assign truth lepton matched by deltaR
    // -------------------------------------
    Double_t DeltaR_tight_plus = 99.;
    Double_t DeltaR_loose_plus = 99.;
    if ( fMCLeptonPlus != 0 ) {
	DeltaR_tight_plus = fMCLeptonPlus->DeltaR(lepton);
    	if ( DeltaR_tight_plus < 0.4 )
    	    fMCTightLepton = fMCLeptonPlus;
	
	if ( loose_lepton != 0 ) {
	    DeltaR_loose_plus = fMCLeptonPlus->DeltaR(loose_lepton);
	    if ( DeltaR_loose_plus < 0.4 )
		fMCLooseLepton = fMCLeptonPlus;
	}
    }
    
    Double_t DeltaR_tight_minus = 99.;
    Double_t DeltaR_loose_minus = 99.;
    if ( fMCLeptonMinus != 0 ) {
	DeltaR_tight_minus = fMCLeptonMinus->DeltaR(lepton);
	if ( DeltaR_tight_minus < 0.4
	     && DeltaR_tight_minus < DeltaR_tight_plus )
    	    fMCTightLepton = fMCLeptonMinus;
    
    	if ( loose_lepton != 0 ) {
	    DeltaR_loose_minus = fMCLeptonMinus->DeltaR(loose_lepton);
	    if ( DeltaR_loose_minus < 0.4 &&
		 DeltaR_loose_minus < DeltaR_loose_plus )
		fMCLooseLepton = fMCLeptonMinus;
	}
    }

    if ( fMCTightLepton != 0 &&
	 fMCLooseLepton != 0 &&
	 fMCLooseLepton == fMCTightLepton ) {
	
	Warning("DoTruthMatching",
		"MC tight lepton and MC loose lepton is identical. Check OverlapRemoval or Truthmatching!");

	// Some debug info
	cout << "DeltaR(lepton, loose lepton) = " << lepton->DeltaR(loose_lepton) << endl;
	if ( fMCLeptonPlus != 0 ) {
	    cout << "DeltaR(plus, lepton) = " << fMCLeptonPlus->DeltaR(lepton) << endl;
	    cout << "DeltaR(plus, loose lepton) = " << fMCLeptonPlus->DeltaR(loose_lepton) << endl;
	}
	if ( fMCLeptonMinus != 0 ) {
	    cout << "DeltaR(minus, lepton) = " << fMCLeptonMinus->DeltaR(lepton) << endl;
	    cout << "DeltaR(minus, loose lepton) = " << fMCLeptonMinus->DeltaR(loose_lepton) << endl;
	}
	    
	// cout << "fMCLeptonPlus: " << endl;
    	// fMCLeptonPlus->P().Print();
    	// cout << "fMCLeptonMinus: " << endl;
    	// fMCLeptonMinus->P().Print();
    	// cout << "tight lepton: " << endl;
    	// lepton->P().Print();
    	// cout << "loose lepton: " << endl;
    	// loose_lepton->P().Print();
    
    	// gSystem->Abort();
    }
    
    // Determine the decay mode of the event
    if ( N_El == 2 ) fTtbarDecayMode = kElEl;
    else if ( N_Mu == 2 ) fTtbarDecayMode = kMuMu;
    else if ( N_Tau == 2 ) fTtbarDecayMode = kTauTau;
    else if ( N_El == 1 && N_Mu == 1) fTtbarDecayMode = kElMu;
    else if ( N_El == 1 && N_Tau == 1) fTtbarDecayMode = kElTau;
    else if ( N_Mu == 1 && N_Tau == 1) fTtbarDecayMode = kMuTau;
    else if ( N_El == 1 && N_Quarks == 2) fTtbarDecayMode = kElQQ;
    else if ( N_Mu == 1 && N_Quarks == 2) fTtbarDecayMode = kMuQQ;
    else if ( N_Tau == 1 && N_Quarks == 2 ) fTtbarDecayMode = kTauQQ;
    else if ( N_Quarks == 4 ) fTtbarDecayMode = kHadronic;
    else {
	Warning("DoTruthMatching",
		"Mode unkown!");
	cout << "N_El     = " << N_El << endl
	     << "N_Mu     = " << N_Mu << endl
	     << "N_Tau    = " << N_Tau << endl
	     << "N_Quarks = " << N_Quarks << endl
	     << endl;
	fTtbarDecayMode = kUnknown;
    }
    
}
