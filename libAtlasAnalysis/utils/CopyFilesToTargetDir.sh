#!/bin/sh
#
# Copy MemTk ntuples to their designated target directories:
#
#   Copy .root files from a common download directory $1
#   to target directories. Target base directory given by $2.
#

if [ "$1" = "-h" -o "$1" = "--help" ]; then
   script=$(basename $0)
   echo "Usage: $script DownloadDirectory TargetBaseDirectory"
   echo "       e.g. DownloadDirectory   = /rdsk/datm7/atlas/rieck/8TeV_s_channel/v14j/sig_INTschanME/download"
   echo "            TargetBaseDirectory = /rdsk/datm7/atlas/stamm/8TeV_s_channel/v14/sig_INTschanME/AtlSgTop_sChannelAnalysis/MemTkAnalysis/2Jets"
   exit 0
fi

if [ $# -ne 2 ]; then
	echo "Wrong number of arguments. Abort."
	exit
fi

DownloadDir=$1
TargetBaseDir=$2

LepFlavs="
enu
munu
"

Systs="
BJesUnc_down
BJesUnc_up
EtaIntercalibrationModel_down
EtaIntercalibrationModel_up
EtaIntercalibrationTotalStat_down
EtaIntercalibrationTotalStat_up
JesEffectiveDet1_down
JesEffectiveDet1_up
JesEffectiveDet2_down
JesEffectiveDet2_up
JesEffectiveDet3_down
JesEffectiveDet3_up
JesEffectiveMix1_down
JesEffectiveMix1_up
JesEffectiveMix2_down
JesEffectiveMix2_up
JesEffectiveMix3_down
JesEffectiveMix3_up
JesEffectiveMix4_down
JesEffectiveMix4_up
JesEffectiveModel1_down
JesEffectiveModel1_up
JesEffectiveModel2_down
JesEffectiveModel2_up
JesEffectiveModel3_down
JesEffectiveModel3_up
JesEffectiveModel4_down
JesEffectiveModel4_up
JesEffectiveStat1_down
JesEffectiveStat1_up
JesEffectiveStat2_down
JesEffectiveStat2_up
JesEffectiveStat3_down
JesEffectiveStat3_up
JesEffectiveStat4_down
JesEffectiveStat4_up
Pileup_OffsetMu_down
Pileup_OffsetMu_up
Pileup_OffsetNPV_down
Pileup_OffsetNPV_up
Pileup_Pt_down
Pileup_Pt_up
Pileup_Rho_down
Pileup_Rho_up
PunchThrough_down
PunchThrough_up
SinglePart_down
SinglePart_up
eer_down
eer_up
ees_down
ees_up
muid_res
mums_res
musc_down
musc_up
flavor_comp_down
flavor_comp_up
flavor_response_down
flavor_response_up
jeff
jer
jes_down
jes_up
jvf_down
jvf_up
nominal
res_soft_down
res_soft_up
sc_soft_down
sc_soft_up
"

for lep in $LepFlavs; do
	echo "Lepton Flavour ${lep}"
	for syst in $Systs; do
		echo "Copy files for systematic ${syst}"
		TargetDir=$(echo "${TargetBaseDir}/${lep}/${syst}")
		for file in $(find $DownloadDir -name "*${lep}_${syst}*.root"); do
			cp ${file} ${TargetDir}
		done
	done
	echo ""
done
