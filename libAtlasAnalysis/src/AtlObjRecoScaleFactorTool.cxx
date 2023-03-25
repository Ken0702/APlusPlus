//____________________________________________________________________
//
// BEGIN_HTML
// <h2>Object Reconstruction Scale Factor Tool</h2>
// used for electrons, muons and jets
// <p>
// This tool changes the current event weight in MC events
// to correct for differences in the reconstruction of leptons and
// jets betwen DATA and MC.
// </p>
// <p>
// The scale factors are valid for ttbar and single-top analyses. An
// overview of all corrections can be found at the <a
// href="https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TopCommonScales">TopCommonScales</a>.
// The different scale factors can be enabled/disabled by setting
// the public data member fOperationMode (default=kAllSF).
// All systematics are included (see below) and can simply be applied
// by setting fSystematicMode to the corresponding systematic.
// (default is nominal, see also AtlTopLevelAnalysis for available systematics).
// </p>
// <p>
// <h3>Lepton Reconstruction:</h3>
// The signal lepton (either electron or muon) of each event must be
// set via SetLepton(). Note that at present there is no thorough treatment of
// di-lepton events. If you use this tool for such kind of events, you
// should use the lepton with the highest transverse momentum.
// </p>
// <p>
// <h3>Jet B-Tagging:</h3>
// The list of all jets for each event after all acceptance cuts
// (tagged and un-tagged) must be set via SetJets().<br>
// Note that the B-tag calibration is valid only in certain eta and Pt
// ranges. For details see GetBtagSF().
// </p>
// <p>
// <h3>Usage:</h3>
// <ol>
// <li>
// Create the tool in the usual way, add it to your analysis selector and configure the tool.
// <pre>
// ...
// AtlObjRecScaleFactorTool *reco_tool = new AtlObjRecoScaleFactorTool("tool_recoscale", "Object Reconstruction Scale Factor Tool");
// my_apptask->AddTool(reco_tool);
// my_apptask->SetToolVariable("tool_recoscale", "fSystematicMode", "AtlTopLevelAnalysis::kNominal");
// ...
// </pre>
// </li>
// <li>
// Configure the rest of the tool settings inside your analysis. Eg.<br>
// In MyAnalysis::BookHistograms():
// <pre>
// ...
// fRecoSFTool = (AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool", "tool_recoscale");
// ...
// </pre>
// </li>
// <li>
// The tool will change set the weights 'fPreTagEvtWeight' and
// 'fTagEvtWeight' in the current event.
// ...
// </pre>
// </li>
// </ol>
//
// END_HTML
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#include <TH1F.h>
#include <TDirectory.h>
#include <AtlEvent.h>
#include <AtlEventHeader.h>
#include <AtlSelector.h>
#include <AtlTopLevelAnalysis.h>
#include <cassert>
#include <iostream>

#ifndef __CINT__
ClassImp(AtlObjRecoScaleFactorTool);
#endif

using namespace std;

static const char* fgBTagWPNames[AtlObjRecoScaleFactorTool::kDL1r_60 + 1] = {
    "undef",
    "MV1_60", "MV1_70", "MV1_80", "MV1_85",
    "MV1c_50", "MV1c_80",
    "MV2c20_77", "MC2c20_85",
    "MV2c10_60", "MV2c10_70", "MV2c10_77", "MV2c10_85",
	"DL1r_85", "DL1r_77", "DL1r_70", "DL1r_60"
};

//____________________________________________________________________

AtlObjRecoScaleFactorTool::AtlObjRecoScaleFactorTool(const char *name,
						     const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fProcessMode    = kScaleFactor;
    fOperationMode  = 0x0;
    fSystematicMode = AtlTopLevelAnalysis::kNOMINAL;
    fLeptons = 0;
    fJets    = 0;
    fBTagger   = AtlBTag::kInvalidTagger;
    fBTagWP    = kUndefined;
    fBTagWeight_min = 1e10;
    fObjDefinitionTool = 0;

    // CDI configuration
    fBTagWeight_min = 1.e10;
    fUseCDI  = kFALSE;
    CDITool  = 0;
    CDIUncertainty    = Analysis::Total;
    CDIVars.jetAuthor = "AntiKt4TopoLCJVF0_5"; // default for 8TeV
    fInitCDI = kTRUE;

    // Initializing default values for scale factors
    for (Int_t i = 0; i < 4; i++) {
	CDIindex_SF[i]=999;
	CDIindex_Eff[i]=999;
    }
    mapIndex = 11; // has to be determined on runtime

    fNjetSystMode = 0;
}

//____________________________________________________________________

AtlObjRecoScaleFactorTool::~AtlObjRecoScaleFactorTool() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlObjRecoScaleFactorTool::Clear(Option_t *option) {
    //
    // Clear this tool at the beginning of each event
    //
}

//____________________________________________________________________

TString AtlObjRecoScaleFactorTool::GetOperationName() const {
    //
    // Get human-readable name of operation modes
    //
    if ( (fOperationMode == kAllSF) ) {
        return TString("kAllSF");
    }
    TString name;
    if ( (fOperationMode & kLeptonSF)  ) name.Append("kLeptonSF ");
    if ( (fOperationMode & kJetBtagSF) ) name.Append("kJetBtagSF ");
    if ( name.Length() == 0 ) name.Append("None");
    return name;
}

//____________________________________________________________________

void AtlObjRecoScaleFactorTool::SetBranchStatus() {
    //
    // Turn on all needed branches
    //
    fTree->SetBranchStatus("fEventHeader*",         kTRUE);
    fTree->SetBranchStatus("fElectrons*",           kTRUE);
    fTree->SetBranchStatus("fMuons*",               kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4LCTopoJets*", kTRUE);
    fTree->SetBranchStatus("fAntiKt4LCTopoJets*",   kTRUE);
}
//____________________________________________________________________

void AtlObjRecoScaleFactorTool::BookHistograms() {
    //
    // Book Histograms
    //
    // Get ObjectSelection Tool + pointers to analysis objects
    //

    // Get configuration from object selection tool (if not yet set)
    // Check if configuration is correct
    if ( fObjDefinitionTool == 0 ) {
	fObjDefinitionTool = (AtlObjectsToolD3PDSgTop*)
	    GetTool("AtlObjectsToolD3PDSgTop", "",kTRUE);
    }
    fBTagger = fObjDefinitionTool->GetBTagger();
    fBTagWeight_min = fObjDefinitionTool->GetBTagWeightMin();

    if ( fObjDefinitionTool->GetBTagger() == AtlBTag::kMV1 ) {

	switch (fBTagWP) {
	    case kMV1_60:
		workingPoint = "0_9827";
		break;
	    case kMV1_70:
		workingPoint = "0_7892";
		break;
	    case kMV1_80:
		workingPoint = "0_3511";
		break;
	    case kMV1_85:
		workingPoint = "0_1340";
		break;
	    default:
		cout << fBTagWP << endl;
		Error("BookHistograms",
		      "Unknown MV1 working point! Abort!");
		gSystem->Abort();
	}

	if ( fUseCDI ) {
	    CDITool = new Analysis::CalibrationDataInterfaceROOT("MV1",
								 Form("%s/dbase/BTagScaleFactors/BTagCalibration.env",
								      gSystem->Getenv("LIBATLASANALYSIS")));
	}
    } else if ( fObjDefinitionTool->GetBTagger() == AtlBTag::kMV1c ) {
	switch (fBTagWP) {
	    case kMV1c_50:
		workingPoint = "0_9195";
		break;
	    case kMV1c_80:
		workingPoint = "0_4051";
		break;
	    default:
		cout << fBTagWP << endl;
		Error("BookHistograms",
		      "Unknown MV1c working point! Abort!");
		gSystem->Abort();
	}

	if ( fUseCDI ) {
	    CDITool = new Analysis::CalibrationDataInterfaceROOT("MV1c",
								 Form("%s/dbase/BTagScaleFactors/BTagCalibration.env",
								      gSystem->Getenv("LIBATLASANALYSIS")));
	}
    } else if ( fObjDefinitionTool->GetBTagger() == AtlBTag::kMV2c20 ) {
	switch (fBTagWP) {
	    case kMV2c20_77:
		workingPoint = "0_4434";
		break;
	    case kMV2c20_85:
		workingPoint = "0_7887";
		break;
	    default:
		cout << fBTagWP << endl;
		Error("BookHistograms",
		      "Unknown MV12c20 working point! Abort!");
		gSystem->Abort();
	}
    } else if ( fObjDefinitionTool->GetBTagger() == AtlBTag::kMV2c10_60 ||
                fObjDefinitionTool->GetBTagger() == AtlBTag::kMV2c10_70 ||
                fObjDefinitionTool->GetBTagger() == AtlBTag::kMV2c10_77 ||
                fObjDefinitionTool->GetBTagger() == AtlBTag::kMV2c10_85 ) {
        workingPoint = "0";
    }
    else if ( fObjDefinitionTool->GetBTagger() == AtlBTag::kDL1r_85 ||
    		fObjDefinitionTool->GetBTagger() == AtlBTag::kDL1r_77 ||
			fObjDefinitionTool->GetBTagger() == AtlBTag::kDL1r_70 ||
			fObjDefinitionTool->GetBTagger() == AtlBTag::kDL1r_60
			) {
    	workingPoint = "0"; //v31 variable not used ?
    }
    else {
	Error("BookHistograms",
	      "Unknown BTagger! Please check CDI configuration! Abort!");
	gSystem->Abort();
    }

    // Check if working point agrees with cut-value in ObjectsTool
    // (only needed for Run-1)
    if ( fEvent->IsRun1() ) {
	TString WorkingPoint = workingPoint;
	WorkingPoint.ReplaceAll("_",".");
	Float_t wp = WorkingPoint.Atof();
	if ( wp != TMath::Abs(fBTagWeight_min) ) {
	    Error("BookHistograms",
		  "B-tag weight used in object definition (AtlObjectsToolD3PDSgTop) does not agree with given working point. Please check! Abort!");
	    gSystem->Abort();
	}
    }

    // Try to open btagging file, if it is not available, abort here.
    // CDI tool should do this, but for some reason it doesn't
    if ( fUseCDI ) {
	TFile *f = TFile::Open("/datc/atlas/stamm/BTagging/2014-Winter-8TeV-MC12-CDI.root");
	cout << "File Pointer to CDI File: " << f << endl;
	if ( f != 0 ) {
	    if ( f->IsOpen() ) {
		// Everything is fine (hopefully)
		// Close the file and let the CDI tool continue
		f->Close();
		delete f;
	    }
	} else {
	    Error("BookHistograms",
	      "Can not open BTagging file. Please check the directory %s on this machine.",
		  "/datc/atlas/stamm/BTagging/2014-Winter-8TeV-MC12-CDI.root");
	    Error("BookHistograms",
		  "Please check with BTagCalibration.env if filename hardcoded here is still correct.");
	    gSystem->Abort();
	}
    }
}

//____________________________________________________________________

void AtlObjRecoScaleFactorTool::FillHistograms() {
    //
    // Fill Histograms
    //
}

//____________________________________________________________________

void AtlObjRecoScaleFactorTool::Print() const {
    //
    // Print tool information
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Object Reconstruction Scale Factor Tool \"" << GetName() << "\"" << endl
	 << "========================================================" << endl
	 << "  Process mode         : " << GetProcessModeName(fProcessMode) << endl
	 << "  Operation mode       : " << GetOperationName().Data() << endl
	 << "  Systematics mode     : "
	 << AtlTopLevelAnalysis::GetSystematicEnum(fSystematicMode) << endl
	 << endl
	 << "  fUseCDI = ";
    if ( fUseCDI == kTRUE ) {
	cout << "true" << endl;
    } else {
	cout << "false" << endl;
    }
    cout << "  fBTagWP = " << GetBTagWPName(fBTagWP) << endl;
    cout << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

Bool_t AtlObjRecoScaleFactorTool::AnalyzeEvent() {
    //
    // Calculate the scale factor for an event
    //


    // No Scale Factors for MC
    if ( !fEvent->IsMC() ) return kTRUE;

    if ( fUseCDI && fInitCDI ) {
	fInitCDI = kFALSE;
	// Init MC/MC SF on first event
	// WARNING: Assuming the following order for the mapIndex
	//          in BTagCalibration.env
	// (Pythia6,-,Herwig,Sherpa,Pythia8,Herwigpp)
	//
	Warning("AnalyzeEvent", "Setup MC/MC SF. BTagCalibration.env file need to have the following order:");
	Warning("AnalyzeEvent", "(Pythia6,-,Herwig,Sherpa,Pythia8,Herwigpp)");

	TString xsecfile = TString(fXsecFile);
	TString shower;
	if ( xsecfile.IsNull() ) {
	    Error("AnalyzeEvent","No Xsection file provided. Abort!");
	    gSystem->Abort();
	} else {
	    FILE *flog = gSystem->OpenPipe(Form("grep %u %s | awk '{print $4}'",
						fEvent->GetChannelNumber(),
						xsecfile.Data()), "r");
	    shower.Gets(flog);
	    gSystem->ClosePipe(flog);
	}

	// This needs to be syncronized with AtlTopLevelAnalysisTask
	// and the BTagCalibration.env file used for the analysis
	if ( shower.EqualTo("pythia") ) {
	    mapIndex = 0;
	} else if ( shower.EqualTo("herwig") ) {
	    mapIndex = 2;
	} else if ( shower.EqualTo("sherpa") ) {
	    mapIndex = 3;
	} else if ( shower.EqualTo("pythia8") ) {
	    mapIndex = 4;
	} else if ( shower.EqualTo("herwigpp") ) {
	    mapIndex = 5;
	} else {
	    Error("AnalyzeEvent", "Unknown shower model: %s. Abort!", shower.Data());
	    gSystem->Abort();
	}
	Info("AnalyzeEvent", "Shower: %s, mapIndex = %u",
	     shower.Data(), mapIndex);

	Info("AnalyzeEvent", "retrieveCalibrationIndex");

	CDITool->retrieveCalibrationIndex("B", workingPoint.c_str(),
					  CDIVars.jetAuthor, true,
					  CDIindex_SF[0], mapIndex);
	CDITool->retrieveCalibrationIndex("C", workingPoint.c_str(),
					  CDIVars.jetAuthor, true,
					  CDIindex_SF[1], mapIndex);
	CDITool->retrieveCalibrationIndex("Light", workingPoint.c_str(),
					  CDIVars.jetAuthor, true,
					  CDIindex_SF[2], mapIndex);
	CDITool->retrieveCalibrationIndex("T", workingPoint.c_str(),
					  CDIVars.jetAuthor, true,
					  CDIindex_SF[3], mapIndex);
	CDITool->retrieveCalibrationIndex("B", workingPoint.c_str(),
					  CDIVars.jetAuthor, false,
					  CDIindex_Eff[0], mapIndex);
	CDITool->retrieveCalibrationIndex("C", workingPoint.c_str(),
					  CDIVars.jetAuthor, false,
					  CDIindex_Eff[1], mapIndex);
	CDITool->retrieveCalibrationIndex("Light", workingPoint.c_str(),
					  CDIVars.jetAuthor, false,
					  CDIindex_Eff[2], mapIndex);
	CDITool->retrieveCalibrationIndex("T", workingPoint.c_str(),
					  CDIVars.jetAuthor, false,
					  CDIindex_Eff[3], mapIndex);
	// Removed Warning about "found / not found extrapolation from charm"
	// --> see BTagScale.cxx
    }

    // Set pretag evt weight (lep SF only) or
    // b-tag evt weight according to operation mode
    //
    // N.B.: if the operation mode is set to lepSF only
    //       tag and pretag evt weight are the same.
    Double_t sf = 1.;
    Double_t weight = GetPreTagEvtWeight(); // w/o scale factors

    // // >>>> Temporary fix for Run2  <<<<
    // if ( fEvent->BeamEnergy() == 6500. ) weight = 1.;

    if ( (fOperationMode & kJetVertexSF) )
	sf = sf * GetJetVertexSF();
    if ( (fOperationMode & kLeptonSF) )
	sf = sf * GetLeptonSF();
    if ( !fNjetSystMode ) {
        TString const & systName = AtlTopLevelAnalysis::GetSystematicName(fSystematicMode);
        fNjetSystMode = 0;
        if ( systName.BeginsWith("Njet_BC_") ) {
            if ( systName == "Njet_BC__1up" ) {
                fNjetSystMode = 2;
                fNjetSystDelta = .2;
                Info(__FUNCTION__, "applying flavor-fraction systematic with Delta=%.2f", fNjetSystDelta);
            }
            else {
                Fatal(__FUNCTION__, "unrecognized Njet systematic: %s", systName.Data());
            }
        }
        else if (systName.BeginsWith("Njet_")) {
            char * tail = 0;
            long result = strtol(systName.Data() + 5, &tail, 10);
            assert(result > 0);
            fNjetSystNumber = result;
            if ( strncmp(tail, "orMore", 6) == 0 ) {
                fNjetSystRestTerm = true;
                tail += 6;
            }
            else {
                fNjetSystRestTerm = false;
            }
            fNjetSystDelta = 0.;
            if ( strcmp(tail, "__1up") == 0 ) {
                fNjetSystMode = 1;
                fNjetSystDelta = .24;
            }
            else {
                Fatal(__FUNCTION__, "unrecognized Njet systematic: %s", systName.Data());
            }
            Info(__FUNCTION__, "applying n-jet systematic with Number=%ld, RestTerm=%d, Delta=%.2f", fNjetSystNumber, fNjetSystRestTerm, fNjetSystDelta);
        }
        else {
            fNjetSystMode = -1;
        }
    }
    if ( fNjetSystMode == 1 && fJets->GetEntries() >= fNjetSystNumber ) {
        int n;
        if ( fNjetSystRestTerm ) {
            n = fJets->GetEntries() - fNjetSystNumber + 1;
        }
        else {
            n = 1;
        }
        double delta = sqrt(n * (fNjetSystDelta * fNjetSystDelta));
        if ( fNjetSystDelta < 0. ) {
            delta = -delta;
        }
        sf *= 1. + delta;
    }
    else if ( fNjetSystMode == 2 ) {
        double delta;
        if ( fEvent->GetEventHeader()->GetTruthFlavor() >= 4 ) {
            delta = fNjetSystDelta;
        }
        else {
            delta = -fNjetSystDelta/4.; // assuming that sigma_{tot,b+c) = 20% * sigma_{tot,b+c+l}
        }
        sf *= 1. + delta;
    }


    //v34: new weights: forward JVT , Global Leptopn Trigger SF
    sf = sf * GetForwardJVT();

    //sf = sf * GetLeptonTriggerSF(); This is already included in LeptonSF-->Not multiply it again

    fEvent->SetPreTagEvtWeight(weight*sf);

    if ( (fOperationMode & kJetBtagSF) )
	sf = sf * GetBtagSF();
    fEvent->SetTagEvtWeight(weight*sf);

    return kTRUE;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetForwardJVT() const {
	//v34 Forward JVT

	Double_t sf = 1.;

	sf = fEvent->GetEventHeader()->ForwardJVT().GetValue();

	return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetLeptonTriggerSF() const {
	//v34 Lepton Trigger SF

	Double_t sf = 1.;

	sf = fEvent->GetEventHeader()->LeptonTriggerSF().GetValue();

	return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetLeptonSF() const {
    //
    // Get lepton scale factor (e or mu)
    //
    Double_t sf = 1.;

    // >>>> Temporary fix for Run2  <<<<
    if ( fEvent->BeamEnergy() == 6500. ) {
	sf = fEvent->GetEventHeader()->LeptonWeight().GetValue();
	return sf;
    }

    // Check if there is a single lepton
    if ( fLeptons->GetEntries() != 1 ) {
	Error("GetLeptonSF", "%d leptons found", fLeptons->GetEntries());
	Error("GetLeptonSF", "Zero leptons or more than one Lepton! Abort!");
	gSystem->Abort(0);
    }

    // Determine lepton type (e or mu)
    HepParticle *lep = (HepParticle*) fLeptons->At(0);
    if ( lep->IsElectron() ) {
	sf = GetElectronSF();
    } else if ( lep->IsMuon() ) {
	sf = GetMuonSF();
    } else {
	Error("GetLeptonSF","Unknown lepton! Abort!");
	gSystem->Abort(0);
    }

    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetElectronSF() const {
    //
    // Get electron scale factor
    //

    Double_t sf = 1.;
    AtlElectron *el = (AtlElectron*) fLeptons->At(0);

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_RECO_SF_DOWN ) {
	sf = el->GetElectronScaleFactor_reco_down();
	sf *= el->GetElectronScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_RECO_SF_UP ) {
	sf = el->GetElectronScaleFactor_reco_up();
	sf *= el->GetElectronScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_ID_SF_DOWN ) {
	sf = el->GetElectronScaleFactor_Id_down();
	sf *= el->GetElectronScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_ID_SF_UP ) {
	sf = el->GetElectronScaleFactor_Id_up();
	sf *= el->GetElectronScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_TRIG_SF_DOWN ) {
	sf = el->GetElectronScaleFactor();
	sf *= el->GetElectronScaleFactor_trigger_down();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_TRIG_SF_UP ) {
	sf = el->GetElectronScaleFactor();
	sf *= el->GetElectronScaleFactor_trigger_up();
    } else {
	// all other systematics take nominal SF
	sf = el->GetElectronScaleFactor();
	sf *= el->GetElectronScaleFactor_trigger();
    }

    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetMuonSF() const {
    //
    // Get muon scale factor
    //

    Double_t sf = 1.;
    AtlMuon *mu = (AtlMuon*) fLeptons->At(0);

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_RECO_SF_DOWN ) {
	sf = mu->GetMuonScaleFactor_reco_down();
	sf *= mu->GetMuonScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_RECO_SF_UP ) {
	sf = mu->GetMuonScaleFactor_reco_up();
	sf *= mu->GetMuonScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_ID_SF_DOWN ) {
	sf = mu->GetMuonScaleFactor_Id_down();
	sf *= mu->GetMuonScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_ID_SF_UP ) {
	sf = mu->GetMuonScaleFactor_Id_up();
	sf *= mu->GetMuonScaleFactor_trigger();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_TRIG_SF_DOWN ) {
	sf = mu->GetMuonScaleFactor();
	sf *= mu->GetMuonScaleFactor_trigger_down();
    } else if ( fSystematicMode == AtlTopLevelAnalysis::kLEP_TRIG_SF_UP ) {
	sf = mu->GetMuonScaleFactor();
	sf *= mu->GetMuonScaleFactor_trigger_up();
    } else {
	// all other systematics take nominal SF
	sf = mu->GetMuonScaleFactor();
	sf *= mu->GetMuonScaleFactor_trigger();
    }

    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetBtagSF() {
    //
    // Get B-tagging scale factor
    //
    // The supported |eta| range is [0,2.5]. If a jet is outside those
    // ranges its weight is set to 1 and does not contribute to the
    // event-wide scale factor.
    //
    // The same holds for jets with "unknown" flavour (ie not b,c or
    // light-flavour). Tau-jets are of this type. Their weight is set
    // to 1 also and does not contribute.
    //
    // The event weight is computed as the product of all jet weights
    // for the jets under consideration (after pT and acceptance cuts).
    //
    // When evaluating uncertainties, the uncertainties for all b- and
    // c- jets is shifted together, but independently for all light
    // jets. Further, the efficiency and the inefficiency scale factor
    // uncertainties are anti-correlated: when one is shifted upwards,
    // the other will be shifted downwards and vice-versa.
    //
    //

    AtlJet *jet = 0;
    TIter next_jet(fJets);

    Double_t sf = 1.;
    if ( fUseCDI ) {
	while ( (jet = (AtlJet*)next_jet()) ) {
	    sf = sf * ComputeBtagSF(jet);
	}
    } else {

    if (fBTagger == AtlBTag::kDL1r_85){

    	sf = sf * fEvent->GetEventHeader()->BtagWeight_DL1r_85().GetValue();
    }
    else if (fBTagger == AtlBTag::kDL1r_77){

       	sf = sf * fEvent->GetEventHeader()->BtagWeight_DL1r_77().GetValue();
       }
    else if (fBTagger == AtlBTag::kDL1r_70){

       	sf = sf * fEvent->GetEventHeader()->BtagWeight_DL1r_70().GetValue();
       }
    else if (fBTagger == AtlBTag::kDL1r_60){

       	sf = sf * fEvent->GetEventHeader()->BtagWeight_DL1r_60().GetValue();
       }
    else if ( fBTagger == AtlBTag::kMV2c20 ) {
	    // >>>> Temporary fix for Run2 <<<<
	    if ( fBTagWP == kMV2c20_77 ) {
		sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c20_77().GetValue();
	    } else if ( fBTagWP == kMV2c20_85 ) {
		sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c20_85().GetValue();
	    } else {
		Error("GetBTagSF", "Unknown tagger or unknown working point. Abort!");
		gSystem->Abort();
	    }
        } else if ( fBTagger == AtlBTag::kMV2c10_60 ) {
            // >>>> Temporary fix for Run2 <<<<

            assert(fBTagWP == kMV2c10_60);
            sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c10_60().GetValue();
        } else if ( fBTagger == AtlBTag::kMV2c10_70 ) {
            // >>>> Temporary fix for Run2 <<<<
            assert(fBTagWP == kMV2c10_70);
            sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c10_70().GetValue();
        } else if ( fBTagger == AtlBTag::kMV2c10_77 ) {
            // >>>> Temporary fix for Run2 <<<<
            assert(fBTagWP == kMV2c10_77);
            sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c10_77().GetValue();
        } else if ( fBTagger == AtlBTag::kMV2c10_85 ) {
            // >>>> Temporary fix for Run2 <<<<
            assert(fBTagWP == kMV2c10_85);
            sf = sf * fEvent->GetEventHeader()->BtagWeight_MV2c10_85().GetValue();
	} else {
	    while ( (jet = (AtlJet*)next_jet()) ) {
		if ( TMath::Abs(jet->Eta()) > 2.5 ) continue; // no btagging for eta > 2.5
		if ( fBTagger == AtlBTag::kMV1 && fBTagWP == kMV1_60 )
		    sf = sf * GetBtagSF_MV1_60(jet);
		else if ( fBTagger == AtlBTag::kMV1 && fBTagWP == kMV1_70 )
		    sf = sf * GetBtagSF_MV1_70(jet);
		else if ( fBTagger == AtlBTag::kMV1 && fBTagWP == kMV1_80 )
		    sf = sf * GetBtagSF_MV1_80(jet);
		else if ( fBTagger == AtlBTag::kMV1c && fBTagWP == kMV1c_50 )
		    sf = sf * GetBtagSF_MV1c_50(jet);
		else if (fBTagger == AtlBTag::kDL1r_85){
			sf = sf * GetBtagSF_DL1r_85(jet);//DL1r
			cout<<"BTaggerDL1r "<<fBTagger<<endl;
		}
		else {
			cout<<"BTagger "<<fBTagger<<endl;
		    Error("GetBTagSF", "Unknown tagger or unknown working point. Abort!");
		    gSystem->Abort();
		}
	    }
	}
    }

    return sf;
}

//____________________________________________________________________
// the wrong way
Double_t AtlObjRecoScaleFactorTool::GetBtagSF_DL1r_85(AtlJet* jet) const {
    //
    // Get BTag DL1r
    //

    Double_t sf = 1.;
/*
    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_btag_down_DL1r();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_UP )
	sf = jet->GetJetScaleFactor_btag_up_DL1r();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_ctag_down_DL1r();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP )
	sf = jet->GetJetScaleFactor_ctag_up_DL1r();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_mistag_down_DL1r();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_UP )
	sf = jet->GetJetScaleFactor_mistag_up_DL1r();
    else // all other systematics take nominal SF
	sf = jet->GetJetScaleFactor_DL1r();
    cout<<"SCALEFACTOR "<<sf<<endl;*/
    return sf;
}



//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetBtagSF_MV1_60(AtlJet* jet) const {
    //
    // Get BTag MV1 @ 60%
    //

    Double_t sf = 1.;

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_btag_down_MV1_60();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_UP )
	sf = jet->GetJetScaleFactor_btag_up_MV1_60();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_ctag_down_MV1_60();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP )
	sf = jet->GetJetScaleFactor_ctag_up_MV1_60();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_mistag_down_MV1_60();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_UP )
	sf = jet->GetJetScaleFactor_mistag_up_MV1_60();
    else // all other systematics take nominal SF
	sf = jet->GetJetScaleFactor_MV1_60();
    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetBtagSF_MV1_70(AtlJet* jet) const {
    //
    // Get BTag MV1 @ 70%
    //

    Double_t sf = 1.;

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_btag_down_MV1_70();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_UP )
	sf = jet->GetJetScaleFactor_btag_up_MV1_70();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_ctag_down_MV1_70();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP )
	sf = jet->GetJetScaleFactor_ctag_up_MV1_70();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_mistag_down_MV1_70();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_UP )
	sf = jet->GetJetScaleFactor_mistag_up_MV1_70();
    else // all other systematics take nominal SF
	sf = jet->GetJetScaleFactor_MV1_70();
    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetBtagSF_MV1_80(AtlJet* jet) const {
    //
    // Get BTag MV1 @ 80%
    //

    Double_t sf = 1.;

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_btag_down_MV1_80();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_UP )
	sf = jet->GetJetScaleFactor_btag_up_MV1_80();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_ctag_down_MV1_80();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP )
	sf = jet->GetJetScaleFactor_ctag_up_MV1_80();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_mistag_down_MV1_80();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_UP )
	sf = jet->GetJetScaleFactor_mistag_up_MV1_80();
    else // all other systematics take nominal SF
	sf = jet->GetJetScaleFactor_MV1_80();
    return sf;
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetBtagSF_MV1c_50(AtlJet* jet) const {
    //
    // Get BTag MV1 @ 80%
    //

    Double_t sf = 1.;

    // Check systematic Mode
    if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_btag_down_MV1c_50();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kBTAGSF_UP )
	sf = jet->GetJetScaleFactor_btag_up_MV1c_50();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_ctag_down_MV1c_50();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP )
	sf = jet->GetJetScaleFactor_ctag_up_MV1c_50();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_DOWN )
	sf = jet->GetJetScaleFactor_mistag_down_MV1c_50();
    else if ( fSystematicMode == AtlTopLevelAnalysis::kMISTAGSF_UP )
	sf = jet->GetJetScaleFactor_mistag_up_MV1c_50();
    else // all other systematics take nominal SF
	sf = jet->GetJetScaleFactor_MV1c_50();
    return sf;
}

//___________________________________________________________

void AtlObjRecoScaleFactorTool::Terminate() {
    //
    // Executed at the end of the tool
    //
}

//___________________________________________________________

Double_t AtlObjRecoScaleFactorTool::ComputeBtagSF(AtlJet* jet) {
    //
    // Compute b-tagging scale factors using the
    // evil CDI
    // not used

    // m_scaleFactor is float, use a double for intermediate computations
    CDIUncertainty = Analysis::None;
    if ( fSystematicMode ==  AtlTopLevelAnalysis::kBTAGSF_UP
	 || fSystematicMode ==  AtlTopLevelAnalysis::kBTAGSF_DOWN
	 || fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_UP
	 || fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_DOWN
	 || fSystematicMode ==  AtlTopLevelAnalysis::kMISTAGSF_UP
	 || fSystematicMode ==  AtlTopLevelAnalysis::kMISTAGSF_DOWN)
	CDIUncertainty = Analysis::Total;
    if ( (fSystematicMode >= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK8) ||
	 (fSystematicMode >= AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK8) ||
	 (fSystematicMode >= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6) ||
	 (fSystematicMode >= AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6) ||
	 (fSystematicMode >= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK11) ||
	 (fSystematicMode >= AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK0
	  && fSystematicMode <= AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK11) ) {
	CDIUncertainty = Analysis::SFEigen;
    }
    if (fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6
	|| fSystematicMode == AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6) {
	// ugly kludge to add extra tau SF uncertainty.
	// Otherwise, C and Tau 100% correlated
	CDIUncertainty = Analysis::SFNamed;
    }

    CDIVars.jetPt = jet->Pt()*1000.; // CDI tool wants MeV
    CDIVars.jetEta = jet->Eta();
    if ( TMath::Abs(CDIVars.jetEta) > 2.5 ) return 1.; // no SF in this case
    if ( jet->Pt() <= 20. ) return 1.; // no SF in this case

    int typeIndex=-1;
    AtlJet::EFlavour jet_flavour = jet->GetTruthFlavour();
    switch (jet_flavour ) {
	// check with setup of CDI interface
	case AtlJet::kBeauty:
	    typeIndex = 0; break;
	case AtlJet::kCharm:
	    typeIndex = 1; break;
	case AtlJet::kTau:
	    typeIndex = 3; break;
	default:
	    typeIndex = 2; break;
	    // default is unknown OR lightflavoured
	    // the assumption that unknown == lightflavoured
    }

    Analysis::CalibResult res;
    Double_t jetWeight = 1.;
    Bool_t IsTagged = (jet->GetTag(fBTagger)->GetWeight() > fBTagWeight_min);
    //separate the 2 cases - eigenvector or not
    if (  CDIUncertainty != Analysis::SFEigen && CDIUncertainty != Analysis::SFNamed) { // old style method

	if ( IsTagged )
	    res = CDITool->getScaleFactor(CDIVars,
					  CDIindex_SF[typeIndex],
					  CDIindex_Eff[typeIndex],
					  CDIUncertainty);
	else
	    res = CDITool->getInefficiencyScaleFactor(CDIVars,
						      CDIindex_SF[typeIndex],
						      CDIindex_Eff[typeIndex],
						      CDIUncertainty);
	jetWeight = res.first;

	if ( CDIUncertainty == Analysis::Total ) {
	    if ( (fSystematicMode== AtlTopLevelAnalysis::kBTAGSF_UP && jet_flavour == AtlJet::kBeauty) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kCTAGSF_UP && jet_flavour == AtlJet::kCharm) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kCTAGSF_UP && jet_flavour == AtlJet::kTau) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kMISTAGSF_UP && jet_flavour == AtlJet::kLightFlavoured )) {
		if (IsTagged)
		    jetWeight = res.first+res.second;
		else
		    jetWeight = res.first-res.second;
	    }
	    if ( (fSystematicMode== AtlTopLevelAnalysis::kBTAGSF_DOWN && jet_flavour == AtlJet::kBeauty) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kCTAGSF_DOWN && jet_flavour == AtlJet::kCharm) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kCTAGSF_DOWN && jet_flavour == AtlJet::kTau) ||
		 (fSystematicMode== AtlTopLevelAnalysis::kMISTAGSF_DOWN && jet_flavour == AtlJet::kLightFlavoured) ) {
		if (IsTagged)
		    jetWeight = res.first-res.second;
		else
		    jetWeight = res.first+res.second;
	    }
	    //protecting SF for sys variations
	    Analysis::CalibResult eff;
	    if (IsTagged)
		eff = CDITool->getMCEfficiency(CDIVars,
					      CDIindex_Eff[typeIndex],
					      CDIUncertainty);
	    else
		eff = CDITool->getMCInefficiency(CDIVars,
						 CDIindex_Eff[typeIndex],
						 CDIUncertainty);
	    double EffMC = eff.first;
	    if (jetWeight*EffMC > 1.)
		jetWeight = 1./EffMC;
	    else if (jetWeight*EffMC < 0.)
		jetWeight = 0.;

	    if (jet_flavour == AtlJet::kTau && !IsTagged) {
		// for tau inefficiency,
		// this needs to be calculated this ugly way

		Analysis::CalibResult tempRes;
		tempRes = CDITool->getScaleFactor(CDIVars,
						 CDIindex_SF[typeIndex],
						 CDIindex_Eff[typeIndex],
						 CDIUncertainty);
		Float_t tausf = tempRes.first;
		if (fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_UP)
		    tausf = tempRes.first+tempRes.second;
		else if (fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_DOWN)
		    tausf = tempRes.first-tempRes.second;
		tempRes = CDITool->getMCEfficiency(CDIVars,
						   CDIindex_Eff[typeIndex],
						   CDIUncertainty);
		float mceff = tempRes.first;
		jetWeight = (1-tausf*mceff)/(1-mceff);
	    }
	}
    } else if (CDIUncertainty == Analysis::SFEigen) { // else is eigenvector method
	bool isOldWay=true; // is OldWay means there is no variation required
	if ( ( jet_flavour == AtlJet::kBeauty
	       && ( fSystematicMode>= AtlTopLevelAnalysis::kBTAGSF_UP
		    && fSystematicMode<= AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK8 ) )
	     || ( jet_flavour == AtlJet::kCharm
		  && ( fSystematicMode>= AtlTopLevelAnalysis::kCTAGSF_UP
		       && fSystematicMode<= AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6 ) )
	     || ( jet_flavour == AtlJet::kTau
		  && ( fSystematicMode>= AtlTopLevelAnalysis::kCTAGSF_UP
		       && fSystematicMode<= AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6 ) )
	     || ( jet_flavour == AtlJet::kLightFlavoured
		  && ( fSystematicMode>= AtlTopLevelAnalysis::kMISTAGSF_UP
		       && fSystematicMode<= AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK11 ) ) )
	    isOldWay=false;

	if ( isOldWay ) { // just taking SF itself
	    if ( IsTagged )
		res = CDITool->getScaleFactor(CDIVars,
					      CDIindex_SF[typeIndex],
					      CDIindex_Eff[typeIndex],
					      Analysis::None);
	    else
		res = CDITool->getInefficiencyScaleFactor(CDIVars,
							  CDIindex_SF[typeIndex],
							  CDIindex_Eff[typeIndex],
							  Analysis::None);
	    jetWeight = res.first;

	    if (jet_flavour == AtlJet::kTau && !IsTagged) { // ugly tau inefficiency again
		Analysis::CalibResult tempRes;
		tempRes = CDITool->getScaleFactor(CDIVars,
						  CDIindex_SF[typeIndex],
						  CDIindex_Eff[typeIndex],
						  CDIUncertainty);
		float tausf = tempRes.first;
		tempRes = CDITool->getMCEfficiency(CDIVars,
						   CDIindex_Eff[typeIndex],
						   CDIUncertainty);
		float mceff = tempRes.first;
		jetWeight=(1-tausf*mceff)/(1-mceff);
	    }
	} else {
	    int varIndex=-1; // this is the index of the variation (ie the eigenvector)
	    if (jet_flavour == AtlJet::kBeauty) {
		if ( fSystematicMode >=  AtlTopLevelAnalysis::kBTAGSF_DOWN )
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kBTAGSF_DOWN-1);
		else
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kBTAGSF_UP-1);
	    } else if (jet_flavour == AtlJet::kCharm) {
		if ( fSystematicMode>= AtlTopLevelAnalysis::kCTAGSF_DOWN )
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kCTAGSF_DOWN-1);
		else
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kCTAGSF_UP-1);
	    } else if (jet_flavour == AtlJet::kLightFlavoured) {
		if ( fSystematicMode>= AtlTopLevelAnalysis::kMISTAGSF_DOWN )
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kMISTAGSF_DOWN-1);
		else
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kMISTAGSF_UP-1);
	    } else if (jet_flavour == AtlJet::kTau) {
		if ( fSystematicMode>= AtlTopLevelAnalysis::kCTAGSF_DOWN )
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kCTAGSF_DOWN-1);
		else
		    varIndex=( fSystematicMode- AtlTopLevelAnalysis::kCTAGSF_UP-1);
	    }

	    if ( IsTagged )
		res = CDITool->getScaleFactor(CDIVars,
					      CDIindex_SF[typeIndex],
					      CDIindex_Eff[typeIndex],
					      CDIUncertainty,
					      varIndex);
	    else
		res = CDITool->getInefficiencyScaleFactor(CDIVars,
							  CDIindex_SF[typeIndex],
							  CDIindex_Eff[typeIndex],
							  CDIUncertainty,
							  varIndex);

	    // annoying that for eigenvectors,
	    // first and second correspond to variations with uncertainties, so different from above!
	    if ( (fSystematicMode >= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK0
		  && fSystematicMode <= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK8 ) ||
		 (fSystematicMode >= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK0
		  && fSystematicMode <= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6 ) ||
		 (fSystematicMode >= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK0
		  && fSystematicMode <= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK11 ) )
		jetWeight = res.first;
	    else
		jetWeight = res.second;

	    if (jet_flavour == AtlJet::kTau && !IsTagged) { // one more ugly variation
		Analysis::CalibResult tempRes;
		tempRes = CDITool->getScaleFactor(CDIVars,
						  CDIindex_SF[typeIndex],
						  CDIindex_Eff[typeIndex],
						  CDIUncertainty,
						  varIndex);
		float tausf(0);
		if ( (fSystematicMode >= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK0
		      && fSystematicMode<= AtlTopLevelAnalysis::kBTAGSF_UP_BREAK8 ) ||
		     (fSystematicMode >= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK0
		      && fSystematicMode<= AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6 ) ||
		     (fSystematicMode >= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK0
		      && fSystematicMode<= AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK11 ))
		    tausf = tempRes.first;
		else
		    tausf = tempRes.second;
		tempRes = CDITool->getMCEfficiency(CDIVars,
						  CDIindex_Eff[typeIndex],
						  CDIUncertainty);
		float mceff = tempRes.first;
		jetWeight=(1-tausf*mceff)/(1-mceff);
	    }
	}
    } else if (CDIUncertainty == Analysis::SFNamed) { // added kludge for ugly additional tau-only SF
	bool isOldWay=true; // is OldWay means there is no variation required
	if (jet_flavour == AtlJet::kTau) isOldWay = false;
	if ( isOldWay ) { // just taking SF itself
	    if ( IsTagged )
		res = CDITool->getScaleFactor(CDIVars,
					      CDIindex_SF[typeIndex],
					      CDIindex_Eff[typeIndex],
					      Analysis::None);
	    else
		res = CDITool->getInefficiencyScaleFactor(CDIVars,
							  CDIindex_SF[typeIndex],
							  CDIindex_Eff[typeIndex],
							  Analysis::None);
	    jetWeight = res.first;
	} else { // is a tau and we are doing one of the extra tau SFs
	    if ( IsTagged ) {
		res = CDITool->getScaleFactor(CDIVars,
					      CDIindex_SF[typeIndex],
					      CDIindex_Eff[typeIndex],
					      CDIUncertainty,
					      tauExtrapFromCharm);
		// here, first and second correspond to variations with uncertainties, so different from above!
		if (fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6)
		    jetWeight = res.first;
		else
		    jetWeight = res.second;
	    } else { // remember that tau inefficiency is "special"
		Analysis::CalibResult tempRes;
		tempRes = CDITool->getScaleFactor(CDIVars,
						  CDIindex_SF[typeIndex],
						  CDIindex_Eff[typeIndex],
						  CDIUncertainty,
						  tauExtrapFromCharm);
		float tausf(0);
		if (fSystematicMode ==  AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6)
		    tausf = tempRes.first;
		else
		    tausf = tempRes.second;
		tempRes = CDITool->getMCEfficiency(CDIVars,
						   CDIindex_Eff[typeIndex],
						   CDIUncertainty);
		float mceff = tempRes.first;
		jetWeight=(1-tausf*mceff)/(1-mceff);
	    }
	}
    }
    return jetWeight;
}

//____________________________________________________________________

const char* AtlObjRecoScaleFactorTool::GetBTagWPName(EBTagWP wp) {
    //
    // Get human-readable name of the b-tag working point
    //
    return fgBTagWPNames[wp];
}

//____________________________________________________________________

Double_t AtlObjRecoScaleFactorTool::GetJetVertexSF() const {
    //
    // Get JVF scale factor
    //

    // >>>> Temporary fix for Run1  <<<<
    if ( fEvent->BeamEnergy() < 5555. )
        return 1.;

    return fEvent->GetEventHeader()->JetVertexWeight().GetValue();
}
