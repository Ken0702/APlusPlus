#!/bin/bash
# shell script to download a specific MEM production from rucio
# run with "source download_MEM_2gridid.sh"
#
# currently I can't get it to run properly, so it only outputs the commands that can be copypasted into the CL
echo ""; echo "Program currently just prints out the commands. Copy and Paste yourself"; echo ""

# download directory
machine="dats1"
version="v02_v32lj_MemTKOnGrid_MC16a"
#gridID1="v14_SRa"
gridID1="v32lj_MC16a"
gridID2="v0"
CampaignName="FirstRun"

ruciodir="/rdsk/${machine}/atlas/kkreul/combination/A++/${version}/rucio/${gridID1}.${gridID2}"

do_ls=0
do_dl=0
while getopts dl arg ; do
    case ${arg} in
	d) do_dl=1 ;;
	l) do_ls=1 ;;
    esac
done
shift $(( OPTIND - 1 ))

# sample specifier
regex="'user.kkreul.*${CampaignName}*MemTk.*_${gridID1}.${gridID2}_mem_*.root.*'"

if [[ ${do_ls} -gt 0 ]] ; then
    ls_cmd="rucio ls $regex"
    echo $ls_cmd
    #$ls_cmd
elif [[ ${do_dl} -gt 0 ]] ; then
    # create if it does not exist
    echo "Downloading in path $ruciodir"
    mkdir -p $ruciodir
    cd_cmd="cd $ruciodir"
    echo $cd_cmd
    #$cd_cmd
    dl_cmd="rucio get $regex"
    echo $dl_cmd
    #$dl_cmd    
else
    echo "Regex specifier for rucio is \"$regex\""
    echo "  rucio account: $RUCIO_ACCOUNT"
    echo "  use -l for \"rucio ls $regex\""
    echo "  use -d for \"rucio get $regex\","
    echo "    will download in $ruciodir"
fi

