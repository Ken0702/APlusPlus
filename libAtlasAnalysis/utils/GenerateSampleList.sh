#!/bin/sh
#
#
#
if [ "x$1" = "x" ] || [ $1 = "-h" ]; then
    echo
    SCRIPT=`basename $0`
    echo "USAGE: $SCRIPT <output filename> <as or ms sample list: 'as' or 'ms'> <SgTopD3PD version (e.g. 'v1')> <list of input datasets> <xsection file>"
    echo
    exit 1
fi

if [ "x$1" = "x" ]; then
    echo
    echo "No output filename given."
    echo "Abort!"
    echo
    exit 1
fi
SampleList=$1
echo $3

if [ "x$3" = "xv34_lj" ]
    then
       MSAS="dat"
        
    else

    # switch for as / ms
    case $2 in
        (as0*)
        MSAS="dats"
        ;;
        (as1*)
        MSAS="datv"
        ;;
        (ms*)
        MSAS="datm"
        ;;
        *)
            echo
            echo "Not v34: " $3
            echo "Unknown data disk: "$2
            echo "Abort!"
            echo
            exit 1
            
    esac
fi

echo
echo "Generating sample list for "$MSAS
echo

# SgTopD3PD version
if [ "x$3" = "x" ]; then
    echo
    echo "No SgTopD3PD version number provided!"
    echo "Abort!"
    echo
    exit 1
fi
Version=$3

# Input list (sample <-> DSID match)
if [ "x$4" = "x" ]; then
    echo
    echo "No Input list for sample <-> DSID match provided!"
    echo "Abort!"
    echo
    exit 1
fi
if [ ! -e $4 ]; then
    echo 
    echo "Input list for sample <-> DSID match does not exist!"
    echo "Abort!"
    echo
    exit 1
fi
DatasetList=$4

if [ ! -e $5 ]; then
    echo 
    echo "No Xsection file (from TopDataPreparation) provided."
    echo "Abort!"
    echo
    exit 1
fi
XsectionFile=$5

Campaign=$6
if [ "x${Campaign}" = xmc* ] ; then
    echo
    echo "Campaign invalid or undefined."
    echo "Abort!"
    echo
    exit 1
fi

if [ -e $SampleList ]; then
    rm $SampleList
    echo "Old Sample List deleted"
fi 
touch $SampleList
echo "Create new Sample List: '"$SampleList"'"

# read input list, line by line
echo "#" >> $SampleList
echo "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" >> $SampleList
echo "# !!! This is an automatically generated file !!!" >> $SampleList
echo "# !!! D O   N O T   E D I T                   !!!" >> $SampleList
echo "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" >> $SampleList
echo "#" >> $SampleList
echo "#" >> $SampleList

OldSampleType=""
for Line in $(cat $DatasetList | grep -v "^#" | grep -v "^$"); do
    SampleShort=$(echo $Line | awk -F, '{print $1}')
    SampleType=$(echo $Line | awk -F, '{print $2}')
    DSID=$(echo $Line | awk -F, '{print $3}')
    Color=$(echo $Line | awk -F, '{print $4}')
    SimType=$(echo $Line | awk -F, '{print $5}')
    
    # print some names
    SampleVar=$SampleType
    if [ "$OldSampleType" != "$SampleVar" ]; then
	echo "#" >> $SampleList
	echo "# "$SampleType >> $SampleList
	echo "#" >> $SampleList
	OldSampleType=$SampleVar
    fi

    # allow reco tag (or some other tag) to be part of DSID, separated with *
    reco=""
    if [[ $DSID = *"*"* ]]; then
	reco=$DSID # could filter out DSID part here, but not needed bc of glob *
	DSID=$( echo $DSID | awk -F* '{print $1}')
    fi 

    # detector sim regex
    regex_af="^.*_a[0-9]+_.*$"
    regex_fs="^.*_s[0-9]+_.*$"
   
    if [ "${Campaign}" != mc12 ] ; then
        # =====
        # Run-2
        # =====
        append=0 # track whether paths should be appended to the current sample
        case ${SampleType} in
            kDATA*)
		for folder in $( find /rdsk/$MSAS[svm2]*/atlas/SgTopD3PD/${Campaign}_${Version}/DATA -type d -name "user*.*${DSID}.*" ) ; do
                    if [ "${append}" -gt 0 ] ; then
			echo + + + + "${folder}"
                    else
			echo "${SampleShort}" "${SampleType}" + "${Color}" "${folder}"
			append=1
                    fi
		done >>"${SampleList}"
		;;
	    kQCDMatrix*)
		for folder in $( find /rdsk/$MSAS[svm2]*/atlas/SgTopD3PD/${Campaign}_${Version}/DATA -type d -name "user*.*${DSID}.*" ) ; do
                    if [ "${append}" -gt 0 ] ; then
			echo + + + + "${folder}"
                    else
			echo "${SampleShort}" "${SampleType}" + "${Color}" "${folder}"
			append=1
                    fi
		done >>"${SampleList}"
		;;
            kQCD_AntiMu*)
		echo "find /rdsk/$MSAS[svm2]*/atlas/SgTop_Fake/AntiMuon_${Version} -type d -name user*.*.${DSID}.*"
                for folder in $( find /rdsk/$MSAS[svm2]*/atlas/SgTop_Fake/AntiMuon_${Version} -type d -name "user*.*.${DSID}.*" ) ; do
		    bname=`basename $folder`
		    if [ "$reco" ] && [[ "$bname" != *$reco* ]]; then
			continue
		    fi
                    
                    if [[ $folder == *"shapeSyst1"* ]]; then
                        echo "#""${SampleShort}"ShapeSyst1 "${SampleType}" + "${Color}" "${folder}"
                        continue
                    fi
                    if [[ $folder == *"shapeSyst2"* ]]; then
                        echo "#""${SampleShort}"ShapeSyst2 "${SampleType}" + "${Color}" "${folder}"
                        continue
                    fi

                    if [ "${append}" -gt 0 ] ; then
                        echo + + + + "${folder}"
                    else
                        echo "${SampleShort}" "${SampleType}" + "${Color}" "${folder}"
                        append=1
                    fi
                done >>"${SampleList}"
                ;;
            kQCD_JetLep*)
                for folder in $( find /rdsk/$MSAS[svm2]*/atlas/SgTop_Fake/JetLepton_${Version} -type d -name "user*${reco}*" ) ; do
                    
                    if [[ $folder == *"shapeSyst1"* ]]; then
                        echo "#""${SampleShort}"ShapeSyst1 "${SampleType}" + "${Color}" "${folder}"
                        continue
                    fi
                    if [[ $folder == *"shapeSyst2"* ]]; then
                        echo "#""${SampleShort}"ShapeSyst2 "${SampleType}" + "${Color}" "${folder}"
                        continue
                    fi
                    if [ "${append}" -gt 0 ] ; then
                        echo + + + + "${folder}"
                    else
                        echo "${SampleShort}" "${SampleType}" + "${Color}" "${folder}"
                        append=1
                    fi
                done >>"${SampleList}"
                ;;
            *) # presumably MC
		Xsection=$( awk -v "DSID=${DSID}" '$1 == DSID { print $2*$3 }' "${XsectionFile}" )
		if [ "x${Xsection}" = x ] && [ "${DSID}" != 957011 ] ; then
                    echo "ERROR: cross section of dataset with ID ${DSID} not known, skipping." >&2
		else
		    if [ "${DSID}" = 957011 ]; then # <--- Fix for SgTop FCNC (normalize to 0.1 pb)
			Xsection=0.1 # <--- 0.1 pb for sigma*BR
		    fi
                    #echo "/rdsk/$MSAS[svm2]*/atlas/SgTopD3PD/${Campaign}_${Version}/MC -type d -name user*.mc*TeV.${DSID}.*"
                    #echo $( find /rdsk/$MSAS[svm2]*/atlas/SgTopD3PD/${Campaign}_${Version}/MC -type d -name "user*.mc*TeV.${DSID}.*" )
                        #
                    for folder in $( find /rdsk/$MSAS[svm2]*/atlas/SgTopD3PD/${Campaign}_${Version}/MC -type d -name "user*.mc*TeV.${DSID}.*" ) ; do
                        #Accept either FullSim or AtlFast2
			bname=`basename $folder`
			case $SimType in
			    (*kFullSim*)
				if [[ "$bname" =~ $regex_af ]]; then
				    continue

				fi
				;;
			    (*kAtlFast*)
				if [[ "$bname" =~ $regex_fs ]]; then
				    continue
				fi
				;;
			esac
			if [ "$reco" ] && [[ "$bname" != *$reco* ]]; then
			    continue
			fi
			if [ "${append}" -gt 0 ] ; then
                            echo + + + + "${folder}"
			else
                            echo "${SampleShort}" "${SampleType}" "${Xsection}" "${Color}" "${folder}"
                            append=1
			fi
                    done >>"${SampleList}"
		fi
		;;
        esac
    else
	# =====
        # Run-1
	# =====
	
	Xsection=0 # dummy cross section for data
	case $SampleType in
	    (*QCD*EGamma*)
	    # For MM one has to use tight (DATA) and loose (QCDMM) samples
	    # commented out data samples for v13 
	    # for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/DATA/Egamma/user*.data* | grep $DSID); do  
	    # echo $SampleShort"_tight "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    # done
	    for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/QCDMM/Egamma/user*.data* | grep $DSID); do  
		echo $SampleShort"_loose "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    done
	    ;;
	    (*QCD*Muon*)
   	    # For MM one has to use tight (DATA) and loose (QCDMM) samples
	    # commented out data samples for v13 
	    # for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/DATA/Muon/user*.data* | grep $DSID); do  
	    # echo $SampleShort"_tight "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    # done
	    for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/QCDMM/Muon/user*.data* | grep $DSID); do  
		echo $SampleShort"_loose "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    done
	    ;;
	    (*DATA*EGamma*)
	    for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/DATA/Egamma/user*.data* | grep $DSID); do  
		echo $SampleShort" "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    done
	    ;;
	    (*DATA*Muon*)
	    for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/DATA/Muon/user*.data* | grep $DSID); do  
		echo $SampleShort" "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
	    done
	    ;;
	    (*) # it's MC
	    for folder in $(ls -d /rdsk/$MSAS*/atlas/SgTopD3PD/mc12_$Version/MC/*/user*.mc* | grep $DSID); do
		Xsection=$( echo $(cat $XsectionFile | grep $DSID | awk -F" "  '{print $2*$3}' ) )
		case $folder in
		    (*ttbar*$Version.*) # ttbar is splitted
		    Number=$(echo $folder | awk -F. '{ print $NF }')
		    echo $SampleShort$Number" "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
		    ;;
		(*$Version) # everything else
		echo $SampleShort" "$SampleType" "$Xsection" "$Color" "$folder >> $SampleList
		;;
		esac
	    done
	esac
    fi
done
