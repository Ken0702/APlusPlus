#!/hint/zsh
#
# QCD param paths:
# /rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM21p2_MC16[a/d]_param[0-4]/
#
# nominal path for other samples:
# /rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj_MC16d/
# /rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj_MC16a_newQCD/

# utility functions
function copy_symlinks () {
    p_source=$1
    p_target=$2
    for sample in ${p_source}/*.root
    do
	# skip QCD (done automatically bc link exists)
	# for now: simply echo link command
	link_cmd="ln -s ${sample} ${p_target}"
	# echo "$link_cmd"
	$link_cmd
    done
}

function copy_region () {
    p_source=$1
    p_target=$2
    p_region=$3
    path_source=${p_source}/${p_region}
    for path_param in ${p_target}_param[0-4]/
    do
	echo $path_param
	path_target=${path_param}/${p_region}
	cmd="copy_symlinks $path_source $path_target"
	# echo "$cmd"
	$cmd
    done
}

# paths
path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj_MC16a_newQCD/"
path_source_a_old="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj/"
path_source_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v10_v21lj_MC16d/"
path_target_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM21p2_MC16a"
path_target_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM21p2_MC16d"
#path_param_a0="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM21p2_MC16a_param0/"
#path_param_a_all=`/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM21p2_MC16a_param[0-4]/`
path_qcd="sgtop_schan_QCD/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal"
path_wjets="sgtop_schan_wjets/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal"
path_ttbar3j="sgtop_schan_ttbar3j/AtlSgTop_sChannelAnalysis/3Jets/lnu/nominal"
path_ttbar4j="sgtop_schan_ttbar4j/AtlSgTop_sChannelAnalysis/4Jets/lnu/nominal"
path_signal="sgtop_schan_signal/AtlSgTop_sChannelAnalysis/2Jets/lnu/nominal"
#QCD_a0="${param_a0}sgtop_schan_QCD/"

# MC16a QCD:
#copy_region $path_source_a $path_target_a $path_qcd

# MC16d QCD:
#copy_region $path_source_d $path_target_d $path_qcd

# MC16a ttbar3j:
#copy_region $path_source_a $path_target_a $path_ttbar3j
#copy_symlinks ${path_source_a}/${path_ttbar3j} ${path_target_a}_param4/${path_ttbar3j}

# MC16d ttbar3j:
#copy_region $path_source_d $path_target_d $path_ttbar3j

# MC16a ttbar4j:
#copy_symlinks ${path_source_a_old}/${path_ttbar4j} ${path_target_a}_param4/${path_ttbar4j}

# MC16d ttbar4j:
#copy_symlinks ${path_source_d}/${path_ttbar4j} ${path_target_d}_param4/${path_ttbar4j}

# MC16a Wjets:
#copy_region $path_source_a $path_target_a $path_wjets

# MC16d Wjets:
#copy_symlinks ${path_source_d}/${path_wjets} ${path_target_d}_param4/${path_wjets}
#copy_region $path_source_d $path_target_d $path_wjets

# new eff by NB, 2 parametrizations, all regions
path_target_NB="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_NB0615_MC16"
# copy_symlinks ${path_source_a_old}/${path_ttbar4j} ${path_target_NB}a_pt/${path_ttbar4j}
# copy_symlinks ${path_source_d}/${path_ttbar4j} ${path_target_NB}d_pt/${path_ttbar4j}
# copy_symlinks ${path_source_a}/${path_ttbar3j} ${path_target_NB}a_pt/${path_ttbar3j}
# copy_symlinks ${path_source_d}/${path_ttbar3j} ${path_target_NB}d_pt/${path_ttbar3j}
# copy_symlinks ${path_source_a}/${path_wjets} ${path_target_NB}a_pt/${path_wjets}
# copy_symlinks ${path_source_d}/${path_wjets} ${path_target_NB}d_pt/${path_wjets}
# copy_symlinks ${path_source_a}/${path_qcd} ${path_target_NB}a_pt/${path_qcd}
# copy_symlinks ${path_source_d}/${path_qcd} ${path_target_NB}d_pt/${path_qcd}

# copy_symlinks ${path_source_a_old}/${path_ttbar4j} ${path_target_NB}a_eta:jetpt:met/${path_ttbar4j}
# copy_symlinks ${path_source_d}/${path_ttbar4j} ${path_target_NB}d_eta:jetpt:met/${path_ttbar4j}
# copy_symlinks ${path_source_a}/${path_ttbar3j} ${path_target_NB}a_eta:jetpt:met/${path_ttbar3j}
# copy_symlinks ${path_source_d}/${path_ttbar3j} ${path_target_NB}d_eta:jetpt:met/${path_ttbar3j}
# copy_symlinks ${path_source_a}/${path_wjets} ${path_target_NB}a_eta:jetpt:met/${path_wjets}
# copy_symlinks ${path_source_d}/${path_wjets} ${path_target_NB}d_eta:jetpt:met/${path_wjets}
# copy_symlinks ${path_source_a}/${path_qcd} ${path_target_NB}a_eta:jetpt:met/${path_qcd}
# copy_symlinks ${path_source_d}/${path_qcd} ${path_target_NB}d_eta:jetpt:met/${path_qcd}

# # new eff, lots of params for QCDVR
# path_target_NB="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_NB0615_MC16"
# MCs=("a" "d")
# params=("eta" "dPhi" "dR" "nbtag" "mtw" "pt:eta" "pt:dPhi" "pt:dR" "eta:dPhi" "eta:dR" "dPhi:dR" "eta:jetpt" "jetpt:eta")
# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     for p in "${params[@]}"
#     do
# 	cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target_NB}${MC}_${p}/${path_qcd}"
# 	echo $cmd
# 	$cmd
# #	break
#     done
# done

# # new eff JM0702, lots of params for QCDVR
# path_target_JM0702="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_JM0702_MC16"
# MCs=("a" "d")
# params=("eta" "pt" "dR" "eta:pt" "eta:dR" "pt:dR" "eta:pt:dR")
# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     for p in "${params[@]}"
#     do
# 	cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target_JM0702}${MC}_${p}/${path_qcd}"
# 	echo $cmd
# 	$cmd
# #	break
#     done
# done

# #############################################################################

# # new eff NBsgtop just one param QCDVR to test
# MCs=("a" "d")
# # doing this with new binning, both a and d don't have QCD included
# path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_newbinning_a"
# path_source_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_newbinning_d"

# # first version:
# # path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_NBsgtop_MC16"
# # params=("pt" "eta" "dR" "eta:pt")

# # update: _newDL for new download of eff. after change
# # path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_NBsgtop_newDL_MC16"
# # params=("pt" "eta" "dR" "eta:pt")

# # update: use ge4j2b region -> parametrization changed
# path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v21lj_QCDtests_NBsgtop_newDL_MC16"
# params=("ge4j2b:pt" "ge4j2b:dPhi" "ge4j2b:dR" "ge4j2b:jetpt")

# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     for p in "${params[@]}"
#     do
# 	cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target}${MC}_${p}/${path_qcd}"
# 	echo $cmd
# 	$cmd
#     done
# done

# #############################################################################

# # v24 with JM0702 eff
# # pt was done with all, all other have to be copied
# path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v24lj_QCDtests_JM0702_MC16"
# path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v11_v24lj_QCDtests_JM0702_MC16a_pt"
# MCs=("a" )
# # params=("eta" "dR" "eta:pt" "dPhi")

# # update: use ge4j2b region -> parametrization changed
# params=("ge4j2b:pt" "ge4j2b:dPhi" "ge4j2b:dR" "ge4j2b:jetpt")

# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     for p in "${params[@]}"
#     do
# 	cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target}${MC}_${p}/${path_qcd}"
# 	echo $cmd
# 	$cmd
# #	break
#     done
# done

# #############################################################################

# # JetLepton method
# path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_QCDtests_JetLepton_MC16"
# path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16a"
# path_source_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16d"
# MCs=("a" "d")
# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     # cmd="copy_symlinks ${path_source}/${path_wjets} ${path_target}${MC}/${path_wjets}"
#     # cmd="copy_symlinks ${path_source}/${path_ttbar3j} ${path_target}${MC}/${path_ttbar3j}"
#     # cmd="copy_symlinks ${path_source}/${path_ttbar4j} ${path_target}${MC}/${path_ttbar4j}"
#     cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target}${MC}/${path_qcd}"
#     # cmd="copy_symlinks ${path_source}/${path_signal} ${path_target}${MC}/${path_signal}"
#     echo $cmd
#     $cmd
# done

# # #############################################################################

# JetLepton method without btagging in JetLepton samples
path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_QCDtests_JetLeptonNoBTag_MC16"
path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16a"
path_source_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16d"
MCs=("a" "d")
for MC in "${MCs[@]}"
do
    if [ ${MC} = "a" ]; then
	path_source=${path_source_a}
    elif [ ${MC} = "d" ]; then
	path_source=${path_source_d}
    fi
    # cmd="copy_symlinks ${path_source}/${path_wjets} ${path_target}${MC}/${path_wjets}"
    # cmd="copy_symlinks ${path_source}/${path_ttbar3j} ${path_target}${MC}/${path_ttbar3j}"
    # cmd="copy_symlinks ${path_source}/${path_ttbar4j} ${path_target}${MC}/${path_ttbar4j}"
    # cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target}${MC}/${path_qcd}"
    cmd="copy_symlinks ${path_source}/${path_signal} ${path_target}${MC}/${path_signal}"
    echo $cmd
    $cmd
done

# # #############################################################################

# # AntiMuon method
# path_target="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_QCDtests_AntiMuon_MC16"
# path_source_a="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16a"
# path_source_d="/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v13_v23lj_noQCD_MC16d"
# MCs=("a" "d")
# for MC in "${MCs[@]}"
# do
#     if [ ${MC} = "a" ]; then
# 	path_source=${path_source_a}
#     elif [ ${MC} = "d" ]; then
# 	path_source=${path_source_d}
#     fi
#     # cmd="copy_symlinks ${path_source}/${path_wjets} ${path_target}${MC}/${path_wjets}"
#     # echo $cmd
#     # $cmd
#     cmd="copy_symlinks ${path_source}/${path_ttbar3j} ${path_target}${MC}/${path_ttbar3j}"
#     echo $cmd
#     $cmd
#     cmd="copy_symlinks ${path_source}/${path_ttbar4j} ${path_target}${MC}/${path_ttbar4j}"
#     echo $cmd
#     $cmd
#     cmd="copy_symlinks ${path_source}/${path_qcd} ${path_target}${MC}/${path_qcd}"
#     echo $cmd
#     $cmd
# done

# # #############################################################################

# for path_param in ${path_target_a}_param[0-4]/
# do
#     path_target="${path_param}/${path_qcd}/"
#     path_source="${path_source_a}/${path_qcd}/"
#     cmd="copy_symlinks $path_source $path_target"
#     echo "$cmd"
#     $cmd
# done 
