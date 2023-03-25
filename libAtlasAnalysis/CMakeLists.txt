cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libAtlasAnalysis CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib AtlasAnalysis)

find_package(RootCore REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libHepUtils/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasRun/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasAnalysis/inc")

set(SOURCES
    src/AtlAnalysisTool.cxx
    src/AtlAppAnalysisTask.cxx
    src/AtlBDTAnalysisTask.cxx
    src/AtlBDecayGenTool.cxx
    src/AtlCtrlPlots.cxx
    src/AtlCut.cxx
    src/AtlCutFlowTool.cxx
    src/AtlDataMCPlotterTask.cxx
    src/AtlDocumenter.cxx
    src/AtlEvtReaderApp.cxx
    src/AtlEvtReaderBase.cxx
    src/AtlEvtReaderD3PDBase.cxx
    src/AtlEvtReaderD3PDCKM.cxx
    src/AtlEvtReaderD3PDJetLepton.cxx
    src/AtlEvtReaderD3PDSgTop.cxx
    src/AtlEvtReaderD3PDSgTopR2.cxx
    src/AtlEvtWriterApp.cxx
    src/AtlEvtWriterBase.cxx
    src/AtlEvtWriterD3PDSgTop.cxx
    src/AtlEvtWriterD3PDSgTopThinned.cxx
    src/AtlEvtWriterMem.cxx
    src/AtlExampleAnalysis.cxx
    src/AtlHforSplittingTask.cxx
    src/AtlHistFactoryBreakdownEntry.cxx
    src/AtlHistFactoryBreakdownTask.cxx
    src/AtlHistFactoryChannel.cxx
    src/AtlHistFactoryMeasurement.cxx
    src/AtlHistFactoryPlotterTask.cxx
    src/AtlHistFactorySample.cxx
    src/AtlHistFactorySystEnvelope.cxx
    src/AtlHistFactorySystModelPair.cxx
    src/AtlHistFactorySystNominal.cxx
    src/AtlHistFactorySystOneSided.cxx
    src/AtlHistFactorySystPair.cxx
    src/AtlHistFactorySystPdf.cxx
    src/AtlHistFactorySystematic.cxx
    src/AtlHistFactoryTask.cxx
    src/AtlHistFactoryYieldTableTask.cxx
    src/AtlHistogramTool.cxx
    src/AtlMemTkAnalysisTask.cxx
    src/AtlMergingTask.cxx
    src/AtlObjRecoScaleFactorTool.cxx
    src/AtlObjectsDefinitionTool.cxx
    src/AtlObjectsDefinitionToolR2.cxx
    src/AtlObjectsToolD3PDSgTop.cxx
    src/AtlPlotterFolder.cxx
    src/AtlQCDMMTool.cxx
    src/AtlRunEvtSelectionTool.cxx
    src/AtlSample.cxx
    src/AtlSelector.cxx
    src/AtlSubselectionCuts.cxx
    src/AtlSystematicsTool.cxx
    src/AtlTask.cxx
    src/AtlToolCut.cxx
    src/AtlTopLevelAnalysis.cxx
    src/AtlTopPairDocumenter.cxx
    src/AtlTriggerTool.cxx
    src/AtlTriggerToolItem.cxx
   )

set(HEADERS
    inc/AtlAnalysisTool.h
    inc/AtlAppAnalysisTask.h
    inc/AtlBDTAnalysisTask.h
    inc/AtlBDecayGenTool.h
    inc/AtlCtrlPlots.h
    inc/AtlCut.h
    inc/AtlCutFlowTool.h
    inc/AtlDataMCPlotterTask.h
    inc/AtlDocumenter.h
    inc/AtlEvtReaderApp.h
    inc/AtlEvtReaderBase.h
    inc/AtlEvtReaderD3PDBase.h
    inc/AtlEvtReaderD3PDCKM.h
    inc/AtlEvtReaderD3PDJetLepton.h
    inc/AtlEvtReaderD3PDSgTop.h
    inc/AtlEvtReaderD3PDSgTopR2.h
    inc/AtlEvtWriterApp.h
    inc/AtlEvtWriterBase.h
    inc/AtlEvtWriterD3PDSgTop.h
    inc/AtlEvtWriterD3PDSgTopThinned.h
    inc/AtlEvtWriterMem.h
    inc/AtlExampleAnalysis.h
    inc/AtlHforSplittingTask.h
    inc/AtlHistFactoryBreakdownEntry.h
    inc/AtlHistFactoryBreakdownTask.h
    inc/AtlHistFactoryChannel.h
    inc/AtlHistFactoryMeasurement.h
    inc/AtlHistFactoryPlotterTask.h
    inc/AtlHistFactorySample.h
    inc/AtlHistFactorySystEnvelope.h
    inc/AtlHistFactorySystModelPair.h
    inc/AtlHistFactorySystNominal.h
    inc/AtlHistFactorySystOneSided.h
    inc/AtlHistFactorySystPair.h
    inc/AtlHistFactorySystPdf.h
    inc/AtlHistFactorySystematic.h
    inc/AtlHistFactoryTask.h
    inc/AtlHistFactoryYieldTableTask.h
    inc/AtlHistogramTool.h
    inc/AtlMemTkAnalysisTask.h
    inc/AtlMergingTask.h
    inc/AtlObjRecoScaleFactorTool.h
    inc/AtlObjectsDefinitionTool.h
    inc/AtlObjectsDefinitionToolR2.h
    inc/AtlObjectsToolD3PDSgTop.h
    inc/AtlPlotterFolder.h
    inc/AtlQCDMMTool.h
    inc/AtlRunEvtSelectionTool.h
    inc/AtlSample.h
    inc/AtlSelector.h
    inc/AtlSubselectionCuts.h
    inc/AtlSystematicsTool.h
    inc/AtlTask.h
    inc/AtlToolCut.h
    inc/AtlTopLevelAnalysis.h
    inc/AtlTopPairDocumenter.h
    inc/AtlTriggerTool.h
    inc/AtlTriggerToolItem.h
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
target_link_libraries(${lib} PUBLIC CalibrationDataInterface)
target_link_libraries(${lib} PUBLIC TopFakes)

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
