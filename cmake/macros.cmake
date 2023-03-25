# CMake macro collection for A++
#
# Author:
#    Sebastian Mergelmeyer <mergelm@physik.hu-berlin.de>
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{ROOTSYS}/etc/cmake" "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)
if (DEFINED ROOT_USE_FILE)
   include(${ROOT_USE_FILE})
endif()
EXECUTE_PROCESS(COMMAND root-config --incdir OUTPUT_VARIABLE ROOT_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
add_definitions(${ROOT_CXX_FLAGS})

string(REPLACE ":" ";" xs "$ENV{LD_LIBRARY_PATH}")
foreach(x ${xs})
    link_directories(${x})
endforeach()

set(libs)
string(REGEX REPLACE "[\n\r\t ][\n\r\t ]*" ";" xs "${ROOT_LIBRARIES}")
foreach(x ${xs})
    if (x MATCHES "^-l(.*)")
       set(libs ${libs} ${CMAKE_MATCH_1})
    endif()
endforeach()
set(ROOT_LIBRARIES ${libs} Gui EG Eve TMVA TreePlayer RooFitCore RooStats HistFactory)

function(add_root_dictionary header)

    if (NOT DEFINED ROOT_DICTS)
       file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/dict")
       set(ROOT_DICTS)
    endif()

    get_filename_component(hdr ${header} NAME)
    get_filename_component(dict ${header} NAME_WE)
    set(dict dict/${dict}Dict)

    get_directory_property(incdirs INCLUDE_DIRECTORIES)
    set(includes)
    foreach(d ${incdirs})
        set(includes ${includes} -I${d})
    endforeach()

    add_custom_command(
        OUTPUT ${dict}.cxx ${dict.h}
        COMMAND $ENV{ROOTSYS}/bin/rootcint -cint -f ${dict}.cxx -c -p "-isystem${ROOT_INCLUDE_DIR}" ${includes} ${hdr}
        IMPLICIT_DEPENDS CXX ${header}
        VERBATIM
       )

    set(ROOT_DICTS ${ROOT_DICTS} ${dict} PARENT_SCOPE)

endfunction()
