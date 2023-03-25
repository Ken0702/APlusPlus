void testbeam_conversion_test(const char* TestBeamFiles = "/users/eeh/danri/data/new_data/cbnt_*2102103*.root", const char* OutputFilePrefix = "test", Long64_t Entries = 100, Int_t NtupleVersion = 0) {

    TStopwatch watch;
    
    // Open file
    TChain* InputChain = new TChain("tree");
    InputChain->Add(TestBeamFiles);
    
    // Create testbeam run info object
    AtlTestBeamRunInfo* RunInfo = new AtlTestBeamRunInfo();
    RunInfo->SetName("CTB_RunInfo");
    RunInfo->SetTitle("Combined Test Beam run information");
    
    // Get the run information from the filename
    TString tmpString( ((TChainElement*)InputChain->GetListOfFiles()->At(0))->GetTitle() );

    if( NtupleVersion == 0 ) {

        /////////////////////////////////////////////////
        // Run Info for NEW version CBNT. We use some
	// regular expressions to get the information
	// we need from the file name                  //
        /////////////////////////////////////////////////
	
    	TPRegexp RunRegExp("(?<=_1206_)[0-9]+(?=\.B[-0-9]+\.)");
	TPRegexp EnergyRegExp("(?<=M[01]\.)[0-9]+(?=GeV)");
	TPRegexp BFieldRegExp("(?<=\.B)[-0-9]+(?=\.M[01]\.)");
	TPRegexp MaterialRegExp("(?<=\.M)[01](?=\.[0-9]+)");

	// SetRunInfo accordingly	
	TString RunNumber =  TString( tmpString(RunRegExp) );
	TString BeamEnergy = TString( tmpString(EnergyRegExp) );
	TString BFieldCurrent = TString( tmpString(BFieldRegExp) );
	TString Material = TString( tmpString(MaterialRegExp) );
	
	if ( RunNumber.IsNull() || BeamEnergy.IsNull() ||
	     BFieldCurrent.IsNull() || Material.IsNull() ) {
	    Error("testbeam_conversion_test","At least one of the RunInfo data members will be empty...aborting");
	    gSystem->Abort(1);
	}
	
	RunInfo->SetRunNumber( RunNumber.Atoi() );
	RunInfo->SetBeamEnergy( BeamEnergy.Atof() );
	RunInfo->SetBFieldCurrent( BFieldCurrent.Atof() );
	RunInfo->SetMaterial( Material.Atoi() );
	
    } else if ( NtupleVersion == 1 ) {

	////////////////////////////////////////////////
	// Run Info for OLD version CBNTs. Get the
	// correct line in the run configuration file
	// and save the information in RunInfo        //      
	////////////////////////////////////////////////

	TPRegexp RunRegExp("(?<=cbnt_RecExTB_Combined_1205_)[0-9]+(?=\.[0-9]\.root)");
	TString RunNo = tmpString(RunRegExp);
	
	if ( RunNo.Length() != 7 ) {
	    Error("testbeam_conversion_test","Length of run number string is invalid: %d...aborting", RunNo.Length()); 
	    gSystem->Abort(1);
	}
	    
	TString RunConfigFileName = "/users/eeh/danri/data/RunList.txt"; 
	ifstream RunFile(RunConfigFileName.Data());
	
	if ( !(&RunFile) || RunFile.bad() ) {
	    Error("testbeam_conversion_test","Could not open RunList.txt");
	    gSystem->Abort(1);
	}
	
	Int_t LineNumber(0);
	Int_t RunSuffix(0);
	Float_t Energy(0.);
	Float_t Current(0.);
	Int_t Geometry(0);
	Int_t BeamType(0);
	
	while (!RunFile.eof()) {
	    RunFile >> LineNumber >> RunSuffix >> Energy >> Current >> Geometry >> BeamType;
	    if ( RunSuffix == TString(RunNo(3,4)).Atoi() )
		break;
	}
	
	RunFile.close();

	// Set RunInfo accordingly
	RunInfo->SetRunNumber( RunNo.Atoi() );
	RunInfo->SetBeamEnergy( Energy );
	RunInfo->SetBFieldCurrent( Current );
	if ( Geometry < 5 ) RunInfo->SetMaterial(0); else RunInfo->SetMaterial(1);
    }

    RunInfo->Print();

    cout << "The following files will be processed:" << endl;
    cout << "----------------------------------------" << endl;
    InputChain->GetListOfFiles()->Print();
   
    
    // Create an analysis selector and initialise
    AtlTestBeamConversion *sel =
	new AtlTestBeamConversion(InputChain, OutputFilePrefix, "tree", RunInfo, NtupleVersion);
    sel->Init(InputChain);
    
    // Process some events and write output trees
    if ( Entries == -1 ) Entries = InputChain->GetEntries();
    cout << "Looping over " << Entries << " events...\n" << endl;
    InputChain->Process(sel,"", Entries );

    sel->PrintPIDResults();
    
    watch.Stop();

    cout << "\n==================================================================" << endl;
    cout << " +++ Conversion job for run " << RunInfo->RunNumber() << " finished." << endl;
    cout << "     "; watch.Print(); cout << endl;

}
