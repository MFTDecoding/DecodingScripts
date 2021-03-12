#!/usr/bin/env bash
#
# Execute the scripts whose skeleton was created 
# by create-plot-scripts-skeleton.sh
#
echo "***********************"
echo "Plot noise scan"
echo "***********************"
./plot-noise-scan.sh
echo
echo "**********************************************"
echo "Plot source short scan with low intensity"
echo "**********************************************"
./plot-source-short-scan-low-intensity.sh
echo
echo "**********************************************"
echo "Plot source long scan with low intensity"
echo "**********************************************"
./plot-source-long-scan-low-intensity.sh
echo
echo "**********************************************"
echo "Plot source short scan with high intensity"
echo "**********************************************"
./plot-source-short-scan-high-intensity.sh
echo
