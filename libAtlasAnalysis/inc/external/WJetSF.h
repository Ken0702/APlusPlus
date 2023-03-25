#ifndef WJetSF_EPS_newlumi_h
#define WJetSF_EPS_newlumi_h

/*********************************************************************************************************************************
 * WJetSF_EPS_newlumi.h                        											 *
 *                                                         									 *
 * double WJet_SF(int HFOR, int jetn) is a function which returns a data/MC flavor scale factor of a given event. 		 *
 *  The inputs of this function are the HFOR flag and the number of jets of that event.						 *
 *  																 *
 *  The function is defined for 1, 2, 3 and 4 jets, if the number of jet is different that those values the function		 *
 *  returns 1. The flavor fractions defined are bb, cc, c and light flavor jets that corresponds with the HFOR 			 *
 *  flag equals to 0, 1, 2 and 3 respectively. If the HFOR flag is different than those vales the function returns 1. 		 *
 *																 *
 *  This file also has defined several functions for the systematic uncertainties, like:					 *
 *    double WJet_SF_err_TtbarDown(int HFOR, int jetn)										 *
 *  The inputs of this function are also the HFOR flag and the number of jets of an event. The output in this case		 *
 *  is the flavor scale factor after the ttbar cross section has been shifted down by 30%.					 *
 *																 *
 *  The rest of the functions for the different systematic uncertainties are:							 *
 *    double WJet_SF_err_TtbarUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_SgTopDown(int HFOR, int jetn);										 *
 *    double WJet_SF_err_SgTopUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_ZjetsDown(int HFOR, int jetn);										 *
 *    double WJet_SF_err_ZjetsUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_DibDown(int HFOR, int jetn);										 *
 *    double WJet_SF_err_DibUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_QCDDown(int HFOR, int jetn);										 *
 *    double WJet_SF_err_QCDUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_JESDown(int HFOR, int jetn);										 *
 *    double WJet_SF_err_JESUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_BtagSFDown(int HFOR, int jetn);									 *
 *    double WJet_SF_err_BtagSFUp(int HFOR, int jetn);										 *
 *    double WJet_SF_err_LqtagSFDown(int HFOR, int jetn);									 *
 *    double WJet_SF_err_LqtagSFUp(int HFOR, int jetn);										 *
 *																 *
 *  The WJet_SF_Staterr(int HFOR, int jetn), WJet_SF_Systerr(int HFOR, int jetn), and WJet_SF_Totalerr(int HFOR, int jetn) 	 *
 *  functions are a little bit different since they return the value of the statistical and the systematic shift, 		 *
 *  they don't return the scale factor with those statistical, or the systematic uncert. applied as the previous functions.      *
 *  i.e., the scale factor will be expresed like :the nominal +/- its statistical uncertainty +/- its systematic uncertainties:	 *
 *  WJet_SF(int HFOR, int jetn) +/- WJet_SF_Staterr(int HFOR, int jetn) (stat) +/- WJet_SF_Systerr(int HFOR, int jetn) (sys)	 *
 *																 *
 *  or showing only the total uncertainty:											 *
 *																 *
 *  WJet_SF(int HFOR, int jetn) +/- WJet_SF_Totalerr(int HFOR, int jetn)(sys)							 *
 *																 *
 * History                                                    									 *
 *         14 Feb 2011 -- created by Jenny Holzbauer          									 *
 *         9 June 2011 -- updated by Barbara Alvarez with the preliminary scale factors for EPS (156/pb)                      	 *
 *         23 June 2011 -- updated by B.Alvarez with the including an extra systematic of 25% up and down for 1, 3, 4 jet bins	 *
 *         25 June 2011 -- updated by B.Alvarez including the QCD estimates fron Kathrin                                 	 *
 *         6  July 2011 -- updated by B.Alvarez including the new luminostiy and new Wc samples                               	 *
 *                                                            									 *
 *********************************************************************************************************************************/

#include <iostream>
#include <math.h>

// forward declaration of functions
double WJet_SF(int HFOR, int jetn);
double WJet_SF_err_ProjectionDown(int HFOR, int jetn);
double WJet_SF_err_ProjectionUp(int HFOR, int jetn);
//statistical error
double WJet_SF_Staterr(int HFOR, int jetn);
//systematic uncertainties
double WJet_SF_err_TtbarDown(int HFOR, int jetn);
double WJet_SF_err_TtbarUp(int HFOR, int jetn);
double WJet_SF_err_SgTopDown(int HFOR, int jetn);
double WJet_SF_err_SgTopUp(int HFOR, int jetn);
double WJet_SF_err_ZjetsDown(int HFOR, int jetn);
double WJet_SF_err_ZjetsUp(int HFOR, int jetn);
double WJet_SF_err_DibDown(int HFOR, int jetn);
double WJet_SF_err_DibUp(int HFOR, int jetn);
double WJet_SF_err_QCDDown(int HFOR, int jetn);
double WJet_SF_err_QCDUp(int HFOR, int jetn);
double WJet_SF_err_JESDown(int HFOR, int jetn);
double WJet_SF_err_JESUp(int HFOR, int jetn);
double WJet_SF_err_BtagSFDown(int HFOR, int jetn);
double WJet_SF_err_BtagSFUp(int HFOR, int jetn);
double WJet_SF_err_LqtagSFDown(int HFOR, int jetn);
double WJet_SF_err_LqtagSFUp(int HFOR, int jetn);
//total systematic uncertainty
double WJet_SF_Systerr(int HFOR, int jetn);
//total uncertainty : stat+syst
double WJet_SF_Totalerr(int HFOR, int jetn);

//fucntion that returns the flavor scale factor
double WJet_SF(int HFOR, int jetn)
{

// HFOR is the heavy flavor flag: 0 for bb, 1 for cc, 2 for c, 3 for light 
// jetn the number of jets 
// flavor fractions estimated for 1, 2, 3 and 4 jet bins  

   
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  //rel 16 numbers
  const double SFmatrix[4][3] = {{1.3360, 1.2959, 0.9172}, //rows are HOFR  bb, c, and light in this order  
				 {1.2391, 1.2020, 0.8507}, //columns are njets
				 {1.1891, 1.1535, 0.8164},
				 {1.1891, 1.1535, 0.8164}};

  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix[jetn_index][HFOR_index];
  }
}
//extra systematic projection factor 25% down
double WJet_SF_err_ProjectionDown(int HFOR, int jetn)
{
  //30% shift down on the ttbar contribution
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.0020,  0.9720,  0.6879}, //rows are HOFR  bb, c, and light in this order  
				     {1.2391,  1.2020,  0.8507}, //columns are njets
				     {0.8919,  0.8651,  0.6123},
				     {0.8919,  0.8651,  0.6123}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//extra systematic projection factor 25% up
double WJet_SF_err_ProjectionUp(int HFOR, int jetn)
{
  //30% shift down on the ttbar contribution
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.6699,  1.6199,  1.1465}, //rows are HOFR  bb, c, and light in this order  
				     {1.2391,  1.2020,  0.8507}, //columns are njets
				     {1.4864,  1.4419,  1.0205},
				     {1.4864,  1.4419,  1.0205}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}


//ttbar cross section shift down
double WJet_SF_err_TtbarDown(int HFOR, int jetn)
{
  //30% shift down on the ttbar contribution
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.5997,  1.2577,  0.9114}, //rows are HOFR  bb, c, and light in this order  
				     {1.4730,  1.1581,  0.8392}, //columns are njets
				     {1.4374,  1.1301,  0.8189},
				     {1.4374,  1.1301,  0.8189}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//ttbar cross section shift up
double WJet_SF_err_TtbarUp(int HFOR, int jetn)
{
  //30% shift up on the ttbar contribution
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.0759,  1.3337,  0.9230}, //rows are HOFR  bb, c, and light in this order  
				     {1.0050,  1.2459,  0.8622}, //columns are njets
				     {0.9479,  1.1751,  0.8132},
				     {0.9479,  1.1751,  0.8132}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//single top cross section shift down
double WJet_SF_err_SgTopDown(int HFOR, int jetn)
{
  //100% shift on the single top cross section
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{2.0969,  1.2196,  0.8965}, //rows are HOFR  bb, c, and light in this order  
				     {1.9060,  1.1086,  0.8149}, //columns are njets
				     {1.8223,  1.0599,  0.7791},
				     {1.8223,  1.0599,  0.7791}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//single top cross section shift up
double WJet_SF_err_SgTopUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{0.6026,  1.3707,  0.9369}, //rows are HOFR  bb, c, and light in this order  
				     {0.5702,  1.2969,  0.8864}, //columns are njets
				     {0.5494,  1.2497,  0.8542},
				     {0.5494,  1.2497,  0.8542}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//Z+jest cross section shift down 30% shift
double WJet_SF_err_ZjetsDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.2961,  1.3020,  0.9476}, //rows are HOFR  bb, c, and light in this order  
				     {1.2283,  1.2340,  0.8981}, //columns are njets
				     {1.2015,  1.2071,  0.8785},
				     {1.2015,  1.2071,  0.8785}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//Z+jest cross section shift up
double WJet_SF_err_ZjetsUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.3821,  1.2890,  0.8867}, //rows are HOFR  bb, c, and light in this order  
				     {1.2524,  1.1680,  0.8034}, //columns are njets
				     {1.1767,  1.0974,  0.7549},
				     {1.1767,  1.0974,  0.7549}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//Diboson cross section shift down 5% shiff
double WJet_SF_err_DibDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.3378,  1.2961,  0.9173}, //rows are HOFR  bb, c, and light in this order  
				     {1.2411,  1.2025,  0.8510}, //columns are njets
				     {1.1913,  1.1542,  0.8168},
				     {1.1913,  1.1542,  0.8168}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//Diboson cross section shift up
double WJet_SF_err_DibUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.3342,  1.2958,  0.9172}, //rows are HOFR  bb, c, and light in this order  
				     {1.2370,  1.2014,  0.8504}, //columns are njets
				     {1.1870,  1.1529,  0.8160},
				     {1.1870,  1.1529,  0.8160}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//QCD shift Down estimated by the data driven methods
double WJet_SF_err_QCDDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.5500,  1.2975,  0.9294}, //rows are HOFR  bb, c, and light in this order  
				     {1.4594,  1.2216,  0.8751}, //columns are njets
				     {1.4121,  1.1820,  0.8467},
				     {1.4121,  1.1820,  0.8467}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//QCD shift Up estimated by the data driven methods 
double WJet_SF_err_QCDUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.1150,  1.2958,  0.9051}, //rows are HOFR  bb, c, and light in this order  
				     {1.0179,  1.1830,  0.8263}, //columns are njets
				     {0.9680,  1.1250,  0.7858},
				     {0.9680,  1.1250,  0.7858}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//JES up, NEED to be UPDATED
double WJet_SF_err_JESUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.7169,  1.3677,  0.7768}, //rows are HOFR  bb, c, and light in this order  
				     {1.4195,  1.1308,  0.6422}, //columns are njets
				     {1.2056,  0.9604,  0.5455},
				     {1.2056,  0.9604,  0.5455}};

  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//JES down. NEED to be UPDATED
double WJet_SF_err_JESDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.0832,  1.2481,  1.0772}, //rows are HOFR  bb, c, and light in this order  
				     {1.1111,  1.2802,  1.1049}, //columns are njets
				     {1.1897,  1.3707,  1.1830},
				     {1.1897,  1.3707,  1.1830}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//B/C tag scale factors shiftted up
double WJet_SF_err_BtagSFUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.0008,  1.0682,  0.9626}, //rows are HOFR  bb, c, and light in this order  
				     {0.9489,  1.0127,  0.9127}, //columns are njets
				     {0.9252,  0.9875,  0.8899},
				     {0.9252,  0.9875,  0.8899}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//B/C tag scale factors shiftted down
 
double WJet_SF_err_BtagSFDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.8389,  1.6492,  0.8475}, //rows are HOFR  bb, c, and light in this order  
				     {1.6511,  1.4807,  0.7609}, //columns are njets
				     {1.5497,  1.3898,  0.7142},
				     {1.5497,  1.3898,  0.7142}};
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//Lq tag scale factor shitd up 
double WJet_SF_err_LqtagSFUp(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.2483,  1.2411,  0.9284}, //rows are HOFR  bb, c, and light in this order  
				     {1.1643,  1.1575,  0.8659}, //columns are njets
				     {1.1218,  1.1153,  0.8343},
				     {1.1218,  1.1153,  0.8343}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}

//Lq tag scale factor shitd down
double WJet_SF_err_LqtagSFDown(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{1.4254,  1.3498,  0.9060}, //rows are HOFR  bb, c, and light in this order  
				     {1.3147,  1.2449,  0.8357}, //columns are njets
				     {1.2567,  1.1900,  0.7988},
				     {1.2567,  1.1900,  0.7988}};
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 1.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 1.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index];
  }

}
//Statistical ucnertainty (symmetric)
double WJet_SF_Staterr(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{9.1601*1.3360 , 3.2636*1.2959 , 0.3993*0.9172}, //rows are HOFR  bb, c, and light in this order  
				     {8.7628*1.2391 , 3.5840*1.2020 , 0.7115*0.8507}, //columns are njets
				     {8.3808*1.1891 , 3.9559*1.1535 , 1.1431*0.8164},
				     {8.3808*1.1891 , 3.9559*1.1535 , 1.1431*0.8164}};

  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 0.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 0.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index]/100;
  }

}
//Total systematic uncertanties up and down squared root (symmetric)
double WJet_SF_Systerr(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{77.8861*1.3360, 35.1209*1.2959, 30.9021*0.9172}, //rows are HOFR  bb, c, and light in this order  
				     {67.8508*1.2391, 23.0118*1.2020, 29.8462*0.8507}, //columns are njets
				     {70.7252*1.1891, 36.9660*1.1535, 48.9900*0.8164},
				     {70.7252*1.1891, 36.9660*1.1535, 48.9900*0.8164}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 0.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 0.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index]/100;
  }

}
//Total uncertaintu stat + syst. (symmetric)
double WJet_SF_Totalerr(int HFOR, int jetn)
{
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{78.4229*1.3360, 35.2722*1.2959, 30.9047*0.9172}, //rows are HOFR  bb, c, and light in this order  
				     {68.4143*1.2391, 23.2893*1.2020, 29.8546*0.8507}, //columns are njets
				     {71.2200*1.1891, 37.1771*1.1535, 49.0033*0.8164},
				     {71.2200*1.1891, 37.1771*1.1535, 49.0033*0.8164}};

  
  
  //check HF flag 
  if(HFOR>3 || HFOR<0 ){
    return 0.0;       
  }  
  //check jet multiplicity
  else if(jetn>4 || jetn<1){
    return 0.0;       
    
  }else { 
    
    int HFOR_index = -999;
    if(HFOR==0)     HFOR_index = HFOR;
    else if(HFOR>0) HFOR_index = HFOR-1;//bb and cc events have the same scale factor
    
    return SFmatrix_err[jetn_index][HFOR_index]/100;
  }

}

#endif
