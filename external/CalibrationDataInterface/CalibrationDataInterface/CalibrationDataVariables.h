//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
// CalibrationDataVariables                                                                 //
//                                                                                          //
// This class (struct, actually) is nothing but a light-weight container of (kinematic or   //
// other) variables. Encapsulating these variables has the advantage of providing a         //
// framework-independent interface.                                                         //
// A drawback is that it is not a priori obvious which variables will be relevant           //
// for which calibration. The following rules of thumb apply:                               //
/* Begin_Html
<ul>
  <li>specifying more information than strictly necessary doesn't hurt!</li>
  <li>always specify the jet collection</li>
  <li>the tag weight variable is relevant only in case of so-called continuous tagging</li>
</ul>
End_Html */
//                                                                                          //
// CalibrationDataVariables.h, (c) ATLAS Detector software                                  //
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ANALYSISCALIBRATIONDATAVARIABLES_H
#define ANALYSISCALIBRATIONDATAVARIABLES_H

#include <string>
#include <utility>

namespace Analysis 
{

  /** @class CalibrationDataVariables

      This class (struct, actually) is nothing but a light-weight container of
      (kinematic or other) variables.

      @author  Frank Filthaut <F.Filthaut@science.ru.nl>
  */  

  struct CalibrationDataVariables {
    std::string jetAuthor;
    double jetPt;                    // in MeV
    double jetEta;
    double jetTagWeight;             // actual output of the tagging algorithm (relevant only for "continuous" tagging)
  };

}

#endif // ANALYSISCALIBRATIONDATAVARIABLES_H
