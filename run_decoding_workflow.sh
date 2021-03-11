#!/usr/bin/env bash

# Run this script to start the following workflow:
#   o2-raw-file-reader-workflow | o2-itsmft-stf-decoder-workflow | o2-itsmft-digit-writer-workflow
# The script takes 3 arguments:
# - the delay (float, in s) needed to delay o2-raw-file-reader-workflow
#   between 2 packets sent to the other DPLs (because they are too slow
#   w.r.t. the raw file reader)
# - the absolute path of the input raw data file to be decoded
# - the input raw data base file name i.e. without the .raw at the end

args=("$@")
NUMBER_OF_ARGS=3

if [ $# -ne ${NUMBER_OF_ARGS} ]; then
    echo "Not enough args ! "
    echo "Mandatory: please give DELAY INPUT_PATH RAWDATA_FILENAME"
    echo "as in the example below :"
    echo "./run_decoding_workflow.sh 0.1 /home/flp/data/ data-2021_03_10__18_23_15__-20551-4"
    exit 1
fi

DELAY_S=${args[0]}
INPUT_PATH=${args[1]}
RAWDATA_FILENAME=${args[2]}


CFGFILE=MFTraw.cfg
touch $CFGFILE
echo "[input-MFT-0]">>$CFGFILE
echo "dataOrigin = MFT" >>$CFGFILE
echo "dataDescription = RAWDATA" >>$CFGFILE
echo "filePath = "${INPUT_PATH}"/"${RAWDATA_FILENAME}".raw" >> $CFGFILE
echo "Creation of "${CFGFILE}" : Done! "
echo "-----------------------------"
cat $CFGFILE
echo "-----------------------------"
START_TIME=$(date +"%Y/%m/%d %H:%M:%S")
echo "Start "${START_TIME}
echo "Using --delay = "$DELAY_S
echo "Run decoder RDHv6 on : "${INPUT_PATH}"/"${RAWDATA_FILENAME}".raw"
timestamp=$(date +"%Y_%m_%d_%H_%M_%S")
mkdir -pv ${INPUT_PATH}/log/
mkdir -pv ${INPUT_PATH}/digits/
log=${INPUT_PATH}/log/decoder-${RAWDATA_FILENAME}-$timestamp.out
ROOTFILE=${INPUT_PATH}/digits/${RAWDATA_FILENAME}.root
#o2-raw-file-reader-workflow -b --delay 0.1 --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --detect-tf0 --configKeyValues 'HBFUtils.nHBFPerTF=128' --input-conf $CFGFILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --writeHW --no-clusters --no-cluster-patterns --decoder-verbosity 1 | o2-itsmft-digit-writer-workflow -b --runmft --writeHW --disable-mc --outfile $ROOTFILE.root > $log
o2-raw-file-reader-workflow -b --delay $DELAY_S --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --detect-tf0 --input-conf $CFGFILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --writeHW --no-clusters --no-cluster-patterns | o2-itsmft-digit-writer-workflow -b --runmft --writeHW --disable-mc --outfile $ROOTFILE > $log
rm -f $CFGFILE

echo "Log in : " $log
echo "Digits in : " $ROOTFILE
echo
echo "[ERROR]"
grep "ERROR" $log | wc -l

echo "Error Jump in RDH_packetCounter :  " 
grep "Jump in RDH_packetCounter" $log | wc -l

echo "[ERROR] Failed to open input file"
grep "Failed to open input file" $log | wc -l

echo "[ERROR] Mismatch between flagged and calculated new TF start"
grep "Mismatch between flagged and calculated new TF start" $log | wc -l

echo "[ERROR] Unknown word"
grep "Unknown word" $log | wc -l

echo "[ERROR] SEVERE" 
grep "SEVERE" $log | wc -l
echo
STOP_TIME=$(date +"%Y %m %d %H:%M:%S")
echo "Stop " ${STOP_TIME}
echo "Done !"
echo "-----------------------------"
