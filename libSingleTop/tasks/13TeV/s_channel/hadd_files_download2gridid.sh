#!/bin/zsh
# shell script to hadd several files from rucio/download into the known file structure for copy_files_gridid2version.sh

# 

prefix="ntup_schan_"
prefix_fixed=$prefix
njets="2j"
channel="lnu"
suffix=".root"

mc="a"
#mc="d"

# #version="v13_v23lj_defaultWithBDT_MC16a"
# #version="v13_v23lj_defaultWithBDT_MC16d"
# #version="v14_v23lj_default_MC16a"
# #version="v14_v23lj_default_MC16d"
# #version="v14_v23lj_default_testnewTF_MC16a"
# #version="v14_v23lj_default_testnewTF_MC16d"
# #version="v15_v25lj_default_MC16a"
# #version="v15_v25lj_default_MC16d"
# version="v15_v25lj_default_MC16${mc}"
# #version="v15_v25lj_default_samplesyst_MC16${mc}"
# #gridID1="v13_SR"
# #gridID1="v13_SRd"
# #gridID1="v14_SRa"
# # gridID1="v14_SRd"
# #gridID1="v25_a"
# #gridID1="v25_d"
# gridID1="v25_${mc}"
# gridID2="v3"
# #gridID2="v2_v1"

# filter="${gridID1}.${gridID2}"
# #filter="410644*.*${gridID1}.${gridID2}"

# #gridID2="v0"

# indir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/rucio/download/"
# ruciodir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/rucio/${gridID1}.${gridID2}"
# # outdir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/MemTkAnalysis"

gridID1="v29_a_nominal"
gridID2="v0_nominal"
version="v16_v29lj_default_v29star_MC16${mc}"
indir="/rdsk/datm8/atlas/kaphle/13TeV_s_channel/${version}/rucio/debug/"
ruciodir="/rdsk/datm8/atlas/kaphle/13TeV_s_channel/${version}/rucio/test"
filter=".*"

memdir=$ruciodir/mem/
appdir=$ruciodir/app/

mkdir -p $ruciodir
mkdir -p $memdir
mkdir -p $appdir

syst_short_list_file='/mnt/eeh/kaphle/atlas/APlusPlus_git/libSingleTop/tasks/13TeV/s_channel/systematics_v25.txt'

# logfile
logfile=$ruciodir/rucio_hadd_`date +%FT%T`.log
touch $logfile

nadded=0

#
do_add=0
do_overwrite=0
do_app=0
completed_only=0
completed_samples_only=0
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_a/v1/downloaded_systs_mc16a.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_d/v1/downloaded_systs_mc16d.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/v1/downloaded_systs_mc16${mc}.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/${gridID2}/systematics.txt"
completed_samples_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/v2/completed_samples.txt"
# temporary hack to include EG_SCALE_ALL__1down

# completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_a/v1/dl_nominal_tmp.txt"

while getopts aop arg ; do
    case ${arg} in
	a) do_add=1 ;;
	o) do_overwrite=1 ;;
	p) do_app=1 ;;
    esac
done
shift $(( OPTIND - 1 ))

# loop
#for dir in ${indir%*/}/*nominal*/
for dir in ${indir%*/}/*/
do
    dir=${dir%*/}

    # filter source first
    if [[ ! $dir =~ $filter ]] ; then
	continue
    fi

    # check if mem or app, just copy mem
    output_type=${dir##*$gridID1.$gridID2\_}

    usedir=$ruciodir
    if [[ $output_type == "mem_output.root" ]] ; then
	usedir=$memdir
	prefix="mem_${prefix_fixed}"
    elif [[ $output_type == "app_output.root" ]] ; then
	usedir=$appdir
	prefix="app_${prefix_fixed}"
	if [[ ${do_app} -eq 0 ]]; then
	    continue # for now don't copy cut-based analysis results
	fi
    else
	echo "Directory $dir is not a mem output" | tee -a $logfile
	continue
    fi

    # remove full path
    dsname=$(basename $dir)
    did=${dsname%_${output_type}}

    # get syst and sample from path
    descr=${dsname[(ws:.:)4]}
    descr=${descr%_${gridID1}*}
    sample=${descr[(ws:_:)1]}
    #syst=${descr[(ws:_:)2]}
    syst_short=${descr#*_}

    # get long form of syst
    syst=$(grep $syst_short $syst_short_list_file | awk '{print $1}')

    # only use completed systematics?
    if [[ ${completed_only} -gt 0 ]]; then
	syst_found=$(grep $syst $completed_file)
	if [[ -z $syst_found ]]; then
	    echo "systematic ${syst} was not found to be completed; skip"
	    continue
	fi
    fi
    # only use completed samples?
    if [[ ${completed_samples_only} -gt 0 ]]; then
	sample_found=$(grep $did $completed_samples_file)
	if [[ -z $sample_found ]]; then
	    echo "sample ${did} was not found to be completed; skip"
	    continue
	fi
    fi

    # target file
    target=$usedir/${prefix}${njets}_${channel}_${syst}_${sample}.root

    # check if target file exists
    # - default: skip existing
    # - overwrite: remove existing first
    # - add: execute hadd cmd as normal
    if [ -e $target ]; then
	if [[ ${do_overwrite} -gt 0 ]]; then
	    rm_cmd="rm -r $target"
	    echo "Target $target exists; removing first..." | tee -a $logfile
	    eval $rm_cmd
	elif [[ ${do_add} -eq 0 ]]; then
	    echo "Target $target exists; skipping. For overwrite, use -o option, for adding use -a." | tee -a $logfile
	    continue
	fi
    fi

    # hadd command
    # - caveat with xargs: if command size limit is reached, xargs splits command
    #   which causes the output file to be overwritten with the last split

    # hadd_cmd="ls $dir/* | xargs hadd -f9 $target"
    hadd_cmd="hadd -f9 $target $dir/*"
    # echo $hadd_cmd | tee -a $logfile
    eval $hadd_cmd | tee -a $logfile
    nadded=$((nadded+1))

    # hadd_cmd="hadd -f9 $target $dir/*"
    # hadd_cmd="echo \"hadd -f9 $target $dir/*\" | xargs zsh -c"
    #echo $hadd_cmd
    # echo $hadd_cmd | xargs zsh -c

    # break
done

echo "hadd'ed $nadded files."  | tee -a $logfile
