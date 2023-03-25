#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dImntdIeehdIkaphledIatlasdIAPlusPlus_gitdIexternaldIRootCoredIobjdIx86_64mILinuxmIgcc4dO9dO3mIroot6dO08dO02dIPdfReweightingTooldIobjdIPdfReweightingToolCINT

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
#include "PdfReweightingTool/PdfReweightingTool.h"
#include "PdfReweightingTool/PdfSelector.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_PdfSet(void *p);
   static void deleteArray_PdfSet(void *p);
   static void destruct_PdfSet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PdfSet*)
   {
      ::PdfSet *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PdfSet >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PdfSet", ::PdfSet::Class_Version(), "PdfReweightingTool/PdfSet.h", 27,
                  typeid(::PdfSet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PdfSet::Dictionary, isa_proxy, 4,
                  sizeof(::PdfSet) );
      instance.SetDelete(&delete_PdfSet);
      instance.SetDeleteArray(&deleteArray_PdfSet);
      instance.SetDestructor(&destruct_PdfSet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PdfSet*)
   {
      return GenerateInitInstanceLocal((::PdfSet*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PdfSet*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PdfReweightingTool(void *p = 0);
   static void *newArray_PdfReweightingTool(Long_t size, void *p);
   static void delete_PdfReweightingTool(void *p);
   static void deleteArray_PdfReweightingTool(void *p);
   static void destruct_PdfReweightingTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PdfReweightingTool*)
   {
      ::PdfReweightingTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PdfReweightingTool >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PdfReweightingTool", ::PdfReweightingTool::Class_Version(), "PdfReweightingTool/PdfReweightingTool.h", 18,
                  typeid(::PdfReweightingTool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PdfReweightingTool::Dictionary, isa_proxy, 4,
                  sizeof(::PdfReweightingTool) );
      instance.SetNew(&new_PdfReweightingTool);
      instance.SetNewArray(&newArray_PdfReweightingTool);
      instance.SetDelete(&delete_PdfReweightingTool);
      instance.SetDeleteArray(&deleteArray_PdfReweightingTool);
      instance.SetDestructor(&destruct_PdfReweightingTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PdfReweightingTool*)
   {
      return GenerateInitInstanceLocal((::PdfReweightingTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PdfReweightingTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PdfSelector(void *p);
   static void deleteArray_PdfSelector(void *p);
   static void destruct_PdfSelector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PdfSelector*)
   {
      ::PdfSelector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PdfSelector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PdfSelector", ::PdfSelector::Class_Version(), "PdfReweightingTool/PdfSelector.h", 35,
                  typeid(::PdfSelector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PdfSelector::Dictionary, isa_proxy, 4,
                  sizeof(::PdfSelector) );
      instance.SetDelete(&delete_PdfSelector);
      instance.SetDeleteArray(&deleteArray_PdfSelector);
      instance.SetDestructor(&destruct_PdfSelector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PdfSelector*)
   {
      return GenerateInitInstanceLocal((::PdfSelector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PdfSelector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PdfSet::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PdfSet::Class_Name()
{
   return "PdfSet";
}

//______________________________________________________________________________
const char *PdfSet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfSet*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PdfSet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfSet*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PdfSet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfSet*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PdfSet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfSet*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PdfReweightingTool::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PdfReweightingTool::Class_Name()
{
   return "PdfReweightingTool";
}

//______________________________________________________________________________
const char *PdfReweightingTool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfReweightingTool*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PdfReweightingTool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfReweightingTool*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PdfReweightingTool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfReweightingTool*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PdfReweightingTool::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfReweightingTool*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PdfSelector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PdfSelector::Class_Name()
{
   return "PdfSelector";
}

//______________________________________________________________________________
const char *PdfSelector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfSelector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PdfSelector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdfSelector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PdfSelector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfSelector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PdfSelector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdfSelector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PdfSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class PdfSet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PdfSet::Class(),this);
   } else {
      R__b.WriteClassBuffer(PdfSet::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PdfSet(void *p) {
      delete ((::PdfSet*)p);
   }
   static void deleteArray_PdfSet(void *p) {
      delete [] ((::PdfSet*)p);
   }
   static void destruct_PdfSet(void *p) {
      typedef ::PdfSet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PdfSet

//______________________________________________________________________________
void PdfReweightingTool::Streamer(TBuffer &R__b)
{
   // Stream an object of class PdfReweightingTool.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PdfReweightingTool::Class(),this);
   } else {
      R__b.WriteClassBuffer(PdfReweightingTool::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PdfReweightingTool(void *p) {
      return  p ? new(p) ::PdfReweightingTool : new ::PdfReweightingTool;
   }
   static void *newArray_PdfReweightingTool(Long_t nElements, void *p) {
      return p ? new(p) ::PdfReweightingTool[nElements] : new ::PdfReweightingTool[nElements];
   }
   // Wrapper around operator delete
   static void delete_PdfReweightingTool(void *p) {
      delete ((::PdfReweightingTool*)p);
   }
   static void deleteArray_PdfReweightingTool(void *p) {
      delete [] ((::PdfReweightingTool*)p);
   }
   static void destruct_PdfReweightingTool(void *p) {
      typedef ::PdfReweightingTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PdfReweightingTool

//______________________________________________________________________________
void PdfSelector::Streamer(TBuffer &R__b)
{
   // Stream an object of class PdfSelector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PdfSelector::Class(),this);
   } else {
      R__b.WriteClassBuffer(PdfSelector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PdfSelector(void *p) {
      delete ((::PdfSelector*)p);
   }
   static void deleteArray_PdfSelector(void *p) {
      delete [] ((::PdfSelector*)p);
   }
   static void destruct_PdfSelector(void *p) {
      typedef ::PdfSelector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PdfSelector

namespace {
  void TriggerDictionaryInitialization_PdfReweightingToolCINT_Impl() {
    static const char* headers[] = {
"PdfReweightingTool/PdfReweightingTool.h",
"PdfReweightingTool/PdfSelector.h",
0
    };
    static const char* includePaths[] = {
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/PdfReweightingTool/Root",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/PdfReweightingTool",
"/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/include",
"/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include",
"/cvmfs/sft.cern.ch/lcg/releases/ROOT/6.08.02-99084/x86_64-slc6-gcc49-opt/include",
"/mnt/eeh/kaphle/atlas/APlusPlus_git/external/PdfReweightingTool/cmt/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PdfReweightingToolCINT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$PdfReweightingTool/PdfSet.h")))  __attribute__((annotate("$clingAutoload$PdfReweightingTool/PdfReweightingTool.h")))  PdfSet;
class __attribute__((annotate("$clingAutoload$PdfReweightingTool/PdfReweightingTool.h")))  PdfReweightingTool;
class __attribute__((annotate(R"ATTRDUMP(PdfReweightingTool analysis selector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PdfReweightingTool/PdfSelector.h")))  PdfSelector;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PdfReweightingToolCINT dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef ROOTCORE
  #define ROOTCORE 1
#endif
#ifndef ROOTCORE_PACKAGE
  #define ROOTCORE_PACKAGE "PdfReweightingTool"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "PdfReweightingTool/PdfReweightingTool.h"
#include "PdfReweightingTool/PdfSelector.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PdfReweightingTool", payloadCode, "@",
"PdfSelector", payloadCode, "@",
"PdfSet", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PdfReweightingToolCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PdfReweightingToolCINT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PdfReweightingToolCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PdfReweightingToolCINT() {
  TriggerDictionaryInitialization_PdfReweightingToolCINT_Impl();
}
