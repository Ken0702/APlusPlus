class PlotHelper : public TObject {
    
public:
    // Define regions to set SF globally
    enum ERegion { kSchanSR_lnu = 0,
		   kSchanSR_enu,
		   kSchanSR_munu,
		   kSchanWCR_lnu,
		   kSchanWCR_enu,
		   kSchanWCR_munu,
		   kSchanTtbarCR_lnu,
		   kSchanTtbarCR_enu,
		   kSchanTtbarCR_munu,
		   kTchanSR_lnu,
		   kTchanSR_enu,
		   kTchanSR_munu,
		   kTchanWCR_lnu,
		   kTchanWCR_enu,
		   kTchanWCR_munu,
		   kTchanTtbarCR_lnu,
		   kTchanTtbarCR_enu,
		   kTchanTtbarCR_munu };

    // Global options for all plots
    static Double_t fgSF_schan;
    static Double_t fgSF_tchan;
    static Double_t fgSF_Wt;
    static Double_t fgSF_ttbar;
    static Double_t fgSF_Wjets;
    static Double_t fgSF_WjetsLF;
    static Double_t fgSF_WjetsHF;
    static Double_t fgSF_ZjetsVV;
    static Double_t fgSF_QCD;

    static Double_t fgUnc_schan;
    static Double_t fgUnc_tchan;
    static Double_t fgUnc_Wt;
    static Double_t fgUnc_ttbar;
    static Double_t fgUnc_Wjets;
    static Double_t fgUnc_WjetsLF;
    static Double_t fgUnc_WjetsHF;
    static Double_t fgUnc_ZjetsVV;
    static Double_t fgUnc_QCD;

    static Bool_t fgDataMCRatioAtOne;
    static Bool_t fgDrawDataErrorX;
    static Bool_t fgDrawNormUncertainty;
    static Bool_t fgGridx;
    static Bool_t fgGridy;
    
    static TString *fgLumiLabel;
    static TString *fgAtlasStatus;
    static TString *fgRegion;
    
    static Int_t NdivisionsY;

    static TList *fLegendNames;
    
    Double_t fXmin;
    Double_t fXmax;

    Double_t fYmin;
    Double_t fYmax;
    
    Double_t fRatioYmin;
    Double_t fRatioYmax;

    Double_t fLegX1;
    Double_t fLegY1;
    Double_t fLegX2;
    Double_t fLegY2;

    Double_t fAtlasStatusX;
    Double_t fAtlasStatusY;
    Double_t fCMSLumiX;
    Double_t fCMSLumiY;
    Double_t fRegionX;
    Double_t fRegionY;

    Float_t  fScaleOverlay;
    
    TString *fXTitle;
    TString *fYTitle;
    
    Int_t fRebin;

    PlotHelper::PlotHelper();

    void Draw(Option_t *option = "");
    void DrawSchannelXaxis(Double_t posy = -90.);
    void DrawTchannelXaxis(Double_t posy = -200.);

    void AddTicks(Double_t x, Double_t ymin, Double_t ymax,
		  Double_t scale = 1.);
    void AddLabel(TPad *pad, Double_t x, Double_t y,
		  const char* label, Double_t lsize,
		  Double_t scale = 1.);
    void SetLogy(Bool_t logy = kTRUE);

    inline HepDataMCPlot* GetHepPlot() { return fHepPlot; }
    inline void SetAtlasCMSLumiX(Double_t x) {
	fAtlasStatusX = x; fCMSLumiX = x; fRegionX = x; }
    inline void SetLegendPos(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
	{ fLegX1 = x1; fLegY1 = y1; fLegX2 = x2; fLegY2 = y2; }

    static void SetScaleFactors(PlotHelper::ERegion region);
    
private:
    HepDataMCPlot *fHepPlot;

    void RenameLegend();
    void ScalePlot();
    void SetNormUncertainties();
    void SetLegendPos();
    void DrawRegion();
};

Double_t PlotHelper::fgSF_schan = 1.;
Double_t PlotHelper::fgSF_tchan = 1.;
Double_t PlotHelper::fgSF_Wt    = 1.;
Double_t PlotHelper::fgSF_ttbar = 1.;
Double_t PlotHelper::fgSF_Wjets = 1.;
Double_t PlotHelper::fgSF_WjetsLF = 1.;
Double_t PlotHelper::fgSF_WjetsHF = 1.;
Double_t PlotHelper::fgSF_ZjetsVV = 1.;
Double_t PlotHelper::fgSF_QCD   = 1.;

Double_t PlotHelper::fgUnc_schan = 0.04;
Double_t PlotHelper::fgUnc_tchan = 0.04;
Double_t PlotHelper::fgUnc_Wt    = 0.07;
Double_t PlotHelper::fgUnc_ttbar = 0.06;
Double_t PlotHelper::fgUnc_Wjets = 0.6;
Double_t PlotHelper::fgUnc_WjetsLF = 0.3;
Double_t PlotHelper::fgUnc_WjetsHF = 0.6;
Double_t PlotHelper::fgUnc_ZjetsVV = 0.6;
Double_t PlotHelper::fgUnc_QCD   = 0.5;

Bool_t PlotHelper::fgDataMCRatioAtOne = true;
Bool_t PlotHelper::fgDrawDataErrorX   = true;
Bool_t PlotHelper::fgDrawNormUncertainty = false;
Bool_t PlotHelper::fgGridx = false;
Bool_t PlotHelper::fgGridy = false;
    
TString* PlotHelper::fgLumiLabel = new TString("36.1 fb^{-1}");
TString* PlotHelper::fgAtlasStatus = new TString("Internal");
TString* PlotHelper::fgRegion = 0;
    
Int_t PlotHelper::NdivisionsY = 505;

TList* PlotHelper::fLegendNames = 0;

PlotHelper::PlotHelper() {
    SetDefaults();
}

PlotHelper::PlotHelper(TFile *f,
		       const char *PlotName) {
    SetDefaults();
    
    fHepPlot = (HepDataMCPlot*) f->Get(PlotName);
    if ( fHepPlot == 0 ) {
	Error("PlotHelper", "HepDataMCPlot %s not found.", PlotName);
	gSystem->Abort();
    }
}

void PlotHelper::Draw(Option_t *option) {

    if ( fHepPlot == 0 ) {
	Error("Draw", "fHepPlot == 0");
	gSystem->Abort();
    }

    // Y-Range
    if ( fYmin != fYmax )
	fHepPlot->SetYRange(fYmin, fYmax);
    
    // Axis titles
    if ( fXTitle != 0 )
	fHepPlot->SetXTitle(fXTitle->Data());

    if ( fYTitle != 0 )
	fHepPlot->SetYTitle(fYTitle->Data());

    // Label Positions
    fHepPlot->SetAtlasLabelPos(fAtlasStatusX, fAtlasStatusY);
    fHepPlot->SetCMSEnergyLabelPos(fCMSLumiX, fCMSLumiY);

    // Misc. options
    fHepPlot->SetDrawDataErrorX(fgDrawDataErrorX);
    fHepPlot->SetLuminosityLabel(fgLumiLabel->Data());
    fHepPlot->SetDataMCRatioCenterOne(fgDataMCRatioAtOne);
    fHepPlot->SetAtlasLabelStatus(fgAtlasStatus->Data());

    // Scale Plot
//    ScalePlot();

    // Norm. uncertainties
    if ( fgDrawNormUncertainty )
    	SetNormUncertainties();

    // Signal overlay scale
    fHepPlot->SetScaleOverlay(fScaleOverlay);

    // Draw
    fHepPlot->Draw(option);
    
    // Rebin?
    if ( fRebin != 0 )
	fHepPlot->Rebin(fRebin);
    
    // Add region label?
    if ( fgRegion != 0 )
	DrawRegion();

    // Rename Legend
    if ( fLegendNames != 0 )
	RenameLegend();
    
    // X-range
    if ( fXmin != fXmax )
	fHepPlot->SetXRange(fXmin, fXmax);

    // Ratio options:
    if ( fHepPlot->GetRatioPad1() != 0 ) {
	// Remove grid lines
	fHepPlot->GetRatioPad1()->SetGridx(fgGridx);
	fHepPlot->GetRatioPad1()->SetGridy(fgGridy);

	// Y-range
	if ( fRatioYmin != fRatioYmax )
	    fHepPlot->SetYRangeRatio(fRatioYmin, fRatioYmax, 1);

	// Y-Title
	if ( fgDataMCRatioAtOne )
	    fHepPlot->GetHistogramRatioPad1()->SetYTitle("Data/Pred.");
	else
	    fHepPlot->GetHistogramRatioPad1()->SetYTitle("Data/Pred.-1");
    }

    // Legend position
    if ( fLegX1 != fLegX2 && fLegY1 !=fLegY2 )
	SetLegendPos();
    
    // Misc. options
    fHepPlot->SetNdivisionsY(505);
    
    // Fix too small top margin
    fHepPlot->GetMainPad()->SetTopMargin(0.06);

    // Update main pad
    fHepPlot->GetMainPad()->Modified();
    fHepPlot->GetMainPad()->Update();

    if ( fHepPlot->GetRatioPad1() != 0 ) {
	fHepPlot->GetRatioPad1()->Modified();
	fHepPlot->GetRatioPad1()->Update();
    }
}

void PlotHelper::RenameLegend() {
    // Rename Legend entries. The position in the TList must
    // corresponds to the position in the legend.

    TLegend *leg = fHepPlot->GetLegend();

    if ( fLegendNames->GetEntries() != leg->GetListOfPrimitives()->GetEntries() ) {
	Warning("RenameLegend", "Can not rename legend entries. Number of entries in TList does not match number of entries in plot");
	return;
    }

    for ( Int_t i = 0; i < fLegendNames->GetEntries(); i++ ) {
	TLegendEntry *entry = (TLegendEntry*) leg->GetListOfPrimitives()->At(i);
	TString label = ((TObjString*)fLegendNames->At(i))->GetString();
	entry->SetLabel(label.Data());
    }
    
    fHepPlot->GetMainPad()->Modified();
    fHepPlot->GetMainPad()->Update();
}

void PlotHelper::ScalePlot() {
    // ToDo: Wjets merged
    fHepPlot->ScaleMCFolder(fgSF_schan, "sgtop_sChannel");
    fHepPlot->ScaleMCFolder(fgSF_tchan, "sgtop_tChannel");
    fHepPlot->ScaleMCFolder(fgSF_Wt, "sgtop_WtChannel");
    fHepPlot->ScaleMCFolder(fgSF_ttbar, "ttbar");
    fHepPlot->ScaleMCFolder(fgSF_WjetsLF, "wjetsLF");
    fHepPlot->ScaleMCFolder(fgSF_WjetsHF, "wjetsHF");
    fHepPlot->ScaleMCFolder(fgSF_ZjetsVV, "zjets_diboson");
    fHepPlot->ScaleMCFolder(fgSF_QCD, "QCD");
}

void PlotHelper::Save(TCanvas *c) {
    c->SaveAs(Form("%s.pdf",
		   c->GetName()));
}

void PlotHelper::SetNormUncertainties() {
    fHepPlot->SetDrawNormError(kTRUE);
    TList *l = fHepPlot->GetMCFolders();
    ((HepDataMCFolder*)l->FindObject("sgtop_sChannel"))->SetNormUncertainty(fgUnc_schan);
    ((HepDataMCFolder*)l->FindObject("sgtop_tChannel"))->SetNormUncertainty(fgUnc_tchan);
    ((HepDataMCFolder*)l->FindObject("sgtop_WtChannel"))->SetNormUncertainty(fgUnc_Wt);
    ((HepDataMCFolder*)l->FindObject("ttbar"))->SetNormUncertainty(fgUnc_ttbar);
    ((HepDataMCFolder*)l->FindObject("wjetsLF"))->SetNormUncertainty(fgUnc_WjetsLF);
    ((HepDataMCFolder*)l->FindObject("wjetsHF"))->SetNormUncertainty(fgUnc_WjetsHF);
    ((HepDataMCFolder*)l->FindObject("zjets_diboson"))->SetNormUncertainty(fgUnc_ZjetsVV);
    ((HepDataMCFolder*)l->FindObject("QCD"))->SetNormUncertainty(fgUnc_QCD);
}

void PlotHelper::SetLegendPos() {
    
    TLegend *leg = fHepPlot->GetLegend();
    leg->SetX1NDC(fLegX1);
    leg->SetY1NDC(fLegY1);
    leg->SetX2NDC(fLegX2);
    leg->SetY2NDC(fLegY2);
    
    fHepPlot->GetMainPad()->Modified();
    fHepPlot->GetMainPad()->Update();
}

void PlotHelper::SetDefaults() {
    fHepPlot = 0;

    fXmin = 0.;
    fXmax = 0.;

    fYmin = 0.;
    fYmax = 0.;

    fXTitle = 0;
    fYTitle = 0;

    fRatioYmin = 0.;
    fRatioYmax = 0.;

    fRebin = 0;

    fLegX1 = 0.59;
    fLegY1 = 0.42;
    fLegX2 = 0.90;
    fLegY2 = 0.92;

    fAtlasStatusX = 0.20;
    fAtlasStatusY = 0.87;

    fCMSLumiX = 0.20;
    fCMSLumiY = 0.81;

    fRegionX  = 0.20;
    fRegionY  = 0.74;
}

void PlotHelper::DrawSchannelXaxis(Double_t posy) {
    //
    // Define x-axis for s-channel binning here
    //
    
    TPad *mp = fHepPlot->GetMainPad();
    TPad *rp = fHepPlot->GetRatioPad1();
    
    TH1 *mh = fHepPlot->GetHistogramMainPad();
    
    // Remove labels and ticks
    mh->GetXaxis()->SetLabelSize(0);
    mh->GetXaxis()->SetTickLength(0);
    mh->GetXaxis()->SetTickSize(0);

    if ( rp != 0 ) {
	TH1 *rh = fHepPlot->GetHistogramRatioPad1();
	
	// Remove labels and ticks
	rh->GetXaxis()->SetLabelSize(0);
	rh->GetXaxis()->SetTickLength(0);
	rh->GetXaxis()->SetTickSize(0);
    }

    // Add ticks to main pad or ratio pad?
    TPad *p = gPad;
    
    Double_t x1,x2,y1,y2;
    
    Double_t bw = mh->GetBinWidth(1);
    Double_t lsize = gStyle->GetLabelSize("X");
    
    mp->cd();
    
    mp->GetRangeAxis(x1,y1,x2,y2);
    
    AddTicks(1.*bw, y1, y2);
    AddTicks(5.*bw, y1, y2);
    AddTicks(9.*bw, y1, y2);
    AddTicks(13.*bw, y1, y2);
    AddTicks(19.*bw, y1, y2);
    AddTicks(23.*bw, y1, y2);
    
    // Pad for labels
    lp = mp;

    if ( rp != 0 ) {
	lp = rp;
	
	rp->cd();

	rp->GetRangeAxis(x1,y1,x2,y2);
	
	AddTicks(1.*bw, y1, y2);
	AddTicks(5.*bw, y1, y2);
	AddTicks(9.*bw, y1, y2);
	AddTicks(13.*bw, y1, y2);
	AddTicks(19.*bw, y1, y2);
	AddTicks(23.*bw, y1, y2);

	lsize = lsize/0.4;

    }

    AddLabel(lp, 1.*bw,  posy, "0.0002", lsize);
    AddLabel(lp, 5.*bw,  posy, "0.0018", lsize);
    AddLabel(lp, 9.*bw,  posy, "0.058" , lsize);
    AddLabel(lp, 13.*bw, posy, "0.102" , lsize);
    AddLabel(lp, 19.*bw, posy, "0.187" , lsize);
    AddLabel(lp, 23.*bw, posy, "1"     , lsize);

    p->cd();
}

void PlotHelper::DrawTchannelXaxis(Double_t posy) {

    TPad *mp = fHepPlot->GetMainPad();
    TPad *rp = fHepPlot->GetRatioPad1();
    
    TH1 *mh = fHepPlot->GetHistogramMainPad();
    
    // Remove labels and ticks
    mh->GetXaxis()->SetLabelSize(0);
    mh->GetXaxis()->SetTickLength(0);
    mh->GetXaxis()->SetTickSize(0);

    if ( rp != 0 ) {
	TH1 *rh = fHepPlot->GetHistogramRatioPad1();
	
	// Remove labels and ticks
	rh->GetXaxis()->SetLabelSize(0);
	rh->GetXaxis()->SetTickLength(0);
	rh->GetXaxis()->SetTickSize(0);
    }

    // Add ticks to main pad or ratio pad?
    TPad *p = gPad;
    
    Double_t x1,x2,y1,y2;
    
    Double_t bw = mh->GetBinWidth(1);
    Double_t lsize = gStyle->GetLabelSize("X");
    
    mp->cd();
    
    mp->GetRangeAxis(x1,y1,x2,y2);
    
    AddTicks(2.*bw, y1, y2);
    AddTicks(4.*bw, y1, y2);
    AddTicks(6.*bw, y1, y2);
    AddTicks(8.*bw, y1, y2);
    AddTicks(10.*bw, y1, y2);
    AddTicks(12.*bw, y1, y2);
    AddTicks(14.*bw, y1, y2);
    AddTicks(16.*bw, y1, y2);
    AddTicks(18.*bw, y1, y2);
    
    // Pad for labels
    lp = mp;

    if ( rp != 0 ) {
	lp = rp;
	
	rp->cd();

	rp->GetRangeAxis(x1,y1,x2,y2);
	
	AddTicks(2.*bw, y1, y2);
	AddTicks(4.*bw, y1, y2);
	AddTicks(6.*bw, y1, y2);
	AddTicks(8.*bw, y1, y2);
	AddTicks(10.*bw, y1, y2);
	AddTicks(12.*bw, y1, y2);
	AddTicks(12.*bw, y1, y2);
	AddTicks(14.*bw, y1, y2);
	AddTicks(16.*bw, y1, y2);
	AddTicks(18.*bw, y1, y2);
	AddTicks(20.*bw, y1, y2);

	lsize = lsize/0.4;
    }

    AddLabel(lp, 2.*bw,  posy, "0.10", lsize);
    AddLabel(lp, 4.*bw,  posy, "0.19", lsize);
    AddLabel(lp, 6.*bw,  posy, "0.29", lsize);
    AddLabel(lp, 8.*bw,  posy, "0.39", lsize);
    AddLabel(lp, 10.*bw, posy, "0.50", lsize);
    AddLabel(lp, 12.*bw, posy, "0.62", lsize);
    AddLabel(lp, 14.*bw, posy, "0.74", lsize);
    AddLabel(lp, 16.*bw, posy, "0.84", lsize);
    AddLabel(lp, 18.*bw, posy, "0.93", lsize);
    AddLabel(lp, 20.*bw, posy, "1"   , lsize);

    p->cd();
}

void PlotHelper::AddTicks(Double_t x,
			  Double_t ymin, Double_t ymax,
			  Double_t scale) {
    
    Double_t yzero  = ymin;
    Double_t yrange = ymax - ymin;
    
    // Add a tick mark (ROOT: major tick length is 3% of yrange)
    TLine *line1 = new TLine(x, yzero, x, yzero + 0.03*yrange*scale);
    TLine *line2 = new TLine(x, yzero + yrange - 0.03*yrange*scale, x, yzero + yrange);
    line1->Draw();
    line2->Draw();
}

void PlotHelper::AddLabel(TPad *pad,
			  Double_t x, Double_t y, const char* label,
			  Double_t lsize, Double_t scale) {
    
    TPad *p = gPad;
    pad->cd();

    TLatex *l = new TLatex(x, y, label);
    l->SetTextSize(lsize);
    l->SetTextAlign(21);
    l->Draw();

    p->cd();
}

void PlotHelper::SetLogy(Bool_t logy) {
   
    TPad *mp = fHepPlot->GetMainPad();
    mp->SetLogy(logy);

}

void PlotHelper::DrawRegion() {
   
    TPad *p = gPad;

    TPad *mp = fHepPlot->GetMainPad();
    mp->cd();

    TLatex *l = new TLatex(fRegionX, fRegionY, fgRegion->Data());
    l->SetTextSize(0.04);
    l->SetNDC();
    l->Draw();

    p->cd();
}

void PlotHelper::SetScaleFactors(ERegion region) {

    switch (region) {
	//
	// s-channel
	//
	case kSchanSR_lnu:
	    PlotHelper::fgSF_schan   = 1.00469;
	    PlotHelper::fgSF_tchan   = 1.00568;
	    PlotHelper::fgSF_Wt      = 0.994952;
	    PlotHelper::fgSF_ttbar   = 0.929561;
	    PlotHelper::fgSF_WjetsLF = 0.791127;
	    PlotHelper::fgSF_WjetsHF = 1.67626;
	    PlotHelper::fgSF_ZjetsVV = 1.23317;
	    PlotHelper::fgSF_QCD     = 0.486434;
	    break;
	case kSchanSR_enu:
	    PlotHelper::fgSF_schan   = 1.00441;
	    PlotHelper::fgSF_tchan   = 1.00616;
	    PlotHelper::fgSF_Wt      = 0.994216;
	    PlotHelper::fgSF_ttbar   = 0.928936;
	    PlotHelper::fgSF_WjetsLF = 0.873755;
	    PlotHelper::fgSF_WjetsHF = 1.62495;
	    PlotHelper::fgSF_ZjetsVV = 1.22873;
	    PlotHelper::fgSF_QCD     = 0.489634;
	    break;
	case kSchanSR_munu:
	    PlotHelper::fgSF_schan   = 1.00347;
	    PlotHelper::fgSF_tchan   = 1.00366;
	    PlotHelper::fgSF_Wt      = 0.997203;
	    PlotHelper::fgSF_ttbar   = 0.970669;
	    PlotHelper::fgSF_WjetsLF = 0.747412;
	    PlotHelper::fgSF_WjetsHF = 1.55919;
	    PlotHelper::fgSF_ZjetsVV = 1.10677;
	    PlotHelper::fgSF_QCD     = 0.740205;
	    break;
	case kSchanWCR_lnu:
	    PlotHelper::fgSF_schan   = 1.00059;
	    PlotHelper::fgSF_tchan   = 1.00219;
	    PlotHelper::fgSF_Wt      = 1.00179;
	    PlotHelper::fgSF_ttbar   = 0.983187;
	    PlotHelper::fgSF_WjetsLF = 0.833645;
	    PlotHelper::fgSF_WjetsHF = 1.41588;
	    PlotHelper::fgSF_ZjetsVV = 0.918658;
	    PlotHelper::fgSF_QCD     = 0.477468;
	    break;
	case kSchanWCR_enu:
	    PlotHelper::fgSF_schan   = 1.00137;
	    PlotHelper::fgSF_tchan   = 1.00476;
	    PlotHelper::fgSF_Wt      = 1.00302;
	    PlotHelper::fgSF_ttbar   = 1.01717;
	    PlotHelper::fgSF_WjetsLF = 0.920779;
	    PlotHelper::fgSF_WjetsHF = 1.25748;
	    PlotHelper::fgSF_ZjetsVV = 1.11037;
	    PlotHelper::fgSF_QCD     = 0.441785;
	    break;
	case kSchanWCR_munu:
	    PlotHelper::fgSF_schan   = 1.00024;
	    PlotHelper::fgSF_tchan   = 1.00074;
	    PlotHelper::fgSF_Wt      = 1.00027;
	    PlotHelper::fgSF_ttbar   = 0.98546;
	    PlotHelper::fgSF_WjetsLF = 0.785031;
	    PlotHelper::fgSF_WjetsHF = 1.43004;
	    PlotHelper::fgSF_ZjetsVV = 0.920617;
	    PlotHelper::fgSF_QCD     = 0.761064;
	    break;
	case kSchanTtbarCR_lnu:
	    PlotHelper::fgSF_schan   = 0.999852;
	    PlotHelper::fgSF_tchan   = 0.999722;
	    PlotHelper::fgSF_Wt      = 0.999129;
	    PlotHelper::fgSF_ttbar   = 1.0226;
	    PlotHelper::fgSF_WjetsLF = 1.28033;
	    PlotHelper::fgSF_WjetsHF = 0.977401;
	    PlotHelper::fgSF_ZjetsVV = 1.02994;
	    PlotHelper::fgSF_QCD     = 0.523769;
	    break;
	case kSchanTtbarCR_enu:
	    PlotHelper::fgSF_schan   = 9.9761e-01;
	    PlotHelper::fgSF_tchan   = 9.9811e-01;
	    PlotHelper::fgSF_Wt      = 1.0011e+00;
	    PlotHelper::fgSF_ttbar   = 1.0273e+00;
	    PlotHelper::fgSF_WjetsLF = 9.3694e-01;
	    PlotHelper::fgSF_WjetsHF = 1.3212e+00;
	    PlotHelper::fgSF_ZjetsVV = 7.2413e-01;
	    PlotHelper::fgSF_QCD     = 5.8508e-01;
	    break;
	case kSchanTtbarCR_munu:
	    PlotHelper::fgSF_schan   = 9.9748e-01;
	    PlotHelper::fgSF_tchan   = 9.9706e-01;
	    PlotHelper::fgSF_Wt      = 9.9825e-01;
	    PlotHelper::fgSF_ttbar   = 1.0280e+00;
	    PlotHelper::fgSF_WjetsLF = 7.5324e-01;
	    PlotHelper::fgSF_WjetsHF = 9.2023e-01;
	    PlotHelper::fgSF_ZjetsVV = 8.0954e-01;
	    PlotHelper::fgSF_QCD     = 7.3678e-01;
	    break;
	//
	// t-channel
	//
	case kTchanSR_lnu:
	    PlotHelper::fgSF_schan   = 9.9922e-01;
	    PlotHelper::fgSF_tchan   = 1.0148e+00;
	    PlotHelper::fgSF_Wt      = 9.8500e-01;
	    PlotHelper::fgSF_ttbar   = 9.3280e-01;
	    PlotHelper::fgSF_WjetsLF = 8.5620e-01;
	    PlotHelper::fgSF_WjetsHF = 1.2834e+00;
	    PlotHelper::fgSF_ZjetsVV = 6.9123e-01;
	    PlotHelper::fgSF_QCD     = 6.9123e-01;
	    break;
	case kTchanSR_enu:
	    PlotHelper::fgSF_schan   = 9.9895e-01;
	    PlotHelper::fgSF_tchan   = 1.0099e+00;
	    PlotHelper::fgSF_Wt      = 9.8620e-01;
	    PlotHelper::fgSF_ttbar   = 9.6576e-01;
	    PlotHelper::fgSF_WjetsLF = 7.6710e-01;
	    PlotHelper::fgSF_WjetsHF = 1.2915e+00;
	    PlotHelper::fgSF_ZjetsVV = 7.2788e-01;
	    PlotHelper::fgSF_QCD     = 6.3809e-01;
	    break;
	case kTchanSR_munu:
	    PlotHelper::fgSF_schan   = 9.9902e-01;
	    PlotHelper::fgSF_tchan   = 1.0082e+00;
	    PlotHelper::fgSF_Wt      = 9.9035e-01;
	    PlotHelper::fgSF_ttbar   = 9.5939e-01;
	    PlotHelper::fgSF_WjetsLF = 8.5145e-01;
	    PlotHelper::fgSF_WjetsHF = 1.2253e+00;
	    PlotHelper::fgSF_ZjetsVV = 7.3543e-01;
	    PlotHelper::fgSF_QCD     = 8.3694e-01;
	    break;
	case kTchanWCR_lnu:
	    PlotHelper::fgSF_schan   = 9.9843e-01;
	    PlotHelper::fgSF_tchan   = 1.0005e+00;
	    PlotHelper::fgSF_Wt      = 9.9105e-01;
	    PlotHelper::fgSF_ttbar   = 9.7672e-01;
	    PlotHelper::fgSF_WjetsLF = 1.3698e+00;
	    PlotHelper::fgSF_WjetsHF = 1.0242e+00;
	    PlotHelper::fgSF_ZjetsVV = 1.0000e+00;
	    PlotHelper::fgSF_QCD     = 1.0000e+00;
	    break;
	case kTchanWCR_enu:
	    PlotHelper::fgSF_schan   = 9.9846e-01;
	    PlotHelper::fgSF_tchan   = 9.9963e-01;
	    PlotHelper::fgSF_Wt      = 9.9489e-01;
	    PlotHelper::fgSF_ttbar   = 9.8905e-01;
	    PlotHelper::fgSF_WjetsLF = 1.2195e+00;
	    PlotHelper::fgSF_WjetsHF = 1.1537e+00;
	    PlotHelper::fgSF_ZjetsVV = 1.0000e+00;
	    PlotHelper::fgSF_QCD     = 1.0000e+00;
	    break;
	case kTchanWCR_munu:
	    PlotHelper::fgSF_schan   = 9.9850e-01;
	    PlotHelper::fgSF_tchan   = 1.0014e+00;
	    PlotHelper::fgSF_Wt      = 9.9213e-01;
	    PlotHelper::fgSF_ttbar   = 9.8157e-01;
	    PlotHelper::fgSF_WjetsLF = 1.2332e+00;
	    PlotHelper::fgSF_WjetsHF = 1.0756e+00;
	    PlotHelper::fgSF_ZjetsVV = 1.0000e+00;
	    PlotHelper::fgSF_QCD     = 1.0000e+00;
	    break;
	case kTchanTtbarCR_lnu:
	    PlotHelper::fgSF_schan   = 9.9842e-01;
	    PlotHelper::fgSF_tchan   = 9.9820e-01;
	    PlotHelper::fgSF_Wt      = 9.9218e-01;
	    PlotHelper::fgSF_ttbar   = 1.0334e+00;
	    PlotHelper::fgSF_WjetsLF = 9.3350e-01;
	    PlotHelper::fgSF_WjetsHF = 8.4361e-01;
	    PlotHelper::fgSF_ZjetsVV = 7.8834e-01;
	    PlotHelper::fgSF_QCD     = 7.8868e-01;
	    break;
	case kTchanTtbarCR_enu:
	    PlotHelper::fgSF_schan   = 9.9853e-01;
	    PlotHelper::fgSF_tchan   = 9.9945e-01;
	    PlotHelper::fgSF_Wt      = 9.9069e-01;
	    PlotHelper::fgSF_ttbar   = 1.0555e+00;
	    PlotHelper::fgSF_WjetsLF = 9.3366e-01;
	    PlotHelper::fgSF_WjetsHF = 8.9718e-01;
	    PlotHelper::fgSF_ZjetsVV = 7.9188e-01;
	    PlotHelper::fgSF_QCD     = 7.5607e-01;
	    break;
	case kTchanTtbarCR_munu:
	    PlotHelper::fgSF_schan   = 9.9843e-01;
	    PlotHelper::fgSF_tchan   = 9.9826e-01;
	    PlotHelper::fgSF_Wt      = 9.9731e-01;
	    PlotHelper::fgSF_ttbar   = 1.0120e+00;
	    PlotHelper::fgSF_WjetsLF = 9.3355e-01;
	    PlotHelper::fgSF_WjetsHF = 8.1966e-01;
	    PlotHelper::fgSF_ZjetsVV = 8.0931e-01;
	    PlotHelper::fgSF_QCD     = 8.4014e-01;
	    break;
	default:
	    cout << "All SF at 1." << endl;
    }
}
