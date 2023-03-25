void analyze_appfile(const char* appfile) {
    //
    // Analyze content of the given A++ file
    //

    // Open file
    TFile *f = new TFile(appfile, "READ");
    AtlEvent *evt = new AtlEvent;
    TTree *t = (TTree*)f->Get("t_app");
    t->SetBranchAddress("Event", &evt);
    t->SetBranchStatus("*", kFALSE);
    t->SetBranchStatus("fEventHeader*", kTRUE);

    cout << endl
	 << "Contents of A++ file \"" << appfile << "\":"
	 << endl << endl;

    // No. of events
    Int_t nevt = t->GetEntries();
    cout << "NEvt = " << nevt << endl << endl;
    
    // Event loop
    Int_t run = 0;
    Int_t LB = 0;
    Int_t LBend   = 0;
    for ( Int_t ievt = 0; ievt < nevt; ievt++ ) {
	evt->Clear();
	t->GetEntry(ievt);
	
	// Lumi block has changed ?
	if ( evt->LumiBlock() != LB ) {
	    if ( run != 0 ) 
		cout << " End=" << LB << endl;
	    
	    // Run number has changed ?
	    if ( evt->RunNr() != run ) {
		run = evt->RunNr();
		cout << "Run " << run << ":" << endl;
	    }
	    
	    LB = evt->LumiBlock();
	    cout << "LB Range Start=" << LB;
	}
    }
    cout << " End=" << LB << endl
	 << endl;
}

