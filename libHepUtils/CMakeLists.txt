cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libHepUtils CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib HepUtils)

include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libHepUtils/inc")

set(SOURCES
    src/HepCrossSectionTask.cxx
    src/HepDataMCFolder.cxx
    src/HepDataMCPlot.cxx
    src/HepDataMCPlotter.cxx
    src/HepDataMCSample.cxx
    src/HepFractionFitterTask.cxx
    src/HepNtuplePlotCmd.cxx
    src/HepTemplate.cxx
    src/HepTemplateFitter.cxx
   )

set(HEADERS
    inc/HepCrossSectionTask.h
    inc/HepDataMCFolder.h
    inc/HepDataMCPlot.h
    inc/HepDataMCPlotter.h
    inc/HepDataMCSample.h
    inc/HepFractionFitterTask.h
    inc/HepNtuplePlotCmd.h
    inc/HepTemplate.h
    inc/HepTemplateFitter.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})
target_link_libraries(${lib} PUBLIC HepEvent)

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
