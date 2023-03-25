#define TToyGentSgTop_cxx

#include <iostream>
#include "KinFitter/TToyGentSgTop.h"
#include "TBenchmark.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TString.h"

ClassImp(TToyGentSgTop)

using namespace std;

TToyGentSgTop::TToyGentSgTop()
  : TNamed("NoName", "NoTitle") {

  fPrintPartIni = false;
  fPrintConsIni = false;
  fPrintSmearedPartBefore = false;
  fPrintConsBefore = false;
  fPrintConsAfter = false;
  fPrintPartAfter = false;
  fDoCheckConstraintsTruth = false;

  fVerbosity=0;

}

//_______________________________________________________________________________

TToyGentSgTop::~TToyGentSgTop() {

    delete fElectron;
    delete fNeutrino;
    delete fBQuark;

}

//_______________________________________________________________________________

void TToyGentSgTop::SetParticles(){

    // get masses from distribution
    fTrueTopMass = gRandom->BreitWigner(fTopMass, fTopWidth); // first application can lead
                                                              // to a large fluctiation, bad seed?
    fTrueTopMass = gRandom->BreitWigner(fTopMass, fTopWidth);
    fTrueWMass   = gRandom->BreitWigner(fWMass, fWWidth);


    // compute momentum of b quark and W boson
    // approximately massless b quark
    Double_t p = (fTrueTopMass*fTrueTopMass-fTrueWMass*fTrueWMass)/2./fTrueTopMass;

    // compute 3 momenta of b quark and W boson
    TVector3 pw;
    Double_t px, py, pz;
    gRandom->Sphere(px, py, pz, p); // isotropic decay of the top quark
    fPBQuarkTrue.SetXYZ(px, py, pz);

    pw=-fPBQuarkTrue;

    // compute 3 momenta of electron and neutrino in W boson rest frame
    gRandom->Sphere(px, py, pz, fTrueWMass/2.); // isotropic decay of the W boson
    fPElectronTrue.SetXYZ(px, py, pz);
    fPNeutrinoTrue=-fPElectronTrue;

    // now transform fPElectronTrue and fPNeutrinoTrue into the top quark restframe
    // first: get velocity vector of the W boson
    TVector3 BetaW=pw;
    BetaW*=1./TMath::Sqrt(fTrueWMass*fTrueWMass+p*p);
    TLorentzVector pe4, pnu4;
    pe4.SetXYZM(fPElectronTrue.X(), fPElectronTrue.Y(), fPElectronTrue.Z(), 0.);
    pnu4.SetXYZM(fPNeutrinoTrue.X(),fPNeutrinoTrue.Y(), fPNeutrinoTrue.Z(), 0.);
    // now apply Lorentz Transformation
    pe4.Boost(BetaW.X(), BetaW.Y(), BetaW.Z());
    pnu4.Boost(BetaW.X(), BetaW.Y(), BetaW.Z());
    fPElectronTrue=pe4.Vect();
    fPNeutrinoTrue=pnu4.Vect();

    // got momenta, now get covariance matrices
    TMatrixD cov_b(3,3);
    TMatrixD cov_e(3,3);
    TMatrixD cov_nu(3,3);
    cov_b.Zero();
    cov_e.Zero();
    cov_nu.Zero();

    // preliminary values for the moment
    // using parameterization (pt, eta, phi)
    cov_b[0][0] = 1.0e-01*fPBQuarkTrue.Pt();
    cov_b[1][1] = 1.0e-02*fPBQuarkTrue.Eta();
    cov_b[2][2] = 1.0e-02*fPBQuarkTrue.Phi();
    cov_b[0][0] *= cov_b[0][0];
    cov_b[1][1] *= cov_b[1][1];
    cov_b[2][2] *= cov_b[2][2];

    cov_e[0][0] = 2.0e-02*fPElectronTrue.Pt();
    cov_e[1][1] = 1.0e-02*fPElectronTrue.Eta();
    cov_e[2][2] = 1.0e-02*fPElectronTrue.Phi();
    cov_e[0][0] *= cov_e[0][0];
    cov_e[1][1] *= cov_e[1][1];
    cov_e[2][2] *= cov_e[2][2];

    cov_nu[0][0] = 2.5e-01*fPNeutrinoTrue.Pt();
    cov_nu[2][2] = 1.e-02*fPNeutrinoTrue.Phi();
    cov_nu[0][0] *= cov_nu[0][0];
    cov_nu[2][2] *= cov_nu[2][2];

    // using parameterization (pt, Theta, phi)
    fElectron = new TFitParticlePtEtaPhi("Electron", "Electron", &fPElectronTrue, 0, &cov_e);
    fNeutrino = new TFitParticlePtThetaPhi("Neutrino", "Neutrino", &fPNeutrinoTrue, 0, &cov_nu);
    fBQuark = new TFitParticlePtEtaPhi("b quark", "b quark", &fPBQuarkTrue, 0, &cov_b);

}

//_______________________________________________________________________________

void TToyGentSgTop::SetFitter(){
    // set fitter object
    fFitter.reset();

    fFitter.addMeasParticle(fElectron);

    fFitter.addMeasParticle(fNeutrino);

    /**
       set the right unmeasured parameter of the neutrino
       this depends on its parametrization
       e.g. par 1 for PtEtaPhi, par 2 for PxPyPz
    */
    fFitter.setParamUnmeas(fNeutrino, 1);

    fFitter.addMeasParticle(fBQuark);

    if ( kBW ) {
	fMConsW = new TFitConstraintMBW2( "MassConstraint W", "Mass-Constraint for W boson", 0,
					  fWMass, fWWidth);
	fMConsW->addParticles1( fElectron, fNeutrino );

	fMConsTop = new TFitConstraintMBW2( "MassConstraint Top", "Mass-Constraint for Top Quark", 0,
					    fTopMass, fTopWidth);
	fMConsTop->addParticles1( fElectron, fNeutrino, fBQuark );
    }
    else {
	fMConsW = new TFitConstraintMGaus( "MassConstraint W", "Mass-Constraint for W boson", 0, 0,
					  fWMass, fWWidth);
	fMConsW->addParticles1( fElectron, fNeutrino );

	fMConsTop = new TFitConstraintMGaus( "MassConstraint Top", "Mass-Constraint for Top Quark", 0, 0,
					     fTopMass, fTopWidth);
	fMConsTop->addParticles1( fElectron, fNeutrino, fBQuark );
    }

    fFitter.addConstraint(fMConsW);
    fFitter.addConstraint(fMConsTop);

    fFitter.setMaxNbIter( 5000 );
    fFitter.setMaxDeltaS( 5e-5 );
    fFitter.setMaxF( 1e-4 );
    fFitter.setVerbosity( fVerbosity );

}

//_______________________________________________________________________________

Bool_t TToyGentSgTop::DoToyExperiments( Int_t NbExperiments ) {

    // set mass values and widths
    fTopMass  = 175.;
    fTopWidth = 2.;
    fWMass    = 80.4;
    fWWidth   = 2.14;

    kBW = kTRUE;

    fVerbosity = 0;

    // start benchmark
    gBenchmark->Start("ToyExperiments");

    // perform pseudo experiments
    for (int i = 0; i < NbExperiments; i++) {

	// get particles from top decay and their covariance matrices
	// Caution: This Toy Monte Carlo has different true particles
	// in every loop. This must be taken into account in the following.
	SetParticles();

	if( fPrintPartIni ) {
	    cout << endl
		 << "----------------------------------" << endl;
	    cout << "--- PRINTING INITIAL PARTICLES ---" << endl;
	    cout << "----------------------------------" << endl ;
	    fElectron->printParticle();
	    fNeutrino->printParticle();
	    fBQuark->printParticle();
	    cout << endl << endl;
	}

	SetFitter();
	SmearParticles();

	// Check initial constraints
	if (  fDoCheckConstraintsTruth ) {
	    if (fFitter.getF() > fFitter.getMaxF()) {
		cout << "Initial constraints are not fulfilled." << endl;
	    }
	}

	// book histograms at the start of the loop
	if ( i==0 ) BookHistograms();

	if( fPrintSmearedPartBefore ) {
	    cout <<  endl
		 << "-------------------------------------------------------" << endl ;
	    cout << "--- PRINTING SMEARED PARTICLES BEFORE FIT FOR experiment # " <<i+1 << endl;
	    cout << "-------------------------------------------------------" << endl;
	    fElectron->printParticle();
	    fNeutrino->printParticle();
	    fBQuark->printParticle();
	}

	fFitter.fit();

	if( fPrintPartAfter ) {
	    cout <<  endl
		 << "--------------------------------------------------------" << endl ;
	    cout << "--- PRINTING PARTICLES AFTER FIT FOR experiment # "<< i+1 << endl;
	    cout << "--------------------------------------------------------" << endl;
	    fElectron->printParticle();
	    fNeutrino->printParticle();
	    fBQuark->printParticle();
	}

	fHistStatus->Fill( fFitter.getStatus() );
	fHistNIter->Fill( fFitter.getNbIter() );
	if ( fFitter.getStatus() == 0 ) {
	    FillHistograms();
	}

	if (i % 176 == 0) {
	    cout << "\r";
	    cout <<" ------ "<< (Double_t) i/NbExperiments*100. << " % PROCESSED ------";
	    cout.flush();
	}

    }

    // Stop Benchmark
    cout << endl;
    gBenchmark->Stop("ToyExperiments");
    gBenchmark->Show("ToyExperiments");

    return true;
}

//_______________________________________________________________________________

void TToyGentSgTop::SmearParticles() {

    // Smear measured particles

    for (UInt_t p = 0; p < 3; p++) {

	TAbsFitParticle* particle = 0;
	if ( p == 0 ) particle = fElectron;
	if ( p == 1 ) particle = fNeutrino;
	if ( p == 2 ) particle = fBQuark;

	TMatrixD parIni( *(particle->getParIni()) );
	const TMatrixD* covM = particle->getCovMatrix();
	for (int m = 0; m < particle->getNPar(); m++) {
	    parIni(m, 0) += gRandom->Gaus(0., TMath::Sqrt( (*covM)(m,m) ) );

// 	    // possibility: set neutrino theta to one certain value, for example pi/2
// 	    // that means: no idea of neutrino theta a priori
// 	    if ( p == 1 && m == 1) {
// 		parIni(m, 0) = TMath::Pi()/2.;
// 	    }

	}
	TLorentzVector* ini4Vec = particle->calc4Vec( &parIni );
	particle->setIni4Vec( ini4Vec );
	delete ini4Vec;
    }

    // guess neutrino eta with the corresponding helper function

    Double_t EtaStart;
    Double_t EStart;
    GuessTopDecayNeutrinoEtaE( EtaStart, EStart );

    const TLorentzVector Neutrino4VectorOld = *(fNeutrino->getCurr4Vec());
    TLorentzVector *Neutrino4Vector = new TLorentzVector(Neutrino4VectorOld);
    Neutrino4Vector->SetPtEtaPhiE( Neutrino4Vector->Pt(),
				   EtaStart,
				   Neutrino4Vector->Phi(),
				   Neutrino4Vector->Pt()*TMath::CosH(EtaStart) );
    fNeutrino->setIni4Vec( Neutrino4Vector );
    delete Neutrino4Vector;

}

//_______________________________________________________________________________

void TToyGentSgTop::FillHistograms(){

    // chi2 and chi2-probability
    Double_t chi2 = fFitter.getS();

    fHistPChi2->Fill( TMath::Prob( chi2, fFitter.getNDF() ) );
    fHistChi2->Fill( chi2 );

    // true top and W masses
    fHistTrueTopMass->Fill(fTrueTopMass);
    fHistTrueWMass->Fill(fTrueWMass);
    fHistRecoTopMass->Fill(( *(fElectron->getCurr4Vec()) +  *(fNeutrino->getCurr4Vec()) + *(fBQuark->getCurr4Vec()) ).M());
    fHistRecoWMass->Fill(( *(fElectron->getCurr4Vec()) +  *(fNeutrino->getCurr4Vec())).M() );

    // Truth Matching
    // comparing reconstructed and true value, normalizing to true value
    fHistTruthMatchElectronPt->Fill( ((*(fElectron->getCurr4Vec())).Pt() - fPElectronTrue.Pt())/fPElectronTrue.Pt() );
    fHistTruthMatchElectronEta->Fill( ((*(fElectron->getCurr4Vec())).Eta() - fPElectronTrue.Eta())/fPElectronTrue.Eta() );
    fHistTruthMatchElectronPhi->Fill( ((*(fElectron->getCurr4Vec())).Phi() - fPElectronTrue.Phi())/fPElectronTrue.Phi() );
    fHistTruthMatchNeutrinoPt->Fill( ((*(fNeutrino->getCurr4Vec())).Pt() - fPNeutrinoTrue.Pt())/fPNeutrinoTrue.Pt() );
    fHistTruthMatchNeutrinoEta->Fill( ((*(fNeutrino->getCurr4Vec())).Eta() - fPNeutrinoTrue.Eta())/fPNeutrinoTrue.Eta() );
    fHistTruthMatchNeutrinoPz->Fill( ((*(fNeutrino->getCurr4Vec())).Pz() - fPNeutrinoTrue.Pz())/fPNeutrinoTrue.Pz() );
    fHistTruthMatchNeutrinoPhi->Fill( ((*(fNeutrino->getCurr4Vec())).Phi() - fPNeutrinoTrue.Phi())/fPNeutrinoTrue.Phi() );
    fHistTruthMatchBQuarkPt->Fill( ((*(fBQuark->getCurr4Vec())).Pt() - fPBQuarkTrue.Pt())/fPBQuarkTrue.Pt() );
    fHistTruthMatchBQuarkEta->Fill( ((*(fBQuark->getCurr4Vec())).Eta() - fPBQuarkTrue.Eta())/fPBQuarkTrue.Eta() );
    fHistTruthMatchBQuarkPhi->Fill( ((*(fBQuark->getCurr4Vec())).Phi() - fPBQuarkTrue.Phi())/fPBQuarkTrue.Phi() );

    // pull distributions

    const TMatrixD *ElectronPull = fElectron->getPull();
    const TMatrixD *NeutrinoPull = fNeutrino->getPull();
    const TMatrixD *BQuarkPull = fBQuark->getPull();

    fHistPullElectronPt->Fill( (*ElectronPull)(0, 0) );
    fHistPullElectronEta->Fill( (*ElectronPull)(1, 0) );
    fHistPullElectronPhi->Fill( (*ElectronPull)(2, 0) );

    fHistPullNeutrinoPt->Fill(  (*NeutrinoPull)(0, 0) );
    fHistPullNeutrinoPhi->Fill( (*NeutrinoPull)(2, 0) );

    fHistPullBQuarkPt->Fill( (*BQuarkPull)(0, 0) );
    fHistPullBQuarkEta->Fill( (*BQuarkPull)(1, 0) );
    fHistPullBQuarkPhi->Fill( (*BQuarkPull)(2, 0) );

}

//_______________________________________________________________________________

void  TToyGentSgTop::BookHistograms() {

    // fitter
    fHistStatus = new TH1F( "hStatus", "Status of the Fit; fit status", 16, -1, 15);
    fHistNIter = new TH1F( "hNIter", "Number of iterations; # iterations", 100, 0, 100);
    fHistPChi2 = new TH1F( "hPChi2", "Chi2 probability; #chi^{2} probability", 100, 0., 1.);
    fHistChi2 = new TH1F(  "hChi2", "Chi2; #chi^{2} ", 100, 0., 10.);

    fHistNIter->GetXaxis()->SetTitle("Anzahl Iterationen");
    fHistNIter->SetTitle("Einträge");
    fHistNIter->SetTitle(
	"Anzahl an Iterationen des kinematischen Fits im Spiel-Monte-Carlo bis zur Konvergenz");

    fHistPChi2->GetXaxis()->SetTitle("P(#chi^{2})");
    fHistPChi2->GetYaxis()->SetTitle("Einträge");
    fHistPChi2->SetTitle("");
    fHistChi2->GetXaxis()->SetTitle("#chi^{2}");
    fHistChi2->GetYaxis()->SetTitle("Einträge");
    fHistChi2->SetTitle("");

    // true and reconstructed masses
    fHistTrueTopMass = new TH1F( "hMTopTrue", "TrueTopMass", 100, 160, 190);
    fHistTrueWMass = new TH1F( "hMWTrue", "TrueWMass", 100, 65, 95);
    fHistRecoTopMass = new TH1F( "hMTopReco", "Reconstructed Top Mass", 250, 160, 190);
    fHistRecoWMass = new TH1F( "hMWReco", "Reconstructed W Mass", 250, 65, 95);

    fHistTrueTopMass->GetXaxis()->SetTitle("M_{top} [GeV]");
    fHistTrueTopMass->GetYaxis()->SetTitle("Einträge");
    fHistTrueTopMass->SetTitle("");

    fHistTrueWMass->GetXaxis()->SetTitle("M_{W} [GeV]");
    fHistTrueWMass->GetYaxis()->SetTitle("Einträge");
    fHistTrueWMass->SetTitle("");

    fHistRecoTopMass->GetXaxis()->SetTitle("M_{top} [GeV]");
    fHistRecoTopMass->GetYaxis()->SetTitle("Einträge");
    fHistRecoTopMass->SetTitle("");

    fHistRecoWMass->GetXaxis()->SetTitle("M_{W} [GeV]");
    fHistRecoWMass->GetYaxis()->SetTitle("Einträge");
    fHistRecoWMass->SetTitle("");

    // histogram of constraint mass parameter
    // this parameter is used to realize a finite mass distribution constraint
    fHistMassParameterW    = new TH1F("hMassParameterW","W boson Mass constraint parameter", 100, -5., 5.);
    fHistMassParameterTop  = new TH1F("hMassParameterTop","Top quark Mass constraint parameter", 100, -5., 5.);
    fHistMassParameterBoth = new TH1F("hMassParametersBoth","Mass constraint parameter, W and top", 100, -5., 5.);
    fHistChi2VsMassParameters = new TH2F("hChi2VsMassParameters", "#chi^{2} vs. mass constraint parameters",
					 20, 0., 10.,
					 10, 0., 10.);

    if ( kBW ){
	fHistMassParameterW->SetXTitle("#mu_{W}");
	fHistMassParameterTop->SetXTitle("#mu_{top}");
	fHistMassParameterBoth->SetXTitle("#mu_{W}^{2}+#mu_{top}^{2}");
	fHistChi2VsMassParameters->SetYTitle("#mu_{W}^{2}+#mu_{top}^{2}");
    }
    else {
	fHistMassParameterW->SetXTitle("#frac{#alpha_W-1}{#Gamma_{W}/M_{W}}");
	fHistMassParameterTop->SetXTitle("#frac{#alpha_t-1}{#Gamma_{t}/M_{t}}");
	fHistMassParameterBoth->SetXTitle("#frac{#alpha_W-1}^{2}{#Gamma_{W}/M_{W}}^{2}+#frac{#alpha_t-1}^{2}{#Gamma_{t}/M_{t}}^{2}");
	fHistChi2VsMassParameters->SetYTitle("#frac{#alpha_W-1}^{2}{#Gamma_{W}/M_{W}}^{2}+#frac{#alpha_t-1}^{2}{#Gamma_{t}/M_{t}}^{2}");
    }

    fHistChi2VsMassParameters->SetXTitle("#chi^{2}");

    fHistChi2ProbVsMassParameters = new TH2F("hChi2ProbVsMassParameters", "P(#chi^{2}) vs. mass constraint parameters",
					     20, 0., 1.,
					     20, 0., 10.);
    fHistChi2ProbVsMassParameters->SetXTitle("P(#chi^{2})");
    fHistChi2ProbVsMassParameters->SetYTitle("#mu_{W}^{2}+#mu_{top}^{2}");
    Float_t ParameterBins[10];
    ParameterBins[0] = 0.;
    ParameterBins[1] = 0.2;
    ParameterBins[2] = 0.4;
    ParameterBins[3] = 0.6;
    ParameterBins[4] = 0.8;
    ParameterBins[5] = 1.;
    ParameterBins[6] = 1.5;
    ParameterBins[7] = 3.;
    ParameterBins[8] = 7.;
    ParameterBins[9] = 10.;
    TAxis *axis = fHistChi2ProbVsMassParameters->GetYaxis();
    axis->Set( 9, ParameterBins );

    // Truth Matching histograms
    fHistTruthMatchElectronPt = new TH1F("hTMElecPt","Electron p_{T} Truth Matching", 100, -0.1, 0.1);
    fHistTruthMatchElectronPt->GetXaxis()->SetTitle("#frac{p_{T}^{fit}-p_{T}^{wahr}}{p_{T}^{wahr}}");
    fHistTruthMatchElectronPt->GetYaxis()->SetTitle("Einträge");
    fHistTruthMatchElectronPt->SetTitle("Elektron p_{T} Monte Carlo Abgleich");

    fHistTruthMatchElectronEta = new TH1F("hTMElecEta","Electron #eta Truth Matching", 100, -0.1, 0.1);
    fHistTruthMatchElectronEta->GetXaxis()->SetTitle("#frac{#eta^{fit}-#eta^{wahr}}{#eta^{wahr}}");
    fHistTruthMatchElectronEta->GetYaxis()->SetTitle("Einträge");
    fHistTruthMatchElectronEta->SetTitle("Elektron #eta Monte Carlo Abgleich");

    fHistTruthMatchElectronPhi = new TH1F("hTMElecPhi","Electron #varphi Truth Matching", 100, -0.1, 0.1);
    fHistTruthMatchElectronPhi->GetXaxis()->SetTitle("#frac{#varphi^{fit}-#varphi^{wahr}}{#varphi^{wahr}}");
    fHistTruthMatchElectronPhi->GetYaxis()->SetTitle("Einträge");
    fHistTruthMatchElectronPhi->SetTitle("Elektron #varphi Monte Carlo Abgleich");

    fHistTruthMatchNeutrinoPt = new TH1F("hTMNuPt","Neutrino p_{T} Truth Matching", 50, -0.5, 0.5);
    fHistTruthMatchNeutrinoEta = new TH1F("hTMNuEta","Neutrino #eta Truth Matching", 100, -6., 6.);
    fHistTruthMatchNeutrinoEta->GetXaxis()->SetTitle("#frac{#eta^{fit}-#eta^{wahr}}{#eta^{wahr}}");
    fHistTruthMatchNeutrinoEta->GetYaxis()->SetTitle("Einträge");
    fHistTruthMatchNeutrinoEta->SetTitle("Neutrino #eta Monte Carlo Abgleich");

    fHistTruthMatchNeutrinoPz = new TH1F("hTMNuPz","Neutrino p_{z} Truth Matching", 100, -6., 6.);
    fHistTruthMatchNeutrinoPz->GetXaxis()->SetTitle("#frac{p_{z}^{fit}-p_{z}^{wahr}}{p_{z}^{wahr}}");
    fHistTruthMatchNeutrinoPz->GetYaxis()->SetTitle("Einträge");
    fHistTruthMatchNeutrinoPz->SetTitle("Neutrino p_{z} Monte Carlo Abgleich");

    fHistTruthMatchNeutrinoPhi = new TH1F("hTMNuPhi","Neutrino #varphi Truth Matching", 50, -0.3, 0.3);
    fHistTruthMatchBQuarkPt = new TH1F("hTMBQuarkPt","b Quark p_{T} Truth Matching", 50, -0.3, 0.3);
    fHistTruthMatchBQuarkEta = new TH1F("hTMBQuarkEta","b Quark #eta Truth Matching", 50, -0.3, 0.3);
    fHistTruthMatchBQuarkPhi = new TH1F("hTMBQuarkPhi","b Quark #varphi Truth Matching", 50, -0.3, 0.3);

    // pull plots
    fHistPullElectronPt = new TH1F("hPullElecPt","Electron p_{T} Pull", 100, -3., 3.);
    fHistPullElectronPt->GetXaxis()->SetTitle("#frac{p_{T}^{fit}-p_{T}^{Start}}{#sigma_{p_{T}^{fit}-p_{T}^{Start}}}");
    fHistPullElectronPt->GetYaxis()->SetTitle("Einträge");
    fHistPullElectronPt->SetTitle("Elektron p_{T} pull Verteilung");

    fHistPullElectronEta = new TH1F("hPullElecEta","Electron #eta Pull", 100, -3., 3.);
    fHistPullElectronEta->GetXaxis()->SetTitle("#frac{#eta^{fit}-#eta^{Start}}{#sigma_{#eta^{fit}-#eta^{Start}}}");
    fHistPullElectronEta->GetYaxis()->SetTitle("Einträge");
    fHistPullElectronEta->SetTitle("Elektron #eta pull Verteilung");

    fHistPullElectronPhi = new TH1F("hPullElecPhi","Electron #varphi Pull", 100, -3., 3.);
    fHistPullElectronPhi->GetXaxis()->
	SetTitle("#frac{#varphi^{fit}-#varphi^{Start}}{#sigma_{#varphi^{fit}-#varphi^{Start}}");
    fHistPullElectronPhi->GetYaxis()->SetTitle("Einträge");
    fHistPullElectronPhi->SetTitle("Elektron #varphi pull Verteilung");

    fHistPullNeutrinoPt = new TH1F("hPullNuPt","Neutrino p_{T} Pull", 50, -3., 3.);
    fHistPullNeutrinoPhi = new TH1F("hPullNuPhi","Neutrino #varphi Pull", 50, -3., 3.);
    fHistPullBQuarkPt = new TH1F("hPullBQuarkPt","b Quark p_{T} Pull", 50, -3., 3.);
    fHistPullBQuarkEta = new TH1F("hPullBQuarkEta","b Quark #eta Pull", 50, -3., 3.);
    fHistPullBQuarkPhi = new TH1F("hPullBQuarkPhi","b Quark #varphi Pull", 50, -3., 3.);

}

//____________________________________________________________________

void TToyGentSgTop::GuessTopDecayNeutrinoEtaE(Double_t& EtaStart, Double_t& EStart) {
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

    const TLorentzVector p_lep = *(fElectron->getCurr4Vec());
    const TLorentzVector p_b   = *(fBQuark->getCurr4Vec());

    TLorentzVector *P_lepStar  = new TLorentzVector(p_lep);
    TLorentzVector *P_bjetStar = new TLorentzVector(p_b);

    TLorentzVector P_lep  = *P_lepStar;
    TLorentzVector P_bjet = *P_bjetStar;

    TLorentzVector P_sum = P_lep + P_bjet;
    TVector2 Et_miss;
    Et_miss.Set( (*(fNeutrino->getIni4Vec())).Px(), (*(fNeutrino->getIni4Vec())).Py() );
    Int_t nW   = TKinFitter::GetEtaNuRoot(P_lep, Et_miss, fWMass,
					  RootW1, RootW2, MinimumW);
    Int_t nTop = TKinFitter::GetEtaNuRoot(P_sum, Et_miss, fTopMass,
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
	}

	EtaStart = 0.5*(roots[0]+roots[1]);
    }

    // Estimate the neutrino energy
    EStart = Et_miss.Mod()*TMath::CosH(EtaStart);
}
