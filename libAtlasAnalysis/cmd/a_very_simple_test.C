void a_very_simple_test(const char* TopViewFile = "/users/eeh/kind/atlas/top/data/ttbar/user.top.TopView121302_MuidTauRec.trig1_misal1_mc12.005200.T1_McAtNlo_Jimmyv12000601.001.AANT0._00001.root") {
    
    // Open file
    TFile *_file0 = TFile::Open(TopViewFile);
    
    // Fetch trees
    TTree *tc = (TTree*)gDirectory->Get("CollectionTree");
    TTree *tf = (TTree*)gDirectory->Get("FullReco0");
    TTree *tt = (TTree*)gDirectory->Get("Truth0");

    // Fix the screwed-up TopView design
    tf->SetTreeIndex(0);
    tt->SetTreeIndex(0);
    tc->AddFriend(tf);
    tc->AddFriend(tt);
    
    // Create an analysis selector and initialise
    AtlTopViewAnalysis *sel =
	new AtlTopViewAnalysis(tc, "test.root", "t_ttbar",
			       "MC@NLO ttbar");
    sel->Init(tc);
    
    // Process the first ten events
    // and write output tree
    tc->Process(sel, "", 10);
}
