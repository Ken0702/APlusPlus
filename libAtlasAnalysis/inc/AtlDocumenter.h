//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Felix Thomas
//
#ifndef ATLAS_AtlDocumenter
#define ATLAS_AtlDocumenter
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif
#ifndef ROOT_THashTable
#include <THashTable.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TH2F
#include <TH2F.h>
#endif


class AtlDocumenter : public TNamed {

    TFile       *fOutputFile; // Output file
    const char  *fPath;       // path under which the histograms are stored
    THashTable  *fHistograms;
  
  protected:
    Double_t     fEvtWeight;
    
  public:
    AtlDocumenter(const char* name, const char* title, TFile* Output, const char* path);
    virtual ~AtlDocumenter();
    virtual void BookHistograms()  = 0;
    virtual void FillHistograms()  = 0;
    virtual void Clear() {;}
    virtual void Terminate() {;}
    virtual void Print() const {;}
    TH1F* AddHistogram(const char* name, const char* title, 
                       Int_t nBins, Double_t Firstbin, Double_t Lastbin, 
                       const char* xAxisTitle, const char* yAxisTitle);
    TObject* GetHistogram(const char* name) { return fHistograms->FindObject(name);}
    
    inline void SetOutputFile(TFile *f)
    { fOutputFile = f; }
   
    virtual void MakeDirectoryStructure();
    
    
    ClassDef(AtlDocumenter,0) // Abstract base class for ATLAS analysis tools
};
#endif

