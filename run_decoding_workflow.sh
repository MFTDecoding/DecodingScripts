#!/bin/bash
args=("$@")
NUMBER_OF_ARGS=2

if [ $# -ne ${NUMBER_OF_ARGS} ]; then
    echo "Not enough args ! "
    echo "Mandatory: please give INPUT_PATH RAWDATA_FILENAME"
    echo "as in the example below :"
    echo "./run_decoding_workflow.sh /home/flp/data/h1/analog data-d0-2021_03_04__17_26_26__-20490-0.raw"
    exit 1
fi

INPUT_PATH=${args[0]}
RAWDATA_FILENAME=${args[1]}


CFGFILE=MFTraw.cfg
touch $CFGFILE
echo "[input-MFT-0]">>$CFGFILE
echo "dataOrigin = MFT" >>$CFGFILE
echo "dataDescription = RAWDATA" >>$CFGFILE
echo "filePath = "${INPUT_PATH}"/"${RAWDATA_FILENAME} >> $CFGFILE
echo "Creation of "${CFGFILE}" : Done! "
echo "-----------------------------"
cat $CFGFILE
echo "-----------------------------"
echo "Run decoder RDHv6 on : "${INPUT_PATH}"/"${RAWDATA_FILENAME}
timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
mkdir -pv ${INPUT_PATH}/log/
mkdir -pv ${INPUT_PATH}/digits/
log=${INPUT_PATH}/log/decoder_$timestamp.out
ROOTFILEPATH=${INPUT_PATH}/digits/
#o2-raw-file-reader-workflow -b --delay 0.1 --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --detect-tf0 --configKeyValues 'HBFUtils.nHBFPerTF=128' --input-conf $CFGFILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --writeHW --no-clusters --no-cluster-patterns --decoder-verbosity 1 | o2-itsmft-digit-writer-workflow -b --runmft --writeHW --disable-mc --outfile $ROOTFILEPATH$1.root > $log
o2-raw-file-reader-workflow -b --delay 0.1 --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --detect-tf0 --input-conf $CFGFILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --writeHW --no-clusters --no-cluster-patterns | o2-itsmft-digit-writer-workflow -b --runmft --writeHW --disable-mc --outfile $ROOTFILEPATH.root > $log
rm -f $CFGFILE
echo "Log in : " $log
echo "Digits in : " $ROOTFILEPATH
echo "Error Jump in RDH_packetCounter :  " 
grep -r "Jump in RDH_packetCounter" $log | wc -l
echo "[ERROR]"
grep -r "[ERROR]" $log | wc -l
echo "Done !"
echo "-----------------------------"
