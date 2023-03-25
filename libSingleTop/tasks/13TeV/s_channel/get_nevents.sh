#!/bin/zsh
# shell script to get number of events in all samples
#
# use as ./get_nevents.sh > nevents.txt

# directory and filename elements:
#basedir="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v05_MEM/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v06_MEM/sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/analysis/3Jets/lnu/nominal/"
#basedir="/rdsk/datm5/atlas/kaphle/13TeV_s_channel/v07_MEM/sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/analysis/4Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v08_v20lj/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_defaultWithBDT_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_defaultWithBDT_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16a/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
#basedir="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v14_v23lj_default_MC16d/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"

#version="v15_v25lj_default_samplesyst"
#version="v15_v25lj_default"
#version="v15_v25lj_default_QCDJetLepton"
#version="v16_v29lj_test_app"
#version="v16_v29lj_default_v29star"
#version="v16_v29lj_Wjv29star_Zjv29"
#version="v16_v29lj_default_samplesyst"
#version="v16_v29lj_default_offline_ZjetsCL"
#version="v16_v29lj_default_QCDsyst"
version="v16_v29lj_QCDJL_TreatAllJetsAsBJets"
MC_ver="MC16e"
#region="SR"
#regiondir=""
#regiondir="sgtop_schan_signal_noBTag"
#regiondir="sgtop_schan_signal_bTag85_noinvalid"
regiondir="sgtop_schan_signal"
#njets="0"
njets="2"
systematic="nominal"
#disk="dats5"
disk="datm8"

if [[ $region == "SR" ]]; then
    njets="2"
    regiondir="sgtop_schan_signal"
    regiondir="sgtop_schan_signal_localNominalRun"
    #regiondir="sgtop_schan_signal_manual"
fi


basedir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version}_${MC_ver}/${regiondir}/AtlSgTop_sChannelAnalysis/analysis/${njets}Jets/lnu/nominal"
prefix="ntup_schan_${njets}j_lnu_nominal_"


# basedir="/rdsk/${disk}/atlas/kaphle/13TeV_s_channel/${version}_${MC_ver}/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/"
# prefix="ntup_schan_2j_lnu_nominal_"
suffix=".log"

#target_file="$APP_HOME/libSingleTop/tasks/13TeV/s_channel/SampleSizeLists/mc"

# take basedir from command line; take care of jet bin in prefix anyway!!
if [ -n "$1" ]; then
    basedir=$1
fi

ntotal=0
cptotal=0
# loop over sample directories
printf '# Number of events taken from %s on %s %s\n' $basedir $(date --rfc-3339=seconds)
printf '# %-18s  %8s  %10s\n' "Sample" "n_events" "CPU time"
for dir in ${basedir%*/}/*/
do
    # remove final /
    dir=${dir%*/}
    # extract match (remove dir)
    match=${dir##*/}
    # build filename
    file=${dir}/$prefix$match$suffix
    if [ ! -f $file ]; then
	continue
    fi
    # get number of events (unweighted)
    nevts=$(grep "No. of accepted events," $file)
    nevts=${nevts##*: }
    ntotal=$((ntotal+nevts))
    # get CP time
    cptime=$(grep "Time consumption" $file)
    cptime=${cptime##*time }
    cptotal=$((cptotal+cptime))
    printf '%-20s  %8d  %10.3f\n' $match $nevts $cptime
#    echo "$match $nevts $cptime"    
done
printf '# %-18s  %8s  %10s\n' "Totals:" $ntotal $cptotal
