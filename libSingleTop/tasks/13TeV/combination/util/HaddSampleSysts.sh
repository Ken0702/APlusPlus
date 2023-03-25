#!/bin/bash

CWD=$(pwd)
cd /rdsk/dat23/atlas/kkreul/combination/A++/v02_v34lj_sampleSysts_MC16e/rucio/download
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410644schanlt.root  user.kkreul.NewSampleSyst.sChanPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410645schanltbar.root  user.kkreul.NewSampleSyst.sChanBarPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600020schanltPH7.root  user.kkreul.NewSampleSyst.sChanBarPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600019schanltbarPH7.root  user.kkreul.NewSampleSyst.sChanPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_503581schanltaMCP8.root  user.kkreul.NewSampleSyst.sChanAMCP8_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_500288schanltbaraMCH7.root  user.kkreul.NewSampleSyst.sChanaMCH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410658tchanlt.root  user.kkreul.NewSampleSyst.tChanPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410659tchanltbar.root  user.kkreul.NewSampleSyst.tChanBarPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600018tchanltPH7.root  user.kkreul.NewSampleSyst.tChanPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600017tchanltbarPH7.root  user.kkreul.NewSampleSyst.tChanBarPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_503582tchanltaMCP8.root  user.kkreul.NewSampleSyst.tChanaMCP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_500289tchanltaMCH7.root  user.kkreul.NewSampleSyst.tchanaMCH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410646Wt.root  user.kkreul.NewSampleSyst.tWPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410647Wtbar.root  user.kkreul.NewSampleSyst.tWBarPP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410654WtDS.root  user.kkreul.NewSampleSyst.tWPP8FS_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410655WtbarDS.root  user.kkreul.NewSampleSyst.tWBarPP8FS_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600727WtPH7.root  user.kkreul.NewSampleSyst.tWPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_600728WtbarPH7.root  user.kkreul.NewSampleSyst.tWBarPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_412002WtaMCP8.root  user.kkreul.NewSampleSyst.tWaMCP8AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410470ttbar.root  user.kkreul.NewSampleSyst.ttbarhdamp15_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410480ttbar1lhdamp3.root  user.kkreul.NewSampleSyst.ttbar1lhdamp3_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410482ttbar2lhdamp3.root  user.kkreul.NewSampleSyst.ttbar2lhdamp3_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411356ttbar1lhdamp2.root  user.kkreul.NewSampleSyst.ttbar1lhdamp2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411358ttbar2lhdamp2.root  user.kkreul.NewSampleSyst.ttbar2lhdamp2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411353ttbar1lhdamp18.root  user.kkreul.NewSampleSyst.ttbar1lhdamp18_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411355ttbar2lhdamp18.root  user.kkreul.NewSampleSyst.ttbar2lhdamp18_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411350ttbar1lhdamp13.root  user.kkreul.NewSampleSyst.ttbar1lhdamp13_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411352ttbar2lhdamp13.root  user.kkreul.NewSampleSyst.ttbar2lhdamp13_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411233ttbar1lPH713.root  user.kkreul.NewSampleSyst.ttbar1lPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_411234ttbar2lPH713.root  user.kkreul.NewSampleSyst.ttbar2lPH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410464ttbar1laMCP8.root  user.kkreul.NewSampleSyst.ttbar1laMCP8_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_410465ttbar2laMCP8.root  user.kkreul.NewSampleSyst.ttbar2laMCP8_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_412116ttbar1laMCH713.root  user.kkreul.NewSampleSyst.ttbar1laMCH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_412117ttbar2laMCH713.root  user.kkreul.NewSampleSyst.ttbar2laMCH7AF2_[1-10]_nominal_v34lj_MC16e.v[1-3]_mem_output.root/* 
cd $CWD
