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

    // =================
    // Add input ntuples
    // =================
    mgr->SetInputTreeName("physics");
    mgr->AddInputFile("INPUTFILE");
    mgr->SetEvtReader(new MemEvtReaderD3PDSgTopRun2);

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
