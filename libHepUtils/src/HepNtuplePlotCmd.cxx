//____________________________________________________________________
//
// Helper class for plotting ntuple variabes inside the
// HepDataMCPlotter class
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef HEP_HepNtuplePlotCmd
#include <HepNtuplePlotCmd.h>
#endif

#ifndef __CINT__
ClassImp(HepNtuplePlotCmd);
#endif

//____________________________________________________________________

HepNtuplePlotCmd::HepNtuplePlotCmd(const char* TreeName,
				   const char* VarExp,
				   const char* Selection,
				   const char* PlotName,
				   const char* PlotTitle, Int_t Nbins,
				   Double_t Xlow, Double_t Xup,
				   const char* XTitle,
				   const char* YTitle) :
    fTreeName(TreeName), fVarExp(VarExp), fSelection(Selection),
    fPlotName(PlotName), fPlotTitle(PlotTitle), fNbins(Nbins),
    fXlow(Xlow), fXup(Xup), fXTitle(XTitle), fYTitle(YTitle) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

HepNtuplePlotCmd::~HepNtuplePlotCmd() {
    //
    // Default destructor
    //
}


