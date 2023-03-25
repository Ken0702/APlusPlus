//____________________________________________________________________
//
// A++ KinFitter Interface
//
// This Tool is supposed to be the base of analysis programs which use the
// KinFitter package.
// It also involves a functionality to create efficiency and purity graphs
// (also see comment in header file).
// Values are plotted versus the variable fX_TruthHistograms which is set
// via SetX_TruthHistograms(Double_t X). Since efficiency  refers to MC,
// fX_TruthHistograms must be a variable defined on MC level (rather than on
// analysis level like chi-square). It could be for instance the invariant
// mass of a mother particle, the opening angle of daughters etc.
//
// X-Axis title and binning of the histograms (except those refering to
// chi-square probability) can be set simultaneously via SetTruthMatchXTitle(...)
// and SetTruthMatchBinning(...).
// In the analysis, do not forget to call AtlKinFitterTool::BookHistograms().
//
// The 'True' histograms are filled via AtlKinFitterTool::FillHistograms("Truth"),
// 'Reco' histograms are filled via AtlKinFitterTool::FillHistograms("Signal") and
// 'TrueReco' histograms are filled also via AtlKinFitterTool::FillHistograms("Signal")
// if you have set fTrueReco to kTRUE via SetTrueReco(...) preceeding the call.
//
// The histograms are used to plot efficiencies and purities which is done by
// AtlKinFitterTool::Terminate() using the BayesDivide() functionality.
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#endif
#include <TLorentzVector.h>
#include <TFile.h>
#include <TH2F.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TRandom3.h>

#ifndef __CINT__
ClassImp(AtlKinFitterTool);
#endif

using namespace std;

const char* AtlKinFitterTool::fgStatusNames[] = {
    "CONVERGED", "NOT CONVERGED"};
//    "ABORTED", "NOT DEFINED"};

//____________________________________________________________________

AtlKinFitterTool::AtlKinFitterTool(const char* name, const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fMode = kKinFit;
    fKinFitter = new TKinFitter;
    fChi2=0.;
    fNDoF=0;
    fAbundanceTrue = 0;
    fTrueReco = kFALSE;
    fHasLoadedCovMatrices = kFALSE;
    fUseLeptonCovRand = kFALSE;
    randnr = new TRandom3();		//is the used randomizer
    fFileCovMatrices = 0;

    // Set PDG values for W and top masses and widths
    HepDatabasePDG *pdg = HepDatabasePDG::Instance();
    fW_Mass    = pdg->GetParticle("W+")->Mass();
    fW_Width   = pdg->GetParticle("W+")->Width();
    fB_Mass    = pdg->GetParticle("b")->Mass();
    fTop_Mass  = pdg->GetParticle("t")->Mass();
    fTop_Width = pdg->GetParticle("t")->Width();
}

//____________________________________________________________________

AtlKinFitterTool::~AtlKinFitterTool() {
    //
    // Default destructor
    //
    delete fFileCovMatrices;
    delete fKinFitter;
    delete randnr;

}

//____________________________________________________________________

void AtlKinFitterTool::SetCutDefaults() {
    //
    // Setting default cut values
    //

}

//____________________________________________________________________

void AtlKinFitterTool::BookHistograms() {
    //
    // Book KinFitter histograms
    //

    gDirectory->cd();
    gDirectory->mkdir("KinFitterTool");
    gDirectory->cd("KinFitterTool");

    //
    // Book CutFlow Histograms
    //

    // First cut-flow
    fHistCutFlowWeighted = new TH1F("h_CutFlowWeighted", "effects of different cuts, weighted",
				    1000, 0., 1000.);
    fHistCutFlowWeighted->GetYaxis()->SetTitle("Number of Entries");
    fHistCutFlowUnweighted = new TH1F("h_CutFlowUnweighted", "effects of different cuts, unweighted",
				      1000, 0., 1000.);
    fHistCutFlowUnweighted->GetYaxis()->SetTitle("Number of Entries");

    // Second cut-flow
    fHistCutFlow2Weighted = new TH1F("h_CutFlow2Weighted", "effects of different cuts, weighted (2nd cut-flow)",
				    1000, 0., 1000.);
    fHistCutFlow2Weighted->GetYaxis()->SetTitle("Number of Entries");
    fHistCutFlow2Unweighted = new TH1F("h_CutFlow2Unweighted", "effects of different cuts, unweighted (2nd cut-flow)",
				      1000, 0., 1000.);
    fHistCutFlow2Unweighted->GetYaxis()->SetTitle("Number of Entries");

    gDirectory->mkdir("Fitter", "Histograms of Kinematic Fit");
    gDirectory->cd("Fitter");
    gDirectory->mkdir("Signal","Histograms of Kinematic Fit, signal events");
    gDirectory->mkdir("Bkg","Histograms of Kinematic Fit, background events");
    gDirectory->mkdir("TruthMatching","Histograms of Kinematic Fit, Truth vs. Reconstr. Comparison");
    gDirectory->mkdir("Performance", "Characteristics of fitting");
    gDirectory->mkdir("LeptonCovMatrix","Lepton Covariance Matrix elements");

    gDirectory->cd("Signal");

    // Book chi2 and -probability histograms
    fHist_KinFit_chi2   = new TH1F("h_chi2", "#chi^2 value of Kinematic Fit",
					  75,0., 20.);
    fHist_KinFit_chi2->GetXaxis()->SetTitle("chi2");
    fHist_KinFit_chi2->GetYaxis()->SetTitle("#Events");

    fHist_KinFit_prob   = new TH1F("h_prob", "Probability value of Kinematic Fit",
					  30, 0., 1.);
    fHist_KinFit_prob->GetXaxis()->SetTitle("Probability");
    fHist_KinFit_prob->GetYaxis()->SetTitle("Events");

    gDirectory->cd("../");

    gDirectory->cd("Bkg");

    fHist_KinFitBkg_chi2   = new TH1F("h_Bkg_chi2", "#chi^2 value of Kinematic Fit, Bkg",
					  75, 0., 100.);
    fHist_KinFitBkg_chi2->GetXaxis()->SetTitle("#chi2");
    fHist_KinFitBkg_chi2->GetYaxis()->SetTitle("Events");

    fHist_KinFitBkg_prob   = new TH1F("h_Bkg_prob", "Probability value of Kinematic Fit, Bkg",
					  30, 0., 1.);
    fHist_KinFitBkg_prob->GetXaxis()->SetTitle("#chi^2 Probability");
    fHist_KinFitBkg_prob->GetYaxis()->SetTitle("Events");

    gDirectory->cd("../");

    gDirectory->cd("TruthMatching");

    // Book truth-matching histograms
    fHist_KinFit_True   = new TH1F("h_True", "Number of true MC Events",
					  50, 50., 130.);
    fHist_KinFit_True->GetYaxis()->SetTitle("True Events");

    fHist_KinFit_TrueReco   = new TH1F("h_TrueReco", "Number of reconstructed true MC Events",
					  50, 50., 130.);
    fHist_KinFit_TrueReco->GetYaxis()->SetTitle("# TrueReco Events");

    fHist_KinFit_TrueRecoProb   = new TH1F("h_TrueRecoProb", "Number of reconstructed true MC Events vs. Probability",
					  30, 0., 1.);
    fHist_KinFit_TrueRecoProb->GetXaxis()->SetTitle("P(#chi^2)");
    fHist_KinFit_TrueRecoProb->GetYaxis()->SetTitle("# TrueReco Events");

    // ------------
    fHist_KinFit_NTrueProb   = new TH1F("h_NTrue", "Number of true MC particles",
					30, 0., 1.);
    fHist_KinFit_TrueRecoProbGreaterP = new TH1F("h_TrueRecoProbGreaterP",
					       "Number of reconstructed true MC Events vs. Probability greater than p*",
					       30, 0., 1.);
    fHist_KinFit_TrueRecoProbGreaterP->GetXaxis()->SetTitle("P(#chi^{2}) < p*");
    fHist_KinFit_TrueRecoProbGreaterP->GetYaxis()->SetTitle("# TrueReco Events");
    fHist_KinFit_RecoProbGreaterP = new TH1F("h_RecoProbGreaterP",
					       "Number of reconstructed MC Events vs. Probability greater than p*",
					       30, 0., 1.);
    fHist_KinFit_RecoProbGreaterP->GetXaxis()->SetTitle("P(#chi^{2}) < p*");
    fHist_KinFit_RecoProbGreaterP->GetYaxis()->SetTitle("# Reco Events");
    // -----------

    fHist_KinFit_Reco   = new TH1F("h_Reco", "Number of reconstructed Events",
				   50, 50., 130.);
    fHist_KinFit_Reco->GetYaxis()->SetTitle("# Reco Events");

    fHist_KinFit_Efficiency   = new TH1F("h_Eff", "Efficiency", 50, 50., 130.);
    fHist_KinFit_Efficiency->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{True}}");

    fHist_KinFit_Purity   = new TH1F("h_Purity", "Purity", 50, 50., 130.);
    fHist_KinFit_Purity->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{Reco}}");

    fHist_KinFit_PurityProb   = new TH1F("h_PurityProb", "Purity vs. Probability", 50, 0., 1.);
    fHist_KinFit_PurityProb->GetXaxis()->SetTitle("#chi^2 Probability");
    fHist_KinFit_PurityProb->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{Reco}}");

    fGraph_KinFit_Efficiency             = new TGraphAsymmErrors();
    fGraph_KinFit_Purity                 = new TGraphAsymmErrors();

    fGraph_KinFit_PurityProb             = new TGraphAsymmErrors();
    fGraph_KinFit_EfficiencyProb         = new TGraphAsymmErrors();
    fGraph_KinFit_PurityProbGreaterP     = new TGraphAsymmErrors();
    fGraph_KinFit_EfficiencyProbGreaterP = new TGraphAsymmErrors();

    gDirectory->cd("../");

    gDirectory->cd("Performance");

    //
    // Book KinFitter performance histograms
    //

    // KinFitter status counts for fits
    fHist_KinFit_KinFitterStatus = new TH1F("h_KinFitterStatus",
				    "Status of KinFitter",
				    2,0,2);
    fHist_KinFit_KinFitterStatus->SetXTitle("Status of Fit");
    fHist_KinFit_KinFitterStatus->SetYTitle("No. of Fits");
    TAxis *axis = fHist_KinFit_KinFitterStatus->GetXaxis();
    for ( Int_t k = 0; k < 2; k++ ) {
	axis->SetBinLabel(k+1, AtlKinFitterTool::fgStatusNames[k]);
    }

    // Number of iterations (all fitting procedures)
    fHist_KinFit_NbIterAll = new TH1F("h_NbIterAll",
			      "No. of Iterations for ALL fitting procedures",
			      50,0,1200);
    fHist_KinFit_NbIterAll->SetXTitle("No. of Iterations");
    fHist_KinFit_NbIterAll->SetYTitle("No. of fitting procedures");

    // Number of iterations (only fitting procedures of chosen candidates)
    fHist_KinFit_NbIterCand = new TH1F("h_NbIterCand",
			       "No. of Iterations for chosen CANDIDATE fitting procedures",
			       50,0,1200);
    fHist_KinFit_NbIterCand->SetXTitle("No. of Iterations");
    fHist_KinFit_NbIterCand->SetYTitle("No. of fitting procedures");

    // Number of fits that DID converge (after pre-selection)
    fHist_KinFit_NbFitsConverged = new TH1F("h_NbFitsConv",
					    "No. of Fitting Procedures converged per event (after pre-selection)",
					    4,-0.5,3.5);

    // Number of fits that DID NOT converge (after pre-selection)
    fHist_KinFit_NbFitsNotConverged = new TH1F("h_NbFitsNotConv",
			       "No. of Fitting Procedures not converged per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsNotConverged->SetXTitle("No. of Fits_{not conv.} / Evt");
    fHist_KinFit_NbFitsNotConverged->SetYTitle("No. of Events");

    // Number of fits that failed numerically (after pre-selection)
    fHist_KinFit_NbFitsFailedNumerically = new TH1F("h_NbFitsFailNumeric",
			       "No. of Fitting Procedures failing numerically per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsFailedNumerically->SetXTitle("No. of Fits_{numeric fail} / Evt");
    fHist_KinFit_NbFitsFailedNumerically->SetYTitle("No. of Events");

    // Final value of constraint (only fitting procedures of chosen candidates)
    fHist_KinFit_ConsValue  = new TH1F("h_ConsValue", "Value of constraints at the end of the fit (candidate events)",
					  50, 0., 1.e-6);
    fHist_KinFit_ConsValue->GetXaxis()->SetTitle("Value of constraints");
    fHist_KinFit_ConsValue->GetYaxis()->SetTitle("Number of events");

    gDirectory->cd("../");
    gDirectory->cd("LeptonCovMatrix");

    gDirectory->mkdir("Electron","Electron Covariance Matrix elements");;
    gDirectory->cd("Electron");

    fhist_CovMatrix_el_11 = new TH1F("h_CovMatrix_el_11","Electron CovMatrix Element 11",40,0,4);
    fhist_CovMatrix_el_11->SetXTitle("CovMatrix element 11");
    fhist_CovMatrix_el_11->SetYTitle("Entries");

    fhist_CovMatrix_el_12 = new TH1F("h_CovMatrix_el_12","Electron CovMatrix Element 12",40,-6e-5,6e-5);
    fhist_CovMatrix_el_12->SetXTitle("CovMatrix element 12");
    fhist_CovMatrix_el_12->SetYTitle("Entries");

    fhist_CovMatrix_el_21 = new TH1F("h_CovMatrix_el_21","Electron CovMatrix Element 21",40,-6e-5,6e-5);
    fhist_CovMatrix_el_21->SetXTitle("CovMatrix element 21");
    fhist_CovMatrix_el_21->SetYTitle("Entries");

    fhist_CovMatrix_el_13 = new TH1F("h_CovMatrix_el_13","Electron CovMatrix Element 13",40,-4e-4,4e-4);
    fhist_CovMatrix_el_13->SetXTitle("CovMatrix element 13");
    fhist_CovMatrix_el_13->SetYTitle("Entries");

    fhist_CovMatrix_el_31 = new TH1F("h_CovMatrix_el_31","Electron CovMatrix Element 31",40,-4e-4,4e-4);
    fhist_CovMatrix_el_31->SetXTitle("CovMatrix element 31");
    fhist_CovMatrix_el_31->SetYTitle("Entries");

    fhist_CovMatrix_el_22 = new TH1F("h_CovMatrix_el_22","Electron CovMatrix Element 22",40,0,1.2e-6);
    fhist_CovMatrix_el_22->SetXTitle("CovMatrix element 22");
    fhist_CovMatrix_el_22->SetYTitle("Entries");

    fhist_CovMatrix_el_23 = new TH1F("h_CovMatrix_el_23","Electron CovMatrix Element 23",40,-8e-9,12e-9);
    fhist_CovMatrix_el_23->SetXTitle("CovMatrix element 23");
    fhist_CovMatrix_el_23->SetYTitle("Entries");

    fhist_CovMatrix_el_32 = new TH1F("h_CovMatrix_el_32","Electron CovMatrix Element 32",40,-8e-9,12e-9);
    fhist_CovMatrix_el_32->SetXTitle("CovMatrix element 32");
    fhist_CovMatrix_el_32->SetYTitle("Entries");

    fhist_CovMatrix_el_33 = new TH1F("h_CovMatrix_el_33","Electron CovMatrix Element 33",40,0,1.5e-7);
    fhist_CovMatrix_el_33->SetXTitle("CovMatrix element 33");
    fhist_CovMatrix_el_33->SetYTitle("Entries");

    gDirectory->cd("../");

    gDirectory->mkdir("Muon","Muon Covariance Matrix elements");
    gDirectory->cd("Muon");

    fhist_CovMatrix_mu_11 = new TH1F("h_CovMatrix_mu_11","Muon CovMatrix Element 11",40,0,4);
    fhist_CovMatrix_mu_11->SetXTitle("CovMatrix element 11");
    fhist_CovMatrix_mu_11->SetYTitle("Entries");

    fhist_CovMatrix_mu_12 = new TH1F("h_CovMatrix_mu_12","Muon CovMatrix Element 12",40,-6e-5,6e-5);
    fhist_CovMatrix_mu_12->SetXTitle("CovMatrix element 12");
    fhist_CovMatrix_mu_12->SetYTitle("Entries");

    fhist_CovMatrix_mu_21 = new TH1F("h_CovMatrix_mu_21","Muon CovMatrix Element 21",40,-6e-5,6e-5);
    fhist_CovMatrix_mu_21->SetXTitle("CovMatrix element 21");
    fhist_CovMatrix_mu_21->SetYTitle("Entries");

    fhist_CovMatrix_mu_13 = new TH1F("h_CovMatrix_mu_13","Muon CovMatrix Element 13",40,-2e-4,2e-4);
    fhist_CovMatrix_mu_13->SetXTitle("CovMatrix element 13");
    fhist_CovMatrix_mu_13->SetYTitle("Entries");

    fhist_CovMatrix_mu_31 = new TH1F("h_CovMatrix_mu_31","Muon CovMatrix Element 31",40,-2e-4,2e-4);
    fhist_CovMatrix_mu_31->SetXTitle("CovMatrix element 31");
    fhist_CovMatrix_mu_31->SetYTitle("Entries");

    fhist_CovMatrix_mu_22 = new TH1F("h_CovMatrix_mu_22","Muon CovMatrix Element 22",40,0,5e-7);
    fhist_CovMatrix_mu_22->SetXTitle("CovMatrix element 22");
    fhist_CovMatrix_mu_22->SetYTitle("Entries");

    fhist_CovMatrix_mu_23 = new TH1F("h_CovMatrix_mu_23","Muon CovMatrix Element 23",40,-6e-9,8e-9);
    fhist_CovMatrix_mu_23->SetXTitle("CovMatrix element 23");
    fhist_CovMatrix_mu_23->SetYTitle("Entries");

    fhist_CovMatrix_mu_32 = new TH1F("h_CovMatrix_mu_32","Muon CovMatrix Element 32",40,-6e-9,8e-9);
    fhist_CovMatrix_mu_32->SetXTitle("CovMatrix element 32");
    fhist_CovMatrix_mu_32->SetYTitle("Entries");

    fhist_CovMatrix_mu_33 = new TH1F("h_CovMatrix_mu_33","Muon CovMatrix Element 33",40,0,1e-7);
    fhist_CovMatrix_mu_33->SetXTitle("CovMatrix element 33");
    fhist_CovMatrix_mu_33->SetYTitle("Entries");

    gDirectory->cd("../");

    gDirectory->cd("../../..");

}

//____________________________________________________________________

void AtlKinFitterTool::FillHistograms(Option_t *HistMode) {
    //
    // Fill histograms of Kinematic Fit
    // HistMode = 1 : fill signal histograms
    // HistMode = 0 : fill bkg histograms
    //

    Double_t prob = TMath::Prob(fChi2,fNDoF); // Probability of a chi-square greater than fChi2

    TString mode = HistMode;

    if ( mode == "Signal" ) {
	fHist_KinFit_chi2->Fill(fChi2, GetPreTagEvtWeight());
	fHist_KinFit_prob->Fill(prob, GetPreTagEvtWeight());
	for(Int_t j = 1; j <= 30; j++)
	    if( (Float_t)j/30 < prob )
		fHist_KinFit_RecoProbGreaterP->AddBinContent(j, GetPreTagEvtWeight());
	// Fill performance histograms for Signal events
	// Histogram for number of iterations is not weighted
// *** fHist_KinFit_NbIter->Fill( fNbIter );  ***
	fHist_KinFit_ConsValue->Fill(fConsValue, GetPreTagEvtWeight());

	if ( fTrueReco ) {
	    fHist_KinFit_TrueRecoProb->Fill(prob, GetPreTagEvtWeight());
	    for(Int_t j = 1; j <= 30; j++)// where does the 30 originate? Hard-coded!
		if( (Float_t)j/30 < prob )
		    fHist_KinFit_TrueRecoProbGreaterP->AddBinContent(j, GetPreTagEvtWeight());
	    fHist_KinFit_TrueReco->Fill(fX_TruthHistograms, GetPreTagEvtWeight());
	}
	fHist_KinFit_Reco->Fill(fX_TruthHistograms, GetPreTagEvtWeight());
    } else if ( mode == "Bkg" ) {
	fHist_KinFitBkg_chi2->Fill(fChi2, GetPreTagEvtWeight());
	fHist_KinFitBkg_prob->Fill(prob, GetPreTagEvtWeight());
    } else if ( mode == "Truth" ) {
	fHist_KinFit_True->Fill(fX_TruthHistograms, GetPreTagEvtWeight());
    }
    else {
       Error("AtlKinfitterTool::FillHistograms",
	     "Invalid mode given. Abort!");
	gSystem->Abort(0);
    }
}

//____________________________________________________________________

void AtlKinFitterTool::FillLeptonCovMatrixHistograms(HepParticle* lepton,TMatrixD& CovLepton){
    //fill covariance matrix elements in histograms
    if ( lepton->IsElectron() ) {
	//set electron covmatrices
	fhist_CovMatrix_el_11 -> Fill(CovLepton[0][0],GetPreTagEvtWeight());
	fhist_CovMatrix_el_12 -> Fill(CovLepton[0][1],GetPreTagEvtWeight());
	fhist_CovMatrix_el_21 -> Fill(CovLepton[1][0],GetPreTagEvtWeight());
	fhist_CovMatrix_el_22 -> Fill(CovLepton[1][1],GetPreTagEvtWeight());
	fhist_CovMatrix_el_13 -> Fill(CovLepton[0][2],GetPreTagEvtWeight());
	fhist_CovMatrix_el_31 -> Fill(CovLepton[2][0],GetPreTagEvtWeight());
	fhist_CovMatrix_el_23 -> Fill(CovLepton[1][2],GetPreTagEvtWeight());
	fhist_CovMatrix_el_32 -> Fill(CovLepton[2][1],GetPreTagEvtWeight());
	fhist_CovMatrix_el_33 -> Fill(CovLepton[2][2],GetPreTagEvtWeight());
    } else if ( lepton->IsMuon() ) {
	fhist_CovMatrix_mu_11 -> Fill(CovLepton[0][0],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_12 -> Fill(CovLepton[0][1],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_21 -> Fill(CovLepton[1][0],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_22 -> Fill(CovLepton[1][1],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_13 -> Fill(CovLepton[0][2],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_31 -> Fill(CovLepton[2][0],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_23 -> Fill(CovLepton[1][2],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_32 -> Fill(CovLepton[2][1],GetPreTagEvtWeight());
	fhist_CovMatrix_mu_33 -> Fill(CovLepton[2][2],GetPreTagEvtWeight());

    } else {
	//not implemented for taus
	Info("FillLeptonCovMatrixHistograms","Not yet implemented for tauons!");
	gSystem->Abort();
    }
}
//____________________________________________________________________

void AtlKinFitterTool::SetCovMatrix(TMatrixD& cov, HepJet *jet,
				    Double_t sigma_pt, Double_t sigma_eta,
				    Double_t sigma_phi, Double_t cov_pt_eta,
				    Double_t cov_pt_phi, Double_t cov_eta_phi) {
    //
    // Fill the given (empty) covariance matrix with the given
    // correlations. These are supposed to be already multiplied by
    // the corresponding absolute values from the jet.
    //
    // Note that the given sigmas are supposed to be the RMS and not
    // the variance (i.e. not squared!)
    //
    cov[0][0] = sigma_pt;
    cov[1][1] = sigma_eta;
    cov[2][2] = sigma_phi;

    cov[0][0] *= cov[0][0];
    cov[1][1] *= cov[1][1];
    cov[2][2] *= cov[2][2];

    cov[0][1] = cov_pt_eta;
    cov[0][2] = cov_pt_phi;
    cov[1][2] = cov_eta_phi;

    cov[1][0] = cov[0][1];
    cov[2][0] = cov[0][2];
    cov[2][1] = cov[1][2];
}

//____________________________________________________________________

void AtlKinFitterTool::SetCovMatrix(TMatrixD& cov, HepParticle *nu,
				    Double_t SigmaPt_MET, Double_t SigmaPhi_MET,
				    Double_t Cov_Pt_Phi_MET) {
    //
    // Fill the given (empty) covariance matrix for the given neutrino
    // (HepParticle object) with the given correlations. These are supposed to
    // be already mulitplied by the corresponding absolute values from missing Et
    // The (unknown) Eta-related entries of the matrix are arbitrary (set to 1 here).
    //
    // Note that the given sigmas are supposed to be the RMS and not
    // the variance (i.e. not squared!).
    //
    cov[0][0] = SigmaPt_MET;
    cov[1][1] = 1.;
    cov[2][2] = SigmaPhi_MET;

    cov[0][0] *= cov[0][0];
    cov[1][1] *= cov[1][1];
    cov[2][2] *= cov[2][2];

    cov[0][1] = 1.;
    cov[0][2] = Cov_Pt_Phi_MET;
    cov[1][2] = 1.;

    cov[1][0] = cov[0][1];
    cov[2][0] = cov[0][2];
    cov[2][1] = cov[1][2];
}

//____________________________________________________________________

void AtlKinFitterTool::SetCovMatrix(TMatrixD& cov, HepJet *jet) {
    //
    // Fill the given (empty) covariance matrix for the given jet.
    // The covariances are taken from the covariance matrix histograms
    // containing these values dependent on Pt and eta of the jet
    //
    if ( !fHasLoadedCovMatrices ) LoadCovMatrices();
    SetCovMatrix(cov, jet,
		 fHistCovMatJets_PtPt->GetBinContent(fHistCovMatJets_PtPt
						     ->FindBin(jet->Pt(), jet->Eta())),
		 fHistCovMatJets_EtaEta->GetBinContent(fHistCovMatJets_EtaEta
						       ->FindBin(jet->Pt(), jet->Eta())),
		 fHistCovMatJets_PhiPhi->GetBinContent(fHistCovMatJets_PhiPhi
						       ->FindBin(jet->Pt(), jet->Eta())),
		 fHistCovMatJets_PtEta ->GetBinContent(fHistCovMatJets_PtEta
						       ->FindBin(jet->Pt(), jet->Eta())),
		 fHistCovMatJets_PtPhi ->GetBinContent(fHistCovMatJets_PtPhi
						       ->FindBin(jet->Pt(), jet->Eta())),
		 fHistCovMatJets_EtaPhi->GetBinContent(fHistCovMatJets_EtaPhi
						       ->FindBin(jet->Pt(), jet->Eta())));
}

//____________________________________________________________________

void AtlKinFitterTool::SetCovMatrix(TMatrixD& cov, HepParticle *nu) {
    //
    // Fill the given (empty) covariance matrix for the given neutrino
    // (HepParticle object).
    // The covariances are taken from the covariance matrix histograms
    // containing these values dependent on Pt and eta of the missing
    // Et
    //
    if ( !fHasLoadedCovMatrices ) LoadCovMatrices();
    SetCovMatrix(cov, nu,
		 fHistCovMatEtmiss_EtEt  ->GetBinContent(fHistCovMatEtmiss_EtEt
							 ->FindBin(nu->Pt(), nu->Eta())),
		 fHistCovMatEtmiss_PhiPhi->GetBinContent(fHistCovMatEtmiss_PhiPhi
							 ->FindBin(nu->Pt(), nu->Eta())),
		 fHistCovMatEtmiss_EtPhi ->GetBinContent(fHistCovMatEtmiss_EtPhi
							 ->FindBin(nu->Pt(), nu->Eta())));
}

//____________________________________________________________________

void AtlKinFitterTool::GetLeptonCovMatRand(HepParticle* lepton,TMatrixD& CovLepton) {
    //
    // Computes randomly the covariance matrix elements of electrons and muons
    // according to fitted distributions from data (Gauss, Landau)
    //
    if ( lepton->IsElectron() ) {
	//set electron covmatrices
	CovLepton[0][0] = randnr -> Landau(0.265,0.2059);
	CovLepton[0][1] = randnr -> Gaus(-1.603e-7,1.17e-5);
	CovLepton[1][0] = CovLepton[0][1];
	CovLepton[0][2] = randnr -> Gaus(-8.295e-6,7.044e-5);
	while((CovLepton[0][2]>=-0.02e-3) && (CovLepton[0][2]<=0.02e-3)){   //exclude the center range
	    CovLepton[0][2] = randnr -> Gaus(-8.295e-6,7.044e-5);
	}
	CovLepton[2][0] = CovLepton[0][2];
	CovLepton[1][1] = randnr -> Landau(1.864e-7,5.954e-8);
	CovLepton[1][2] = randnr -> Gaus(8.423e-10,3.064e-9);
	CovLepton[2][1] = CovLepton[1][2];
	CovLepton[2][2] = randnr-> Landau(1.695e-8,6.151e-9);
    } else if ( lepton->IsMuon() ) {
	CovLepton[0][0] = randnr -> Landau(0.3224,0.1611);
	CovLepton[0][1] = randnr -> Gaus(-1.103e-6,1.159e-5);
	CovLepton[1][0] = CovLepton[0][1];
	CovLepton[0][2] = randnr -> Gaus(-7.914e-6,5.128e-5);
	while((CovLepton[0][2]>=-0.01e-3) && (CovLepton[0][2]<=0.01e-3)){   //exclude the center range
	    CovLepton[0][2] = randnr -> Gaus(-7.914e-6,5.128e-5);
	}
	CovLepton[2][0] = CovLepton[0][2];
	CovLepton[1][1] = randnr -> Landau(1.155e-7,3.413e-8);
	CovLepton[1][2] = randnr -> Gaus(8.962e-10,2.142e-9);
	CovLepton[2][1] = CovLepton[1][2];
	CovLepton[2][2] = randnr-> Landau(1.67e-8,6.33e-9);
    } else {
	// not implemented for tau
	Info("GetLeptonCovMatRand","Not yet implemented for tauons!");
	gSystem->Abort();
    }
}

//____________________________________________________________________

void AtlKinFitterTool::LoadCovMatrices() {
    //
    // Load covariance matrices
    //
    if ( fHasLoadedCovMatrices ) {
	cout << endl;
	Info("LoadCovMatrices", "Histograms already loaded");
	return;
    }

    TDirectory *savdir = gDirectory;

    // Open file (re-open in case of an already open file)
    if ( fFileCovMatrices != 0 ) delete fFileCovMatrices;
    fFileCovMatrices = new TFile("$LIBATLASANALYSIS/dbase/CovMatrices/CovarianceMatricesJetsEtMiss.root",
				 "READ");
    cout << endl;
    Info("LoadCovMatrices", "Open file %s and fetch histograms",
	 fFileCovMatrices->GetName());

    // Fetch jet histograms
    fHistCovMatJets_EtaEta = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_Eta");
    fHistCovMatJets_PtPt   = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_Pt");
    fHistCovMatJets_PhiPhi = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_Phi");
    fHistCovMatJets_PtEta  = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_PtEta");
    fHistCovMatJets_PtPhi  = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_PtPhi");
    fHistCovMatJets_EtaPhi = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_Jets/fFinalHist_EtaPhi");

    // Fetch Et_miss histograms
    fHistCovMatEtmiss_EtEt   = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_EtMiss/fFinalHist_Et_miss");
    fHistCovMatEtmiss_PhiPhi = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_EtMiss/fFinalHist_Phi_miss");
    fHistCovMatEtmiss_EtPhi  = (TH2F*)fFileCovMatrices
	->Get("CovarianceMatrix_EtMiss/fFinalHist_EtPhi_miss");

    if ( fHistCovMatJets_EtaEta == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_Eta\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatJets_PtPt == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_Pt\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatJets_PhiPhi == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_Phi\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatJets_PtEta == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_PtEta\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatJets_PtPhi == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_PtPhi\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatJets_EtaPhi == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_EtaPhi\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatEtmiss_EtEt == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_Et_miss\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatEtmiss_PhiPhi == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_Phi_miss\" not available. Please check!");
	gSystem->Abort(0);
    }
    if ( fHistCovMatEtmiss_EtPhi == 0 ) {
	Error("LoadCovMatrices",
	      "Histogram \"fFinalHist_EtPhi_miss\" not available. Please check!");
	gSystem->Abort(0);
    }

    fHasLoadedCovMatrices = kTRUE;
    savdir->cd();
}

//____________________________________________________________________

void AtlKinFitterTool::Terminate() {
    //
    // Terminate tool for kinematic fitting
    //
    fHist_KinFit_TrueReco->Sumw2();
    fHist_KinFit_TrueRecoProb->Sumw2();
    fHist_KinFit_TrueRecoProbGreaterP->Sumw2();
    fHist_KinFit_RecoProbGreaterP->Sumw2();
    fHist_KinFit_True->Sumw2();

    fHist_KinFit_Reco->Sumw2();
    fHist_KinFit_prob->Sumw2();

    //
    // KinFitter purity and efficiency
    //
    fOutputFile->cd();
    gDirectory->cd(GetName());
    gDirectory->cd("KinFitterTool/Fitter/TruthMatching");

    fGraph_KinFit_Efficiency->BayesDivide(fHist_KinFit_TrueReco,fHist_KinFit_True);
    fGraph_KinFit_Efficiency->SetTitle("Efficiency");
    fGraph_KinFit_Efficiency->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{True}}");
    fGraph_KinFit_Efficiency->GetXaxis()->SetTitle(fHist_KinFit_Efficiency->GetXaxis()->GetTitle());
    fGraph_KinFit_Efficiency->Write("efficiency");

    fGraph_KinFit_Purity->BayesDivide(fHist_KinFit_TrueReco,fHist_KinFit_Reco);
    fGraph_KinFit_Purity->SetTitle("Purity");
    fGraph_KinFit_Purity->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{Reco}}");
    fGraph_KinFit_Purity->GetXaxis()->SetTitle(fHist_KinFit_Purity->GetXaxis()->GetTitle());
    fGraph_KinFit_Purity->Write("purity");

    fGraph_KinFit_PurityProb->BayesDivide(fHist_KinFit_TrueRecoProb,fHist_KinFit_prob);
    fGraph_KinFit_PurityProb->SetTitle("Purity");
    fGraph_KinFit_PurityProb->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{Reco}}");
    fGraph_KinFit_PurityProb->GetXaxis()->SetTitle("#chi^{2} Probability");
    fGraph_KinFit_PurityProb->Write("PurityProb");

    fGraph_KinFit_PurityProbGreaterP->BayesDivide(fHist_KinFit_TrueRecoProbGreaterP,fHist_KinFit_RecoProbGreaterP);
    fGraph_KinFit_PurityProbGreaterP->SetTitle("Purity");
    fGraph_KinFit_PurityProbGreaterP->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{Reco}}");
    fGraph_KinFit_PurityProbGreaterP->GetXaxis()->SetTitle("P(#chi^{2}) > p*");
    fGraph_KinFit_PurityProbGreaterP->Write("PurityProbGreaterP");

    if ( GetAbundanceTrue() != 0 ){
	fHist_KinFit_NTrueProb->Sumw2();

	fGraph_KinFit_EfficiencyProbGreaterP->BayesDivide(fHist_KinFit_TrueRecoProbGreaterP,fHist_KinFit_NTrueProb);
	fGraph_KinFit_EfficiencyProbGreaterP->SetTitle("Efficiency");
	fGraph_KinFit_EfficiencyProbGreaterP->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{True}}");
	fGraph_KinFit_EfficiencyProbGreaterP->GetXaxis()->SetTitle("P(#chi^{2}) > p*");
	fGraph_KinFit_EfficiencyProbGreaterP->Write("EfficiencyProbGreaterP");

	fGraph_KinFit_EfficiencyProb->BayesDivide(fHist_KinFit_TrueRecoProb,fHist_KinFit_NTrueProb);
	fGraph_KinFit_EfficiencyProb->SetTitle("Efficiency");
	fGraph_KinFit_EfficiencyProb->GetYaxis()->SetTitle("#frac{N_{Reco&True}}{N_{True}}");
	fGraph_KinFit_EfficiencyProb->GetXaxis()->SetTitle("P(#chi^{2}) > p*");
	fGraph_KinFit_EfficiencyProb->Write("EfficiencyProb");
    }

    // Kill empty bins of CutOverview histogram
    fHistCutFlowWeighted->LabelsDeflate("x");
    fHistCutFlowUnweighted->LabelsDeflate("x");

    fHistCutFlow2Weighted->LabelsDeflate("x");
    fHistCutFlow2Unweighted->LabelsDeflate("x");

    // Use reasonable Y axis range starting at zero to prevent confusion
    fHistCutFlowWeighted->SetAxisRange( 1.e-1,
					fHistCutFlowWeighted->GetMaximumStored(), "Y" );
    fHistCutFlowUnweighted->SetAxisRange( 1.e-1,
					  fHistCutFlowUnweighted->GetMaximumStored(), "Y" );

    fHistCutFlow2Weighted->SetAxisRange( 1.e-1,
					fHistCutFlow2Weighted->GetMaximumStored(), "Y" );
    fHistCutFlow2Unweighted->SetAxisRange( 1.e-1,
					  fHistCutFlow2Unweighted->GetMaximumStored(), "Y" );

    gDirectory->cd("../../../..");
}

//____________________________________________________________________

void AtlKinFitterTool::GuessTopDecayNeutrinoEtaE(Double_t& EtaStart,
						 Double_t& EStart,
						 TVector2 Et_miss,
						 HepParticle *Lepton,
						 AtlJet *BJet) {
    //
    // Provide reasonable starting values for eta and E of the
    // neutrino in leptonic top-quark decays with the help of the
    // currently set lepton and the b-jet, and TKinFitter::GetEtaNuRoot().
    //
    // This routine might be useful for ttbar or single-top
    // reconstruction
    //
    Double_t RootW1     = 0.;
    Double_t RootW2     = 0.;
    Double_t MinimumW   = 0.;
    Double_t RootTop1   = 0.;
    Double_t RootTop2   = 0.;
    Double_t MinimumTop = 0.;
    TLorentzVector P_lep  = Lepton->P();
    TLorentzVector P_bjet = BJet->P();
    TLorentzVector P_sum = P_lep + P_bjet;

    Int_t nW   = TKinFitter::GetEtaNuRoot(P_lep, Et_miss, fW_Mass,
					  RootW1, RootW2, MinimumW);
    Int_t nTop = TKinFitter::GetEtaNuRoot(P_sum, Et_miss, fTop_Mass,
					  RootTop1, RootTop2, MinimumTop);

    // Estimate the neutrino eta
    if ( !nTop && !nW ) { // no roots of constraints found
	EtaStart =  0.5*(MinimumW+MinimumTop);
    } else if ( nTop && !nW ) { // root found only for Top mass constraint
	if ( TMath::Abs(RootTop1-MinimumW) < TMath::Abs(RootTop2-MinimumW) ) {
	    EtaStart = 0.5*(RootTop1+MinimumW);
	} else {
	    EtaStart = 0.5*(RootTop2+MinimumW);
	}
    } else if ( !nTop && nW ) { // root found only for W mass constraint
	if ( TMath::Abs(RootW1-MinimumTop) < TMath::Abs(RootW2-MinimumTop) ) {
	    EtaStart = 0.5*(RootW1+MinimumTop);
	} else {
	    EtaStart = 0.5*(RootW2+MinimumTop);
	}
    } else {
	// two roots found for each mass constraint
	// search for pair of roots with smallest distance
	// return their mean value
	Double_t roots[2];
	roots[0] = 0;
	roots[1] = 0;

	// =================
	// 4 possible pairs:
	// =================
	Int_t n = 4;
	TArrayD *dist = new TArrayD(n);

	// (W1,T1)
	dist->AddAt(TMath::Abs(RootW1-RootTop1),0);

	// (W1,T2)
	dist->AddAt(TMath::Abs(RootW1-RootTop2),1);

	// (W2,T1)
	dist->AddAt(TMath::Abs(RootW2-RootTop1),2);

	// (W2,T2)
	dist->AddAt(TMath::Abs(RootW2-RootTop2),3);

	// Create index array
	TArrayI *index = new TArrayI(n);
	TMath::Sort(n, dist->GetArray(), index->GetArray(), kFALSE);

	Int_t opt = index->At(0);

	delete dist;
	delete index;

	switch ( opt ) {
	    case 0 :
		roots[0] = RootW1;
		roots[1] = RootTop1;
		break;
	    case 1 :
		roots[0] = RootW1;
		roots[1] = RootTop2;
		break;
	    case 2 :
		roots[0] = RootW2;
		roots[1] = RootTop1;
		break;
	    case 3 :
		roots[0] = RootW2;
		roots[1] = RootTop2;
		break;
	    default :
		Error("GuessTopDecayNeutrinoEtaE","Variable \"opt\" out of bounds.");
		cout<<endl<<"Variable \"opt\" = "<<opt<<" !"<<endl;
		gSystem->Abort();
	}

	EtaStart = 0.5*(roots[0]+roots[1]);
    }

    // Estimate the neutrino energy
    EStart = Et_miss.Mod()*TMath::CosH(EtaStart);
}
