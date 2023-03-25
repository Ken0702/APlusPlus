#!/bin/bash
#
# Check out & compile RootCore packages, as defined in 'packages.rc', into the
# 'external' subdirectory.
#
# Options:
#    -c  Check out packages.
#    -N  Do not check out RootCore itself if missing.
#    -u  Update packages.

set -e
set -o pipefail

# parse command-line options
do_checkout=0
do_update=0
no_checkout=0
while getopts cNu arg ; do
   case ${arg} in
   c) do_checkout=1 ;;
   N) no_checkout=1 ;;
   u) do_checkout=1 ; do_update=1 ;;
   esac
done
shift $(( OPTIND - 1 ))

packages_file=$( readlink -f packages.rc )

. setup.sh norc,noapp
[[ ${USER_CACHE_DIR} = /* ]] || { printf 'ERROR: environment variable USER_CACHE_DIR not (properly) set.\n' >&2 ; exit 1 ; }

if mkdir external 2>/dev/null ; then
   printf 'NOTICE: created directory %q\n' external >&2
fi
cd external
ROOTCORECONFIG=${ARCH_TYPE}
export ROOTCORECONFIG

# maybe check out RootCore
if ! [[ -e RootCore ]] ; then
   [[ ${no_checkout} -eq 0 ]] || { printf 'ERROR: RootCore not found, and not allowed to auto-install (option -C).\n' >&2 ; exit 1 ; }
   do_checkout=1
   RootCoreTags=( $( sed -ne '/RootCore\(\/tags\|-[^\/]*\)$/p' "${packages_file}" ) )
   [[ ${#RootCoreTags[@]} -eq 1 ]] || { printf 'ERROR: unable to find RootCore in packages.rc.\n' >&2 ; exit 1 ; }
   RootCoreTag=${RootCoreTags[0]}
   RootCoreTag=${RootCoreTag/#'atlas'/'svn+ssh://svn.cern.ch/reps/atlas'}
   [[ ${RootCoreTag} = svn*://* ]] || { printf 'ERROR: RootCore in packages.rc does not look like a valid SVN path.\n' >&2 ; exit 1 ; }
   printf 'checking out %s\n' RootCore
   svn co "${RootCoreTag}" RootCore
fi

# tweak environment to help Asg_* packages find existing package locations
where_lib() {
   local IFS=:
   local x y
   for x in ${LD_LIBRARY_PATH:-} ; do
      for y in "${x}/lib${1}"{-*.so,_*.so} ; do
         if [[ ${y} != *\** ]] ; then
            printf '%s\n' "${x}"
            return
         fi
      done
   done
}
boost_lib_path=$( where_lib boost )
if [[ -n ${boost_lib_path} ]] ; then
    echo "boost_lib_path: ${boost_lib_path}"
   [[ -n ${BOOSTLIBDIR:-} ]] || export BOOSTLIBDIR="${boost_lib_path}"
   [[ -n ${BOOSTINCDIR:-} ]] || export BOOSTINCDIR="$( cd "${boost_lib_path}"/../include/boost* && pwd )"
fi

# set up RootCore
source RootCore/scripts/setup.sh
( cd "${USER_CACHE_DIR}" && mkdir -p build/RootCore/obj )
ROOTCOREOBJ=${USER_CACHE_DIR}/build/RootCore/obj
export ROOTCOREOBJ

# build RootCore packages
if [[ ${do_checkout} -gt 0 ]] ; then
   rc checkout "${packages_file}"
fi
if [[ ${do_update} -gt 0 ]] ; then
   rc update "${packages_file}"
fi
rc find_packages
rc compile

# retrieve libraries from USER_CACHE_DIR
# (RootCore gives us symlinks)
cd "${ROOTCOREDIR}/lib/${ROOTCORECONFIG}"
for x in ./* ; do
   [[ -h ${x} ]] || continue
   src=$( readlink -f "${x}" )
   [[ ${src} = ${ROOTCOREOBJ}/* ]] || continue
   tmp=$( mktemp "./.cp.XXXXXXXX" )
   cp --preserve=times,links "${src}" "${tmp}"
   mv -fT "${tmp}" "${x}"
done
