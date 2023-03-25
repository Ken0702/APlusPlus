void hfor_split(const char* InputFile, const char* SubDir) {
    //
    // HFOR splitting for given input file. The splitted output files
    // are written into the given sub-directory (must exist)
    //
    cout << "Processing file " << InputFile << " ..." << endl;
    
    // Open input file for reading
    TFile  *f_in = new TFile(InputFile, "read");
    TTree *t_in = (TTree*)f_in->Get("physics");
    Int_t n_in = t_in->GetEntries();

	// Loop over all valid HFOR types (0-3)
	for ( Int_t hfor = 0; hfor < 4; hfor++ ) {
	    
	    // Open output file
	    TString fname_dir(gSystem->DirName(InputFile));
	    TString fname_base(gSystem->BaseName(InputFile));
	    fname_base.ReplaceAll("user.", Form("%s/user.", SubDir));
	    fname_base.ReplaceAll(".root", Form("_hfor%d.root", hfor));
	    TString fname_out(fname_dir);
	    fname_out.Append("/");
	    fname_out.Append(fname_base);
	    
	    TFile *f_out = new TFile(fname_out.Data(), "recreate");
	    
	    // Copy tree
	    TTree *t_out = t_in->CopyTree(Form("top_hfor_type==%d", hfor));
	    Int_t n_out = t_out->GetEntries();
	    Float_t scale;
	    if ( n_in == 0) {
		scale = 0;
	    } else {
		scale = (Float_t)n_out/n_in;
	    }
	    printf("HFOR = %d :   selected events = %5d of %5d   scale = %.4f\n",
		   hfor, n_out, n_in, scale);
	    
	    // Copy histograms
	    copy_folder(f_in, f_out, 1.0); //scaled with 1, that all splitted files have same histograms from event generation
	    
	    f_out->Write();
	    delete f_out;
	}
}
void copy_folder(TDirectory *SrcDir, TDirectory *DestDir, Float_t scale) {
    //
    // Recursively copying of the content (histograms, subfolders) of the given source
    // directory to the destination folder.
    // All histograms will be scaled by the given factor.
    //

    // Search source content
    SrcDir->cd();
    
    TIter next_key(SrcDir->GetListOfKeys());
    TKey *key = 0;
    while ( (key = (TKey*)next_key()) ) {
	if ( strcmp(key->GetClassName(), "TDirectoryFile") == 0 ) {
	    TDirectory *NewSrcDir = (TDirectoryFile*)key->ReadObj();
 	    DestDir->cd();
 	    TDirectory *NewDestDir = DestDir->mkdir(key->GetName());
	    copy_folder(NewSrcDir, NewDestDir, scale);
	} else if ( (strcmp(key->GetClassName(), "TH1F") == 0) ) {
	    DestDir->cd();
	    TH1F *h1f = (TH1F*)key->ReadObj()->Clone();
	    h1f->SetDirectory(DestDir);
	    h1f->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH1D") == 0) ) {
	    DestDir->cd();
	    TH1D *h1d = (TH1D*)key->ReadObj()->Clone();
	    h1d->SetDirectory(DestDir);
	    h1d->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH2F") == 0) ) {
	    DestDir->cd();
	    TH2F *h2f = (TH2F*)key->ReadObj()->Clone();
	    h2f->SetDirectory(DestDir);
	    h2f->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH2D") == 0) ) {
	    DestDir->cd();
	    TH2D *h2d = (TH2D*)key->ReadObj()->Clone();
	    h2d->SetDirectory(DestDir);
	    h2d->Scale(scale);
	}
    }
}
