//____________________________________________________________________
//
// Example Z->ll analysis (in preparation for an tZ analysis)
//
//  
// Author: Sebastian Mergelmeyer <mailto: mergelm@physik.hu-berlin.de>
// Update: $Id$
//
// Copyright: 2015 (C) Sebastian Mergelmeyer
//
#ifndef SINGLETOP_AtlZllAnalysisDemo
#include <AtlZllAnalysisDemo.h>
#endif

#include <TH1.h>
#include <TLorentzVector.h>
#include <TList.h>
#include <TVector2.h>
#include <TVector3.h>
#include <AtlEvtWriterD3PDSgTopThinned.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlZllAnalysisDemo);
#endif

//____________________________________________________________________

AtlZllAnalysisDemo::AtlZllAnalysisDemo(const char* OutputFileName) :
    AtlSelector(OutputFileName) {
    //
    // Default constructor
    //
    fLeptons     = 0;
    fElectrons   = 0;
    fMuons       = 0;
    fJets        = 0;
    fBJets       = 0;
    fObjTool     = 0;
    fSFTool      = 0;
    SetCutDefaults();
}

//____________________________________________________________________

void AtlZllAnalysisDemo::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fChgCombination = kChgOpposite;
    fNLeptons_min = 2;
    fNBTags_min   = 0;
    fNBTags_max   = 9999;
    fPt_Lep1_min  = 0.;
    fMZ_min       = 0.;
    fMZ_max       = 1e9;
    fMET_min      = 0.;
    fMET_max      = 1e9;
    fMtW_min      = 0.;
    fMtW_max      = 1e9;
}

//____________________________________________________________________

AtlZllAnalysisDemo::~AtlZllAnalysisDemo() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlZllAnalysisDemo::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    fTree->SetBranchStatus("*", kTRUE);
}

//____________________________________________________________________

void AtlZllAnalysisDemo::Clear(Option_t * option) {
    //
    // Clear variables and lists
    // (or be very careful to define them before use for each event!)
    //
}

//____________________________________________________________________

void AtlZllAnalysisDemo::Begin(TTree * tree) {
    //
    // Begin analysis job
    //
    fEvtWriter = new AtlEvtWriterD3PDSgTopThinned();
    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlZllAnalysisDemo::SlaveBegin(TTree * tree) {
    //
    // Initialise before the first event.
    //
    AtlSelector::SlaveBegin(tree);

    // Get ObjectSelectionTool + pointers to analysis objects
    fObjTool = (AtlObjectsToolD3PDSgTop*)
	GetTool("AtlObjectsToolD3PDSgTop", "", kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Get ScaleFactorTool
    fSFTool = (AtlObjRecoScaleFactorTool*)
	GetTool("AtlObjRecoScaleFactorTool", "", kTRUE);
    // Provide SF tool with pointers to analysis objects
    if ( fSFTool ) {
	fSFTool->SetObjDefinitionTool(fObjTool);
        fSFTool->SetLeptons(fLeptons);
        fSFTool->SetJets(fJets);
    }

    // Initialise event writer
    if ( AtlEvtWriterD3PDSgTopThinned *evtWriter
	 = dynamic_cast<AtlEvtWriterD3PDSgTopThinned*>(fEvtWriter) ) {
        evtWriter->SetLeptons(fLeptons);
        evtWriter->SetJets(fJets);
    }
}

//____________________________________________________________________

void AtlZllAnalysisDemo::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlZllAnalysisDemo::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << "\n"
	 << "========================================================\n"
	 << "  Atlas Z->ll Analysis Demo Selector                    \n"
	 << "========================================================\n"
	 << "\n";
    AtlSelector::Print();
    cout << "Final event selection:" << endl
	 << "  fChgCombination = ";
    switch ( fChgCombination ) {
	case kChgOpposite:
	    cout << "opposite charge" << endl;
	    break;
	case kChgSame:
	    cout << "same charge" << endl;
	    break;
	case kChgAny:
	    cout << "any" << endl;
	    break;
	default:
	    cout << "undef" << endl;
    }
    cout << "  fNLeptons_min   = " << fNLeptons_min << endl
	 << "  fNBTags_min     = " << fNBTags_min << endl
	 << "  fNBTags_max     = " << fNBTags_max << endl
	 << "  fPt_Lep1_min    = " << fPt_Lep1_min << " GeV" << endl
	 << "  fMZ_min         = " << fMZ_min << " GeV" << endl
	 << "  fMZ_max         = " << fMZ_max << " GeV" << endl
	 << "  fMET_min        = " << fMET_min << " GeV" << endl
	 << "  fMET_max        = " << fMET_max << " GeV" << endl
	 << "  fMtW_min        = " << fMtW_min << " GeV" << endl
	 << "  fMtW_max        = " << fMtW_max << " GeV" << endl
	 << endl
	 << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlZllAnalysisDemo::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
    AtlSelector::Terminate();
}

//____________________________________________________________________

Bool_t AtlZllAnalysisDemo::ProcessPreCut() {
    //
    // Event pre-selection
    //
    // While generally not needed, one might want to reject events
    // early to save computing time. Some information (e.g. the event
    // weight) cannot be used here.
    //

    // Check whether "trigger" fired
    return (fEvent->GetEventHeader()
	    ->TestPreselectionFlagsAny(AtlEventHeader::kLeptonic));
}

//____________________________________________________________________

Bool_t AtlZllAnalysisDemo::ProcessCut() {
    //
    // Event selection
    //

    // Minimum no. of leptons (default = 2)
    if ( fLeptons->GetEntries() < fNLeptons_min ) return kFALSE;

    // Leading lepton Pt
    HepParticle *lep1 = (HepParticle*)fLeptons->At(0);
    HepParticle *lep2 = (HepParticle*)fLeptons->At(1);
    if ( lep1->Pt() < fPt_Lep1_min ) return kFALSE;

    // Charge combination
    switch ( fChgCombination ) {
	case kChgOpposite:
	    if ( lep1->Charge()*lep2->Charge() != -1 ) return kFALSE;
	    break;
	case kChgSame:
	    if ( lep1->Charge()*lep2->Charge() != 1 ) return kFALSE;
	    break;
	case kChgAny:
	    break;
	default:
	    ;
    }
    
    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( fMET < fMET_min || fMET > fMET_max ) return kFALSE;
    
    // Transverse W mass
    fMtW = fEvent->W_Mperp((HepParticle*)fLeptons->At(0));
    if ( fMtW < fMtW_min || fMtW > fMtW_max ) return kFALSE;

    // Compute dilepton invariant mass
    TLorentzVector p_dilep = lep1->P() + lep2->P();
    fDileptonM = p_dilep.M();

    // Fill all pre-tagged histograms
    FillHistogramsCommon(kPreTag);
    // Count no. of pre-tagged events
    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();
    
    // B-tagging requirement
    if ( fBJets->GetEntries() < fNBTags_min
	 || fBJets->GetEntries() > fNBTags_max) return kFALSE;

    // Fill all b-tagged histograms
    FillHistogramsCommon(kBTag);

    // Z mass requirement
    if ( fDileptonM < fMZ_min || fDileptonM > fMZ_max ) return kFALSE;
    
    return kTRUE;
}

//____________________________________________________________________

void AtlZllAnalysisDemo::BookHistograms() {
    //
    // Book histograms
    //    

    // Pre-tag histograms
    gDirectory->mkdir("PreTag");
    gDirectory->cd("PreTag");
    BookHistogramsCommon(kPreTag);
    gDirectory->cd("..");
    
    // B-tag histograms
    gDirectory->mkdir("BTag");
    gDirectory->cd("BTag");
    BookHistogramsCommon(kBTag);
    gDirectory->cd("..");

    // Z peak histograms
    gDirectory->mkdir("Zpeak");
    gDirectory->cd("Zpeak");
    BookHistogramsCommon(kZPeak);
    gDirectory->cd("..");
}

//____________________________________________________________________

void AtlZllAnalysisDemo::BookHistogramsCommon(EHistDir HistDir) {
    //
    // Book common histograms
    //    
    gDirectory->mkdir("Common");
    gDirectory->cd("Common");

    // =======
    // Leptons
    // =======
    gDirectory->mkdir("Leptons");
    gDirectory->cd("Leptons");

    // Leading lepton Pt
    fHist_Lep1_Pt[HistDir] = new TH1D("h_Lep1_Pt", "Leading Lepton Pt",
				      40, 0., 200.);
    fHist_Lep1_Pt[HistDir]->SetXTitle("Leading lepton p_{T} [GeV]");
    fHist_Lep1_Pt[HistDir]->SetYTitle("Events");
    
    // Leading lepton eta
    fHist_Lep1_Eta[HistDir] = new TH1D("h_Lep1_Eta", "Leading Lepton Eta",
				       50, -2.5, 2.5);
    fHist_Lep1_Eta[HistDir]->SetXTitle("Leading lepton #eta");
    fHist_Lep1_Eta[HistDir]->SetYTitle("Events");

    // Leading lepton phi
    fHist_Lep1_Phi[HistDir] = new TH1D("h_Lep1_Phi", "Leading Lepton Azimuth",
				       20, -M_PI, M_PI);
    fHist_Lep1_Phi[HistDir]->SetXTitle("Leading lepton #phi [rad]");
    fHist_Lep1_Phi[HistDir]->SetYTitle("Events");

    // Sub-leading lepton Pt
    fHist_Lep2_Pt[HistDir] = new TH1D("h_Lep2_Pt", "Sub-leading Lepton Pt",
				      40, 0., 100.);
    fHist_Lep2_Pt[HistDir]->SetXTitle("Sub-leading lepton p_{T} [GeV]");
    fHist_Lep2_Pt[HistDir]->SetYTitle("Events");
    
    // Sub-leading lepton eta
    fHist_Lep2_Eta[HistDir] = new TH1D("h_Lep2_Eta", "Sub-leading Lepton Eta",
				       50, -2.5, 2.5);
    fHist_Lep2_Eta[HistDir]->SetXTitle("Sub-leading lepton #eta");
    fHist_Lep2_Eta[HistDir]->SetYTitle("Events");
    
    // Sub-leading lepton phi
    fHist_Lep2_Phi[HistDir] = new TH1D("h_Lep2_Phi", "Sub-leading Lepton Azimuth",
				       20, -M_PI, M_PI);
    fHist_Lep2_Phi[HistDir]->SetXTitle("Sub-leading lepton #phi [rad]");
    fHist_Lep2_Phi[HistDir]->SetYTitle("Events");

    // Lepton charge
    fHist_Lep_Chg[HistDir] = new TH1D("h_Lep_Chg", "Lepton Charge",
				      3, -1.5, 1.5);
    fHist_Lep_Chg[HistDir]->SetXTitle("Lepton charge");
    fHist_Lep_Chg[HistDir]->SetYTitle("Entries");

    // No. of leptons per event
    fHist_Lep_N[HistDir] = new TH1D("h_Lep_N", "No. of Leptons per Event",
				    5, -0.5, 4.5);
    fHist_Lep_N[HistDir]->SetXTitle("Number of leptons / event");
    fHist_Lep_N[HistDir]->SetYTitle("Events");

    // Loose/tightness of identification for electrons and muons
    fHist_Lep_ID[HistDir] = new TH1D("h_Lep_ID", "Lepton Identification Quality",
				     4, 0, 4);
    fHist_Lep_ID[HistDir]->GetXaxis()->SetBinLabel(1, "Loose e");
    fHist_Lep_ID[HistDir]->GetXaxis()->SetBinLabel(2, "Tight e");
    fHist_Lep_ID[HistDir]->GetXaxis()->SetBinLabel(3, "Loose #mu");
    fHist_Lep_ID[HistDir]->GetXaxis()->SetBinLabel(4, "Tight #mu");
    fHist_Lep_ID[HistDir]->SetYTitle("Entries");
    
    gDirectory->cd("..");
    
    // ====
    // Jets
    // ====
    gDirectory->mkdir("Jets");
    gDirectory->cd("Jets");

    // Leading jet Pt
    fHist_Jet1_Pt[HistDir] = new TH1D("h_Jet1_Pt", "Leading Jet Pt",
				       40, 0., 200.);
    fHist_Jet1_Pt[HistDir]->SetXTitle("Leading jet p_{T} [GeV]");
    fHist_Jet1_Pt[HistDir]->SetYTitle("Events");
    
    // Leading jet eta
    fHist_Jet1_Eta[HistDir] = new TH1D("h_Jet1_Eta", "Leading Jet Eta",
				       100, -5., 5.);
    fHist_Jet1_Eta[HistDir]->SetXTitle("Leading jet #eta");
    fHist_Jet1_Eta[HistDir]->SetYTitle("Events");
    
    // Leading jet phi
    fHist_Jet1_Phi[HistDir] = new TH1D("h_Jet1_Phi", "Leading Jet Azimuth",
				       20, -M_PI, M_PI);
    fHist_Jet1_Phi[HistDir]->SetXTitle("Leading jet #phi [rad]");
    fHist_Jet1_Phi[HistDir]->SetYTitle("Events");

    // No. of jets per event
    fHist_Jet_N[HistDir] = new TH1D("h_Jet_N", "", 6, -0.5, 5.5);
    fHist_Jet_N[HistDir]->SetXTitle("Number of jets / event");
    fHist_Jet_N[HistDir]->SetYTitle("Events");
    
    gDirectory->cd("..");
    
    // ==============
    // Missing energy
    // ==============
    gDirectory->mkdir("MET");
    gDirectory->cd("MET");

    // MET magnitude
    fHist_MET_mag[HistDir] = new TH1D("h_MET_mag", "Missing Transverse Momentum",
				      40, 0., 100.);
    fHist_MET_mag[HistDir]->SetXTitle("E_{T}^{miss} [GeV]");
    fHist_MET_mag[HistDir]->SetYTitle("Events");

    // MET azimuth
    fHist_MET_phi[HistDir] = new TH1D("h_MET_phi", "Missing Transverse Momentum",
				      20, -M_PI, M_PI);
    fHist_MET_phi[HistDir]->SetXTitle("E_{T}^{miss} #phi [rad]");
    fHist_MET_phi[HistDir]->SetYTitle("Events");
    
    // Transverse W mass
    fHist_MtW[HistDir] = new TH1D("h_MtW", "Transverse W Boson Mass",
				  40, 0., 200);
    fHist_MtW[HistDir]->SetXTitle("M_{T, W} [GeV]");
    fHist_MtW[HistDir]->SetYTitle("Events");
    
    gDirectory->cd("..");

    // ========
    // DiLepton
    // ========
    gDirectory->mkdir("DiLepton");
    gDirectory->cd("DiLepton");

    // Delta Phi
    fHist_Dilepton_DeltaPhi[HistDir] = new TH1D("h_Dilepton_DeltaPhi",
						"Dilepton Azimuth Difference",
						100, -M_PI, M_PI);
    fHist_Dilepton_DeltaPhi[HistDir]->SetXTitle("#Delta#phi(lep,lep) [rad]"); 
    fHist_Dilepton_DeltaPhi[HistDir]->SetYTitle("Events");

    // Invariant mass
    fHist_Dilepton_Mass[HistDir] = new TH1D("h_Dilepton_Mass",
					    "Dilepton Invariant Mass",
					    200, 0., 200.);
    fHist_Dilepton_Mass[HistDir]->SetXTitle("M(lep,lep) [GeV]");
    fHist_Dilepton_Mass[HistDir]->SetYTitle("Events");
    
    // Dilepton charge
    fHist_Dilepton_Chg[HistDir] = new TH1D("h_Dilepton_Chg", "Dilepton Charge",
					   3, -1.5, 1.5);
    fHist_Dilepton_Chg[HistDir]->SetXTitle("chg(lep1)*chg(lep2)");
    fHist_Dilepton_Chg[HistDir]->SetYTitle("Entries");

    gDirectory->cd("..");
    
    gDirectory->cd("..");
}

//____________________________________________________________________

void AtlZllAnalysisDemo::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    // Fill Z peak histograms (b-tagged)
    FillHistogramsCommon(kZPeak);
}
    
//____________________________________________________________________

void AtlZllAnalysisDemo::FillHistogramsCommon(EHistDir HistDir) {
    //
    // Fill common histograms
    //
	  
    // Fill every histogram with the proper weight
    // (do not use the b-tagging SFs for the pre-tagged histograms)
    Double_t w = 1.;
    if ( HistDir == kPreTag ) {
	w = fEvent->GetPreTagEvtWeight();
    } else {
	w = fEvent->GetTagEvtWeight();
    }
    
    // =======
    // Leptons
    // =======
    HepParticle *lep1 = (HepParticle*)fLeptons->At(0);
    HepParticle *lep2 = (HepParticle*)fLeptons->At(1);
    fHist_Lep1_Pt[HistDir]->Fill(lep1->Pt(), w);
    fHist_Lep2_Pt[HistDir]->Fill(lep2->Pt(), w);
    fHist_Lep1_Eta[HistDir]->Fill(lep1->Eta(), w);
    fHist_Lep2_Eta[HistDir]->Fill(lep2->Eta(), w);
    fHist_Lep1_Phi[HistDir]->Fill(lep1->Phi(), w);
    fHist_Lep2_Phi[HistDir]->Fill(lep2->Phi(), w);
    fHist_Lep_Chg[HistDir]->Fill(lep1->Charge(), w);
    fHist_Lep_Chg[HistDir]->Fill(lep2->Charge(), w);
    fHist_Lep_N[HistDir]->Fill(fLeptons->GetEntries(), w);

    // Lepton identification quality
    if ( lep1->IsElectron() ) {
	if ( ((AtlElectron*)lep1)->IsLoose() ) {
	    fHist_Lep_ID[HistDir]->Fill(0.5);
	} else if ( ((AtlElectron*)lep1)->IsTight() )
	    fHist_Lep_ID[HistDir]->Fill(1.5);
    } else if ( lep1->IsMuon() ) {
	if ( ((AtlMuon*)lep1)->IsLoose() ) {
	    fHist_Lep_ID[HistDir]->Fill(2.5);
	} else if ( ((AtlMuon*)lep1)->IsTight() )
	    fHist_Lep_ID[HistDir]->Fill(3.5);
    }
    
    // ====
    // Jets
    // ====
    AtlJet *jet1 = (AtlJet*)fJets->At(0);
    fHist_Jet1_Pt[HistDir]->Fill(jet1->Pt(), w);
    fHist_Jet1_Eta[HistDir]->Fill(jet1->Eta(), w);
    fHist_Jet1_Phi[HistDir]->Fill(jet1->Phi(), w);
    fHist_Jet_N[HistDir]->Fill(fJets->GetEntries(), w);

    // ==============
    // Missing energy
    // ==============
    fHist_MET_mag[HistDir]->Fill(fMET, w);
    fHist_MET_phi[HistDir]->Fill(fEvent->GetEnergySum()
				 ->MissingEt_Phi(), w);
    fHist_MtW[HistDir]->Fill(fMtW, w);
    
    // ========
    // DiLepton
    // ========
    fHist_Dilepton_DeltaPhi[HistDir]->Fill(lep1->DeltaPhi(lep2), w);
    fHist_Dilepton_Mass[HistDir]->Fill(fDileptonM, w);
    fHist_Dilepton_Chg[HistDir]->Fill(lep1->Charge()*lep2->Charge(), w);
}

