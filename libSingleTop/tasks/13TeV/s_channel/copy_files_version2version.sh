#!/bin/zsh
# shell script to copy files from one path to another

# run this with ./copy_files_version2version.sh

# don't allow source, execute this script directly
# ZSH_EVAL_CONTEXT is 'toplevel' when executed, but 'toplevel:file' when sourced
if [[ $ZSH_EVAL_CONTEXT != 'toplevel' ]] ; then
    echo "Usage: ./copy_files_version2version.sh [-cf]"
    return 1
fi

prefix="ntup_schan_"
suffix=".root"

mc="e"
# version_in="v16_v29lj_Wjv29star_Zjv29_MC16${mc}"
# version_in="v16_v29lj_default_QCDsyst_MC16${mc}"
version_in="v16_v29lj_default_v29star_MC16${mc}"
# version_out="v16_v29lj_default_v29star_MC16${mc}"
# version_out="v16_v29lj_QCDJL_TreatAllJetsAsBJets_MC16${mc}"
# version_out="v16_v29lj_default_QCDsyst_MC16${mc}"
version_out="v16_v29lj_default_MCWEIGHT_MC16${mc}"

# version_in="v15_v25lj_default_MC16${mc}"
# version_out="v15_v25lj_default_MC16${mc}"
#version_in="v15_v25lj_default_samplesyst_MC16${mc}"
#version_out="v15_v25lj_default_samplesyst_MC16${mc}"
#region_in="sgtop_schan_signal_manual"
#region_in="sgtop_schan_signal_manual364188-90"
# region_in="sgtop_schan_signal_localNominalRun"
region_in="sgtop_schan_signal"
region_out="sgtop_schan_signal"
# region_in="sgtop_schan_wjets"
# region_out="sgtop_schan_wjets"
# region_in="sgtop_schan_ttbar3j"
# region_out="sgtop_schan_ttbar3j"
# region_in="sgtop_schan_ttbar4j"
# region_out="sgtop_schan_ttbar4j"

disk="datm8"

filter="nominal"
#filter=""
#filter="nominal_QCDA"
numfilter="^[0-9]+$"
# numfilter=""

#indir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_in}/${region_in}/AtlSgTop_sChannelAnalysis/MemTkAnalysis"
#outdir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_out}/${region_out}/AtlSgTop_sChannelAnalysis/MemTkAnalysis"
indir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_in}/${region_in}/AtlSgTop_sChannelAnalysis/MemDisc"
outdir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_out}/${region_out}/AtlSgTop_sChannelAnalysis/MemDisc"
# indir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_in}/${region_in}/AtlSgTop_sChannelAnalysis/MemTkAnalysis"
# outdir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_out}/${region_out}/AtlSgTop_sChannelAnalysis/MemTkAnalysis"
# inprefix="mem_$prefix"
# outprefix="mem_$prefix"
# indir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_in}/${region_in}/AtlSgTop_sChannelAnalysis"
# outdir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_out}/${region_out}/AtlSgTop_sChannelAnalysis"
inprefix="$prefix"
outprefix="$prefix"

# indir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_in}/${region_in}/AtlSgTop_sChannelAnalysis"
# outdir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version_out}/${region_out}/AtlSgTop_sChannelAnalysis"


# logfile
logfile=$indir/normal_copy_`date +%s`.log
touch $logfile

nchecked=0
ncopied=0
nlinked=0

do_copy=0
# maybe later with copy option: if [ -n $1 ] 
force=""
interactive=""

while getopts cf arg ; do
    case ${arg} in
	c) do_copy=1 ;;
	f) force="f" ;;
	\?) echo "Usage: ./copy_files_version2version.sh [-cf]" ; exit 1 ;;
    esac
done
shift $(( OPTIND - 1 ))

# loop
for file in $( find -L ${indir}/[2-4]Jets/ -type f -name "${inprefix}*.root*" )
do
    # skip log files
    if [[ $file == *.log ]] || [[ $file != *.root* ]] ; then
	echo "File $file is not a root file"
	continue
    fi

    # filter source first
    if [[ ! ${file} =~ ${filter} ]] ; then
	#{ echo "file ${file} does not match filter $filter" ; continue }
	continue
    fi
    nchecked=$((nchecked+1))

    # input file
    fin=$file

    # cut unnecessary part from beginning of file: dir and prefix
    file=${file[(ws:/:)-1]}
    file=${file##*$inprefix}
    # get substructure from file (split array)
    njets=${file[(ws:_:)1]%%j*}
    channel=${file[(ws:_:)2]}
    sample=${file[(ws:_:)-1]%%.root*}
    syst=${${file#*${channel}_}%_${sample}*}

    # check valid number of jets
    if [[ ! ${njets} =~ ${numfilter} ]] ; then
	# must comment the echo because for some reason I get a segfault if it stays
	# echo "Number of jets from input: ${njets} is not a number; skip"
	continue
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
    if [[ -f $fout || -h $fout && -z $force ]]; then
	echo "File $fout already exists; skip!" | tee -a $logfile
	continue
    fi
    # copy or link file
    if [[ ${do_copy} -gt 0 ]] ; then
	cp_cmd="cp $fin $fout"
	ncopied=$((ncopied+1))
	echo "${cp_cmpd}"
	$cp_cmd
    else
	link_cmd="ln -sT${force}${interactive} ${fin} ${fout}"
	nlinked=$((nlinked+1))
	# echo "${link_cmd}"
	$link_cmd
    fi
done




echo "Checked $nchecked files."  | tee -a $logfile
echo "Copied $ncopied files."  | tee -a $logfile
echo "Linked $nlinked files."  | tee -a $logfile
