void AtlSgTopValidationTask(
    //const char* InputTree = "t_app",
    const char* InputTree = "LooseTopInputsTree",
    const char* InputFiles =
    //
    // 2011 ACCEPTANCE CHALLENGE
    //
    // electron channel
    //

    "/dats1/atlas/ruthh/user.csuhr.v16.6.6.2.2.data11_7TeV.00183038.physics_Egamma.SGTOP_NTUP_TOPEL.f383_m872_p568_p569_v1.110708170834/*.root",
    const char* OutputFile = "$LIBSINGLETOP/hist/tChannelAnalysis/Validation/SgTopValidation_DATA_Egamma_Run183038.root",
    //
    // muon channel
    //
//     "/dats1/atlas/ruthh/user.herrberg.mc10_7TeV.108341.st_tchan_munu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.APP_tchan_munu_2011_all.110514195343/*.root",
//     const char* OutputFile = "$LIBSINGLETOP/hist/tChannelAnalysis/Validation/SgTopValidation_MC_tchan_munu.root",
    //"$HOME/user.herrberg.mc10_7TeV.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e598_s933_s946_r2215_r2260.APP_tchan_enu_2011.110514155607/*.root",
    Int_t NEvt = 20000000) {
    //
    // Small run script for single-top validation analysis
    //
    TChain *ch = new TChain(InputTree);
    ch->Add(InputFiles);
    AtlSgTopValidation *sel = new AtlSgTopValidation(OutputFile);
    
//    sel->fInputMode = AtlSelector::kD3PDSgTop;
    
//     sel->fEleTriggerChoice = \"EF_e20_medium\";//"EF_e20_medium";
//     sel->fMuTriggerChoice = "EF_mu18";
//     sel->fMET_Mag_min   = 25.;
//     sel->fElPt_Min      = 25.;
//     sel->fJets_Pt_min   = 25.;
//     sel->fJets_Eta_min  = -4.5;
//     sel->fJets_Eta_max  = 4.5;
//     sel->fBJets_Eta_min = -2.5;
//     sel->fBJets_Eta_max = 2.5;

    
    // ====================================
    // Add top common object selection tool
    // ====================================
 //    AtlTopCommonObjectsTool *top_sel = new AtlTopCommonObjectsTool("TopSelection","Top Common Object Selection");

//     // Set Single-Top Mode
//     top_sel->fSingleTopMode           = kTRUE;
    
//     // Set type of MET used
//     top_sel->fUseMETRefFinal          = kFALSE;
//     top_sel->fUseMETRefFinalEM_etight = kTRUE;
    
//     // Jets
//     top_sel->fApplyOR     = kTRUE ;
//     top_sel->fFindBadJets = kTRUE;
    
//     // B-jets
// //     top_sel->fBJetTagger = AtlBTag::kSV0;
//     top_sel->fBJetWeightMin = 5.85;// means 49.7% efficient (was 5.38 which  is 50.6% efficient)

//     sel->AddTool(top_sel);

    
    ch->Process(sel, "", NEvt);
}



//
// 2010 ACCEPTANCE CHALLENGE
//
//"/datm1/atlas/ruthh/grid_tests/sgtop_acceptance_challenge/user.fthomas.003604._00036.app.root",
//"/datm1/atlas/ruthh/grid_tests/sgtop_acceptance_challenge/tchannel_enu_acceptance_challenge.root",
