//
// Example binary for demonstrating the different functions
// of the tool
//
// All info that is needed for the tool to operate, is generated.
//
// if you use a recent RootCore version, the binary can be found
// in "RootCore/bin/generic/testPdfTool"
//
// This test should run in a few seconds. If it runs successful,
// you will see the lines:
//
// Info in <PdfReweightingTool::AddPdfSet>: Adding PdfSet No. 1 with name CT10nlo to list of PdfSets.
// Info in <PdfReweightingTool::AddPdfSet>: Adding PdfSet No. 2 with name MSTW2008nlo68cl to list of PdfSets.
// Info in <PdfReweightingTool::AddPdfSet>: Adding PdfSet No. 3 with name NNPDF23_nlo_as_0119 to list of PdfSets.
// Time to run (CPU) :3.47 s  | 0.00115667 s/evt 
// Time to run (wall):3.6781 s  | 0.00122603 s/evt
//
// The output file "PdfTool_output.root" is created in the current directory.
//
#include "PdfReweightingTool/PdfReweightingTool.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
#include <sys/time.h>
#include <iostream>
#include "TCanvas.h"
#include "TList.h"
#include "TStopwatch.h"

using namespace std;

int main() {

  // Runtime information
  TStopwatch *watch = new TStopwatch;

  // Start clock
  watch->Start();

  // One instance
  PdfReweightingTool *tool = new PdfReweightingTool;
  // tool->SetApplyNormfactor(kTRUE);
  // tool->GetNormfactor(105200);

  // Create new file to save histograms
  TFile *f = new TFile("PdfTool_output.root", "RECREATE");
  cout << "Created output-file 'PdfTool_output.root' in current directory. " << endl;

  // Random numbers
  TRandom3 *rand = new TRandom3();

  // 1. observable: event count histogram
  TH1F *h1 = new TH1F("events", "events", 1, 0.5, 1.5);

  // 2. observable: lepton pt
  TH1F *h2 = new TH1F("lepton_pt", "lepton_pt", 30., 0., 200.);

  // Provide the tool with information on binning, etc.
  tool->AddObservable(h1);
  tool->AddObservable(h2);
  
  // pseudo-event loop
  Int_t Nevents = 3000;
  for ( Int_t event = 0; event < Nevents; event++) {

    // read input-variables from TopD3PDs, here just random numbers
    double x1, x2, scale, pdf1, pdf2;
    int id1, id2;
    float evt_weight, observable1, lepton_pt;
    
    // Please fill these variables below with the correct numbers
    // you get from ntup common ntuples (or any other source)
    // I generate here values to get some testing results
    // to show the main features of the tool
    x1 = 1.e-3 + rand->Uniform();
    if ( x1 > 0.3 ) { x1 = 0.3; } 
    x2 = 172.5*172.5/(8000*8000)/x1;
    scale = 100.;           // ntup-common input
    pdf1 = rand->Uniform(); // ntup-common input
    pdf2 = rand->Uniform(); // ntup-common input
    id1 = 1;                // ntup-common input 
    id2 = 2;                // ntup-common input 
    evt_weight = 1.;
    // rand->Gaus(1,0.3); // ntup-common input
    // if ( evt_weight < 0 ) evt_weight = 0.1;
        
    // only calculate weights once!
    tool->ComputePdfWeights(x1,x2,scale,id1,id2,pdf1,pdf2);

    // 1. observable
    observable1  = 1; // 1. observable: simple event counter
    // 2. observable: lepton_pt (please use the lepton pt from 'ntup-common input')
    lepton_pt    = 25. + rand->Exp(60); // create dummy distribution

    // Fill your histogram as usual
    h1->Fill(observable1, evt_weight);
    h2->Fill(lepton_pt, evt_weight);
    
    // Provide the same information to the PdfReweightingTool
    // You need to provide:
    //  - the pointer to the original histogram
    //  - the value of the observable
    //  - the evt weight
    tool->FillObservable(h1, observable1, evt_weight);
    tool->FillObservable(h2, lepton_pt, evt_weight);

    // The pdf weights per event can be retrieved as follows
    TList *PdfSets = tool->GetListOfPdfSets();
    TIter next_set(PdfSets);
    PdfSet *set;
    vector<double> *weights;
    while ( (set = (PdfSet*) next_set()) ) {
      weights = set->GetPdfWeights();
    }

  }

  // Get the indivdual reweighted histograms
  // This is a debug information! If you are only interested
  // in the envelope for the total pdf uncertainty, you don't need them!
  //
  // You need to provide the name of the pdf set that is
  // defined in PdfSet::fgPdfNames
  //
  // e.g for CT10nlo 
  TList* list = tool->GetReweightedObservable(h1, "CT10nlo");
  TIter next_hist(list);
  TH1D* hist = 0;
  while ( (hist = (TH1D*) next_hist()) ) {
    // save the histograms to a TFile
    hist->SetDirectory(f);
  }
  
  // Get The total PDF uncertainty for each observable
  TH1F* h1_up   = tool->PdfUncertainty(h1, PdfSet::kUp);
  TH1F* h1_down = tool->PdfUncertainty(h1, PdfSet::kDown);

  TH1F* h2_up   = tool->PdfUncertainty(h2, PdfSet::kUp);
  TH1F* h2_down = tool->PdfUncertainty(h2, PdfSet::kDown);

  // save all histograms to a TFile
  h1->SetDirectory(f); // this is the default nominal histogram
  
  // These are the histograms for the pdf uncertainty
  h1_up->SetDirectory(f);
  h1_down->SetDirectory(f);

  h2_up->SetDirectory(f);
  h2_down->SetDirectory(f);

  f->Write();
  f->Close();

  watch->Stop();
  
  // print timing information
  cout << "Time to run (CPU) :" 
       << watch->CpuTime()  << " s " << " | " << watch->CpuTime()/Nevents << " s/evt " << endl;
  cout << "Time to run (wall):"
       << watch->RealTime() << " s " << " | " << watch->RealTime()/Nevents << " s/evt " << endl;

  return 0;
}
