#!/bin/bash
echo "Decoding raw data files"
RAWDATANAME=$1 ######example: h0/thr/data-d1-f0-charge5-ninj50-2020_09_17__18_14_52__-0.raw
DATARAW=$(echo $RAWDATANAME | sed 's/.*-\([0-9_]\+\)__-.*/\1/g') ####extraction of raw data timestamp
FOLDER=$(echo $RAWDATANAME | sed 's/\([a-z0-9/]\+\)data-.*/\1/g') #######extraction folder example: h0/thr
LOG=$(echo $RAWDATANAME | sed 's/.*data-\([a-z0-9_-]\+\)/\1/g') #####log file name
DISK=$(echo $RAWDATANAME | sed 's/.*data-\([a-z0-9_-]\+\)-f.*/\1/g')
PATHROOT=/home/alice/alice/output
PATHLOG=/home/alice/alice/output/log
current_time=$(date "+%Y_%m_%d__%H_%M_%S")
starttime=$(date +"%H.%M.%S,%3N")
echo "-----------------------------------"
echo "Mapping at $starttime"
root.exe -l -b -q macro/mapping_mft.C+ | tee $PATHLOG/mapping\_$current_time\_$LOG.out
echo "Let's analyse digits : $current_time"
echo "-----------------------------------"
root -l -b -q macro/PlotRawDec.C\(\"$PATHROOT/$RAWDATANAME.root\",\"$DATARAW\",\"$current_time\"\)
PATHPLOT=/home/alice/alice/O2/Plots/
if [ ! -d "$PATHPLOT$FOLDER$DISK" ]; then
    echo "Directory $PATHPLOT$FOLDER$DISK DOES NOT exists. Creation of folder ongoing!"
    mkdir -p $PATHPLOT$FOLDER$DISK
fi
set -e
mv $PATHPLOT/*.pdf $PATHPLOT$FOLDER$DISK/.
finishtime=$(date +"%H.%M.%S,%3N")
echo "-----------------------------------"
echo "Done at time $finishtime"
