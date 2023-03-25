//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCPlot
#define HEP_HepDataMCPlot
#ifndef HEP_HepDataMCFolder
#include <HepDataMCFolder.h>
#endif
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_HStack
#include <THStack.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TString
#include <TString.h>
#endif
#ifndef ROOT_TLegend
#include <TLegend.h>
#endif
#ifndef ROOT_TCanvas
#include <TCanvas.h>
#endif
#ifndef ROOT_TGraphAsymmErrors
#include <TGraphAsymmErrors.h>
#endif

#include <HepTemplate.h>

class TList;
class TObjString;
class HepDataMCSample;
class TStyle;
class TBrowser;
class TPad;
class TFeldmanCousins;
class TFractionFitter;
class TLine;

#if (__cplusplus > 201100L)
#define const_or_constexpr constexpr
#else
#define const_or_constexpr const
#endif

class HepDataMCPlot : public TNamed {
    
  private:
    static const Color_t fgUnifyColor = kYellow;        //! Color used for unify the MC stack
    static const_or_constexpr Double_t fgFeldmanCousinsCL = 0.680;   //! Confidence level for error calculation
    static const Int_t fgFeldmanCousinsMaxEntries = 0;  //! Separates Feldman-Cousins error
                                                        //  calculation from standard ROOT errors
    
    
    THStack  *fMCHistStack;             // MC HStack object
    TH1F     *fHistDATA;                // Pointer to DATA histograms for fast access
    TH1F     *fHistMCTop;               //! Pointer to top-most MC histogram of the stack
    TLegend  *fLegend;                  // Legend
    TLegend  *fLegendLarge;             // Large legend
    Bool_t    fFirstDraw;               //! Drawing the first time ?
    Bool_t    fFirstLegend;             //! Drawing the legend the first time ?
    TString  *fXTitle;                  // Title of x-axis
    TString  *fYTitle;                  // Title of y-axis
    Int_t     fN_MCHistograms;          // No. of MC histograms (used for histogram id)
    Bool_t    fGroupHistograms;         // Group histograms when drawing and in the legend
    TList    *fMCFolders;               // List of folders to group MC samples
    TList    *fMCSingleSamples;         // List of single MC samples, not belonging to folder
    Bool_t    fUnifyMCStack;            // Draw MC stack histograms as one
    Bool_t    fDrawMCError;             // Draw the total MC uncertainty on top
    Bool_t    fDrawNormError;           // Include the normalization error when computing the total uncertainty
    Bool_t    fUseOverflowUnderflow;    // Add overflow in last bin and underflow in first bin
    Bool_t    fDrawData;                // Draw DATA histogram (default=on)
    Bool_t    fDrawDataErrorX;          //! Draw x-errors for data histogram (default=on)
    Bool_t    fDrawDataZeroEntryErrors; // Draw the Data error bars even for 0 data entry bins
    Bool_t    fDrawDataZeroEntries;     //! Draw markers for bins with no data
    TGraphAsymmErrors *fMCErrors;       // Graph for drawing the MC error band
    TGraphAsymmErrors *fGraphDATA;      //! Graph for drawing data
    Float_t   fLumiDATA;                // Luminosity of data sample
    TList    *fMCFitTemplates;          // List of MC templates given to the fitter
    TFractionFitter *fFitter;           //! TFractionFitter instance.
    THStack  *fMCFittedHistStack;       // MC HStack object for fitted MC histograms
    Bool_t    fDrawCMSLumiAtlasLabel;   // Draw Labels,if kTRUE
    Bool_t    fUseAtlasStyle;	        // Use ATLAS style if it is true
    Bool_t    fUseShortCMSLumiLabel;    //! Use short label for CMS + Lumi
    TString   fCenterOfMassEnergyLabel; // Character string which denotes the centre of mass energy (example: "13")
    TString   fAtlasLabelStatus;        // Status label (example: "work in progress")
    TString   fLuminosityLabel;         // string which denotes the luminosity (example: "4.7 pb^{-1}")
    Float_t   fAtlasLabelPosX;          // X position of the Atlas label
    Float_t   fAtlasLabelPosY;          // Y position of the Atlas label
    Float_t   fCMSEnergyLabelPosX;      // X position of the cms energy label
    Float_t   fCMSEnergyLabelPosY;      // Y position of the cms energy label
    Float_t   fLuminosityLabelPosX;     // X position of the luminosity label
    Float_t   fLuminosityLabelPosY;     // Y position of the luminosity label
    Float_t   fAddRange;                // Extra space added when setting axis limit (default: 1.1)
    Bool_t    fDrawDataMCRatio;         // Switch for drawing data/mc ratio
    Bool_t    fDataMCRatioCenterOne;    // Switch for drawing data/mc ratio centered at one
    Bool_t    fDrawSignificance;        // Switch for drawing significance plot
    Float_t   fRatioHeight;             // Height of Ratio plots w.r.t stack plot ([0,1], default: 0.4)
    Bool_t    fYRangeUser;              // Flag if user defined y range should be used
    Double_t  fYRangeUserMin;           // Y-axis limit (min)
    Double_t  fYRangeUserMax;           // Y-axis limit (max)
    
    TCanvas  *fCanvas;                  //! Pointer to the canvas (if plot is drawn in a canvas)
    TPad     *fMainPad;                 //! Pointer to the pad of the stacked plot
    TPad     *fRatioPad1;               //! Pointer to the pad of the first ratio plot
    TPad     *fRatioPad2;               //! Pointer to the pad of the second ratio plot

    TH1      *fHistogramMainPad;        //! Pointer to the histogram used for drawing the axis of the stacked plot
    TH1      *fHistogramRatioPad1;      //! Pointer to the histogram used for drawing the axis the first ratio plot
    TH1      *fHistogramRatioPad2;      //! Pointer to the histogram used for drawing the axis second ratio plot
    TLine    *fRatioLine;               //! Line for Ratio plots indicating 0 or 1
    TH1      *fHistogramLastInStack;    //! Pointer to the last histogram inside the MC stack (supposed to be tha MC signal)
    Float_t   fScaleOverlay;            // Scale factor of the overlay histogram
    Bool_t    fDrawSignalOverlay;       // Flag for drawing the signal MC (assumed to be the least entry in the MC histogram stack) as overlay instead as part of the stack
    
  public:
    HepDataMCPlot();
    HepDataMCPlot(const char* name, const char* title);
    virtual ~HepDataMCPlot();
    virtual void Draw(Option_t *option = ""); // *MENU*
//    virtual void Paint(Option_t *option = "");
    virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);
    virtual void Browse(TBrowser *b);
    virtual void Print(Option_t *option = "") const; // *MENU*
    virtual Bool_t IsFolder() const { return kFALSE; }
    void DrawLegend();
    void DrawLegendPad(); // *MENU*
    void SetHistDATA(TH1F *h, const char* label, Float_t lumiData);
    
    void AddHistMC(TH1F *h, HepDataMCSample *sample);
    void AddHistMC(TH1F *h, HepDataMCFolder *folder,
		   HepDataMCSample *sample);
    void SetXTitle(const char* title); // *MENU*
    void SetYTitle(const char* title); // *MENU*
    void SetXRange(Double_t xmin, Double_t xmax); // *MENU*
    void SetYRange(Double_t ymin, Double_t ymax); // *MENU*
    void SetYRangeRatio(Double_t ymin, Double_t ymax, Int_t ratio); // *MENU*
    void SetNdivisionsX(Int_t n = 510, Bool_t optim = kTRUE); // *MENU*
    void SetNdivisionsY(Int_t n = 510, Bool_t optim = kTRUE); // *MENU*
    static TString GetPathInsideFile(TDirectory *dir);
    Bool_t GetGroupHistograms() const { return fGroupHistograms; }
    void SetGroupHistograms(Bool_t GroupHistograms); // *TOGGLE*
    Bool_t GetUnifyMCStack() const { return fUnifyMCStack; }
    void SetUnifyMCStack(Bool_t UnifyMCStack); // *TOGGLE*
    void SetUnifyMCStack(Bool_t UnifyMCStack, Bool_t DoRedraw);
    Bool_t GetDrawMCError() { return fDrawMCError; }
    Bool_t GetDrawNormError() { return fDrawNormError; }
    void SetDrawMCError(Bool_t DrawMCError); // *TOGGLE*
    void SetDrawNormError(Bool_t DrawMCError); // *TOGGLE*
    void SetDrawMCError(Bool_t DrawMCError, Bool_t DoRedraw);
    void SetDrawData(Bool_t DrawData); // *TOGGLE*
    Bool_t GetDrawData() { return fDrawData; }
    void SetUseOverflowUnderflow(Bool_t UseOverflowUnderflow = kTRUE); // *TOGGLE*
    Bool_t GetUseOverflowUnderflow() {return fUseOverflowUnderflow;}
    Bool_t GetDrawDataZeroEntryErrors() { return fDrawDataZeroEntryErrors; }
    void SetDrawDataZeroEntryErrors(Bool_t DrawDataZeroEntryErrors = kTRUE); // *TOGGLE*
    void SetDrawDataZeroEntries(Bool_t flag = kTRUE); // *TOGGLE*
    void ComputeStatisticalUncertainty(TH1F* h, Int_t bin,
				       Double_t &up, Double_t &lo);
    void ComputeNormUncertainty(Int_t bin,
				Double_t &up, Double_t &lo);
    void PrepareFit(Bool_t single_samples = kFALSE, Bool_t prepare_effs = kFALSE);
    Int_t Fit();
    virtual void DrawFit(Option_t *option = "");
    void ListFolders();
    void ListSingleSamples();
    void ListTemplateFractions();
    Double_t GetTemplateFitFraction(const char* template_name);
    Double_t GetTemplateFitFractionError(const char* template_name);
    Float_t GetTemplateCutEfficiency(const char* template_name);
    void SetTemplateBounds(const char* template_name, Double_t Lower, Double_t Upper);
    void SetTemplateStartValue(const char* template_name, Double_t frac, Double_t frac_err);
    void FixTemplateFraction(const char* template_name, Bool_t fix = kTRUE);
    void Rebin(Int_t ngroup); // *MENU*
    void ScaleMCStack(Float_t scale); // *MENU*
    void ScaleMCFolder(Float_t scale, const char *mc_folder);

    void SetDrawDataMCRatio(Bool_t DrawDataMCRatio = kTRUE); // *TOGGLE*
    void SetDrawSignificance(Bool_t DrawSignificance = kTRUE); // *TOGGLE*
    void SetDataMCRatioCenterOne(Bool_t DataMCRatioCenter = kTRUE); // *TOGGLE*
    Bool_t GetDrawDataMCRatio() {return fDrawDataMCRatio;}
    Bool_t GetDrawSignificance() {return fDrawSignificance;}
    void ExportTemplates(TFile *f, const char* suffix);
    TList* GetListOfMCTemplates(const char* suffix);
    void PrintBinStatistics(Option_t *option = "");
    void OptimizeBinning(Double_t zs = 15, Double_t zb = 15,
			 Bool_t DrawStack = kFALSE);
    void SetDrawSignalOverlay(Bool_t DrawSignalOverlay); // *TOGGLE*
    void SetScaleOverlay(Float_t ScaleOverlay); // *MENU*
    
    inline void AddMCFolder(HepDataMCFolder *folder) {
	// Add MC folder for grouping histograms
	if ( fMCFolders == 0 ) fMCFolders = new TList;
	fMCFolders->Add(folder);
    }
    inline void AddMCSingleSample(HepDataMCSample *sample) {
	// Add MC single sample
	if ( fMCSingleSamples == 0 ) fMCSingleSamples = new TList;
	fMCSingleSamples->Add(sample);
    }
    inline void SetMCErrors(TGraphAsymmErrors *ErrorGraph) {
	fMCErrors = ErrorGraph;
    }
    inline void SetLumiDATA(Float_t lumiData) {
	fLumiDATA = lumiData;
    }
    inline TList* GetMCFolders() const { return fMCFolders; }
    inline TList* GetMCSingleSamples() { return fMCSingleSamples; }
    inline TH1F* GetHistDATA() const { return fHistDATA; }
    inline Float_t GetLumiDATA() { return fLumiDATA; }
    inline THStack* GetMCHistStack() const { return fMCHistStack; }
    inline TLegend* GetLegend() { return fLegend; }
    inline TLegend* GetLegendLarge() { return fLegendLarge; }
    inline const char* GetXTitle() { return fXTitle->Data(); }
    inline const char* GetYTitle() { return fYTitle->Data(); }
    inline TGraphAsymmErrors*  GetMCErrors() { return fMCErrors; }
    inline HepTemplate* GetTemplate(const char* title) {
	return dynamic_cast<HepTemplate*>(fMCFitTemplates->FindObject(title));
    }
    inline THStack* GetMCFittedHistStack() { return fMCFittedHistStack; }
    inline Int_t GetNFitTemplates() {
	if(fMCFitTemplates!=0) return fMCFitTemplates->GetEntries();
	else return 0;
    }
    
    //function to design plot in ATLAS style
    void SetUseAtlasStyle(Bool_t UseAtlasStyle = kTRUE){ 
	fUseAtlasStyle = UseAtlasStyle;
    }
    Bool_t GetUseAtlasStyle(){ return fUseAtlasStyle;}
    void SetAtlasStyle(); //set current style to ATLAS style
    inline void SetDrawCMSLumiAtlasLabel(Bool_t DrawCMSLumiAtlasLabel = kTRUE){
	fDrawCMSLumiAtlasLabel = DrawCMSLumiAtlasLabel;
    }
    inline Bool_t GetDrawCMSLumiAtlasLabel(){return fDrawCMSLumiAtlasLabel;}
    void DrawATLASLabel(Double_t x,Double_t y,TString text,Color_t color=1); 
    inline void SetAtlasLabelStatus(TString AtlasLabelStatus){
	fAtlasLabelStatus = AtlasLabelStatus;
    }
    inline TString  GetAtlasLabelStatus(){ return fAtlasLabelStatus;}
    inline void SetCentreOfMassEnergyLabel(TString CMSEnergy) {
	fCenterOfMassEnergyLabel = CMSEnergy;
    }
    inline void SetCenterOfMassEnergyLabel(TString CMSEnergy) {
	SetCentreOfMassEnergyLabel(CMSEnergy);
    }
    inline TString  GetCentreOfEnergyLabel() { return fCenterOfMassEnergyLabel;}
    inline TString  GetCenterOfEnergyLabel() { return GetCentreOfEnergyLabel(); }
    inline void SetLuminosityLabel(TString Luminosity){
	fLuminosityLabel = Luminosity;
    }
    inline TString  GetLuminosityLabel() { return fLuminosityLabel;}
    void DrawTextLatex(Double_t x, Double_t y,
		       Color_t color, TString text,
		       Double_t tsize = 0.04);

    inline void SetDrawDataErrorX(Bool_t DrawErrorX) {
	fDrawDataErrorX = DrawErrorX;
    }
    inline void SetAtlasLabelPos(Float_t x, Float_t y) {
	fAtlasLabelPosX = x;
	fAtlasLabelPosY = y;
    }
    inline void SetLuminosityLabelPos(Float_t x, Float_t y) {
	fLuminosityLabelPosX = x;
	fLuminosityLabelPosY = y;
    }
    inline void SetCMSEnergyLabelPos(Float_t x, Float_t y) {
	fCMSEnergyLabelPosX = x;
	fCMSEnergyLabelPosY = y;
    }
    inline void SetLegendPos(Float_t x1, Float_t y1,
			     Float_t x2, Float_t y2) {
	fLegend->SetX1NDC(x1); fLegend->SetY1NDC(y1);
	fLegend->SetX2NDC(x2); fLegend->SetY2NDC(y2);
    }
    inline void SetRatioHeight(Float_t ratio) { fRatioHeight = ratio; }
    TCanvas * GetCanvas() const { return fCanvas; }
    inline TPad* GetRatioPad1() { return fRatioPad1; }
    inline TPad* GetRatioPad2() { return fRatioPad2; }
    inline TPad* GetMainPad()   { return fMainPad; }
    inline TH1* GetHistogramMainPad() { return fHistogramMainPad; }
    inline TH1* GetHistogramRatioPad1() { return fHistogramRatioPad1; }
    inline TH1* GetHistogramRatioPad2() { return fHistogramRatioPad2; }
    inline Bool_t GetDrawSignalOverlay() const {
	//
	// Flag for drawing the signal MC (assumed to be the least
	// entry in the MC histogram stack) as overlay instead as part of the
	// stack
	//
	return fDrawSignalOverlay;
    }
	
    
  private:
    TGraphAsymmErrors* SetErrors(TH1F *FromHist,
				 Bool_t IsData,
				 Option_t *option = "");
    Double_t pValuePoisson(unsigned nObs, double nExp);
    Double_t pValuePoissonError(unsigned nObs, double E=1, double V=1);
    Double_t pValueToSignificance(double p, bool excess=true);
    TH1F* CompareHistograms(TH1* hObs=0, TH1* hExp=0,
			    bool neglectUncertainty=false,
			    bool variableBinning=false,
			    TH1* hPull=0);
    Double_t pja_normal_quantile(double p);
    void SetXRange(TPad *pad, Double_t xmin, Double_t xmax);
    void SetYRange(TPad *pad, Double_t ymin, Double_t ymax);
    void DrawNoStack(Option_t *option = "");
    void DrawStack(Option_t *option = "");
    void DrawDataMCRatio();
    void DrawSignificance();
    void Init();
    void AddOverflowUnderflowBins(TH1* hist);
    void SetColorsNoStack();
    void SetColorsStack();
    void SetupPad();
    void SetupAxis();
    
    ClassDef(HepDataMCPlot,21) // DATA-MC overlay/stacked plot
};
#endif

