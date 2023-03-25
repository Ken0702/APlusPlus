cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libHepEvent CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib HepEvent)

include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")

set(SOURCES
    src/Hep2BodyDecay.cxx
    src/Hep3DHit.cxx
    src/HepDatabasePDG.cxx
    src/HepDecayParticle.cxx
    src/HepDigit.cxx
    src/HepDriftChamberDigit.cxx
    src/HepElectron.cxx
    src/HepEnergySum.cxx
    src/HepEvent.cxx
    src/HepJet.cxx
    src/HepK0sDecay.cxx
    src/HepMCParticle.cxx
    src/HepMCVertex.cxx
    src/HepMagneticField.cxx
    src/HepMuon.cxx
    src/HepParticle.cxx
    src/HepPhoton.cxx
    src/HepTau.cxx
    src/HepTopDecay.cxx
    src/HepTrackHelix.cxx
    src/HepVertex.cxx
    src/HepVtxTrackHelix.cxx
    src/HepWDecay.cxx
    src/HepZ0Decay.cxx
   )

set(HEADERS
    inc/Hep2BodyDecay.h
    inc/Hep3DHit.h
    inc/HepDatabasePDG.h
    inc/HepDecayParticle.h
    inc/HepDigit.h
    inc/HepDriftChamberDigit.h
    inc/HepElectron.h
    inc/HepEnergySum.h
    inc/HepEvent.h
    inc/HepJet.h
    inc/HepK0sDecay.h
    inc/HepMCParticle.h
    inc/HepMCVertex.h
    inc/HepMagneticField.h
    inc/HepMuon.h
    inc/HepParticle.h
    inc/HepPhoton.h
    inc/HepTau.h
    inc/HepTopDecay.h
    inc/HepTrackHelix.h
    inc/HepVertex.h
    inc/HepVtxTrackHelix.h
    inc/HepWDecay.h
    inc/HepZ0Decay.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
