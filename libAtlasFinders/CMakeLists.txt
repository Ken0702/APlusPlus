cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libAtlasFinders CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib AtlasFinders)

find_package(RootCore REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libHepUtils/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasRun/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasAnalysis/inc")
include_directories("${CMAKE_SOURCE_DIR}/libKinFitter/KinFitter")
include_directories("${CMAKE_SOURCE_DIR}/libKinFitter")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasFinders/inc")

set(SOURCES
    src/AtlConvFinder.cxx
    src/AtlD0Finder.cxx
    src/AtlDstarFinder.cxx
    src/AtlK0StarFinder.cxx
    src/AtlK0sFinder.cxx
    src/AtlKaonHistos.cxx
    src/AtlKinFitterTool.cxx
    src/AtlLambdaFinder.cxx
    src/AtlPhiFinder.cxx
    src/AtlSgTop_WtChannelFinder.cxx
    src/AtlSgTop_tChannelFinder.cxx
    src/AtlTopPairFinder.cxx
    src/AtlZ0Finder.cxx
   )

set(HEADERS
    inc/AtlConvFinder.h
    inc/AtlD0Finder.h
    inc/AtlDstarFinder.h
    inc/AtlK0StarFinder.h
    inc/AtlK0sFinder.h
    inc/AtlKaonHistos.h
    inc/AtlKinFitterTool.h
    inc/AtlLambdaFinder.h
    inc/AtlPhiFinder.h
    inc/AtlSgTop_WtChannelFinder.h
    inc/AtlSgTop_tChannelFinder.h
    inc/AtlTopPairFinder.h
    inc/AtlZ0Finder.h
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
target_link_libraries(${lib} PUBLIC CalibrationDataInterface)
target_link_libraries(${lib} PUBLIC TopFakes)

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
