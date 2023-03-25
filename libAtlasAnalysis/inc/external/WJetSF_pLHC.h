#ifndef WJetSF_pLHC_h
#define WJetSF_pLHC_h

/*********************************************************************************************************************************
 * WJetSF_pLHC.h                        											 *
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
 *         13 May 2011 -- updated by  Barbara Alvarez, including the pLHC files                                          	 *
 *         26 May 2011 -- updated by  Barbara Alvarez, including the new JES recomendations                                  	 *
 *                                                            									 *
 *********************************************************************************************************************************/

#include <iostream>
#include <math.h>

// forward declaration of functions
double WJet_SF(int HFOR, int jetn);
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
  const double SFmatrix[4][3] =
      {{1.164, 1.071, 0.979}, //rows are HOFR  bb, c, and light in this order  
       {1.150, 1.058, 0.968}, //columns are njets
       {1.141, 1.049, 0.960},
       {1.131, 1.041, 0.952}};
/*                       {{0.8413, 1.4526, 1.0352}, //rows are HOFR  bb, c, and light in this order   */
/* 				 {0.8188, 1.4138, 1.0075}, //columns are njets */
/* 				 {0.7627, 1.3169, 0.9385}, */
/* 				 {0.6450, 1.1136, 0.7936}}; */

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

//ttbar cross section shift down
double WJet_SF_err_TtbarDown(int HFOR, int jetn)
{
  //30% shift down on the ttbar contribution
  int jetn_index = jetn-1; //need to modify the jet index to make it from 0 to 3
  
  const double SFmatrix_err[4][3] = {{0.8340, 1.4658, 1.0344}, //rows are HOFR  bb, c, and light in this order  
				     {0.8144, 1.4314, 1.0102}, //columns are njets
				     {0.8340, 1.4658, 1.0344},
				     {0.8144, 1.4314, 1.0102}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8486, 1.4394, 1.0360}, //rows are HOFR  bb, c, and light in this order  
				     {0.8231, 1.3962, 1.0049}, //columns are njets
				     {0.8486, 1.4394, 1.0360},
				     {0.8231, 1.3962, 1.0049}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.7854, 1.5546, 1.0294}, //rows are HOFR  bb, c, and light in this order  
				     {0.7735, 1.5311, 1.0139}, //columns are njets
				     {0.7854, 1.5546, 1.0294},
				     {0.7735, 1.5311, 1.0139}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8972, 1.3505, 1.0410}, //rows are HOFR  bb, c, and light in this order  
				     {0.8627, 1.2987, 1.0010}, //columns are njets
				     {0.8972, 1.3505, 1.0410},
				     {0.8627, 1.2987, 1.0010}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8849, 1.4417, 1.0616}, //rows are HOFR  bb, c, and light in this order  
				     {0.8743, 1.4243, 1.0488}, //columns are njets
				     {0.8849, 1.4417, 1.0616},
				     {0.8743, 1.4243, 1.0488}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.7976, 1.4634, 1.0088}, //rows are HOFR  bb, c, and light in this order  
				     {0.7641, 1.4019, 0.9664}, //columns are njets
				     {0.7976, 1.4634, 1.0088},
				     {0.7641, 1.4019, 0.9664}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8411, 1.4534, 1.0353}, //rows are HOFR  bb, c, and light in this order  
				     {0.8189, 1.4151, 1.0080}, //columns are njets
				     {0.8411, 1.4534, 1.0353},
				     {0.8189, 1.4151, 1.0080}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8415, 1.4518, 1.0351}, //rows are HOFR  bb, c, and light in this order  
				     {0.8187, 1.4125, 1.0071}, //columns are njets
				     {0.8415, 1.4518, 1.0351},
				     {0.8187, 1.4125, 1.0071}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8109, 1.5480, 1.0448}, //rows are HOFR  bb, c, and light in this order  
				     {0.8017, 1.5304, 1.0329}, //columns are njets
				     {0.8109, 1.5480, 1.0448},
				     {0.8017, 1.5304, 1.0329}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.8716, 1.3572, 1.0257}, //rows are HOFR  bb, c, and light in this order  
				     {0.8345, 1.2995, 0.9820}, //columns are njets
				     {0.8716, 1.3572, 1.0257},
				     {0.8345, 1.2995, 0.9820}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.4438, 1.6981, 0.9125}, //rows are HOFR  bb, c, and light in this order  
				     {0.4026, 1.5406, 0.8278}, //columns are njets
				     {0.4438, 1.6981, 0.9125},
				     {0.4026, 1.5406, 0.8278}};

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
  
  const double SFmatrix_err[4][3] = {{1.1559, 1.2649, 1.1857}, //rows are HOFR  bb, c, and light in this order  
				     {1.2164, 1.3311, 1.2478}, //columns are njets
				     {1.1559, 1.2649, 1.1857},
				     {1.2164, 1.3311, 1.2478}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{1.0873, 1.0242, 1.0673}, //rows are HOFR  bb, c, and light in this order  
				     {1.0583, 0.9968, 1.0388}, //columns are njets
				     {1.0873, 1.0242, 1.0673},
				     {1.0583, 0.9968, 1.0388}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.4158, 2.1933, 0.9797}, //rows are HOFR  bb, c, and light in this order  
				     {0.4047, 2.1347, 0.9535}, //columns are njets
				     {0.4158, 2.1933, 0.9797},
				     {0.4047, 2.1347, 0.9535}};
  
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
  
  const double SFmatrix_err[4][3] = {{0.9099, 1.3331, 1.0442}, //rows are HOFR  bb, c, and light in this order  
				     {0.8856, 1.2975, 1.0163}, //columns are njets
				     {0.9099, 1.3331, 1.0442},
				     {0.8856, 1.2975, 1.0163}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{0.7737, 1.5702, 1.0264}, //rows are HOFR  bb, c, and light in this order  
				     {0.7531, 1.5282, 0.9990}, //columns are njets
				     {0.7737, 1.5702, 1.0264},
				     {0.7531, 1.5282, 0.9990}};
  
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
  
  const double SFmatrix_err[4][3] = {{4.9403*0.8413  ,4.9808*1.4526  , 0.5913*1.0352}, //rows are HOFR  bb, c, and light in this order  
				     {4.9591*0.8188 , 4.9995*1.4138 ,  0.7321*1.0075}, //columns are njets
				     {4.9403*0.7627 , 4.9808*1.3169  , 0.5913*0.9385},
				     {4.9591*0.6450 , 4.9995*1.1136 ,  0.7321*0.7936}};

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
  
  const double SFmatrix_err[4][3] = {{62.0216*0.8413, 46.2554*1.4526, 13.8716*1.0352}, //rows are HOFR  bb, c, and light in this order  
				     {66.2327*0.8188, 44.7116*1.4138, 21.1596*1.0075}, //columns are njets
				     {62.0216*0.7627, 46.2554*1.3169, 13.8716*0.9385},
				     {66.2327*0.6450, 44.7116*1.1136, 21.1596*0.7936}};

  
  
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
  
  const double SFmatrix_err[4][3] = {{ 62.2181*0.8413, 46.5228*1.4526, 13.8842*1.0352}, //rows are HOFR  bb, c, and light in this order  
				     { 66.4180*0.8188, 44.9902*1.4138, 21.1723*1.0075}, //columns are njets
				     { 62.2181*0.7627, 46.5228*1.3169, 13.8842*0.9385},
				     { 66.4180*0.6450, 44.9902*1.1136, 21.1723*0.7936}};

  
  
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

