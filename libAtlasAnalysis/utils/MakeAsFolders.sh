# script makes mc folders at as machines

# ttbar folders

if [ "x$1" = "x" ] || [ $1 = "-h" ]; then
    echo
    SCRIPT=`basename $0`
    echo "USAGE: $SCRIPT <production version (e.g. 'v1')> "
    echo
    exit 1
fi
ProductionVersion=$1


cd /rdsk/dats1/atlas/SgTopD3PD
echo cd /rdsk/dats1/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats2/atlas/SgTopD3PD
echo cd /rdsk/dats2/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats3/atlas/SgTopD3PD
echo cd /rdsk/dats3/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats4/atlas/SgTopD3PD
echo cd /rdsk/dats4/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats5/atlas/SgTopD3PD
echo cd /rdsk/dats5/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats6/atlas/SgTopD3PD
echo cd /rdsk/dats6/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
cd /rdsk/dats7/atlas/SgTopD3PD
echo cd /rdsk/dats7/atlas/SgTopD3PD
mkdir mc12_$ProductionVersion
mkdir mc12_$ProductionVersion/MC
mkdir mc12_$ProductionVersion/MC/ttbar
#cd /rdsk/dats8/atlas/SgTopD3PD
#echo cd /rdsk/dats8/atlas/SgTopD3PD
#mkdir mc12_$ProductionVersion
#mkdir mc12_$ProductionVersion/MC
#mkdir mc12_$ProductionVersion/MC/ttbar

# Remaining folders
cd /rdsk/dats3/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
echo cd /rdsk/dats3/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
mkdir wjetsLF
cd /rdsk/dats4/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
echo cd /rdsk/dats4/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
mkdir wjetsHF
cd /rdsk/dats5/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
echo cd /rdsk/dats5/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
mkdir sgtop
cd /rdsk/dats6/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
echo cd /rdsk/dats6/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
mkdir zjets
cd /rdsk/dats7/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
echo cd /rdsk/dats7/atlas/SgTopD3PD/mc12_$ProductionVersion/MC
mkdir diboson

# DATA & QCDMM
cd /rdsk/dats1/atlas/SgTopD3PD/mc12_$ProductionVersion/
echo cd /rdsk/dats1/atlas/SgTopD3PD/mc12_$ProductionVersion/
mkdir DATA
mkdir DATA/Egamma
mkdir QCDMM
mkdir QCDMM/Muon
cd /rdsk/dats2/atlas/SgTopD3PD/mc12_$ProductionVersion/
echo cd /rdsk/dats2/atlas/SgTopD3PD/mc12_$ProductionVersion/
mkdir DATA
mkdir DATA/Muon
mkdir QCDMM
mkdir QCDMM/Egamma



