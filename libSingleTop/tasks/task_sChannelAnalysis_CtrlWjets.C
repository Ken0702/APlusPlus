{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for the W_jets ctrl region 
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Msria Kind
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
    ESelection Selection     = kWjetsCtrlRegion;
    
    gROOT->ProcessLine(".x task_sChannelAnalysis_Main.C");
}
