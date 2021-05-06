#!/usr/bin/env bash
# Copy and execute this script from the directory 
#   where the raw data files are, for e.g. /home/mft/data
#
# It will create skeleton of bash scripts to run the 
#   decoding worflow for the listed files.
#   Each obtained .sh file is chmod+x so they can be executed
#   via decode-flp.sh
# Edit the skeleton script to make them run (remove
#   .raw or .tf at the end of each file name)

ls -1 2021-05-03*.tf | awk '{print "./run_decoding_workflow.sh 0.001 0 /home/mft/data/mw4/151 256 " $1}' > ~/alice/decoding-scripts/decode-mc-hit-pattern.sh
#ls -1 2021-04-01*.tf | awk '{print "./run_decoding_workflow.sh 0.001 0 /home/mft/data/mw3/63 256 " $1}' > ~/alice/decoding-scripts/decode-mc-hit-pattern.sh
#ls -1 data-2021_03_12__16_24_08__-20* | awk '{print "./run_decoding_workflow.sh 0.001 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-noise-scan.sh
#ls -1 data-2021_03_12__18_28_10__-20* | awk '{print "./run_decoding_workflow.sh 0.01 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-source-long-scan-nobb.sh
#ls -1 data-2021_03_12__19_29_35__-20* | awk '{print "./run_decoding_workflow.sh 0.01 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-source-long-scan-wbb.sh
#
#ls -1 data-2021_03_10__16_59_27__-20* | awk '{print "./run_decoding_workflow.sh 0.001 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-noise-scan.sh
#ls -1 data-2021_03_10__17_38_49__-20* | awk '{print "./run_decoding_workflow.sh 0.01 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-source-short-scan-low-intensity.sh
#ls -1 data-2021_03_10__18_03_41__-20* | awk '{print "./run_decoding_workflow.sh 0.01 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-source-long-scan-low-intensity.sh
#ls -1 data-2021_03_10__18_23_15__-20* | awk '{print "./run_decoding_workflow.sh 0.01 /home/mft/data 128 " $1}' > ~/alice/decoding-scripts/decode-source-short-scan-high-intensity.sh
chmod u+x ~/alice/decoding-scripts/*.sh
