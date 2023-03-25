# set up environment for A++
#
# Author:
#    Sebastian Mergelmeyer <mergelm@physik.hu-berlin.de>
#
# Instructions:
#    Sourcing setup.sh will set up your environment for A++. A comma-separated list of options is
#    accepted in the first argument. Currently, "norc" (do not set up RootCore) and "noapp" (do not
#    set up A++ libraries) are supported. The file ~/bin/setup-root.sh is automatically executed if
#    it exists. This mechanism can be used to set up ROOT and the C compiler.
#
# Known issues:
#    RootCore is set up only if it's located in the subdirectory 'external' (otherwise you'll have
#    to set it up by hand, or use a symbolic link).
#    Does not warn if a library isn't found (because it cannot know whether the library is needed).
#    Requires bash or zsh.
#
hostname
echo "A++ Setup"
#source ~/bin/setup-root.sh
# automatically set up ROOT
if [[ -z ${ROOTSYS:-} && -e ~/bin/setup-root.sh ]] ; then
   setupAPlusPlusArgs=( "$@" )
   set --
   . ~/bin/setup-root.sh
   set -- "${setupAPlusPlusArgs[@]}"
fi


setupAPlusPlus() {

   # declare local variables
   local EnvVarName Options Root Source x y
   local -a LibPaths IncludePaths RootLibs

   if [[ -n ${BASH_VERSION:-} ]] ; then
      Source=${BASH_SOURCE[0]}
   elif [[ -n ${ZSH_VERSION:-} ]] ; then
      Source=${(%):-%x}
   else
      unset Source
   fi
   [[ -n ${Source} ]] || { printf "ERROR: could not find myself.\n" >&2 ; return 1 ; }
   Options=,$( tr '[A-Z]' '[a-z]' <<<"${1:-}" ),

   # check for ROOT
   which root >/dev/null || { printf "ERROR: no ROOT installation found.\n" >&2 ; return 1 ; }

   # derive architecture
   x=$(
         set -e
         Processor=$( uname -p )
         OperatingSystem=$( uname -s )
         GccVersion=$( gcc -dumpversion )
         RootVersion=$( root-config --version )
         printf "%s-%s-%s-%s" "${Processor}" "${OperatingSystem}" "gcc${GccVersion}" "root${RootVersion//\//.}"
      )
   [[ -n ${x} ]] || { printf "ERROR: could not detect ARCH_TYPE.\n" >&2 ; return 1 ; }
   [[ -z ${ARCH_TYPE:-} ]] || [[ ${ARCH_TYPE} = "${x}" ]] || { printf "ERROR: ARCH_TYPE conflict (%q in environment vs. %q from current setup).\n" "${ARCH_TYPE}" "${x}" >&2 ; return 1 ; }
   export ARCH_TYPE="${x}"

   # locate A++
   if [[ -n ${APP_HOME:-} ]] ; then
      Root=${APP_HOME}
   else
      Root=$( cd -P "$( dirname "${Source}" )" && pwd ) || :
   fi
   x=$( basename "${Source}" )
   export APLUSPLUS_SETUP="${Root}/${x}"

   if [[ ${Options} != *,norc,* ]] ; then

      # set up external libraries (RootCore)
      if [[ -z ${ROOTCOREBIN:-} ]] ; then
         x=${Root}/external/RootCore
         y=$( cd -P "${x}" && pwd )
         if [[ -n ${y} ]] ; then
            export ROOTCOREBIN="${y}"
            IncludePaths+=( "${ROOTCOREBIN}/include" )
            LibPaths+=( "${ROOTCOREBIN}/lib/${ARCH_TYPE}" )
         fi
      fi

   fi

   if [[ ${Options} != *,noapp,* ]] ; then

      export APP_HOME="${Root}"

      # set deprecated environment variables for external libraries to dummy values
      export CDI=/var/empty
      export LHAPDF6=/var/empty
      export PDFTOOL=/var/empty

      # set up A++ libraries
      for x in libHepEvent libHepUtils libAtlasRun libAtlasEvent libAtlasAnalysis libKinFitter libAtlasFinders libSingleTop ; do
         if [[ -e ${Root}/${x} ]] ; then
            y=$( cd -P "${Root}/${x}" && pwd )
            [[ -n ${y} ]] || continue
            EnvVarName=$( tr '[a-z]' '[A-Z]' <<<"${x}" )
            export "${EnvVarName}"="${y}"
            IncludePaths+=( "${y}/inc" )
            LibPaths+=( "${y}/lib/${ARCH_TYPE}" )
            RootLibs+=( "${x}" )
         fi
      done

   fi

   # set PATH-like variables
   for y in "${LibPaths[@]}" ; do
      export LD_LIBRARY_PATH="${y}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
   done
   for y in "${RootLibs[@]}" ; do
      export ROOT_LIBS="${ROOT_LIBS:+${ROOT_LIBS}:}${y}"
   done
   for y in "${IncludePaths[@]}" ; do
      export ROOT_INCLUDE_PATH="${ROOT_INCLUDE_PATH:+${ROOT_INCLUDE_PATH}:}${y}"
   done
}
   

setupAPlusPlus "$@"
