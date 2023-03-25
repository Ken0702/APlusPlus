//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTau
#define ATLAS_AtlTau
#ifndef HEP_HepTau
#include <HepTau.h>
#endif
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#ifndef ATLAS_AtlMETWeights
#include <AtlMETWeights.h>
#endif

class AtlTau : public HepTau, public AtlTriggerMatch, public AtlMETWeights {
  
 public:
  enum EAuthor { kInvalidAuthor = BIT(0),
		 kTauRec        = BIT(1),   // CAL-based reconstruction
  		 kTau1P3P       = BIT(2) }; // Track-based reconstruction
  // Details can be found at the Tau working group wiki page
  // BEGIN_HTML
  // <a href="https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TauWG">
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TauWG</a>
  // END_HTML

  static const Int_t fgNAuthors;
  static const char* fgAuthorNames[];
  
  enum ETauFlag { kInvalidTauFlag     = BIT(0),
		  kMuonVeto           = BIT(1),
		  kElectronVetoLoose  = BIT(2),
		  kElectronVetoMedium = BIT(3),
		  kElectronVetoTight  = BIT(4),
		  kJetVetoLoose       = BIT(5),
		  kJetVetoMedium      = BIT(6),
		  kJetVetoTight       = BIT(7)
  };

  static const Int_t fgNTauFlags;
  static const char* fgTauFlagNames[];
  
 private:
  EAuthor  fAuthor;  // Reconstruction algorithm(s) used
  ETauFlag fTauFlag; // Tau quality flag

 public:
  AtlTau();
  AtlTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	 Bool_t IsTauPlus, EAuthor Author, ETauFlag TauFlag);
  virtual ~AtlTau();
  virtual void Clear(Option_t *option = "");
  virtual void Print(Option_t *option = "");
  static void PrintHeader();
  static void PrintFooter();
  const char* NameOfAuthor();
  void PrintTauFlag();

  inline EAuthor GetAuthor()   const { return fAuthor; }
  inline ETauFlag GetTauFlag() const { return fTauFlag; }
  
  inline Bool_t IsInvalidAuthor() const { return fAuthor & kInvalidAuthor; }
  inline Bool_t IsTauRec()        const { return fAuthor & kTauRec; }
  inline Bool_t IsTau1P3P()       const { return fAuthor & kTau1P3P; }

  inline Bool_t IsInvalidTauFlag()     const { return fTauFlag & kInvalidTauFlag; }
  inline Bool_t IsMuonVeto()           const { return fTauFlag & kMuonVeto; }
  inline Bool_t IsElectronVetoLoose()  const { return fTauFlag & kElectronVetoLoose; }
  inline Bool_t IsElectronVetoMedium() const { return fTauFlag & kElectronVetoMedium; }
  inline Bool_t IsElectronVetoTight()  const { return fTauFlag & kElectronVetoTight; }
  inline Bool_t IsJetVetoLoose()       const { return fTauFlag & kJetVetoLoose; }
  inline Bool_t IsJetVetoMedium()      const { return fTauFlag & kJetVetoMedium; }
  inline Bool_t IsJetVetoTight()       const { return fTauFlag & kJetVetoTight; }

  ClassDef(AtlTau,3) // Atlas tau class
};
#endif
