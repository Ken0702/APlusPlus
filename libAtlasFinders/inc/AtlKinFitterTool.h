//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlKinFitterTool
#define ATLAS_AtlKinFitterTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ROOT_TMatrixD
#include <TMatrixD.h>
#endif
#ifndef ROOT_TRandom3
#include <TRandom3.h>
#endif
#ifndef TFITPARTICLEPXPYPZ_H
#include <TFitParticlePxPyPz.h>
#endif
#ifndef TFITPARTICLERELPXPYPZ_H
#include <TFitParticleRelPxPyPz.h>
#endif
#ifndef TFITPARTICLEPTETAPHI_H
#include <TFitParticlePtEtaPhi.h>
#endif
#ifndef TFITPARTICLEPTHETAPHI_H
#include <TFitParticlePThetaPhi.h>
#endif
#ifndef TFITPARTICLEPTTHETAPHI_H
#include <TFitParticlePtThetaPhi.h>
#endif
#ifndef TFITPARTICLEPINVTHETAPHI_H
#include <TFitParticlePInvThetaPhi.h>
#endif
#ifndef TFITPARTICLERELPTETAPHI_H
#include <TFitParticleRelPtEtaPhi.h>
#endif
#ifndef TFITPARTICLEPTETAPHIE_H
#include <TFitParticlePtEtaPhiE.h>
#endif
#ifndef TFITPARTICLERELPTETAPHIE_H
#include <TFitParticleRelPtEtaPhiE.h>
#endif
#ifndef TFITCONSTRAINTM_H
#include <TFitConstraintM.h>
#endif
#ifndef TKINFITTER_H
#include <TKinFitter.h>
#endif
#ifndef TABSFITCONSTRAINT_H
#include <TAbsFitConstraint.h>
#endif
#ifndef TFITCONSTRAINTPT_H
#include <TFitConstraintPt.h>
#endif
#ifndef TFITCONSTRAINTMGAUS_H
#include <TFitConstraintMGaus.h>
#endif
#ifndef TFITCONSTRAINTMBW_H
#include <TFitConstraintMBW.h>
#endif
#ifndef TFITCONSTRAINTMBW2_H
#include <TFitConstraintMBW2.h>
#endif
#ifndef TFITCONSTRAINTEPGAUS_H
#include <TFitConstraintEpGaus.h>
#endif
#ifndef TFITCONSTRAINTEP_H
#include <TFitConstraintEp.h>
#endif
#ifndef ROOT_TH1I
#include <TH1I.h>
#endif

class TFile;
class TH1F;
class TH2F;
class TLorentzVector;
class TGraph;
class TGraphAsymmErrors;

class AtlKinFitterTool : public AtlAnalysisTool {

  public:
    enum EMode {
	kCutBased = BIT(0),
	kKinFit   = BIT(1),
      kAll      = (kCutBased | kKinFit)
    };
    enum EModeMass { kGauss, kBW };
    
  protected:
    TKinFitter *fKinFitter;          // KinFitter object
    Float_t     fChi2;               // Chi2 of recent fit
    Int_t       fNDoF;               // No. of degrees of freedom of recent fit
    Int_t       fNbIter;             // No. of iterations of fit
    Double_t    fConsValue;          // Value of constraints ( sum_i|f_i| )
    Int_t       fAbundanceTrue;      // Abundance of true particles
    TRandom3   *randnr;      	     // for random covariance matrices of leptons
   

    static const char* fgStatusNames[]; // Status strings from KinFitter object
    
    // Histograms
    
    //control plots of covariance matrix elements 
    //electron
    TH1F *fhist_CovMatrix_el_11;		//covariance matrix electron element 11
    TH1F *fhist_CovMatrix_el_12;		//covariance matrix electron element 12
    TH1F *fhist_CovMatrix_el_21;		//covariance matrix electron element 21
    TH1F *fhist_CovMatrix_el_13;		//covariance matrix electron element 13
    TH1F *fhist_CovMatrix_el_31;		//covariance matrix electron element 31
    TH1F *fhist_CovMatrix_el_22;		//covariance matrix electron element 22
    TH1F *fhist_CovMatrix_el_23;		//covariance matrix electron element 23
    TH1F *fhist_CovMatrix_el_32;		//covariance matrix electron element 32
    TH1F *fhist_CovMatrix_el_33;		//covariance matrix electron element 11
    
     //muon
    TH1F *fhist_CovMatrix_mu_11;	//covariance matrix muon element 11
    TH1F *fhist_CovMatrix_mu_12;	//covariance matrix muon element 12
    TH1F *fhist_CovMatrix_mu_21;	//covariance matrix muon element 21
    TH1F *fhist_CovMatrix_mu_13;	//covariance matrix muon element 13
    TH1F *fhist_CovMatrix_mu_31;	//covariance matrix muon element 31
    TH1F *fhist_CovMatrix_mu_22;	//covariance matrix muon element 22
    TH1F *fhist_CovMatrix_mu_23;	//covariance matrix muon element 23
    TH1F *fhist_CovMatrix_mu_32;	//covariance matrix muon element 32
    TH1F *fhist_CovMatrix_mu_33;	//covariance matrix muon element 33
    
    // KinFit plots
    TH1F *fHist_KinFit_chi2;                // chi square values of the Kinematic Fit in case of convergence
    TH1F *fHist_KinFitBkg_chi2;             // corresponding histogram for background events
    TH1F *fHist_KinFit_prob;                // Probability values of the Kinematic Fit in case of convergence
    TH1F *fHist_KinFitBkg_prob;             // corresponding histogram for background events

    TH1F *fHist_KinFit_NbIterCand;          // Number of iterations performed in the fit of accepted candidates
    TH1F *fHist_KinFit_NbIterAll;          // Number of iterations performed in ALL fitting procedures
    TH1F *fHist_KinFit_ConsValue;           // Value of constraints for candidate evts after fit
    TH1F *fHist_KinFit_NbFitsConverged;  // Number of fits converged
    TH1F *fHist_KinFit_NbFitsNotConverged;  // Number of fits not converged
    TH1F *fHist_KinFit_NbFitsFailedNumerically;// Number of fits failing numerically (1 or more matrix inversions fail)
    TH1F *fHist_KinFit_KinFitterStatus;     // Status of KinFitter after fit
    
    // Graphs and histograms to plot efficiency and purity
    TH1F *fHist_KinFit_Reco;                // Number of reconstructed events
    TH1F *fHist_KinFit_True;                // Number of true events
    TH1F *fHist_KinFit_NTrueProb;           // Number of true particles, constant for all prob values!
    TH1F *fHist_KinFit_TrueReco;            // Number of true reconstructed events
    TH1F *fHist_KinFit_TrueRecoProb;        // Number of true reconstructed events vs. probability
    TH1F *fHist_KinFit_TrueRecoProbGreaterP;// Number of true reconstructed events vs. probability greater than p*
    TH1F *fHist_KinFit_RecoProbGreaterP;    // Number of reconstructed prts vs. probability greater than p*
    TH1F *fHist_KinFit_Efficiency;          // Efficiency = N_TrueReco / N_True
    TH1F *fHist_KinFit_Purity;              // Purity = N_TrueReco / N_Reco
    TH1F *fHist_KinFit_PurityProb;          // Purity vs. Probability
       
    Bool_t fTrueReco;                       // particle reconstructed is true?

    // cut flow histograms for finders, weighted and unweighted
    // (possible to use the 2nd cut-flow in different mode of finder)
    TH1F *fHistCutFlowWeighted;             // show effects of cuts, weighted events
    TH1F *fHistCutFlowUnweighted;           // show effects of cuts, unweighted events
    TH1F *fHistCutFlow2Weighted;            // show effects of cuts, weighted events
    TH1F *fHistCutFlow2Unweighted;          // show effects of cuts, unweighted events

    
    // histograms to plot efficiency and purity versus fX_TruthHistograms, which is
    // set by the user in the analysis.
    TGraphAsymmErrors *fGraph_KinFit_Efficiency;
    TGraphAsymmErrors *fGraph_KinFit_Purity;
    
    // histogram to plot purity versus chi-square probability
    TGraphAsymmErrors *fGraph_KinFit_PurityProb;
    TGraphAsymmErrors *fGraph_KinFit_EfficiencyProb;
    TGraphAsymmErrors *fGraph_KinFit_PurityProbGreaterP;     // purity of [ p(chi^2) > p* ]
    TGraphAsymmErrors *fGraph_KinFit_EfficiencyProbGreaterP; // efficiency of [ p(chi^2) > p* ]
    
    Double_t fX_TruthHistograms;             // variable to bin True, Reco and TrueReco 
                                            // Truth Matching histograms

    // Jet/Etmiss covariances binned in Pt and Eta
    Bool_t  fHasLoadedCovMatrices;    // Histograms already loaded ?
    TFile  *fFileCovMatrices;         // File containing the covariance matrix histograms
    TH2F   *fHistCovMatJets_EtaEta;   // Jet cov matrix element eta-eta
    TH2F   *fHistCovMatJets_PtPt;     // Jet cov matrix element Pt-Pt
    TH2F   *fHistCovMatJets_PhiPhi;   // Jet cov matrix element phi-phi
    TH2F   *fHistCovMatJets_PtEta;    // Jet cov matrix element pt-eta
    TH2F   *fHistCovMatJets_PtPhi;    // Jet cov matrix element pt-phi
    TH2F   *fHistCovMatJets_EtaPhi;   // Jet cov matrix element eta-phi
    TH2F   *fHistCovMatEtmiss_EtEt;   // Et_miss cov matrix element Et-Et
    TH2F   *fHistCovMatEtmiss_PhiPhi; // Et_miss cov matrix element phi-phi
    TH2F   *fHistCovMatEtmiss_EtPhi;  // Et_miss cov matrix element Et-phi

  public:
    // Tool paramters / cuts
    EMode     fMode;       // Indicate the mode of running (eg cut-based or kinematic fit)
    EModeMass fModeMass;// Indicate the mode of mass constraint (Gauss or BW mass constraint)
    Bool_t    fUseLeptonCovRand; // Use randomly generated lepton covariance matrices?
        
    // True W and top masses and widths used in some fits (ttbar, single-top etc.)
    Double_t fW_Mass;     // Pole mass of the W used in  the fit
    Double_t fW_Width;    // Width of the W used in the fit
    Double_t fB_Mass;     // Assumed mass of b-jet = mass of b-quark
    Double_t fTop_Mass;   // Pole mass of the t-quark used in the fit
    Double_t fTop_Width;  // Width of the t-quark used in the fit
    
  public:
    AtlKinFitterTool(const char* name, const char* title);
    virtual ~AtlKinFitterTool();
    virtual void SetCutDefaults();
    virtual void BookHistograms();
    virtual void FillHistograms(Option_t *HistMode);
    virtual void FillLeptonCovMatrixHistograms(HepParticle* lepton,TMatrixD& CovLepton);
    virtual void Terminate();
    void SetCovMatrix(TMatrixD& cov, HepJet *jet,
		      Double_t sigma_pt, Double_t sigma_eta,Double_t sigma_phi,
		      Double_t cov_pt_eta, Double_t cov_pt_phi, Double_t cov_eta_phi);
    void SetCovMatrix(TMatrixD& cov, HepJet *jet);
    void SetCovMatrix(TMatrixD& cov, HepParticle *nu,
		      Double_t SigmaPt_MET, Double_t SigmaPhi_MET, Double_t Cov_Pt_Phi_MET);
    void SetCovMatrix(TMatrixD& cov, HepParticle *nu);
    void GetLeptonCovMatRand(HepParticle* lepton,TMatrixD& CovLepton);
    void LoadCovMatrices();
    void GuessTopDecayNeutrinoEtaE(Double_t& EtaStart, Double_t& EStart, TVector2 Et_Miss,
				   HepParticle *Lepton, AtlJet *BJet);
    
    inline Float_t GetChi2() const { return fChi2; }
    inline Int_t GetNDoF() const { return fNDoF; }
    inline Float_t GetChi2overNDoF() const { return fChi2/fNDoF; }
    inline Float_t GetNbIter() const { return fNbIter; }
    inline Float_t GetConsValue() const { return fConsValue; }
    inline Double_t GetBinContent_WeightedCutFlow(Int_t i_bin) {
	return fHistCutFlowWeighted->GetBinContent(i_bin);
    }
    inline Double_t GetBinContent_UnweightedCutFlow(Int_t i_bin) {
	return fHistCutFlowUnweighted->GetBinContent(i_bin);
    }
    inline Double_t GetBinContent_WeightedCutFlow2(Int_t i_bin) {
	return fHistCutFlow2Weighted->GetBinContent(i_bin);
    }
    inline Double_t GetBinContent_UnweightedCutFlow2(Int_t i_bin) {
	return fHistCutFlow2Unweighted->GetBinContent(i_bin);
    }

    
    inline void SetChi2(Double_t chi2) { fChi2 = chi2; }
    inline void SetNDoF(Int_t ndf) { fNDoF = ndf; }
    inline void SetNbIter(Int_t nbIter) { fNbIter = nbIter; }
    inline void SetConsValue(Double_t cons) { fConsValue = cons; }
    inline void SetMode(EMode mode = kKinFit) { fMode = mode; }    
    inline void SetX_TruthHistograms(Double_t X) { fX_TruthHistograms = X; }
    inline void SetTrueReco(Bool_t TrueReco) { fTrueReco = TrueReco; }
    inline void SetCutFlow(const char* cut) {
	fHistCutFlowWeighted->Fill( cut, GetPreTagEvtWeight() );
	fHistCutFlowUnweighted->Fill( cut, 1. );
    }
    inline void SetCutFlow2(const char* cut) {
	fHistCutFlow2Weighted->Fill( cut, GetPreTagEvtWeight() );
	fHistCutFlow2Unweighted->Fill( cut, 1.);
    }

    inline void SetTruthMatchXTitle(const char* XTitle) {
	fHist_KinFit_True->GetXaxis()->SetTitle(XTitle);
	fHist_KinFit_Reco->GetXaxis()->SetTitle(XTitle);
	fHist_KinFit_TrueReco->GetXaxis()->SetTitle(XTitle);
	fHist_KinFit_Efficiency->GetXaxis()->SetTitle(XTitle);
	fHist_KinFit_Purity->GetXaxis()->SetTitle(XTitle);
    }

    inline void SetTruthMatchBinning (Int_t Nbins, Double_t xlow, Double_t xup) {
	fHist_KinFit_True->SetBins(Nbins, xlow, xup);
	fHist_KinFit_Reco->SetBins(Nbins, xlow, xup);
	fHist_KinFit_TrueReco->SetBins(Nbins, xlow, xup);
	fHist_KinFit_Efficiency->SetBins(Nbins, xlow, xup);
	fHist_KinFit_Purity->SetBins(Nbins, xlow, xup);
    }
    //inline void SetTruthType(char *type) {fTruthType = type;}
    //inline char* GetTruthType() {return fTruthType;}
    inline void AddAbundanceTrue(Int_t abundance) {
	for( Int_t j = 1; j <= 30; j++){
	    Float_t xBin = fHist_KinFit_NTrueProb->GetBinCenter(j);
	    for( Int_t i = 1; i <= abundance; i++)
		fHist_KinFit_NTrueProb->Fill(xBin,GetPreTagEvtWeight());
	}
	fAbundanceTrue += abundance;
    }
    inline Int_t GetAbundanceTrue() {return fAbundanceTrue;}
    
    ClassDef(AtlKinFitterTool,0) // A++ KinFitter interface
};
#endif

