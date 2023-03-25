{
    // taken from: $LIBMEM/test/run_master.C
    // Load libriaries on grid machines
    if ( gSystem->Getenv("PandaID") != 0 ) {
	cout << "Loading MemTk libraries..." << endl << endl;
	gSystem->Load("libEG.so");
	gSystem->AddIncludePath(" -I$LIBMEM/inc ");
	gInterpreter->AddIncludePath("$LIBMEM/inc");
	gSystem->Load("$LIBMEM/lib/$ARCH_TYPE/libMEM.so");
    }
    
    // ==================
    // Create MEM manager
    // ==================
    MemMgr *mgr = new MemMgr;
    mgr->SetCollider(MemMgr::kPP, 13000.);
    mgr->SetCtrlPlots(kFALSE);
    mgr->SetNSubjobs(NSUBJOBS);
    
    // ========================
    // Parton density functions
    // ========================
    mgr->SetPdfMgr("cteq66");
    
    // =============================
    // ATLAS MC12 Transfer Functions
    // =============================
    MemTFcnSet *tfcnSetAtlasMC12 = new MemTFcnSet(MemTFcnAtlasBase::kMC12);
    tfcnSetAtlasMC12->SetMetTFcnMag(0);
    tfcnSetAtlasMC12->SetMetTFcnPhi(0);
	
    MemTFcnSet *tfcnSetAtlasMC12_lepDelta = new MemTFcnSet(MemTFcnAtlasBase::kMC12);
    tfcnSetAtlasMC12_lepDelta->SetMuonTFcnMagnitude(new MemTFcnDelta(MemTFcnBase::kPt));
    tfcnSetAtlasMC12_lepDelta->SetElectronTFcnMagnitude(new MemTFcnDelta(MemTFcnBase::kEnergy));
    tfcnSetAtlasMC12_lepDelta->SetMetTFcnMag(0);
    tfcnSetAtlasMC12_lepDelta->SetMetTFcnPhi(0);
    
    // ========================
    // Create and add processes
    // ========================

    // SgTop s-channel, 2 partons
    MemProcSgTop_sChannel_2jets *proc_sChannel_2j
	= new MemProcSgTop_sChannel_2jets("sChannel2j",
					  "SgTop s-channel, 2 jets",
					  172.5, kTRUE);
    proc_sChannel_2j->GetMCMgr()->SetEpsRel(0.05);
    proc_sChannel_2j->GetMCMgr()->SetMaxEval(5e5);
    proc_sChannel_2j->GetMCMgr()->SetNStart(4000);
    proc_sChannel_2j->GetMCMgr()->SetNIncrease(2000);
    proc_sChannel_2j->SetTFcnSet(tfcnSetAtlasMC12);
    mgr->AddProcess(proc_sChannel_2j);

    // SgTop s-channel, 3 partons
    MemProcSgTop_sChannel_3jets *proc_sChannel_3j
	= new MemProcSgTop_sChannel_3jets("sChannel3j",
					  "SgTop s-channel, 3 jets",
					  172.5, kTRUE);
    proc_sChannel_3j->GetMCMgr()->SetEpsRel(0.15);
    proc_sChannel_3j->GetMCMgr()->SetMaxEval(5e5);
    proc_sChannel_3j->GetMCMgr()->SetNStart(4e4);
    proc_sChannel_3j->GetMCMgr()->SetNIncrease(0.0);
    proc_sChannel_3j->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    mgr->AddProcess(proc_sChannel_3j);
	
    // SgTop t-channel, 4-flavour scheme, 3 partons
    MemProcSgTop_tChannel_4FS_3jets *proc_tChannel_4FS =
	new MemProcSgTop_tChannel_4FS_3jets("tChannel4FS",
					    "SgTop t-channel 4 flavour scheme",
					    172.5);
    proc_tChannel_4FS->GetMCMgr()->SetEpsRel(0.2);
    proc_tChannel_4FS->GetMCMgr()->SetMaxEval(5e5);
    proc_tChannel_4FS->GetMCMgr()->SetNStart(20000);
    proc_tChannel_4FS->GetMCMgr()->SetNIncrease(6600);
    proc_tChannel_4FS->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    mgr->AddProcess(proc_tChannel_4FS);
	
    // Ttbar semi-leptonic
    MemProcTTbar *procTTSL = new MemProcTTbar("ttbarSL", "ttbar semi-leptonic",
					      2.0 * 172.5,
					      1, kFALSE,
					      172.5, kTRUE);
    procTTSL->GetMCMgr()->SetEpsRel(0.1);
    procTTSL->GetMCMgr()->SetMaxEval(5e5);
    procTTSL->GetMCMgr()->SetNStart(6e4);
    procTTSL->GetMCMgr()->SetNIncrease(5e3);
    procTTSL->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    mgr->AddProcess(procTTSL);
    
    // Ttbar di-leptonic
    MemProcTTbar *procTTDL = new MemProcTTbar("ttbarDL", "ttbar di-leptonic",
					      2.*172.5,
					      2, kFALSE,
					      172.5, kTRUE);
    procTTDL->SetValidNJets(2, 2);
    procTTDL->GetMCMgr()->SetEpsRel(0.1);
    procTTDL->GetMCMgr()->SetMaxEval(5e5);
    procTTDL->GetMCMgr()->SetNStart(4e4);
    procTTDL->GetMCMgr()->SetNIncrease(8e3);
    procTTDL->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    mgr->AddProcess(procTTDL);
	
    // W + jj
    MemProcW2jets *procW2jets = new MemProcW2jets("Wjj", "W + 2 jets", 80.4);
    procW2jets->GetMCMgr()->SetEpsRel(0.05);
    procW2jets->GetMCMgr()->SetMaxEval(5e4);
    procW2jets->GetMCMgr()->SetNStart(2500);
    procW2jets->GetMCMgr()->SetNIncrease(500);
    procW2jets->SetTFcnSet(tfcnSetAtlasMC12);
    procW2jets->SetUseMisTags(kTRUE);
    mgr->AddProcess(procW2jets);

    // W + bb
    MemProcWbb *procWbb = new MemProcWbb("Wbb", "W + bb", 80.4);
    procWbb->GetMCMgr()->SetEpsRel(0.05);
    procWbb->GetMCMgr()->SetMaxEval(5e4);
    procWbb->GetMCMgr()->SetNStart(1600);
    procWbb->GetMCMgr()->SetNIncrease(500);
    procWbb->SetTFcnSet(tfcnSetAtlasMC12);
    mgr->AddProcess(procWbb);

    // W + cj
    MemProcWcjet *procWcj = new MemProcWcjet("Wcj", "W + c + jet", 80.4);
    procWcj->GetMCMgr()->SetEpsRel(0.05);
    procWcj->GetMCMgr()->SetMaxEval(5e4);
    procWcj->GetMCMgr()->SetNStart(2500);
    procWcj->GetMCMgr()->SetNIncrease(800);
    procWcj->SetUseMisTags(kTRUE);
    procWcj->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    mgr->AddProcess(procWcj);

    // // W + H -> bb
    // MemProcWHbb *procWHbb = new MemProcWHbb("WHbb", "W+H Higgs-Strahlung, H->bb", 205.8);
    // procWHbb->GetMCMgr()->SetEpsRel(0.05);
    // procWHbb->GetMCMgr()->SetMaxEval(5e5);
    // procWHbb->GetMCMgr()->SetNStart(4e3);
    // procWHbb->GetMCMgr()->SetNIncrease(2e3);
    // procWHbb->SetTFcnSet(new MemTFcnSet(MemTFcnAtlasBase::kMC12));
    // mgr->AddProcess(procWHbb);

    // // W + j + H -> bb
    // MemProcWHbbj *procWHbbj = new MemProcWHbbj("WHbbj", "W+H+jet Higgs-Strahlung, H->bb", 205.8);
    // procWHbbj->GetMCMgr()->SetEpsRel(0.15);
    // procWHbbj->GetMCMgr()->SetMaxEval(5e5);
    // procWHbbj->GetMCMgr()->SetNStart(1.6e4);
    // procWHbbj->GetMCMgr()->SetNIncrease(5e3);
    // procWHbbj->SetTFcnSet(tfcnSetAtlasMC12_lepDelta);
    // mgr->AddProcess(procWHbbj);
	
    // =================
    // Add input ntuples
    // =================
    mgr->SetInputTreeName("physics");
    mgr->AddInputFile("INPUTFILE");
    mgr->SetEvtReader(new MemEvtReaderD3PDSgTopRun2("v20lj"));
    
    // =================
    // Set output ntuple
    // =================
    mgr->SetOutputFile("OUTPUTFILE");
    mgr->SetOutputTree("physics", "SgTop-D3PD + MEM");
    mgr->SetEvtWriter(new MemEvtWriterInclJobInfo);
    mgr->SetWriteAllEvents(kTRUE);
    
    // ============
    // Run analysis
    // ============
    mgr->RunSubjob(SUBJOBID);
}
