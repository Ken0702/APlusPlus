#include <map>
#include <string>
#include "TString.h"
#include <TPileupReweighting.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace Root ;

#pragma link C++ class Root::TPileupReweighting+ ; 
#pragma link C++ class std::vector<double>+ ;
#pragma link C++ class std::map<UInt_t,std::map<UInt_t,UInt_t> >+ ;
#pragma link C++ class std::map<UInt_t,std::vector<UInt_t> >+ ; 
#pragma link C++ class std::map<TString, TH1D*>+ ;
#pragma link C++ class std::map<int,int>+ ;
#pragma link C++ class std::map<int,TH1D*>+ ;
#pragma link C++ class std::map<int, std::map<int,TH1D*> >+ ;
#pragma link C++ class std::map<int,double>+ ;
#pragma link C++ class std::map<int, std::map<UInt_t,TH1D*> >+ ;
#pragma link C++ class std::map<int, std::map<int,bool> >+ ;
#pragma link C++ class std::map<TString, std::map<int,TH1D*> >+ ;
#pragma link C++ class std::map<TString, double>+ ;
#pragma link C++ class std::map<TString, std::map<int, std::map<int,TH1D*> > >+ ;
#pragma link C++ class std::map<TString, std::map<int,double> >+ ;
#pragma link C++ class std::map<TString, std::map<int, std::map<UInt_t,TH1D*> > >+ ;
#pragma link C++ class std::map<TString, std::map<int, std::map<int,bool> > >+ ;

#pragma link C++ class std::map<UInt_t, std::map<UInt_t, double> >+ ;
#pragma link C++ class std::map<UInt_t, double>+ ;
#pragma link C++ class std::map<int, std::map<UInt_t, double> >+ ;

#endif
