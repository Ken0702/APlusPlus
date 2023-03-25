cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libAtlasRun CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib AtlasRun)

include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasRun/inc")

set(SOURCES
    src/AtlEvtTreeInfo.cxx
    src/AtlTriggerConf.cxx
    src/AtlTriggerConfItem.cxx
   )

set(HEADERS
    inc/AtlEvtTreeInfo.h
    inc/AtlTriggerConf.h
    inc/AtlTriggerConfItem.h
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
