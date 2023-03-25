{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for the ttbar ctrl region 
    //
    // Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Soeren Stamm
    //
    enum ESelection {
	kSignalRegion,
	kWjetsCtrlRegion,
	kTtbarCtrlRegion
    };

    Bool_t UseNewInputFormat = kTRUE;

    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildPlottingTree = kTRUE;
    Bool_t BuildMergingTree  = kTRUE;
    ESelection Selection     = kTtbarCtrlRegion;
    
    gROOT->ProcessLine(".x task_sChannelAnalysis_Main.C");
}
