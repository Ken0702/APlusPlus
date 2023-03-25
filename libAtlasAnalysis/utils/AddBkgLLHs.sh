#!/bin/sh
#
# Add 
#

if [ "x$1" = "x" ] || [ $1 = "-h" ] || [ $1 = "-help" ] || [ $# -lt 2 ] || [ $# -gt 3 ]; then
	THIS=$(basename $0)
	echo
	echo "USAGE: ${THIS} <signal likelihood directory> <bkg likelihood directory> <log file name (optional)>"
	echo
	echo "ATLAS 8TeV s-channel analysis: Add bkg likelihoods to files containing signal likelihoods."
	echo
	echo "For instance: ${THIS} .../sChanCorrect/MemTkAnalysis/2Jets .../bkgCorrect/MemTkAnalysis/2Jets"
	echo "Will loop over all files in \$1 and search for the corresponding file in \$2."
	echo "If there is one exact match: add bkg likelihoods to the signal file."
	echo "Using tree 'physics' in both files."
	echo "Assuming that there are no background likelihoods in the signal files."
	echo
	echo "Suggestion: cp the signal directory first because its files will be changed."
	echo
	exit 0
fi

SigDir=$1
BkgDir=$2

RootProg=$LIBATLASANALYSIS/utils/AddBkgLLHs.C

SUCCESS=0

LogFile=$3
if [ "$3x" = "x" ]; then
	# default log file name
	LogFile=AddBkgLLhs.job.log
fi
touch $LogFile

# Loop over signal files
for SigFile in $(find $SigDir -name "*\.root"); do
	SigFileBase=$(basename $SigFile)
	BkgFile=$(find $BkgDir -name $SigFileBase)
	nBkgFiles=$(for file in $BkgFile; do echo $file; done | wc -l)
	if [ $nBkgFiles -ne 1 ]; then
		echo "Found ${nBkgFiles} matches for signal file ${SigFile}"
		if [ $nBkgFiles -ne 0 ]; then
			echo ":"
			echo $BkgFile
		fi
		echo "Abort."
		SUCCESS=-1
	fi
	if [ $SUCCESS -eq -1 ]; then
		break
	fi
	echo "Signal file: ${SigFile}"
	echo "Bkg file   : ${BkgFile}"
	echo 
	root -l -q -b "${RootProg}+(\"${SigFile}\",\"${BkgFile}\")" >> $LogFile 2>&1
done

if [ $SUCCESS -eq -1 ]; then
	echo "Warning. Executation not successful."
fi

echo "Log file: ${LogFile}"
