#ifndef ATLAS_AtlObjectsDefinitionToolR2
#define ATLAS_AtlObjectsDefinitionToolR2

#include <Rtypes.h>

#include "AtlAnalysisTool.h"
#include "AtlBTag.h"
#include "AtlObjectsDefinitionTool.h"

class AtlScaleFactor;
class TList;


class AtlObjectsDefinitionToolR2 : public AtlObjectsDefinitionTool {

public:

    enum EWeights {
        kPileupSF = BIT(0),
        kZVertexSF = BIT(1),
        kElectronSF = BIT(12),
        kMuonSF = BIT(13),
        kTriggerSF = BIT(14),
        kBtaggingSF = BIT(15),
        kJetVertexSF = BIT(16),
        kLeptonSF = (kElectronSF | kMuonSF | kTriggerSF),
        kTruthSF = 0x00000fff,
        kRecoSF = 0x0ffff000,
        kAllSF = 0x0fffffff,
    };

    AtlObjectsDefinitionToolR2(char const * name, char const * title);
    ~AtlObjectsDefinitionToolR2();

    TList * GetLeptons() /*override*/ { return fLeptons; }
    TList * GetElectrons() /*override*/ { return fElectrons; }
    TList * GetMuons() /*override*/ { return fMuons; }
    TList * GetJets() /*override*/ { return fJets; }
    TList * GetBJets() /*override*/ { return fBJets; }

    Bool_t AnalyzeEvent() /*override*/;
    void Clear() /*override*/;

public:

    /* configuration */

    Double_t fElectronPt_min;         // Min. Pt of signal electrons
    Double_t fMuonPt_min;             // Min. Pt of signal muons
    Double_t fJetEta_max;             // Max. allowed |Eta| for jets
    Double_t fJetPt_min;              // Min. Pt of jets

    AtlJet::EType fJetsAuthor;        // Type of jets

    AtlBTag::ETagger fBtagger;        // b-tagging algorithm
    Double_t fBtaggingWorkingPoint;   // working point
    Int_t fBtaggingNominalEfficiency; // nominal efficiency of the working point [%]

    UInt_t fWeights;                  // flags which weights should be applied (see EWeights)

protected:

    /* event-associated data */

    TList * fLeptons;                 // List of leptons
    TList * fElectrons;               // List of electrons
    TList * fMuons;                   // List of muons
    TList * fJets;                    // List of jets
    TList * fBJets;                   // List of b-jets

private:

    Bool_t fClean;

    AtlScaleFactor const & GetBtaggingSF() const;

    ClassDef(AtlObjectsDefinitionToolR2, 0);
};

#endif
