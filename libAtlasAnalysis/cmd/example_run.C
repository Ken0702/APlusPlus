void example_run(const char* InputTree = "t_app",
		 //		 const char* InputFiles = "/datm6/atlas/MC/singletop/t_channel/enu/app/mc08.108340.st_tchan_enu_McAtNlo_Jimmy.recon.APP.e360_s462_r541_tid027131.root",
		 //const char* InputFiles = "../../../../mandry/zee.np0.app.root",

		 //const char* InputFiles = "/dats2/atlas/mandry/app_20_02_2010/user09.RoccoMandrysch.ganga.mc08.107683.AlpgenJimmyWenuNp3_pt20.merge.AOD.e368_s462_s520_r808_r838.app.test_20.02.2010.CSCS-LCG2_SCRATCHDISK/user09.RoccoMandrysch.ganga.mc08.107683.AlpgenJimmyWenuNp3_pt20.merge.AOD.e368_s462_s520_r808_r838.app.test_20.02.201002200254300248._00002.app.root",

		 // const char* InputFiles = "/datm2/atlas/MC/Z+jets/ee/Np0/app/mc08.107650.AlpgenJimmyZeeNp0_pt20.recon.AOD.e376_s462_r563_tid032322.root",
		 
		 //const char* InputFiles = "/datm1/atlas/MC/W+jets/enu/Np4/app/mc08.107684.AlpgenJimmyWenuNp4_pt20.recon.AOD.e368_s462_r563_tid040124.root",
		 
		 //const char* InputFiles = "/datm1/atlas/MC/W+jets/enu/Np0/app/mc08.107680.AlpgenJimmyWenuNp0_pt20.recon.AOD.e368_s462_r563_tid039012.root",
		 
		 //const char* InputFiles = "/datm8/atlas/MC/WZ+jets/Np0/app/mc_WZplusJets_Np0.root",

		 //		 const char* InputFiles = "/dats1/atlas/mandry/app_20_02_2010/user09.RoccoMandrysch.ganga.mc08.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e405_s495_s520_r808_r838.app.test_20.02.2010.LRZ-LMU_SCRATCHDISK/user09.RoccoMandrysch.ganga.mc08.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e405_s495_s520_r808_r838.app.test_*.root",

		 const char* OutputFile = "Wt_hist.root",
		 Int_t NEvt = 2000)
  
{
  
    // Small run script for testing the A++ example analysis
    // $Id$
  
    //
    TChain *ch = new TChain(InputTree);
    ch->Add("/dats3/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.108346.st_Wt_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.08.05.2010.app.test/user10.RoccoMandrysch.mc09_7TeV.108346.st_Wt_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.08.05.2010.app.test._*.root");
    // ch->Add(InputFiles);
    AtlExampleAnalysis *sel = new AtlExampleAnalysis(OutputFile);
    // AtlZ0Finder *Z0Reco = new AtlZ0Finder("Z0Reco", "Z0 Reconstruction");
    AtlSgTop_WtChannelFinder *WtReco = new AtlSgTop_WtChannelFinder ("WtReco", "Wt Reconstruction");
    sel->AddTool(WtReco);
    ch->Process(sel, "", NEvt);
}


