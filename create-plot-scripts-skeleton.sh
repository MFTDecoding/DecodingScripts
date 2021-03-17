#!/usr/bin/env bash
# Copy and execute this script from /home/mft/data
#
# It will create skeleton of bash scripts to run the 
#   plot script for the listed files.
#   Each obtained .sh file is chmod+x so they can be executed
#   via plot-flp.sh
# Edit the skeleton script to make them run (remove
#   .raw at the end of each file name)
ls -1 data-2021_03_12__16_24_08__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-noise-scan.sh
ls -1 data-2021_03_12__18_28_10__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-source-long-scan-nobb.sh
ls -1 data-2021_03_12__19_29_35__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-source-long-scan-wbb.sh
#ls -1 data-2021_03_10__16_59_27__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-noise-scan.sh
#ls -1 data-2021_03_10__17_38_49__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-source-short-scan-low-intensity.sh
#ls -1 data-2021_03_10__18_03_41__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-source-long-scan-low-intensity.sh
#ls -1 data-2021_03_10__18_23_15__-20* | awk '{print "./run_plot.sh "$1" /home/mft/data/digits /home/mft/plots"}' > ~/alice/decoding-scripts/plot-source-short-scan-high-intensity.sh
chmod u+x ~/alice/decoding-scripts/*.sh
