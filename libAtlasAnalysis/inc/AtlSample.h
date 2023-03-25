//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlSample
#define ATLAS_AtlSample
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#include <TObjArray.h>
#include <TObjString.h>

class TString;
class TClass;
class TList;
class TSystem;

class AtlSample : public TNamed {

  public:
    enum EType {

	kDATA_EGamma,         // DATA Egamma stream
	kDATA_Muon,           // DATA Muon stream
	kDATA_TOPQ1,          // DATA TOPQ1 derivation (lepton+jets)
	kDATA_TOPQ2,          // DATA TOPQ2 derivation (multi-lepton)
	kDATA_TOPQ3,          // DATA TOPQ3 derivation (lepton+fatjets)
       ktZq,
	   k3tops,
	   k4tops,
	   kttH0l,
	   kttH1l,
	   kttH2l,
	   kttW,
	   kttZnunu,
	   kttZqq,
	   kttee,
	   kttmumu,
	   ktttautau,
	   kttee_mll,
	   kttmumu_mll,
	   ktttautau_mll,
	kSgTop_tChannel_e,    // Single-top t-channel, e+nu
	kSgTop_tChannel_mu,   // Single-top t-channel, mu+nu
	kSgTop_tChannel_tau,  // Single-top t-channel, tau+nu
	kSgTop_sChannel_e,    // Single-top s-channel, e+nu
	kSgTop_sChannel_mu,   // Single-top s-channel, mu+nu
	kSgTop_sChannel_tau,  // Single-top s-channel, tau+nu
	kSgTop_tChannel_incl, // Single-top t-channel, inclusive
	kSgTop_tChannel_top_incl,     // Single-top t-channel top, inclusive
	kSgTop_tChannel_antitop_incl, // Single-top t-channel antitop, inclusive
	kSgTop_sChannel_incl, // Single-top s-channel, inclusive
	kSgTop_Wt,            // Single-top Wt production (inclusive)
	kSgTop_tZj,           // Single-top production in association with Z boson (t-channel)
	kTtbar,               // Top -pair production
	kTtbarPlusW,          // Top-quark pair production in association with W boson
	kTtbarPlusZ,          // Top-quark pair production in association with Z boson
	kWenuNp0,             // W+jets, light-flavour Np0, e+nu
	kWenuNp0_hfor0,       // W+jets, light-flavour Np0 HFOR 0, e+nu
	kWenuNp0_hfor1,       // W+jets, light-flavour Np0 HFOR 1, e+nu
	kWenuNp0_hfor2,       // W+jets, light-flavour Np0 HFOR 2, e+nu
	kWenuNp0_hfor3,       // W+jets, light-flavour Np0 HFOR 3, e+nu
	kWenuNp1,             // W+jets, light-flavour Np1, e+nu
	kWenuNp1_hfor0,       // W+jets, light-flavour Np1 HFOR 0, e+nu
	kWenuNp1_hfor1,       // W+jets, light-flavour Np1 HFOR 1, e+nu
	kWenuNp1_hfor2,       // W+jets, light-flavour Np1 HFOR 2, e+nu
	kWenuNp1_hfor3,       // W+jets, light-flavour Np1 HFOR 3, e+nu
	kWenuNp2,             // W+jets, light-flavour Np2, e+nu
	kWenuNp2_hfor0,       // W+jets, light-flavour Np2 HFOR 0, e+nu
	kWenuNp2_hfor1,       // W+jets, light-flavour Np2 HFOR 1, e+nu
	kWenuNp2_hfor2,       // W+jets, light-flavour Np2 HFOR 2, e+nu
	kWenuNp2_hfor3,       // W+jets, light-flavour Np2 HFOR 3, e+nu
	kWenuNp3,             // W+jets, light-flavour Np3, e+nu
	kWenuNp3_hfor0,       // W+jets, light-flavour Np3 HFOR 0, e+nu
	kWenuNp3_hfor1,       // W+jets, light-flavour Np3 HFOR 1, e+nu
	kWenuNp3_hfor2,       // W+jets, light-flavour Np3 HFOR 2, e+nu
	kWenuNp3_hfor3,       // W+jets, light-flavour Np3 HFOR 3, e+nu
	kWenuNp4,             // W+jets, light-flavour Np4, e+nu
	kWenuNp4_hfor0,       // W+jets, light-flavour Np4 HFOR 0, e+nu
	kWenuNp4_hfor1,       // W+jets, light-flavour Np4 HFOR 1, e+nu
	kWenuNp4_hfor2,       // W+jets, light-flavour Np4 HFOR 2, e+nu
	kWenuNp4_hfor3,       // W+jets, light-flavour Np4 HFOR 3, e+nu
	kWenuNp5,             // W+jets, light-flavour Np5, e+nu
	kWenuNp5_hfor0,       // W+jets, light-flavour Np5 HFOR 0, e+nu
	kWenuNp5_hfor1,       // W+jets, light-flavour Np5 HFOR 1, e+nu
	kWenuNp5_hfor2,       // W+jets, light-flavour Np5 HFOR 2, e+nu
	kWenuNp5_hfor3,       // W+jets, light-flavour Np5 HFOR 3, e+nu
	kWmunuNp0,            // W+jets, light-flavour Np0, mu+nu
	kWmunuNp0_hfor0,      // W+jets, light-flavour Np0 HFOR 0, mu+nu
	kWmunuNp0_hfor1,      // W+jets, light-flavour Np0 HFOR 1, mu+nu
	kWmunuNp0_hfor2,      // W+jets, light-flavour Np0 HFOR 2, mu+nu
	kWmunuNp0_hfor3,      // W+jets, light-flavour Np0 HFOR 3, mu+nu
	kWmunuNp1,            // W+jets, light-flavour Np1, mu+nu
	kWmunuNp1_hfor0,      // W+jets, light-flavour Np1 HFOR 0, mu+nu
	kWmunuNp1_hfor1,      // W+jets, light-flavour Np1 HFOR 1, mu+nu
	kWmunuNp1_hfor2,      // W+jets, light-flavour Np1 HFOR 2, mu+nu
	kWmunuNp1_hfor3,      // W+jets, light-flavour Np1 HFOR 3, mu+nu
	kWmunuNp2,            // W+jets, light-flavour Np2, mu+nu
	kWmunuNp2_hfor0,      // W+jets, light-flavour Np2 HFOR 0, mu+nu
	kWmunuNp2_hfor1,      // W+jets, light-flavour Np2 HFOR 1, mu+nu
	kWmunuNp2_hfor2,      // W+jets, light-flavour Np2 HFOR 2, mu+nu
	kWmunuNp2_hfor3,      // W+jets, light-flavour Np2 HFOR 3, mu+nu
	kWmunuNp3,            // W+jets, light-flavour Np3, mu+nu
	kWmunuNp3_hfor0,      // W+jets, light-flavour Np3 HFOR 0, mu+nu
	kWmunuNp3_hfor1,      // W+jets, light-flavour Np3 HFOR 1, mu+nu
	kWmunuNp3_hfor2,      // W+jets, light-flavour Np3 HFOR 2, mu+nu
	kWmunuNp3_hfor3,      // W+jets, light-flavour Np3 HFOR 3, mu+nu
	kWmunuNp4,            // W+jets, light-flavour Np4, mu+nu
	kWmunuNp4_hfor0,      // W+jets, light-flavour Np4 HFOR 0, mu+nu
	kWmunuNp4_hfor1,      // W+jets, light-flavour Np4 HFOR 1, mu+nu
	kWmunuNp4_hfor2,      // W+jets, light-flavour Np4 HFOR 2, mu+nu
	kWmunuNp4_hfor3,      // W+jets, light-flavour Np4 HFOR 3, mu+nu
	kWmunuNp5,            // W+jets, light-flavour Np5, mu+nu
	kWmunuNp5_hfor0,      // W+jets, light-flavour Np5 HFOR 0, mu+nu
	kWmunuNp5_hfor1,      // W+jets, light-flavour Np5 HFOR 1, mu+nu
	kWmunuNp5_hfor2,      // W+jets, light-flavour Np5 HFOR 2, mu+nu
	kWmunuNp5_hfor3,      // W+jets, light-flavour Np5 HFOR 3, mu+nu
	kWtaunuNp0,           // W+jets, light-flavour Np0, tau+nu
	kWtaunuNp0_hfor0,     // W+jets, light-flavour Np0 HFOR 0, tau+nu
	kWtaunuNp0_hfor1,     // W+jets, light-flavour Np0 HFOR 1, tau+nu
	kWtaunuNp0_hfor2,     // W+jets, light-flavour Np0 HFOR 2, tau+nu
	kWtaunuNp0_hfor3,     // W+jets, light-flavour Np0 HFOR 3, tau+nu
	kWtaunuNp1,           // W+jets, light-flavour Np1, tau+nu
	kWtaunuNp1_hfor0,     // W+jets, light-flavour Np1 HFOR 0, tau+nu
	kWtaunuNp1_hfor1,     // W+jets, light-flavour Np1 HFOR 1, tau+nu
	kWtaunuNp1_hfor2,     // W+jets, light-flavour Np1 HFOR 2, tau+nu
	kWtaunuNp1_hfor3,     // W+jets, light-flavour Np1 HFOR 3, tau+nu
	kWtaunuNp2,           // W+jets, light-flavour Np2, tau+nu
	kWtaunuNp2_hfor0,     // W+jets, light-flavour Np2 HFOR 0, tau+nu
	kWtaunuNp2_hfor1,     // W+jets, light-flavour Np2 HFOR 1, tau+nu
	kWtaunuNp2_hfor2,     // W+jets, light-flavour Np2 HFOR 2, tau+nu
	kWtaunuNp2_hfor3,     // W+jets, light-flavour Np2 HFOR 3, tau+nu
	kWtaunuNp3,           // W+jets, light-flavour Np3, tau+nu
	kWtaunuNp3_hfor0,     // W+jets, light-flavour Np3 HFOR 0, tau+nu
	kWtaunuNp3_hfor1,     // W+jets, light-flavour Np3 HFOR 1, tau+nu
	kWtaunuNp3_hfor2,     // W+jets, light-flavour Np3 HFOR 2, tau+nu
	kWtaunuNp3_hfor3,     // W+jets, light-flavour Np3 HFOR 3, tau+nu
	kWtaunuNp4,           // W+jets, light-flavour Np4, tau+nu
	kWtaunuNp4_hfor0,     // W+jets, light-flavour Np4 HFOR 0, tau+nu
	kWtaunuNp4_hfor1,     // W+jets, light-flavour Np4 HFOR 1, tau+nu
	kWtaunuNp4_hfor2,     // W+jets, light-flavour Np4 HFOR 2, tau+nu
	kWtaunuNp4_hfor3,     // W+jets, light-flavour Np4 HFOR 3, tau+nu
	kWtaunuNp5,           // W+jets, light-flavour Np5, tau+nu
	kWtaunuNp5_hfor0,     // W+jets, light-flavour Np5 HFOR 0, tau+nu
	kWtaunuNp5_hfor1,     // W+jets, light-flavour Np5 HFOR 1, tau+nu
	kWtaunuNp5_hfor2,     // W+jets, light-flavour Np5 HFOR 2, tau+nu
	kWtaunuNp5_hfor3,     // W+jets, light-flavour Np5 HFOR 3, tau+nu
	kWcNp0,               // W+charm Np0
	kWcNp0_hfor0,         // W+charm Np0 HFOR 0
	kWcNp0_hfor1,         // W+charm Np0 HFOR 1
	kWcNp0_hfor2,         // W+charm Np0 HFOR 2
	kWcNp0_hfor3,         // W+charm Np0 HFOR 3
	kWcNp1,               // W+charm Np1
	kWcNp1_hfor0,         // W+charm Np1 HFOR 0
	kWcNp1_hfor1,         // W+charm Np1 HFOR 1
	kWcNp1_hfor2,         // W+charm Np1 HFOR 2
	kWcNp1_hfor3,         // W+charm Np1 HFOR 3
	kWcNp2,               // W+charm Np2
	kWcNp2_hfor0,         // W+charm Np2 HFOR 0
	kWcNp2_hfor1,         // W+charm Np2 HFOR 1
	kWcNp2_hfor2,         // W+charm Np2 HFOR 2
	kWcNp2_hfor3,         // W+charm Np2 HFOR 3
	kWcNp3,               // W+charm Np3
	kWcNp3_hfor0,         // W+charm Np3 HFOR 0
	kWcNp3_hfor1,         // W+charm Np3 HFOR 1
	kWcNp3_hfor2,         // W+charm Np3 HFOR 2
	kWcNp3_hfor3,         // W+charm Np3 HFOR 3
	kWcNp4,               // W+charm Np4	
	kWcNp4_hfor0,         // W+charm Np4 HFOR 0
	kWcNp4_hfor1,         // W+charm Np4 HFOR 1
	kWcNp4_hfor2,         // W+charm Np4 HFOR 2
	kWcNp4_hfor3,         // W+charm Np4 HFOR 3
	kWccNp0,              // W+ccbar Np0
	kWccNp0_hfor0,        // W+ccbar Np0 HFOR 0
	kWccNp0_hfor1,        // W+ccbar Np0 HFOR 1
	kWccNp0_hfor2,        // W+ccbar Np0 HFOR 2
	kWccNp0_hfor3,        // W+ccbar Np0 HFOR 3
	kWccNp1,              // W+ccbar Np1
	kWccNp1_hfor0,        // W+ccbar Np1 HFOR 0
	kWccNp1_hfor1,        // W+ccbar Np1 HFOR 1
	kWccNp1_hfor2,        // W+ccbar Np1 HFOR 2
	kWccNp1_hfor3,        // W+ccbar Np1 HFOR 3
	kWccNp2,              // W+ccbar Np2
	kWccNp2_hfor0,        // W+ccbar Np2 HFOR 0
	kWccNp2_hfor1,        // W+ccbar Np2 HFOR 1
	kWccNp2_hfor2,        // W+ccbar Np2 HFOR 2
	kWccNp2_hfor3,        // W+ccbar Np2 HFOR 3
	kWccNp3,              // W+ccbar Np3
	kWccNp3_hfor0,        // W+ccbar Np3 HFOR 0
	kWccNp3_hfor1,        // W+ccbar Np3 HFOR 1
	kWccNp3_hfor2,        // W+ccbar Np3 HFOR 2
	kWccNp3_hfor3,        // W+ccbar Np3 HFOR 3
	kWbbNp0,              // W+bbbar Np0
	kWbbNp0_hfor0,        // W+bbbar Np0 HFOR 0
	kWbbNp0_hfor1,        // W+bbbar Np0 HFOR 1
	kWbbNp0_hfor2,        // W+bbbar Np0 HFOR 2
	kWbbNp0_hfor3,        // W+bbbar Np0 HFOR 3
	kWbbNp1,              // W+bbbar Np1
	kWbbNp1_hfor0,        // W+bbbar Np1 HFOR 0
	kWbbNp1_hfor1,        // W+bbbar Np1 HFOR 1
	kWbbNp1_hfor2,        // W+bbbar Np1 HFOR 2
	kWbbNp1_hfor3,        // W+bbbar Np1 HFOR 3
	kWbbNp2,              // W+bbbar Np2
	kWbbNp2_hfor0,        // W+bbbar Np2 HFOR 0
	kWbbNp2_hfor1,        // W+bbbar Np2 HFOR 1
	kWbbNp2_hfor2,        // W+bbbar Np2 HFOR 2
	kWbbNp2_hfor3,        // W+bbbar Np2 HFOR 3
	kWbbNp3,              // W+bbbar Np3
	kWbbNp3_hfor0,        // W+bbbar Np3 HFOR 0
	kWbbNp3_hfor1,        // W+bbbar Np3 HFOR 1
	kWbbNp3_hfor2,        // W+bbbar Np3 HFOR 2
	kWbbNp3_hfor3,        // W+bbbar Np3 HFOR 3
	kWenuIncl,            // W+jets incl., e+nu
	kWenuInclB,           // W+jets incl., B-Hadron, e+nu
	kWenuInclC,           // W+jets incl., C-Hadron, e+nu
	kWenuInclLight,       // W+jets incl., light hadrons, e+nu
	kWmunuIncl,           // W+jets incl., mu+nu
	kWmunuInclB,          // W+jets, B-Hadron, mu+nu
	kWmunuInclC,          // W+jets, C-Hadron, mu+nu
	kWmunuInclLight,      // W+jets, light hadrons, mu+nu
	kWtaunuIncl,          // W+jets incl., tau+nu
	kWtaunuInclB,         // W+jets, B-Hadron, tau+nu
	kWtaunuInclC,         // W+jets, C-Hadron, tau+nu
	kWtaunuInclLight,     // W+jets, light hadrons, tau+nu
	kZeeNp0,              // Z+jets Np0, e+e
	kZeeNp1,              // Z+jets Np1, e+e
	kZeeNp2,              // Z+jets Np2, e+e
	kZeeNp3,              // Z+jets Np3, e+e
	kZeeNp4,              // Z+jets Np4, e+e
	kZeeNp5,              // Z+jets Np5, e+e
	kZmumuNp0,            // Z+jets Np0, mu+mu
	kZmumuNp1,            // Z+jets Np1, mu+mu
	kZmumuNp2,            // Z+jets Np2, mu+mu
	kZmumuNp3,            // Z+jets Np3, mu+mu
	kZmumuNp4,            // Z+jets Np4, mu+mu
	kZmumuNp5,            // Z+jets Np5, mu+mu
	kZtautauNp0,          // Z+jets Np0, tau+ttau
	kZtautauNp1,          // Z+jets Np1, tau+tau
	kZtautauNp2,          // Z+jets Np2, tau+tau
	kZtautauNp3,          // Z+jets Np3, tau+tau
	kZtautauNp4,          // Z+jets Np4, tau+tau
	kZtautauNp5,          // Z+jets Np5, tau+tau
	kZeeccNp0,            // Z+cc Np0, e+e
	kZeeccNp1,            // Z+cc Np1, e+e
	kZeeccNp2,            // Z+cc Np2, e+e
	kZeeccNp3incl,        // Z+cc Np3incl, e+e
	kZmumuccNp0,          // Z+cc Np0, mu+mu
	kZmumuccNp1,          // Z+cc Np1, mu+mu
	kZmumuccNp2,          // Z+cc Np2, mu+mu
	kZmumuccNp3incl,      // Z+cc Np3incl, mu+mu
	kZtautauccNp0,        // Z+cc Np0, tau+tau
	kZtautauccNp1,        // Z+cc Np1, tau+tau
	kZtautauccNp2,        // Z+cc Np2, tau+tau
	kZtautauccNp3incl,    // Z+cc Np3incl, tau+tau
	kZeebbNp0,            // Z+bb Np0, e+e
	kZeebbNp1,            // Z+bb Np1, e+e
	kZeebbNp2,            // Z+bb Np2, e+e
	kZeebbNp3incl,        // Z+bb Np3incl, e+e
	kZmumubbNp0,          // Z+bb Np0, mu+mu
	kZmumubbNp1,          // Z+bb Np1, mu+mu
	kZmumubbNp2,          // Z+bb Np2, mu+mu
	kZmumubbNp3incl,      // Z+bb Np3incl, mu+mu
	kZtautaubbNp0,        // Z+bb Np0, tau+tau
	kZtautaubbNp1,        // Z+bb Np1, tau+tau
	kZtautaubbNp2,        // Z+bb Np2, tau+tau
	kZtautaubbNp3incl,    // Z+bb Np3incl, tau+tau
	kZeeIncl,             // Z+jets incl., e+e
	kZeeInclB,            // Z+jets incl., B-Hadron, e+e
	kZeeInclC,            // Z+jets incl., C-Hadron, e+e
	kZeeInclLight,        // Z+jets incl., light hadrons, e+e
	kZmumuIncl,           // Z+jets incl., mu+mu
	kZmumuInclB,          // Z+jets incl., B-Hadron, mu+mu
	kZmumuInclC,          // Z+jets incl., C-Hadron, mu+mu
	kZmumuInclLight,      // Z+jets incl., light hadrons, mu+mu
	kZtautauIncl,         // Z+jets incl., tau+tau
	kZtautauInclB,        // Z+jets incl., B-Hadron, tau+tau
	kZtautauInclC,        // Z+jets incl., C-Hadron, tau+tau
	kZtautauInclLight,    // Z+jets incl., light hadrons, tau+tau
	kZnunuInclB,          // Z+jets incl., B-Hadron, nu+mu
	kZnunuInclC,          // Z+jets incl., C-Hadron, nu+nu
	kZnunuInclLight,      // Z+jets incl., light hadrons, nu+nu
	kDibosonWW,           // WW production
	kDibosonWZ,           // WZ production
	kDibosonZZ,           // ZZ production
	kWWenuqq,             // WW -> e nu + qq
	kWZenuqq,             // WZ -> e nu + qq
	kWWmunuqq,            // WW -> mu nu + qq
	kWZmunuqq,            // WZ -> mu nu + qq
	kWWtaunuqq,           // WW -> tau nu + qq
	kWZtaunuqq,           // WZ -> tau nu + qq
	kZWeeqq,              // ZW -> e e + qq
	kZZeeqq,              // ZZ -> e e + qq
	kZWmumuqq,            // ZW -> mu mu + qq
	kZZmumuqq,            // ZZ -> mu mu + qq
	kZWtautauqq,          // ZW -> tau tau + qq
	kZZtautauqq,          // ZZ -> tau tau + qq
	kWHlnubb,             // WH production (W -> lnu, H -> bbar)
	kQCDMatrixMethod,        // QCD estimate (matrix method default: loose tree), Run-2
 	kQCDMatrixMethod_Tight,  // QCD estimate (matrix method for tight leptons), Run-2
 	kQCDMatrixMethod_Loose,  // QCD estimate (matrix method for loose leptons), Run-2
 	kQCDMatrixMethod_EGamma, // QCD estimate (matrix method) EGamma, Run-1
 	kQCDMatrixMethod_Muon,   // QCD estimate (matrix method) Muon, Run-1
 	kQCDJetElectron_EGamma,  // QCD estimate (JetElectron  method) EGamma, Run-1
	kQCDJetElectron_Muon,    // QCD estimate (JetElectron  method) Muon, Run-1
	kQCD_AntiMuon,           // QCD estimate (AntiMuon  method) Muon, Run-2
	kQCD_JetLepton,          // QCD estimate (JetLepton  method) Electron, Run-2
	kFCNC_utZ_K01L,          // Single-top utZ FCNC, K=0.01, left-handed
	kFCNC_ctZ_K01L,          // Single-top ctZ FCNC, K=0.01, left-handed
	kUndefined               // Undefined sample type (needed for type comparisons)
    };
    
  private:
    TString   fColor;    // Sample Color
    EType     fType;     // Sample type
    Float_t   fXsection; // Sample Xsection
    TObjArray fPaths;    // Sample paths
    Bool_t    fIsSystematicSample; // Used for systematic only?
    
  public:
    AtlSample(const char* Name, const char* Title, const char* color,
	      EType Type, Float_t xsection, const char* PathName = 0);
    virtual ~AtlSample();
    virtual void Print(Option_t *option = "") const;
    const char* GetTypeName() const;
    const char* GetCategoryName() const;
    //    static const char* GetCategoryName(ECategory Category);
    Bool_t IsSgTop() const;
    Bool_t IsSgTopTChannel() const;
    Bool_t IsSgTopSChannel() const;
    Bool_t IsSgTopWt() const;
    Bool_t IstZj() const;
    Bool_t IsTtbar() const;
    Bool_t IsTtbarV() const;
    Bool_t IsWjetsLight() const;
    Bool_t IsWjetsHeavy() const;
    Bool_t IsWjetsC() const;
    Bool_t IsWjetsB() const;
    Bool_t IsWjets() const;
    Bool_t IsHFOR0() const;
    Bool_t IsHFOR1() const;
    Bool_t IsHFOR2() const;
    Bool_t IsHFOR3() const;
    Bool_t IsZjets() const;
    Bool_t IsZjetsB() const;
    Bool_t IsZjetsC() const;
    Bool_t IsZjetsLF() const;
    Bool_t IsZjetsHF() const;
    Bool_t IsDiboson() const;
    Bool_t IsFCNCtZ() const;
    Bool_t IsDATA() const;
    Bool_t IsMC() const;
    Bool_t IsDATAEGamma() const;
    Bool_t IsDATAMuon() const;
    Bool_t IsQCD() const;
    Bool_t IsQCDTight() const;
    Bool_t IsQCDLoose() const;
    Bool_t IsQCDEGamma() const;
    Bool_t IsQCDMuon() const;
    Bool_t IsQCDJetLepton() const { return fType == kQCD_JetLepton; }
    Bool_t IsQCDAntiMuon() const { return fType == kQCD_AntiMuon; }

    inline EType GetType() const {
	//
	// Return sample type
	//
	return fType;
    }
    inline const char* GetPathName() const {
	//
	// Return sample path name
	//
        return ( GetNPaths() == 1 ? GetPath(0) : 0 );
    }
    char const * GetPath(int index) const { return static_cast<TObjString *>(fPaths.At(index))->GetName(); }
    int GetNPaths() const { return fPaths.GetEntries(); }
    void AddPath(char const * path);
    inline Float_t GetXsection() const { return fXsection; }
    inline const char* GetColor() const { return fColor.Data(); }
    static Int_t ReadType(const char* Type);
    inline Bool_t IsSystematicSample() const { return fIsSystematicSample; }
	
    ClassDef(AtlSample,0) // A++ analysis data/MC sample
};
#endif

