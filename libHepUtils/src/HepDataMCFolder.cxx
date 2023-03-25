//____________________________________________________________________
//
// Folder to group MC samples
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef ATLAS_HepDataMCFolder
#include <HepDataMCFolder.h>
#endif

#ifndef __CINT__
ClassImp(HepDataMCFolder);
#endif

//____________________________________________________________________

HepDataMCFolder::HepDataMCFolder() {
    //
    // Default constructor
    //
    fNormUncertainty = 0.;
}

//____________________________________________________________________

HepDataMCFolder::HepDataMCFolder(const char* name, const char* title,
				 Color_t color, Style_t line_style,
				 Width_t line_width, Style_t fill_style) :
    TNamed(name, title), fAttFill(color, fill_style),
    fAttLine(color, line_style, line_width) {
    //
    // Normal constructor
    //
    fMCSamples = new TList;
}

//____________________________________________________________________

HepDataMCFolder::~HepDataMCFolder() {
    //
    // Default destructor
    //
//    fMCSamples->Delete();
    delete fMCSamples;
}

