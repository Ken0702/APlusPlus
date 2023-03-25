{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for the signal region 
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

    Bool_t UseMV1c           = kTRUE;
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildPlottingTree = kTRUE;
    Bool_t BuildMergingTree  = kTRUE;
    ESelection Selection     = kSignalRegion;

    gROOT->ProcessLine(".x task_tChannelAnalysis_Main.C");
}
