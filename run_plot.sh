#!/usr/bin/env bash

# Run this script to make 2D histo plots with the coordinates (row, col)
#   of the hit pixels read from the digits. It will produce one plot
#   per sensor found in the data.
# The script takes 3 arguments:
# - the input digit base file name i.e. without the .root at the end 
# - the absolute path of the input digit file
# - the absolute path of the output PDF plots (one per sensor 
#   found in the data) 

args=("$@")
NUMBER_OF_ARGS=3

if [ $# -ne ${NUMBER_OF_ARGS} ]; then
    echo "Not enough args ! "
    echo "Mandatory: please give DIGIT_FILENAME INPUT_PATH OUTPUT_PATH"
    echo "as in the example below :"
    echo "./run_plot.sh data-2021_03_10__18_23_15__-20551-4 /home/flp/data/digits /home/flp/plots"
    exit 1
fi

DIGIT_FILENAME=${args[0]}
INPUT_PATH=${args[1]}
OUTPUT_PATH=${args[2]}

PATHLOG=${OUTPUT_PATH}/log
if [ ! -d "${PATHLOG}" ]; then
    mkdir -p ${PATHLOG}
fi

echo "-----------------------------"
START_TIME=$(date +"%Y/%m/%d %H:%M:%S")

timestamp=$(date +"%Y_%m_%d_%H_%M_%S")
log=${PATHLOG}/plotter-${RAWDATA_FILENAME}-$timestamp.out

root -l -b -q macro/plot.C\(\"${DIGIT_FILENAME}\",\"${INPUT_PATH}\",\"${OUTPUT_PATH}\"\) > $log

echo "Log in : " $log
echo "Plots in : " 
ls -1 ${OUTPUT_PATH}/${DIGIT_FILENAME}/*.pdf
echo

echo "ERROR opening digit file : "
grep "ERROR opening digit file" $log | wc -l
echo

STOP_TIME=$(date +"%Y %m %d %H:%M:%S")
echo "Stop " ${STOP_TIME}
echo "Done !"
echo "-----------------------------"
