{
    //____________________________________________________________________
    //
    // MEM Plotting script for the W_jets ctrl region 
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
    
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildPlottingTree = kTRUE;
    Bool_t BuildMergingTree  = kTRUE;
    ESelection Selection     = kTtbarCtrlRegion;
    
    gROOT->ProcessLine(".x task_memplotting_Main.C");
}
