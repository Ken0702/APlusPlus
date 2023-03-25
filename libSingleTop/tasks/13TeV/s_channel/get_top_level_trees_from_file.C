// root script to print out list of all top-level trees in a file
void get_top_level_trees_from_file(const char* infilename="in.txt") {
    printf("# This is %s called in path %s on file %s",
           __FUNCTION__, gSystem->pwd(), infilename);
    auto infile = TFile::Open(infilename);
    printf("\n\n## BEGIN_LIST_OF_TREES\n");
    for (const auto&& key : *infile->GetListOfKeys()) {
        TTree* t;
        infile->GetObject(key->GetName(), t);
        if (nullptr != t)
            printf("%s\n", key->GetName());
    }
    printf("## END_LIST_OF_TREES\n\n");
}
