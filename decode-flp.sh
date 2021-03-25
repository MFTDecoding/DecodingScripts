#!/usr/bin/env bash
#
# Execute the scripts whose skeleton was created 
# by create-decode-scripts-skeleton.sh
#
echo "***********************"
echo "Decoding noise scan"
echo "***********************"
./decode-noise-scan.sh
echo
echo "**********************************************"
echo "Decoding source long scan without back-bias"
echo "**********************************************"
./decode-source-long-scan-nobb.sh
echo
echo "**********************************************"
echo "Decoding source long scan with back-bias"
echo "**********************************************"
./decode-source-long-scan-wbb.sh
# echo
# echo "**********************************************"
# echo "Decoding source short scan with low intensity"
# echo "**********************************************"
# ./decode-source-short-scan-low-intensity.sh
# echo
# echo "**********************************************"
# echo "Decoding source long scan with low intensity"
# echo "**********************************************"
# ./decode-source-long-scan-low-intensity.sh
# echo
# echo "**********************************************"
# echo "Decoding source short scan with high intensity"
# echo "**********************************************"
# ./decode-source-short-scan-high-intensity.sh
echo
