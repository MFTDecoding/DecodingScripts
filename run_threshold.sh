#!/bin/bash
echo "Decoding raw data files"
RAWDATANAME=$1 ######example: h0/thr/data-d1-f0-charge5-ninj50-2020_09_17__18_14_52__-0.raw
VPULSEL=$(echo $RAWDATANAME | sed 's/.*-charge\([0-9_]\+\).*/\1/g') ##########extraction charge injected example : 5
DATARAW=$(echo $RAWDATANAME | sed 's/.*-\([0-9_]\+\)__-.*/\1/g') ######## extraction timestamp raw data creation
FOLDER=$(echo $RAWDATANAME | sed 's/\([a-z0-9/]\+\)data-.*/\1/g') #######extraction folder example: h0/thr
LOG=$(echo $RAWDATANAME | sed 's/.*data-\([a-z0-9_-]\+\)/\1/g') #####log file name
PATHROOT=/home/alice/alice/output
PATHLOG=/home/alice/alice/output/log
current_time=$(date "+%y%m%d_%H%M%S")
starttime=$(date +"%H.%M.%S,%3N")
echo "-----------------------------------"
echo "Mapping at $starttime"
root.exe -l -b -q macro/mapping_mft.C+ | tee $PATHLOG/mapping\_$current_time.out
echo "Used charge : $VPULSEL"
echo "Chips name analysis done!!!"
echo "--------------------------------"
digittime=$(date +"%H.%M.%S,%3N")
echo "Digit analysis started at $digittime"
root -l -b -q macro/RawDec.C\(\"$PATHROOT/$RAWDATANAME.root\",\"$VPULSEL\"\)
if [  `expr $VPULSEL` -lt 10 ]; then
    DIGITFILE=digit_coordinates_0$VPULSEL"_-"$DATARAW.txt
    echo "Creation de txt file $DIGITFILE"
    mv digit_coordinate.txt macro/$DIGITFILE
    mv macro/$DIGITFILE Thr/$DIGITFILE
else
    DIGITFILE=digit_coordinates_$VPULSEL"_-"$DATARAW.txt
    echo "Creation de txt file $DIGITFILE"
    mv digit_coordinate.txt macro/$DIGITFILE
    mv macro/$DIGITFILE Thr/$DIGITFILE
fi
if [ "$VPULSEL" == 30 ]; then
    sh creationScurve.sh $RAWDATANAME
fi
echo "-----------------------------------"

finishtime=$(date +"%H.%M.%S,%3N")
echo "Done at time $finishtime"
