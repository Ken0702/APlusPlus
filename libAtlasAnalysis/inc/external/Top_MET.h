#ifndef Top_MET_h
#define Top_MET_h

#include <vector>
#include "TLorentzVector.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class Top_MET
// Proves a standard interface to modify the missing ET in the TopInputs D3PD
// -enables modification fo the missing ET for default corrections to the selected objects and for studies of systematic uncertainties. 
// -implements the MET only uncertainties for the CellOut term and the SoftJet term.   
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Top_MET {

 public:
  Top_MET();
  virtual ~Top_MET();


  //MET composition list
  enum MET_Types{MET_All=0,MET_e,MET_ph,MET_jet,MET_softjet,MET_mu,MET_cellout};

  //Resets the class -- cleans all the set objects. This should be run for new events and/or when switching systematic uncertainties    
  virtual void Reset();

  // Functions to set objects
  virtual void Set_Electrons(std::vector<TLorentzVector> e_scaled_, std::vector<TLorentzVector> e_);
  virtual void Set_Photons(std::vector<TLorentzVector> ph_scaled_, std::vector<TLorentzVector> ph_);
  virtual void Set_Jets(std::vector<TLorentzVector> jet_scaled_, std::vector<TLorentzVector> jet_, std::vector<TLorentzVector> jet_em_);
  virtual void Set_Muons(std::vector<TLorentzVector> cb_scaled_, std::vector<TLorentzVector> mucb_,
		 std::vector<TLorentzVector> mums_scaled_, std::vector<TLorentzVector> mums_,
		 std::vector<TLorentzVector> muid_scaled_, std::vector<TLorentzVector> muid_);

  // Function for retrieving the corrected MET
  virtual float MET_EtMiss(bool correct=true,bool METComp=false,int type=MET_All);
  virtual float MET_EyMiss(bool correct=true,bool METComp=false,int type=MET_All);
  virtual float MET_ExMiss(bool correct=true,bool METComp=false,int type=MET_All);
  virtual float MET_SumEt(bool correct=true,bool METComp=false,int type=MET_All);
  virtual float MET_MetPhi(bool correct=true,bool METComp=false,int type=MET_All);
  virtual void  Get_MET(float &met_et, float &met_sumet, float &met_etx, float &met_ety, float &met_phi,
	       bool correct=true, bool METComp=false, int type=MET_All);
  virtual void  Get_MET(float &met_sumet, float &met_etx, float &met_ety, 
	       bool correct=true, bool METComp=false, int type=MET_All);

  // Functions for turning on the MEt only sustematic 
  virtual void ApplySoftJetUncertainty(float sigma,std::vector<TLorentzVector> Jets_JESup_, int softjet_sysType);
  virtual void ApplyCellOutUncertainty(float sigma,int cellout_sysType);

  virtual void ApplyPileupUncertainty(float sigma);

  virtual void CheckConsistency();

  virtual void Set_METComposition(  float &MET_etx_,
				    float &MET_ety_,
				    float &MET_sumet_,
				    float &MET_RefEle_sumet_,
				    float &MET_RefEle_etx_,
				    float &MET_RefEle_ety_,
				    float &MET_RefGamma_sumet_,
				    float &MET_RefGamma_etx_,
				    float &MET_RefGamma_ety_,
				    float &MET_RefJet_sumet_,
				    float &MET_RefJet_etx_,
				    float &MET_RefJet_ety_,
				    float &MET_SoftJets_sumet_,
				    float &MET_SoftJets_etx_,
				    float &MET_SoftJets_ety_,
				    float &MET_CellOut_sumet_,
				    float &MET_CellOut_etx_,
				    float &MET_CellOut_ety_,
				    float &MET_Muon_Total_Muid_sumet_,
				    float &MET_Muon_Total_Muid_etx_,
				    float &MET_Muon_Total_Muid_ety_
				    );

  virtual void Set_METWeights(  std::vector<std::vector<float> >*  el_MET_wpx_,
				std::vector<std::vector<float> >*  el_MET_wpy_,
				std::vector<std::vector<float> >*  el_MET_wet_,
				std::vector<std::vector<float> >*  ph_MET_wpx_,
				std::vector<std::vector<float> >*  ph_MET_wpy_,
				std::vector<std::vector<float> >*  ph_MET_wet_,
				std::vector<std::vector<float> >*  jet_wpx_,
				std::vector<std::vector<float> >*  jet_wpy_,
				std::vector<std::vector<float> >*  jet_wet_,
				std::vector<std::vector<unsigned int> >*  mu_muid_MET_statusWord_,
				std::vector<std::vector<float> >*  mu_muid_MET_wpx_,
				std::vector<std::vector<float> >*  mu_muid_MET_wpy_,
				std::vector<std::vector<float> >*  mu_muid_MET_wet_
				);
  
  virtual void Get_ElectronWeights(int i,float  &wet, float  &wpx, float  &wpy);
  virtual void Get_PhotonWeights(int i,float  &wet, float  &wpx, float  &wpy);
  virtual void Get_JetWeights(int i,float  &wet, float  &wpx, float  &wpy);
  virtual void Get_MuonWeights(int i,float  &wet, float  &wpx, float  &wpy,unsigned int &mu_statusWord);

  virtual void CorrectMet(float &met_et, float &met_etx, float &met_ety, float obj_pt, float obj_px, float obj_py,
			  float obj_wt,  float obj_wx,   float obj_wy,   float obj_scale,bool remove=true);


 protected:
  enum MET_SYSType{EM=0,LC,EFlow};

 private:
  // Muon types
  enum Tags {
    UNKNOWN           = 0x0000,
    DEFAULT           = 0x0001,
    SPECTRO           = 0x0002,
    TRACK             = 0x0004,
    REFMUON           = 0x0008,
     MUID              = 0x0010
    //NEXT_COMES      = 0x0020,
    //AND_THEN        = 0x0040,
  };

  std::vector<TLorentzVector> e_scaled;
  std::vector<TLorentzVector> e;
  std::vector<TLorentzVector> ph_scaled; 
  std::vector<TLorentzVector> ph;
  std::vector<TLorentzVector> jet_scaled;
  std::vector<TLorentzVector> jet;
  std::vector<TLorentzVector> jet_em;
  std::vector<TLorentzVector> mucb_scaled;  
  std::vector<TLorentzVector> mucb;
  std::vector<TLorentzVector> mums_scaled;
  std::vector<TLorentzVector> mums;
  std::vector<TLorentzVector> muid_scaled;
  std::vector<TLorentzVector> muid;

  std::vector<TLorentzVector> Jets_JESup;

  float SoftJet_sigma; 
  float CellOut_sigma;
  float JetPtThreshold;
  float GeV;

  // MET variables
  float* MET_etx;
  float* MET_ety;
  float* MET_sumet;
  float* MET_RefEle_sumet;
  float* MET_RefEle_etx;
  float* MET_RefEle_ety;
  float* MET_RefGamma_sumet;
  float* MET_RefGamma_etx;
  float* MET_RefGamma_ety;
  float* MET_RefJet_sumet;
  float* MET_RefJet_etx;
  float* MET_RefJet_ety;
  float* MET_SoftJets_sumet;
  float* MET_SoftJets_etx;
  float* MET_SoftJets_ety;
  float* MET_CellOut_sumet;
  float* MET_CellOut_etx;
  float* MET_CellOut_ety;
  float* MET_Muon_Total_Muid_sumet;
  float* MET_Muon_Total_Muid_etx;
  float* MET_Muon_Total_Muid_ety;
  std::vector<std::vector<float> >*  el_MET_wpx;
  std::vector<std::vector<float> >*  el_MET_wpy;
  std::vector<std::vector<float> >*  el_MET_wet;
  std::vector<std::vector<float> >*  ph_MET_wpx;
  std::vector<std::vector<float> >*  ph_MET_wpy;
  std::vector<std::vector<float> >*  ph_MET_wet;
  std::vector<std::vector<float> >*  jet_wpx;
  std::vector<std::vector<float> >*  jet_wpy;
  std::vector<std::vector<float> >*  jet_wet;
  std::vector<std::vector<unsigned int> >*  mu_muid_MET_statusWord;
  std::vector<std::vector<float> >*  mu_muid_MET_wpx;
  std::vector<std::vector<float> >*  mu_muid_MET_wpy;
  std::vector<std::vector<float> >*  mu_muid_MET_wet;

  bool METComOk;
  bool METWeightsOK;
  int SoftJet_SysType;
  int CellOut_SysType;
  float pileupsigma;

  //Setup uncertianties for the SoftJet
  static double SoftJetEM_FractionUncert[];
  static double SoftJetEM_SumETBins[];
  static double SoftJetEM_NBins;
  static double CellOutEM_FractionUncert[];
  static double CellOutEM_SumETBins[];
  static double CellOutEM_NBins;

  //Setup uncerianties for the CellOut
  static double SoftJetLC_FractionUncert[];
  static double SoftJetLC_SumETBins[];
  static double SoftJetLC_NBins;
  static double CellOutEFlow_FractionUncert[];
  static double CellOutEFlow_SumETBins[];
  static double CellOutEFlow_NBins;


};
#endif
