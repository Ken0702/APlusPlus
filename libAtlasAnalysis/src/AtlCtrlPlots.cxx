//____________________________________________________________________
//
// Helper class for control plots common for all ATLAS analyses.
// Used by AtlSelector.
//
// See the class descriptions of AtlSelector and AtlExampleAnalysis
// how to use this class in your analysis job.
//
// When being used this class appends an additional directory
// structure to the output file in which the control histograms are
// stored. This new structure is basically divided into two
// paren-folders. One for all histograms filled without any selection
// cuts, and the other one containing a doubled of all histograms
// which are filled after the AtlSelector::ProcessCut() method call,
// ie. after event selection.
//
//     CtrlPlots
//       |
//       |
//       +--- NoEvtSel
//       |       |
//       |       +--- Jet histograms
//       |       |
//       |       +--- Electron histograms
//       |       |
//       |       +--- ...
//       |
//       |
//       +--- EvtSel
//               |
//               +--- Jet histograms
//               |
//               +--- Electron histograms
//               |
//               +--- ...
// 
// In order to append control histograms perform the following steps.
//
// (1) Switch the necessary branches on by adding the respective lines
// to the SetBranchStatus() member function.
//
// (2) Extend the BookHistograms() and BookXXXHistogram() member
// functions. Do not forget to add the histogram to the
// fHistogramsNoEvtSel list. You must NOT give the same name to
// more than one histogram.
//
// (3) Extend the FillHistograms() and FillXXXHistograms() member
// functions accordingly. The histograms are accessed by their names.
//
// Note: All histogram have to be booked and filled only once. The
// second set (which will be filled after the event selection) is
// booked and filled automatically.
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCtrlPlots
#include <AtlCtrlPlots.h>
#endif
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlCtrlPlots);
#endif

//____________________________________________________________________

AtlCtrlPlots::AtlCtrlPlots(const char* name, const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fHistogramsNoEvtSel = new THashTable;
    fHistogramsEvtSel   = new THashTable;
    fProcessMode        = kPostAnalysis;
}

//____________________________________________________________________

AtlCtrlPlots::~AtlCtrlPlots() {
  //
  // Default destructor
  //
  fHistogramsNoEvtSel->Delete(); delete fHistogramsNoEvtSel;
  fHistogramsEvtSel  ->Delete(); delete fHistogramsEvtSel;
}

//____________________________________________________________________

void AtlCtrlPlots::CopyFolder(TDirectory *dir_src,
			      TDirectory *dir_dest) {
  //
  // Copy given directory and all its sub-directories from
  // "NoEvtSel" folder to "EvtSel" folder
  //

  // Search for sub-folders
  TIter next(dir_src->GetList());
  TObject *subdir_src = 0;
  while ( (subdir_src = (TObject*)next()) ) {
    if ( subdir_src->InheritsFrom("TDirectory")
	 && (TDirectory*)subdir_src->IsFolder() ) {
      CopyFolder((TDirectory*)subdir_src,
		 dir_dest->mkdir(subdir_src->GetName(),
				 subdir_src->GetTitle()));
    }
  }
}
    
//____________________________________________________________________

void AtlCtrlPlots::BookHistograms() {
    //
    // Book histograms
    //
    // Note: The trigger histograms are not booked here. For their
    // booking the trigger configuration is needed which is not loaded
    // at this stage of the job (it will be loaded when the first
    // input file is opened). The trigger histograms are booked when
    // the corresponding FillTriggerHistograms() routine is called the
    // first time.
    //
    
    // Create sub-directories
    fDir_noevtsel = gDirectory->mkdir("NoEvtSel",
				      "Before event selection");
    fDir_evtsel = gDirectory->mkdir("EvtSel",
				    "After event selection");
    fDir_noevtsel->cd();   
    // Book histograms
    BookEnergySumsHistograms();
    BookJetHistograms();
    BookElectronHistograms();
    BookMuonHistograms();
    BookTauHistograms(); 
    BookPhotonHistograms();
    BookAtlFastHistograms();
    
    // Clone folder structure
    CopyFolder(fDir_noevtsel, fDir_evtsel);
    
    // Book twin-histograms with event selection
    fDir_evtsel->cd();
    TH1 *h1 = 0; TH1 *h2 = 0;
    TIter next(fHistogramsNoEvtSel);
    while ( (h1 = (TH1*)next()) ) {
	// Clone histogram
	h2 = (TH1*)h1->Clone();
	fHistogramsEvtSel->Add(h2);
	// Extend histogram titles
	TString t1 = TString(h1->GetTitle()) + "   (w/o event selection)";
	TString t2 = TString(h2->GetTitle()) + "   (after event selection)";
	h1->SetTitle(t1);
	h2->SetTitle(t2);
	// Move clone histogram to correct directory
	TString path = h1->GetDirectory()->GetPath();
	path.ReplaceAll("/NoEvtSel", 9, "/EvtSel", 7);
	h2->SetDirectory(gROOT->GetDirectory(path.Data()));
    }
}

//____________________________________________________________________

void AtlCtrlPlots::SetBranchStatus() {
  //
  // Switch on needed branches
  //
  fTree->SetBranchStatus("fN_MCParticles*",       kTRUE);
  fTree->SetBranchStatus("fMCParticles*",         kTRUE);
  fTree->SetBranchStatus("fTrigger*",             kTRUE);
  fTree->SetBranchStatus("fEventHeader*",         kTRUE);
  fTree->SetBranchStatus("fEnergySum*",           kTRUE);
  fTree->SetBranchStatus("fN_Cone4H1TowerJets*",  kTRUE);
  fTree->SetBranchStatus("fCone4H1TowerJets*",    kTRUE);
  fTree->SetBranchStatus("fN_MCCone4HadronJets*", kTRUE);
  fTree->SetBranchStatus("fMCCone4HadronJets*",   kTRUE);
  fTree->SetBranchStatus("fN_Electrons*",         kTRUE);
  fTree->SetBranchStatus("fElectrons*",           kTRUE);
  fTree->SetBranchStatus("fN_Muons*",             kTRUE);
  fTree->SetBranchStatus("fMuons*",               kTRUE);
  fTree->SetBranchStatus("fN_Taus*",              kTRUE);
  fTree->SetBranchStatus("fTaus*",                kTRUE);
  fTree->SetBranchStatus("fN_Photons*",           kTRUE);
  fTree->SetBranchStatus("fPhotons*",             kTRUE);
  fTree->SetBranchStatus("fN_AtlFastElectrons*",  kTRUE);
  fTree->SetBranchStatus("fAtlFastElectrons*",    kTRUE);
  fTree->SetBranchStatus("fN_AtlFastMuons*",      kTRUE);
  fTree->SetBranchStatus("fAtlFastMuons*",        kTRUE);
  fTree->SetBranchStatus("fN_AtlFastPhotons*",    kTRUE);
  fTree->SetBranchStatus("fAtlFastPhotons*",      kTRUE);
  fTree->SetBranchStatus("fN_AtlFastJets*",       kTRUE);
  fTree->SetBranchStatus("fAtlFastJets*",         kTRUE);
}

//____________________________________________________________________

void AtlCtrlPlots::FillHistograms(Bool_t EvtSelection) {
    //
    // Fill control histograms
    // In case the EvtSelection bit is set all histograms
    // after event selection are filled - otherwise the ones
    // before the event selection are filled
    //
    
    // Fill histograms
    FillJetHistograms(EvtSelection);
    FillElectronHistograms(EvtSelection);
    FillMuonHistograms(EvtSelection);
    FillTauHistograms(EvtSelection);
    FillPhotonHistograms(EvtSelection);
    if(fEvent->IsMC()) FillAtlFastHistograms(EvtSelection);
    FillEnergySumsHistograms(EvtSelection);
    FillTriggerHistograms(EvtSelection);
}

//____________________________________________________________________

void AtlCtrlPlots::BookEnergySumsHistograms() {
  //
  // Book energy sums information, three basic ones for now:
  // -> Missing Energy Amplitude
  // -> Missing Energy Phi Angle
  // -> Scalar Sum (every Transverse Energy or Momentum is summed up, indipendently from the relative direction.
  // 
  //    
  // Book histograms showing MC-vs-Rec quantities  
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
    
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("EnergySums", "Energy Sums Ctrl Plots");
  gDirectory->cd("EnergySums");

  h1 = new TH1F("h_Energy_Scalar_Sum",
		"Energy Scalar Sum",
		100, 0, 1000);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("Energy scalar sum(GeV)");
  h1->SetYTitle("Number of Entries");

  // Missing Energy Magnitude
  h1 = new TH1F("h_Missing_Energy_Magnitude",
		"Magnitude of the Missing Energy",
		80, 0, 300);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("Missing Energy (GeV)");
  h1->SetYTitle("Number of Entries");

  // Missing Energy Phi angle
  h1 = new TH1F("h_Missing_Energy_PhiAngle",
		"Phi Angle of the Missing Energy",
		100, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi_{Missing Energy} (rad)");
  h1->SetYTitle("Number of Entries");
     
  // Ex-Ey plane
  h2 = new TH2F("h_Missing_Energy_xyPLANE",
		"Ex_miss vs Ey_miss x-y plane",
		100, -300., 300., 
		100, -300., 300.);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("E_{x,miss}");
  h2->SetYTitle("E_{y,miss}");
  h2->SetZTitle("Number of Entries");

  // --------------------------------------------------------
  // MC vs REC comparison (MCTruthNonInt quantities are used)
  // --------------------------------------------------------
  
  // Missing Energy Phi Angle

  // -> 2-Dim Distribution for comparison
  h2 = new TH2F("h_Missing_Energy_PhiAngle_MCvsREC",
		"Phi Angle of the Missing Energy, MC vs REC comparison",
		100, -3.2, 3.2,
		100, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#phi_{Missing Energy,MCTruthNonInt} (rad)");
  h2->SetYTitle("#phi_{Missing Energy,REC} (rad)");
  // -> Signed Difference
  h1 = new TH1F("h_Missing_Energy_PhiAngle_DELTA_MC-REC",
		"Phi Angle of the Missing Energy, MCTruthNonInt vs REC comparison",
		100, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#Delta#phi_{MCTruthNonInt - REC}signed (rad)");
  // -> Signed Difference (MC normalized)
  h1 = new TH1F("h_Missing_Energy_PhiAngle_DELTA_MC-REC_norm",
		"Phi Angle of the Missing Energy, MCTruthNonInt vs REC comparison, MC-normalized",
		100, -3, 3);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#Delta#phi_{MCTruthNonInt - REC}/#phi_{MCTruthNonInt}signed }");
  

  // Missing Energy Magnitude

  // -> 2-Dim Distribution 
  h2 = new TH2F("h_Missing_Energy_Magnitude_MCvsREC",
		"Magnitude of the Missing Energy, MCTruthNonInt vs REC comparison",
		100, 0., 300.,
		100, 0., 300.);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("ME_{T,MCTruthNonInt} (GeV)");
  h2->SetYTitle("ME_{T,REC} (GeV)");
  // -> Signed Difference
  h1 = new TH1F("h_Missing_Energy_Magnitude_DELTA_MC-REC",
		"Magnitude of the Missing Energy, MCTruthNonInt - REC",
		100, -100., 100.);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#DeltaMET_{MCTruthNonInt - REC}signed (GeV)");
  // -> Signed Difference (MC normalized)
  h1 = new TH1F("h_Missing_Energy_Magnitude_DELTA_MC-REC_norm",
		"Magnitude of the Missing Energy, MCTruthNonInt - REC comparison, MC-normalized",
		100, -3., 3.);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#DeltaMET_{MCTruthNonInt - REC}signed (GeV)/#MET_{MCTruthNonInt}signed }");

  // Scalar Energy Sum 
  
  // -> 2-Dim Distribution 
  h2 = new TH2F("h_Energy_Scalar_Sum_MCvsREC",
		"Transverse Energies Scalar Sum, MC vs REC comparison",
		100, 0., 1000.,
		100, 0., 1000.);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("E_Scalar_Sum_{MCTruthInt} (GeV)");
  h2->SetYTitle("E_Scalar_Sum_{REC} (GeV)");

  // -> Signed Difference
  h1 = new TH1F("h_Energy_Scalar_Sum_DELTA_MC-REC",
		"E_Scalar_Sum, MCTruthInt - REC",
		50, -150., 150.);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#DeltaE_Scalar_Sum_{MCTruthInt - REC}signed (GeV)");

  // Correlations?
  //  -> 2-Dim Distr.: MCNonIntParticles_summedEta vs Phi(REC MissingEt)
  h2 = new TH2F("h_MCNonIntEta_RECPhiMET",
		"Eta of MC non-interacting particles vs Phi(MET,REC)",
		100, -5., 5.,
		100, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta_{MCTruthNonInt} (rad)");
  h2->SetYTitle("#phi_{Missing Energy,REC} (rad)");

  //  -> 2-Dim Distr.: MCNonIntParticles_summedEta vs MCNonIntParticles_summedPhi
  h2 = new TH2F("h_MCNonIntEta_Phi",
		"Eta vs Phi of MC non-interacting particles",
		100, -5., 5.,
		100, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta_{MCTruthNonInt} (rad)");
  h2->SetYTitle("#phi_{MCTruthNonInt} (rad)");


  savdir->cd();
}

//____________________________________________________________________

void AtlCtrlPlots::BookJetHistograms() {
  //
  // Book ctrl plots for jets
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
  TProfile *prof1 = 0;
    
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("Jets", "Jet Ctrl Plots");
  gDirectory->cd("Jets");

  
  // AntiKt4-H1-Topo Jets  ++++++++++++++++++++++++++++++
  gDirectory->mkdir("AntiKt4H1Topo_Jets", "Jet Ctrl Plots");
  gDirectory->cd("AntiKt4H1Topo_Jets");


  // =====================================
  // AntiKt4-H1-Topo jets @ detector level
  // =====================================
    
  // No. of jets
  h1 = new TH1F("h_antikt4H1topo_njets",
		"No. of AntiKt4-H1-Topo Jets",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{Jets}");
  h1->SetYTitle("Number of Entries");
    
  // Eta-phi plane
  h2 = new TH2F("h_antikt4H1topo_etaphi",
		"AntiKt4-H1-Topo Jets #eta-#phi plane",
		100, -5, 5, 128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta");
  h2->SetYTitle("#phi");
  h2->SetZTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_antikt4H1topo_Et",
		"AntiKt4-H1-Topo Jets Transverse Energy",
		200, 0, 1000);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("E_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Eta
  h1 = new TH1F("h_antikt4H1topo_eta",
		"AntiKt4-H1-Topo Jets #eta",
		100, -5, 5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_antikt4H1topo_phi",
		"AntiKt4-H1-Topo Jets #phi",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi");
  h1->SetYTitle("Number of Entries");


  // ...........................
  // Truth vs Reconstructed Jets
  // ...........................
  
  gDirectory->mkdir("MC_vs_REC_PLOTS", "Truth vs Reconstructed Jets");
  gDirectory->cd("MC_vs_REC_PLOTS");

  // Eta_MC - Eta_DL

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned",
		"Eta difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta_{DL} - #eta_{HL}");
  // -> signed difference - normalized
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned_norm",
		"Eta difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet - normalized to MC quantity",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta_{DL} - #eta_{HL} / #eta_{HL}");

  // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_Eta",
		"2-Dim Plot: Eta_MC vs Eta_REC",
		100, -4., 4., 
		100, -4., 4. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta_{HL}");
  h2->SetYTitle("#eta_{DL}");
    

  // Phi_MC - Phi_REC

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned",
		"Phi difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi_{DL} - #phi_{HL}");
  // -> signed difference - normalized
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned_norm",
		"Phi difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet - normalized to MC quantity",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi_{DL} - #phi_{HL}");
  // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_Phi",
		"2-Dim Plot: Phi_MC vs Phi_REC",
		100, -3.2, 3.2, 
		100, -3.2, 3.2 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#phi_{HL}");
  h2->SetYTitle("#phi_{DL}");


 
  // Pt_MC - Pt_REC

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaPtSigned",
		"Pt difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet",
		100, -30, 30);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t,DL} - P_{t,HL}");
  // Pt_MC - Pt_REC

  // -> signed difference -normalized
  h1 = new TH1F("h_MC_vs_REC_antikt4H1topo_DeltaPtSigned_norm",
		"Pt difference between reconstructed Jet and MC best matched MCAntiKt4HadronJet - normalized",
		100, -2., 2.);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t,DL} - P_{t,HL}");
   // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_Pt",
		"2-Dim Plot: Pt_MC vs Pt_REC",
		100, 0 , 300. , 
		100, 0, 300. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("P_{t,HL} (GeV)");
  h2->SetYTitle("P_{t,DL} (GeV)");

   // -> 2-dim confrontation: Delta ET vs ET-TRUTH
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt",
		"2-Dim Plot: Pt_MC vs DeltaPt",
		100, 0 , 300. , 
		100, -40., 40. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" P_{t,HL} (GeV)");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL} (GeV)");

   // -> 2-dim confrontation:  Delta ET vs ETA-TRUTH
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt",
		"2-Dim Plot: EtaMC vs DeltaPt",
		100, -3. , 3. , 
		100, -40., 40. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" #eta_{HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL} (GeV)");


   // -> 2-dim confrontation: Delta ET-norm vs ET-TRUTH 
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm",
		"2-Dim Plot: Pt_MC vs DeltaPt-norm",
		100, 0 , 300. , 
		100, -0.5, 0.5 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" P_{t,HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL}/P_{t,HL}");
   // -> 2-dim confrontation:  Delta ET-norm vs ETA-TRUTH
  h2 = new TH2F("h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm",
		"2-Dim Plot: EtaMC vs DeltaPt",
		100, -3. , 3. , 
		100, -0.5, 0.5 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" #eta_{HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL}/P_{t,HL}");


  // ============================
  // Pt & Eta vs DeltaPt Profiles
  // ============================

   Double_t bins[7];
   bins[1]=25.;
   for(int i=2;i<7;i++){
     bins[i]=bins[i-1]*2;
   }

   // -> Delta ET ( ET-TRUTH )
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt",
  		       "Profile:  DeltaPt (Pt_MC)",
  		       6, bins , 
 		       -40., 40.  ,"s");
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" P_{t,HL}(GeV)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL} (GeV)");
   // -> Delta ET ( ETA-TRUTH)
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt",
		"2-Dim Plot:  DeltaPt (EtaMC)",
		100, -3. , 3. , 
		 -40., 40. );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #eta_{HL}");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}(GeV)");
   // -> Delta ET ( PHI-TRUTH)
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt",
			"2-Dim Plot:  DeltaPt (EtaMC)",
			100, -3.2 , 3.2 , 
			-40., 40. );
   // =======
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #phi_{HL} (rad)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL} (GeV)");
   
   // -> Delta ET-norm vs ET-TRUTH 
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (Pt_MC) -norm",
			6, bins , 
			-0.5, 0.5 ,"s");
   fHistogramsNoEvtSel->Add(prof1);

   prof1->SetXTitle(" P_{t,HL} (GeV)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");
   // ->  Delta ET-norm vs ETA-TRUTH
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (EtaMC) -norm",
			100, -3. , 3. , 
			-0.5, 0.5 );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #eta_{HL}");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");

   // ->  Delta ET-norm vs PHI-TRUTH
   prof1 = new TProfile("prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (phiMC) -norm",
			6, bins , 
			-0.5, 0.5 );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #phi_{HL} (rad)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");

   gDirectory->cd("../..");


  // Cone4-H1-Tower Jets ++++++++++++++++++++++++++++++
  gDirectory->mkdir("Cone4H1tower_Jets", "Jet Ctrl Plots");
  gDirectory->cd("Cone4H1tower_Jets");


  // ===================================
  // Cone4-H1-Tower jets @detector level
  // ===================================
    
  // No. of jets
  h1 = new TH1F("h_cone4H1tower_njets",
		"No. of Cone4-H1-Tower Jets",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{Jets}");
  h1->SetYTitle("Number of Entries");
    
  // Eta-phi plane
  h2 = new TH2F("h_cone4H1tower_etaphi",
		"Cone4-H1-Tower Jets #eta-#phi plane",
		100, -5, 5, 128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta");
  h2->SetYTitle("#phi");
  h2->SetZTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_cone4H1tower_Et",
		"Cone4-H1-Tower Jets Transverse Energy",
		200, 0, 1000);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("E_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Eta
  h1 = new TH1F("h_cone4H1tower_eta",
		"Cone4-H1-Tower Jets #eta",
		100, -5, 5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_cone4H1tower_phi",
		"Cone4-H1-Tower Jets #phi",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi");
  h1->SetYTitle("Number of Entries");


  // ............................
  // Truth vs Reconstructed Jets
  // ............................
  
  gDirectory->mkdir("MC_vs_REC_PLOTS", "Truth vs Reconstructed Jets");
  gDirectory->cd("MC_vs_REC_PLOTS");

  // Eta_MC - Eta_DL

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaEtaSigned",
		"Eta difference between reconstructed Jet and MC best matched MCCone4HadronJet",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta_{DL} - #eta_{HL}");
  // -> signed difference - normalized
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaEtaSigned_norm",
		"Eta difference between reconstructed Jet and MC best matched MCCone4HadronJet - normalized to MC quantity",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta_{DL} - #eta_{HL} / #eta_{HL}");

  // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_Eta",
		"2-Dim Plot: Eta_MC vs Eta_REC",
		100, -4., 4., 
		100, -4., 4. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta_{HL}");
  h2->SetYTitle("#eta_{DL}");
    

  // Phi_MC - Phi_REC

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaPhiSigned",
		"Phi difference between reconstructed Jet and MC best matched MCCone4HadronJet",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi_{DL} - #phi_{HL}");
  // -> signed difference - normalized
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaPhiSigned_norm",
		"Phi difference between reconstructed Jet and MC best matched MCCone4HadronJet - normalized to MC quantity",
		100, -1, 1);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi_{DL} - #phi_{HL}");
  // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_Phi",
		"2-Dim Plot: Phi_MC vs Phi_REC",
		100, -3.2, 3.2, 
		100, -3.2, 3.2 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#phi_{HL}");
  h2->SetYTitle("#phi_{DL}");


 
  // Pt_MC - Pt_REC

  // -> signed difference
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaPtSigned",
		"Pt difference between reconstructed Jet and MC best matched MCCone4HadronJet",
		100, -30, 30);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t,DL} - P_{t,HL}");
  // Pt_MC - Pt_REC

  // -> signed difference -normalized
  h1 = new TH1F("h_MC_vs_REC_cone4H1tower_DeltaPtSigned_norm",
		"Pt difference between reconstructed Jet and MC best matched MCCone4HadronJet - normalized",
		100, -2., 2.);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t,DL} - P_{t,HL}");
   // -> 2-dim confrontation
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_Pt",
		"2-Dim Plot: Pt_MC vs Pt_REC",
		100, 0 , 300. , 
		100, 0, 300. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("P_{t,HL} (GeV)");
  h2->SetYTitle("P_{t,DL} (GeV)");

   // -> 2-dim confrontation: Delta ET vs ET-TRUTH
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt",
		"2-Dim Plot: Pt_MC vs DeltaPt",
		100, 0 , 300. , 
		100, -40., 40. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" P_{t,HL} (GeV)");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL} (GeV)");

   // -> 2-dim confrontation:  Delta ET vs ETA-TRUTH
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt",
		"2-Dim Plot: EtaMC vs DeltaPt",
		100, -3. , 3. , 
		100, -40., 40. );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" #eta_{HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL} (GeV)");


   // -> 2-dim confrontation: Delta ET-norm vs ET-TRUTH 
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm",
		"2-Dim Plot: Pt_MC vs DeltaPt-norm",
		100, 0 , 300. , 
		100, -0.5, 0.5 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" P_{t,HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL}/P_{t,HL}");
   // -> 2-dim confrontation:  Delta ET-norm vs ETA-TRUTH
  h2 = new TH2F("h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm",
		"2-Dim Plot: EtaMC vs DeltaPt",
		100, -3. , 3. , 
		100, -0.5, 0.5 );
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle(" #eta_{HL}");
  h2->SetYTitle(" P_{t,DL}- P_{t,HL}/P_{t,HL}");


  // ============================
  // Pt & Eta vs DeltaPt Profiles
  // ============================
   bins[1]=25.;
   for(int i=2;i<7;i++){
     bins[i]=bins[i-1]*2;
   }

   // -> Delta ET ( ET-TRUTH )
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt",
  		       "Profile:  DeltaPt (Pt_MC)",
  		       6, bins , 
 		       -40., 40.  ,"s");
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" P_{t,HL}(GeV)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL} (GeV)");
   // -> Delta ET ( ETA-TRUTH)
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt",
		"2-Dim Plot:  DeltaPt (EtaMC)",
		100, -3. , 3. , 
		 -40., 40. );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #eta_{HL}");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}(GeV)");
   // -> Delta ET ( PHI-TRUTH)
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt",
			"2-Dim Plot:  DeltaPt (EtaMC)",
			100, -3.2 , 3.2 , 
			-40., 40. );
   // =======
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #phi_{HL} (rad)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL} (GeV)");
   
   // -> Delta ET-norm vs ET-TRUTH 
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (Pt_MC) -norm",
			6, bins , 
			-0.5, 0.5 ,"s");
   fHistogramsNoEvtSel->Add(prof1);

   prof1->SetXTitle(" P_{t,HL} (GeV)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");
   // ->  Delta ET-norm vs ETA-TRUTH
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (EtaMC) -norm",
			100, -3. , 3. , 
			-0.5, 0.5 );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #eta_{HL}");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");

   // ->  Delta ET-norm vs PHI-TRUTH
   prof1 = new TProfile("prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt_norm",
			"Profile:  DeltaPt (phiMC) -norm",
			6, bins , 
			-0.5, 0.5 );
   fHistogramsNoEvtSel->Add(prof1);
   prof1->SetXTitle(" #phi_{HL} (rad)");
   prof1->SetYTitle(" P_{t,DL} - P_{t,HL}/P_{t,HL}");

   gDirectory->cd("../");
   
   
   // Restore pwd
   savdir->cd();
}

//____________________________________________________________________

void AtlCtrlPlots::BookElectronHistograms() {
  //
  // Book ctrl plots for electrons
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
  TProfile *prof1 = 0;
    
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("Electrons", "Electron Ctrl Plots");
  gDirectory->cd("Electrons");


  //******************************
  // Number of electrons by author
  //******************************
  h1 = new TH1F("h_electron_author",
		"Electron Author",
		AtlEMShower::fgNAuthors, 0, AtlEMShower::fgNAuthors);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  TAxis *axis = h1->GetXaxis();
  for ( Int_t k = 0; k < AtlEMShower::fgNAuthors; k++ ) {
    axis->SetBinLabel(k+1, AtlEMShower::fgAuthorNames[k]);
  }
  //******************************

  //*********************************
  // Number of electrons by IsEM-Flag
  //*********************************
  h1 = new TH1F("h_electron_isem",
		"Electron IsEM-Flag",
		AtlEMShower::fgNIsEM, 0, AtlEMShower::fgNIsEM);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  axis = h1->GetXaxis();
  for ( Int_t k = 0; k < AtlEMShower::fgNIsEM; k++ ) {
    axis->SetBinLabel(k+1, AtlEMShower::fgIsEMNames[k]);
  }
  //*********************************

  // ---------------------------------------------------------


  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // -----------------------------------
  // Distinction by author and IsEM flag
  // -----------------------------------

  
  for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {

    // ---------------------
    // Distinction by author
    // ---------------------

    // Making sure author is not invalid
    if ( 1<<i_author & AtlEMShower::kInvalid ) continue;

    // Retrieving author name
    TString author_name = AtlEMShower::fgAuthorNames[i_author];

      
    //-------------------------------
    // Creating author-sorted folders
    //-------------------------------
      
    gDirectory->mkdir(author_name.Data(),Form("%s Electron Ctrl Plots",author_name.Data()));
    gDirectory->cd(author_name.Data());

    
    for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
      
      // .........................
      // Distinction by IsEM-Flag
      // .........................
	  
      // Making sure IsEM-Flag is not invalid
      if ( 1<<i_isem & kInvalid ) continue;
	  
      // Retrieving IsEM-Flag name
      TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
	 

      // Shorthand notation "IsEMFlag_Author" for use in histogram names
      TString isemflag_author = Form("%s_%s",isemflag_name.Data(),author_name.Data());
	  
      // ................................
      // Creating IsEMFlag-sorted folders
      // ................................
	  
      gDirectory->mkdir(isemflag_name.Data(),Form("%s %s Electron Ctrl Plots",
						  isemflag_name.Data(),author_name.Data()));
      gDirectory->cd(isemflag_name.Data());
	  
	  
      // ................................
      // Truth vs Reconstructed Electrons
      // ................................
      
      gDirectory->mkdir("MC_vs_REC_plots",
			Form("Truth vs Reconstructed %s %s Electrons",
			     isemflag_name.Data(),author_name.Data()));
      gDirectory->cd("MC_vs_REC_plots");
	  

      // Eta_MC - Eta_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaEtaSigned",
			 isemflag_author.Data()),
		    Form("#eta difference between reconstructed %s %s electron and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta_{REC} - #eta_{MC}");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaEtaSigned_norm",
			 isemflag_author.Data()),
		    Form("#eta difference between reconstructed %s %s electron and MC best matching particle, normalized to MC-truth",
			 isemflag_name.Data(),author_name.Data()),
		    100, -0.2, 0.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta_{REC} - #eta_{MC} / #eta_{MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_electrons_Eta",isemflag_author.Data()),
		    Form("2-Dim Plot: #eta_{MC} vs #eta_{REC} of %s %s electrons",
			 isemflag_name.Data(),author_name.Data()),
		    100, -5., 5., 
		    100, -5., 5. );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("#eta_{MC}");
      h2->SetYTitle("#eta_{REC}");
	  
	  

      // Phi_MC - Phi_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaPhiSigned",isemflag_author.Data()),
		    Form("#phi difference between reconstructed %s %s electron and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi_{REC} - #phi_{MC} (rad)");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaPhiSigned_norm",
			 isemflag_author.Data()),
		    Form("#phi difference between reconstructed %s %s electron and MC best matching particle, normalized to MC-truth",
			 isemflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi_{REC} - #phi_{MC} / #phi_{MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_electrons_Phi",isemflag_author.Data()),
		    Form("2-Dim Plot: #phi_{MC} vs #phi_{REC} of %s %s electrons",
			 isemflag_name.Data(),author_name.Data()),
		    100, -3.2, 3.2, 
		    100, -3.2, 3.2 );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("#phi_{MC} (rad)");
      h2->SetYTitle("#phi_{REC}(rad)");

 
      // Pt_MC - Pt_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaPtSigned",isemflag_author.Data()),
		    Form("p_{t} difference between reconstructed %s %s electron and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		    100, -20, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t,REC} - p_{t,MC} (GeV)");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_electrons_DeltaPtSigned_norm",
			 isemflag_author.Data()),
		    Form("p_{t} difference between reconstructed %s %s electron and MC best matching particle, normalized to MC-truth",
			 isemflag_name.Data(),author_name.Data()),
		    100, -0.5, 0.5);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t,REC} - p_{t,MC} / p_{t,MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_electrons_Pt",isemflag_author.Data()),
		    Form("2-Dim Plot: p_{t,MC} vs p_{t,REC} of %s %s electrons",
			 isemflag_name.Data(),author_name.Data()),
		    100, 0, 300., 
		    100, 0, 300. );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("p_{t,MC} (GeV)");
      h2->SetYTitle("p_{t,REC} (GeV)");
	  
  
      // ===============
      //     PROFILES
      // ===============
	  
      // -> Delta PT vs PT-TRUTH 	  
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_PtMC_vs_DeltaPt",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} vs p_{t,MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, 0, 300., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" p_{t,MC} (GeV)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
      // ->  Delta PT vs ETA-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_EtaMC_vs_DeltaPt",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} vs #eta_{MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, -5., 5., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #eta_{MC}");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
  
      // ->  Delta PT vs PHI-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_PhiMC_vs_DeltaPt",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} vs #phi_{MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, -3.2, 3.2, 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #phi_{MC} (rad)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
      // -> Delta PT (norm.) vs PT-TRUTH 
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_PtMC_vs_DeltaPt_norm",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} (norm.) vs p_{t,MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, 0, 300., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" p_{t,MC} (GeV)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
      // ->  Delta PT (norm.) vs ETA-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_EtaMC_vs_DeltaPt_norm",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} (norm.) vs #eta_{MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, -5., 5., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #eta_{MC}");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");

      // ->  Delta PT (norm.) vs PHI-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_electrons_PhiMC_vs_DeltaPt_norm",
				isemflag_author.Data()),
			   Form("Profile: #Delta p_{t} (norm.) vs #phi_{MC} of %s %s electrons",isemflag_name.Data(),author_name.Data()),
			   100, -3.2, 3.2, 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #phi_{MC} (rad)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
      gDirectory->cd("..");
    
	  
      // ============
      // Both charges
      // ============
	  
      gDirectory->mkdir("Both", Form("%s %s charge-incl. Electron/Positron Ctrl Plots",
				     isemflag_name.Data(),author_name.Data()));
      gDirectory->cd("Both");
	  
    
      // Eta of incl. electrons:
      h1 = new TH1F(Form("h_%s_electron_eta",isemflag_author.Data()),
		    Form("%s %s Incl. Electron Pseudo-Rapidity",
			 isemflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Phi of incl. electrons:
      h1 = new TH1F(Form("h_%s_electron_phi",isemflag_author.Data()),
		    Form("%s %s Incl. Electron Azimuth",
			 isemflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Pt of incl. electrons:
      h1 = new TH1F(Form("h_%s_electron_pt",isemflag_author.Data()),
		    Form("%s %s Incl. Electron Transverse Momentum",
			 isemflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");
	  
    
      // ---> Cone Isolation Energy Plots
	  
      // Et inside cones of 0.2, 0.3, 0.4, 0.45 (isolation) for
      // incl. electrons:
      h1 = new TH1F(Form("h_%s_electron_EtCone20",isemflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.2 (%s %s electrons)",
			 isemflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_electron_EtCone30",isemflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.3 (%s %s electrons)",
			 isemflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_electron_EtCone40",isemflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.4 (%s %s electrons)",
			 isemflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_electron_EtCone45",isemflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.45 (%s %s electrons)",
			 isemflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
	  
      gDirectory->cd("..");
    

      // ===================
      // Positrons (e+) only
      // ===================
	  
      gDirectory->mkdir("Eplus", Form("%s %s Positron Ctrl Plots",
				      isemflag_name.Data(),author_name.Data()));
      gDirectory->cd("Eplus");
	  
	  
      // Eta of positrons:
      h1 = new TH1F(Form("h_%s_eplus_eta",isemflag_author.Data()),
		    Form("%s %s e^{+} Pseudo-Rapidity",
			 isemflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Phi of positrons:   
      h1 = new TH1F(Form("h_%s_eplus_phi",isemflag_author.Data()),
		    Form("%s %s e^{+} Azimuth",
			 isemflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Pt of positrons:
      h1 = new TH1F(Form("h_%s_eplus_pt",isemflag_author.Data()),
		    Form("%s %s e^{+} Transverse Momentum",
			 isemflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");
	  
	  
	  
      gDirectory->cd("..");
  

      // ===================
      // Electrons (e-) only
      // ===================
	  
      gDirectory->mkdir("Eminus", Form("%s %s Electron Ctrl Plots",
				       isemflag_name.Data(),author_name.Data()));
      gDirectory->cd("Eminus");
	  
			
      // Eta of electrons:
      h1 = new TH1F(Form("h_%s_eminus_eta",isemflag_author.Data()),
		    Form("%s %s e^{-} Pseudo-Rapidity",
			 isemflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");


      // Phi of electrons:   
      h1 = new TH1F(Form("h_%s_eminus_phi",isemflag_author.Data()),
		    Form("%s %s e^{-} Azimuth",
			 isemflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");

    
      // Pt of electrons:
      h1 = new TH1F(Form("h_%s_eminus_pt",isemflag_author.Data()),
		    Form("%s %s e^{-} Transverse Momentum",
			 isemflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");


    
      gDirectory->cd("../../");

    }
      
    gDirectory->cd("..");
  }
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
  //---------------------------------------------------------------------
  // Number of inclusive electrons with and without distinction of charge
  //---------------------------------------------------------------------

  gDirectory->mkdir("N_electrons","Number of incl. Electrons/Positrons (Ctrl Plots)");
  gDirectory->cd("N_electrons");
  
  
  // No. of incl. electrons:
  h1 = new TH1F("h_all_electron_n","No. of all Incl. Electrons",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e}");
  h1->SetYTitle("Number of Entries");

  // No. of incl. e+:
  h1 = new TH1F("h_all_eplus_n","No. of all Incl. e^{+}",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e^{+}}");
  h1->SetYTitle("Number of Entries");
  
  // No. of incl. e-:
  h1 = new TH1F("h_all_eminus_n","No. of all Incl. e^{-}",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e^{-}}");
  h1->SetYTitle("Number of Entries");

  
  gDirectory->cd("..");
  
  //---------------------------------------------------------------------
  
  // Restore pwd
  savdir->cd();
}

//____________________________________________________________________

void AtlCtrlPlots::BookMuonHistograms() {
  //
  // Book ctrl plots for muons
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
  TProfile *prof1 = 0;
    
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("Muons", "Muon Ctrl Plots");
  gDirectory->cd("Muons");

    
  // Author



  //******************************
  // Number of muons by author
  //******************************
  h1 = new TH1F("h_muon_author",
		"Muon Author",
		AtlMuon::fgNAuthors, 0, AtlMuon::fgNAuthors);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  TAxis *axis = h1->GetXaxis();
  for ( Int_t i = 0; i < AtlMuon::fgNAuthors; i++ ) {
      axis->SetBinLabel(i+1, AtlMuon::fgAuthorNames[i]);
  }
  //******************************

  //*********************************
  // Number of muons by Quality-Flag
  //*********************************
  h1 = new TH1F("h_muon_quality",
		"Muon Quality-Flag",
		AtlMuon::fgNQuality, 0, AtlMuon::fgNQuality);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  axis = h1->GetXaxis();
  for ( Int_t k = 0; k < AtlMuon::fgNQuality; k++ ) {
      axis->SetBinLabel(k+1, AtlMuon::fgQualityNames[k]);
  }
  //*********************************

  // ---------------------------------------------------------


  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ---------------------------------------
  // Distinction by author and Quality flag
  // ---------------------------------------

  
  for ( Int_t i_author = 0; i_author < AtlMuon::fgNAuthors; i_author++ ) {

    // ---------------------
    // Distinction by author
    // ---------------------

    // Making sure author is not invalid
    if ( 1<<i_author & AtlMuon::kInvalid ) continue;

    // Retrieving author name
    TString author_name = AtlMuon::fgAuthorNames[i_author];

      
    //-------------------------------
    // Creating author-sorted folders
    //-------------------------------
      
    gDirectory->mkdir(author_name.Data(),Form("%s Muon Ctrl Plots",author_name.Data()));
    gDirectory->cd(author_name.Data());

    
    for ( Int_t i_quality = 0; i_quality < AtlMuon::fgNQuality; i_quality++ ) {
      
      // ...........................
      // Distinction by Quality-Flag
      // ...........................
	  
      // Making sure Quality-Flag is not invalid
      if ( 1<<i_quality & kInvalid ) continue;
	  
      // Retrieving Quality-Flag name
      TString qualityflag_name = AtlMuon::fgQualityNames[i_quality];
	 

      // Shorthand notation "QualityFlag_Author" for use in histogram names
      TString qualityflag_author = Form("%s_%s",qualityflag_name.Data(),author_name.Data());
	  
      // ...................................
      // Creating QualityFlag-sorted folders
      // ...................................
	  
      gDirectory->mkdir(qualityflag_name.Data(),Form("%s %s Muon Ctrl Plots",
						     qualityflag_name.Data(),author_name.Data()));
      gDirectory->cd(qualityflag_name.Data());
	  
	  
      // .............................
      // Truth vs Reconstructed Muons
      // .............................
      
      gDirectory->mkdir("MC_vs_REC_plots",
			Form("Truth vs Reconstructed %s %s Muons",
			     qualityflag_name.Data(),author_name.Data()));
      gDirectory->cd("MC_vs_REC_plots");
	  

      // Eta_MC - Eta_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaEtaSigned",
			 qualityflag_author.Data()),
		    Form("#eta difference between reconstructed %s %s muon and MC best matching particle",qualityflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta_{REC} - #eta_{MC}");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaEtaSigned_norm",
			 qualityflag_author.Data()),
		    Form("#eta difference between reconstructed %s %s muon and MC best matching particle, normalized to MC-truth",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -0.2, 0.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta_{REC} - #eta_{MC} / #eta_{MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_muons_Eta",qualityflag_author.Data()),
		    Form("2-Dim Plot: #eta_{MC} vs #eta_{REC} of %s %s muons",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -5., 5., 
		    100, -5., 5. );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("#eta_{MC}");
      h2->SetYTitle("#eta_{REC}");
	  
	  

      // Phi_MC - Phi_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaPhiSigned",qualityflag_author.Data()),
		    Form("#phi difference between reconstructed %s %s muon and MC best matching particle",qualityflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi_{REC} - #phi_{MC} (rad)");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaPhiSigned_norm",
			 qualityflag_author.Data()),
		    Form("#phi difference between reconstructed %s %s muon and MC best matching particle, normalized to MC-truth",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -0.05, 0.05);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi_{REC} - #phi_{MC} / #phi_{MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_muons_Phi",qualityflag_author.Data()),
		    Form("2-Dim Plot: #phi_{MC} vs #phi_{REC} of %s %s muons",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -3.2, 3.2, 
		    100, -3.2, 3.2 );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("#phi_{MC} (rad)");
      h2->SetYTitle("#phi_{REC}(rad)");

 
      // Pt_MC - Pt_REC
	  
      // -> signed difference
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaPtSigned",qualityflag_author.Data()),
		    Form("p_{t} difference between reconstructed %s %s muon and MC best matching particle",qualityflag_name.Data(),author_name.Data()),
		    100, -20, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t,REC} - p_{t,MC} (GeV)");
	  
      // -> signed difference - normalized
      h1 = new TH1F(Form("h_MC_vs_REC_%s_muons_DeltaPtSigned_norm",
			 qualityflag_author.Data()),
		    Form("p_{t} difference between reconstructed %s %s muon and MC best matching particle, normalized to MC-truth",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -0.5, 0.5);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t,REC} - p_{t,MC} / p_{t,MC}");
	  
      // -> 2-dim confrontation
      h2 = new TH2F(Form("h_MC_vs_REC_%s_muons_Pt",qualityflag_author.Data()),
		    Form("2-Dim Plot: p_{t,MC} vs p_{t,REC} of %s %s muons",
			 qualityflag_name.Data(),author_name.Data()),
		    100, 0, 300., 
		    100, 0, 300. );
      fHistogramsNoEvtSel->Add(h2);
      h2->SetXTitle("p_{t,MC} (GeV)");
      h2->SetYTitle("p_{t,REC} (GeV)");
	  
  
      // ===============
      //     PROFILES
      // ===============
	  
      // -> Delta PT vs PT-TRUTH 	  
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_PtMC_vs_DeltaPt",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} vs p_{t,MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, 0, 300., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" p_{t,MC} (GeV)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
      // ->  Delta PT vs ETA-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_EtaMC_vs_DeltaPt",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} vs #eta_{MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, -5., 5., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #eta_{MC}");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
  
      // ->  Delta PT vs PHI-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_PhiMC_vs_DeltaPt",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} vs #phi_{MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, -3.2, 3.2, 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #phi_{MC} (rad)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
      // -> Delta PT (norm.) vs PT-TRUTH 
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_PtMC_vs_DeltaPt_norm",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} (norm.) vs p_{t,MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, 0, 300., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" p_{t,MC} (GeV)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
      // ->  Delta PT (norm.) vs ETA-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_EtaMC_vs_DeltaPt_norm",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} (norm.) vs #eta_{MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, -5., 5., 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #eta_{MC}");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");

      // ->  Delta PT (norm.) vs PHI-TRUTH
      prof1 = new TProfile(Form("prof_MC_vs_REC_%s_muons_PhiMC_vs_DeltaPt_norm",
				qualityflag_author.Data()),
			   Form("#Delta p_{t} (norm.) vs #phi_{MC} of %s %s muons",qualityflag_name.Data(),author_name.Data()),
			   100, -3.2, 3.2, 
			   -0.5, 0.5 );
      fHistogramsNoEvtSel->Add(prof1);
      prof1->SetXTitle(" #phi_{MC} (rad)");
      prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
      gDirectory->cd("..");
      
	  
      // ============
      // Both charges
      // ============
	  
      gDirectory->mkdir("Both", Form("%s %s charge-incl. Mu-/Mu+ Ctrl Plots",
				     qualityflag_name.Data(),author_name.Data()));
      gDirectory->cd("Both");
	  
    
      // Eta of incl. muons:
      h1 = new TH1F(Form("h_%s_muon_eta",qualityflag_author.Data()),
		    Form("%s %s Incl. Muon Pseudo-Rapidity",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Phi of incl. muons:
      h1 = new TH1F(Form("h_%s_muon_phi",qualityflag_author.Data()),
		    Form("%s %s Incl. Muon Azimuth",
			 qualityflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Pt of incl. muons:
      h1 = new TH1F(Form("h_%s_muon_pt",qualityflag_author.Data()),
		    Form("%s %s Incl. Muon Transverse Momentum",
			 qualityflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");
	  
    
      // ---> Cone Isolation Energy Plots
	  
      // Et inside cones of 0.1, 0.2, 0.3, 0.4 (isolation) for
      // incl. muons:
      h1 = new TH1F(Form("h_%s_muon_EtCone10",qualityflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.1 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_EtCone20",qualityflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.2 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_EtCone30",qualityflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.3 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_EtCone40",qualityflag_author.Data()),
		    Form("E_{t} inside cone of dR=0.4 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("E_{t}-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  

      // ---> Summed Pt of charged tracks inside
      // cones of 0.1, 0.2, 0.3, 0.4
      h1 = new TH1F(Form("h_%s_muon_PtTrkCone10",qualityflag_author.Data()),
		    Form("SumPt of charged tracks inside cone of dR=0.1 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("( #sum P_{t}^{chg} )-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_PtTrkCone20",qualityflag_author.Data()),
		    Form("SumPt of charged tracks inside cone of dR=0.2 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("( #sum P_{t}^{chg} )-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_PtTrkCone30",qualityflag_author.Data()),
		    Form("SumPt of charged tracks inside cone of dR=0.3 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("( #sum P_{t}^{chg} )-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_PtTrkCone40",qualityflag_author.Data()),
		    Form("SumPt of charged tracks inside cone of dR=0.4 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    50, 0, 100);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("( #sum P_{t}^{chg} )-cone (GeV)");
      h1->SetYTitle("Number of Entries");
	  
      // ---> No. of charged tracks inside cones of 0.1, 0.2, 0.3, 0.4
      h1 = new TH1F(Form("h_%s_muon_NtrkCone10",qualityflag_author.Data()),
		    Form("No. of charged tracks inside cone of dR=0.1 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    20, 0, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("N_{trk}-cone");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_NtrkCone20",qualityflag_author.Data()),
		    Form("No. of charged tracks inside cone of dR=0.2 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    20, 0, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("N_{trk}-cone");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_NtrkCone30",qualityflag_author.Data()),
		    Form("No. of charged tracks inside cone of dR=0.3 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    20, 0, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("N_{trk}-cone");
      h1->SetYTitle("Number of Entries");
	  
      h1 = new TH1F(Form("h_%s_muon_NtrkCone40",qualityflag_author.Data()),
		    Form("No. of charged tracks inside cone of dR=0.4 (%s %s muons)",
			 qualityflag_name.Data(),author_name.Data()),
		    20, 0, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("N_{trk}-cone");
      h1->SetYTitle("Number of Entries");
  
      // ---> Track-segment matching
      h1 = new TH1F(Form("h_%s_muon_chi2",qualityflag_author.Data()),
		    Form("Track-Segment Matching for %s %s Muons",
			 qualityflag_name.Data(),author_name.Data()),
		    20, 0, 20);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#chi^{2}/NDoF");
      h1->SetYTitle("Number of Entries");
	  
	  
      gDirectory->cd("..");
    

      // ===================
      // Mu+ only
      // ===================
	  
      gDirectory->mkdir("Muplus", Form("%s %s Mu+ Ctrl Plots",
				       qualityflag_name.Data(),author_name.Data()));
      gDirectory->cd("Muplus");
	  
	  
      // Eta of mu+s:
      h1 = new TH1F(Form("h_%s_muplus_eta",qualityflag_author.Data()),
		    Form("%s %s #mu^{+} Pseudo-Rapidity",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Phi of mu+s:   
      h1 = new TH1F(Form("h_%s_muplus_phi",qualityflag_author.Data()),
		    Form("%s %s #mu^{+} Azimuth",
			 qualityflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");
	  
	  
      // Pt of mu+s:
      h1 = new TH1F(Form("h_%s_muplus_pt",qualityflag_author.Data()),
		    Form("%s %s #mu^{+} Transverse Momentum",
			 qualityflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");
	  
	  
	  
      gDirectory->cd("..");
  

      // ===================
      // Mu- only
      // ===================
	  
      gDirectory->mkdir("Muminus", Form("%s %s Muon Ctrl Plots",
					qualityflag_name.Data(),author_name.Data()));
      gDirectory->cd("Muminus");
	  
			
      // Eta of muons:
      h1 = new TH1F(Form("h_%s_muminus_eta",qualityflag_author.Data()),
		    Form("%s %s #mu^{-} Pseudo-Rapidity",
			 qualityflag_name.Data(),author_name.Data()),
		    100, -5., 5.);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#eta");
      h1->SetYTitle("Number of Entries");


      // Phi of muons:   
      h1 = new TH1F(Form("h_%s_muminus_phi",qualityflag_author.Data()),
		    Form("%s %s #mu^{-} Azimuth",
			 qualityflag_name.Data(),author_name.Data()),
		    128, -3.2, 3.2);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("#phi (rad)");
      h1->SetYTitle("Number of Entries");

    
      // Pt of muons:
      h1 = new TH1F(Form("h_%s_muminus_pt",qualityflag_author.Data()),
		    Form("%s %s #mu^{-} Transverse Momentum",
			 qualityflag_name.Data(),author_name.Data()),
		    200, 0, 200);
      fHistogramsNoEvtSel->Add(h1);
      h1->SetXTitle("p_{t} (GeV)");
      h1->SetYTitle("Number of Entries");


    
      gDirectory->cd("../../");

    }
      
    gDirectory->cd("..");
  }
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
  //---------------------------------------------------------------------
  // Number of inclusive muons with and without distinction of charge
  //---------------------------------------------------------------------

  gDirectory->mkdir("N_muons","Number of incl. Mu-/Mu+ (Ctrl Plots)");
  gDirectory->cd("N_muons");
  
  
  // No. of incl. muons:
  h1 = new TH1F("h_all_muon_n","No. of all Incl. Muons",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu}");
  h1->SetYTitle("Number of Entries");

  // No. of incl. mu+:
  h1 = new TH1F("h_all_muplus_n","No. of all Incl. #mu^{+}",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu^{+}}");
  h1->SetYTitle("Number of Entries");
  
  // No. of incl. mu-:
  h1 = new TH1F("h_all_muminus_n","No. of all Incl. #mu^{-}",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu^{-}}");
  h1->SetYTitle("Number of Entries");

  
  gDirectory->cd("..");
  
  //---------------------------------------------------------------------
      

  // Restore pwd
  savdir->cd();
}

//-------------------------------------------------------------------

void AtlCtrlPlots::BookTauHistograms() {
  //
  // Book ctrl plots for tauons
  //
  TH1F *h1 = 0;
    
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("Tauons", "Tauon Ctrl Plots");
  gDirectory->cd("Tauons");
 

  // ---------------------------------------------------------

  // ------------------------
  // Distinction by author
  // ------------------------


  // -------
  // TauRec   ------------------------------------------------
  // -------
  
  gDirectory->mkdir("TauRec", "TauRec Tauon Ctrl Plots");
  gDirectory->cd("TauRec");

    
    
  // ..................................
  // Distinction by Reconstruction-Flag
  // ..................................


  // ...............
  // TauCutTight
  // ...............

  gDirectory->mkdir("TauCutTight", "TauCutTight TauRec Tauon Ctrl Plots");
  gDirectory->cd("TauCutTight");
    

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", " TauCutTight TauRec incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutTight TauRec incl. tauons:
  h1 = new TH1F("h_tauctight_taurec_tauon_n",
		"No. of TauCutTight TauRec Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight TauRec incl. tauons:
  h1 = new TH1F("h_tauctight_taurec_tauon_eta",
		"TauCutTight TauRec Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight TauRec incl. tauons:
  h1 = new TH1F("h_tauctight_taurec_tauon_phi",
		"TauCutTight TauRec Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight TauRec incl. tauons:
  h1 = new TH1F("h_tauctight_taurec_tauon_pt",
		"TauCutTight TauRec Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutTight TauRec Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutTight TauRec antitauons:
  h1 = new TH1F("h_tauctight_taurec_tauplus_n",
		"No. of TauCutTight TauRec #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight TauRec antitauons:
  h1 = new TH1F("h_tauctight_taurec_tauplus_eta",
		"TauCutTight TauRec #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight TauRec antitauons:   
  h1 = new TH1F("h_tauctight_taurec_tauplus_phi",
		"TauCutTight TauRec #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight TauRec antitauons:
  h1 = new TH1F("h_tauctight_taurec_tauplus_pt",
		"TauCutTight TauRec #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutTight TauRec Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutTight TauRec tauons:
  h1 = new TH1F("h_tauctight_taurec_tauminus_n",
		"No. of TauCutTight TauRec #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight TauRec tauons:
  h1 = new TH1F("h_tauctight_taurec_tauminus_eta",
		"TauCutTight TauRec #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight TauRec tauons:   
  h1 = new TH1F("h_tauctight_taurec_tauminus_phi",
		"TauCutTight TauRec #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight TauRec tauons:
  h1 = new TH1F("h_tauctight_taurec_tauminus_pt",
		"TauCutTight TauRec #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../../");


  // ......
  // Medium
  // ......

  gDirectory->mkdir("TauCutMedium", "TauCutMedium TauRec Tauon Ctrl Plots");
  gDirectory->cd("TauCutMedium");

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", "TauCutMedium TauRec incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutMedium TauRec incl. tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauon_n",
		"No. of TauCutMedium TauRec Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium TauRec incl. tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauon_eta",
		"TauCutMedium TauRec Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium TauRec incl. tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauon_phi",
		"TauCutMedium TauRec Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium TauRec incl. tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauon_pt",
		"TauCutMedium TauRec Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutMedium TauRec Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutMedium TauRec antitauons:
  h1 = new TH1F("h_taucmedium_taurec_tauplus_n",
		"No. of TauCutMedium TauRec #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium TauRec antitauons:
  h1 = new TH1F("h_taucmedium_taurec_tauplus_eta",
		"TauCutMedium TauRec #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium TauRec antitauons:   
  h1 = new TH1F("h_taucmedium_taurec_tauplus_phi",
		"TauCutMedium TauRec #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium TauRec antitauons:
  h1 = new TH1F("h_taucmedium_taurec_tauplus_pt",
		"TauCutMedium TauRec #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutMedium TauRec Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutMedium TauRec tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauminus_n",
		"No. of TauCutMedium TauRec #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium TauRec tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauminus_eta",
		"TauCutMedium TauRec #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium TauRec tauons:   
  h1 = new TH1F("h_taucmedium_taurec_tauminus_phi",
		"TauCutMedium TauRec #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium TauRec tauons:
  h1 = new TH1F("h_taucmedium_taurec_tauminus_pt",
		"TauCutMedium TauRec #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../../");



  // ...............
  // TauCutLoose
  // ...............

  gDirectory->mkdir("TauCutLoose", "TauCutLoose TauRec Tauon Ctrl Plots");
  gDirectory->cd("TauCutLoose");

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", "TauCutLoose TauRec incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutLoose TauRec incl. tauons:
  h1 = new TH1F("h_taucloose_taurec_tauon_n",
		"No. of TauCutLoose TauRec Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose TauRec incl. tauons:
  h1 = new TH1F("h_taucloose_taurec_tauon_eta",
		"TauCutLoose TauRec Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose TauRec incl. tauons:
  h1 = new TH1F("h_taucloose_taurec_tauon_phi",
		"TauCutLoose TauRec Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose TauRec incl. tauons:
  h1 = new TH1F("h_taucloose_taurec_tauon_pt",
		"TauCutLoose TauRec Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutLoose TauRec Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutLoose TauRec antitauons:
  h1 = new TH1F("h_taucloose_taurec_tauplus_n",
		"No. of TauCutLoose TauRec #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose TauRec antitauons:
  h1 = new TH1F("h_taucloose_taurec_tauplus_eta",
		"TauCutLoose TauRec #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose TauRec antitauons:   
  h1 = new TH1F("h_taucloose_taurec_tauplus_phi",
		"TauCutLoose TauRec #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose TauRec antitauons:
  h1 = new TH1F("h_taucloose_taurec_tauplus_pt",
		"TauCutLoose TauRec #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutLoose TauRec Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutLoose TauRec tauons:
  h1 = new TH1F("h_taucloose_taurec_tauminus_n",
		"No. of TauCutLoose TauRec #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose TauRec tauons:
  h1 = new TH1F("h_taucloose_taurec_tauminus_eta",
		"TauCutLoose TauRec #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose TauRec tauons:   
  h1 = new TH1F("h_taucloose_taurec_tauminus_phi",
		"TauCutLoose TauRec #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose TauRec tauons:
  h1 = new TH1F("h_taucloose_taurec_tauminus_pt",
		"TauCutLoose TauRec #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../../../");


  // -------
  // Tau1P3P      ------------------------------------------------
  // -------
  
  gDirectory->mkdir("Tau1P3P", "Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("Tau1P3P");

    
  // ..................................
  // Distinction by Reconstruction-Flag
  // ..................................


  // ...............
  // TauCutTight
  // ...............

  gDirectory->mkdir("TauCutTight", "TauCutTight Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("TauCutTight");
    

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", "TauCutTight Tau1P3P incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutTight Tau1P3P incl. tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauon_n",
		"No. of TauCutTight Tau1P3P Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight Tau1P3P incl. tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauon_eta",
		"TauCutTight Tau1P3P Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight Tau1P3P incl. tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauon_phi",
		"TauCutTight Tau1P3P Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight Tau1P3P incl. tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauon_pt",
		"TauCutTight Tau1P3P Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutTight Tau1P3P Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutTight Tau1P3P antitauons:
  h1 = new TH1F("h_tauctight_1p3p_tauplus_n",
		"No. of TauCutTight Tau1P3P #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight Tau1P3P antitauons:
  h1 = new TH1F("h_tauctight_1p3p_tauplus_eta",
		"TauCutTight Tau1P3P #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight Tau1P3P antitauons:   
  h1 = new TH1F("h_tauctight_1p3p_tauplus_phi",
		"TauCutTight Tau1P3P #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight Tau1P3P antitauons:
  h1 = new TH1F("h_tauctight_1p3p_tauplus_pt",
		"TauCutTight Tau1P3P #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutTight Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutTight Tau1P3P tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauminus_n",
		"No. of TauCutTight Tau1P3P #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutTight Tau1P3P tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauminus_eta",
		"TauCutTight Tau1P3P #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutTight Tau1P3P tauons:   
  h1 = new TH1F("h_tauctight_1p3p_tauminus_phi",
		"TauCutTight Tau1P3P #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutTight Tau1P3P tauons:
  h1 = new TH1F("h_tauctight_1p3p_tauminus_pt",
		"TauCutTight Tau1P3P #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../../");


  // ................
  // TauCutMedium
  // ................

  gDirectory->mkdir("TauCutMedium", "TauCutMedium Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("TauCutMedium");

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", "TauCutMedium Tau1P3P incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutMedium Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauon_n",
		"No. of TauCutMedium Tau1P3P Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauon_eta",
		"TauCutMedium Tau1P3P Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauon_phi",
		"TauCutMedium Tau1P3P Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauon_pt",
		"TauCutMedium Tau1P3P Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutMedium Tau1P3P Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutMedium Tau1P3P antitauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauplus_n",
		"No. of TauCutMedium Tau1P3P #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium Tau1P3P antitauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauplus_eta",
		"TauCutMedium Tau1P3P #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium Tau1P3P antitauons:   
  h1 = new TH1F("h_taucmedium_1p3p_tauplus_phi",
		"TauCutMedium Tau1P3P #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium Tau1P3P antitauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauplus_pt",
		"TauCutMedium Tau1P3P #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutMedium Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutMedium Tau1P3P tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauminus_n",
		"No. of TauCutMedium Tau1P3P #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutMedium Tau1P3P tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauminus_eta",
		"TauCutMedium Tau1P3P #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutMedium Tau1P3P tauons:   
  h1 = new TH1F("h_taucmedium_1p3p_tauminus_phi",
		"TauCutMedium Tau1P3P #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutMedium Tau1P3P tauons:
  h1 = new TH1F("h_taucmedium_1p3p_tauminus_pt",
		"TauCutMedium Tau1P3P #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../../");



  // ...............
  // TauCutLoose
  // ...............

  gDirectory->mkdir("TauCutLoose", "TauCutLoose Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("TauCutLoose");

    
  // ============
  // Both charges
  // ============
    
  gDirectory->mkdir("Both", "TauCutLoose Tau1P3P incl. Tauon/Antitauon Ctrl Plots");
  gDirectory->cd("Both");

    
  // No. of TauCutLoose Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauon_n",
		"No. of TauCutLoose Tau1P3P Incl. Tauons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauon_eta",
		"TauCutLoose Tau1P3P Incl. Tauon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauon_phi",
		"TauCutLoose Tau1P3P Incl. Tauon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose Tau1P3P incl. tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauon_pt",
		"TauCutLoose Tau1P3P Incl. Tauon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");
    

  // ===================
  // Antitauons (tau+) only
  // ===================
    
  gDirectory->mkdir("Tauplus", "TauCutLoose Tau1P3P Antitauon Ctrl Plots");
  gDirectory->cd("Tauplus");

  // No. of TauCutLoose Tau1P3P antitauons:
  h1 = new TH1F("h_taucloose_1p3p_tauplus_n",
		"No. of TauCutLoose Tau1P3P #tau^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{+}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose Tau1P3P antitauons:
  h1 = new TH1F("h_taucloose_1p3p_tauplus_eta",
		"TauCutLoose Tau1P3P #tau^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose Tau1P3P antitauons:   
  h1 = new TH1F("h_taucloose_1p3p_tauplus_phi",
		"TauCutLoose Tau1P3P #tau^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose Tau1P3P antitauons:
  h1 = new TH1F("h_taucloose_1p3p_tauplus_pt",
		"TauCutLoose Tau1P3P #tau^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");


    
  gDirectory->cd("../");


  // ===================
  // Tauons (tau-) only
  // ===================
    
  gDirectory->mkdir("Tauminus", "TauCutLoose Tau1P3P Tauon Ctrl Plots");
  gDirectory->cd("Tauminus");

  // No. of TauCutLoose Tau1P3P tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauminus_n",
		"No. of TauCutLoose Tau1P3P #tau^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#tau^{-}}");
  h1->SetYTitle("Number of Entries");

    
  // Eta of TauCutLoose Tau1P3P tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauminus_eta",
		"TauCutLoose Tau1P3P #tau^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");


  // Phi of TauCutLoose Tau1P3P tauons:   
  h1 = new TH1F("h_taucloose_1p3p_tauminus_phi",
		"TauCutLoose Tau1P3P #tau^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

    
  // Pt of TauCutLoose Tau1P3P tauons:
  h1 = new TH1F("h_taucloose_1p3p_tauminus_pt",
		"TauCutLoose Tau1P3P #tau^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

    
  // Restore pwd
  savdir->cd();    
}

//____________________________________________________________________

void AtlCtrlPlots::BookPhotonHistograms() {
  //
  // Book ctrl plots for photons
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
  TProfile *prof1 = 0;
  
  // Save present working directory
  TDirectory *savdir = gDirectory;
    
  // Create sub-folder
  gDirectory->mkdir("Photons", "Photon Ctrl Plots");
  gDirectory->cd("Photons");
  
  
  //******************************
  // Number of photons by author
  //******************************
  h1 = new TH1F("h_photon_author",
		"Photon Author",
		AtlEMShower::fgNAuthors, 0, AtlEMShower::fgNAuthors);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  TAxis *axis = h1->GetXaxis();
  for ( Int_t k = 0; k < AtlEMShower::fgNAuthors; k++ )
      axis->SetBinLabel(k+1, AtlEMShower::fgAuthorNames[k]);
  //******************************
  
  //*******************************
  // Number of photons by IsEM-Flag
  //*******************************
  h1 = new TH1F("h_photon_isem",
		"Photon IsEM-Flag",
		AtlEMShower::fgNIsEM, 0, AtlEMShower::fgNIsEM);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetYTitle("Number of Entries");
  axis = h1->GetXaxis();
  for ( Int_t k = 0; k < AtlEMShower::fgNIsEM; k++ )
      axis->SetBinLabel(k+1, AtlEMShower::fgIsEMNames[k]);
  //*******************************  

  
  // ---------------------------------------------------------
  
  // -----------------------------------
  // Distinction by author and IsEM flag
  // -----------------------------------
  
    
  for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {

      // ---------------------
      // Distinction by author
      // ---------------------

      // Making sure author is not invalid
      if ( 1<<i_author & AtlEMShower::kInvalid ) continue;

      // retrieving author name
      TString author_name = AtlEMShower::fgAuthorNames[i_author];
      
      
      //-------------------------------
      // Creating author-sorted folders
      //-------------------------------
      
      gDirectory->mkdir(author_name.Data(),Form("%s Photon Ctrl Plots",author_name.Data()));
      gDirectory->cd(author_name.Data());

      for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
      
	// .........................
	// Distinction by IsEM-Flag
	// .........................

	// Making sure IsEM flag is not invalid
	if ( 1<<i_isem & AtlEMShower::kInvalid ) continue;

	// retrieving IsEM-Flag name
	TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
	  
	  
	// Shorthand notation "IsEMFlag_Author" for use in histogram names
	TString isemflag_author = Form("%s_%s",isemflag_name.Data(),author_name.Data());
	  
	// ................................
	// Creating IsEMFlag-sorted folders
	// ................................
	  
	gDirectory->mkdir(isemflag_name.Data(),Form("%s %s Photon Ctrl Plots",
						    isemflag_name.Data(),author_name.Data()));
	gDirectory->cd(isemflag_name.Data());
	  
	  
	// ................................
	// Truth vs Reconstructed Photons
	// ................................
	
	gDirectory->mkdir("MC_vs_REC_plots",
			  Form("Truth vs Reconstructed %s %s Photons",
			       isemflag_name.Data(),author_name.Data()));
	gDirectory->cd("MC_vs_REC_plots");
	  

	// Eta_MC - Eta_REC
	  
	// -> signed difference
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaEtaSigned",
			   isemflag_author.Data()),
		      Form("#eta difference between reconstructed %s %s photon and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		      100, -0.05, 0.05);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#eta_{REC} - #eta_{MC}");
	  
	// -> signed difference - normalized
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaEtaSigned_norm",
			   isemflag_author.Data()),
		      Form("#eta difference between reconstructed %s %s photon and MC best matching particle, normalized to MC-truth",
			   isemflag_name.Data(),author_name.Data()),
		      100, -0.2, 0.2);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#eta_{REC} - #eta_{MC} / #eta_{MC}");
	  
	// -> 2-dim confrontation
	h2 = new TH2F(Form("h_MC_vs_REC_%s_photons_Eta",isemflag_author.Data()),
		      Form("2-Dim Plot: #eta_{MC} vs #eta_{REC} of %s %s photons",
			   isemflag_name.Data(),author_name.Data()),
		      100, -5., 5., 
		      100, -5., 5. );
	fHistogramsNoEvtSel->Add(h2);
	h2->SetXTitle("#eta_{MC}");
	h2->SetYTitle("#eta_{REC}");
	  
	  

	// Phi_MC - Phi_REC
	  
	// -> signed difference
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaPhiSigned",isemflag_author.Data()),
		      Form("#phi difference between reconstructed %s %s photon and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		      100, -0.05, 0.05);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#phi_{REC} - #phi_{MC} (rad)");
	  
	// -> signed difference - normalized
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaPhiSigned_norm",
			   isemflag_author.Data()),
		      Form("#phi difference between reconstructed %s %s photon and MC best matching particle, normalized to MC-truth",
			   isemflag_name.Data(),author_name.Data()),
		      100, -0.05, 0.05);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#phi_{REC} - #phi_{MC} / #phi_{MC}");
	  
	// -> 2-dim confrontation
	h2 = new TH2F(Form("h_MC_vs_REC_%s_photons_Phi",isemflag_author.Data()),
		      Form("2-Dim Plot: #phi_{MC} vs #phi_{REC} of %s %s photons",
			   isemflag_name.Data(),author_name.Data()),
		      100, -3.2, 3.2, 
		      100, -3.2, 3.2 );
	fHistogramsNoEvtSel->Add(h2);
	h2->SetXTitle("#phi_{MC} (rad)");
	h2->SetYTitle("#phi_{REC}(rad)");

 
	// Pt_MC - Pt_REC
	  
	// -> signed difference
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaPtSigned",isemflag_author.Data()),
		      Form("p_{t} difference between reconstructed %s %s photon and MC best matching particle",isemflag_name.Data(),author_name.Data()),
		      100, -20, 20);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("p_{t,REC} - p_{t,MC} (GeV)");
	  
	// -> signed difference - normalized
	h1 = new TH1F(Form("h_MC_vs_REC_%s_photons_DeltaPtSigned_norm",
			   isemflag_author.Data()),
		      Form("p_{t} difference between reconstructed %s %s photon and MC best matching particle, normalized to MC-truth",
			   isemflag_name.Data(),author_name.Data()),
		      100, -0.5, 0.5);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("p_{t,REC} - p_{t,MC} / p_{t,MC}");
	  
	// -> 2-dim confrontation
	h2 = new TH2F(Form("h_MC_vs_REC_%s_photons_Pt",isemflag_author.Data()),
		      Form("2-Dim Plot: p_{t,MC} vs p_{t,REC} of %s %s photons",
			   isemflag_name.Data(),author_name.Data()),
		      100, 0, 300., 
		      100, 0, 300. );
	fHistogramsNoEvtSel->Add(h2);
	h2->SetXTitle("p_{t,MC} (GeV)");
	h2->SetYTitle("p_{t,REC} (GeV)");
	  
  
	// ===============
	//     PROFILES
	// ===============
	  
	// -> Delta PT vs PT-TRUTH 	  
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_PtMC_vs_DeltaPt",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} vs p_{t,MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, 0, 300., 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" p_{t,MC} (GeV)");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
	// ->  Delta PT vs ETA-TRUTH
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_EtaMC_vs_DeltaPt",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} vs #eta_{MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, -5., 5., 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" #eta_{MC}");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
  
	// ->  Delta PT vs PHI-TRUTH
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_PhiMC_vs_DeltaPt",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} vs #phi_{MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, -3.2, 3.2, 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" #phi_{MC} (rad)");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC} (GeV)");
	  
	// -> Delta PT (norm.) vs PT-TRUTH 
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_PtMC_vs_DeltaPt_norm",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} (norm.) vs p_{t,MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, 0, 300., 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" p_{t,MC} (GeV)");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
	// ->  Delta PT (norm.) vs ETA-TRUTH
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_EtaMC_vs_DeltaPt_norm",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} (norm.) vs #eta_{MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, -5., 5., 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" #eta_{MC}");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");

	// ->  Delta PT (norm.) vs PHI-TRUTH
	prof1 = new TProfile(Form("prof_MC_vs_REC_%s_photons_PhiMC_vs_DeltaPt_norm",
				  isemflag_author.Data()),
			     Form("#Delta p_{t} (norm.) vs #phi_{MC} of %s %s photons",isemflag_name.Data(),author_name.Data()),
			     100, -3.2, 3.2, 
			     -0.5, 0.5 );
	fHistogramsNoEvtSel->Add(prof1);
	prof1->SetXTitle(" #phi_{MC} (rad)");
	prof1->SetYTitle(" p_{t,REC}-p_{t,MC}/p_{t,MC}");
	  
	gDirectory->cd("..");
      
	  
	// =====================
	// Reconstructed Photons
	// =====================
	  
	gDirectory->mkdir("RecoPhotons", Form("%s %s Photon Ctrl Plots",
					      isemflag_name.Data(),author_name.Data()));
	gDirectory->cd("RecoPhotons");
	  
	  
    
	// Eta of photons:
	h1 = new TH1F(Form("h_%s_photon_eta",isemflag_author.Data()),
		      Form("%s %s Photon Pseudo-Rapidity",
			   isemflag_name.Data(),author_name.Data()),
		      100, -5., 5.);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#eta");
	h1->SetYTitle("Number of Entries");
	  
	  
	// Phi of photons:
	h1 = new TH1F(Form("h_%s_photon_phi",isemflag_author.Data()),
		      Form("%s %s Photon Azimuth",
			   isemflag_name.Data(),author_name.Data()),
		      128, -3.2, 3.2);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("#phi (rad)");
	h1->SetYTitle("Number of Entries");
	  
	  
	// Pt of photons:
	h1 = new TH1F(Form("h_%s_photon_pt",isemflag_author.Data()),
		      Form("%s %s Photon Transverse Momentum",
			   isemflag_name.Data(),author_name.Data()),
		      200, 0, 200);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("p_{t} (GeV)");
	h1->SetYTitle("Number of Entries");
	  
    
	// ---> Cone Isolation Energy Plots
	  
	// Et inside cones of 0.2, 0.3, 0.4, 0.45 (isolation) for
	// photons:
	h1 = new TH1F(Form("h_%s_photon_EtCone20",isemflag_author.Data()),
		      Form("E_{t} inside cone of dR=0.2 (%s %s photons)",
			   isemflag_name.Data(),author_name.Data()),
		      50, 0, 100);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("E_{t}-cone (GeV)");
	h1->SetYTitle("Number of Entries");
	  
	h1 = new TH1F(Form("h_%s_photon_EtCone30",isemflag_author.Data()),
		      Form("E_{t} inside cone of dR=0.3 (%s %s photons)",
			   isemflag_name.Data(),author_name.Data()),
		      50, 0, 100);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("E_{t}-cone (GeV)");
	h1->SetYTitle("Number of Entries");
	  
	h1 = new TH1F(Form("h_%s_photon_EtCone40",isemflag_author.Data()),
		      Form("E_{t} inside cone of dR=0.4 (%s %s photons)",
			   isemflag_name.Data(),author_name.Data()),
		      50, 0, 100);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("E_{t}-cone (GeV)");
	h1->SetYTitle("Number of Entries");
	  
	h1 = new TH1F(Form("h_%s_photon_EtCone45",isemflag_author.Data()),
		      Form("E_{t} inside cone of dR=0.45 (%s %s photons)",
			   isemflag_name.Data(),author_name.Data()),
		      50, 0, 100);
	fHistogramsNoEvtSel->Add(h1);
	h1->SetXTitle("E_{t}-cone (GeV)");
	h1->SetYTitle("Number of Entries");
	  
	 
	gDirectory->cd("../../");
	  
      }	  
      
      gDirectory->cd("..");
  }
  
  //--------------------------------------
  // Number of inclusive photons
  //--------------------------------------
  
  gDirectory->mkdir("N_photons","Number of inclusive photons (Ctrl Plots)");
  gDirectory->cd("N_photons");
  
  
  // No. of photons:
  h1 = new TH1F("h_all_photon_n","No. of all Photons",20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#gamma}");
  h1->SetYTitle("Number of Entries");
  
    
  gDirectory->cd("..");
  
  //--------------------------------------
  

  // Restore pwd
  savdir->cd();
  
}

//____________________________________________________________________

void AtlCtrlPlots::BookAtlFastHistograms() {
  //
  // Book ctrl plots for ATLFAST
  //
  TH1F *h1 = 0;
  TH2F *h2 = 0;
        
  // Save present working directory
  TDirectory *savdir = gDirectory;

  // Create sub-folder
  gDirectory->mkdir("AtlFast", "AtlFast Ctrl Plots");
  gDirectory->cd("AtlFast");

  // =========
  // Electrons
  // =========
  gDirectory->mkdir("Electrons", "Electron Ctrl Plots");
  gDirectory->cd("Electrons");
    
  //
  // Both charges
  // ------------
    
  // No. of electrons
  h1 = new TH1F("h_atlfast_electron_n",
		"No. of Electrons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_electron_eta",
		"Electron Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");
    
  // Phi
  h1 = new TH1F("h_atlfast_electron_phi",
		"Electron Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");
    
  // Et
  h1 = new TH1F("h_atlfast_electron_pt",
		"Electron Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  //
  // Positrons only
  // --------------
  gDirectory->mkdir("Eplus", "Positron Ctrl Plots");
  gDirectory->cd("Eplus");

  // No. of electrons
  h1 = new TH1F("h_atlfast_eplus_n",
		"No. of e^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e^{+}}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_eplus_eta",
		"e^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_eplus_phi",
		"e^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_eplus_pt",
		"e^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  //
  // Electrons (e-) only
  // -------------------
  gDirectory->cd("../");
  gDirectory->mkdir("Eminus", "Positron Ctrl Plots");
  gDirectory->cd("Eminus");

  // No. of electrons
  h1 = new TH1F("h_atlfast_eminus_n",
		"No. of e^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{e^{-}}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_eminus_eta",
		"e^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_eminus_phi",
		"e^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_eminus_pt",
		"e^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Restore pwd
  savdir->cd();

  // ===============
  // AtlFast Photons
  // ===============
    
  // Create sub-folder
  gDirectory->cd("AtlFast");
  gDirectory->mkdir("Photons", "Photon Ctrl Plots");
  gDirectory->cd("Photons");

  // No. of photons
  h1 = new TH1F("h_atlfast_photon_n",
		"No. of Photons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#gamma}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_photon_eta",
		"Photon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_photon_phi",
		"Photon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_photon_pt",
		"Photon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Restore pwd
  savdir->cd();

  // =============
  // AtlFast Muons
  // =============
  gDirectory->cd("AtlFast");
    
  // Create sub-folder
  gDirectory->mkdir("Muons", "Muon Ctrl Plots");
  gDirectory->cd("Muons");

  //
  // Both charges
  // ------------
    
  // No. of muons
  h1 = new TH1F("h_atlfast_muon_n",
		"No. of Muons",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_muon_eta",
		"Muon Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_muon_phi",
		"Muon Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_muon_pt",
		"Muon Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  //
  // Mu+ only
  // --------
  gDirectory->mkdir("Mu_plus", "Mu_plus Ctrl Plots");
  gDirectory->cd("Mu_plus");

  // No. of muons
  h1 = new TH1F("h_atlfast_muplus_n",
		"No. of #mu^{+}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu^{+}}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_muplus_eta",
		"#mu^{+} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_muplus_phi",
		"#mu^{+} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_muplus_pt",
		"#mu^{+} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  //
  // Mu- only
  // --------
  gDirectory->cd("../");
  gDirectory->mkdir("Mu_minus", "Mu_minus Ctrl Plots");
  gDirectory->cd("Mu_minus");

  // No. of muons
  h1 = new TH1F("h_atlfast_muminus_n",
		"No. of #mu^{-}",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{#mu^{-}}");
  h1->SetYTitle("Number of Entries");
    
  // Eta
  h1 = new TH1F("h_atlfast_muminus_eta",
		"#mu^{-} Pseudo-Rapidity",
		100, -2.5, 2.5);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#eta");
  h1->SetYTitle("Number of Entries");

  // Phi
  h1 = new TH1F("h_atlfast_muminus_phi",
		"#mu^{-} Azimuth",
		128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("#phi (rad)");
  h1->SetYTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_muminus_pt",
		"#mu^{-} Transverse Momentum",
		200, 0, 200);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("P_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Restore pwd
  savdir->cd();

  // ======================================
  // AtlFast Jets (SisCone4 @detectorlevel)
  // ======================================
    
  // Create sub-folder
  gDirectory->cd("AtlFast");
  gDirectory->mkdir("Jets", "Jet Ctrl Plots");
  gDirectory->cd("Jets");
    
  // No. of jets
  h1 = new TH1F("h_atlfast_njets",
		"No. of AtlFast Jets",
		20, 0, 20);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("N_{Jets}");
  h1->SetYTitle("Number of Entries");
    
  // Eta-phi plane
  h2 = new TH2F("h_atlfast_etaphi",
		"AtlFast Jets #eta-#phi plane",
		100, -5, 5, 128, -3.2, 3.2);
  fHistogramsNoEvtSel->Add(h2);
  h2->SetXTitle("#eta");
  h2->SetYTitle("#phi");
  h2->SetZTitle("Number of Entries");

  // Et
  h1 = new TH1F("h_atlfast_Et",
		"AtlFast Jets Transverse Energy",
		200, 0, 1000);
  fHistogramsNoEvtSel->Add(h1);
  h1->SetXTitle("E_{t} (GeV)");
  h1->SetYTitle("Number of Entries");

  // Restore pwd
  savdir->cd();
}

//____________________________________________________________________

void AtlCtrlPlots::FillJetHistograms(Bool_t EvtSelection) {
  //
  // Fill jet histograms
  //


  // AntiKt4-H1-Topo Jets ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TH1F *h_n_antikt4H1topo      = (TH1F*)GetHistogram("h_antikt4H1topo_njets",     EvtSelection);
  TH2F *h_etaphi_antikt4H1topo = (TH2F*)GetHistogram("h_antikt4H1topo_etaphi",    EvtSelection);
  TH1F *h_Et_antikt4H1topo     = (TH1F*)GetHistogram("h_antikt4H1topo_Et",        EvtSelection);
  TH1F *h_eta_antikt4H1topo    = (TH1F*)GetHistogram("h_antikt4H1topo_eta",       EvtSelection);
  TH1F *h_phi_antikt4H1topo    = (TH1F*)GetHistogram("h_antikt4H1topo_phi",       EvtSelection);
  
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned      = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned",      EvtSelection);
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned_norm", EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_Eta                 = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_Eta",                 EvtSelection);
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned      = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned",      EvtSelection);
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned_norm", EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_Phi                 = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_Phi",                 EvtSelection);
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaPtSigned       = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaPtSigned",       EvtSelection); 
  TH1F *h_MC_vs_REC_antikt4H1topo_DeltaPtSigned_norm  = (TH1F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_DeltaPtSigned_norm",  EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_Pt                  = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_Pt",                  EvtSelection);

  TH2F *h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt              = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt",              EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt       = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt",       EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt             = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt",             EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt",      EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt",      EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm         = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm" ,        EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm  = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm" , EvtSelection);
  TH2F *h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm        = (TH2F*)GetHistogram("h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm",        EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm", EvtSelection);
  TProfile *prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt_norm", EvtSelection);

  // Cone4-H1-Tower Jets ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TH1F *h_n_cone4H1tower      = (TH1F*)GetHistogram("h_cone4H1tower_njets",     EvtSelection);
  TH2F *h_etaphi_cone4H1tower = (TH2F*)GetHistogram("h_cone4H1tower_etaphi",    EvtSelection);
  TH1F *h_Et_cone4H1tower     = (TH1F*)GetHistogram("h_cone4H1tower_Et",        EvtSelection);
  TH1F *h_eta_cone4H1tower    = (TH1F*)GetHistogram("h_cone4H1tower_eta",       EvtSelection);
  TH1F *h_phi_cone4H1tower    = (TH1F*)GetHistogram("h_cone4H1tower_phi",       EvtSelection);
  
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaEtaSigned      = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaEtaSigned",      EvtSelection);
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaEtaSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaEtaSigned_norm", EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_Eta                 = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_Eta",                 EvtSelection);
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaPhiSigned      = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaPhiSigned",      EvtSelection);
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaPhiSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaPhiSigned_norm", EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_Phi                 = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_Phi",                 EvtSelection);
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaPtSigned       = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaPtSigned",       EvtSelection); 
  TH1F *h_MC_vs_REC_cone4H1tower_DeltaPtSigned_norm  = (TH1F*)GetHistogram("h_MC_vs_REC_cone4H1tower_DeltaPtSigned_norm",  EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_Pt                  = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_Pt",                  EvtSelection);

  TH2F *h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt              = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt",              EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt       = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt",       EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt             = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt",             EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt",      EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt",      EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm         = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm" ,        EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm  = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm" , EvtSelection);
  TH2F *h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm        = (TH2F*)GetHistogram("h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm",        EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm", EvtSelection);
  TProfile *prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt_norm", EvtSelection);

  // =======
  
 
  // No. of jets
  Int_t njets_antikt4H1topo = fEvent->GetN_AntiKt4H1TopoJets();
  h_n_antikt4H1topo->Fill(njets_antikt4H1topo);
  Int_t njets_cone4H1tower = fEvent->GetN_Cone4H1TowerJets();
  h_n_cone4H1tower->Fill(njets_cone4H1tower);
  
  HepJet *jet_mc = 0;
  Float_t eta_jet = 0.;
  Float_t phi_jet = 0.;
  Float_t et_jet = 0.;
  Float_t eta_mc = 0.;
  Float_t phi_mc = 0.;
  Float_t et_mc  = 0.;
  
  // Loop over all AntiKt4-H1-Topo Jets
  for ( Int_t i = 0; i < njets_antikt4H1topo; i++ ) {
      AtlJet *jet = (AtlJet*)fEvent->GetAntiKt4H1TopoJets()->At(i);
      eta_jet = jet->Eta();
      phi_jet = jet->Phi();
      et_jet = jet->Et();	
      // Eta-phi
      h_etaphi_antikt4H1topo->Fill(jet->Eta(), jet->Phi(), GetTagEvtWeight());
      
      // Et
      h_Et_antikt4H1topo->Fill(jet->Et(), GetTagEvtWeight());
      
      // Eta
      h_eta_antikt4H1topo->Fill(jet->Eta(), GetTagEvtWeight());
      
      // Phi
      h_phi_antikt4H1topo->Fill(jet->Phi(), GetTagEvtWeight());
      
      // ================================         
      // MC Comparison
      // ================================         
      if(fEvent->IsMC()) {
	  
	  // Find best matching HepMCParticle	
	  jet_mc = (HepJet*)fEvent->FindMatchedMCJet(jet,0.2,0.2);
	  if(jet_mc !=0) {
	      phi_mc = jet_mc->Phi();
	      et_mc  = jet_mc->Pt();
	      eta_mc = jet_mc->Eta();
	      
	      // Fill MC vs REC comparison histograms
	      //eta
	      h_MC_vs_REC_antikt4H1topo_Eta                               -> Fill(  eta_mc, eta_jet          , GetTagEvtWeight());
	      h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned                    -> Fill(  eta_jet - eta_mc         , GetTagEvtWeight());
	      if(eta_mc!=0) h_MC_vs_REC_antikt4H1topo_DeltaEtaSigned_norm -> Fill( (eta_jet - eta_mc)/eta_mc , GetTagEvtWeight());
	      
	      //phi
	      h_MC_vs_REC_antikt4H1topo_Phi                               -> Fill(  phi_mc, phi_jet          , GetTagEvtWeight());
	      h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned                    -> Fill(  phi_jet - phi_mc         , GetTagEvtWeight());	  
	      if(phi_mc!=0) h_MC_vs_REC_antikt4H1topo_DeltaPhiSigned_norm -> Fill( (phi_jet - phi_mc)/phi_mc , GetTagEvtWeight());
	      
	      //Pt
	      h_MC_vs_REC_antikt4H1topo_Pt                                -> Fill(  et_mc, et_jet            , GetTagEvtWeight());
	      h_MC_vs_REC_antikt4H1topo_DeltaPtSigned                     -> Fill(  et_jet - et_mc           , GetTagEvtWeight());	  
	      if(et_mc!=0) {
		  h_MC_vs_REC_antikt4H1topo_DeltaPtSigned_norm              -> Fill( (et_jet - et_mc)/et_mc    , GetTagEvtWeight());
		  // et&eta(MC) vs e_t resolution  
		  h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt         -> Fill( et_mc,  (et_jet - et_mc)        , GetTagEvtWeight());
		  h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt	 -> Fill( eta_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt	 -> Fill( et_mc,  (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt	 -> Fill( eta_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt	 -> Fill( phi_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  // et&eta(MC) vs e_t resolution -norm 
		  h_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm	  -> Fill( et_mc,  (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  h_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm    -> Fill( eta_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_PtMC_vs_DeltaPt_norm  -> Fill( et_mc,  (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_EtaMC_vs_DeltaPt_norm -> Fill( eta_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_antikt4H1topo_PhiMC_vs_DeltaPt_norm -> Fill( phi_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
	      }      
	  }
      }
  }

  
  // Loop over all Cone4-H1-Tower Jets
  for ( Int_t j = 0; j < njets_cone4H1tower; j++ ) {
      AtlJet *jet = (AtlJet*)fEvent->GetCone4H1TowerJets()->At(j);
      eta_jet = jet->Eta();
      phi_jet = jet->Phi();
      et_jet = jet->Et();	
      // Eta-phi
      h_etaphi_cone4H1tower->Fill(jet->Eta(), jet->Phi(), GetTagEvtWeight());
      
      // Et
      h_Et_cone4H1tower->Fill(jet->Et(), GetTagEvtWeight());
      
      // Eta
      h_eta_cone4H1tower->Fill(jet->Eta(), GetTagEvtWeight());
      
      // Phi
      h_phi_cone4H1tower->Fill(jet->Phi(), GetTagEvtWeight());

      
      // ================================         
      // MC Comparison
      // ================================         
      if(fEvent->IsMC()) {
	  
	  // Find best matching HepMCParticle	
	  jet_mc = (HepJet*)fEvent->FindMatchedMCJet(jet,0.2,0.2);
	  if(jet_mc !=0) {
	      phi_mc = jet_mc->Phi();
	      et_mc  = jet_mc->Pt();
	      eta_mc = jet_mc->Eta();
	      
	      // Fill MC vs REC comparison histograms
	      //eta
	      h_MC_vs_REC_cone4H1tower_Eta                               -> Fill(  eta_mc, eta_jet          , GetTagEvtWeight());
	      h_MC_vs_REC_cone4H1tower_DeltaEtaSigned                    -> Fill(  eta_jet - eta_mc         , GetTagEvtWeight());
	      if(eta_mc!=0) h_MC_vs_REC_cone4H1tower_DeltaEtaSigned_norm -> Fill( (eta_jet - eta_mc)/eta_mc , GetTagEvtWeight());
	      
	      //phi
	      h_MC_vs_REC_cone4H1tower_Phi                               -> Fill(  phi_mc, phi_jet          , GetTagEvtWeight());
	      h_MC_vs_REC_cone4H1tower_DeltaPhiSigned                    -> Fill(  phi_jet - phi_mc         , GetTagEvtWeight());	  
	      if(phi_mc!=0) h_MC_vs_REC_cone4H1tower_DeltaPhiSigned_norm -> Fill( (phi_jet - phi_mc)/phi_mc , GetTagEvtWeight());
	      
	      //Pt
	      h_MC_vs_REC_cone4H1tower_Pt                                -> Fill(  et_mc, et_jet            , GetTagEvtWeight());
	      h_MC_vs_REC_cone4H1tower_DeltaPtSigned                     -> Fill(  et_jet - et_mc           , GetTagEvtWeight());	  
	      if(et_mc!=0) {
		  h_MC_vs_REC_cone4H1tower_DeltaPtSigned_norm              -> Fill( (et_jet - et_mc)/et_mc    , GetTagEvtWeight());
		  // et&eta(MC) vs e_t resolution  
		  h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt         -> Fill( et_mc,  (et_jet - et_mc)        , GetTagEvtWeight());
		  h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt	 -> Fill( eta_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt	 -> Fill( et_mc,  (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt	 -> Fill( eta_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt	 -> Fill( phi_mc, (et_jet - et_mc)        , GetTagEvtWeight());
		  // et&eta(MC) vs e_t resolution -norm 
		  h_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm	  -> Fill( et_mc,  (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  h_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm    -> Fill( eta_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_PtMC_vs_DeltaPt_norm  -> Fill( et_mc,  (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_EtaMC_vs_DeltaPt_norm -> Fill( eta_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
		  prof_MC_vs_REC_cone4H1tower_PhiMC_vs_DeltaPt_norm -> Fill( phi_mc, (et_jet - et_mc)/et_mc  , GetTagEvtWeight());
	      }
	  }
      }
  }  
}


//_____________________________________________________________

void AtlCtrlPlots::FillEnergySumsHistograms(Bool_t EvtSelection) {
  //
  // Fill Energy sums histograms, including REC vs MCTruthNonInt comparisons
  //
  Float_t norm_DeltaPhi=0,norm_DeltaMag=0;
  // Get histogram pointers
  TH1F *h_Energy_Scalar_Sum        = (TH1F*)GetHistogram("h_Energy_Scalar_Sum",        EvtSelection);
  TH1F *h_Missing_Energy_Magnitude = (TH1F*)GetHistogram("h_Missing_Energy_Magnitude", EvtSelection);
  TH1F *h_Missing_Energy_PhiAngle  = (TH1F*)GetHistogram("h_Missing_Energy_PhiAngle",  EvtSelection);
  TH2F *h_Missing_Energy_xyPLANE   = (TH2F*)GetHistogram("h_Missing_Energy_xyPLANE",   EvtSelection);

  TH1F *h_Missing_Energy_PhiAngle_DELTA_MC_REC = 0;
  TH1F *h_Missing_Energy_PhiAngle_DELTA_MC_REC_norm = 0;
  TH2F *h_Missing_Energy_PhiAngle_MCvsREC = 0;
  TH1F *h_Missing_Energy_Magnitude_DELTA_MC_REC = 0;
  TH1F *h_Missing_Energy_Magnitude_DELTA_MC_REC_norm = 0;
  TH2F *h_Missing_Energy_Magnitude_MCvsREC = 0;
  TH1F *h_Energy_Scalar_Sum_DELTA_MC_REC = 0;
  TH2F *h_Energy_Scalar_Sum_MCvsREC = 0;
  TH2F *h_MCNonIntEta_RECPhiMET = 0;
  TH2F *h_MCNonIntEta_Phi = 0;

  if(fEvent->IsMC()) {
    h_Missing_Energy_PhiAngle_DELTA_MC_REC  = (TH1F*)GetHistogram("h_Missing_Energy_PhiAngle_DELTA_MC-REC", EvtSelection);
    h_Missing_Energy_PhiAngle_DELTA_MC_REC_norm  = (TH1F*)GetHistogram("h_Missing_Energy_PhiAngle_DELTA_MC-REC_norm", EvtSelection);
    h_Missing_Energy_PhiAngle_MCvsREC       = (TH2F*)GetHistogram("h_Missing_Energy_PhiAngle_MCvsREC", EvtSelection);
    h_Missing_Energy_Magnitude_DELTA_MC_REC = (TH1F*)GetHistogram("h_Missing_Energy_Magnitude_DELTA_MC-REC", EvtSelection);
    h_Missing_Energy_Magnitude_DELTA_MC_REC_norm = (TH1F*)GetHistogram("h_Missing_Energy_Magnitude_DELTA_MC-REC_norm", EvtSelection);
    h_Missing_Energy_Magnitude_MCvsREC      = (TH2F*)GetHistogram("h_Missing_Energy_Magnitude_MCvsREC", EvtSelection);
    h_Energy_Scalar_Sum_DELTA_MC_REC        = (TH1F*)GetHistogram("h_Energy_Scalar_Sum_DELTA_MC-REC", EvtSelection);
    h_Energy_Scalar_Sum_MCvsREC             = (TH2F*)GetHistogram("h_Energy_Scalar_Sum_MCvsREC", EvtSelection);
    h_MCNonIntEta_RECPhiMET = (TH2F*)GetHistogram("h_MCNonIntEta_RECPhiMET", EvtSelection);
    h_MCNonIntEta_Phi = (TH2F*)GetHistogram("h_MCNonIntEta_Phi", EvtSelection);
  }

  h_Energy_Scalar_Sum        ->Fill( fEvent->GetEnergySum()->GetSumEt(),            GetTagEvtWeight() );

  if(fEvent->IsMC()) {
    h_Energy_Scalar_Sum_DELTA_MC_REC -> Fill( (fEvent->GetEnergySum()->GetMCIntSumEt()) - ( fEvent->GetEnergySum()->GetSumEt() ), GetTagEvtWeight() );            
    h_Energy_Scalar_Sum_MCvsREC                  -> Fill( fEvent->GetEnergySum()->GetMCIntSumEt(), fEvent->GetEnergySum()->GetSumEt() ,  GetTagEvtWeight() );
  }

  // MET plots only to be filled if there is MET at all:
  if( fEvent->GetEnergySum()->GetMissingEt().Mod() != 0. ) {
      
    h_Missing_Energy_Magnitude ->Fill( fEvent->GetEnergySum()->GetMissingEt().Mod(), GetTagEvtWeight() );
    h_Missing_Energy_PhiAngle  ->Fill( fEvent->GetEnergySum()->MissingEt_Phi(), GetTagEvtWeight() );
    h_Missing_Energy_xyPLANE   ->Fill( fEvent->GetEnergySum()->GetMissingEt().Px(),
				       fEvent->GetEnergySum()->GetMissingEt().Py(),  GetTagEvtWeight() );
    if(fEvent->IsMC()) {
      h_Missing_Energy_PhiAngle_DELTA_MC_REC       ->Fill( fEvent->GetEnergySum()->GetMCNonIntMissingEt().DeltaPhi( fEvent->GetEnergySum()->GetMissingEt() ),
							   GetTagEvtWeight());
      h_Missing_Energy_PhiAngle_MCvsREC            ->Fill( fEvent->GetEnergySum()->MCNonIntMissingEt_Phi(),
							   fEvent->GetEnergySum()->MissingEt_Phi(),   
							   GetTagEvtWeight() );
  
      h_Missing_Energy_Magnitude_DELTA_MC_REC      ->Fill(fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod() - fEvent->GetEnergySum()->GetMissingEt().Mod(), 
							  GetTagEvtWeight()); 

      h_Missing_Energy_PhiAngle_DELTA_MC_REC ->Fill(fEvent->GetEnergySum()->GetMCNonIntMissingEt().DeltaPhi( fEvent->GetEnergySum()->GetMissingEt() )
						    ,GetTagEvtWeight());
      h_Missing_Energy_PhiAngle_MCvsREC      ->Fill( fEvent->GetEnergySum()->MCNonIntMissingEt_Phi(),
						     fEvent->GetEnergySum()->MissingEt_Phi(),   GetTagEvtWeight() );
 
      h_Missing_Energy_Magnitude_DELTA_MC_REC->Fill(fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod() - fEvent->GetEnergySum()->GetMissingEt().Mod(), 
						    GetTagEvtWeight()); 
      h_Missing_Energy_Magnitude_MCvsREC     ->Fill(fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod(),
						    fEvent->GetEnergySum()->GetMissingEt().Mod(),  GetTagEvtWeight() );
     
      h_MCNonIntEta_RECPhiMET -> Fill( fEvent->GetMCSumEnergy_NonInt_Eta(),
				       fEvent->GetEnergySum()->MissingEt_Phi(), GetTagEvtWeight() );
      h_MCNonIntEta_Phi -> Fill( fEvent->GetMCSumEnergy_NonInt_Eta(),
				 fEvent->GetMCSumEnergy_NonInt_Phi(), GetTagEvtWeight() );
    }
  }
  if(fEvent->IsMC()) {
    h_Missing_Energy_Magnitude_MCvsREC           ->Fill(fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod(),
							fEvent->GetEnergySum()->GetMissingEt().Mod(),  
							GetTagEvtWeight() );
  
    h_Energy_Scalar_Sum_DELTA_MC_REC             ->Fill( (fEvent->GetEnergySum()->GetMCIntSumEt()) // + fEvent->GetEnergySum()->GetMCMuonsSumEt()
							 - ( fEvent->GetEnergySum()->GetSumEt() ),
							 GetTagEvtWeight() );            
    h_Energy_Scalar_Sum_MCvsREC                  ->Fill( fEvent->GetEnergySum()->GetMCIntSumEt(),  //  + fEvent->GetEnergySum()->GetMCMuonsSumEt()
							 fEvent->GetEnergySum()->GetSumEt(),  
							 GetTagEvtWeight() );
    if( (fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod()!=0) && (fEvent->GetEnergySum()->GetMCNonIntMissingEt().Phi()!=0) ) {
      norm_DeltaPhi = (fEvent->GetEnergySum()->GetMCNonIntMissingEt().DeltaPhi( fEvent->GetEnergySum()->GetMissingEt()) )  / (fEvent->GetEnergySum()->GetMCNonIntMissingEt().Phi());
      norm_DeltaMag = (fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod() - fEvent->GetEnergySum()->GetMissingEt().Mod())/ (fEvent->GetEnergySum()->GetMCNonIntMissingEt().Mod()); 
      h_Missing_Energy_Magnitude_DELTA_MC_REC_norm ->Fill( norm_DeltaMag, GetTagEvtWeight()); 
      h_Missing_Energy_PhiAngle_DELTA_MC_REC_norm  ->Fill( norm_DeltaPhi , GetTagEvtWeight());
    }
  }
}


//____________________________________________________________________

void AtlCtrlPlots::BookTriggerHistograms(Bool_t EvtSelection) {
    //
    // Book trigger histograms
    //
    TDirectory *savdir = gDirectory;
    if ( EvtSelection ) {
	fDir_evtsel->cd();
	gDirectory->mkdir("Trigger", "Trigger Ctrl Plots");
	gDirectory->cd("Trigger");
    } else {
	fDir_noevtsel->cd();
	gDirectory->mkdir("Trigger", "Trigger Ctrl Plots");
	gDirectory->cd("Trigger");
    }
    
    // Get trigger configuration
    AtlTriggerConf *trig_conf = AtlTrigger::GetTriggerConf();
    Int_t nitems_l1  = trig_conf->GetN_TriggerItems(AtlTriggerConf::kL1);
    Int_t nitems_hlt = trig_conf->GetN_TriggerItems(AtlTriggerConf::kHLT);
    
    // =======
    // Level 1
    // =======

    // Allow for ~10% more bins since the number of items can vary
    // during the job
    Int_t nbins_l1 = Int_t(nitems_l1*1.1);
    TH1F *h_l1 = new TH1F("h_trigger_l1",
			  "No. of Passed L1 Triggers",
			  nbins_l1, 0, nbins_l1);
    if ( EvtSelection ) {
	fHistogramsEvtSel->Add(h_l1);
    } else {
        fHistogramsNoEvtSel->Add(h_l1);
    }
    h_l1->SetYTitle("Number of Entries");
    TAxis *axis = h_l1->GetXaxis();
    for ( Int_t i = 0; i < nitems_l1; i++ ) {
	axis->SetBinLabel(i+1, trig_conf->GetL1ItemName(i, fEvent->RunNr()));
    }
    h_l1->SetStats(kFALSE);

    // ============
    // Higher level
    // ============

    // Allow for ~10% more bins since the number of items can vary
    // during the job
    Int_t nbins_hlt = Int_t(nitems_hlt*1.1);
    TH1F *h_hlt = new TH1F("h_trigger_hlt",
			   "No. of Passed Higher Level Triggers",
			   nbins_hlt, 0, nbins_hlt);
    if ( EvtSelection ) {
	fHistogramsEvtSel->Add(h_hlt);
    } else {
	fHistogramsNoEvtSel->Add(h_hlt);
    }
    h_hlt->SetYTitle("Number of Entries");
    axis = h_hlt->GetXaxis();
    for ( Int_t i = 0; i < nitems_hlt; i++ ) {
	axis->SetBinLabel(i+1, trig_conf->GetHLTItemName(i, fEvent->RunNr()));
    }
    h_hlt->SetStats(kFALSE);

    savdir->cd();
}

//____________________________________________________________________

void AtlCtrlPlots::FillTriggerHistograms(Bool_t EvtSelection) {
    //
    // Fill trigger histograms
    //
    // Note: event weights are not accounted for
    //
    
    // Get histograms. Book them if necessary
    TH1F *h_l1 = (TH1F*)GetHistogram("h_trigger_l1", EvtSelection);
    if ( h_l1 == 0 ) {
	BookTriggerHistograms(EvtSelection);
	h_l1 = (TH1F*)GetHistogram("h_trigger_l1", EvtSelection);
    }
    TH1F *h_hlt = (TH1F*)GetHistogram("h_trigger_hlt", EvtSelection);
    
    // Get trigger configuration
    AtlTriggerConf *trig_conf = AtlTrigger::GetTriggerConf();
    Int_t nitems_l1  = trig_conf->GetN_TriggerItems(AtlTriggerConf::kL1);
    Int_t nitems_hlt = trig_conf->GetN_TriggerItems(AtlTriggerConf::kHLT);

    // Level 1
    for ( Int_t i = 0; i < nitems_l1; i++ ) {
	if ( ((AtlTriggerItem*)fEvent->GetTrigger()->GetL1Items()
	      ->At(i))->HasPassed() ) {
	    h_l1->AddBinContent(i+1);
	}
    }

    // Higher level
    for ( Int_t i = 0; i < nitems_hlt; i++ ) {
	if ( ((AtlTriggerItem*)fEvent->GetTrigger()->GetHLTItems()
	      ->At(i))->HasPassed() ) {
	    h_hlt->AddBinContent(i+1);
	}
    }
}
    

//____________________________________________________________________

void AtlCtrlPlots::FillElectronHistograms(Bool_t EvtSelection) {
  //
  // Fill electron histograms
  //

    
  //=======================
  // Get histogram pointers
  //======================= 
    
  //******************************
  // Number of electrons by author
  //******************************
  
  TH1F *h_el_author = (TH1F*)GetHistogram("h_electron_author", EvtSelection);

  //******************************

  //*********************************
  // Number of electrons by IsEM-Flag
  //*********************************
  
  TH1F *h_el_isem = (TH1F*)GetHistogram("h_electron_isem", EvtSelection);

  //*********************************

  
  //-----------------------------------------------------
  // all other histograms, sorted by author and IsEM flag
  //-----------------------------------------------------

  // !!!!!
  // NOTE: [0][0], [0][x], [x][0] exist in arrays below, but don't get filled, since 0 <-> invalid!!!
  // BETTER IDEA to synchronise arrays with loop-requirements?
  //  -> can you tell an array in C++ to begin with 1?
  //  -> if not, can you synchronise in a way that does not assume invalid corresponding to index 0 ?
  // !!!!!
  
  //if(fEvent->IsMC()) {
    // MC vs REC

    TH1F *h_MC_vs_REC_electrons_DeltaEtaSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_MC_vs_REC_electrons_DeltaEtaSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH2F *h_MC_vs_REC_electrons_Eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_MC_vs_REC_electrons_DeltaPhiSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_MC_vs_REC_electrons_DeltaPhiSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH2F *h_MC_vs_REC_electrons_Phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_MC_vs_REC_electrons_DeltaPtSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_MC_vs_REC_electrons_DeltaPtSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH2F *h_MC_vs_REC_electrons_Pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];

    // Profile Plots
  
    TProfile *prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TProfile *prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TProfile *prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TProfile *prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TProfile *prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TProfile *prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    //}

  // Both charges
  
  TH1F *h_electrons_all_n;
  TH1F *h_electrons_all_eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_all_phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_all_pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_etcone20[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_etcone30[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_etcone40[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_etcone45[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM]; 
  TH1F *h_electrons_plus_n;
  TH1F *h_electrons_plus_eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_plus_phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_plus_pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_minus_n;
  TH1F *h_electrons_minus_eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_minus_phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
  TH1F *h_electrons_minus_pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];

  h_electrons_all_n   = (TH1F*)GetHistogram("h_all_electron_n",EvtSelection);
  h_electrons_plus_n  = (TH1F*)GetHistogram("h_all_eplus_n", EvtSelection);
  h_electrons_minus_n = (TH1F*)GetHistogram("h_all_eminus_n", EvtSelection);
  
      
  for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {
      // ---------------------
      // Distinction by author
      // ---------------------

      // Making sure author is not invalid
      if ( 1<<i_author & AtlEMShower::kInvalid ) continue;

      // Retrieving author name
      TString author_name = AtlEMShower::fgAuthorNames[i_author];
      
      
      for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
      
	  // .........................
	  // Distinction by IsEM-Flag
	  // .........................

	  // Making sure IsEM flag is not invalid
	  if ( 1<<i_isem & AtlEMShower::kInvalid ) continue;
	  
	  // Retrieving IsEM-Flag name
	  TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
	  

	  // Shorthand notation "IsEMFlag_Author" for use in histogram names
	  TString isemflag_author = Form("%s_%s",isemflag_name.Data(),author_name.Data());
 
	  // 
	  // MC comparison histograms
	  // 
	  if(fEvent->IsMC()) {
	    // Eta

	    h_MC_vs_REC_electrons_DeltaEtaSigned[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaEtaSigned",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_DeltaEtaSigned_norm[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaEtaSigned_norm",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_Eta[i_author][i_isem]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_Eta",
					 isemflag_author.Data()),
				    EvtSelection);
	    // Phi
	  
	    h_MC_vs_REC_electrons_DeltaPhiSigned[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaPhiSigned",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_DeltaPhiSigned_norm[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaPhiSigned_norm",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_Phi[i_author][i_isem]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_Phi",
					 isemflag_author.Data()),
				    EvtSelection);

	    // Pt
				    
	    h_MC_vs_REC_electrons_DeltaPtSigned[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaPtSigned",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_DeltaPtSigned_norm[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_DeltaPtSigned_norm",
					 isemflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_electrons_Pt[i_author][i_isem]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_electrons_Pt",
					 isemflag_author.Data()),
				    EvtSelection);
	    // Profile Plots
	  
	    prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt[i_author][i_isem]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_electrons_PtMC_vs_DeltaPt",
					     isemflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt[i_author][i_isem]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_electrons_EtaMC_vs_DeltaPt",
					     isemflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt[i_author][i_isem]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_electrons_PhiMC_vs_DeltaPt",
					     isemflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt_norm[i_author][i_isem]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_electrons_PtMC_vs_DeltaPt_norm",
					     isemflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt_norm[i_author][i_isem]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_electrons_EtaMC_vs_DeltaPt_norm",
					     isemflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt_norm[i_author][i_isem]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_electrons_PhiMC_vs_DeltaPt_norm",
					     isemflag_author.Data()),
					EvtSelection);
	  }
    
	  // ============
	  // Both charges
	  // ============
	  
	  h_electrons_all_eta[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_eta",
					 isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_all_phi[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_phi",
					 isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_all_pt[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_pt",
					 isemflag_author.Data()),
				    EvtSelection);

	  h_electrons_etcone20[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_EtCone20",
					 isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_etcone30[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_EtCone30",
					 isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_etcone40[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_EtCone40",
					 isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_etcone45[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_electron_EtCone45",
					 isemflag_author.Data()),
				    EvtSelection);

  
	  // ===
	  // e+
	  // ===
	  
	  h_electrons_plus_eta[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eplus_eta",isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_plus_phi[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eplus_phi",isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_plus_pt[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eplus_pt",isemflag_author.Data()),
				    EvtSelection);
	  
	  
	  // ===
	  // e-
	  // ===
  
	  h_electrons_minus_eta[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eminus_eta",isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_minus_phi[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eminus_phi",isemflag_author.Data()),
				    EvtSelection);
	  h_electrons_minus_pt[i_author][i_isem]
	      = (TH1F*)GetHistogram(Form("h_%s_eminus_pt",isemflag_author.Data()),
				    EvtSelection);
	  
	  
      }  
  }


  // ================================
  // Fill histograms for eta, phi, pt
  // ================================
  
  Int_t n_electrons       = fEvent->GetN_Electrons();
  Int_t n_electrons_plus  = 0;
  Int_t n_electrons_minus = 0;


  AtlElectron *el = 0;
	  
  HepMCParticle *el_mc = 0;
  
  Float_t eta_mc = 0.;
  Float_t phi_mc = 0.;
  Float_t pt_mc  = 0.;    
  
  Float_t eta = 0.;
  Float_t phi = 0.;
  Float_t pt  = 0.;
  
  Bool_t eplus;
  
  for ( Int_t i = 0; i < n_electrons; i++ ) {
      el = (AtlElectron*)fEvent->GetElectrons()->At(i);
      
      // variables
      pt  = el->Pt();
      eta = el->Eta();
      phi = el->Phi();
	      
	      
      // charge
      eplus = el->IsPositive();

      if ( eplus ) {
	  n_electrons_plus++;
      } else {
	  n_electrons_minus++;
      }


      //******************************
      // Number of electrons by author
      //******************************
      AtlEMShower::EAuthor author = el->GetAuthor();
      for ( Int_t j = 0; j < AtlEMShower::fgNAuthors; j++ ) {
	  if ( author & (1<<j) ) h_el_author->
				     AddBinContent(j+1, GetTagEvtWeight());
      }
      //******************************

      //*********************************
      // Number of electrons by IsEM-Flag
      //*********************************
      AtlEMShower::EIsEM isem = el->GetIsEM();
      for ( Int_t j = 0; j < AtlEMShower::fgNIsEM; j++ ) {
	  if ( isem & (1<<j) ) h_el_isem->
				     AddBinContent(j+1, GetTagEvtWeight());
      }
      //*********************************


      for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {
	  // ---------------------
	  // Distinction by author
	  // ---------------------
	  
	  // Making sure author is not invalid
	  if ( 1<<i_author & AtlEMShower::kInvalid ) continue;

	  // Retrieving author name
	  TString author_name = AtlEMShower::fgAuthorNames[i_author];
	  
	  
	  for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
	      
	      // .........................
	      // Distinction by IsEM-Flag
	      // .........................
	      
	      
	      // Making sure IsEM-Flag is not invalid
	      if ( 1<<i_isem & AtlEMShower::kInvalid ) continue;

	      // Retrieving IsEM-Flag name
	      TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
	      
	      	     
	      // Be sure, author and IsEM flag match current electron 
	      if ( (author & 1<<i_author) && (isem & 1<<i_isem) ) {
		  
	      	  
		// ========================
		// MC comparison histograms
		// ========================
		  
		if(fEvent->IsMC()) {
		  // Find best matching HepMCParticle	
		  el_mc = (HepMCParticle*)fEvent->FindMatchedMCParticle(el);
		  if(el_mc != 0) {
		    phi_mc = el_mc->Phi();
		    pt_mc  = el_mc->Pt();
		    eta_mc = el_mc->Eta();
		      
		    // Fill MC vs REC comparison histograms
		      
		    // Eta
		    h_MC_vs_REC_electrons_Eta[i_author][i_isem]
		      -> Fill(eta_mc, eta           , GetTagEvtWeight());
		    h_MC_vs_REC_electrons_DeltaEtaSigned[i_author][i_isem]
		      -> Fill(eta - eta_mc          , GetTagEvtWeight());
		    if(eta_mc!=0) h_MC_vs_REC_electrons_DeltaEtaSigned_norm[i_author][i_isem]
				    -> Fill( (eta - eta_mc)/eta_mc, GetTagEvtWeight());
		      
		    // Phi
		    h_MC_vs_REC_electrons_Phi[i_author][i_isem]
		      -> Fill(phi_mc, phi           , GetTagEvtWeight());
		    h_MC_vs_REC_electrons_DeltaPhiSigned[i_author][i_isem]
		      -> Fill(phi - phi_mc          , GetTagEvtWeight());	  
		    if(phi_mc!=0) h_MC_vs_REC_electrons_DeltaPhiSigned_norm[i_author][i_isem]
				    -> Fill( (phi - phi_mc)/phi_mc, GetTagEvtWeight());
		      
		    // Pt
		    h_MC_vs_REC_electrons_Pt[i_author][i_isem]
		      -> Fill(pt_mc, pt , GetTagEvtWeight());
		    h_MC_vs_REC_electrons_DeltaPtSigned[i_author][i_isem]
		      -> Fill(pt - pt_mc, GetTagEvtWeight());
		      
		    if(pt_mc!=0){ 
		      h_MC_vs_REC_electrons_DeltaPtSigned_norm[i_author][i_isem]
			-> Fill( (pt - pt_mc)/pt_mc          , GetTagEvtWeight());
		      // Fill Profile plots
		      prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt_norm[i_author][i_isem]
			-> Fill( pt_mc,  (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt_norm[i_author][i_isem]
			-> Fill( eta_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt_norm[i_author][i_isem]
			-> Fill( phi_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_electrons_PtMC_vs_DeltaPt[i_author][i_isem]
			-> Fill( pt_mc,  (pt - pt_mc)        , GetTagEvtWeight());
		      prof_MC_vs_REC_electrons_EtaMC_vs_DeltaPt[i_author][i_isem]
			-> Fill( eta_mc, (pt - pt_mc)        , GetTagEvtWeight());
		      prof_MC_vs_REC_electrons_PhiMC_vs_DeltaPt[i_author][i_isem]
			-> Fill( phi_mc, (pt - pt_mc)        , GetTagEvtWeight());
		    }
		  }
		}
		  
		// ============
		// Both charges
		// ============
		  
		  
		h_electrons_all_eta[i_author][i_isem]->Fill(eta, GetTagEvtWeight());
		h_electrons_all_phi[i_author][i_isem]->Fill(phi, GetTagEvtWeight());
		h_electrons_all_pt[i_author][i_isem]->Fill(pt,  GetTagEvtWeight());
		  
		h_electrons_etcone20[i_author][i_isem]->Fill(el->GetEtCone20(), GetTagEvtWeight());
		h_electrons_etcone30[i_author][i_isem]->Fill(el->GetEtCone30(), GetTagEvtWeight());
		h_electrons_etcone40[i_author][i_isem]->Fill(el->GetEtCone40(), GetTagEvtWeight());
		h_electrons_etcone45[i_author][i_isem]->Fill(el->GetEtCone45(), GetTagEvtWeight());
		  
		if ( eplus ) {
		  //===============
		  // Positrons only
		  //===============
		      
		      
		  h_electrons_plus_eta[i_author][i_isem]->Fill(eta, GetTagEvtWeight());
		  h_electrons_plus_phi[i_author][i_isem]->Fill(phi, GetTagEvtWeight());
		  h_electrons_plus_pt[i_author][i_isem]->Fill(pt,  GetTagEvtWeight());
		      
		} else {
		  //===============
		  // Electrons only
		  //===============
		      
		      
		  h_electrons_minus_eta[i_author][i_isem]->Fill(eta, GetTagEvtWeight());
		  h_electrons_minus_phi[i_author][i_isem]->Fill(phi, GetTagEvtWeight());
		  h_electrons_minus_pt[i_author][i_isem]->Fill(pt,  GetTagEvtWeight());
		      
		}
	      }	      
	  }	  
      }
  }
  

  // ------------------------------------------------------------------------
  
  // Fill in counts of electrons in event 
  
  // ------------------------------------------------------------------------
  

    
  h_electrons_all_n  ->Fill(n_electrons      , GetTagEvtWeight());
  h_electrons_plus_n ->Fill(n_electrons_plus , GetTagEvtWeight());
  h_electrons_minus_n->Fill(n_electrons_minus, GetTagEvtWeight());
	
  // ------------------------------------------------------------------------
  //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	      
    
}


  //____________________________________________________________________

void AtlCtrlPlots::FillMuonHistograms(Bool_t EvtSelection) {
  //
  // Fill muon histograms
  //

    
  //=======================
  // Get histogram pointers
  //======================= 
    
  //******************************
  // Number of muons by author
  //******************************
  
  TH1F *h_mu_author = (TH1F*)GetHistogram("h_muon_author", EvtSelection);

  //******************************

  //*********************************
  // Number of muons by Quality-Flag
  //*********************************
  
  TH1F *h_mu_quality = (TH1F*)GetHistogram("h_muon_quality", EvtSelection);

  //*********************************

  
  //--------------------------------------------------------
  // all other histograms, sorted by author and Quality flag
  //--------------------------------------------------------

  // !!!!!
  // NOTE: [0][0], [0][x], [x][0] exist in arrays below, but don't get filled, since 0 <-> invalid!!!
  // BETTER IDEA to synchronise arrays with loop-requirements?
  //  -> can you tell an array in C++ to begin with 1?
  //  -> if not, can you synchronise in a way that does not assume invalid corresponding to index 0 ?
  // !!!!!
  
  //if(fEvent->IsMC()) {
    // MC vs REC

    TH1F *h_MC_vs_REC_muons_DeltaEtaSigned[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH1F *h_MC_vs_REC_muons_DeltaEtaSigned_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH2F *h_MC_vs_REC_muons_Eta[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH1F *h_MC_vs_REC_muons_DeltaPhiSigned[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH1F *h_MC_vs_REC_muons_DeltaPhiSigned_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH2F *h_MC_vs_REC_muons_Phi[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH1F *h_MC_vs_REC_muons_DeltaPtSigned[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH1F *h_MC_vs_REC_muons_DeltaPtSigned_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TH2F *h_MC_vs_REC_muons_Pt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];

    // Profile Plots
  
    TProfile *prof_MC_vs_REC_muons_PtMC_vs_DeltaPt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TProfile *prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TProfile *prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TProfile *prof_MC_vs_REC_muons_PtMC_vs_DeltaPt_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TProfile *prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    TProfile *prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt_norm[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
    //}

  // Both charges
  
  TH1F *h_muons_all_n;
  TH1F *h_muons_all_eta[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_all_phi[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_all_pt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_etcone10[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_etcone20[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_etcone30[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_etcone40[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_ptcone10[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_ptcone20[AtlMuon::fgNAuthors][AtlMuon::fgNQuality]; 
  TH1F *h_muons_ptcone30[AtlMuon::fgNAuthors][AtlMuon::fgNQuality]; 
  TH1F *h_muons_ptcone40[AtlMuon::fgNAuthors][AtlMuon::fgNQuality]; 
  TH1F *h_muons_ntrkcone10[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_ntrkcone20[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_ntrkcone30[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_ntrkcone40[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_chi2[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_plus_n;
  TH1F *h_muons_plus_eta[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_plus_phi[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_plus_pt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_minus_n;
  TH1F *h_muons_minus_eta[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_minus_phi[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];
  TH1F *h_muons_minus_pt[AtlMuon::fgNAuthors][AtlMuon::fgNQuality];

  h_muons_all_n   = (TH1F*)GetHistogram("h_all_muon_n",EvtSelection);
  h_muons_plus_n  = (TH1F*)GetHistogram("h_all_muplus_n", EvtSelection);
  h_muons_minus_n = (TH1F*)GetHistogram("h_all_muminus_n", EvtSelection);
  
      
  for ( Int_t i_author = 0; i_author < AtlMuon::fgNAuthors; i_author++ ) {
      // ---------------------
      // Distinction by author
      // ---------------------

      // Making sure author is not invalid
      if ( 1<<i_author & AtlMuon::kInvalid ) continue;

      // Retrieving author name
      TString author_name = AtlMuon::fgAuthorNames[i_author];
      
      
      for ( Int_t i_quality = 0; i_quality < AtlMuon::fgNQuality; i_quality++ ) {
      
	  // ...........................
	  // Distinction by Quality-Flag
	  // ...........................

	  // Making sure Quality flag is not invalid
	  if ( 1<<i_quality & AtlMuon::kInvalid ) continue;
	  
	  // Retrieving Quality-Flag name
	  TString qualityflag_name = AtlMuon::fgQualityNames[i_quality];
	  

	  // Shorthand notation "QualityFlag_Author" for use in histogram names
	  TString qualityflag_author = Form("%s_%s",qualityflag_name.Data(),author_name.Data());
 
	  // 
	  // MC comparison histograms
	  // 
	  if(fEvent->IsMC()) {
	    // Eta

	    h_MC_vs_REC_muons_DeltaEtaSigned[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaEtaSigned",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_DeltaEtaSigned_norm[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaEtaSigned_norm",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_Eta[i_author][i_quality]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_Eta",
					 qualityflag_author.Data()),
				    EvtSelection);
	    // Phi
	  
	    h_MC_vs_REC_muons_DeltaPhiSigned[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaPhiSigned",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_DeltaPhiSigned_norm[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaPhiSigned_norm",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_Phi[i_author][i_quality]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_Phi",
					 qualityflag_author.Data()),
				    EvtSelection);

	    // Pt
				    
	    h_MC_vs_REC_muons_DeltaPtSigned[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaPtSigned",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_DeltaPtSigned_norm[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_DeltaPtSigned_norm",
					 qualityflag_author.Data()),
				    EvtSelection); 
	    h_MC_vs_REC_muons_Pt[i_author][i_quality]
	      = (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_muons_Pt",
					 qualityflag_author.Data()),
				    EvtSelection);
	    // Profile Plots
	  
	    prof_MC_vs_REC_muons_PtMC_vs_DeltaPt[i_author][i_quality]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_muons_PtMC_vs_DeltaPt",
					     qualityflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt[i_author][i_quality]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_muons_EtaMC_vs_DeltaPt",
					     qualityflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt[i_author][i_quality]
	      = (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_muons_PhiMC_vs_DeltaPt",
					     qualityflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_muons_PtMC_vs_DeltaPt_norm[i_author][i_quality]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_muons_PtMC_vs_DeltaPt_norm",
					     qualityflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt_norm[i_author][i_quality]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_muons_EtaMC_vs_DeltaPt_norm",
					     qualityflag_author.Data()),
					EvtSelection);
	    prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt_norm[i_author][i_quality]
	      = (TProfile*)GetHistogram(
					Form("prof_MC_vs_REC_%s_muons_PhiMC_vs_DeltaPt_norm",
					     qualityflag_author.Data()),
					EvtSelection);
	  }
    
	  // ============
	  // Both charges
	  // ============
	  
	  h_muons_all_eta[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_eta",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_all_phi[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_phi",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_all_pt[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_pt",
					 qualityflag_author.Data()),
				    EvtSelection);

	  h_muons_etcone10[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_EtCone10",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_etcone20[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_EtCone20",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_etcone30[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_EtCone30",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_etcone40[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_EtCone40",
					 qualityflag_author.Data()),
				    EvtSelection);
	  
	  h_muons_ptcone10[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_PtTrkCone10",
					qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ptcone20[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_PtTrkCone20",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ptcone30[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_PtTrkCone30",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ptcone40[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_PtTrkCone40",
					 qualityflag_author.Data()),
				    EvtSelection);

	  h_muons_ntrkcone10[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_NtrkCone10",
					qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ntrkcone20[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_NtrkCone20",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ntrkcone30[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_NtrkCone30",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_ntrkcone40[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_NtrkCone40",
					 qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_chi2[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muon_chi2",
					 qualityflag_author.Data()),
				    EvtSelection);
	  
	  // ===
	  // mu+
	  // ===
	  
	  h_muons_plus_eta[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muplus_eta",qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_plus_phi[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muplus_phi",qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_plus_pt[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muplus_pt",qualityflag_author.Data()),
				    EvtSelection);
	  
	  
	  // ===
	  // mu-
	  // ===
  
	  h_muons_minus_eta[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muminus_eta",qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_minus_phi[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muminus_phi",qualityflag_author.Data()),
				    EvtSelection);
	  h_muons_minus_pt[i_author][i_quality]
	      = (TH1F*)GetHistogram(Form("h_%s_muminus_pt",qualityflag_author.Data()),
				    EvtSelection);
	  
	  
      }  
  }


  // ================================
  // Fill histograms for eta, phi, pt
  // ================================
  
  Int_t n_muons       = fEvent->GetN_Muons();
  Int_t n_muons_plus  = 0;
  Int_t n_muons_minus = 0;


  AtlMuon *mu = 0;
	  
  HepMCParticle *mu_mc = 0;

  TString mu_author;
  TString mu_quality;
  
  Float_t eta_mc = 0.;
  Float_t phi_mc = 0.;
  Float_t pt_mc  = 0.;    
  
  Float_t eta = 0.;
  Float_t phi = 0.;
  Float_t pt  = 0.;
  
  Bool_t muplus;
  
  for ( Int_t i = 0; i < n_muons; i++ ) {
      mu = (AtlMuon*)fEvent->GetMuons()->At(i);
      
      // variables
      pt  = mu->Pt();
      eta = mu->Eta();
      phi = mu->Phi();
	      
//       // Author of current muon
//       mu_author = mu->NameOfAuthor();
    
//       // Quality flag of current muon
//       mu_quality = mu->NameOfQuality();

	      
      // charge
      muplus = mu->IsPositive();

      if ( muplus ) {
	  n_muons_plus++;
      } else {
	  n_muons_minus++;
      }


      //******************************
      // Number of muons by author
      //******************************
      AtlMuon::EAuthor author = mu->GetAuthor();
      for ( Int_t j = 0; j < AtlMuon::fgNAuthors; j++ ) {
	  if ( author & (1<<j) ) h_mu_author->
				     AddBinContent(j+1, GetTagEvtWeight());
      }
      //******************************

      //*********************************
      // Number of muons by Quality-Flag
      //*********************************
      AtlMuon::EQuality quality = mu->GetQuality();
      for ( Int_t k = 0; k < AtlMuon::fgNQuality; k++ ) {
	  if ( quality & (1<<k) ) h_mu_quality->
				     AddBinContent(k+1, GetTagEvtWeight());
      }
      //*********************************


      for ( Int_t i_author = 0; i_author < AtlMuon::fgNAuthors; i_author++ ) {
	  // ---------------------
	  // Distinction by author
	  // ---------------------
	  
	  // Making sure author is not invalid
	  if ( 1<<i_author & AtlMuon::kInvalid ) continue;

	  // Retrieving author name
	  TString author_name = AtlMuon::fgAuthorNames[i_author];
	  
	  
	  for ( Int_t i_quality = 0; i_quality < AtlMuon::fgNQuality; i_quality++ ) {
	      
	      // .........................
	      // Distinction by Quality-Flag
	      // .........................
	      
	      
	      // Making sure Quality-Flag is not invalid
	      if ( 1<<i_quality & AtlMuon::kInvalid ) continue;

	      // Retrieving Quality-Flag name
	      TString qualityflag_name = AtlMuon::fgQualityNames[i_quality];
	      
	      	     
	      // Be sure, author and Quality flag match current muon 
	      if ( (author & 1<<i_author) && (quality & 1<<i_quality) ) {
		  
	      	  
		// ========================
		// MC comparison histograms
		// ========================
		  
		if(fEvent->IsMC()) {
		  // Find best matching HepMCParticle	
		  mu_mc = (HepMCParticle*)fEvent->FindMatchedMCParticle(mu);
		  if(mu_mc != 0) {
		    phi_mc = mu_mc->Phi();
		    pt_mc  = mu_mc->Pt();
		    eta_mc = mu_mc->Eta();
		      
		    // Fill MC vs REC comparison histograms
		      
		    // Eta
		    h_MC_vs_REC_muons_Eta[i_author][i_quality]
		      -> Fill(eta_mc, eta           , GetTagEvtWeight());
		    h_MC_vs_REC_muons_DeltaEtaSigned[i_author][i_quality]
		      -> Fill(eta - eta_mc          , GetTagEvtWeight());
		    if(eta_mc!=0) h_MC_vs_REC_muons_DeltaEtaSigned_norm[i_author][i_quality]
				    -> Fill( (eta - eta_mc)/eta_mc, GetTagEvtWeight());
		      
		    // Phi
		    h_MC_vs_REC_muons_Phi[i_author][i_quality]
		      -> Fill(phi_mc, phi           , GetTagEvtWeight());
		    h_MC_vs_REC_muons_DeltaPhiSigned[i_author][i_quality]
		      -> Fill(phi - phi_mc          , GetTagEvtWeight());	  
		    if(phi_mc!=0) h_MC_vs_REC_muons_DeltaPhiSigned_norm[i_author][i_quality]
				    -> Fill( (phi - phi_mc)/phi_mc, GetTagEvtWeight());
		      
		    // Pt
		    h_MC_vs_REC_muons_Pt[i_author][i_quality]
		      -> Fill(pt_mc, pt , GetTagEvtWeight());
		    h_MC_vs_REC_muons_DeltaPtSigned[i_author][i_quality]
		      -> Fill(pt - pt_mc, GetTagEvtWeight());
		      
		    if(pt_mc!=0){ 
		      h_MC_vs_REC_muons_DeltaPtSigned_norm[i_author][i_quality]
			-> Fill( (pt - pt_mc)/pt_mc          , GetTagEvtWeight());
		      // Fill Profile plots
		      prof_MC_vs_REC_muons_PtMC_vs_DeltaPt_norm[i_author][i_quality]
			-> Fill( pt_mc,  (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt_norm[i_author][i_quality]
			-> Fill( eta_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt_norm[i_author][i_quality]
			-> Fill( phi_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
		      prof_MC_vs_REC_muons_PtMC_vs_DeltaPt[i_author][i_quality]
			-> Fill( pt_mc,  (pt - pt_mc)        , GetTagEvtWeight());
		      prof_MC_vs_REC_muons_EtaMC_vs_DeltaPt[i_author][i_quality]
			-> Fill( eta_mc, (pt - pt_mc)        , GetTagEvtWeight());
		      prof_MC_vs_REC_muons_PhiMC_vs_DeltaPt[i_author][i_quality]
			-> Fill( phi_mc, (pt - pt_mc)        , GetTagEvtWeight());
		    }
		  }
		}
		  
		  // ============
		  // Both charges
		  // ============
		  
		  
		  h_muons_all_eta[i_author][i_quality]->Fill(eta, GetTagEvtWeight());
		  h_muons_all_phi[i_author][i_quality]->Fill(phi, GetTagEvtWeight());
		  h_muons_all_pt[i_author][i_quality]->Fill(pt,  GetTagEvtWeight());

		  h_muons_etcone10[i_author][i_quality]->Fill(mu->GetEtCone10(), GetTagEvtWeight());
		  h_muons_etcone20[i_author][i_quality]->Fill(mu->GetEtCone20(), GetTagEvtWeight());
		  h_muons_etcone30[i_author][i_quality]->Fill(mu->GetEtCone30(), GetTagEvtWeight());
		  h_muons_etcone40[i_author][i_quality]->Fill(mu->GetEtCone40(), GetTagEvtWeight());
		  
		  if ( muplus ) {
		      //===============
		      // mu+ only
		      //===============
		      
		      
		      h_muons_plus_eta[i_author][i_quality]->Fill(eta, GetTagEvtWeight());
		      h_muons_plus_phi[i_author][i_quality]->Fill(phi, GetTagEvtWeight());
		      h_muons_plus_pt[i_author][i_quality]->Fill(pt,  GetTagEvtWeight());
		      
		  } else {
		      //===============
		      // mu- only
		      //===============
		      
		      
		      h_muons_minus_eta[i_author][i_quality]->Fill(eta, GetTagEvtWeight());
		      h_muons_minus_phi[i_author][i_quality]->Fill(phi, GetTagEvtWeight());
		      h_muons_minus_pt[i_author][i_quality]->Fill(pt,  GetTagEvtWeight());
		      
		  }
	      }	      
	  }	  
      }
  }
  

  // ------------------------------------------------------------------------
  
  // Fill in counts of muons in event 
  
  // ------------------------------------------------------------------------
  

    
  h_muons_all_n  ->Fill(n_muons      , GetTagEvtWeight());
  h_muons_plus_n ->Fill(n_muons_plus , GetTagEvtWeight());
  h_muons_minus_n->Fill(n_muons_minus, GetTagEvtWeight());
	
  // ------------------------------------------------------------------------
  //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	      


    
//   // Get histogram pointers
//   TH1F *h_all_n   = (TH1F*)GetHistogram("h_muon_n",   EvtSelection);
//   TH1F *h_all_eta = (TH1F*)GetHistogram("h_muon_eta", EvtSelection);
//   TH1F *h_all_phi = (TH1F*)GetHistogram("h_muon_phi", EvtSelection);
//   TH1F *h_all_pt  = (TH1F*)GetHistogram("h_muon_pt",  EvtSelection);
  
//   TH1F *h_combSTACO_all_n   = (TH1F*)GetHistogram("h_combSTACO_muon_n",   EvtSelection);
//   TH1F *h_combSTACO_all_eta = (TH1F*)GetHistogram("h_combSTACO_muon_eta", EvtSelection);
//   TH1F *h_combSTACO_all_phi = (TH1F*)GetHistogram("h_combSTACO_muon_phi", EvtSelection);
//   TH1F *h_combSTACO_all_pt  = (TH1F*)GetHistogram("h_combSTACO_muon_pt",  EvtSelection);
  
//   TH1F *h_combSTACO_etcone10 = (TH1F*)GetHistogram("h_combSTACO_muon_EtCone10", EvtSelection);
//   TH1F *h_combSTACO_etcone20 = (TH1F*)GetHistogram("h_combSTACO_muon_EtCone20", EvtSelection);
//   TH1F *h_combSTACO_etcone30 = (TH1F*)GetHistogram("h_combSTACO_muon_EtCone30", EvtSelection);
//   TH1F *h_combSTACO_etcone40 = (TH1F*)GetHistogram("h_combSTACO_muon_EtCone40", EvtSelection);
  

//   TH1F *h_combSTACO_chi2 = (TH1F*)GetHistogram("h_combSTACO_muon_chi2", EvtSelection);
//   TH1F *h_author = (TH1F*)GetHistogram("h_muon_author", EvtSelection);
  
//   TH1F *h_plus_n   = (TH1F*)GetHistogram("h_muplus_n",   EvtSelection);
//   TH1F *h_plus_eta = (TH1F*)GetHistogram("h_muplus_eta", EvtSelection);
//   TH1F *h_plus_phi = (TH1F*)GetHistogram("h_muplus_phi", EvtSelection);
//   TH1F *h_plus_pt  = (TH1F*)GetHistogram("h_muplus_pt",  EvtSelection);
  
//   TH1F *h_combSTACO_plus_n   = (TH1F*)GetHistogram("h_combSTACO_muplus_n",   EvtSelection);
//   TH1F *h_combSTACO_plus_eta = (TH1F*)GetHistogram("h_combSTACO_muplus_eta", EvtSelection);
//   TH1F *h_combSTACO_plus_phi = (TH1F*)GetHistogram("h_combSTACO_muplus_phi", EvtSelection);
//   TH1F *h_combSTACO_plus_pt  = (TH1F*)GetHistogram("h_combSTACO_muplus_pt",  EvtSelection);
  
//   TH1F *h_minus_n   = (TH1F*)GetHistogram("h_muminus_n",   EvtSelection);
//   TH1F *h_minus_eta = (TH1F*)GetHistogram("h_muminus_eta", EvtSelection);
//   TH1F *h_minus_phi = (TH1F*)GetHistogram("h_muminus_phi", EvtSelection);
//   TH1F *h_minus_pt  = (TH1F*)GetHistogram("h_muminus_pt",  EvtSelection);
  
//   TH1F *h_combSTACO_minus_n   = (TH1F*)GetHistogram("h_combSTACO_muminus_n",   EvtSelection);
//   TH1F *h_combSTACO_minus_eta = (TH1F*)GetHistogram("h_combSTACO_muminus_eta", EvtSelection);
//   TH1F *h_combSTACO_minus_phi = (TH1F*)GetHistogram("h_combSTACO_muminus_phi", EvtSelection);
//   TH1F *h_combSTACO_minus_pt  = (TH1F*)GetHistogram("h_combSTACO_muminus_pt",  EvtSelection);
  
//   // MC comparison
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaEtaSigned = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaEtaSigned",   EvtSelection); 
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaEtaSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaEtaSigned_norm",   EvtSelection); 
//   TH2F *h_MC_vs_REC_StacoMuons_Eta            = (TH2F*)GetHistogram("h_MC_vs_REC_StacoMuons_Eta",   EvtSelection);
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaPhiSigned = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaPhiSigned",   EvtSelection); 
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaPhiSigned_norm = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaPhiSigned_norm",   EvtSelection); 
//   TH2F *h_MC_vs_REC_StacoMuons_Phi            = (TH2F*)GetHistogram("h_MC_vs_REC_StacoMuons_Phi",   EvtSelection);
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaPtSigned  = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaPtSigned",   EvtSelection); 
//   TH1F *h_MC_vs_REC_StacoMuons_DeltaPtSigned_norm  = (TH1F*)GetHistogram("h_MC_vs_REC_StacoMuons_DeltaPtSigned_norm",   EvtSelection); 
//   TH2F *h_MC_vs_REC_StacoMuons_Pt             = (TH2F*)GetHistogram("h_MC_vs_REC_StacoMuons_Pt",   EvtSelection);
  
//   TProfile *prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt       = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt",       EvtSelection);
//   TProfile *prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt",      EvtSelection);
//   TProfile *prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt      = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt",      EvtSelection);
//   TProfile *prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt_norm  = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt_norm" , EvtSelection);
//   TProfile *prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt_norm", EvtSelection);
//   TProfile *prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt_norm = (TProfile*)GetHistogram("prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt_norm", EvtSelection);
  
//   // Fill them
//   AtlMuon *mu = 0;
  
//   HepMCParticle *mu_mc =0;
  
//   Int_t n_plus = 0; Int_t n_minus = 0;
//   Int_t n_combSTACO_plus = 0; Int_t n_combSTACO_minus = 0; Int_t n_combSTACO = 0;
  
//   Float_t eta = 0.;
//   Float_t phi = 0.;
//   Float_t pt  = 0.;
//   Float_t eta_mc = 0.;
//   Float_t phi_mc = 0.;
//   Float_t pt_mc  = 0.;
  
//   Bool_t combstaco;
  
//   for ( Int_t i = 0; i < fEvent->GetN_Muons(); i++ ) {
//     mu = (AtlMuon*)fEvent->GetMuons()->At(i);
//     phi = mu->Phi();
//     pt  = mu->Pt();
//     eta = mu->Eta();
    
//     // Find best matching HepMCParticle
    
//     mu_mc = (HepMCParticle*)fEvent->FindMatchedMCParticle(mu);
//     if(mu_mc !=0){
//       phi_mc = mu_mc->Phi();
//       pt_mc  = mu_mc->Pt();
//       eta_mc = mu_mc->Eta();
      
//       // Fill MC vs REC comparison histograms
      
//       // eta
//       h_MC_vs_REC_StacoMuons_Eta                 -> Fill(eta_mc,  eta, GetTagEvtWeight());
//       h_MC_vs_REC_StacoMuons_DeltaEtaSigned      -> Fill(eta_mc - eta, GetTagEvtWeight());
      
//       if(eta_mc!=0) h_MC_vs_REC_StacoMuons_DeltaEtaSigned_norm -> Fill( (eta - eta_mc)/eta_mc, GetTagEvtWeight());
      
//       //phi
//       h_MC_vs_REC_StacoMuons_Phi                 -> Fill(phi_mc,  phi, GetTagEvtWeight());
//       h_MC_vs_REC_StacoMuons_DeltaPhiSigned      -> Fill(phi - phi_mc, GetTagEvtWeight());
      
//       if(phi_mc!=0) h_MC_vs_REC_StacoMuons_DeltaPhiSigned_norm -> Fill( (phi - phi_mc)/phi_mc, GetTagEvtWeight());
      
//       //Pt
//       h_MC_vs_REC_StacoMuons_Pt                  -> Fill(pt_mc,pt, GetTagEvtWeight());
//       h_MC_vs_REC_StacoMuons_DeltaPtSigned       -> Fill(pt - pt_mc, GetTagEvtWeight());
      
//       if(pt_mc!=0) {
// 	h_MC_vs_REC_StacoMuons_DeltaPtSigned_norm  -> Fill( (pt - pt_mc)/pt_mc, GetTagEvtWeight());      
// 	prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt_norm  -> Fill( pt_mc,  (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
// 	prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt_norm -> Fill( eta_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
// 	prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt_norm -> Fill( phi_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
// 	prof_MC_vs_REC_StacoMuons_PtMC_vs_DeltaPt	  -> Fill( pt_mc,  (pt - pt_mc)        , GetTagEvtWeight());
// 	prof_MC_vs_REC_StacoMuons_EtaMC_vs_DeltaPt	  -> Fill( eta_mc, (pt - pt_mc)        , GetTagEvtWeight());
// 	prof_MC_vs_REC_StacoMuons_PhiMC_vs_DeltaPt	  -> Fill( phi_mc, (pt - pt_mc)        , GetTagEvtWeight());
//       }
//     }
    
//     combstaco = mu->IsCombinedSTACO();
    
//     // ============
//     // Both charges
//     // ============
//     h_all_eta->Fill(eta, GetTagEvtWeight());
//     h_all_phi->Fill(phi, GetTagEvtWeight());
//     h_all_pt ->Fill(pt,  GetTagEvtWeight());
    
//     if (combstaco ){
//       n_combSTACO++;
//       h_combSTACO_all_eta->Fill(eta, GetTagEvtWeight());
//       h_combSTACO_all_phi->Fill(phi, GetTagEvtWeight());
//       h_combSTACO_all_pt ->Fill(pt,  GetTagEvtWeight());
      
      
//       h_combSTACO_etcone10->Fill(mu->GetEtCone10(), GetTagEvtWeight());
//       h_combSTACO_etcone20->Fill(mu->GetEtCone20(), GetTagEvtWeight());
//       h_combSTACO_etcone30->Fill(mu->GetEtCone30(), GetTagEvtWeight());
//       h_combSTACO_etcone40->Fill(mu->GetEtCone40(), GetTagEvtWeight());
      
//       h_combSTACO_ptcone10->Fill(mu->GetPtCone10(), GetTagEvtWeight());
//       h_combSTACO_ptcone20->Fill(mu->GetPtCone20(), GetTagEvtWeight());
//       h_combSTACO_ptcone30->Fill(mu->GetPtCone30(), GetTagEvtWeight());
//       h_combSTACO_ptcone40->Fill(mu->GetPtCone40(), GetTagEvtWeight());
      
//       h_combSTACO_ntrkcone10->Fill(mu->GetNtrkCone10(), GetTagEvtWeight());
//       h_combSTACO_ntrkcone20->Fill(mu->GetNtrkCone20(), GetTagEvtWeight());
//       h_combSTACO_ntrkcone30->Fill(mu->GetNtrkCone30(), GetTagEvtWeight());
//       h_combSTACO_ntrkcone40->Fill(mu->GetNtrkCone40(), GetTagEvtWeight());
      
//     }
    
//     h_muons_chi2->Fill(mu->GetMatchingChi2overNDoF(), GetTagEvtWeight());
    
//     if ( combstaco ){
//       h_combSTACO_chi2->Fill(mu->GetMatchingChi2overNDoF(), GetTagEvtWeight());
//     }
    
//     AtlMuon::EAuthor author = mu->GetAuthor();
//     for ( Int_t i = 0; i < AtlMuon::fgNAuthors; i++ ) {
//       if ( author & 1<<i ) h_author->AddBinContent(i+1, GetTagEvtWeight());
//     }
    
//     if ( mu->IsPositive() ) {
//       // ========
//       // Mu+ only
//       // ========
//       n_plus++;
//       h_plus_eta->Fill(eta, GetTagEvtWeight());
//       h_plus_phi->Fill(phi, GetTagEvtWeight());
//       h_plus_pt ->Fill(pt,  GetTagEvtWeight());
      
//       if ( combstaco ){
// 	n_combSTACO_plus++;
// 	h_combSTACO_plus_eta->Fill(eta, GetTagEvtWeight());
// 	h_combSTACO_plus_phi->Fill(phi, GetTagEvtWeight());
// 	h_combSTACO_plus_pt ->Fill(pt,  GetTagEvtWeight());
//       }
      
//     } else {
//       // ========
//       // Mu- only
//       // ========
//       n_minus++;
//       h_minus_eta->Fill(eta, GetTagEvtWeight());
//       h_minus_phi->Fill(phi, GetTagEvtWeight());
//       h_minus_pt ->Fill(pt,  GetTagEvtWeight());
      
//       if ( combstaco ){
// 	n_combSTACO_minus++;
// 	h_combSTACO_minus_eta->Fill(eta, GetTagEvtWeight());
// 	h_combSTACO_minus_phi->Fill(phi, GetTagEvtWeight());
// 	h_combSTACO_minus_pt ->Fill(pt,  GetTagEvtWeight());	
//       }
//     }
//   }
//   h_all_n  ->Fill(fEvent->GetN_Muons(), GetTagEvtWeight());
//   h_plus_n ->Fill(n_plus,  GetTagEvtWeight());
//   h_minus_n->Fill(n_minus, GetTagEvtWeight());
  
//   h_combSTACO_all_n  ->Fill(n_combSTACO, GetTagEvtWeight());
//   h_combSTACO_plus_n ->Fill(n_combSTACO_plus,  GetTagEvtWeight());
//   h_combSTACO_minus_n->Fill(n_combSTACO_minus, GetTagEvtWeight());
}


//__________________________________________________________

  void AtlCtrlPlots::FillTauHistograms(Bool_t EvtSelection) {
    //
    // Fill tauon histograms
    //



    // Get histogram pointers



    
    // -------
    // TauRec  ----------------------------------------------------------------------
    // -------


    
    // ...............
    // TauCutTight 
    // ...............

    // ============
    // Both charges
    // ============
      
    TH1F *h_tauctight_taurec_all_n   = (TH1F*)GetHistogram("h_tauctight_taurec_tauon_n",   EvtSelection);
    TH1F *h_tauctight_taurec_all_eta = (TH1F*)GetHistogram("h_tauctight_taurec_tauon_eta", EvtSelection);
    TH1F *h_tauctight_taurec_all_phi = (TH1F*)GetHistogram("h_tauctight_taurec_tauon_phi", EvtSelection);
    TH1F *h_tauctight_taurec_all_pt  = (TH1F*)GetHistogram("h_tauctight_taurec_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_tauctight_taurec_plus_n   = (TH1F*)GetHistogram("h_tauctight_taurec_tauplus_n",   EvtSelection);
    TH1F *h_tauctight_taurec_plus_eta = (TH1F*)GetHistogram("h_tauctight_taurec_tauplus_eta", EvtSelection);
    TH1F *h_tauctight_taurec_plus_phi = (TH1F*)GetHistogram("h_tauctight_taurec_tauplus_phi", EvtSelection);
    TH1F *h_tauctight_taurec_plus_pt  = (TH1F*)GetHistogram("h_tauctight_taurec_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_tauctight_taurec_minus_n   = (TH1F*)GetHistogram("h_tauctight_taurec_tauminus_n",   EvtSelection);
    TH1F *h_tauctight_taurec_minus_eta = (TH1F*)GetHistogram("h_tauctight_taurec_tauminus_eta", EvtSelection);
    TH1F *h_tauctight_taurec_minus_phi = (TH1F*)GetHistogram("h_tauctight_taurec_tauminus_phi", EvtSelection);
    TH1F *h_tauctight_taurec_minus_pt  = (TH1F*)GetHistogram("h_tauctight_taurec_tauminus_pt",  EvtSelection);
    
    
    // ................
    // TauCutMedium
    // ................
    
    // ============
    // Both charges
    // ============
      
    TH1F *h_taucmedium_taurec_all_n   = (TH1F*)GetHistogram("h_taucmedium_taurec_tauon_n",   EvtSelection);
    TH1F *h_taucmedium_taurec_all_eta = (TH1F*)GetHistogram("h_taucmedium_taurec_tauon_eta", EvtSelection);
    TH1F *h_taucmedium_taurec_all_phi = (TH1F*)GetHistogram("h_taucmedium_taurec_tauon_phi", EvtSelection);
    TH1F *h_taucmedium_taurec_all_pt  = (TH1F*)GetHistogram("h_taucmedium_taurec_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_taucmedium_taurec_plus_n   = (TH1F*)GetHistogram("h_taucmedium_taurec_tauplus_n",   EvtSelection);
    TH1F *h_taucmedium_taurec_plus_eta = (TH1F*)GetHistogram("h_taucmedium_taurec_tauplus_eta", EvtSelection);
    TH1F *h_taucmedium_taurec_plus_phi = (TH1F*)GetHistogram("h_taucmedium_taurec_tauplus_phi", EvtSelection);
    TH1F *h_taucmedium_taurec_plus_pt  = (TH1F*)GetHistogram("h_taucmedium_taurec_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_taucmedium_taurec_minus_n   = (TH1F*)GetHistogram("h_taucmedium_taurec_tauminus_n",   EvtSelection);
    TH1F *h_taucmedium_taurec_minus_eta = (TH1F*)GetHistogram("h_taucmedium_taurec_tauminus_eta", EvtSelection);
    TH1F *h_taucmedium_taurec_minus_phi = (TH1F*)GetHistogram("h_taucmedium_taurec_tauminus_phi", EvtSelection);
    TH1F *h_taucmedium_taurec_minus_pt  = (TH1F*)GetHistogram("h_taucmedium_taurec_tauminus_pt",  EvtSelection);


    
    // ...............
    // TauCutLoose
    // ...............

    // ============
    // Both charges
    // ============
      
    TH1F *h_taucloose_taurec_all_n   = (TH1F*)GetHistogram("h_taucloose_taurec_tauon_n",   EvtSelection);
    TH1F *h_taucloose_taurec_all_eta = (TH1F*)GetHistogram("h_taucloose_taurec_tauon_eta", EvtSelection);
    TH1F *h_taucloose_taurec_all_phi = (TH1F*)GetHistogram("h_taucloose_taurec_tauon_phi", EvtSelection);
    TH1F *h_taucloose_taurec_all_pt  = (TH1F*)GetHistogram("h_taucloose_taurec_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_taucloose_taurec_plus_n   = (TH1F*)GetHistogram("h_taucloose_taurec_tauplus_n",   EvtSelection);
    TH1F *h_taucloose_taurec_plus_eta = (TH1F*)GetHistogram("h_taucloose_taurec_tauplus_eta", EvtSelection);
    TH1F *h_taucloose_taurec_plus_phi = (TH1F*)GetHistogram("h_taucloose_taurec_tauplus_phi", EvtSelection);
    TH1F *h_taucloose_taurec_plus_pt  = (TH1F*)GetHistogram("h_taucloose_taurec_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_taucloose_taurec_minus_n   = (TH1F*)GetHistogram("h_taucloose_taurec_tauminus_n",   EvtSelection);
    TH1F *h_taucloose_taurec_minus_eta = (TH1F*)GetHistogram("h_taucloose_taurec_tauminus_eta", EvtSelection);
    TH1F *h_taucloose_taurec_minus_phi = (TH1F*)GetHistogram("h_taucloose_taurec_tauminus_phi", EvtSelection);
    TH1F *h_taucloose_taurec_minus_pt  = (TH1F*)GetHistogram("h_taucloose_taurec_tauminus_pt",  EvtSelection);





    
    // -------
    // Tau1P3P ----------------------------------------------------------------------
    // -------


    
    // ...............
    // TauCutTight
    // ...............

    // ============
    // Both charges
    // ============
      
    TH1F *h_tauctight_1p3p_all_n   = (TH1F*)GetHistogram("h_tauctight_1p3p_tauon_n",   EvtSelection);
    TH1F *h_tauctight_1p3p_all_eta = (TH1F*)GetHistogram("h_tauctight_1p3p_tauon_eta", EvtSelection);
    TH1F *h_tauctight_1p3p_all_phi = (TH1F*)GetHistogram("h_tauctight_1p3p_tauon_phi", EvtSelection);
    TH1F *h_tauctight_1p3p_all_pt  = (TH1F*)GetHistogram("h_tauctight_1p3p_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_tauctight_1p3p_plus_n   = (TH1F*)GetHistogram("h_tauctight_1p3p_tauplus_n",   EvtSelection);
    TH1F *h_tauctight_1p3p_plus_eta = (TH1F*)GetHistogram("h_tauctight_1p3p_tauplus_eta", EvtSelection);
    TH1F *h_tauctight_1p3p_plus_phi = (TH1F*)GetHistogram("h_tauctight_1p3p_tauplus_phi", EvtSelection);
    TH1F *h_tauctight_1p3p_plus_pt  = (TH1F*)GetHistogram("h_tauctight_1p3p_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_tauctight_1p3p_minus_n   = (TH1F*)GetHistogram("h_tauctight_1p3p_tauminus_n",   EvtSelection);
    TH1F *h_tauctight_1p3p_minus_eta = (TH1F*)GetHistogram("h_tauctight_1p3p_tauminus_eta", EvtSelection);
    TH1F *h_tauctight_1p3p_minus_phi = (TH1F*)GetHistogram("h_tauctight_1p3p_tauminus_phi", EvtSelection);
    TH1F *h_tauctight_1p3p_minus_pt  = (TH1F*)GetHistogram("h_tauctight_1p3p_tauminus_pt",  EvtSelection);
    

    
    // ................
    // TauCutMedium
    // ................
    
    // ============
    // Both charges
    // ============
     
    TH1F *h_taucmedium_1p3p_all_n   = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauon_n",   EvtSelection);
    TH1F *h_taucmedium_1p3p_all_eta = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauon_eta", EvtSelection);
    TH1F *h_taucmedium_1p3p_all_phi = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauon_phi", EvtSelection);
    TH1F *h_taucmedium_1p3p_all_pt  = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_taucmedium_1p3p_plus_n   = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauplus_n",   EvtSelection);
    TH1F *h_taucmedium_1p3p_plus_eta = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauplus_eta", EvtSelection);
    TH1F *h_taucmedium_1p3p_plus_phi = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauplus_phi", EvtSelection);
    TH1F *h_taucmedium_1p3p_plus_pt  = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_taucmedium_1p3p_minus_n   = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauminus_n",   EvtSelection);
    TH1F *h_taucmedium_1p3p_minus_eta = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauminus_eta", EvtSelection);
    TH1F *h_taucmedium_1p3p_minus_phi = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauminus_phi", EvtSelection);
    TH1F *h_taucmedium_1p3p_minus_pt  = (TH1F*)GetHistogram("h_taucmedium_1p3p_tauminus_pt",  EvtSelection);


    
    // ...............
    // TauCutLoose
    // ...............

    // ============
    // Both charges
    // ============
      
    TH1F *h_taucloose_1p3p_all_n   = (TH1F*)GetHistogram("h_taucloose_1p3p_tauon_n",   EvtSelection);
    TH1F *h_taucloose_1p3p_all_eta = (TH1F*)GetHistogram("h_taucloose_1p3p_tauon_eta", EvtSelection);
    TH1F *h_taucloose_1p3p_all_phi = (TH1F*)GetHistogram("h_taucloose_1p3p_tauon_phi", EvtSelection);
    TH1F *h_taucloose_1p3p_all_pt  = (TH1F*)GetHistogram("h_taucloose_1p3p_tauon_pt",  EvtSelection);

    
    // ===
    // tau+
    // ===
   
    TH1F *h_taucloose_1p3p_plus_n   = (TH1F*)GetHistogram("h_taucloose_1p3p_tauplus_n",   EvtSelection);
    TH1F *h_taucloose_1p3p_plus_eta = (TH1F*)GetHistogram("h_taucloose_1p3p_tauplus_eta", EvtSelection);
    TH1F *h_taucloose_1p3p_plus_phi = (TH1F*)GetHistogram("h_taucloose_1p3p_tauplus_phi", EvtSelection);
    TH1F *h_taucloose_1p3p_plus_pt  = (TH1F*)GetHistogram("h_taucloose_1p3p_tauplus_pt",  EvtSelection);

    
    // ===
    // tau-
    // ===
   
    TH1F *h_taucloose_1p3p_minus_n   = (TH1F*)GetHistogram("h_taucloose_1p3p_tauminus_n",   EvtSelection);
    TH1F *h_taucloose_1p3p_minus_eta = (TH1F*)GetHistogram("h_taucloose_1p3p_tauminus_eta", EvtSelection);
    TH1F *h_taucloose_1p3p_minus_phi = (TH1F*)GetHistogram("h_taucloose_1p3p_tauminus_phi", EvtSelection);
    TH1F *h_taucloose_1p3p_minus_pt  = (TH1F*)GetHistogram("h_taucloose_1p3p_tauminus_pt",  EvtSelection);


    // Fill histograms for eta, phi, pt


    
    AtlTau *tau = 0;

    Int_t n_tauctight_taurec = 0;
    Int_t n_tauctight_taurec_plus = 0;
    Int_t n_tauctight_taurec_minus = 0;
    Int_t n_taucmedium_taurec = 0;
    Int_t n_taucmedium_taurec_plus = 0;
    Int_t n_taucmedium_taurec_minus = 0;
    Int_t n_taucloose_taurec = 0;
    Int_t n_taucloose_taurec_plus = 0;
    Int_t n_taucloose_taurec_minus = 0;
    
    Int_t n_tauctight_1p3p = 0;
    Int_t n_tauctight_1p3p_plus = 0;
    Int_t n_tauctight_1p3p_minus = 0;
    Int_t n_taucmedium_1p3p = 0;
    Int_t n_taucmedium_1p3p_plus = 0;
    Int_t n_taucmedium_1p3p_minus = 0;
    Int_t n_taucloose_1p3p = 0;
    Int_t n_taucloose_1p3p_plus = 0;
    Int_t n_taucloose_1p3p_minus = 0;

    
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t pt  = 0.;
    
    Bool_t taurec;
    Bool_t tau_1p3p;

    Bool_t tauctight;
    Bool_t taucmedium;
    Bool_t taucloose;
    
    Bool_t tauplus;


    
    
    for ( Int_t i = 0; i < fEvent->GetN_Taus(); i++ ) {
      tau = (AtlTau*)fEvent->GetTaus()->At(i);

      // variables
      pt  = tau->Pt();
      eta = tau->Eta();
      phi = tau->Phi();

      // authors
      taurec = tau->IsTauRec();
      tau_1p3p = tau->IsTau1P3P();

      //reconstruction-flags
      // tauctight = tau->IsTauCutTight();
      // taucmedium = tau->IsTauCutMedium();
      // taucloose = tau->IsTauCutLoose();

      // charge
      tauplus = tau->IsTauPlus();
	
 
	
      // -------
      // TauRec  --------------------------------------------------------------
      // -------

      if( taurec ) {
	    
	// ...............
	// TauCutTight
	// ...............
	    
	if( tauctight ) {

	  n_tauctight_taurec++;

		
	  // ============
	  // Both charges
	  // ============
	
	  h_tauctight_taurec_all_eta->Fill(eta, GetTagEvtWeight());
	  h_tauctight_taurec_all_phi->Fill(phi, GetTagEvtWeight());
	  h_tauctight_taurec_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_tauctight_taurec_plus++;
		    
	    h_tauctight_taurec_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_tauctight_taurec_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_tauctight_taurec_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_tauctight_taurec_minus++;
		    
	    h_tauctight_taurec_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_tauctight_taurec_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_tauctight_taurec_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
	}


	// ................
	// TauCutMedium
	// ................
	    
	if( taucmedium ) {

	  n_taucmedium_taurec++;

		
	  // ============
	  // Both charges
	  // ============
	
	  h_taucmedium_taurec_all_eta->Fill(eta, GetTagEvtWeight());
	  h_taucmedium_taurec_all_phi->Fill(phi, GetTagEvtWeight());
	  h_taucmedium_taurec_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_taucmedium_taurec_plus++;
		    
	    h_taucmedium_taurec_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucmedium_taurec_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucmedium_taurec_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_taucmedium_taurec_minus++;
		    
	    h_taucmedium_taurec_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucmedium_taurec_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucmedium_taurec_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
	}

	    
	// ...............
	// TauCutLoose
	// ...............
	    
	if( taucloose ) {
		
	  n_taucloose_taurec++;
		
		
	  // ============
	  // Both charges
	  // ============
	
	  h_taucloose_taurec_all_eta->Fill(eta, GetTagEvtWeight());
	  h_taucloose_taurec_all_phi->Fill(phi, GetTagEvtWeight());
	  h_taucloose_taurec_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_taucloose_taurec_plus++;
		    
	    h_taucloose_taurec_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucloose_taurec_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucloose_taurec_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_taucloose_taurec_minus++;
		    
	    h_taucloose_taurec_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucloose_taurec_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucloose_taurec_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
	}
      }


      // --------------------------------------------------------------------


      // -------
      // Tau1P3P   --------------------------------------------------------------
      // -------


      if( tau_1p3p ) {
	    
	// ...............
	// TauCutTight
	// ...............
	    
	if( tauctight ) {

	  n_tauctight_1p3p++;

		
	  // ============
	  // Both charges
	  // ============
	
	  h_tauctight_1p3p_all_eta->Fill(eta, GetTagEvtWeight());
	  h_tauctight_1p3p_all_phi->Fill(phi, GetTagEvtWeight());
	  h_tauctight_1p3p_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_tauctight_1p3p_plus++;
		    
	    h_tauctight_1p3p_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_tauctight_1p3p_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_tauctight_1p3p_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_tauctight_1p3p_minus++;
		    
	    h_tauctight_1p3p_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_tauctight_1p3p_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_tauctight_1p3p_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
	}


	// ................
	// TauCutMedium
	// ................
	    
	if( taucmedium ) {

	  n_taucmedium_1p3p++;

		
	  // ============
	  // Both charges
	  // ============
	
	  h_taucmedium_1p3p_all_eta->Fill(eta, GetTagEvtWeight());
	  h_taucmedium_1p3p_all_phi->Fill(phi, GetTagEvtWeight());
	  h_taucmedium_1p3p_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_taucmedium_1p3p_plus++;
		    
	    h_taucmedium_1p3p_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucmedium_1p3p_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucmedium_1p3p_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_taucmedium_1p3p_minus++;
		    
	    h_taucmedium_1p3p_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucmedium_1p3p_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucmedium_1p3p_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
	}

	    
	// ...............
	// TauCutLoose
	// ...............
	    
	if( taucloose ) {
		
	  n_taucloose_1p3p++;
		
		
	  // ============
	  // Both charges
	  // ============
	
	  h_taucloose_1p3p_all_eta->Fill(eta, GetTagEvtWeight());
	  h_taucloose_1p3p_all_phi->Fill(phi, GetTagEvtWeight());
	  h_taucloose_1p3p_all_pt ->Fill(pt,  GetTagEvtWeight());

	  if ( tauplus ) { // Tau+ only

	    n_taucloose_1p3p_plus++;
		    
	    h_taucloose_1p3p_plus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucloose_1p3p_plus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucloose_1p3p_plus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  } else { // Tau- only

	    n_taucloose_1p3p_minus++;
		    
	    h_taucloose_1p3p_minus_eta->Fill(eta, GetTagEvtWeight());
	    h_taucloose_1p3p_minus_phi->Fill(phi, GetTagEvtWeight());
	    h_taucloose_1p3p_minus_pt ->Fill(pt,  GetTagEvtWeight());
		    
	  }
		
	}
	    
      }
	
    }

    
    // ------------------------------------------------------------------------


    // Fill in counts of tauons in event
	
    // -------
    // TauRec  --------------------------------------------------------------
    // -------
    
    h_tauctight_taurec_all_n->Fill(n_tauctight_taurec, GetTagEvtWeight());
    h_tauctight_taurec_plus_n->Fill(n_tauctight_taurec_plus, GetTagEvtWeight());
    h_tauctight_taurec_minus_n->Fill(n_tauctight_taurec_minus, GetTagEvtWeight());
	
    h_taucmedium_taurec_all_n->Fill(n_taucmedium_taurec, GetTagEvtWeight());
    h_taucmedium_taurec_plus_n->Fill(n_taucmedium_taurec_plus, GetTagEvtWeight());
    h_taucmedium_taurec_minus_n->Fill(n_taucmedium_taurec_minus, GetTagEvtWeight());
    
    h_taucloose_taurec_all_n->Fill(n_taucloose_taurec, GetTagEvtWeight());
    h_taucloose_taurec_plus_n->Fill(n_taucloose_taurec_plus, GetTagEvtWeight());
    h_taucloose_taurec_minus_n->Fill(n_taucloose_taurec_minus, GetTagEvtWeight());

    // -------
    // Tau1P3P --------------------------------------------------------------
    // -------
    
    h_tauctight_1p3p_all_n->Fill(n_tauctight_1p3p, GetTagEvtWeight());
    h_tauctight_1p3p_plus_n->Fill(n_tauctight_1p3p_plus, GetTagEvtWeight());
    h_tauctight_1p3p_minus_n->Fill(n_tauctight_1p3p_minus, GetTagEvtWeight());
	
    h_taucmedium_1p3p_all_n->Fill(n_taucmedium_1p3p, GetTagEvtWeight());
    h_taucmedium_1p3p_plus_n->Fill(n_taucmedium_1p3p_plus, GetTagEvtWeight());
    h_taucmedium_1p3p_minus_n->Fill(n_taucmedium_1p3p_minus, GetTagEvtWeight());
    
    h_taucloose_1p3p_all_n->Fill(n_taucloose_1p3p, GetTagEvtWeight());
    h_taucloose_1p3p_plus_n->Fill(n_taucloose_1p3p_plus, GetTagEvtWeight());
    h_taucloose_1p3p_minus_n->Fill(n_taucloose_1p3p_minus, GetTagEvtWeight());
    
    // ------------------------------------------------------------------------
    
  }

  //____________________________________________________________________

void AtlCtrlPlots::FillPhotonHistograms(Bool_t EvtSelection) {
    //
    // Fill photon histograms
    //
    
         
    //=======================
    // Get histogram pointers
    //======================= 
    
    //******************************
    // Number of photons by author
    //******************************
    
    TH1F *h_ph_author = (TH1F*)GetHistogram("h_photon_author", EvtSelection);
    
    //******************************
    
    //*******************************
    // Number of photons by IsEM-Flag
    //*******************************
    
    TH1F *h_ph_isem = (TH1F*)GetHistogram("h_photon_isem", EvtSelection);
    
    //*******************************
    
    //-----------------------------------------------------
    // all other histograms, sorted by author and IsEM flag
    //-----------------------------------------------------
    
    // !!!!!
    // NOTE: [0][0], [0][x], [x][0] exist in arrays below, but don't get filled, since 0 <-> invalid!!!
    // BETTER IDEA to synchronise arrays with loop-requirements?
    //  -> can you tell an array in C++ to begin with 1?
    //  -> if not, can you synchronise in a way that does not assume invalid corresponding to index 0 ?
    // !!!!!
    
    //if (fEvent->IsMC()) {
      // MC vs REC
    
      TH1F *h_MC_vs_REC_photons_DeltaEtaSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH1F *h_MC_vs_REC_photons_DeltaEtaSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH2F *h_MC_vs_REC_photons_Eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH1F *h_MC_vs_REC_photons_DeltaPhiSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH1F *h_MC_vs_REC_photons_DeltaPhiSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH2F *h_MC_vs_REC_photons_Phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH1F *h_MC_vs_REC_photons_DeltaPtSigned[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH1F *h_MC_vs_REC_photons_DeltaPtSigned_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TH2F *h_MC_vs_REC_photons_Pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    
      // Profile Plots
    
      TProfile *prof_MC_vs_REC_photons_PtMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TProfile *prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TProfile *prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TProfile *prof_MC_vs_REC_photons_PtMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TProfile *prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      TProfile *prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt_norm[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
      //}    

    // Reconstructed Photons
    
    TH1F *h_photons_all_n;
    TH1F *h_photons_all_eta[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_all_phi[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_all_pt[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_etcone20[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_etcone30[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_etcone40[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];
    TH1F *h_photons_etcone45[AtlEMShower::fgNAuthors][AtlEMShower::fgNIsEM];

    
    h_photons_all_n = (TH1F*)GetHistogram("h_all_photon_n",EvtSelection);
    
    
    for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {
	// ---------------------
	// Distinction by author
	// ---------------------
	
	// Making sure author is not invalid
	if ( 1<<i_author & AtlEMShower::kInvalid ) continue;

	// Retrieving author name
	TString author_name = AtlEMShower::fgAuthorNames[i_author];
	
	
	for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
	    
	    // .........................
	    // Distinction by IsEM-Flag
	    // .........................

	    // Making sure IsEM-Flag is not invalid
	    if ( 1<<i_isem & AtlEMShower::kInvalid ) continue;

	    // Retrieving IsEM-Flag name
	    TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
	    
	    
	    // Shorthand notation "IsEMFlag_Author" for use in histogram names
	    TString isemflag_author = Form("%s_%s",isemflag_name.Data(),author_name.Data());
	    
	    // 
	    // MC comparison histograms
	    // 
	    if(fEvent->IsMC()) {
	      // Eta
	    
	      h_MC_vs_REC_photons_DeltaEtaSigned[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaEtaSigned",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_DeltaEtaSigned_norm[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaEtaSigned_norm",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_Eta[i_author][i_isem]
		= (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_Eta",
					   isemflag_author.Data()),
				      EvtSelection);
	      // Phi
	    
	      h_MC_vs_REC_photons_DeltaPhiSigned[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaPhiSigned",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_DeltaPhiSigned_norm[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaPhiSigned_norm",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_Phi[i_author][i_isem]
		= (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_Phi",
					   isemflag_author.Data()),
				      EvtSelection);
	    
	      // Pt
	    
	      h_MC_vs_REC_photons_DeltaPtSigned[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaPtSigned",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_DeltaPtSigned_norm[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_DeltaPtSigned_norm",
					   isemflag_author.Data()),
				      EvtSelection); 
	      h_MC_vs_REC_photons_Pt[i_author][i_isem]
		= (TH2F*)GetHistogram(Form("h_MC_vs_REC_%s_photons_Pt",
					   isemflag_author.Data()),
				      EvtSelection);
	      // Profile Plots
	    
	      prof_MC_vs_REC_photons_PtMC_vs_DeltaPt[i_author][i_isem]
		= (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_photons_PtMC_vs_DeltaPt",
					       isemflag_author.Data()),
					  EvtSelection);
	      prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt[i_author][i_isem]
		= (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_photons_EtaMC_vs_DeltaPt",
					       isemflag_author.Data()),
					  EvtSelection);
	      prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt[i_author][i_isem]
		= (TProfile*)GetHistogram(Form("prof_MC_vs_REC_%s_photons_PhiMC_vs_DeltaPt",
					       isemflag_author.Data()),
					  EvtSelection);
	      prof_MC_vs_REC_photons_PtMC_vs_DeltaPt_norm[i_author][i_isem]
		= (TProfile*)GetHistogram(
					  Form("prof_MC_vs_REC_%s_photons_PtMC_vs_DeltaPt_norm",
					       isemflag_author.Data()),
					  EvtSelection);
	      prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt_norm[i_author][i_isem]
		= (TProfile*)GetHistogram(
					  Form("prof_MC_vs_REC_%s_photons_EtaMC_vs_DeltaPt_norm",
					       isemflag_author.Data()),
					  EvtSelection);
	      prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt_norm[i_author][i_isem]
		= (TProfile*)GetHistogram(
					  Form("prof_MC_vs_REC_%s_photons_PhiMC_vs_DeltaPt_norm",
					       isemflag_author.Data()),
					  EvtSelection);
	    }
	    
	    // =====================
	    // Reconstructed Photons
	    // =====================
	    
	    h_photons_all_eta[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_eta",
					   isemflag_author.Data()),
				      EvtSelection);
	    h_photons_all_phi[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_phi",
					   isemflag_author.Data()),
				      EvtSelection);
	    h_photons_all_pt[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_pt",
					   isemflag_author.Data()),
				      EvtSelection);
	    
	    h_photons_etcone20[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_EtCone20",
					   isemflag_author.Data()),
				      EvtSelection);
	    h_photons_etcone30[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_EtCone30",
					   isemflag_author.Data()),
				      EvtSelection);
	    h_photons_etcone40[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_EtCone40",
					   isemflag_author.Data()),
				      EvtSelection);
	    h_photons_etcone45[i_author][i_isem]
		= (TH1F*)GetHistogram(Form("h_%s_photon_EtCone45",
					   isemflag_author.Data()),
				      EvtSelection);	  	  	  
	}  
    }
    
    
    // ================================
    // Fill histograms for eta, phi, pt
    // ================================
    
    AtlPhoton *ph = 0;
    
    HepMCParticle *ph_mc = 0;
    
    Float_t eta_mc = 0.;
    Float_t phi_mc = 0.;
    Float_t pt_mc  = 0.;    
    
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t pt  = 0.;

    Int_t n_photons = fEvent->GetN_Photons();

    
    for ( Int_t i = 0; i < n_photons; i++ ) {
	
	ph = (AtlPhoton*)fEvent->GetPhotons()->At(i);
	
	// variables
	pt  = ph->Pt();
	eta = ph->Eta();
	phi = ph->Phi();
	
	      
	//******************************
	// Number of photons by author
	//******************************
	AtlEMShower::EAuthor author = ph->GetAuthor();
	for ( Int_t j = 0; j < AtlEMShower::fgNAuthors; j++ ) {
	    if ( author & (1<<j) ) h_ph_author->AddBinContent(j+1, GetTagEvtWeight());
	}

	//*******************************
	// Number of photons by IsEM-Flag
	//*******************************
	AtlEMShower::EIsEM isem = ph->GetIsEM();
	for ( Int_t j = 0; j < AtlEMShower::fgNIsEM; j++ ) {
	    if ( isem & (1<<j) ) h_ph_isem->AddBinContent(j+1, GetTagEvtWeight());
	}
	
	
	for ( Int_t i_author = 0; i_author < AtlEMShower::fgNAuthors; i_author++ ) {
	    // ---------------------
	    // Distinction by author
	    // ---------------------

	    // retrieving author name
	    TString author_name = AtlEMShower::fgAuthorNames[i_author];
	    
	    // Making sure author is not invalid
	    if ( 1<<i_author & AtlEMShower::kInvalid ) continue;
	    
	    
	    for ( Int_t i_isem = 0; i_isem < AtlEMShower::fgNIsEM; i_isem++ ) {
		
		// .........................
		// Distinction by IsEM-Flag
		// .........................

		// Making sure IsEM flag is not invalid
		if ( 1<<i_isem & AtlEMShower::kInvalid ) continue;
		
		// retrieving IsEM-Flag name
		TString isemflag_name = AtlEMShower::fgIsEMNames[i_isem];
		
		
		// Be sure, author and IsEM flag match current photon
		if ( (author & 1<<i_author) && (isem & 1<<i_isem) ) {
		    
		    
		  // ========================
		  // MC comparison histograms
		  // ========================
		  if(fEvent->IsMC()) {
		    
		    // Find best matching HepMCParticle	
		    ph_mc = (HepMCParticle*)fEvent->FindMatchedMCParticle(ph);
		    if(ph_mc != 0) {
		      phi_mc = ph_mc->Phi();
		      pt_mc  = ph_mc->Pt();
		      eta_mc = ph_mc->Eta();
			
		      // Fill MC vs REC comparison histograms
			
		      // Eta
		      h_MC_vs_REC_photons_Eta[i_author][i_isem]
			-> Fill(eta_mc, eta           , GetTagEvtWeight());
		      h_MC_vs_REC_photons_DeltaEtaSigned[i_author][i_isem]
			-> Fill(eta - eta_mc          , GetTagEvtWeight());
		      if(eta_mc!=0) h_MC_vs_REC_photons_DeltaEtaSigned_norm[i_author][i_isem]
				      -> Fill( (eta - eta_mc)/eta_mc, GetTagEvtWeight());
			
		      // Phi
		      h_MC_vs_REC_photons_Phi[i_author][i_isem]
			-> Fill(phi_mc, phi           , GetTagEvtWeight());
		      h_MC_vs_REC_photons_DeltaPhiSigned[i_author][i_isem]
			-> Fill(phi - phi_mc          , GetTagEvtWeight());	  
		      if(phi_mc!=0) h_MC_vs_REC_photons_DeltaPhiSigned_norm[i_author][i_isem]
				      -> Fill( (phi - phi_mc)/phi_mc, GetTagEvtWeight());
			
		      // Pt
		      h_MC_vs_REC_photons_Pt[i_author][i_isem]
			-> Fill(pt_mc, pt , GetTagEvtWeight());
		      h_MC_vs_REC_photons_DeltaPtSigned[i_author][i_isem]
			-> Fill(pt - pt_mc, GetTagEvtWeight());
			
		      if(pt_mc!=0){ 
			h_MC_vs_REC_photons_DeltaPtSigned_norm[i_author][i_isem]
			  -> Fill( (pt - pt_mc)/pt_mc          , GetTagEvtWeight());
			// Fill Profile plots
			prof_MC_vs_REC_photons_PtMC_vs_DeltaPt_norm[i_author][i_isem]
			  -> Fill( pt_mc,  (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
			prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt_norm[i_author][i_isem]
			  -> Fill( eta_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
			prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt_norm[i_author][i_isem]
			  -> Fill( phi_mc, (pt - pt_mc)/pt_mc  , GetTagEvtWeight());
			prof_MC_vs_REC_photons_PtMC_vs_DeltaPt[i_author][i_isem]
			  -> Fill( pt_mc,  (pt - pt_mc)        , GetTagEvtWeight());
			prof_MC_vs_REC_photons_EtaMC_vs_DeltaPt[i_author][i_isem]
			  -> Fill( eta_mc, (pt - pt_mc)        , GetTagEvtWeight());
			prof_MC_vs_REC_photons_PhiMC_vs_DeltaPt[i_author][i_isem]
			  -> Fill( phi_mc, (pt - pt_mc)        , GetTagEvtWeight());
		      }
		    }
		  }
		    
		  // =====================
		  // Reconstructed Photons
		  // =====================
		    
		  h_photons_all_eta[i_author][i_isem]->Fill(eta, GetTagEvtWeight());
		  h_photons_all_phi[i_author][i_isem]->Fill(phi, GetTagEvtWeight());
		  h_photons_all_pt [i_author][i_isem]->Fill(pt,  GetTagEvtWeight());
		    
		  h_photons_etcone20[i_author][i_isem]
		    ->Fill(ph->GetEtCone20(), GetTagEvtWeight());
		  h_photons_etcone30[i_author][i_isem]
		    ->Fill(ph->GetEtCone30(), GetTagEvtWeight());
		  h_photons_etcone40[i_author][i_isem]
		    ->Fill(ph->GetEtCone40(), GetTagEvtWeight());
		  h_photons_etcone45[i_author][i_isem]
		    ->Fill(ph->GetEtCone45(), GetTagEvtWeight());
		}
	    }
	}
    }
    
    
    // ------------------------------------------------------------------------
    
    // Fill in count of all photons in event
    	
    // ------------------------------------------------------------------------
    
    h_photons_all_n->Fill(n_photons, GetTagEvtWeight());
    
    // ------------------------------------------------------------------------
    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

//____________________________________________________________________

void AtlCtrlPlots::FillAtlFastHistograms(Bool_t EvtSelection) {
    //
    // Fill ATLFAST histograms
    //
    
    // Get histogram pointers
    TH1F *h_el_all_n   = (TH1F*)GetHistogram("h_atlfast_electron_n",
					     EvtSelection);
    TH1F *h_el_all_eta = (TH1F*)GetHistogram("h_atlfast_electron_eta",
					     EvtSelection);
    TH1F *h_el_all_phi = (TH1F*)GetHistogram("h_atlfast_electron_phi",
					     EvtSelection);
    TH1F *h_el_all_pt  = (TH1F*)GetHistogram("h_atlfast_electron_pt",
					     EvtSelection);
    
    TH1F *h_el_plus_n   = (TH1F*)GetHistogram("h_atlfast_eplus_n",
					      EvtSelection);
    TH1F *h_el_plus_eta = (TH1F*)GetHistogram("h_atlfast_eplus_eta",
					      EvtSelection);
    TH1F *h_el_plus_phi = (TH1F*)GetHistogram("h_atlfast_eplus_phi",
					      EvtSelection);
    TH1F *h_el_plus_pt  = (TH1F*)GetHistogram("h_atlfast_eplus_pt",
					      EvtSelection);
    
    TH1F *h_el_minus_n   = (TH1F*)GetHistogram("h_atlfast_eminus_n",
					       EvtSelection);
    TH1F *h_el_minus_eta = (TH1F*)GetHistogram("h_atlfast_eminus_eta",
					       EvtSelection);
    TH1F *h_el_minus_phi = (TH1F*)GetHistogram("h_atlfast_eminus_phi",
					       EvtSelection);
    TH1F *h_el_minus_pt  = (TH1F*)GetHistogram("h_atlfast_eminus_pt",
					       EvtSelection);
    
    // Fill them
    AtlFastElectron *el = 0;
    Int_t n_el_plus = 0; Int_t n_el_minus = 0;
    Float_t el_eta = 0.;
    Float_t el_phi = 0.;
    Float_t el_pt  = 0.;
    for ( Int_t i = 0; i < fEvent->GetN_AtlFastElectrons(); i++ ) {
      el = (AtlFastElectron*)fEvent->GetAtlFastElectrons()->At(i);
      el_phi = el->Phi();
      el_pt  = el->Pt();
      el_eta = el->Eta();
	
      // Both charges
      h_el_all_eta->Fill(el_eta, GetTagEvtWeight());
      h_el_all_phi->Fill(el_phi, GetTagEvtWeight());
      h_el_all_pt ->Fill(el_pt,  GetTagEvtWeight());

      if ( el->IsPositive() ) {
	// Positrons only
	n_el_plus++;
	h_el_plus_eta->Fill(el_eta, GetTagEvtWeight());
	h_el_plus_phi->Fill(el_phi, GetTagEvtWeight());
	h_el_plus_pt ->Fill(el_pt,  GetTagEvtWeight());
      } else {
	// Electrons only
	n_el_minus++;
	h_el_minus_eta->Fill(el_eta, GetTagEvtWeight());
	h_el_minus_phi->Fill(el_phi, GetTagEvtWeight());
	h_el_minus_pt ->Fill(el_pt,  GetTagEvtWeight());
      }
    }
    h_el_all_n  ->Fill(fEvent->GetN_AtlFastElectrons(), GetTagEvtWeight());
    h_el_plus_n ->Fill(n_el_plus,  GetTagEvtWeight());
    h_el_minus_n->Fill(n_el_minus, GetTagEvtWeight());

    
    //
    // Fill photon histograms
    //
    
    // Get histogram pointers
    TH1F *h_ph_n   = (TH1F*)GetHistogram("h_atlfast_photon_n",   EvtSelection);
    TH1F *h_ph_eta = (TH1F*)GetHistogram("h_atlfast_photon_eta", EvtSelection);
    TH1F *h_ph_phi = (TH1F*)GetHistogram("h_atlfast_photon_phi", EvtSelection);
    TH1F *h_ph_pt  = (TH1F*)GetHistogram("h_atlfast_photon_pt",  EvtSelection);
    
    // Fill them
    AtlFastPhoton *gamma = 0;
    Float_t gamma_pt  = 0.;
    Float_t gamma_eta = 0.;
    for ( Int_t i = 0; i < fEvent->GetN_AtlFastPhotons(); i++ ) {
      gamma = (AtlFastPhoton*)fEvent->GetAtlFastPhotons()->At(i);
      gamma_pt = gamma->Pt();
      gamma_eta = gamma->Eta();
	
      h_ph_eta->Fill(gamma->Eta(), GetTagEvtWeight());
      h_ph_phi->Fill(gamma->Phi(), GetTagEvtWeight());
      h_ph_pt ->Fill(gamma->Pt(),  GetTagEvtWeight());
      h_ph_eta->Fill(gamma_eta,    GetTagEvtWeight());
      h_ph_phi->Fill(gamma->Phi(), GetTagEvtWeight());
      h_ph_pt ->Fill(gamma_pt,     GetTagEvtWeight());
    }
    h_ph_n  ->Fill(fEvent->GetN_AtlFastPhotons(), GetTagEvtWeight());
    
    
    //
    // Fill muon histograms
    //
    
    // Get histogram pointers
    TH1F *h_mu_all_n   = (TH1F*)GetHistogram("h_atlfast_muon_n",
					     EvtSelection);
    TH1F *h_mu_all_eta = (TH1F*)GetHistogram("h_atlfast_muon_eta",
					     EvtSelection);
    TH1F *h_mu_all_phi = (TH1F*)GetHistogram("h_atlfast_muon_phi",
					     EvtSelection);
    TH1F *h_mu_all_pt  = (TH1F*)GetHistogram("h_atlfast_muon_pt",
					     EvtSelection);

    TH1F *h_mu_plus_n   = (TH1F*)GetHistogram("h_atlfast_muplus_n",
					      EvtSelection);
    TH1F *h_mu_plus_eta = (TH1F*)GetHistogram("h_atlfast_muplus_eta",
					      EvtSelection);
    TH1F *h_mu_plus_phi = (TH1F*)GetHistogram("h_atlfast_muplus_phi",
					      EvtSelection);
    TH1F *h_mu_plus_pt  = (TH1F*)GetHistogram("h_atlfast_muplus_pt",
					      EvtSelection);

    TH1F *h_mu_minus_n   = (TH1F*)GetHistogram("h_atlfast_muminus_n",
					       EvtSelection);
    TH1F *h_mu_minus_eta = (TH1F*)GetHistogram("h_atlfast_muminus_eta",
					       EvtSelection);
    TH1F *h_mu_minus_phi = (TH1F*)GetHistogram("h_atlfast_muminus_phi",
					       EvtSelection);
    TH1F *h_mu_minus_pt  = (TH1F*)GetHistogram("h_atlfast_muminus_pt",
					       EvtSelection);

    // Fill them
    AtlFastMuon *mu = 0;
    Int_t n_mu_plus = 0; Int_t n_mu_minus = 0;
    Float_t mu_eta = 0.;
    Float_t mu_phi = 0.;
    Float_t mu_pt  = 0.;
    for ( Int_t i = 0; i < fEvent->GetN_AtlFastMuons(); i++ ) {
      mu = (AtlFastMuon*)fEvent->GetAtlFastMuons()->At(i);
      mu_phi = mu->Phi();
      mu_pt  = mu->Pt();
      mu_eta = mu->Eta();
	
      // Both charges
      h_mu_all_eta->Fill(mu_eta, GetTagEvtWeight());
      h_mu_all_phi->Fill(mu_phi, GetTagEvtWeight());
      h_mu_all_pt ->Fill(mu_pt,  GetTagEvtWeight());

      if ( mu->IsPositive() ) {
	// Mu+ only
	n_mu_plus++;
	h_mu_plus_eta->Fill(mu_eta, GetTagEvtWeight());
	h_mu_plus_phi->Fill(mu_phi, GetTagEvtWeight());
	h_mu_plus_pt ->Fill(mu_pt,  GetTagEvtWeight());
      } else {
	// Mu- only
	n_mu_minus++;
	h_mu_minus_eta->Fill(mu_eta, GetTagEvtWeight());
	h_mu_minus_phi->Fill(mu_phi, GetTagEvtWeight());
	h_mu_minus_pt ->Fill(mu_pt,  GetTagEvtWeight());
      }
    }
    h_mu_all_n  ->Fill(fEvent->GetN_AtlFastMuons(), GetTagEvtWeight());
    h_mu_plus_n ->Fill(n_mu_plus,  GetTagEvtWeight());
    h_mu_minus_n->Fill(n_mu_minus, GetTagEvtWeight());

    
    //
    // Fill jet histograms
    //
    TH1F *h_jet_n      = (TH1F*)GetHistogram("h_atlfast_njets",
					     EvtSelection);
    TH2F *h_jet_etaphi = (TH2F*)GetHistogram("h_atlfast_etaphi",
					     EvtSelection);
    TH1F *h_jet_Et     = (TH1F*)GetHistogram("h_atlfast_Et",
					     EvtSelection);
    
    // No. of jets
    h_jet_n->Fill(fEvent->GetN_AtlFastJets());
    
    // Loop over all jets
    Float_t jet_Et  = 0.;
    Float_t jet_eta = 0.;
    for ( Int_t i = 0; i < fEvent->GetN_AtlFastJets(); i++ ) {
      AtlFastJet *jet = (AtlFastJet*)fEvent->GetAtlFastJets()->At(i);
      jet_Et  = jet->Et();
      if ( jet_Et != 0. ) {
	jet_eta = jet->Eta();
      } else {
	jet_eta = ( jet->Pz() > 0. ) ? 10e10 : -10e10;
      }
	
      // Eta-phi
      h_jet_etaphi->Fill(jet_eta, jet->Phi(), GetTagEvtWeight());
	
      // Et
      h_jet_Et->Fill(jet_Et, GetTagEvtWeight());
    }
}
