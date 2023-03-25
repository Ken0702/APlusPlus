cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libSingleTop CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib SingleTop)

find_package(RootCore REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libHepUtils/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasRun/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasAnalysis/inc")
include_directories("${CMAKE_SOURCE_DIR}/libKinFitter/KinFitter")
include_directories("${CMAKE_SOURCE_DIR}/libKinFitter")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasFinders/inc")
include_directories("${CMAKE_SOURCE_DIR}/libSingleTop/inc")

set(SOURCES
    src/AtlEvtReaderMem.cxx
    src/AtlEvtReaderMemBDT.cxx
    src/AtlEvtReaderMemR2.cxx
    src/AtlEvtReaderMemR2JetLepton.cxx
    src/AtlEvtWriterLPSC.cxx
    src/AtlEvtWriterMemR2.cxx
    src/AtlMemNtupleProduction.cxx
    src/AtlPdfReweightingTool.cxx
    src/AtlSgTopEntryListWriter.cxx
    src/AtlSgTopFCNC_tZ_Base.cxx
    src/AtlSgTopFCNC_tZ_Base3L.cxx
    src/AtlSgTopFCNC_tZ_CtrlSM.cxx
    src/AtlSgTopFCNC_tZ_CtrlTtbar.cxx
    src/AtlSgTopFCNC_tZ_CtrlWZ3L.cxx
    src/AtlSgTopFCNC_tZ_CtrlZjets2L.cxx
    src/AtlSgTopFCNC_tZ_CtrlZjets3L.cxx
    src/AtlSgTopFCNC_tZ_Ctrlttbar2L.cxx
    src/AtlSgTopFCNC_tZ_Multi.cxx
    src/AtlSgTopFCNC_tZ_Signal.cxx
    src/AtlSgTopValidation.cxx
    src/AtlSgTop_combinationAnalysis.cxx
    src/AtlSgTop_combinationMemDiscR2.cxx
    src/AtlSgTop_sChannelAnalysis.cxx
    src/AtlSgTop_sChannelBDT.cxx
    src/AtlSgTop_sChannelMemDisc.cxx
    src/AtlSgTop_sChannelMemDiscR2.cxx
    src/AtlSgTop_tChannelAnalysis.cxx
    src/AtlSgTop_tChannelMemDiscr.cxx
    src/AtlTRExFitterTask.cxx
    src/AtlWjetsScaleFactorTool.cxx
    src/AtlWtChallenge.cxx
    src/AtlZllAnalysisDemo.cxx
    src/Atl_WtChannelAnalysis.cxx
    src/Atl_tChannelAnalysis.cxx
   )

set(HEADERS
    inc/AtlEvtReaderMem.h
    inc/AtlEvtReaderMemBDT.h
    inc/AtlEvtReaderMemR2.h
    inc/AtlEvtReaderMemR2JetLepton.h
    inc/AtlEvtWriterLPSC.h
    inc/AtlEvtWriterMemR2.h
    inc/AtlMemNtupleProduction.h
    inc/AtlPdfReweightingTool.h
    inc/AtlSgTopEntryListWriter.h
    inc/AtlSgTopFCNC_tZ_Base.h
    inc/AtlSgTopFCNC_tZ_Base3L.h
    inc/AtlSgTopFCNC_tZ_CtrlSM.h
    inc/AtlSgTopFCNC_tZ_CtrlTtbar.h
    inc/AtlSgTopFCNC_tZ_CtrlWZ3L.h
    inc/AtlSgTopFCNC_tZ_CtrlZjets2L.h
    inc/AtlSgTopFCNC_tZ_CtrlZjets3L.h
    inc/AtlSgTopFCNC_tZ_Ctrlttbar2L.h
    inc/AtlSgTopFCNC_tZ_Multi.h
    inc/AtlSgTopFCNC_tZ_Signal.h
    inc/AtlSgTopValidation.h
    inc/AtlSgTop_combinationAnalysis.h
    inc/AtlSgTop_combinationMemDiscR2.h
    inc/AtlSgTop_sChannelAnalysis.h
    inc/AtlSgTop_sChannelBDT.h
    inc/AtlSgTop_sChannelMemDisc.h
    inc/AtlSgTop_sChannelMemDiscR2.h
    inc/AtlSgTop_tChannelAnalysis.h
    inc/AtlSgTop_tChannelMemDiscr.h
    inc/AtlTRExFitterTask.h
    inc/AtlWjetsScaleFactorTool.h
    inc/AtlWtChallenge.h
    inc/AtlZllAnalysisDemo.h
    inc/Atl_WtChannelAnalysis.h
    inc/Atl_tChannelAnalysis.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})
target_link_libraries(${lib} PUBLIC HepEvent)
target_link_libraries(${lib} PUBLIC HepUtils)
target_link_libraries(${lib} PUBLIC AtlasRun)
target_link_libraries(${lib} PUBLIC AtlasEvent)
target_link_libraries(${lib} PUBLIC AtlasAnalysis)
target_link_libraries(${lib} PUBLIC KinFitter)
target_link_libraries(${lib} PUBLIC AtlasFinders)
target_link_libraries(${lib} PUBLIC CalibrationDataInterface)
target_link_libraries(${lib} PUBLIC PdfReweightingTool)
target_link_libraries(${lib} PUBLIC TopFakes)

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
