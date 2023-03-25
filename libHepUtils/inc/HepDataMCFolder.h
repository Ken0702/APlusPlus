//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef ATLAS_HepDataMCFolder
#define ATLAS_HepDataMCFolder
#ifndef HEP_HepDataMCSample
#include <HepDataMCSample.h>
#endif
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TAttFill
#include <TAttFill.h>
#endif
#ifndef ROOT_TAttLine
#include <TAttLine.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif

class HepDataMCFolder : public TNamed {

private:
    TAttFill  fAttFill;     // Fill attributes (used for stacked drawing)
    TAttLine  fAttLine;     // Line attributes (used for overlay drawing)
    TList    *fMCSamples;   // List of MC samples
                            // (input filenames and drawing attributes)
    Float_t  fNormUncertainty; // Norm uncertainty

public:
    HepDataMCFolder();
    HepDataMCFolder(const char* name, const char* title,
		    Color_t color, Style_t line_style, Width_t line_width,
		    Style_t fill_style);
    virtual ~HepDataMCFolder();

    inline void Add(HepDataMCSample *sample) { fMCSamples->Add(sample); }
    inline TList* GetMCSamples() { return fMCSamples; }
    inline void SetColor(Color_t color) { fAttFill.SetFillColor(color); }
    inline void SetLineStyle(Style_t lstyle) { fAttLine.SetLineStyle(lstyle); }
    inline void SetLineWidth(Width_t lwidth) { fAttLine.SetLineWidth(lwidth); }
    inline TAttFill GetAttFill() const { return fAttFill; }
    inline TAttLine GetAttLine() const { return fAttLine; }
    inline Color_t GetColor() { return fAttFill.GetFillColor(); }
    inline void SetNormUncertainty(Float_t unc) { fNormUncertainty = unc; }
    inline Float_t GetNormUncertainty() const { return fNormUncertainty; }
    
    ClassDef(HepDataMCFolder,2) // Folder for MC samples
};
#endif

