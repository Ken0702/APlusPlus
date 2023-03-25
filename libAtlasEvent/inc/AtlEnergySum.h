//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEnergySum
#define ATLAS_AtlEnergySum
#ifndef HEP_HepEnergySum
#include <HepEnergySum.h>
#endif
#ifndef ROOT_TVector2
#include <TVector2.h>
#endif
#ifndef ROOT_TMath
#include <TMath.h>
#endif

class TString;

class AtlEnergySum : public HepEnergySum {

  private:
    Double_t  fRefFinalSumEt;         // Scalar energy by RefFinal (GeV)
    TVector2 fRefFinalMissingEt;     // RefFinal missing Et (GeV)

    Double_t  fRefFinalEMSumEt;    // Scalar energy by RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt;// RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_Ele;    // Scalar energy by Ele of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_Ele;// Ele of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_Gamma;    // Scalar energy by Gamma of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_Gamma;// Gamma of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_Tau;    // Scalar energy by Tau of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_Tau;// Tau of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_Jet;    // Scalar energy by Jet of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_Jet;// Jet of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_SoftJets;    // Scalar energy by SoftJets of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_SoftJets;// SoftJets of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_MuonTrack;    // Scalar energy by MuonTrack of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_MuonTrack;// MuonTrack of RefFinal em missing Et (GeV),
    Double_t  fRefFinalEMSumEt_CellOut;    // Scalar energy by CellOut of RefFinal em (GeV)
    TVector2 fRefFinalEMMissingEt_CellOut;// CellOut of RefFinal em missing Et (GeV),
    
    Double_t fRefFinalEM_etight_SumEt; // Scalar energy by RefFinal em (GeV) with a tight electron and muid mu
    TVector2 fRefFinalEM_etight_MissingEt;// RefFinal em missing Et (GeV)
    Double_t fRefFinalEM_etight_SumEt_CellOut; // Scalar energy by CellOut of RefFinal em (GeV) with a tight electron and muid mu
    TVector2 fRefFinalEM_etight_MissingEt_CellOut; // CellOut of RefFinal em (GeV) with a tight electron and muid m
    Double_t fRefFinalEM_etight_SumEt_SoftJets; // Scalar energy by SoftJets of RefFinal em (GeV) with a tight electron and muid mu
    TVector2 fRefFinalEM_etight_MissingEt_SoftJets; // SoftJets of RefFinal em (GeV) with a tight electron and muid mu

    Double_t fRefFinalEM_etightpp_SumEt; // Scalar energy by RefFinal em (GeV) with a tight++ electron and muid mu
    TVector2 fRefFinalEM_etightpp_MissingEt;// RefFinal em missing Et (GeV) with tight++ electron
    
    Double_t fRefFinalEM_emedium_SumEt; // Scalar energy by RefFinal em (GeV) with a medium electron ad muid mu
    TVector2 fRefFinalEM_emedium_MissingEt;// RefFinal em missing Et (GeV),
    Double_t fRefFinalEM_emediumPP_SumEt; // Scalar energy by RefFinal em (GeV) with a medium electron ad muid mu
    TVector2 fRefFinalEM_emediumPP_MissingEt;// RefFinal em missing Et (GeV),
    
    
    Double_t fRefFinalEM_emedium_SumEt_CellOut; // Scalar energy by CellOut of RefFinal em (GeV) with a medium electron and muid mu
    TVector2 fRefFinalEM_emedium_MissingEt_CellOut; // CellOut of RefFinal em (GeV) with a medium electron and muid mu
    Double_t fRefFinalEM_emedium_SumEt_SoftJets; // Scalar energy by SoftJets of RefFinal em (GeV) with a medium electron and muid mu
    TVector2 fRefFinalEM_emedium_MissingEt_SoftJets; // SoftJets of RefFinal em (GeV) with a medium electron and muid m

    
    Double_t fRefFinalEM_eloose_SumEt; // Scalar energy by RefFinal em (GeV) with a loose electron and muid mu
    TVector2 fRefFinalEM_eloose_MissingEt;// RefFinal em missing Et (GeV),


    
/*     Double_t  fRefFinalEM_ewtm_sgtop_SumEt; // Scalar energy by RefFinal em (GeV), */
/*                                            // recalculated for electron with track match */
/*                                            // AND jets with pile-up correction removed */
/*     TVector2 fRefFinalEM_ewtm_sgtop_MissingEt;// RefFinal em missing Et (GeV), */
/*                                               // recalculated for electron with track match */
/*                                               // AND jets with pile-up correction removed */
    
    Double_t  fSimplifiedRefFinalSumEt;    // Scalar energy by Simplified RefFinal (GeV)
    TVector2 fSimplifiedRefFinalMissingEt;// Simplified RefFinal missing Et (GeV)
    
    Double_t  fRefCompositionAntiktEmjesSumEt;     // Scalar energy by RefComposition_antikt_emjes (GeV)
    TVector2 fRefCompositionAntiktEmjesMissingEt; // RefComposition_antikt_emjes missing Et (GeV)
    
    Double_t  fAtlFastSumEt;           // Scalar energy by ATLFAST (GeV)
    TVector2 fAtlFastMissingEt;       // ATLFAST missing Et (GeV)
    
    Double_t  fMCNonIntSumEt;          // Scalar energy by MC (GeV), all non-interacting particles
    TVector2 fMCNonIntMissingEt;      // MC missing Et (GeV)
    Double_t  fMCIntSumEt;             // Scalar energy sum by MC (GeV), all
                                      // interacting particles, full detector (|eta|<5)
    TVector2 fMCIntMissingEt;         // MC missing Et (GeV), all interacting
                                      // particles, full detector (|eta|<5)
    Double_t  fMCIntCentralSumEt;      // Scalar energy by MC (GeV), all
                                      // interacting particles, central region (|eta|<3.2)
    TVector2 fMCIntCentralMissingEt;  // MC missing Et (GeV), all
                                      // interacting particles, central region (|eta|<3.2)
    Double_t  fMCIntFwdSumEt;          // Scalar energy by MC (GeV), all
                                      // interacting particles, forward region (3.2<|eta|<5)
    TVector2 fMCIntFwdMissingEt;      // MC missing Et (GeV), all
                                      // interacting particles, forward region (3.2<|eta|<5)
    Double_t  fMCIntOutCoverSumEt;     // Scalar energy by MC (GeV), all
                                      // interacting particles, not-covered region (|eta|>5)
    TVector2 fMCIntOutCoverMissingEt; // MC missing Et (GeV), all
                                      // interacting particles, not-covered region (|eta|>5)
    Double_t  fMCMuonsSumEt;           // Scalar energy by MC (GeV), all muons
    TVector2 fMCMuonsMissingEt;       // MC missing Et (GeV), all muons

  public:
    AtlEnergySum();
    virtual ~AtlEnergySum();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;

    
    //
    // Reconstructed SumEt and Etmiss
    //
    
    // SumEt and Etmiss by RefFinal
    inline Double_t GetRefFinalSumEt() const { return fRefFinalSumEt; }
    inline const TVector2& GetRefFinalMissingEt() const
    { return fRefFinalMissingEt; }
    inline Double_t RefFinalMissingEt_Phi() const
    { return fRefFinalMissingEt.Phi(); }
    inline Double_t RefFinalMissingEt_Mag() const
    { return fRefFinalMissingEt.Mod(); }
    inline void SetRefFinalSumEt(Double_t RefFinalSumEt)
    { fRefFinalSumEt = RefFinalSumEt; }
    inline void SetRefFinalMissingEt(Double_t RefFinalMissingEx, 
				     Double_t RefFinalMissingEy)
    { fRefFinalMissingEt.Set(RefFinalMissingEx, RefFinalMissingEy); }
    
    // SumEt and Etmiss by RefFinal em
    inline Double_t GetRefFinalEMSumEt() const { return fRefFinalEMSumEt; }
    inline const TVector2& GetRefFinalEMMissingEt() const
    { return fRefFinalEMMissingEt; }
    inline Double_t RefFinalEMMissingEt_Phi() const
    { return fRefFinalEMMissingEt.Phi(); }
    inline Double_t RefFinalEMMissingEt_Mag() const
    { return fRefFinalEMMissingEt.Mod(); }
    inline void SetRefFinalEMSumEt(Double_t RefFinalEMSumEt)
    { fRefFinalEMSumEt = RefFinalEMSumEt; }
    inline void SetRefFinalEMMissingEt(Double_t RefFinalEMMissingEx, 
				     Double_t RefFinalEMMissingEy)
    { fRefFinalEMMissingEt.Set(RefFinalEMMissingEx, RefFinalEMMissingEy); }
    
    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_Ele() const { return fRefFinalEMSumEt_Ele; }
    inline const TVector2& GetRefFinalEMMissingEt_Ele() const
    { return fRefFinalEMMissingEt_Ele; }
    inline Double_t RefFinalEMMissingEt_Ele_Phi() const
    { return fRefFinalEMMissingEt_Ele.Phi(); }
    inline Double_t RefFinalEMMissingEt_Ele_Mag() const
    { return fRefFinalEMMissingEt_Ele.Mod(); }
    inline void SetRefFinalEMSumEt_Ele(Double_t RefFinalEMSumEt_Ele)
    { fRefFinalEMSumEt_Ele = RefFinalEMSumEt_Ele; }
    inline void SetRefFinalEMMissingEt_Ele(Double_t RefFinalEMMissingEx_Ele, 
				     Double_t RefFinalEMMissingEy_Ele)
    { fRefFinalEMMissingEt_Ele.Set(RefFinalEMMissingEx_Ele, RefFinalEMMissingEy_Ele); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_Gamma() const { return fRefFinalEMSumEt_Gamma; }
    inline const TVector2& GetRefFinalEMMissingEt_Gamma() const
    { return fRefFinalEMMissingEt_Gamma; }
    inline Double_t RefFinalEMMissingEt_Gamma_Phi() const
    { return fRefFinalEMMissingEt_Gamma.Phi(); }
    inline Double_t RefFinalEMMissingEt_Gamma_Mag() const
    { return fRefFinalEMMissingEt_Gamma.Mod(); }
    inline void SetRefFinalEMSumEt_Gamma(Double_t RefFinalEMSumEt_Gamma)
    { fRefFinalEMSumEt_Gamma = RefFinalEMSumEt_Gamma; }
    inline void SetRefFinalEMMissingEt_Gamma(Double_t RefFinalEMMissingEx_Gamma, 
				     Double_t RefFinalEMMissingEy_Gamma)
    { fRefFinalEMMissingEt_Gamma.Set(RefFinalEMMissingEx_Gamma, RefFinalEMMissingEy_Gamma); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_Tau() const { return fRefFinalEMSumEt_Tau; }
    inline const TVector2& GetRefFinalEMMissingEt_Tau() const
    { return fRefFinalEMMissingEt_Tau; }
    inline Double_t RefFinalEMMissingEt_Tau_Phi() const
    { return fRefFinalEMMissingEt_Tau.Phi(); }
    inline Double_t RefFinalEMMissingEt_Tau_Mag() const
    { return fRefFinalEMMissingEt_Tau.Mod(); }
    inline void SetRefFinalEMSumEt_Tau(Double_t RefFinalEMSumEt_Tau)
    { fRefFinalEMSumEt_Tau = RefFinalEMSumEt_Tau; }
    inline void SetRefFinalEMMissingEt_Tau(Double_t RefFinalEMMissingEx_Tau, 
				     Double_t RefFinalEMMissingEy_Tau)
    { fRefFinalEMMissingEt_Tau.Set(RefFinalEMMissingEx_Tau, RefFinalEMMissingEy_Tau); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_Jet() const { return fRefFinalEMSumEt_Jet; }
    inline const TVector2& GetRefFinalEMMissingEt_Jet() const
    { return fRefFinalEMMissingEt_Jet; }
    inline Double_t RefFinalEMMissingEt_Jet_Phi() const
    { return fRefFinalEMMissingEt_Jet.Phi(); }
    inline Double_t RefFinalEMMissingEt_Jet_Mag() const
    { return fRefFinalEMMissingEt_Jet.Mod(); }
    inline void SetRefFinalEMSumEt_Jet(Double_t RefFinalEMSumEt_Jet)
    { fRefFinalEMSumEt_Jet = RefFinalEMSumEt_Jet; }
    inline void SetRefFinalEMMissingEt_Jet(Double_t RefFinalEMMissingEx_Jet, 
				     Double_t RefFinalEMMissingEy_Jet)
    { fRefFinalEMMissingEt_Jet.Set(RefFinalEMMissingEx_Jet, RefFinalEMMissingEy_Jet); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_SoftJets() const { return fRefFinalEMSumEt_SoftJets; }
    inline const TVector2& GetRefFinalEMMissingEt_SoftJets() const
    { return fRefFinalEMMissingEt_SoftJets; }
    inline Double_t RefFinalEMMissingEt_SoftJets_Phi() const
    { return fRefFinalEMMissingEt_SoftJets.Phi(); }
    inline Double_t RefFinalEMMissingEt_SoftJets_Mag() const
    { return fRefFinalEMMissingEt_SoftJets.Mod(); }
    inline void SetRefFinalEMSumEt_SoftJets(Double_t RefFinalEMSumEt_SoftJets)
    { fRefFinalEMSumEt_SoftJets = RefFinalEMSumEt_SoftJets; }
    inline void SetRefFinalEMMissingEt_SoftJets(Double_t RefFinalEMMissingEx_SoftJets, 
				     Double_t RefFinalEMMissingEy_SoftJets)
    { fRefFinalEMMissingEt_SoftJets.Set(RefFinalEMMissingEx_SoftJets, RefFinalEMMissingEy_SoftJets); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_MuonTrack() const { return fRefFinalEMSumEt_MuonTrack; }
    inline const TVector2& GetRefFinalEMMissingEt_MuonTrack() const
    { return fRefFinalEMMissingEt_MuonTrack; }
    inline Double_t RefFinalEMMissingEt_MuonTrack_Phi() const
    { return fRefFinalEMMissingEt_MuonTrack.Phi(); }
    inline Double_t RefFinalEMMissingEt_MuonTrack_Mag() const
    { return fRefFinalEMMissingEt_MuonTrack.Mod(); }
    inline void SetRefFinalEMSumEt_MuonTrack(Double_t RefFinalEMSumEt_MuonTrack)
    { fRefFinalEMSumEt_MuonTrack = RefFinalEMSumEt_MuonTrack; }
    inline void SetRefFinalEMMissingEt_MuonTrack(Double_t RefFinalEMMissingEx_MuonTrack, 
				     Double_t RefFinalEMMissingEy_MuonTrack)
    { fRefFinalEMMissingEt_MuonTrack.Set(RefFinalEMMissingEx_MuonTrack, RefFinalEMMissingEy_MuonTrack); }

    // SumEt and Etmiss by Ele of RefFinal em
    inline Double_t GetRefFinalEMSumEt_CellOut() const { return fRefFinalEMSumEt_CellOut; }
    inline const TVector2& GetRefFinalEMMissingEt_CellOut() const
    { return fRefFinalEMMissingEt_CellOut; }
    inline Double_t RefFinalEMMissingEt_CellOut_Phi() const
    { return fRefFinalEMMissingEt_CellOut.Phi(); }
    inline Double_t RefFinalEMMissingEt_CellOut_Mag() const
    { return fRefFinalEMMissingEt_CellOut.Mod(); }
    inline void SetRefFinalEMSumEt_CellOut(Double_t RefFinalEMSumEt_CellOut)
    { fRefFinalEMSumEt_CellOut = RefFinalEMSumEt_CellOut; }
    inline void SetRefFinalEMMissingEt_CellOut(Double_t RefFinalEMMissingEx_CellOut, 
					       Double_t RefFinalEMMissingEy_CellOut)
    { fRefFinalEMMissingEt_CellOut.Set(RefFinalEMMissingEx_CellOut, RefFinalEMMissingEy_CellOut); }

    //
    // SumEt and Etmiss by RefFinal em with tight electron and muid muon
    //
    inline Double_t GetRefFinalEM_etight_SumEt() const { return fRefFinalEM_etight_SumEt; }
    inline const TVector2& GetRefFinalEM_etight_MissingEt() const
    { return fRefFinalEM_etight_MissingEt; }
    inline Double_t RefFinalEM_etight_MissingEt_Phi() const
    { return fRefFinalEM_etight_MissingEt.Phi(); }
    inline Double_t RefFinalEM_etight_MissingEt_Mag() const
    { return fRefFinalEM_etight_MissingEt.Mod(); }
    inline void SetRefFinalEM_etight_SumEt(Double_t RefFinalEM_etight_SumEt)
    { fRefFinalEM_etight_SumEt = RefFinalEM_etight_SumEt; }
    inline void SetRefFinalEM_etight_MissingEt(Double_t RefFinalEM_etight_MissingEx, 
				     Double_t RefFinalEM_etight_MissingEy)
    { fRefFinalEM_etight_MissingEt.Set(RefFinalEM_etight_MissingEx, RefFinalEM_etight_MissingEy); }

    // CellOut term
    inline Double_t GetRefFinalEM_etight_SumEt_CellOut() const { return fRefFinalEM_etight_SumEt_CellOut; }
    inline const TVector2& GetRefFinalEM_etight_MissingEt_CellOut() const
    { return fRefFinalEM_etight_MissingEt_CellOut; }
    inline Double_t RefFinalEM_etight_MissingEt_CellOut_Phi() const
    { return fRefFinalEM_etight_MissingEt_CellOut.Phi(); }
    inline Double_t RefFinalEM_etight_MissingEt_CellOut_Mag() const
    { return fRefFinalEM_etight_MissingEt_CellOut.Mod(); }
    inline void SetRefFinalEM_etight_SumEt_CellOut(Double_t RefFinalEM_etight_SumEt_CellOut)
    { fRefFinalEM_etight_SumEt_CellOut = RefFinalEM_etight_SumEt_CellOut; }
    inline void SetRefFinalEM_etight_MissingEt_CellOut(Double_t RefFinalEM_etight_MissingEx_CellOut, 
						       Double_t RefFinalEM_etight_MissingEy_CellOut)
    { fRefFinalEM_etight_MissingEt_CellOut.Set(RefFinalEM_etight_MissingEx_CellOut, RefFinalEM_etight_MissingEy_CellOut); }

    // SoftJets term
    inline Double_t GetRefFinalEM_etight_SumEt_SoftJets() const { return fRefFinalEM_etight_SumEt_SoftJets; }
    inline const TVector2& GetRefFinalEM_etight_MissingEt_SoftJets() const
    { return fRefFinalEM_etight_MissingEt_SoftJets; }
    inline Double_t RefFinalEM_etight_MissingEt_SoftJets_Phi() const
    { return fRefFinalEM_etight_MissingEt_SoftJets.Phi(); }
    inline Double_t RefFinalEM_etight_MissingEt_SoftJets_Mag() const
    { return fRefFinalEM_etight_MissingEt_SoftJets.Mod(); }
    inline void SetRefFinalEM_etight_SumEt_SoftJets(Double_t RefFinalEM_etight_SumEt_SoftJets)
    { fRefFinalEM_etight_SumEt_SoftJets = RefFinalEM_etight_SumEt_SoftJets; }
    inline void SetRefFinalEM_etight_MissingEt_SoftJets(Double_t RefFinalEM_etight_MissingEx_SoftJets, 
						       Double_t RefFinalEM_etight_MissingEy_SoftJets)
    { fRefFinalEM_etight_MissingEt_SoftJets.Set(RefFinalEM_etight_MissingEx_SoftJets, RefFinalEM_etight_MissingEy_SoftJets); }



    //
    // SumEt and Etmiss by RefFinal em with tight++ electron and muid muon
    //
    inline Double_t GetRefFinalEM_etightpp_SumEt() const { return fRefFinalEM_etightpp_SumEt; }
    inline const TVector2& GetRefFinalEM_etightpp_MissingEt() const
    { return fRefFinalEM_etightpp_MissingEt; }
    inline Double_t RefFinalEM_etightpp_MissingEt_Phi() const
    { return fRefFinalEM_etightpp_MissingEt.Phi(); }
    inline Double_t RefFinalEM_etightpp_MissingEt_Mag() const
    { return fRefFinalEM_etightpp_MissingEt.Mod(); }
    inline void SetRefFinalEM_etightpp_SumEt(Double_t RefFinalEM_etightpp_SumEt)
    { fRefFinalEM_etightpp_SumEt = RefFinalEM_etightpp_SumEt; }
    inline void SetRefFinalEM_etightpp_MissingEt(Double_t RefFinalEM_etightpp_MissingEx, 
						 Double_t RefFinalEM_etightpp_MissingEy)
    { fRefFinalEM_etightpp_MissingEt.Set(RefFinalEM_etightpp_MissingEx, RefFinalEM_etightpp_MissingEy); }

    
    
    
    //
    // SumEt and Etmiss by RefFinal em with medium electron and muid muon
    //
    inline Double_t GetRefFinalEM_emedium_SumEt() const { return fRefFinalEM_emedium_SumEt; }
    inline const TVector2& GetRefFinalEM_emedium_MissingEt() const
    { return fRefFinalEM_emedium_MissingEt; }
    inline Double_t RefFinalEM_emedium_MissingEt_Phi() const
    { return fRefFinalEM_emedium_MissingEt.Phi(); }
    inline Double_t RefFinalEM_emedium_MissingEt_Mag() const
    { return fRefFinalEM_emedium_MissingEt.Mod(); }
    inline void SetRefFinalEM_emedium_SumEt(Double_t RefFinalEM_emedium_SumEt)
    { fRefFinalEM_emedium_SumEt = RefFinalEM_emedium_SumEt; }
    inline void SetRefFinalEM_emedium_MissingEt(Double_t RefFinalEM_emedium_MissingEx, 
						Double_t RefFinalEM_emedium_MissingEy)
    { fRefFinalEM_emedium_MissingEt.Set(RefFinalEM_emedium_MissingEx, RefFinalEM_emedium_MissingEy); }

    
    inline Double_t GetRefFinalEM_emediumPP_SumEt() const { return fRefFinalEM_emediumPP_SumEt; }
    inline const TVector2& GetRefFinalEM_emediumPP_MissingEt() const
    { return fRefFinalEM_emediumPP_MissingEt; }    
    inline Double_t RefFinalEM_emediumPP_MissingEt_Phi() const
    { return fRefFinalEM_emediumPP_MissingEt.Phi(); }
    inline Double_t RefFinalEM_emediumPP_MissingEt_Mag() const
    { return fRefFinalEM_emediumPP_MissingEt.Mod(); }
    inline void SetRefFinalEM_emediumPP_SumEt(Double_t RefFinalEM_emediumPP_SumEt)
    { fRefFinalEM_emediumPP_SumEt = RefFinalEM_emediumPP_SumEt; }
    inline void SetRefFinalEM_emediumPP_MissingEt(Double_t RefFinalEM_emediumPP_MissingEx, 
						Double_t RefFinalEM_emediumPP_MissingEy)
    { fRefFinalEM_emediumPP_MissingEt.Set(RefFinalEM_emediumPP_MissingEx, RefFinalEM_emediumPP_MissingEy); }


    // CellOut term
    inline Double_t GetRefFinalEM_emedium_SumEt_CellOut() const { return fRefFinalEM_emedium_SumEt_CellOut; }
    inline const TVector2& GetRefFinalEM_emedium_MissingEt_CellOut() const
    { return fRefFinalEM_emedium_MissingEt_CellOut; }
    inline Double_t RefFinalEM_emedium_MissingEt_CellOut_Phi() const
    { return fRefFinalEM_emedium_MissingEt_CellOut.Phi(); }
    inline Double_t RefFinalEM_emedium_MissingEt_CellOut_Mag() const
    { return fRefFinalEM_emedium_MissingEt_CellOut.Mod(); }
    inline void SetRefFinalEM_emedium_SumEt_CellOut(Double_t RefFinalEM_emedium_SumEt_CellOut)
    { fRefFinalEM_emedium_SumEt_CellOut = RefFinalEM_emedium_SumEt_CellOut; }
    inline void SetRefFinalEM_emedium_MissingEt_CellOut(Double_t RefFinalEM_emedium_MissingEx_CellOut, 
							Double_t RefFinalEM_emedium_MissingEy_CellOut)
    { fRefFinalEM_emedium_MissingEt_CellOut.Set(RefFinalEM_emedium_MissingEx_CellOut, RefFinalEM_emedium_MissingEy_CellOut); }

    // SoftJets term
    inline Double_t GetRefFinalEM_emedium_SumEt_SoftJets() const { return fRefFinalEM_emedium_SumEt_SoftJets; }
    inline const TVector2& GetRefFinalEM_emedium_MissingEt_SoftJets() const
    { return fRefFinalEM_emedium_MissingEt_SoftJets; }
    inline Double_t RefFinalEM_emedium_MissingEt_SoftJets_Phi() const
    { return fRefFinalEM_emedium_MissingEt_SoftJets.Phi(); }
    inline Double_t RefFinalEM_emedium_MissingEt_SoftJets_Mag() const
    { return fRefFinalEM_emedium_MissingEt_SoftJets.Mod(); }
    inline void SetRefFinalEM_emedium_SumEt_SoftJets(Double_t RefFinalEM_emedium_SumEt_SoftJets)
    { fRefFinalEM_emedium_SumEt_SoftJets = RefFinalEM_emedium_SumEt_SoftJets; }
    inline void SetRefFinalEM_emedium_MissingEt_SoftJets(Double_t RefFinalEM_emedium_MissingEx_SoftJets, 
						       Double_t RefFinalEM_emedium_MissingEy_SoftJets)
    { fRefFinalEM_emedium_MissingEt_SoftJets.Set(RefFinalEM_emedium_MissingEx_SoftJets, RefFinalEM_emedium_MissingEy_SoftJets); }

    
    //
    // SumEt and Etmiss by RefFinal em with loose electron and muid muon
    //
    inline Double_t GetRefFinalEM_eloose_SumEt() const { return fRefFinalEM_eloose_SumEt; }
    inline const TVector2& GetRefFinalEM_eloose_MissingEt() const
    { return fRefFinalEM_eloose_MissingEt; }
    inline Double_t RefFinalEM_eloose_MissingEt_Phi() const
    { return fRefFinalEM_eloose_MissingEt.Phi(); }
    inline Double_t RefFinalEM_eloose_MissingEt_Mag() const
    { return fRefFinalEM_eloose_MissingEt.Mod(); }
    inline void SetRefFinalEM_eloose_SumEt(Double_t RefFinalEM_eloose_SumEt)
    { fRefFinalEM_eloose_SumEt = RefFinalEM_eloose_SumEt; }
    inline void SetRefFinalEM_eloose_MissingEt(Double_t RefFinalEM_eloose_MissingEx, 
				     Double_t RefFinalEM_eloose_MissingEy)
    { fRefFinalEM_eloose_MissingEt.Set(RefFinalEM_eloose_MissingEx, RefFinalEM_eloose_MissingEy); }
    
/*     // SumEt and Etmiss by RefFinal em, */
/*     // recalculated for electron with track match */
/*     // AND jets with pile-up correction removed     */
/*     inline Double_t GetRefFinalEM_ewtm_sgtop_SumEt() const { return fRefFinalEM_ewtm_sgtop_SumEt; } */
/*     inline const TVector2& GetRefFinalEM_ewtm_sgtop_MissingEt() const */
/*     { return fRefFinalEM_ewtm_sgtop_MissingEt; } */
/*     inline Double_t RefFinalEM_ewtm_sgtop_MissingEt_Phi() const */
/*     { return fRefFinalEM_ewtm_sgtop_MissingEt.Phi(); } */
/*     inline Double_t RefFinalEM_ewtm_sgtop_MissingEt_Mag() const */
/*     { return fRefFinalEM_ewtm_sgtop_MissingEt.Mod(); } */
/*     inline void SetRefFinalEM_ewtm_sgtop_SumEt(Double_t RefFinalEM_ewtm_sgtop_SumEt) */
/*     { fRefFinalEM_ewtm_sgtop_SumEt = RefFinalEM_ewtm_sgtop_SumEt; } */
/*     inline void SetRefFinalEM_ewtm_sgtop_MissingEt(Double_t RefFinalEM_ewtm_sgtop_MissingEx,  */
/* 				     Double_t RefFinalEM_ewtm_sgtop_MissingEy) */
/*     { fRefFinalEM_ewtm_sgtop_MissingEt.Set(RefFinalEM_ewtm_sgtop_MissingEx, RefFinalEM_ewtm_sgtop_MissingEy); } */

    
    // Simplified SumEt and Etmiss by RefFinal    
    inline Double_t GetSimplifiedRefFinalSumEt() const { return fSimplifiedRefFinalSumEt; }
    inline const TVector2& GetSimplifiedRefFinalMissingEt() const
    { return fSimplifiedRefFinalMissingEt; }
    inline Double_t SimplifiedRefFinalMissingEt_Phi() const
    { return fSimplifiedRefFinalMissingEt.Phi(); }
    inline Double_t SimplifiedRefFinalMissingEt_Mag() const
    { return fSimplifiedRefFinalMissingEt.Mod(); }
    inline void SetSimplifiedRefFinalSumEt(Double_t SimplifiedRefFinalSumEt)
    { fSimplifiedRefFinalSumEt = SimplifiedRefFinalSumEt; }
    inline void SetSimplifiedRefFinalMissingEt(Double_t SimplifiedRefFinalMissingEx, 
						      Double_t SimplifiedRefFinalMissingEy)
    { fSimplifiedRefFinalMissingEt.Set(SimplifiedRefFinalMissingEx, SimplifiedRefFinalMissingEy); }

    // SumEt and Etmiss by RefCompositionAntiKtEMJES    
    inline Double_t GetRefCompositionAntiktEmjesSumEt() const { return fRefCompositionAntiktEmjesSumEt; }
    inline const TVector2& GetRefCompositionAntiktEmjesMissingEt() const
    { return fRefCompositionAntiktEmjesMissingEt; }
    inline Double_t RefCompositionAntiktEmjesMissingEt_Phi() const
    { return fRefCompositionAntiktEmjesMissingEt.Phi(); }
    inline Double_t RefCompositionAntiktEmjesMissingEt_Mag() const
    { return fRefCompositionAntiktEmjesMissingEt.Mod(); }
    inline void SetRefCompositionAntiktEmjesSumEt(Double_t RefCompositionAntiktEmjesSumEt)
    { fRefCompositionAntiktEmjesSumEt = RefCompositionAntiktEmjesSumEt; }
    inline void SetRefCompositionAntiktEmjesMissingEt(Double_t RefCompositionAntiktEmjesMissingEx, 
				     Double_t RefCompositionAntiktEmjesMissingEy)
    { fRefCompositionAntiktEmjesMissingEt.Set(RefCompositionAntiktEmjesMissingEx, RefCompositionAntiktEmjesMissingEy); }

    // AtlFast SumEt and Etmiss
    inline Double_t GetAtlFastSumEt() const { return fAtlFastSumEt; }
    inline const TVector2& GetAtlFastMissingEt() const
    { return fAtlFastMissingEt; }
    inline Double_t AtlFastMissingEt_Phi() const
    { return fAtlFastMissingEt.Phi(); }
    inline Double_t AtlFastMissingEt_Mag() const
    { return fAtlFastMissingEt.Mod(); }
    inline void SetAtlFastSumEt(Double_t AtlFastSumEt)
    { fAtlFastSumEt = AtlFastSumEt; }
    inline void SetAtlFastMissingEt(Double_t AtlFastMissingEx,
				    Double_t AtlFastMissingEy)
    { fAtlFastMissingEt.Set(AtlFastMissingEx, AtlFastMissingEy); }


    //
    // MC SumEt and Etmiss
    //
    inline Double_t GetMCIntSumEt() const { return fMCIntSumEt; }
    inline const TVector2& GetMCIntMissingEt() const
    { return fMCIntMissingEt; }
    inline Double_t MCIntMissingEt_Phi() const
    { return fMCIntMissingEt.Phi(); }
    inline Double_t MCIntMissingEt_Mag() const
    { return fMCIntMissingEt.Mod(); }
    inline void SetMCIntSumEt(Double_t MCIntSumEt)
    { fMCIntSumEt = MCIntSumEt; }
    inline void SetMCIntMissingEt(Double_t MCIntMissingEx,
			       Double_t MCIntMissingEy)
    { fMCIntMissingEt.Set(MCIntMissingEx, MCIntMissingEy); }

    inline Double_t GetMCNonIntSumEt() const { return fMCNonIntSumEt; }
    inline const TVector2& GetMCNonIntMissingEt() const
    { return fMCNonIntMissingEt; }
    inline Double_t MCNonIntMissingEt_Phi() const
    { return TVector2::Phi_mpi_pi(fMCNonIntMissingEt.Phi()); }
    inline Double_t MCNonIntMissingEt_Mag() const
    { return fMCNonIntMissingEt.Mod(); }
    inline void SetMCNonIntSumEt(Double_t MCNonIntSumEt)
    { fMCNonIntSumEt = MCNonIntSumEt; }
    inline void SetMCNonIntMissingEt(Double_t MCNonIntMissingEx,
			       Double_t MCNonIntMissingEy)
    { fMCNonIntMissingEt.Set(MCNonIntMissingEx, MCNonIntMissingEy); }

    inline Double_t GetMCIntCentralSumEt() const { return fMCIntCentralSumEt; }
    inline const TVector2& GetMCIntCentralMissingEt() const
    { return fMCIntCentralMissingEt; }
    inline Double_t MCIntCentralMissingEt_Phi() const
    { return fMCIntCentralMissingEt.Phi(); }
    inline Double_t MCIntCentralMissingEt_Mag() const
    { return fMCIntCentralMissingEt.Mod(); }
    inline void SetMCIntCentralSumEt(Double_t MCIntCentralSumEt)
    { fMCIntCentralSumEt = MCIntCentralSumEt; }
    inline void SetMCIntCentralMissingEt(Double_t MCIntCentralMissingEx,
			       Double_t MCIntCentralMissingEy)
    { fMCIntCentralMissingEt.Set(MCIntCentralMissingEx, MCIntCentralMissingEy); }

    inline Double_t GetMCIntFwdSumEt() const { return fMCIntFwdSumEt; }
    inline const TVector2& GetMCIntFwdMissingEt() const
    { return fMCIntFwdMissingEt; }
    inline Double_t MCIntFwdMissingEt_Phi() const
    { return fMCIntFwdMissingEt.Phi(); }
    inline Double_t MCIntFwdMissingEt_Mag() const
    { return fMCIntFwdMissingEt.Mod(); }
    inline void SetMCIntFwdSumEt(Double_t MCIntFwdSumEt)
    { fMCIntFwdSumEt = MCIntFwdSumEt; }
    inline void SetMCIntFwdMissingEt(Double_t MCIntFwdMissingEx,
			       Double_t MCIntFwdMissingEy)
    { fMCIntFwdMissingEt.Set(MCIntFwdMissingEx, MCIntFwdMissingEy); }

    inline Double_t GetMCIntOutCoverSumEt() const { return fMCIntOutCoverSumEt; }
    inline const TVector2& GetMCIntOutCoverMissingEt() const
    { return fMCIntOutCoverMissingEt; }
    inline Double_t MCIntOutCoverMissingEt_Phi() const
    { return fMCIntOutCoverMissingEt.Phi(); }
    inline Double_t MCIntOutCoverMissingEt_Mag() const
    { return fMCIntOutCoverMissingEt.Mod(); }
    inline void SetMCIntOutCoverSumEt(Double_t MCIntOutCoverSumEt)
    { fMCIntOutCoverSumEt = MCIntOutCoverSumEt; }
    inline void SetMCIntOutCoverMissingEt(Double_t MCIntOutCoverMissingEx,
			       Double_t MCIntOutCoverMissingEy)
    { fMCIntOutCoverMissingEt.Set(MCIntOutCoverMissingEx, MCIntOutCoverMissingEy); }
    inline Double_t GetMCMuonsSumEt() const { return fMCMuonsSumEt; }
    inline const TVector2& GetMCMuonsMissingEt() const
    { return fMCMuonsMissingEt; }
    inline Double_t MCMuonsMissingEt_Phi() const
    { return fMCMuonsMissingEt.Phi(); }
    inline Double_t MCMuonsMissingEt_Mag() const
    { return fMCMuonsMissingEt.Mod(); }
    inline void SetMCMuonsSumEt(Double_t MCMuonsSumEt)
    { fMCMuonsSumEt = MCMuonsSumEt; }
    inline void SetMCMuonsMissingEt(Double_t MCMuonsMissingEx,
			       Double_t MCMuonsMissingEy)
    { fMCMuonsMissingEt.Set(MCMuonsMissingEx, MCMuonsMissingEy); }

    ClassDef(AtlEnergySum,18) // ATLAS energy sums
};
#endif

