cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libTopFakes CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib TopFakes)

include_directories("${CMAKE_SOURCE_DIR}/libTopFakes/TopFakes")

set(SOURCES
   )

set(HEADERS
    TopFakes/FakesWeights.h
    TopFakes/MMEffSet.h
    TopFakes/MMEfficiency.h
    TopFakes/MatrixUtils.h
    TopFakes/TopFakesMMWeightCalculator.h
    TopFakes/TopFakesxAODUtils.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})

#--- all symbols must resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
