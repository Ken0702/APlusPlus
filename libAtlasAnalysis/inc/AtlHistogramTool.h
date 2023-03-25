//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlHistogramTool
#define ATLAS_AtlHistogramTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ROOT_THashList
#include <THashList.h>
#endif
#ifndef ROOT_TH1D
#include <TH1D.h>
#endif
#ifndef ROOT_TH2D
#include <TH2D.h>
#endif
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class TDirectory;
class AtlSelector;

class AtlHistObject : public TNamed {
    //
    // Small helper class for the histogram tool
    //
    // To be able to store multiple historams with the same name in
    // different folders, the full path name of a histogram is used
    // for fast access to the hash list. The histogram itself stores
    // only the (short) basename as identifier.
    //
  private:
    TH1 *fHistogram; // Histogram
    
  public:
    AtlHistObject(const char* name, const char* title, TH1 *hist) :
    TNamed(name, title) { fHistogram = hist; }
    virtual ~AtlHistObject() {;}
    inline TH1* GetHistogram() { return fHistogram; }
};

class AtlHistogramTool : public AtlAnalysisTool {

  private:
    TDirectory *fParentDir;  // Top-level folder
    THashList  *fHistograms; // List of histograms
    
  public:
    AtlHistogramTool(const char* name, const char* title);
    virtual ~AtlHistogramTool();

    TH1D* Add(TH1D* hist);
    TH1D* Add(const char* hname, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xup,
	      const char* xtitle, const char* ytitle);
    TH2D* Add(const char* hname, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xup,
	      Int_t nbinsy, Double_t ylow, Double_t yup,
	      const char* xtitle, const char* ytitle,
	      const char* ztitle);
    TH2D* Add(const char* hname, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xup,
	      Int_t nbinsy, const Double_t *ybins,
	      const char* xtitle, const char* ytitle,
	      const char* ztitle);
    void Fill(const char* hname, Double_t x, Double_t w);
    void Fill(const char* hname, Double_t x, Double_t y, Double_t w);
    virtual void SetBranchStatus() {;}
    virtual void BookHistograms() {;}
    virtual void FillHistograms() {;}
    virtual void Print() const;

    inline void Fill(const char* path, const char* hname,
		     Double_t x, Double_t w) {
	//
	// Convenience Fill() member function (see the main Fill()
	// function). The given path will be prepended to the
	// histogram name
	//
	Fill(Form("%s/%s", path, hname), x, w);
    }
    inline void Fill(const char* path, const char* hname,
		     Double_t x, Double_t y, Double_t w) {
	//
	// Convenience Fill() member function (see the main Fill()
	// function). The given path will be prepended to the
	// histogram name
	//
	Fill(Form("%s/%s", path, hname), x, y, w);
    }
	
  private:
    TDirectory* MkDirWithParents(const char* dir);
    
    ClassDef(AtlHistogramTool,0) // Histogram tool
};
#endif

