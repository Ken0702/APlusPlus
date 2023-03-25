//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Felix Thomas
//
#ifndef ATLAS_AtlTopPairDocumenter
#define ATLAS_AtlTopPairDocumenter
#ifndef ATLAS_AtlDocumenter
#include <AtlDocumenter.h>
#endif
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#ifndef ROOT_TVector2
#include <TVector2.h>
#endif
#ifndef ROOT_TObject
#include <TObject.h>
#endif



class AtlTopPairDocumenter : public AtlDocumenter {

    public:
      static const Int_t fgNSides    = 3;         // Number of sides (inclusive, leptonic, hadronic)
      static const Int_t fgNOrigins  = 2;         // calculated variables are either from fitted or original objects

    private:
      static const char* fgSideNames[];           // Names of sides
      static const char* fgHistSideNames[];       // Names of sides in histograms
      static const char* fgOriginNames[];         // Names of origins (see above fgNOrigins)
      static const char* fgHistOriginNames[];     // Names of origins in histograms (see above fgNOrigins)

      enum ESide { kInclusive = 0, kLeptonic, kHadronic};
      enum EOrigin { kOriginal = 0, kFitted};

    AtlTopPair* fTopPair;
    Float_t  fEtMissMag;  
    Float_t  fEtMissPhi;  
    
  // W reconstruction histograms
    TH1F *fHistWreco_M[fgNSides][fgNOrigins];           // Reconstructed invariant mass of the W
    TH1F *fHistWreco_phi[fgNSides][fgNOrigins];         // Reconstructed azimuth angle of the W 
    TH1F *fHistWreco_eta[fgNSides][fgNOrigins];         // Reconstructed Eta of the W
    TH1F *fHistWreco_pt[fgNSides][fgNOrigins];          // Reconstructed Pt of the W

  // top flavour reconstruction histograms
    TH1F *fHistTopReco_M[fgNSides][fgNOrigins];         // Reconstructed invariant transverse mass of the tops
    TH1F *fHistTopReco_phi[fgNSides][fgNOrigins];       // Reconstructed azimuth angle of the tops
    TH1F *fHistTopReco_eta[fgNSides][fgNOrigins];       // Reconstructed Eta of the tops
    TH1F *fHistTopReco_pt[fgNSides][fgNOrigins];        // Reconstructed Pt of the tops
                                   
    TH1F *fHistChi2[fgNSides];                          // Chi2 of the kinematic fits
    TH1F *fHistChi2Prob[fgNSides];                      // Chi2Prob of the kinematic fits
                                   
    TH2F *fHistTopMvsChi2[fgNSides][fgNOrigins];        // Mass of reconstructed top vs. Chi2
    TH2F *fHistTopMvsChi2prob[fgNSides][fgNOrigins];    // Mass of reconstructed top vs. Chi2prob
                                   
    TH2F *fHistTopPtvsChi2[fgNSides][fgNOrigins];       // Mass of reconstructed top vs. Chi2
    TH2F *fHistTopPtvsChi2prob[fgNSides][fgNOrigins];   // Mass of reconstructed top vs. Chi2prob
    
  // Leptonic decays
    TH1F *fHistLepNuReco_phi;
    TH1F *fHistLepNuReco_pt;
    TH1F *fHistLepNuReco_eta;
 
  // Hadronic decays

  // Pull distributions
    TH1F *fHistResidualLepton_pt;        // alteration of lepton pt by fit
    TH1F *fHistResidualLepton_eta;       // alteration of lepton eta by fit
    TH1F *fHistResidualLepton_phi;       // alteration of lepton phi by fit
    TH1F *fHistResidualLepBJet_Et;       // alteration of lep bjet Et by fit
    TH1F *fHistResidualLepBJet_pt;       // alteration of lep bjet pt by fit
    TH1F *fHistResidualLepBJet_eta;      // alteration of lep bjet eta by fit
    TH1F *fHistResidualLepBJet_phi;      // alteration of lep bjet phi by fit
    TH1F *fHistResidualEtmiss_Mag;       // alteration of met et by fit
    TH1F *fHistResidualEtmiss_phi;       // alteration of met phi by fit
                                      
    TH1F *fHistResidualHadBJet_Et;       // alteration of had bjet Et by fit
    TH1F *fHistResidualHadBJet_pt;       // alteration of had bjet pt by fit
    TH1F *fHistResidualHadBJet_eta;      // alteration of had bjet eta by fit
    TH1F *fHistResidualHadBJet_phi;      // alteration of had bjet phi by fit
    TH1F *fHistResidualHadJet1_Et;       // alteration of had jet1 Et by fit
    TH1F *fHistResidualHadJet1_pt;       // alteration of had jet1 pt by fit
    TH1F *fHistResidualHadJet1_eta;      // alteration of had jet1 eta by fit
    TH1F *fHistResidualHadJet1_phi;      // alteration of had jet1 phi by fit
    TH1F *fHistResidualHadJet2_Et;       // alteration of had jet2 Et by fit
    TH1F *fHistResidualHadJet2_pt;       // alteration of had jet2 pt by fit
    TH1F *fHistResidualHadJet2_eta;      // alteration of had jet2 eta by fit
    TH1F *fHistResidualHadJet2_phi;      // alteration of had jet2 phi by fit

    TH1F *fHistPullLepton_pt;        // pull of lepton pt by fit
    TH1F *fHistPullLepton_eta;       // pull of lepton eta by fit
    TH1F *fHistPullLepton_phi;       // pull of lepton phi by fit
    TH1F *fHistPullLepBJet_pt;       // pull of lep bjet pt by fit
    TH1F *fHistPullLepBJet_eta;      // pull of lep bjet eta by fit
    TH1F *fHistPullLepBJet_phi;      // pull of lep bjet phi by fit
    TH1F *fHistPullEtmiss_Mag;       // pull of met et by fit
    TH1F *fHistPullEtmiss_phi;       // pull of met phi by fit
                                      

    TH1F *fHistPullHadBJet_pt;       // pull of had bjet pt by fit
    TH1F *fHistPullHadBJet_eta;      // pull of had bjet eta by fit
    TH1F *fHistPullHadBJet_phi;      // pull of had bjet phi by fit
    TH1F *fHistPullHadJet1_pt;       // pull of had jet1 pt by fit
    TH1F *fHistPullHadJet1_eta;      // pull of had jet1 eta by fit
    TH1F *fHistPullHadJet1_phi;      // pull of had jet1 phi by fit
    TH1F *fHistPullHadJet2_pt;       // pull of had jet2 pt by fit
    TH1F *fHistPullHadJet2_eta;      // pull of had jet2 eta by fit
    TH1F *fHistPullHadJet2_phi;      // pull of had jet2 phi by fit


  public:
    AtlTopPairDocumenter(const char* name, const char* title, TFile* Output, const char* path);
    virtual ~AtlTopPairDocumenter();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void Clear() {;}
    virtual void Terminate() {;}
    virtual void Print() const {;}
    void Document(AtlTopPair *toppair, TVector2 *etmiss, Double_t weight);
    
  private:
    void BookHadHistograms();
    void BookLepHistograms();
    void BookResidualHistograms();
    void BookPullHistograms();  
    void FillHadHistograms();
    void FillLepHistograms();
    void FillResidualHistograms();
    void FillPullHistograms();
    
    ClassDef(AtlTopPairDocumenter,0) // Abstract base class for ATLAS histogramming tools
};
#endif

