//========================================================================
// SampleList.C
//
// Arrays containing data and MC samples
// used for various analyses.
// Cross sections, display names and colors
// are already set for plotter class.
//
//------------------------------------------------------------------------
// Author: Michelangelo Giorgi <strider@physik.hu-berlin.de>
//========================================================================


//===================================
// DATA Samples (Lumis in pb^-1 !!!)
//===================================

// =======================
// Egamma and Muons Stream
// =======================


// All periods
const Int_t n_data_files_Egamma = 1; 
TString data_files_Egamma[] = {
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/DATA/Egamma/DATA_Egamma_AllPeriods.root",
    "data11_Egamma"};

const Int_t n_data_files_Muons = 1; 
TString data_files_Muons[] = {
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/DATA/Muons/DATA_Muons_AllPeriods.root",
    "data11_Muons"  };

//===================================
// MC Samples
//===================================


//----------------------------------
// Signals
//----------------------------------

// ===============
// Singletop group
// ===============

const Int_t n_signal_sgtop_files = 7;
TString signal_sgtop_files[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117360.st_tchan_enu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e835_s1310_s1300_r2730_r2700_p766__v2.111129233818/",
    "tchannel_enu",
    
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117361.st_tchan_munu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e835_s1310_s1300_r2730_r2700_p766__v2.111129233831/",
    "tchannel_munu",

    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117362.st_tchan_taunu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e825_s1310_s1300_r2730_r2780_p766__v2.111129233840/",
    "tchannel_taunu",

    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117363.st_schan_enu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e825_s1310_s1300_r2730_r2700_p766__v2.111129233850/",
    "schannel_enu",

    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117364.st_schan_munu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e825_s1310_s1300_r2730_r2780_p766__v2.111129233906/",
    "schannel_munu",

    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.117365.st_schan_taunu_AcerMC..NTUP_SGTOP_PRESEL_LEPT1.e825_s1310_s1300_r2730_r2700_p766__v2.111129233918/",
    "schannel_taunu",

    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/sgtop/user.kind.mc11_7TeV.105500.AcerMC_Wt..NTUP_SGTOP_PRESEL_LEPT1.e825_s1310_s1300_r2730_r2780_p766__v2.111129233941/",    
    "wtchannel",
};

TString signal_sgtop_files_tex[] = { "tchannel $e\\nu$", "tchannel $\\mu\\nu$", "tchannel $\\tau\\nu$", 
				     "schannel $e\\nu$", "schannel $\\mu\\nu$", "schannel $\\tau\\nu$", 
				     "Wtchannel",
};
Float_t signal_sgtop_cs[] = { // AcerMC
    7.0928,
    7.0928,
    7.084, 
    0.47, 
    0.47, 
    0.47, 
    14.59
};
Float_t signal_sgtop_genfilteff[] = { 1., 1., 1., //singletop_tchannel_enu
				      1., 1., 1., //singletop_tchannel_munu
				      1., 1., 1., //singletop_tchannel_taunu
				      1., 1., 1., //singletop_schannel_enu
				      1., 1., 1., //singletop_schannel_munu
				      1., 1., 1., //singletop_schannel_taunu
				      1., 1., 1., //singletop_wtchannel
};

//----------------------------------
// BG
//----------------------------------

// ===========
// TTbar group
// ===========


const Int_t n_bg_ttbar_files = 4;
TString bg_ttbar_files[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1.root",
    "ttbar_4"
};

TString bg_ttbar_files_jet_scaled_up[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_jet_scaled_up.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_jet_scaled_up.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_jet_scaled_up.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_jet_scaled_up.root",
    "ttbar_4"
};

TString bg_ttbar_files_jet_scaled_down[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_jet_scaled_down.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_jet_scaled_down.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_jet_scaled_down.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_jet_scaled_down.root",
    "ttbar_4"
};


TString bg_ttbar_files_pileup_up[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_pileup_up.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_pileup_up.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_pileup_up.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_pileup_up.root",
    "ttbar_4"
};


TString bg_ttbar_files_pileup_down[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_pileup_down.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_pileup_down.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_pileup_down.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_pileup_down.root",
    "ttbar_4"
};


TString bg_ttbar_files_outsoft_up[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_outsoft_up.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_outsoft_up.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_outsoft_up.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_outsoft_up.root",
    "ttbar_4"
};


TString bg_ttbar_files_outsoft_down[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_outsoft_down.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_outsoft_down.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_outsoft_down.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_outsoft_down.root",
    "ttbar_4"
};


TString bg_ttbar_files_el_scaled_up[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_el_scaled_up.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_el_scaled_up.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_el_scaled_up.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_el_scaled_up.root",
    "ttbar_4"
};

TString bg_ttbar_files_el_scaled_down[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_el_scaled_down.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_el_scaled_down.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_el_scaled_down.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_el_scaled_down.root",
    "ttbar_4"
};

TString bg_ttbar_files_jet_goodReco[] = {
    "/rdsk/datm5/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00001.MC_LEPT1_jet_goodReco.root",
    "ttbar_1",
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00002.MC_LEPT1_jet_goodReco.root",
    "ttbar_2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00003.MC_LEPT1_jet_goodReco.root",
    "ttbar_3",
    "/rdsk/datm8/atlas/SgTopD3PD/rel17/Phase2/MC/ttbar/user.kind.mc11_7TeV.105200.T1_McAtNlo_Jimmy..NTUP_SGTOP_PRESEL_LEPT1.e835_s1272_s1274_r2730_r2780_p766__v2.111129234713/user.kind.000323._00004.MC_LEPT1_jet_goodReco.root",
    "ttbar_4"
};



TString bg_ttbar_files_incl_tex[] = { "$t \\bar{t} incl" };

Float_t bg_ttbar_cs[] = {
    89.399412 // ttbar incl (no fully hadronic; default)
    //79.961 // ttbar incl (no fully hadronic and no spin correlations)
};

Float_t bg_ttbar_genfilteff[] = { 1., 1., 1., 1., 1., 1., 1., 1.
};

// ============
// W+jets group
// ============

const Int_t n_bg_wjets_files = 31; 
TString bg_wjets_files[] = {

    // LIGHT ======================================================    

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107680.AlpgenJimmyWenuNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2780_p766__v2.111130000230/",
    "wenu_np0",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107681.AlpgenJimmyWenuNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000304/",
    "wenu_np1",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107682.AlpgenJimmyWenuNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2780_p766__v2.111130000322/",
    "wenu_np2",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107683.AlpgenJimmyWenuNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000339/",
    "wenu_np3",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107684.AlpgenJimmyWenuNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2780_p766__v2.111130000353/",
    "wenu_np4",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107685.AlpgenJimmyWenuNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000411/",
    "wenu_np5",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107690.AlpgenJimmyWmunuNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2780_p766__v2.111130000428/",
    "wmunu_np0",

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107691.AlpgenJimmyWmunuNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000443/",
    "wmunu_np1",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107692.AlpgenJimmyWmunuNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2780_p766__v2.111130000458/",
    "wmunu_np2",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107693.AlpgenJimmyWmunuNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000513/",
    "wmunu_np3",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107694.AlpgenJimmyWmunuNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000526/",
    "wmunu_np4",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107695.AlpgenJimmyWmunuNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e825_s1299_s1300_r2730_r2700_p766__v2.111130000541/",
    "wmunu_np5",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107700.AlpgenJimmyWtaunuNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111130000602/",
    "wtaunu_np0",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107701.AlpgenJimmyWtaunuNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111130000621/",
    "wtaunu_np1",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107702.AlpgenJimmyWtaunuNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111130000713/",
    "wtaunu_np2",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107703.AlpgenJimmyWtaunuNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111130000731/",
    "wtaunu_np3",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107704.AlpgenJimmyWtaunuNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111130000746/",
    "wtaunu_np4",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsLF/user.fmgiorgi.mc11_7TeV.107705.AlpgenJimmyWtaunuNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111130000813/",
    "wtaunu_np5",

    // HEAVY ======================================================
    
    //-------------------------------------------------------------- W c
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117293.AlpgenWcNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000221/",
    "wc_np0",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117294.AlpgenWcNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000231/",
    "wc_np1",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117295.AlpgenWcNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000240/",
    "wc_np2",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117296.AlpgenWcNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2700_p766__v2.111130000253/",
    "wc_np3",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117297.AlpgenWcNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2700_p766__v2.111130000301/",
    "wc_np4",
    //-------------------------------------------------------------- W cc
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117284.AlpgenWccFullNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2700_p766__v2.111130000310/",
    "wcc_np0",

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117285.AlpgenWccFullNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000319/",
    "wcc_np1",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117286.AlpgenWccFullNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000329/",
    "wcc_np2",
    
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.117287.AlpgenWccFullNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000339/",
    "wcc_np3",
    //-------------------------------------------------------------- W bb
    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.107280.AlpgenJimmyWbbFullNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000348/",
    "wbb_np0",

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.107281.AlpgenJimmyWbbFullNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2700_p766__v2.111130000405/",
    "wbb_np1",

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.107282.AlpgenJimmyWbbFullNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2780_p766__v2.111130000418/",
    "wbb_np2",

    "/rdsk/datm6/atlas/SgTopD3PD/rel17/Phase2/MC/wjetsHF/user.kind.mc11_7TeV.107283.AlpgenJimmyWbbFullNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e887_s1310_s1300_r2730_r2700_p766__v2.111130000431/",
    "wbb_np3",
};





TString bg_wjets_files_tex[] = { "$We\\nu$ np0",    "$We\\nu$ np1",    "$We\\nu$ np2",    "$We\\nu$ np3",    "$We\\nu$ np4",     "$We\\nu$ np5", 
				 "$W\\mu\\nu$ np0", "$W\\mu\\nu$ np1", "$W\\mu\\nu$ np2", "$W\\mu\\nu$ np3", "$W\\mu\\nu$ np4",  "$W\\mu\\nu$ np5", 
				 "$W\\tau\\nu$ np0","$W\\tau\\nu$ np1","$W\\tau\\nu$ np2","$W\\tau\\nu$ np3","$W\\tau\\nu$ np4", "$W\\tau\\nu$ np5",
				 "$Wc$ np0",        "$Wc$ np1",        "$Wc$ np2",        "$Wc$ np3",        "$Wc$ np4",
				 "$Wcc$ np0",       "$Wcc$ np1",       "$Wcc$ np2",       "$Wcc$ np3",
				 "$Wbb$ np0",       "$Wbb$ np1",       "$Wbb$ np2",       "$Wbb$ np3",

};

Float_t bg_wjets_cs[] = { 8305.92, //8296.0,  // NNLO // 6913.3,  //cs_wenu_np0
			  1565.16, //1551.6,  // NNLO // 1293.0,  //cs_wenu_np1
			  453.948, //452.5,  // NNLO //  377.1,  //cs_wenu_np2
			  121.716, //121.1,  // NNLO //  100.9,  //cs_wenu_np3
			  31.044, //30.4,  // NNLO //   25.3,  //cs_wenu_np4
			  8.4, //8.3,  // NNLO //    6.9,  //cs_wenu_np5
			     
			  8303.52, //8284.2,  // NNLO // 6935.4,  //cs_wmunu_np0
			  1565.04, //1561.6,  // NNLO // 1281.2,  //cs_wmunu_np1
			  453.396, //453.25, // NNLO //  375.3,  //cs_wmunu_np2
			  122.256, //121.66, // NNLO // 101.1,  //cs_wmunu_np3
			  30.9, //31.01, // NNLO // 25.7,  //cs_wmunu_np4
			  8.304, // 8.32, // NNLO // 7.0,  //cs_wmunu_np5
			     
			  8302.32, //8284.2,  // NNLO // 6835.8,  //cs_wtaunu_np0
			  1563.84, //1561.6,  // NNLO // 1276.8,  //cs_wtaunu_np1
			  453.816, //453.25, // NNLO //  376.6,  //cs_wtaunu_np2
			  121.812, //121.66, // NNLO //  100.8,  //cs_wtaunu_np3
			  30.768, //31.01, // NNLO //   25.7,  //cs_wtaunu_np4
			  8.448, //8.32, // NNLO //    7.0,  //cs_wtaunu_np5

			  979.488, // 644.4*1.52 //cs_wc_np0
			  311.6,   // 205.0*1.52 //cs_wc_np1
			  77.216,  // 50.8*1.52  //cs_wc_np2
			  17.328,  // 11.4*1.52  //cs_wc_np3
			  4.256,   // 2.8*1.52   //cs_wc_np4
			  
			  153.036, //127.5,  //cs_wcc_np0
			  125.616, //103.2,  //cs_wcc_np1
			  62.496, //51.7,  //cs_wcc_np2
			  20.352, //16.9,  //cs_wcc_np3
			  
			  56.784, // 47.32*1.2  //cs_wbb_np0 NO dRcut
			  42.924, // 35.77*1.2  //cs_wbb_np1 NO dRcut
			  20.808, // 17.34*1.2  //cs_wbb_np2 NO dRcut
			  7.956,  // 6.63*1.2   //cs_wbb_np3 NO dRcut
};


Float_t bg_wjets_genfilteff[] = { 1., 1., 1., //wenu_np0
				  1., 1., 1., //wenu_np1
				  1., 1., 1., //wenu_np2
				  1., 1., 1., //wenu_np3
				  1., 1., 1., //wenu_np4
				  1., 1., 1., //wenu_np5
				  1., 1., 1., //wmunu_np0
				  1., 1., 1., //wmunu_np1
				  1., 1., 1., //wmunu_np2
				  1., 1., 1., //wmunu_np3
				  1., 1., 1., //wmunu_np4
				  1., 1., 1., //wmunu_np5
				  1., 1., 1., //wtaunu_np0
				  1., 1., 1., //wtaunu_np1
				  1., 1., 1., //wtaunu_np2
				  1., 1., 1., //wtaunu_np3
				  1., 1., 1., //wtaunu_np4
				  1., 1., 1., //wtaunu_np5
				  1., 1., 1., //wc_np0
				  1., 1., 1., //wc_np1
				  1., 1., 1., //wc_np2
				  1., 1., 1., //wc_np3
				  1., 1., 1., //wc_np4
				  1., 1., 1., //wcc_np0
				  1., 1., 1., //wcc_np1
				  1., 1., 1., //wcc_np2
				  1., 1., 1., //wcc_np3
				  1., 1., 1., //wbb_np0 dRcut
				  1., 1., 1., //wbb_np1 dRcut
				  1., 1., 1., //wbb_np2 dRcut 
				  1., 1., 1., //wbb_np3 dRcut
				  

};


// ============
// Z+jets group
// ============

const Int_t n_bg_zjets_files = 18;
TString bg_zjets_files[] = {
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107650.AlpgenJimmyZeeNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235240/",
    "zee_np0",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107651.AlpgenJimmyZeeNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235301/",
    "zee_np1",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107652.AlpgenJimmyZeeNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235312/",
    "zee_np2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107653.AlpgenJimmyZeeNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235349/",
    "zee_np3",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107654.AlpgenJimmyZeeNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235414/",
    "zee_np4",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107655.AlpgenJimmyZeeNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235429/",
    "zee_np5",

    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107660.AlpgenJimmyZmumuNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111129235440/",
    "zmumu_np0",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107661.AlpgenJimmyZmumuNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111129235453/",
    "zmumu_np1",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107662.AlpgenJimmyZmumuNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235509/",
    "zmumu_np2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107663.AlpgenJimmyZmumuNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235521/",
    "zmumu_np3",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107664.AlpgenJimmyZmumuNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235531/",
    "zmumu_np4",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107665.AlpgenJimmyZmumuNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235540/",
    "zmumu_np5",
    
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107670.AlpgenJimmyZtautauNp0_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2780_p766__v2.111129235550/",
    "ztautau_np0",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107671.AlpgenJimmyZtautauNp1_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235607/",
    "ztautau_np1",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107672.AlpgenJimmyZtautauNp2_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235617/",
    "ztautau_np2",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107673.AlpgenJimmyZtautauNp3_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235627/",
    "ztautau_np3",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107674.AlpgenJimmyZtautauNp4_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235636/",
    "ztautau_np4",
    "/rdsk/datm7/atlas/SgTopD3PD/rel17/Phase2/MC/zjets/user.kind.mc11_7TeV.107675.AlpgenJimmyZtautauNp5_pt20..NTUP_SGTOP_PRESEL_LEPT1.e835_s1299_s1300_r2730_r2700_p766__v2.111129235646/",
    "ztautau_np5"
};

TString bg_zjets_files_tex[] = { "$Zee$ np0",         "$Zee$ np1",         "$Zee$ np2",         "$Zee$ np3",         "$Zee$ np4",         "$Zee$ np5",
				 "$Z\\mu\\mu$ np0",   "$Z\\mu\\mu$ np1",   "$Z\\mu\\mu$ np2",   "$Z\\mu\\mu$ np3",   "$Z\\mu\\mu$ np4",   "$Z\\mu\\mu$ np5",
				 "$Z\\tau\\tau$ np0", "$Z\\tau\\tau$ np1", "$Z\\tau\\tau$ np2", "$Z\\tau\\tau$ np3", "$Z\\tau\\tau$ np4", "$Z\\tau\\tau$ np5",
//  not				 "$Zee_bb$ np0",         "$Zee_bb$ np1",         "$Zee_bb$ np2",         "$Zee_bb$ np3",
//  used			 "$Z\\mu\\mu_bb$ np0",   "$Z\\mu\\mu_bb$ np1",   "$Z\\mu\\mu$ np2",      "$Z\\mu\\mu_bb$ np3",
// in SINGLE TOP		 "$Z\\tau\\tau_bb$ np0", "$Z\\tau\\tau_bb$ np1", "$Z\\tau\\tau_bb$ np2", "$Z\\tau\\tau_bb$ np3",
};

Float_t bg_zjets_cs[] = { 835.4, //830.125, // NNLO? 661.9,    //cs_zee_np0
			  167.95, //166.237, // NNLO? 133.3,    //cs_zee_np1
			  50.675,//50.282, // NNLO?  40.3,    //cs_zee_np2
			  13.95, //13.922, // NNLO?  11.2,    //cs_zee_np3 
			  3.6, //3.616, // NNLO?   2.7,    //cs_zee_np4
			  1.0375, //0.942, // NNLO?   0.8,    //cs_zee_np5
			  
			  835.85, //830.125, // NNLO? 657.7,    //cs_zmumu_np0
			  167.675, //166.237, // NNLO? 132.8,    //cs_zmumu_np1
			  50.4125, //50.282, // NNLO?  39.6,    //cs_zmumu_np2
			  13.9875, //13.922, // NNLO?  11.1,    //cs_zmumu_np3
			  3.4375, //3.616, // NNLO?   2.8,    //cs_zmumu_np4
			  0.9625, //0.942, // NNLO?   0.8,    //cs_zmumu_np5
			  
			  835.5, //830.125, // NNLO? 657.4,    //cs_ztautau_np0
			  168.5125, //166.237, // NNLO? 133.0,    //cs_ztautau_np1
			  50.45, //50.282, // NNLO?  40.4,    //cs_ztautau_np2
			  14.0625, //13.922, // NNLO?  11.0,    //cs_ztautau_np3
			  3.4875, //3.616, // NNLO?   2.9,    //cs_ztautau_np4
			  0.9625, //0.942, // NNLO?   0.7,    //cs_ztautau_np5
/////////////////////////////////////////////////////////////////////////////////////////
// 			  8.2125, //6.52,   //cs_zee_bb_np0      //!
// 			  3.1, //2.47,   //cs_zee_bb_np1
// 	u		  1.1125, //0.808,  //cs_zee_bb_np2
//  	n		  0.4875, //0.387,  //cs_zee_bb_np3
// 	u		  8.2, //6.52,   //cs_zmumu_bb_np0
// 	s		  3.0875, //2.47,   //cs_zmumu_bb_np1
// 	e		  1.1125, //0.808,  //cs_zmumu_bb_np2
// 	d		  0.4875, //0.387,  //cs_zmumu_bb_np3
// 	!		  8.2125, //6.52,   //cs_ztautau_bb_np0
// 			  3.1125, //2.47,   //cs_ztautau_bb_np1
// 			  1.1125, //0.808,  //cs_ztautau_bb_np2
// 			  0.4875, //0.387,  //cs_ztautau_bb_np3  //!
};

Float_t bg_zjets_genfilteff[] = { 1., 1., 1., //zee_np0
				  1., 1., 1., //zee_np1
				  1., 1., 1., //zee_np2	     
				  1., 1., 1., //zee_np3 
				  1., 1., 1., //zee_np4
				  1., 1., 1., //zee_np5
				  1., 1., 1., //zmumu_np0
				  1., 1., 1., //zmumu_np1
				  1., 1., 1., //zmumu_np2
				  1., 1., 1., //zmumu_np3
				  1., 1., 1., //zmumu_np4
				  1., 1., 1., //zmumu_np5
				  1., 1., 1., //ztautau_np0
				  1., 1., 1., //ztautau_np1
				  1., 1., 1., //ztautau_np2
				  1., 1., 1., //ztautau_np3
				  1., 1., 1., //ztautau_np4
				  1., 1., 1., //ztautau_np5
/////////////////////////////////////////////////////////////////////
// 				  1., 1., 1., //cs_zee_bb_np0
// 				  1., 1., 1., //cs_zee_bb_np1
// 				  1., 1., 1., //cs_zee_bb_np2
// 		u		  1., 1., 1., //cs_zee_bb_np3
// 		n		  1., 1., 1., //cs_zmumu_bb_np0
// 		u		  1., 1., 1., //cs_zmumu_bb_np1
// 		s		  1., 1., 1., //cs_mumu_bb_np2
// 		e		  1., 1., 1., //cs_zmumu_bb_np3
// 		d		  1., 1., 1., //cs_ztautau_bb_np0
// 		!		  1., 1., 1., //cs_ztautau_bb_np1
// 				  1., 1., 1., //cs_ztautau_bb_np2
// 				  1., 1., 1., //cs_ztautau_bb_np3

};

// =============
// Diboson group
// =============

const Int_t n_bg_diboson_files = 3; 
TString bg_diboson_files[] = {
    "",
    "ww_incl",
    "",
    "wz_incl",
    "",
    "zz_incl",
};

TString bg_diboson_files_tex[] = {
    "WW incl",
    "WZ incl",
    "ZZ incl",
};
Float_t bg_diboson_cs[] = { 17.020444,  // 11.5003*1.48 ww incl
			    5.54256, // 3.4641*1.60 wz incl
			    1.26386, // 0.9722*1.30 zz incl
};
Float_t bg_diboson_genfilteff[] = { 1., 1., 1., //ww inlc
				    1., 1., 1., //wz incl
				    1., 1., 1., //zz incl
};

TString bg_diboson_names[] = { "WW incl",
			       "WZ incl",
			       "ZZ incl",
};



// ======================================================================
// new color code -> Moriond 2012
//
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SingleTopColorCode
// ======================================================================

//kBlack 	ATLAS data


//Signal color
//Int_t signal_color = kRed;

// official colors of sgtop group
Int_t sg_t_chan_color  = kAzure+5;
Int_t bg_t_chan_color  = kAzure+4; 
Int_t bg_s_chan_color  = kAzure+3;
Int_t bg_Wt_chan_color = kAzure-3; 
//---------------------------------
Int_t bg_ttbar_color     = kRed-4;       // Other top (must include ttbar, sgtop s-channel, t-channel w different lepton)
Int_t bg_wLFjets_color   = kGreen-7;   // W+light jets
Int_t bg_wHFjets_color   = kGreen-3;   // W+heavy flavour
Int_t bg_Z_diboson_color = kOrange+1;    // Z+jets, Diboson -----> (NB: GROUPED TOGETHER for MORIOND 2012
Int_t bg_qcdjets_color   = kMagenta+2;   // Multijets

// names
TString sg_tchannel_name       = "t-channel";
TString sg_Wtchannel_name      = "Wt-channel";
//---------------------------------
TString bg_ttbar_name          = "MC t#bar{t} ";
TString bg_wLFjets_groupname   = "MC W + LF jets";
TString bg_wHFjets_groupname   = "MC W + HF jets";
TString bg_Z_diboson_groupname = "MC Z + jets";
// TString bg_Z_diboson_groupname = "MC Z and diboson";
TString bg_qcdjets_name        = "QCD Multijets (Data Driven)";
