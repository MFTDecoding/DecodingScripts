#!/usr/bin/env bash

function go_o2() {
    # enter O2 environment
    module load O2PDPSuite
}
export -f go_o2

function go_eos() {
    # needed to be able to acces EOS files
    export XrdSecPROTOCOL=sss,unix
    export XrdSecSSSKT=~/.alimftvs.sss.keys
}
export -f go_eos

function groupfiles () {
    # create a subgroup of files for reco
    # from a list of files, obtained for e.g. with:
    #      xrdfs root://eosaliceo2.cern.ch ls /eos/aliceo2/ls2data/GC/ECS/compressed/2TkZ5uNgEvW_502879 > ctflist
    # use:
    #      FilesPerGroup=15 groupfiles ctflist
    # alternatively, use the bash program split instead:
    #      split -l 15 -d ctflist group_
    FilesPerGroup=${FilesPerGroup:-"50"}
    GroupCounter=0
    FileCounter=0
    rm group_*
    for file in $(cat $@)
    do
	echo ${file} >> group_${GroupCounter}
	((FileCounter++))

	if [ ${FileCounter} -eq ${FilesPerGroup} ]
	then
            ((GroupCounter++))
            FileCounter=0
	fi          
    done
}
export -f groupfiles

function go_tf_reco() {
    # read TF file(s), run reco workflow on these files, output tracks and cluster ROOT files
    # with a list of TF files obtained from for e.g.
    #      xrdfs root://eosaliceo2.cern.ch ls /eos/aliceo2/ls2data/GC/ECS/raw/2021-10-01_18-29/run0502879_2021-10-01T18_30_45Z > tflist
    # use:
    #      go_tfreco tflist
    inputfile=${@}
    prefix=${@}

#    o2-raw-tf-reader-workflow   --shm-segment-size 15000000000 --input-data ${inputfile} --onlyDet MFT |  o2-itsmft-stf-decoder-workflow  --shm-segment-size 15000000000 --nthreads 8 --runmft --digits --no-clusters --no-cluster-patterns | o2-mft-reco-workflow --mft-track-writer "--outfile mfttracks_${prefix}.root" --mft-cluster-writer "--outfile mftclusters_${prefix}.root" --shm-segment-size 15000000000 --digits-from-upstream --disable-mc --configKeyValues "MFTTracking.FullClusterScan=true;  MFTTracking.MinTrackPointsLTF=3; MFTTracking.MinTrackStationsLTF=1; MFTTracking.LTFclsRCut=0.5; MFTTracking.LTFseed2BinWin=6; MFTTracking.MinTrackStationsCA=1; MFTTracking.ROADclsRCut=0.5; MFTTracking.MinTrackPointsCA=3;" --run | tee decode_and_reco_TF${prefix}.log
    o2-raw-tf-reader-workflow   --shm-segment-size 15000000000 --input-data ${inputfile} --onlyDet MFT |  o2-itsmft-stf-decoder-workflow  --shm-segment-size 15000000000 --nthreads 8 --runmft --digits --no-clusters --no-cluster-patterns | o2-mft-reco-workflow --mft-track-writer "--outfile mfttracks_${prefix}.root" --mft-cluster-writer "--outfile mftclusters_${prefix}.root" --shm-segment-size 15000000000 --digits-from-upstream --disable-mc --configKeyValues "MFTTracking.FullClusterScan=true;  MFTTracking.MinTrackPointsLTF=3; MFTTracking.MinTrackStationsLTF=1; MFTTracking.LTFclsRCut=0.5; MFTTracking.LTFseed2BinWin=6; MFTTracking.MinTrackStationsCA=1; MFTTracking.ROADclsRCut=0.5; MFTTracking.MinTrackPointsCA=3;" -b --run > decode_and_reco_TF${prefix}.log && mv ${inputfile} done_${inputfile}
}
export -f go_tf_reco

function go_tf_to_digits() {
    # read TF file(s), run decoder on these files, output digit ROOT files
    # with a list of TF files obtained from for e.g.
    #      xrdfs root://eosaliceo2.cern.ch ls /eos/aliceo2/ls2data/GC/ECS/raw/2021-10-01_18-29/run0502879_2021-10-01T18_30_45Z > tflist
    # use:
    #      go_tf_to_digits tflist
    inputfile=${@}
    prefix=${@}

    o2-raw-tf-reader-workflow   --shm-segment-size 15000000000 --input-data ${inputfile} --onlyDet MFT |  o2-itsmft-stf-decoder-workflow  --shm-segment-size 15000000000 --nthreads 8 --runmft --digits --no-clusters --no-cluster-patterns | o2-itsmft-digit-writer-workflow  --disable-mc --outfile mftdigits_${prefix}.root -b --runmft > decode_and_digits_TF${prefix}.log && mv ${inputfile} done_${inputfile}
}
export -f go_tf_to_digits

function go_ctf_reco() {
    # read CTF file(s), run reco workflow on these files, output tracks and cluster ROOT files
    # with a list of CTF files obtained from for e.g.
    #      xrdfs root://eosaliceo2.cern.ch ls /eos/aliceo2/ls2data/GC/ECS/compressed/2TkZ5uNgEvW_502879 > ctflist
    # use:
    #      go_ctf_reco ctflist
    inputfile=${@}
    MINCLUSTERS=${MINCLUSTERS:-"4"}
    ROADRADIUS=${ROADRADIUS:-"0.1"}
    prefix=${@}_${MINCLUSTERS}cls_${ROADRADIUS}road

    o2-ctf-reader-workflow --onlyDet MFT --delay 0.1 --ctf-input ${inputfile}  | o2-mft-reco-workflow --mft-track-writer "--outfile mfttracks_${prefix}.root" --mft-cluster-writer "--outfile mftclusters_${prefix}.root" --shm-segment-size 15000000000 --clusters-from-upstream --disable-mc --configKeyValues "MFTTracking.FullClusterScan=true;  MFTTracking.MinTrackPointsLTF=${MINCLUSTERS}; MFTTracking.MinTrackStationsLTF=1; MFTTracking.LTFclsRCut=${ROADRADIUS}; MFTTracking.LTFseed2BinWin=6; MFTTracking.MinTrackStationsCA=1; MFTTracking.ROADclsRCut=${ROADRADIUS}; MFTTracking.MinTrackPointsCA=${MINCLUSTERS};" -b --run > reco_${prefix}.log && mv ${inputfile} done_${inputfile}
}

export -f go_ctf_reco

function go_cluster_reco() {
    # read cluster file(s), run reco workflow on these clusters, output tracks and cluster ROOT files
    # use:
    #     go_cluster_reco file_with_clusterfilelist
    inputfile=${@}
    MINCLUSTERS=${MINCLUSTERS:-"4"}
    ROADRADIUS=${ROADRADIUS:-"0.5"}
    prefix=${@}_${MINCLUSTERS}cls_${ROADRADIUS}road

    o2-mft-cluster-reader-workflow --shm-segment-size 15000000000 --mft-cluster-infile ${inputfile}  | o2-mft-reco-workflow --mft-track-writer "--outfile mfttracks_${prefix}.root" --mft-cluster-writer "--outfile /dev/null" --shm-segment-size 15000000000 --clusters-from-upstream --disable-mc --configKeyValues "MFTTracking.FullClusterScan=true;  MFTTracking.MinTrackPointsLTF=${MINCLUSTERS}; MFTTracking.MinTrackStationsLTF=1; MFTTracking.LTFclsRCut=${ROADRADIUS}; MFTTracking.LTFseed2BinWin=10; MFTTracking.MinTrackStationsCA=1; MFTTracking.ROADclsRCut=${ROADRADIUS}; MFTTracking.MinTrackPointsCA=${MINCLUSTERS};" -b --run > reco_${prefix}.log 
}
export -f go_cluster_reco

