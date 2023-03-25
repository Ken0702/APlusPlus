//___________________________________________________________
//
//  A++ Object Selection Tool for SgTop D3PDs
//
//  This tool is used to select objects, like jets or leptons
//  for top analysis and the corresponding tools for the
//  SgTop D3PDs.
// 
//  Author: Soeren Stamm <mailto:stamm@physik.hu-berlin.de> 
//  Update: $Id$
//  Copyright: 2013 (C) Soeren Stamm
//
//
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#include <iostream>
#include <vector>
#include <TIterator.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TString.h>
#include "AtlEvent.h"
#include "AtlQCDMMTool.h"

using namespace std;

#ifndef __CINT__
ClassImp(AtlObjectsToolD3PDSgTop);
#endif

//___________________________________________________________

AtlObjectsToolD3PDSgTop::AtlObjectsToolD3PDSgTop(const char* name, const char* title) : 
    AtlObjectsDefinitionTool(name, title) {
    //
    // Default constructor
    //
    fLeptons   = new TList;
    fElectrons = new TList;
    fMuons     = new TList;
    fLooseLeptons   = new TList;
    fLooseElectrons = new TList;
    fLooseMuons     = new TList;
    fLooseTaus      = new TList;
    fJets      = new TList;
    fBJets     = new TList;
    fQCDMMToolIsOn  = kFALSE;
    fCutFlowTool    = 0;
    fIsClean = kFALSE;
    
    fCounter = 0;
    SetCutDefaults();
}

//___________________________________________________________

AtlObjectsToolD3PDSgTop::~AtlObjectsToolD3PDSgTop(){
    //
    // Default destructor
    //
    delete fLeptons;
    delete fElectrons;
    delete fMuons;
    delete fLooseLeptons;
    delete fLooseElectrons;
    delete fLooseMuons;
    delete fLooseTaus;
    delete fJets;
    delete fBJets;    
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fEventHeader*",               kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4LCTopoJets*",       kTRUE);
    fTree->SetBranchStatus("fAntiKt4LCTopoJets*",         kTRUE);
    fTree->SetBranchStatus("fN_Electrons*",               kTRUE);
    fTree->SetBranchStatus("fElectrons*",                 kTRUE);
    fTree->SetBranchStatus("fN_Muons*",                   kTRUE);
    fTree->SetBranchStatus("fMuons*",                     kTRUE);
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    
    // Define when the tool runs and how
    fProcessMode       = AtlAnalysisTool::kObjectsDefinition;
    fLepChannel        = 0x0;
    fAddToCutFlow      = kFALSE;
    
    // Leptons
    // =======
    fElectronPt_min = 25.0; // already preselected in SgTopD3PD
    fMuonPt_min     = 25.0; // already preselected in SgTopD3PD
    fLeptonN_min    =  0;
    fLeptonN_max    =  0;

    fUseTaus = kFALSE; // Disable hadronic taus

    // Jets
    // ====
    fJetsAuthor  = AtlJet::kAntiKt4LCTopo;
    fJetEta_min  = 0.;
    fJetEta_max  = 1.e10;
    fBJetEta_min = 0.;  // B-Jets need inner detector tracks
    fBJetEta_max = 2.5; // (|Eta| < 2.5)
    fJetPt_min   = 0.;
    fJetN_min    = 0;
    fJetN_max    = 0;
    fFwdJetEta_min  = 0.;
    fFwdJetEta_max  = 1.e10;
    fFwdJetPt_min   = 0.;
    
    // B-tag weight cuts
    fBTagger        = AtlBTag::kMV1;
    fBTagWeight_min = 0.7892; // MV1@70% 
    fBTagWeight_max = 1.e10;  // no upper cut value by default 

    fFilterMEPhoton = 0;
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::Clear() {
    //
    //  Clears the dynamically allocated objects
    //    
    fLeptons->Clear();
    fElectrons->Clear();
    fMuons->Clear();
    fLooseLeptons->Clear();
    fLooseElectrons->Clear();
    fLooseMuons->Clear();
    fLooseTaus->Clear();
    fJets->Clear();
    fBJets->Clear();
    if ( gDebug > 1 )
        Info(__FUNCTION__, "This event was cleared");
    fIsClean = kTRUE;
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::BookHistograms(){
    //
    // Book histograms
    //

    // Get pointer to cut-flow tool if available (set to 0 otherwise)
    if ( fAddToCutFlow ) {
        fCutFlowTool = (AtlCutFlowTool*)GetTool("AtlCutFlowTool");
    }

    // Check if QCDMMTool is running
    AtlAnalysisTool *tool = (AtlQCDMMTool*)GetTool("AtlQCDMMTool", "", kFALSE);
    if ( ( nullptr != tool ) && ( tool->fProcessMode != AtlAnalysisTool::kOff ) ) {
        fQCDMMToolIsOn = kTRUE;
        Info(__FUNCTION__, "QCD Tool found");
    } else {
        Info(__FUNCTION__, "QCD Tool not found");
    }
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindElectrons() {
    // 
    // Find Signal Electrons
    // 
    AtlElectron *el = 0;	
    TClonesArray *electrons = fEvent->GetElectrons();
    for ( Int_t i = 0; i < fEvent->GetN_Electrons(); ++i ){
	el = (AtlElectron*)electrons->At(i);
	if ( el->Pt() >= fElectronPt_min &&
	     el->IsTight() ) {
	    fElectrons->Add(el);
	}
	if ( el->IsLoose() ) {
	    fLooseElectrons->Add(el);
	}
    }
    fElectrons->Sort(kSortDescending);
    fLooseElectrons->Sort(kSortDescending);
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindMuons() {
    // 
    // Find Muons
    // 
    AtlMuon *mu = 0;
    TClonesArray *muons = fEvent->GetMuons();
    for ( Int_t i = 0; i < fEvent->GetN_Muons(); i++ ) {
	mu = (AtlMuon*)muons->At(i);
	if ( mu->Pt() >= fMuonPt_min
	     && mu->IsTight() )
	    fMuons->Add(mu);
	if ( mu->IsLoose() )
	    fLooseMuons->Add(mu);
    }
    fMuons->Sort(kSortDescending);
    fLooseMuons->Sort(kSortDescending);
} 

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindTaus() {
    // 
    // Find Taus
    //
    
    AtlTau *tau = 0;
    TClonesArray* taus = fEvent->GetTaus();
    for ( Int_t i = 0; i < fEvent->GetN_Taus(); i++ ) {
	tau = (AtlTau*)taus->At(i);
	// Taus are already loose
	fLooseTaus->Add(tau);
    }
    fLooseTaus->Sort(kSortDescending);
} 

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindJets() {
    // 
    // Find Jets
    // 
    AtlJet *jet = 0;
    TClonesArray *jets = fEvent->GetJets(fJetsAuthor);

    for ( Int_t i = 0; i < fEvent->GetN_Jets(fJetsAuthor); ++i ) {
  	jet = (AtlJet*)jets->At(i);

	if ( jet->Pt() >= fJetPt_min 
	     && TMath::Abs(jet->Eta()) >= fJetEta_min 
	     && TMath::Abs(jet->Eta()) <= fJetEta_max ) {

	    /*// Forward jet cut
	    if ( jet->Pt() < fFwdJetPt_min 
		 && TMath::Abs(jet->Eta()) >= fFwdJetEta_min
		 && TMath::Abs(jet->Eta()) <= fFwdJetEta_max )
	    {

	    	continue;
	    }*/

	    fJets->Add(jet);
	}
    }
    fJets->Sort(kSortDescending);
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindBJets() {
	//
    // Find B-Jets
    // 


    AtlJet  *jet = 0;
    TClonesArray *jets = fEvent->GetJets(fJetsAuthor);
    for ( Int_t i = 0; i < fEvent->GetN_Jets(fJetsAuthor); ++i ) {
     	jet = (AtlJet*)jets->At(i);
     	
	const AtlBTag *tag = jet->GetTag(fBTagger);

	if ( jet->Pt() >= fJetPt_min
	     && TMath::Abs(jet->Eta()) >= fBJetEta_min 
	     && TMath::Abs(jet->Eta()) <= fBJetEta_max
	     && tag->IsValid() ) {

	    if ( fEvent->IsRun1() ) {
		if ( tag->GetWeight() > fBTagWeight_min
		     && tag->GetWeight() < fBTagWeight_max )
		    fBJets->Add(jet);
	    } else if ( fEvent->IsRun2() ) {
		// No tag weights used any longer; tags are stored as
		// booleans only (in the weight data member for
		// convenience)

		if ( tag->GetWeight() > 0.5 ){
		    fBJets->Add(jet);
			//}

			}
	    } else {
		Fatal(__FUNCTION__, "Invalid run phase. Abort!");
		gSystem->Abort(1);
	    }
	}


    }
    fBJets->Sort(kSortDescending);


}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FindLeptons() {
    //
    // Construct signal lepton list dependent on lepton mode
    //

    // Find all leptons
    FindElectrons();
    FindMuons();
    FindTaus();

    // Construct lepton list
    if ( fLepChannel & (AtlSelector::kElectron | AtlSelector::kLepton) ) {
	fLeptons->AddAll(fElectrons);
        fLooseLeptons->AddAll(fLooseElectrons);
    }
    if ( fLepChannel & (AtlSelector::kMuon | AtlSelector::kLepton) ) {
	fLeptons->AddAll(fMuons);
        fLooseLeptons->AddAll(fLooseMuons);
    }
    if ( fUseTaus ) 
	fLooseLeptons->AddAll(fLooseTaus);

    // Sort descending by Pt
    fLeptons->Sort(kSortDescending);
    fLooseLeptons->Sort(kSortDescending);
}

//___________________________________________________________

Bool_t AtlObjectsToolD3PDSgTop::AnalyzeEvent() {
    //
    // Event analysis: required object selection
    //

    if ( !fIsClean )
        Fatal(__FUNCTION__, "Event has to be Clear()ed before analysis.");
    fIsClean = kFALSE;

    if ( fCutFlowTool ) {

        fCutFlowTool->Fill("Objects_All", fEvent->GetTagEvtWeight());


    }
    if (! fCutFlowTool){

    }


    // ttgamma overlap removal
    if ( fFilterMEPhoton ) {
        if ( (fFilterMEPhoton > 0) != fEvent->GetEventHeader()->HasMEPhoton() )
            return kFALSE;
        if ( fCutFlowTool ) {
            fCutFlowTool->Fill("Objects_ttgammaOR", fEvent->GetTagEvtWeight());
        }
    }

    // Leptons
    FindLeptons();

    LooseLeptonOverlapRemoval();
    fLooseLeptons->Sort(kSortDescending);

    // >>>> Temporary fix for Run2  <<<<
    bool needTriggerMatching = ( fEvent->BeamEnergy() > 5555. );




    if(	fEvent->GetEventHeader()->ejets2015 == 0 && fEvent->GetEventHeader()->mujets2015 == 0 &&
    	fEvent->GetEventHeader()->ejets2016 == 0 && fEvent->GetEventHeader()->mujets2016 == 0 &&
		fEvent->GetEventHeader()->ejets2017 == 0 && fEvent->GetEventHeader()->mujets2017 == 0 &&
		fEvent->GetEventHeader()->ejets2018 == 0 && fEvent->GetEventHeader()->mujets2018 == 0
				) return kFALSE;
    if ( fCutFlowTool ) {
            fCutFlowTool->Fill("Objects_ejets", fEvent->GetTagEvtWeight());
        }

    // Trigger matching: one of the tight leptons must match
    if ( needTriggerMatching ) {

        bool isTriggerMatched = false;
        for ( TIter next(fElectrons); AtlElectron * lep = static_cast<AtlElectron *>(next()); ) {
            if (lep->IsTriggerMatched()) {
                isTriggerMatched = true;
            }
        }
        for ( TIter next(fMuons); AtlMuon * lep = static_cast<AtlMuon *>(next()); ) {
            if (lep->IsTriggerMatched()) {
                isTriggerMatched = true;
            }
        }
        if ( !isTriggerMatched )
            return kFALSE;

        if ( fCutFlowTool ) {
            fCutFlowTool->Fill("Objects_TrigMatch", fEvent->GetTagEvtWeight());
        }
    }


    // Apply lepton number cut
    Int_t NLeptons = -1;
    if ( !fQCDMMToolIsOn ) {
        NLeptons = fLeptons->GetEntries();
    } else {
        NLeptons = fLeptons->GetEntries() + fLooseLeptons->GetEntries();
    }
    if ( NLeptons < fLeptonN_min ||
         NLeptons > fLeptonN_max )
        return kFALSE;
    if ( fCutFlowTool ) {
        fCutFlowTool->Fill("Objects_NLep", fEvent->GetTagEvtWeight());
    }

    // Jets
    FindJets();

    // Find b-jets
    FindBJets();

    //tchan OK here
    // Apply Jet-Bin cut

    if ( fJets->GetEntries() < fJetN_min ||
     	 fJets->GetEntries() > fJetN_max )
    	return kFALSE;



    if ( fCutFlowTool ) {
        fCutFlowTool->Fill("Objects_JetBin", fEvent->GetTagEvtWeight());
    }

    return kTRUE;
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::FillHistograms() {
    //
    // Method for filling histograms, only events that survive 
    // AnalyzeEvent() are taken into account
    //
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::Init() {
    //
    // Init Jet-Bin values once
    //

    // Set JetN_max & JetN_min
    SetJetN();
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::Print() const {
    //
    // Print cuts for object selection of the tool
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atl Objects Tool D3PD SgTop                           " << endl
	 << "========================================================" << endl
	 << "  Chosen process mode is       "
	 << ( (fProcessMode == AtlAnalysisTool::kObjectsDefinition) ? "kObjectsDefinition" : "Should be kObjectsDefinition!" ) << endl
	 << "--------------------------------------------------------" << endl 
	 << "  Specifications of physics objects:                    " << endl
	 << "--------------------------------------------------------" << endl
	 << "  Leptons: " << endl;
    switch ( fLepChannel ) {
	case AtlSelector::kElectron:
	    cout << "  Lepton Mode      = kElectron" << endl
		 << "  fElectronPt_min  = " << fElectronPt_min << " GeV" << endl;
	    break;
	case AtlSelector::kMuon:
	    cout << "  Lepton Mode      = kMuon" << endl
		 << "  fMuonPt_min      = " << fMuonPt_min << " GeV" << endl;
	    break;
	case AtlSelector::kLepton:
	    cout << "  Lepton Mode      = kLepton" << endl
		 << "  fElectronPt_min  = " << fElectronPt_min << " GeV" << endl
		 << "  fMuonPt_min      = " << fMuonPt_min << " GeV" << endl
                 << "  fLeptonN_min     = " << fLeptonN_min << endl
                 << "  fLeptonN_max     = " << fLeptonN_max << endl;
	    break;
	default:
	    Error("Print", "Which Lepton?");
    }
    cout << endl
	 << "  fUseTaus = ";
    if ( fUseTaus == kTRUE ) {
	cout << "true" << endl;
    } else {
	cout << "false" << endl;
    }
    cout << "--------------------------------------------------------" << endl
	 << "  Jets: " << endl
	 << "  fJetMultiplicity = " << AtlSelector::GetJetMultEnum(fJetMults) << endl
	 << "  fJetN_min        = " << fJetN_min << endl
	 << "  fJetN_max        = " << fJetN_max << endl
	 << endl
	 << "  fJetsAuthor      = " << AtlJet::NameOfType(fJetsAuthor) << endl
	 << "  fJetPt_min       = " << fJetPt_min << " GeV" << endl
	 << "  fJetEta_min      = " << fJetEta_min << endl
	 << "  fJetEta_max      = " << fJetEta_max << endl
	 << endl
	 << "  fFwdJetPt_min    = " << fFwdJetPt_min << endl
	 << "  fFwdJetEta_min   = " << fFwdJetEta_min << endl
	 << "  fFwdJetEta_max   = " << fFwdJetEta_max << endl
	 << endl
	 << "  fBTagger         = " << AtlBTag::GetTaggerName(fBTagger) << endl
	 << "  fBTagWeight_min  = " << fBTagWeight_min << endl
	 << "  fBTagWeight_max  = " << fBTagWeight_max << endl
	 << "  fBJetEta_min     = " << fBJetEta_min << endl
	 << "  fBJetEta_max     = " << fBJetEta_max << endl
	 << "========================================================" << endl
	 << endl;
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::Terminate() {
    //
    // Executed at the end of the tool
    //
}

//___________________________________________________________________

void AtlObjectsToolD3PDSgTop::SetJetN() {
    //
    // Set Jet Multiplicity
    // (i.e. fJetN_min & fJetN_max)
    //
    switch ( fJetMults ) {
	case AtlSelector::kOneJet:
	    fJetN_min = 1; fJetN_max = 1;
	    break;
	case AtlSelector::kTwoJet:
	    fJetN_min = 2; fJetN_max = 2;
	    break;
	case AtlSelector::kThreeJet:
	    fJetN_min = 3; fJetN_max = 3;
	    break;
	case AtlSelector::kFourJet:
	    fJetN_min = 4; fJetN_max = 4;
	    break;
	case AtlSelector::kFiveJet:
	    fJetN_min = 5; fJetN_max = 5;
	    break;
	case AtlSelector::kSixJet:
	    fJetN_min = 6; fJetN_max = 6;
	    break;
	case AtlSelector::k4To6Jets:
	    fJetN_min = 4; fJetN_max = 6;
		break;
	case AtlSelector::kOneOrMoreJets:
		fJetN_min = 1; fJetN_max = 999;
		break;
	case AtlSelector::kTwoOrMoreJets:
		fJetN_min = 2; fJetN_max = 999;
		break;
	case AtlSelector::kThreeOrMoreJets:
		fJetN_min = 3; fJetN_max = 999;
		break;
	case AtlSelector::kAllJets:
		fJetN_min = 0; fJetN_max = 999;
		break;
	default:
		Error("SetJetN()", "Jet Multiplicity not known! Abort!");
		gSystem->Abort(0);
	}
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::LooseLeptonOverlapRemoval() {
	//
	// Remove any loose lepton that is overlapping with the leading
	// tight lepton
	//
	// This funtion is needed since this kind of overlap removal is
	// not performed in AnalysisTop
	//
	HepParticle *TightLepton = 0;
	if ( fLepChannel & AtlSelector::kElectron ) {
		TightLepton = (HepParticle*) fElectrons->At(0);
	}
	if ( fLepChannel & AtlSelector::kMuon ) {
		TightLepton = (HepParticle*) fMuons->At(0);
	}
	if ( !TightLepton )
		return;

	TIter next_lep(fLooseLeptons);
	HepParticle *LooseLepton = 0;
	while ( (LooseLepton = (HepParticle*) next_lep()) ) {
		if ( TightLepton->DeltaR(LooseLepton) < 0.4 ) {
			fLooseLeptons->Remove(LooseLepton);
		}
	}
}

//___________________________________________________________

void AtlObjectsToolD3PDSgTop::ApplyLooseLeptonPtCut() {
    //
    // When Using FindElectrons() or FindMuons(), there is no pT cut
    // applied for loose leptons. However, this is needed for the
    // QCD Matrix Method.
    //
    TIter next_lep(fLooseLeptons);
    HepParticle *loose_lep = 0;
    while ( (loose_lep = (HepParticle*) next_lep()) ) {
	if ( loose_lep->IsElectron() ) {
	    if ( loose_lep->Pt() < fElectronPt_min ) {
		fLooseLeptons->Remove(loose_lep);
	    }
	} else if ( loose_lep->IsMuon() ) {
	    if ( loose_lep->Pt() < fMuonPt_min ) {
		fLooseLeptons->Remove(loose_lep);
	    }
	} else {
	    // something went wrong; this should not happen unless
	    // fUseTaus is set
	    Warning(__FUNCTION__, "Lepton has invalid ID of %d",
		    loose_lep->GetPdgCode());
	    fLooseLeptons->Remove(loose_lep);
	}
    }
}
