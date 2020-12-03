#!/bin/bash
FILE=MFTraw.cfg
touch $FILE
echo "[input-MFT-0]">>$FILE
echo "dataOrigin = MFT" >>$FILE
echo "dataDescription = RAWDATA" >>$FILE
echo "filePath = /root/alice/data/"$1 >> $FILE
echo "Creation of MFTraw.cfg : Done! "
echo "-----------------------------"
echo "Run decoder RDHv6"
timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
log=/root/alice/output/log/decode_$timestamp.out
ROOTFILEPATH=/root/alice/output/
o2-raw-file-reader-workflow -b --nocheck-missing-stop --nocheck-starts-with-tf --nocheck-packet-increment --nocheck-hbf-jump --nocheck-hbf-per-tf --input-conf $FILE | o2-itsmft-stf-decoder-workflow -b --runmft --digits --writeHW --no-clusters --no-cluster-patterns --decoder-verbosity 1 | o2-itsmft-digit-writer-workflow -b --runmft --writeHW --disable-mc --outfile $ROOTFILEPATH$1.root > $log
rm -f $FILE
echo "Error Jump in RDH_packetCounter :  " 
grep -r "Jump in RDH_packetCounter" $log | wc -l
echo "Done !"
