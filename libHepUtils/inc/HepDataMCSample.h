//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCSample
#define HEP_HepDataMCSample
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TAttFill
#include <TAttFill.h>
#endif
#ifndef ROOT_TAttLine
#include <TAttLine.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif

class HepDataMCSample : public TNamed {

private:
    TAttFill fAttFill;   // Fill attributes (used for stacked drawing)
    TAttLine fAttLine;   // Line attributes (used for overlay drawing)
    Float_t  fXsection;  // X-section for this sample (pb)
    Float_t  fNEvents;   // Weighted number of events in this sample
    Float_t  fLumi;      // Luminosity (pb^-1), see also GetLumi()
    TFile   *fFile;      // Pointer to histogram file (if open)
    TH1F    *fHistogram; // Corresponding histogram (used in HepDataMCPlot only)
    
public:
    HepDataMCSample();
    HepDataMCSample(const char* name, const char* title,
		    Color_t color, Style_t line_style, Width_t line_width,
		    Style_t fill_style);
    virtual ~HepDataMCSample();
    TFile* Open();
    void Close();
    Float_t GetLumi() const;
    
    inline const char* GetLabel() { return GetTitle(); }
    inline Color_t GetColor() { return fAttFill.GetFillColor(); }
    inline Style_t GetLineStyle() { return fAttLine.GetLineStyle(); }
    inline Width_t GetLineWidth() { return fAttLine.GetLineWidth(); }
    inline Style_t GetFillStyle() { return fAttFill.GetFillStyle(); }
    inline Float_t GetXsection() const { return fXsection; }
    inline Float_t GetNEvents() const { return fNEvents; }
    inline TFile* GetFile() const { return fFile; }
    inline TH1F* GetHistogram() const { return fHistogram; }
    inline TAttFill GetAttFill() const { return fAttFill; }
    inline TAttLine GetAttLine() const { return fAttLine; }
    inline void SetColor(Color_t color) { fAttFill.SetFillColor(color); }
    inline void SetLineStyle(Style_t lstyle) { fAttLine.SetLineStyle(lstyle); }
    inline void SetLineWidth(Width_t lwidth) { fAttLine.SetLineWidth(lwidth); }
    inline void SetFillStyle(Style_t fstyle) { fAttFill.SetFillStyle(fstyle); }
    inline void SetXsection(Float_t xsec) { fXsection = xsec; }
    inline void SetLumi(Float_t lumi) { fLumi = lumi; }
    inline void SetHistogram(TH1F *h) { fHistogram = h; }
    
    ClassDef(HepDataMCSample,1) // MC sample inside HepDataMCPlotter
};
#endif
