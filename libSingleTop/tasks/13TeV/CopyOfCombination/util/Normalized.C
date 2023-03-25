HepDataMCPlot* GetPlot(TString Name){
	HepDataMCPlot* Plot;
	TFile* File = new TFile(Name,"READ");
	Plot = dynamic_cast<HepDataMCPlot*> (((TFile*)File->Get("MemDiscriminant"))->Get("sChannelProb"));
	if(Plot) return Plot;
	else {
		std::cerr<<"GetPlot:: Plot null"<<std::endl;
		return Plot;
	}
	
}

TH1* GetSChannel(HepDataMCPlot* Plot1){
	TH1* Result ;
	THStack* Stack = Plot1->GetMCHistStack();
	TList* List = Stack->GetHists();
	
	TIter next(List);
	while (TH1 *hist = dynamic_cast<TH1*>( next()) )
		if(((TString)hist->GetName())== "sChannelProb_mc98")
   			Result = hist;	

	if(Result)
	return Result;
	
	else
	std::cerr<<"GetSCHannel null"<<std::endl;
	return Result;
}

TH1* Normalize(TH1* Hist){
	int Total = Hist->Integral();
	Hist->Scale(1./Total);

	return Hist;
}

void AddToStack(THStack* HistStack, TString Title, TString File){
	HepDataMCPlot* Plot1= GetPlot(File);
	TH1* Hist = Normalize(GetSChannel(Plot1));
	Hist->SetLineColor(HistStack->GetNhists()+1);
	Hist->SetMarkerColor(0);
	Hist->GetYaxis()->SetRangeUser(0,0.5);
	Hist->SetOption("L H");
	Hist->SetTitle(Title);
	Hist->SetFillColor(0);
	HistStack->Add(Hist);
	std::cout<<Hist->Integral()<<std::endl;
	return ;
}


void  Normalized(){
	TCanvas* c1 = new TCanvas();
	THStack* HistStack = new THStack ("Name","Comparison");
	AddToStack(HistStack,"0.8 (0.8 * 2jsChannel)","/rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.8_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/MCPlotter.root");
	AddToStack(HistStack,"0.65","/rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.65_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/MCPlotter.root");	
	AddToStack(HistStack,"0.5","/rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.5_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/MCPlotter.root");	
	AddToStack(HistStack,"0.1 (0.1 * 2jsChannel)","/rdsk/dats1/atlas/kkreul/combination/A++/v02_v32lj_2jVs3jRatioTest_0.1_MC16a/sgtop_tchan_signal/AtlSgTop_tChannelAnalysis/MemDisc/2Jets/lnu/nominal/SgTop_s_QCDsplit_wjetsMerged/MCPlotter.root");	


	gStyle->SetLegendBorderSize(1);
	gStyle->SetLegendFillColor(0);
	gStyle->SetLegendFont(42);
	gStyle->SetLegendTextSize(0.);

	

	HistStack->Draw("nostack HIST");
	TLegend* Legend = gPad->BuildLegend();
	Legend->SetHeader("sample: s-channel");
	HistStack->GetXaxis()->SetTitle("s-channel discriminant");

	c1->Modified();
	c1->SaveAs("PlotNormalized.pdf");
	return;



}
