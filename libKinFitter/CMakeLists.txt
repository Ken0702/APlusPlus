cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(libKinFitter CXX)
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set(lib KinFitter)

include_directories("${CMAKE_SOURCE_DIR}/libKinFitter/KinFitter")
include_directories("${CMAKE_SOURCE_DIR}/libKinFitter")

set(SOURCES
    src/TAbsFitConstraint.cxx
    src/TAbsFitParticle.cxx
    src/TFitConstraintEp.cxx
    src/TFitConstraintEpGaus.cxx
    src/TFitConstraintEta.cxx
    src/TFitConstraintM.cxx
    src/TFitConstraintMBW.cxx
    src/TFitConstraintMBW2.cxx
    src/TFitConstraintMGaus.cxx
    src/TFitConstraintPt.cxx
    src/TFitParticlePInvThetaPhi.cxx
    src/TFitParticlePInvThetaPhiE.cxx
    src/TFitParticlePInvThetaPhiM.cxx
    src/TFitParticlePThetaPhi.cxx
    src/TFitParticlePThetaPhiE.cxx
    src/TFitParticlePThetaPhiM.cxx
    src/TFitParticlePtEtaPhi.cxx
    src/TFitParticlePtEtaPhiE.cxx
    src/TFitParticlePtEtaPhiM.cxx
    src/TFitParticlePtThetaPhi.cxx
    src/TFitParticlePxPyPz.cxx
    src/TFitParticlePxPyPzE.cxx
    src/TFitParticlePxPyPzM.cxx
    src/TFitParticleRelE.cxx
    src/TFitParticleRelPtEtaPhi.cxx
    src/TFitParticleRelPtEtaPhiE.cxx
    src/TFitParticleRelPtEtaPhiM.cxx
    src/TFitParticleRelPxPyPz.cxx
    src/TFitParticleRelPxPyPzE.cxx
    src/TFitParticleRelPxPyPzM.cxx
    src/TGetErrorMatrix.cxx
    src/TKinFitter.cxx
    src/TToyGentSgTop.cxx
    src/TToyGentT.cxx
   )

set(HEADERS
    KinFitter/TAbsFitConstraint.h
    KinFitter/TAbsFitParticle.h
    KinFitter/TFitConstraintEp.h
    KinFitter/TFitConstraintEpGaus.h
    KinFitter/TFitConstraintEta.h
    KinFitter/TFitConstraintM.h
    KinFitter/TFitConstraintMBW.h
    KinFitter/TFitConstraintMBW2.h
    KinFitter/TFitConstraintMGaus.h
    KinFitter/TFitConstraintPt.h
    KinFitter/TFitParticlePInvThetaPhi.h
    KinFitter/TFitParticlePInvThetaPhiE.h
    KinFitter/TFitParticlePInvThetaPhiM.h
    KinFitter/TFitParticlePThetaPhi.h
    KinFitter/TFitParticlePThetaPhiE.h
    KinFitter/TFitParticlePThetaPhiM.h
    KinFitter/TFitParticlePtEtaPhi.h
    KinFitter/TFitParticlePtEtaPhiE.h
    KinFitter/TFitParticlePtEtaPhiM.h
    KinFitter/TFitParticlePtThetaPhi.h
    KinFitter/TFitParticlePxPyPz.h
    KinFitter/TFitParticlePxPyPzE.h
    KinFitter/TFitParticlePxPyPzM.h
    KinFitter/TFitParticleRelE.h
    KinFitter/TFitParticleRelPtEtaPhi.h
    KinFitter/TFitParticleRelPtEtaPhiE.h
    KinFitter/TFitParticleRelPtEtaPhiM.h
    KinFitter/TFitParticleRelPxPyPz.h
    KinFitter/TFitParticleRelPxPyPzE.h
    KinFitter/TFitParticleRelPxPyPzM.h
    KinFitter/TGetErrorMatrix.h
    KinFitter/TKinFitter.h
    KinFitter/TToyGentSgTop.h
    KinFitter/TToyGentT.h
   )

foreach(h ${HEADERS})
    add_root_dictionary(${h})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})
target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})

#--- all symbols shall resolve at compile time ---
target_link_libraries(${lib} PRIVATE -Wl,-z,defs)
