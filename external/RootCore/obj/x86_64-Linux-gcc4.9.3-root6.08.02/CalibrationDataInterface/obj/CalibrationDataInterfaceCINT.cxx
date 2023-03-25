#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dImntdIeehdIkaphledIatlasdIAPlusPlus_gitdIexternaldIRootCoredIobjdIx86_64mILinuxmIgcc4dO9dO3mIroot6dO08dO02dICalibrationDataInterfacedIobjdICalibrationDataInterfaceCINT

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "CalibrationDataInterface/CalibrationDataVariables.h"
#include "CalibrationDataInterface/CalibrationDataInterfaceBase.h"
#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#include "CalibrationDataInterface/CalibrationDataContainer.h"
#include "CalibrationDataInterface/CalibrationDataEigenVariations.h"
#include "CalibrationDataInterface/CalibrationDataBackend.h"
#include "CalibrationDataInterface/CalibrationDataUtilities.h"

// Header files passed via #pragma extra_include

namespace Analysis {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *Analysis_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("Analysis", 0 /*version*/, "CalibrationDataInterface/CalibrationDataVariables.h", 27,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &Analysis_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *Analysis_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *AnalysiscLcLCalibrationDataVariables_Dictionary();
   static void AnalysiscLcLCalibrationDataVariables_TClassManip(TClass*);
   static void *new_AnalysiscLcLCalibrationDataVariables(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataVariables(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataVariables(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataVariables(void *p);
   static void destruct_AnalysiscLcLCalibrationDataVariables(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataVariables*)
   {
      ::Analysis::CalibrationDataVariables *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analysis::CalibrationDataVariables));
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataVariables", "CalibrationDataInterface/CalibrationDataVariables.h", 38,
                  typeid(::Analysis::CalibrationDataVariables), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysiscLcLCalibrationDataVariables_Dictionary, isa_proxy, 0,
                  sizeof(::Analysis::CalibrationDataVariables) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataVariables);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataVariables);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataVariables);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataVariables);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataVariables);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataVariables*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataVariables*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataVariables*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysiscLcLCalibrationDataVariables_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataVariables*)0x0)->GetClass();
      AnalysiscLcLCalibrationDataVariables_TClassManip(theClass);
   return theClass;
   }

   static void AnalysiscLcLCalibrationDataVariables_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_AnalysiscLcLCalibrationDataInterfaceBase(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceBase(void *p);
   static void destruct_AnalysiscLcLCalibrationDataInterfaceBase(void *p);
   static void streamer_AnalysiscLcLCalibrationDataInterfaceBase(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataInterfaceBase*)
   {
      ::Analysis::CalibrationDataInterfaceBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataInterfaceBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataInterfaceBase", ::Analysis::CalibrationDataInterfaceBase::Class_Version(), "CalibrationDataInterface/CalibrationDataInterfaceBase.h", 43,
                  typeid(::Analysis::CalibrationDataInterfaceBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataInterfaceBase::Dictionary, isa_proxy, 16,
                  sizeof(::Analysis::CalibrationDataInterfaceBase) );
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataInterfaceBase);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataInterfaceBase);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataInterfaceBase);
      instance.SetStreamerFunc(&streamer_AnalysiscLcLCalibrationDataInterfaceBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataInterfaceBase*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataInterfaceBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceBase*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysiscLcLCalibrationDataInterfaceROOT(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataInterfaceROOT(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataInterfaceROOT(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceROOT(void *p);
   static void destruct_AnalysiscLcLCalibrationDataInterfaceROOT(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataInterfaceROOT*)
   {
      ::Analysis::CalibrationDataInterfaceROOT *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataInterfaceROOT >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataInterfaceROOT", ::Analysis::CalibrationDataInterfaceROOT::Class_Version(), "CalibrationDataInterface/CalibrationDataInterfaceROOT.h", 80,
                  typeid(::Analysis::CalibrationDataInterfaceROOT), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataInterfaceROOT::Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataInterfaceROOT) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataInterfaceROOT);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataInterfaceROOT);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataInterfaceROOT);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataInterfaceROOT);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataInterfaceROOT);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataInterfaceROOT*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataInterfaceROOT*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_Dictionary();
   static void AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_TClassManip(TClass*);
   static void *new_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p);
   static void destruct_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)
   {
      ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper));
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper", "CalibrationDataInterface/CalibrationDataInterfaceROOT.h", 280,
                  typeid(::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)0x0)->GetClass();
      AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_TClassManip(theClass);
   return theClass;
   }

   static void AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_AnalysiscLcLCalibrationDataContainer(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataContainer(void *p);
   static void destruct_AnalysiscLcLCalibrationDataContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataContainer*)
   {
      ::Analysis::CalibrationDataContainer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataContainer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataContainer", ::Analysis::CalibrationDataContainer::Class_Version(), "CalibrationDataInterface/CalibrationDataContainer.h", 47,
                  typeid(::Analysis::CalibrationDataContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataContainer::Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataContainer) );
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataContainer);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataContainer);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataContainer*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataContainer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataContainer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysiscLcLCalibrationDataHistogramContainer(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataHistogramContainer(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataHistogramContainer(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataHistogramContainer(void *p);
   static void destruct_AnalysiscLcLCalibrationDataHistogramContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataHistogramContainer*)
   {
      ::Analysis::CalibrationDataHistogramContainer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataHistogramContainer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataHistogramContainer", ::Analysis::CalibrationDataHistogramContainer::Class_Version(), "CalibrationDataInterface/CalibrationDataContainer.h", 247,
                  typeid(::Analysis::CalibrationDataHistogramContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataHistogramContainer::Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataHistogramContainer) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataHistogramContainer);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataHistogramContainer);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataHistogramContainer);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataHistogramContainer);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataHistogramContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataHistogramContainer*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataHistogramContainer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataHistogramContainer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataMappedHistogramContainer(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p);
   static void destruct_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataMappedHistogramContainer*)
   {
      ::Analysis::CalibrationDataMappedHistogramContainer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataMappedHistogramContainer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataMappedHistogramContainer", ::Analysis::CalibrationDataMappedHistogramContainer::Class_Version(), "CalibrationDataInterface/CalibrationDataContainer.h", 329,
                  typeid(::Analysis::CalibrationDataMappedHistogramContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataMappedHistogramContainer::Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataMappedHistogramContainer) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataMappedHistogramContainer);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataMappedHistogramContainer);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataMappedHistogramContainer);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainer);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataMappedHistogramContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataMappedHistogramContainer*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataMappedHistogramContainer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_Dictionary();
   static void AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_TClassManip(TClass*);
   static void *new_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p);
   static void destruct_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataMappedHistogramContainer::Bin*)
   {
      ::Analysis::CalibrationDataMappedHistogramContainer::Bin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analysis::CalibrationDataMappedHistogramContainer::Bin));
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataMappedHistogramContainer::Bin", "CalibrationDataInterface/CalibrationDataContainer.h", 361,
                  typeid(::Analysis::CalibrationDataMappedHistogramContainer::Bin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataMappedHistogramContainer::Bin) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataMappedHistogramContainer::Bin*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataMappedHistogramContainer::Bin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer::Bin*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer::Bin*)0x0)->GetClass();
      AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_TClassManip(theClass);
   return theClass;
   }

   static void AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysiscLcLCalibrationDataFunctionContainer(void *p = 0);
   static void *newArray_AnalysiscLcLCalibrationDataFunctionContainer(Long_t size, void *p);
   static void delete_AnalysiscLcLCalibrationDataFunctionContainer(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataFunctionContainer(void *p);
   static void destruct_AnalysiscLcLCalibrationDataFunctionContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataFunctionContainer*)
   {
      ::Analysis::CalibrationDataFunctionContainer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analysis::CalibrationDataFunctionContainer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataFunctionContainer", ::Analysis::CalibrationDataFunctionContainer::Class_Version(), "CalibrationDataInterface/CalibrationDataContainer.h", 420,
                  typeid(::Analysis::CalibrationDataFunctionContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Analysis::CalibrationDataFunctionContainer::Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::CalibrationDataFunctionContainer) );
      instance.SetNew(&new_AnalysiscLcLCalibrationDataFunctionContainer);
      instance.SetNewArray(&newArray_AnalysiscLcLCalibrationDataFunctionContainer);
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataFunctionContainer);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataFunctionContainer);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataFunctionContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataFunctionContainer*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataFunctionContainer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataFunctionContainer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysiscLcLCalibrationDataEigenVariations_Dictionary();
   static void AnalysiscLcLCalibrationDataEigenVariations_TClassManip(TClass*);
   static void delete_AnalysiscLcLCalibrationDataEigenVariations(void *p);
   static void deleteArray_AnalysiscLcLCalibrationDataEigenVariations(void *p);
   static void destruct_AnalysiscLcLCalibrationDataEigenVariations(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::CalibrationDataEigenVariations*)
   {
      ::Analysis::CalibrationDataEigenVariations *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analysis::CalibrationDataEigenVariations));
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::CalibrationDataEigenVariations", "CalibrationDataInterface/CalibrationDataEigenVariations.h", 20,
                  typeid(::Analysis::CalibrationDataEigenVariations), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysiscLcLCalibrationDataEigenVariations_Dictionary, isa_proxy, 0,
                  sizeof(::Analysis::CalibrationDataEigenVariations) );
      instance.SetDelete(&delete_AnalysiscLcLCalibrationDataEigenVariations);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLCalibrationDataEigenVariations);
      instance.SetDestructor(&destruct_AnalysiscLcLCalibrationDataEigenVariations);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::CalibrationDataEigenVariations*)
   {
      return GenerateInitInstanceLocal((::Analysis::CalibrationDataEigenVariations*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::CalibrationDataEigenVariations*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysiscLcLCalibrationDataEigenVariations_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataEigenVariations*)0x0)->GetClass();
      AnalysiscLcLCalibrationDataEigenVariations_TClassManip(theClass);
   return theClass;
   }

   static void AnalysiscLcLCalibrationDataEigenVariations_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataInterfaceBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataInterfaceBase::Class_Name()
{
   return "Analysis::CalibrationDataInterfaceBase";
}

//______________________________________________________________________________
const char *CalibrationDataInterfaceBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataInterfaceBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataInterfaceBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataInterfaceBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceBase*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataInterfaceROOT::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataInterfaceROOT::Class_Name()
{
   return "Analysis::CalibrationDataInterfaceROOT";
}

//______________________________________________________________________________
const char *CalibrationDataInterfaceROOT::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataInterfaceROOT::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataInterfaceROOT::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataInterfaceROOT::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataInterfaceROOT*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataContainer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataContainer::Class_Name()
{
   return "Analysis::CalibrationDataContainer";
}

//______________________________________________________________________________
const char *CalibrationDataContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataContainer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataContainer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataContainer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataContainer*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataHistogramContainer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataHistogramContainer::Class_Name()
{
   return "Analysis::CalibrationDataHistogramContainer";
}

//______________________________________________________________________________
const char *CalibrationDataHistogramContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataHistogramContainer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataHistogramContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataHistogramContainer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataHistogramContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataHistogramContainer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataHistogramContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataHistogramContainer*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataMappedHistogramContainer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataMappedHistogramContainer::Class_Name()
{
   return "Analysis::CalibrationDataMappedHistogramContainer";
}

//______________________________________________________________________________
const char *CalibrationDataMappedHistogramContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataMappedHistogramContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataMappedHistogramContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataMappedHistogramContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataMappedHistogramContainer*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace Analysis {
//______________________________________________________________________________
atomic_TClass_ptr CalibrationDataFunctionContainer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CalibrationDataFunctionContainer::Class_Name()
{
   return "Analysis::CalibrationDataFunctionContainer";
}

//______________________________________________________________________________
const char *CalibrationDataFunctionContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataFunctionContainer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CalibrationDataFunctionContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataFunctionContainer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalibrationDataFunctionContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataFunctionContainer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalibrationDataFunctionContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analysis::CalibrationDataFunctionContainer*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Analysis
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataVariables(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Analysis::CalibrationDataVariables : new ::Analysis::CalibrationDataVariables;
   }
   static void *newArray_AnalysiscLcLCalibrationDataVariables(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Analysis::CalibrationDataVariables[nElements] : new ::Analysis::CalibrationDataVariables[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataVariables(void *p) {
      delete ((::Analysis::CalibrationDataVariables*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataVariables(void *p) {
      delete [] ((::Analysis::CalibrationDataVariables*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataVariables(void *p) {
      typedef ::Analysis::CalibrationDataVariables current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataVariables

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataInterfaceBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataInterfaceBase.

   //This works around a msvc bug and should be harmless on other platforms
   typedef ::Analysis::CalibrationDataInterfaceBase thisClass;
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      {
         map<string,vector<string> > &R__stl =  m_calibrationEffNames;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::vector<class std::basic_string<char>, class std::allocator<class std::basic_string<char> > >));
         if (R__tcl2==0) {
            Error("m_calibrationEffNames streamer","Missing the TClass object for class std::vector<class std::basic_string<char>, class std::allocator<class std::basic_string<char> > >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            vector<string> R__t2;
            R__b.StreamObject(&R__t2,R__tcl2);
            typedef class std::basic_string<char> Value_t;
            std::pair<Value_t const, class std::vector<class std::basic_string<char>, class std::allocator<class std::basic_string<char> > > > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<string,string> &R__stl =  m_calibrationSFNames;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            string R__t2;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t2 = R__str.Data();}
            typedef class std::basic_string<char> Value_t;
            std::pair<Value_t const, class std::basic_string<char> > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      { TString R__str; R__str.Streamer(R__b); m_taggerName = R__str.Data(); }
      {
         vector<string> &R__stl =  m_operatingPoints;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, thisClass::IsA());
   } else {
      R__c = R__b.WriteVersion(thisClass::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      {
         map<string,vector<string> > &R__stl =  m_calibrationEffNames;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::vector<class std::basic_string<char>, class std::allocator<class std::basic_string<char> > >));
         if (R__tcl2==0) {
            Error("m_calibrationEffNamesstreamer","Missing the TClass object for class std::vector<class std::basic_string<char>, class std::allocator<class std::basic_string<char> > >!");
            return;
         }
            map<string,vector<string> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            R__b.StreamObject((vector<string>*)&((*R__k).second),R__tcl2);
            }
         }
      }
      {
         map<string,string> &R__stl =  m_calibrationSFNames;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<string,string>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            {TString R__str(((*R__k).second).c_str());
             R__str.Streamer(R__b);};
            }
         }
      }
      { TString R__str = m_taggerName.c_str(); R__str.Streamer(R__b);}
      {
         vector<string> &R__stl =  m_operatingPoints;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<string>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str((*R__k).c_str());
             R__str.Streamer(R__b);};
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataInterfaceBase(void *p) {
      delete ((::Analysis::CalibrationDataInterfaceBase*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceBase(void *p) {
      delete [] ((::Analysis::CalibrationDataInterfaceBase*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataInterfaceBase(void *p) {
      typedef ::Analysis::CalibrationDataInterfaceBase current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysiscLcLCalibrationDataInterfaceBase(TBuffer &buf, void *obj) {
      ((::Analysis::CalibrationDataInterfaceBase*)obj)->::Analysis::CalibrationDataInterfaceBase::Streamer(buf);
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataInterfaceBase

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataInterfaceROOT::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataInterfaceROOT.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Analysis::CalibrationDataInterfaceROOT::Class(),this);
   } else {
      R__b.WriteClassBuffer(Analysis::CalibrationDataInterfaceROOT::Class(),this);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataInterfaceROOT(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataInterfaceROOT : new ::Analysis::CalibrationDataInterfaceROOT;
   }
   static void *newArray_AnalysiscLcLCalibrationDataInterfaceROOT(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataInterfaceROOT[nElements] : new ::Analysis::CalibrationDataInterfaceROOT[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataInterfaceROOT(void *p) {
      delete ((::Analysis::CalibrationDataInterfaceROOT*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceROOT(void *p) {
      delete [] ((::Analysis::CalibrationDataInterfaceROOT*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataInterfaceROOT(void *p) {
      typedef ::Analysis::CalibrationDataInterfaceROOT current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataInterfaceROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper : new ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper;
   }
   static void *newArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper[nElements] : new ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p) {
      delete ((::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p) {
      delete [] ((::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelper(void *p) {
      typedef ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Analysis::CalibrationDataContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(Analysis::CalibrationDataContainer::Class(),this);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataContainer(void *p) {
      delete ((::Analysis::CalibrationDataContainer*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataContainer(void *p) {
      delete [] ((::Analysis::CalibrationDataContainer*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataContainer(void *p) {
      typedef ::Analysis::CalibrationDataContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataContainer

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataHistogramContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataHistogramContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Analysis::CalibrationDataHistogramContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(Analysis::CalibrationDataHistogramContainer::Class(),this);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataHistogramContainer(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataHistogramContainer : new ::Analysis::CalibrationDataHistogramContainer;
   }
   static void *newArray_AnalysiscLcLCalibrationDataHistogramContainer(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataHistogramContainer[nElements] : new ::Analysis::CalibrationDataHistogramContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataHistogramContainer(void *p) {
      delete ((::Analysis::CalibrationDataHistogramContainer*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataHistogramContainer(void *p) {
      delete [] ((::Analysis::CalibrationDataHistogramContainer*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataHistogramContainer(void *p) {
      typedef ::Analysis::CalibrationDataHistogramContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataHistogramContainer

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataMappedHistogramContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataMappedHistogramContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Analysis::CalibrationDataMappedHistogramContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(Analysis::CalibrationDataMappedHistogramContainer::Class(),this);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataMappedHistogramContainer : new ::Analysis::CalibrationDataMappedHistogramContainer;
   }
   static void *newArray_AnalysiscLcLCalibrationDataMappedHistogramContainer(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataMappedHistogramContainer[nElements] : new ::Analysis::CalibrationDataMappedHistogramContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p) {
      delete ((::Analysis::CalibrationDataMappedHistogramContainer*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p) {
      delete [] ((::Analysis::CalibrationDataMappedHistogramContainer*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataMappedHistogramContainer(void *p) {
      typedef ::Analysis::CalibrationDataMappedHistogramContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataMappedHistogramContainer

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataMappedHistogramContainer::Bin : new ::Analysis::CalibrationDataMappedHistogramContainer::Bin;
   }
   static void *newArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataMappedHistogramContainer::Bin[nElements] : new ::Analysis::CalibrationDataMappedHistogramContainer::Bin[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p) {
      delete ((::Analysis::CalibrationDataMappedHistogramContainer::Bin*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p) {
      delete [] ((::Analysis::CalibrationDataMappedHistogramContainer::Bin*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataMappedHistogramContainercLcLBin(void *p) {
      typedef ::Analysis::CalibrationDataMappedHistogramContainer::Bin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataMappedHistogramContainer::Bin

namespace Analysis {
//______________________________________________________________________________
void CalibrationDataFunctionContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analysis::CalibrationDataFunctionContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Analysis::CalibrationDataFunctionContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(Analysis::CalibrationDataFunctionContainer::Class(),this);
   }
}

} // namespace Analysis
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysiscLcLCalibrationDataFunctionContainer(void *p) {
      return  p ? new(p) ::Analysis::CalibrationDataFunctionContainer : new ::Analysis::CalibrationDataFunctionContainer;
   }
   static void *newArray_AnalysiscLcLCalibrationDataFunctionContainer(Long_t nElements, void *p) {
      return p ? new(p) ::Analysis::CalibrationDataFunctionContainer[nElements] : new ::Analysis::CalibrationDataFunctionContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataFunctionContainer(void *p) {
      delete ((::Analysis::CalibrationDataFunctionContainer*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataFunctionContainer(void *p) {
      delete [] ((::Analysis::CalibrationDataFunctionContainer*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataFunctionContainer(void *p) {
      typedef ::Analysis::CalibrationDataFunctionContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataFunctionContainer

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysiscLcLCalibrationDataEigenVariations(void *p) {
      delete ((::Analysis::CalibrationDataEigenVariations*)p);
   }
   static void deleteArray_AnalysiscLcLCalibrationDataEigenVariations(void *p) {
      delete [] ((::Analysis::CalibrationDataEigenVariations*)p);
   }
   static void destruct_AnalysiscLcLCalibrationDataEigenVariations(void *p) {
      typedef ::Analysis::CalibrationDataEigenVariations current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::CalibrationDataEigenVariations

namespace ROOT {
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = 0);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 214,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)0x0)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 214,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary();
   static void vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);
   static void destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<unsigned int,unsigned int> >*)
   {
      vector<pair<unsigned int,unsigned int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<unsigned int,unsigned int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<unsigned int,unsigned int> >", -2, "vector", 214,
                  typeid(vector<pair<unsigned int,unsigned int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<unsigned int,unsigned int> >) );
      instance.SetNew(&new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<unsigned int,unsigned int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<pair<unsigned int,unsigned int> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<unsigned int,unsigned int> >*)0x0)->GetClass();
      vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned int,unsigned int> > : new vector<pair<unsigned int,unsigned int> >;
   }
   static void *newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned int,unsigned int> >[nElements] : new vector<pair<unsigned int,unsigned int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      delete ((vector<pair<unsigned int,unsigned int> >*)p);
   }
   static void deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      delete [] ((vector<pair<unsigned int,unsigned int> >*)p);
   }
   static void destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      typedef vector<pair<unsigned int,unsigned int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<unsigned int,unsigned int> >

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_Dictionary();
   static void vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_TClassManip(TClass*);
   static void *new_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p = 0);
   static void *newArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(Long_t size, void *p);
   static void delete_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p);
   static void deleteArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p);
   static void destruct_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>*)
   {
      vector<Analysis::CalibrationDataMappedHistogramContainer::Bin> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>", -2, "vector", 214,
                  typeid(vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>) );
      instance.SetNew(&new_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR);
      instance.SetNewArray(&newArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR);
      instance.SetDelete(&delete_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR);
      instance.SetDestructor(&destruct_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Analysis::CalibrationDataMappedHistogramContainer::Bin> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>*)0x0)->GetClass();
      vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Analysis::CalibrationDataMappedHistogramContainer::Bin> : new vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>;
   }
   static void *newArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>[nElements] : new vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p) {
      delete ((vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>*)p);
   }
   static void deleteArray_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p) {
      delete [] ((vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>*)p);
   }
   static void destruct_vectorlEAnalysiscLcLCalibrationDataMappedHistogramContainercLcLBingR(void *p) {
      typedef vector<Analysis::CalibrationDataMappedHistogramContainer::Bin> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Analysis::CalibrationDataMappedHistogramContainer::Bin>

namespace ROOT {
   static TClass *vectorlEAnalysiscLcLCalibrationDataContainermUgR_Dictionary();
   static void vectorlEAnalysiscLcLCalibrationDataContainermUgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p = 0);
   static void *newArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR(Long_t size, void *p);
   static void delete_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p);
   static void deleteArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p);
   static void destruct_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Analysis::CalibrationDataContainer*>*)
   {
      vector<Analysis::CalibrationDataContainer*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Analysis::CalibrationDataContainer*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Analysis::CalibrationDataContainer*>", -2, "vector", 214,
                  typeid(vector<Analysis::CalibrationDataContainer*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysiscLcLCalibrationDataContainermUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Analysis::CalibrationDataContainer*>) );
      instance.SetNew(&new_vectorlEAnalysiscLcLCalibrationDataContainermUgR);
      instance.SetNewArray(&newArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR);
      instance.SetDelete(&delete_vectorlEAnalysiscLcLCalibrationDataContainermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR);
      instance.SetDestructor(&destruct_vectorlEAnalysiscLcLCalibrationDataContainermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Analysis::CalibrationDataContainer*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<Analysis::CalibrationDataContainer*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysiscLcLCalibrationDataContainermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Analysis::CalibrationDataContainer*>*)0x0)->GetClass();
      vectorlEAnalysiscLcLCalibrationDataContainermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysiscLcLCalibrationDataContainermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Analysis::CalibrationDataContainer*> : new vector<Analysis::CalibrationDataContainer*>;
   }
   static void *newArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Analysis::CalibrationDataContainer*>[nElements] : new vector<Analysis::CalibrationDataContainer*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p) {
      delete ((vector<Analysis::CalibrationDataContainer*>*)p);
   }
   static void deleteArray_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p) {
      delete [] ((vector<Analysis::CalibrationDataContainer*>*)p);
   }
   static void destruct_vectorlEAnalysiscLcLCalibrationDataContainermUgR(void *p) {
      typedef vector<Analysis::CalibrationDataContainer*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Analysis::CalibrationDataContainer*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEstringgRsPgR_Dictionary();
   static void maplEstringcOvectorlEstringgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEstringgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEstringgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEstringgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEstringgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEstringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<string> >*)
   {
      map<string,vector<string> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<string> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<string> >", -2, "map", 96,
                  typeid(map<string,vector<string> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEstringgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<string> >) );
      instance.SetNew(&new_maplEstringcOvectorlEstringgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEstringgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEstringgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<string> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,vector<string> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEstringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<string> >*)0x0)->GetClass();
      maplEstringcOvectorlEstringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEstringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEstringgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<string> > : new map<string,vector<string> >;
   }
   static void *newArray_maplEstringcOvectorlEstringgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<string> >[nElements] : new map<string,vector<string> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEstringgRsPgR(void *p) {
      delete ((map<string,vector<string> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEstringgRsPgR(void *p) {
      delete [] ((map<string,vector<string> >*)p);
   }
   static void destruct_maplEstringcOvectorlEstringgRsPgR(void *p) {
      typedef map<string,vector<string> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<string> >

namespace ROOT {
   static TClass *maplEstringcOunsignedsPintgR_Dictionary();
   static void maplEstringcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEstringcOunsignedsPintgR(void *p = 0);
   static void *newArray_maplEstringcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEstringcOunsignedsPintgR(void *p);
   static void deleteArray_maplEstringcOunsignedsPintgR(void *p);
   static void destruct_maplEstringcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,unsigned int>*)
   {
      map<string,unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,unsigned int>", -2, "map", 96,
                  typeid(map<string,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,unsigned int>) );
      instance.SetNew(&new_maplEstringcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEstringcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEstringcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEstringcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,unsigned int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,unsigned int>*)0x0)->GetClass();
      maplEstringcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,unsigned int> : new map<string,unsigned int>;
   }
   static void *newArray_maplEstringcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,unsigned int>[nElements] : new map<string,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOunsignedsPintgR(void *p) {
      delete ((map<string,unsigned int>*)p);
   }
   static void deleteArray_maplEstringcOunsignedsPintgR(void *p) {
      delete [] ((map<string,unsigned int>*)p);
   }
   static void destruct_maplEstringcOunsignedsPintgR(void *p) {
      typedef map<string,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,unsigned int>

namespace ROOT {
   static TClass *maplEstringcOstringgR_Dictionary();
   static void maplEstringcOstringgR_TClassManip(TClass*);
   static void *new_maplEstringcOstringgR(void *p = 0);
   static void *newArray_maplEstringcOstringgR(Long_t size, void *p);
   static void delete_maplEstringcOstringgR(void *p);
   static void deleteArray_maplEstringcOstringgR(void *p);
   static void destruct_maplEstringcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,string>*)
   {
      map<string,string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,string>", -2, "map", 96,
                  typeid(map<string,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,string>) );
      instance.SetNew(&new_maplEstringcOstringgR);
      instance.SetNewArray(&newArray_maplEstringcOstringgR);
      instance.SetDelete(&delete_maplEstringcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOstringgR);
      instance.SetDestructor(&destruct_maplEstringcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,string>*)0x0)->GetClass();
      maplEstringcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,string> : new map<string,string>;
   }
   static void *newArray_maplEstringcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,string>[nElements] : new map<string,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOstringgR(void *p) {
      delete ((map<string,string>*)p);
   }
   static void deleteArray_maplEstringcOstringgR(void *p) {
      delete [] ((map<string,string>*)p);
   }
   static void destruct_maplEstringcOstringgR(void *p) {
      typedef map<string,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,string>

namespace ROOT {
   static TClass *maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_Dictionary();
   static void maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_TClassManip(TClass*);
   static void *new_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p = 0);
   static void *newArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(Long_t size, void *p);
   static void delete_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p);
   static void deleteArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p);
   static void destruct_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>*)
   {
      map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>", -2, "map", 96,
                  typeid(map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>) );
      instance.SetNew(&new_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR);
      instance.SetNewArray(&newArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR);
      instance.SetDelete(&delete_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR);
      instance.SetDestructor(&destruct_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>*)0x0)->GetClass();
      maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*> : new map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>;
   }
   static void *newArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>[nElements] : new map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p) {
      delete ((map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>*)p);
   }
   static void deleteArray_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p) {
      delete [] ((map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>*)p);
   }
   static void destruct_maplEstringcOAnalysiscLcLCalibrationDataInterfaceROOTcLcLHadronisationReferenceHelpermUgR(void *p) {
      typedef map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper*>

namespace ROOT {
   static TClass *maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_Dictionary();
   static void maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_TClassManip(TClass*);
   static void *new_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p = 0);
   static void *newArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(Long_t size, void *p);
   static void delete_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p);
   static void deleteArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p);
   static void destruct_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>*)
   {
      map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>", -2, "map", 96,
                  typeid(map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>) );
      instance.SetNew(&new_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR);
      instance.SetNewArray(&newArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR);
      instance.SetDelete(&delete_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR);
      instance.SetDeleteArray(&deleteArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR);
      instance.SetDestructor(&destruct_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>*)0x0)->GetClass();
      maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*> : new map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>;
   }
   static void *newArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>[nElements] : new map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p) {
      delete ((map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>*)p);
   }
   static void deleteArray_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p) {
      delete [] ((map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>*)p);
   }
   static void destruct_maplEconstsPAnalysiscLcLCalibrationDataContainermUcOconstsPAnalysiscLcLCalibrationDataEigenVariationsmUgR(void *p) {
      typedef map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<const Analysis::CalibrationDataContainer*,const Analysis::CalibrationDataEigenVariations*>

namespace {
  void TriggerDictionaryInitialization_CalibrationDataInterfaceCINT_Impl() {
    static const char* headers[] = {
"CalibrationDataInterface/CalibrationDataVariables.h",
"CalibrationDataInterface/CalibrationDataInterfaceBase.h",
"CalibrationDataInterface/CalibrationDataInterfaceROOT.h",
"CalibrationDataInterface/CalibrationDataContainer.h",
"CalibrationDataInterface/CalibrationDataEigenVariations.h",
"CalibrationDataInterface/CalibrationDataBackend.h",
"CalibrationDataInterface/CalibrationDataUtilities.h",
0
    };
    static const char* includePaths[] = {
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/CalibrationDataInterface/Root",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/CalibrationDataInterface",
"/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/include",
"/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include",
"/cvmfs/sft.cern.ch/lcg/releases/ROOT/6.08.02-99084/x86_64-slc6-gcc49-opt/include",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/CalibrationDataInterface/cmt/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "CalibrationDataInterfaceCINT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace Analysis{struct __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataVariables.h")))  CalibrationDataVariables;}
namespace Analysis{class __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataInterfaceBase.h")))  CalibrationDataInterfaceBase;}
namespace Analysis{class __attribute__((annotate(R"ATTRDUMP(platform-independent (main) interface for access to b-tagging information)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataInterfaceROOT.h")))  CalibrationDataInterfaceROOT;}
namespace Analysis{class __attribute__((annotate(R"ATTRDUMP(base class for calibration data objects)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(base class for calibration data objects)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataContainer.h")))  CalibrationDataContainer;}
namespace Analysis{class __attribute__((annotate(R"ATTRDUMP(histogram based calibration data object)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(histogram based calibration data object)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataContainer.h")))  CalibrationDataHistogramContainer;}
namespace Analysis{class __attribute__((annotate(R"ATTRDUMP('mapped' histogram based calibration data object)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP('mapped' histogram based calibration data object)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataContainer.h")))  CalibrationDataMappedHistogramContainer;}
namespace Analysis{class __attribute__((annotate(R"ATTRDUMP(function based calibration data object)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(function based calibration data object)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataContainer.h")))  CalibrationDataFunctionContainer;}
namespace Analysis{class __attribute__((annotate("$clingAutoload$CalibrationDataInterface/CalibrationDataEigenVariations.h")))  CalibrationDataEigenVariations;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CalibrationDataInterfaceCINT dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef ROOTCORE
  #define ROOTCORE 1
#endif
#ifndef ROOTCORE_PACKAGE
  #define ROOTCORE_PACKAGE "CalibrationDataInterface"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "CalibrationDataInterface/CalibrationDataVariables.h"
#include "CalibrationDataInterface/CalibrationDataInterfaceBase.h"
#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#include "CalibrationDataInterface/CalibrationDataContainer.h"
#include "CalibrationDataInterface/CalibrationDataEigenVariations.h"
#include "CalibrationDataInterface/CalibrationDataBackend.h"
#include "CalibrationDataInterface/CalibrationDataUtilities.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Analysis::CalibrationDataContainer", payloadCode, "@",
"Analysis::CalibrationDataEigenVariations", payloadCode, "@",
"Analysis::CalibrationDataFunctionContainer", payloadCode, "@",
"Analysis::CalibrationDataHistogramContainer", payloadCode, "@",
"Analysis::CalibrationDataInterfaceBase", payloadCode, "@",
"Analysis::CalibrationDataInterfaceROOT", payloadCode, "@",
"Analysis::CalibrationDataInterfaceROOT::HadronisationReferenceHelper", payloadCode, "@",
"Analysis::CalibrationDataMappedHistogramContainer", payloadCode, "@",
"Analysis::CalibrationDataMappedHistogramContainer::Bin", payloadCode, "@",
"Analysis::CalibrationDataVariables", payloadCode, "@",
"Analysis::getHadronisations", payloadCode, "@",
"Analysis::showBuildNumber", payloadCode, "@",
"Analysis::showHadronisations", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CalibrationDataInterfaceCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CalibrationDataInterfaceCINT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CalibrationDataInterfaceCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CalibrationDataInterfaceCINT() {
  TriggerDictionaryInitialization_CalibrationDataInterfaceCINT_Impl();
}
