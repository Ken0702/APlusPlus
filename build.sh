#!/bin/bash
#
# Build script for A++, based on CMake.
#
# Usage:
# - Make sure that setup.sh works as intended, and that $USER_CACHE_DIR points
#   to your preferred cache directory.
# - Run ./RootCore-build.sh if not done yet, or after upgrading ROOT.
#   (May need to specify -c if RootCore is already installed but packages are missing.)
# - Check whether you need to add libraries/headers to configure().
# - Run ./build.sh. Options:
#   -c force to run cmake
#   -g enable debug flags for compilation
#   -v verbose make
#   -jN force make to use N cores

configure() {

   add_header_dir libKinFitter KinFitter .

   add_rootcore_dep libAtlasAnalysis CalibrationDataInterface TopFakes
   add_rootcore_dep libSingleTop PdfReweightingTool

   project libHepEvent
   project libHepUtils       libHepEvent
   project libAtlasRun       libHepEvent
   project libAtlasEvent     libHepEvent libAtlasRun
   project libAtlasAnalysis  libHepEvent libHepUtils libAtlasRun libAtlasEvent
   project libKinFitter
   project libAtlasFinders   libHepEvent libHepUtils libAtlasRun libAtlasEvent libAtlasAnalysis libKinFitter
   project libSingleTop      libHepEvent libHepUtils libAtlasRun libAtlasEvent libAtlasAnalysis libKinFitter libAtlasFinders

   superProject APlusPlus

}


# ============================================================ #

set -e
set -o pipefail
shopt -s nullglob
export LC_ALL=C
UMASK=$( umask )
umask 077
exec 9>&1

# ============================================================ #

HEADER_DIRS=( )
ROOTCORE_DEPS=( )
PROJECTS=( )

_find_headers() {
   # _find_headers <Mode> <Module>
   # Locate header directories for the given Module. If no directories have been defined using
   # add_header_dir, use a suitable default (subdirectory 'inc').
   # Mode=ALL : return all header directories
   # Mode=PRIMARY : restrict the output to the first header (used to build ROOT dictionaries)
   local Found HeaderDir Mode ModuleName
   Mode="${1}"
   ModuleName="${2}"
   Found=0
   for x in "${HEADER_DIRS[@]}" ; do
      if [[ ${x} = ${ModuleName}:* ]] ; then
         HeaderDir="${x#${ModuleName}:}"
         if [[ ${HeaderDir} = . ]] ; then
            printf "%s\n" "${ModuleName}"
         else
            printf "%s/%s\n" "${ModuleName}" "${HeaderDir}"
         fi
         [[ ${Mode} = ALL ]] || return
         Found=1
      fi
   done
   [[ ${Found} -gt 0 ]] ||
   printf "%s/inc\n" "${ModuleName}"
}

# ============================================================ #

add_header_dir() {
   # add_header_dir <Module> <Subdirectory>...
   #
   # Define which Subdirectories contain header files for the given Module.
   # The first Subdirectory will also be considered for the dictionary generation.
   # If no Subdirectories are defined for a Module, its list defaults to: ( inc )
   local Module x
   Module=${1}
   shift
   for x ; do
      HEADER_DIRS+=( "${Module}:${x}" )
   done
}

add_rootcore_dep() {
   # add_rc_dep <Module> <RootCore package>...
   #
   # Declare a dependency of <Module> on one or more RootCore packages.
   local Module x
   Module=${1}
   shift
   for x ; do
      ROOTCORE_DEPS+=( "${Module}:${x}" )
   done
}

project() {
   # project <Module> [<Dependencies>...]
   # Write the CMakeLists.txt file for Module.
   local ModulePath="${1}"
   local ModuleName="${1##*/}"
   shift 1
   local m x

   exec >"${TmpFile}"
   echo 'cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)'
   echo "project(${ModuleName} CXX)"
   echo 'include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")'
   echo "set(lib ${ModuleName#lib})"
   echo ''

   local RootCorePackages=( )
   local RootCoreDep
   for m in "$@" "${ModuleName}" ; do
      for RootCoreDep in "${ROOTCORE_DEPS[@]}" ; do
         [[ ${RootCoreDep} = ${m}:* ]] || continue
         RootCorePackages+=( "${RootCoreDep#*:}" )
      done
   done
   if [[ ${#RootCorePackages} -gt 0 ]] ; then
      echo "find_package(RootCore REQUIRED)"
   fi

   for m in "$@" "${ModuleName}" ; do
      _find_headers ALL "${m}" |
      while read -r x ; do
         echo 'include_directories("${CMAKE_SOURCE_DIR}/'"${x}"'")'
      done
   done
   echo
   echo 'set(SOURCES'
   ( set -e ; cd "${ModuleName}/src" && for x in *.cxx ; do echo "    src/${x}" ; done )
   echo '   )'
   echo ''
   echo 'set(HEADERS'
   ( set -e ; cd "$( _find_headers PRIMARY "${ModuleName}" )" && for x in *.h ; do echo "    ${PWD##*/}/${x}" ; done )
   echo '   )'
   echo
   echo 'foreach(h ${HEADERS})'
   echo '    add_root_dictionary(${h})'
   echo 'endforeach()'
   echo ''
   echo 'set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")'
   echo 'add_library(${lib} SHARED ${SOURCES} ${ROOT_DICTS})'
   echo 'target_link_libraries(${lib} PUBLIC ${ROOT_LIBRARIES})'
   for m in "$@" ; do
      echo 'target_link_libraries(${lib} PUBLIC '"${m#lib}"')'
   done
   printf "%s\n" "${RootCorePackages[@]}" |
      sort -u |
      while read -r x ; do
         [[ ${x} != '' ]] || continue
         echo 'target_link_libraries(${lib} PUBLIC '"${x#lib}"')'
      done
   echo
   echo '#--- all symbols shall resolve at compile time ---'
   echo 'target_link_libraries(${lib} PRIVATE -Wl,-z,defs)'
   exec >&9
   cmp -s "${TmpFile}" "${ModulePath}"/CMakeLists.txt ||
   mv "${TmpFile}" "${ModulePath}"/CMakeLists.txt
   PROJECTS+=( "${ModulePath}" )
}

superProject() {
   exec >"${TmpFile}"
   echo "cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)"
   echo "project(${1} CXX)"
   echo 'if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")'
   echo '  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-switch")'
   echo 'endif()'
   if [[ ${debug} -gt 0 ]] ; then
      for x in CMAKE_CXX_FLAGS CMAKE_SHARED_LINKER_FLAGS ; do
         printf 'set(%s "${%s} -g")\n' "${x}" "${x}"
#	 printf 'set(%s "${%s} -fsanitize=undefined,address -Og")\n' "${x}" "${x}"
	 printf 'set(%s "${%s} -Wextra -Wno-unused-parameter")\n' "${x}" "${x}"
      done
      echo 'set(CMAKE_EXPORT_COMPILE_COMMANDS ON)'
   else
       # use optimization
      for x in CMAKE_CXX_FLAGS CMAKE_SHARED_LINKER_FLAGS ; do
         printf 'set(%s "${%s} -O3")\n' "${x}" "${x}"
      done
   fi

   # note: should we use optimization if we don't use debug flags?
   # note: -fsanitize=address conflicts with valgrind

   for proj in "${PROJECTS[@]}" ; do
      echo "add_subdirectory($proj)"
   done
   exec >&9
   cmp -s "${TmpFile}" ./CMakeLists.txt ||
   cp --preserve=times "${TmpFile}" ./CMakeLists.txt
}

# ============================================================ #

# process command-line options
force_cmake_init=0
force_nproc=0
debug=0
make_verbose=0
while getopts cgvj: arg ; do
   case ${arg} in
   c) force_cmake_init=1 ;;
   g) debug=1 ;;
   j) force_nproc=${OPTARG} ;;
   v) make_verbose=1 ;;
   esac
done
shift $(( OPTIND - 1 ))
for arg ; do
   [[ ${arg} != -?* ]] || { printf 'ERROR: options must precede any non-option arguments.\n' >&2 ; exit 1 ; }
done

# set up environment
. setup.sh

PROJECTS=( )
[[ ${USER_CACHE_DIR} = /* ]] || { printf 'ERROR: environment variable USER_CACHE_DIR not (properly) set.\n' >&2 ; exit 1 ; }
SourceDir=$( pwd -P )
BuildDir=${USER_CACHE_DIR}/build/${SourceDir#/}
if [[ ${debug} -gt 0 ]] ; then
   BuildDir=${USER_CACHE_DIR}/build-debug/${SourceDir#/}
fi

# configure project
mkdir -p "${BuildDir}"
do_cmake_init=${force_cmake_init}
[[ -e ${BuildDir}/.cmake-init-done ]] || do_cmake_init=1
TmpFile=${BuildDir}/CMakeLists.tmp
echo -n "Looking for source files and directories ... " >&9
configure
echo "done." >&9
exec >&9

# maybe run cmake
cd "${BuildDir}"
if [[ ${do_cmake_init} -gt 0 ]] ; then
   cmake "${SourceDir}"
   touch .cmake-init-done
fi

# run make
if [[ ${force_nproc} -gt 0 ]] ; then
   nproc=$(( 0 + force_nproc ))
else
   nproc=$( nproc )
   nproc=$(( nproc * 3 / 4 ))
fi
make_opt=( )
[[ ${make_verbose} -gt 0 ]] &&
   make_opt+=( "VERBOSE=1" )
[[ ${nproc} -lt 2 ]] ||
   make_opt+=( "-j${nproc}" )
make "${make_opt[@]}" all </dev/null
echo "Build complete."

# copy libraries to the usual locations
umask "${UMASK}"
echo "Installing/updating shared libraries ..."
for x in "${PROJECTS[@]}" ; do
   mkdir -p "${SourceDir}/${x}/lib"
   rsync -rti --delete --omit-dir-times \
      --include="*.so" \
      --include="*.pcm" \
      --exclude="*" \
      "${x}/dict/" "${x}/lib/" "${SourceDir}/${x}/lib/${ARCH_TYPE}/"
done
echo "Installation finished."

# copy compile_commands.json
if [[ ${debug} -gt 0 ]] ; then
   rsync -ti "${BuildDir%/}/compile_commands.json" \
      "${SourceDir%/}/compile_commands_orig.json"
   sed -e "s#${BuildDir%/}/build-debug##" \
      "${SourceDir%/}/compile_commands_orig.json" >| \
      "${SourceDir%/}/compile_commands.json"
fi
