//____________________________________________________________________
//
// Abstract base class for ATLAS documenting tools
// 
// This class is intended to provide the basic functionality/interface 
// needed for documenting tools.
// The AtlTopPairFinder provides an example for the usage of such tools.
// 
//
//    Author: Felix Thomas <mailto:Felix.Thomas@mailbox.tu-dresden.de> 
//    Update: $Id$
//    Copyright: 2011 (C) Felix Thomas



#ifndef ATLAS_AtlDocumenter
#include <AtlDocumenter.h>
#endif

#ifndef __CINT__
ClassImp(AtlDocumenter);
#endif

//____________________________________________________________________

AtlDocumenter::AtlDocumenter(const char* name, const char* title, TFile* Output, const char* path = "" ) :
    TNamed(name, title) {
    //
    // Default constructor
    //

    fOutputFile = Output;
    fPath = path;
    fEvtWeight = 0;
    fHistograms = new THashTable();

}

//____________________________________________________________________

AtlDocumenter::~AtlDocumenter() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

TH1F* AtlDocumenter::AddHistogram(const char* name, const char* title, 
                                Int_t nBins, Double_t Firstbin, Double_t Lastbin, 
                                const char* xAxisTitle, const char* yAxisTitle){
  
  
  TH1F *hist = new TH1F(name, title, nBins, Firstbin, Lastbin);
  hist->SetXTitle(xAxisTitle);
  hist->SetYTitle(yAxisTitle);
  fHistograms->Add(hist);

  return hist;
}

//____________________________________________________________________

void AtlDocumenter::MakeDirectoryStructure() {
    //
    // Histogram booking
    //

 
  TDirectory *savdir = gDirectory;
  if (strcmp(fPath, "") != 0){
    gDirectory->mkdir(fPath);
    gDirectory->cd(fPath);
  }
  else{
     gDirectory->mkdir(this->GetName(), this->GetTitle()); 
  }
  
  BookHistograms();

  savdir->cd();


}

