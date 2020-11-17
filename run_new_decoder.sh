#!/bin/bash
FILE=MFTraw.cfg
touch $FILE
echo "[input-MFT-0]">>$FILE
echo "dataOrigin = MFT" >>$FILE
echo "dataDescription = RAWDATA" >>$FILE
echo "filePath = /home/o2flp/alice/data_raw/"$1 >> $FILE
echo "Creation of MFTraw.cfg : Done! "
echo "-----------------------------"
echo "Run decoder RDHv6"
timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
log=/home/alice/alice/output/log/decode_$timestamp.out
ROOTFILEPATH=/home/alice/alice/output/
o2-raw-file-reader-workflow -b --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --input-conf $FILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --no-clusters --no-cluster-patterns | o2-itsmft-digit-writer-workflow -b --runmft --disable-mc --outfile $ROOTFILEPATH$1.root > $log
rm -f $FILE
echo "Error Jump in RDH_packetCounter :  " 
grep -r "Jump in RDH_packetCounter" $log | wc -l
grep -o -P 'FEE.{0,7}' $log
root -l -q -b macro/ChipDecod.C\(\"$ROOTFILEPATH$1.root\"\)
if [ $(echo $1 | grep "charge") ];then
    VPULSE=$(echo $1 | sed 's/.*-charge\([0-9_]\+\).*/\1/g') 
    echo $VPULSE
    if [ "$VPULSE" -gt 50 ]; then
        echo "We prepare plots from raw data for the Analog scan"
        ./run_plot.sh $1
     else
        echo "We analyze the digits to create a S-Curve for the Threshold scan"
        ./run_threshold.sh $1 
    fi
else
    echo "We prepare plots from raw data for Noise or Digital scan"
    ./run_plot.sh $1
fi
