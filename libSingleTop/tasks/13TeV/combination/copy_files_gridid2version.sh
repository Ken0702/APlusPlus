#!/bin/zsh
# shell script to copy files from rucio download path into normal paths

# run this with ./copy_files_gridid2version.sh -s after using ./download_MEM_2gridid.sh
# or after using ~/atlas/download/v25lj_MEM/check_submissions.py -d and hadd_files_download2gridid.sh

# don't allow source, execute this script directly
# ZSH_EVAL_CONTEXT is 'toplevel' when executed, but 'toplevel:file' when sourced
if [[ $ZSH_EVAL_CONTEXT != 'toplevel' ]] ; then
    echo "Usage: ./copy_files_gridid2version.sh [-cfis]"
    return 1
fi

prefix="ntup_schan_"
suffix=".root"
add_JET=1

mc="a"
#mc="d"
#mc="e"

# v29 campaign
#gridID1="v29_a_nominal"
#gridID1="v29lj_MC16${mc}"
#gridID1="v29lj_MC16${mc}_syst"
gridID1="v32lj_MC16${mc}"
#gridID2="v0_nominal"
#gridID2="v0"
#gridID2="v1"
#gridID2="v3"
#gridID2="v10"
gridID2="v0"
#version="v16_v29lj_default_v29star_MC16${mc}"
version="v02_v32lj_2ndGridRun_MC16${mc}"

region_out="sgtop_tchan_signal"
#region_out="sgtop_schan_signal_manual"

filter=""
#filter="nominal"
#filter="410644"
#filter="364114"
#filter="364100"
#filter="BJES.*410658"

#gridID2="v0"

# ruciodir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/rucio/${gridID1}.${gridID2}"
# memdir=$ruciodir/mem/
# appdir=$ruciodir/app/
# memoutdir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/${region_out}/AtlSgTop_sChannelAnalysis/MemTkAnalysis"
# appoutdir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/${version}/${region_out}/AtlSgTop_sChannelAnalysis"

# test v29
# ruciodir="/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_default_v29star_MC16a/rucio/debug"
# memdir=$ruciodir/mem/
# appdir=$ruciodir/app/
# ruciodir="/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_default_v29star_MC16a/rucio/test/hadd_test_1/"
ruciodir="/rdsk/dats1/atlas/kkreul/combination/A++/${version}/rucio/${gridID1}.${gridID2}"
appdir=$ruciodir/app/
memdir=$ruciodir/mem/
# appoutdir="/rdsk/datm8/atlas/kaphle/13TeV_s_channel/v16_v29lj_default_v29star_MC16a/rucio/test_out"
appoutdir="/rdsk/dats1/atlas/kkreul/combination/A++/${version}/${region_out}/AtlSgTop_sChannelAnalysis"
memoutdir="${appoutdir}/MemTkAnalysis"

indir=$memdir
outdir=$memoutdir
prefix="mem_$prefix"
inprefix=$prefix
outprefix=$prefix

# indir=$appdir
# outdir=$appoutdir
# inprefix="app_$prefix"
# outprefix=$prefix

# logfile
logfile=$ruciodir/rucio_copy_`date +%FT%T`.log
touch $logfile

nchecked=0
ncopied=0
nlinked=0

do_copy=0
# maybe later with copy option: if [ -n $1 ] 
use_subdir=0
force=""
interactive=""
completed_only=0
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_a/v1/downloaded_systs_mc16a.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_d/v1/downloaded_systs_mc16d.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/v1/downloaded_systs_mc16${mc}.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/v1/dl_nominal_tmp.txt"
#completed_file="$HOME/atlas/download/v25lj_MEM/v25lj_default_${mc}/v2/systematics.txt"
completed_file="$HOME/atlas/download/v29lj_MEM_test/${mc}/v0/systematics.txt"

# OPTIND=1
while getopts ":cfis" arg ; do
    case ${arg} in
	c) do_copy=1 ;;
	f) force="f" ;;
	i) interactive="i" ;;
	s) use_subdir=1 ;;
	\?) echo "Usage: ./copy_files_gridid2version.sh [-cfis]" ; exit 1 ;;
    esac
done
shift $(( OPTIND - 1 ))



# loop
if [[ ${use_subdir} -gt 0 ]] ; then
    for dir in ${indir%*/}/*/
    do
	# get file
	dir=${dir%*/}
	# test that there is exactly 1 file
	nfiles=`ls $dir | wc -l`
	if (( $nfiles != 1 )); then
	    echo "Not exactly 1 file in $dir; skip!" | tee -a $logfile
	    continue
	fi
	# cut unnecessary part from file
	file=`ls $dir`
	fin=$dir/$file
	file=${file##*$inprefix}
	# get substructure from file (split array)
	njets=${file[(ws:_:)1]%%j*}
	channel=${file[(ws:_:)2]}
	sample=${file[(ws:_:)-1]%%.root*}
	syst=${${file#*${channel}_}%_${sample}*}
	# add JET_ to systematic if necessary
	if [[ ${add_JET} -gt 0 ]] ; then
	    if [[ "${syst}" =~ "^CategoryReduction*" ]] ; then
		syst="JET_${syst}"
	    fi
	fi
	# check if directory exists and create if necessary
	dout=$outdir/${njets}Jets/$channel/$syst
	if [ ! -e $dout ]; then
	    echo "Output dir $dout does not exist; mkdir -p" | tee -a $logfile
	    mkdir -p $dout
	fi
	# get output file name (build file name to remove possible .2 endings)
	fout=$dout/$outprefix${njets}j_${channel}_${syst}_${sample}.root
	# check if output file exists already
	# -f: exists and regular file
	# -h: exists and symlink
	# -z: length of string is zero
	if [[ ( -f $fout || -h $fout ) && -z $force ]]; then
	    echo "File $fout already exists; skip!" | tee -a $logfile
	    continue
	fi

	# copy or link file
	if [[ ${do_copy} -gt 0 ]] ; then
	    cp_cmd="cp -T${force}${interactive} $fin $fout"
	    ncopied=$((ncopied+1))
	    echo "${cp_cmd}"
	    # $cp_cmd
	else
	    link_cmd="ln -T${force}${interactive} ${fin} ${fout}"
	    nlinked=$((nlinked+1))
	    # echo "${link_cmd}"
	    $link_cmd
	fi
    done
else
    # input dir is already ruciodir
    # loop over files inside
    for file in ${indir%*/}/*
    do
	# skip log files
	if [[ $file == *.log ]] || [[ $file != *.root* ]] ; then
	    echo "File $file is not a root file"
	    continue
	fi

	# filter source first
	if [[ ! ${file} =~ ${filter} ]]
	then
	    continue #{ echo "file $file did not match filter $filter" ; continue }
	fi
	nchecked=$((nchecked+1))
	
	# input file
	fin=$file
	# cut unnecessary part from beginning of file
	file=${file##*$inprefix}
	# get substructure from file (split array)
	njets=${file[(ws:_:)1]%%j*}
	channel=${file[(ws:_:)2]}
	sample=${file[(ws:_:)-1]%%.root*}
	syst=${${file#*${channel}_}%_${sample}*}
	# add JET_ to systematic if necessary
	if [[ ${add_JET} -gt 0 ]] ; then
	    if [[ "${syst}" =~ "^CategoryReduction*" ]] ; then
		syst="JET_${syst}"
	    fi
	fi
	# only use completed systematics?
	if [[ ${completed_only} -gt 0 ]]; then
	    syst_found=$(grep $syst $completed_file)
	    if [[ -z $syst_found ]]; then
		echo "systematic ${syst} was not found to be completed; skip"
		continue
	    fi
	fi
	# check if directory exists and create if necessary
	dout=$outdir/${njets}Jets/$channel/$syst
	if [ ! -e $dout ]; then
	    echo "Output dir $dout does not exist; mkdir -p" | tee -a $logfile
	    mkdir -p $dout
	fi
	# get output file name (build file name to remove possible .2 endings; here: also other numbers)
	fout=$dout/$outprefix${njets}j_${channel}_${syst}_${sample}.root
	# check if output file exists already
	# -f: exists and regular file
	# -h: exists and symlink
	# -z: length of string is zero
	if [[ ( -f $fout || -h $fout ) && -z $force ]]; then
	    echo "File $fout already exists; skip!" | tee -a $logfile
	    continue
	fi
	# copy or link file
	if [[ ${do_copy} -gt 0 ]] ; then
	    cp_cmd="cp $fin $fout"
	    ncopied=$((ncopied+1))
	    echo "${cp_cmpd}"
	    echo HI
            #cp_cmd=("echo"" HALLO")
            eval $cp_cmd
	else
	    link_cmd="ln -sT${force}${interactive} ${fin} ${fout}"
	    nlinked=$((nlinked+1))
	     echo "${link_cmd}"
	    
            eval $link_cmd
	fi
    done
fi



echo "Checked $nchecked files."  | tee -a $logfile
echo "Copied $ncopied files."  | tee -a $logfile
echo "Linked $nlinked files."  | tee -a $logfile
echo "See logfile ${logfile} for details."
