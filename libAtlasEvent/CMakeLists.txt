cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libAtlasEvent CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib AtlasEvent)

include_directories("${CMAKE_SOURCE_DIR}/libHepEvent/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasRun/inc")
include_directories("${CMAKE_SOURCE_DIR}/libAtlasEvent/inc")

set(SOURCES
    src/AtlBTag.cxx
    src/AtlD0DecayKPi.cxx
    src/AtlDstarDecayDPi.cxx
    src/AtlEMShower.cxx
    src/AtlElectron.cxx
    src/AtlEnergySum.cxx
    src/AtlEvent.cxx
    src/AtlEventHeader.cxx
    src/AtlFastElectron.cxx
    src/AtlFastJet.cxx
    src/AtlFastMuon.cxx
    src/AtlFastPhoton.cxx
    src/AtlFastTau.cxx
    src/AtlIDTrack.cxx
    src/AtlJet.cxx
    src/AtlK0sDecayPiPi.cxx
    src/AtlLambdaDecayPPi.cxx
    src/AtlMCJet.cxx
    src/AtlMCTruthClassifier.cxx
    src/AtlMETWeights.cxx
    src/AtlMuon.cxx
    src/AtlMuonSegment.cxx
    src/AtlPhiDecayKK.cxx
    src/AtlPhoton.cxx
    src/AtlPhotonConv.cxx
    src/AtlPixelHit.cxx
    src/AtlSCT3DHit.cxx
    src/AtlScaleFactor.cxx
    src/AtlSilicon3DHit.cxx
    src/AtlTRTDigit.cxx
    src/AtlTau.cxx
    src/AtlTopPair.cxx
    src/AtlTrigger.cxx
    src/AtlTriggerItem.cxx
    src/AtlTriggerMatch.cxx
    src/AtlWDecayJJ.cxx
    src/AtlWDecayLNu.cxx
    src/AtlZ0DecayEE.cxx
    src/AtlZ0DecayJJ.cxx
    src/AtlZ0DecayMuMu.cxx
    src/AtlZ0DecayTauTau.cxx
   )

set(HEADERS
    inc/AtlBTag.h
    inc/AtlD0DecayKPi.h
    inc/AtlDstarDecayDPi.h
    inc/AtlEMShower.h
    inc/AtlElectron.h
    inc/AtlEnergySum.h
    inc/AtlEvent.h
    inc/AtlEventHeader.h
    inc/AtlFastElectron.h
    inc/AtlFastJet.h
    inc/AtlFastMuon.h
    inc/AtlFastPhoton.h
    inc/AtlFastTau.h
    inc/AtlIDTrack.h
    inc/AtlJet.h
    inc/AtlK0sDecayPiPi.h
    inc/AtlLambdaDecayPPi.h
    inc/AtlMCJet.h
    inc/AtlMCTruthClassifier.h
    inc/AtlMETWeights.h
    inc/AtlMuon.h
    inc/AtlMuonSegment.h
    inc/AtlPhiDecayKK.h
    inc/AtlPhoton.h
    inc/AtlPhotonConv.h
    inc/AtlPixelHit.h
    inc/AtlSCT3DHit.h
    inc/AtlScaleFactor.h
    inc/AtlSilicon3DHit.h
    inc/AtlTRTDigit.h
    inc/AtlTau.h
    inc/AtlTopPair.h
    inc/AtlTrigger.h
    inc/AtlTriggerItem.h
    inc/AtlTriggerMatch.h
    inc/AtlWDecayJJ.h
    inc/AtlWDecayLNu.h
    inc/AtlZ0DecayEE.h
    inc/AtlZ0DecayJJ.h
    inc/AtlZ0DecayMuMu.h
    inc/AtlZ0DecayTauTau.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})
target_link_libraries(${lib} PUBLIC HepEvent)
target_link_libraries(${lib} PUBLIC AtlasRun)

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
