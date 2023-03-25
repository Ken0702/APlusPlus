//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// PdfReweighingTool                                                            //
//                                                                              //
// uses LHAPDF for reweighing events and observables                            //
// Please note the README-file, how to set compile this tool.                   //
//                                                                              //
//                                                                              //
// The implementation follows the TopPdfUncertainty TWiki:                      //
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TopPdfUncertainty    //
//                                                                              //
//                                                                              //
// Usage:                                                                       //
// - Create a unique instance using the tool's constructor                      //
//     PdfReweightingTool *tool = new PdfReweightingTool;                       //
//                                                                              //
// - Add the histogram you want to reweight (e.g. 'h1', need to be a TH1F)      //
//     tool->AddObservable(h1);                                                 //
//                                                                              //
// - For each event passing the event selection, one need to compute the        //
//   pdf weights (using the mcevt-information) and in addition fill the tool's  //
//   internal histograms using the observable (e.g. 'lepton_pt') and            //
//   the event weight:                                                          //
//     tool->ComputePdfWeights(x1,x2,scale,id1,id2,pdf1,pdf2);                  //
//     tool->FillObservable(h1, lepton_pt, evt_weight);                         //
//                                                                              //
// - After the event loop, one can retrieve the up/down variations of the       //
//   observable's histogram with                                                //
//     TH1F* h1_up   = tool->PdfUncertainty(h1, PdfSet::kUp);                   //
//     TH1F* h1_down = tool->PdfUncertainty(h1, PdfSet::kDown);                 //
//                                                                              //
// An example code can be found inside 'PdfReweightingTool/test'                //
//                                                                              //
//                                                                              //
//                                                                              //
// Author: Soren Stamm, <soren.stamm@cern.ch>                                   //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include "PdfReweightingTool/PdfReweightingTool.h"
#include <LHAPDF/LHAPDF.h>
#include <iostream>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>

using namespace std;

#ifndef __CINT__
ClassImp(PdfReweightingTool);
#endif

PdfReweightingTool* PdfReweightingTool::fgPdfInstance = 0;

//__________________________________________________________________

PdfReweightingTool::PdfReweightingTool(const char* name) {
  //
  // Default constructor
  //

  // Protect against multiple instances
  if ( fgPdfInstance != 0 ) {
    Error("PdfReweightingTool", "Multiple instances of this object not allowed. Please use PdfReweightingTool::Instance().");
    gSystem->Abort();
  }
  fgPdfInstance = this;

  fListOfPdfSets = new TList();
  fListOfObservables = new TList();
  fNsets = fListOfPdfSets->GetEntries();

  fVarUp = 0;
  fVarDown = 0;

  // Verbosity of LHAPDF
  // LHAPDF::setVerbosity(LHAPDF::SILENT);

  // At the moment only 3 selected pdfsets are available
  // it is assumed that they are in lhapdf's default path for pdfsets
  // Checking LHAPDF_DATA_PATH variable (should be set by Asg_Lhapdf)
  const char* lhapath = gSystem->Getenv("LHAPDF_DATA_PATH");
  cout << lhapath << endl;
  if ( strcmp(lhapath, "") == 0 ) {
    Error("PdfReweightingTool", "Environment variable LHAPATH not set!");
    Error("PdfReweightingTool", "Can not find PdfSets! Abort!");
    gSystem->Abort(0);
  }
  // not needed???
  // LHAPDF::setPDFPath(lhapath);
    
  if ( strcasecmp(name, "") == 0 ) {
    // if no dedicated pdf set is given, initial all default pdf sets
    for ( Int_t i = 0; i < PdfSet::fgNumPdfSets; i++ ) {
      AddPdfSet(PdfSet::fgPdfNames[i]);
    }
  } else {
    Info("PdfReweigtingTool", "Adding only one specific set!");
    AddPdfSet(name);
  }
  // Turn normfactors off by default
  SetApplyNormfactor(kFALSE);
}

//__________________________________________________________________

PdfReweightingTool::~PdfReweightingTool() {
  //
  // Default destructor
  //
  fListOfPdfSets->Delete();
  delete fListOfPdfSets;

  // Delete the list only, not the observables itself!
  // The tool does not own the observables!
  // fListOfObservables->Delete();
  delete fListOfObservables;
  
  fgPdfInstance = 0;
}

//__________________________________________________________________

PdfReweightingTool* PdfReweightingTool::Instance(const char* name) {
  //
  // Access PdfReweightingTool singleton
  //
  // If one provides a 'name' when initiating the PdfReweightingtTool,
  // only a single PDF set will be added to the list of PdfSets. See
  // PdfSet for available PDFs.
  //
  // Subsequent calls to this function with a 'name' do not have any
  // effect, i.e. the mode of the tool can not be changed once
  // initialised.
  //
  return ( fgPdfInstance != 0 ) ? (PdfReweightingTool*) fgPdfInstance : new PdfReweightingTool(name);
}

//__________________________________________________________________

void PdfReweightingTool::AddPdfSet(const char* pdfset, const char* path) {
  //
  // Add PDF set to list of PDF sets
  //
  fNsets++;
  Info("AddPdfSet",
       "Adding PdfSet No. %d with name %s to list of PdfSets.",
       fNsets, pdfset);
  fListOfPdfSets->Add(new PdfSet(pdfset, path, fNsets));
}

//__________________________________________________________________

void PdfReweightingTool::Print(Option_t* opt) const {
  //
  // Print information on the tool content
  //
  TString option = TString(opt);
  
  if ( option == "ALL" ) {
    cout << "// PdfReweightingTool" << endl;
    cout << endl;  
    Print("PDFinfo");
  } else if ( option == "PDFinfo" ) {
    cout << "Current Pdf sets:" << endl;
    cout << endl;  
    
    TIter next_pdf(fListOfPdfSets);
    PdfSet* set;
    while ( (set = (PdfSet*) next_pdf()) ) {
      cout << ">>>>>> PDF description: Set " << set->GetPdfId() << " <<<<<<" << endl;
      cout << set->GetDescription() << endl;
      cout << ">>>>>>                        <<<<<<" << endl;
      cout << endl;
      cout << endl;
    }
  }
}

//__________________________________________________________________

void PdfReweightingTool::ComputePdfWeights(double x1, double x2, double scale, int pid1, int pid2, double pdf1, double pdf2) {
  //
  // Compute new pdf weights for all PdfSets
  //
  TIter next_pdf(fListOfPdfSets);
  PdfSet* set;
  while ( (set = (PdfSet*) next_pdf()) ) {
    set->ComputePdfWeights(x1, x2, scale, pid1, pid2, pdf1, pdf2);
  }
}

//__________________________________________________________________

void PdfReweightingTool::AddObservable(TH1F *hist_obs) {
  //
  // Add observable to the tool and each PdfSet
  //
  fListOfObservables->Add( hist_obs );

  TIter next_pdf(fListOfPdfSets);
  PdfSet* set;
  while ( (set = (PdfSet*) next_pdf()) ) {
    set->AddObservable( hist_obs );
  }
}

//__________________________________________________________________

void PdfReweightingTool::FillObservable(TH1F *hist_obs, float observable, float evt_weight ) {
  //
  // Fill pdf reweighting histogram (TH2D) for each pdf set
  // Use same histogram as in AddObservable(TH1F*). 
  //
  // User has to call ComputePdfWeights() first!
  //
  TIter next_pdf(fListOfPdfSets);
  PdfSet* set;
  while ( (set = (PdfSet*) next_pdf()) ) {
    set->FillObservable( hist_obs, observable, evt_weight );
  }
}

//__________________________________________________________________

TList* PdfReweightingTool::EnvelopePlot(TH1F* hist_obs, Int_t bin) {
  //
  // Return projection histograms for envelope plot (one bin only)
  //

  TList *out = new TList;
  // Get maximum number of pdf members (x-axis)
  TIter next_pdf(fListOfPdfSets);
  PdfSet* set;

  while ( (set = (PdfSet*) next_pdf()) ) {
    TList* list_obs = set->GetListOfObservables();
    TH2D* hist = (TH2D*) list_obs->FindObject( Form("%s_%s",hist_obs->GetName(), set->GetName()) );
    TH1D* h_proj = hist->ProjectionY(Form("_py_%s_bin%d",set->GetName(),bin), bin, bin);
    out->Add(h_proj);
  }
  return out;
}

//__________________________________________________________________

TH1F* PdfReweightingTool::PdfUncertainty(TH1F* hist_obs, PdfSet::EVariation var) {
  //
  // Returns the variation of the observable 'hist_obs'
  //

  // Create lists for up/down variations (one for each pdf set)
  if ( fVarUp != 0 ) {
    fVarUp->Delete();
    delete fVarUp;
  }
  
  if ( fVarDown != 0 ) {
    fVarDown->Delete();
    delete fVarDown;
  }
  
  fVarUp   = new TList;
  fVarDown = new TList;
  
  TIter next_pdf(fListOfPdfSets);
  PdfSet* set;
  while ( (set = (PdfSet*) next_pdf()) ) {
    fVarUp->Add(   set->PdfUncertainty(hist_obs, PdfSet::kUp)   );
    fVarDown->Add( set->PdfUncertainty(hist_obs, PdfSet::kDown) );
  }

  // Get maximum/minimum variations for each bin
  TH1F* envelope_up;
  envelope_up = (TH1F*) hist_obs->Clone( Form("%s_envelope_up",hist_obs->GetName()) );
  envelope_up->SetDirectory(0);

  TIter next_up(fVarUp);
  TH1F* h_up;
  
  Int_t Nbins = hist_obs->GetNbinsX();
  for ( Int_t bin = 1; bin <= Nbins; bin++ ) {
    Float_t maxbin = 0;
    while ( (h_up = (TH1F*) next_up()) ) {
      if ( h_up->GetBinContent(bin) > maxbin )
	maxbin = h_up->GetBinContent(bin);
    } // Reset Iterator
    next_up.Reset();
    
    envelope_up->SetBinContent(bin,maxbin);
  }

  TH1F* envelope_down;
  envelope_down = (TH1F*) hist_obs->Clone( Form("%s_envelope_down",hist_obs->GetName()) );
  envelope_down->SetDirectory(0);
  
  TIter next_down(fVarDown);
  TH1F* h_down;
  
  for ( Int_t bin = 1; bin <= Nbins; bin++ ) {
    Float_t minbin = -1.;
    while ( (h_down = (TH1F*) next_down()) ) {
      if ( minbin == -1. ) {
	minbin = h_down->GetBinContent(bin);
      } else if ( h_down->GetBinContent(bin) < minbin ) {
	minbin = h_down->GetBinContent(bin);
      }
    } // Reset Iterator
    next_down.Reset();
    
    envelope_down->SetBinContent(bin,minbin);
  }

  // Compute the relative variation per bin
  Float_t uncertainty[Nbins];
  Float_t Upper, Lower, Midpoint;
  for ( Int_t bin = 1; bin <= Nbins; bin++ ) {
    Upper = envelope_up->GetBinContent(bin);
    Lower = envelope_down->GetBinContent(bin);
    Midpoint = (Upper + Lower)/2.;

    if ( Midpoint == 0. )
      uncertainty[bin-1] = 0.;
    else 
      uncertainty[bin-1] = (Upper - Midpoint)/Midpoint;
  }

  // Apply the relative variations to the observable's histogram
  TH1F* out = 0;
  Float_t oldbin = 0;
  if ( var == PdfSet::kUp ) {
    out = (TH1F*) hist_obs->Clone( Form("%s_PDF_up",hist_obs->GetName()) );    
    for ( Int_t bin = 1; bin <= Nbins; bin++ ) {
      oldbin = out->GetBinContent(bin);
      out->SetBinContent( bin, oldbin*(1.+uncertainty[bin-1]) );
    }
  } else {
    out = (TH1F*) hist_obs->Clone( Form("%s_PDF_down",hist_obs->GetName()) );    
    for ( Int_t bin = 1; bin <= Nbins; bin++ ) {
      oldbin = out->GetBinContent(bin);
      out->SetBinContent( bin, oldbin*(1.-uncertainty[bin-1]) );
    }
  }
  return out;
}

//__________________________________________________________________

TList* PdfReweightingTool::GetReweightedObservable(TH1F* hist_obs,  const char* pdf_set) {
  //
  // Return a list of all reweighted histograms of the set 
  // specified with 'pdf_set'
  //
  // The name need to match one of the PdfSets used in the constructor.
  //
  // It is the users' responsibility to delete the returned list
  // and it's pointers
  //

  // create new list
  TList *out = new TList;

  // find TH2D histogram matching pdf set and observable
  PdfSet* set = (PdfSet*) fListOfPdfSets->FindObject( pdf_set );
  if ( set == 0 ) {
    Error("GetReweightedObservable()", 
	  Form("Pdf set with name '%s' not found. Abort.", pdf_set) );
    gSystem->Abort();
  }
  
  TList* list_obs = set->GetListOfObservables();
  TH2D* hist = (TH2D*) list_obs->FindObject( Form("%s_%s",hist_obs->GetName(), set->GetName()) );

  if ( hist == 0 ) {
    Error("PdfReweightingTool::GetReweightedObservable()", 
	  Form("Observable with name '%s' not found. Abort.", hist_obs->GetName()) );
    gSystem->Abort();
  }
  
  // loop over all members and add projections to list
  // add clones to list, it is the users' responsibility to delete the objects
  for ( Int_t bin = 1; bin <= hist->GetNbinsY(); bin++ ) { 
    // index of bins start with 1, members with 0 --> member = bin - 1
    TH1D* h_proj = hist->ProjectionX(Form("%s_%s_member%d",hist_obs->GetName(), set->GetName(),bin-1), bin, bin);
    out->Add(h_proj);
  }
  return out;
}

//__________________________________________________________________

void PdfReweightingTool::GetNormfactor(Int_t DSID) {
  //
  // Load normfactor from file
  //
  TIter next_pdf(fListOfPdfSets);
  PdfSet* pdf_set;
  while ( (pdf_set = (PdfSet*) next_pdf()) ) {
    pdf_set->GetNormfactor(DSID);
  }
}

//__________________________________________________________________

void PdfReweightingTool::SetApplyNormfactor(Bool_t set) {
  //
  // Apply scale factor during pdf reweighting
  //
  TIter next_pdf(fListOfPdfSets);
  PdfSet* pdf_set;
  while ( (pdf_set = (PdfSet*) next_pdf()) ) {
    pdf_set->SetApplyNormfactor(set);
  }
}
