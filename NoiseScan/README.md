# Tools for noise calibration

### `root readCCDB.C` 
displays the list of noisy pixels in the last NoiseMap object written in the CCDB. It can also displays any CCDB entry provided the correct timestamp with a command such as : `root -b -q "readCCDB.C(1625066988580)"` (a timestamp=-1 means the latest entry in the CCDB).

### `root -b -q -l 'ChangeName.C("noise_1625066988580.root")'` 
takes as input the name of a root file downloaded locally from the CCDB containing a NoiseMap object and writes in output a file noise.root which contains the same NoiseMap but withe the correct name ("Noise") to be used by the decoder to mask noisy pixels.

### `root -b -q -l "DiffNoise.C(-1,1625066988580)"` 
takes as input the timestamps of two NoiseMap objects in the CCDB to be comapred. The macro will output the list per zone of noisy pixels that are new and the noisy pixels that diseappeared. 

### `root -b NoisePlotDisk.C` 
plots the content of the latest NoiseMap object in the CCDB in the usual half-disk 2D histograms.

