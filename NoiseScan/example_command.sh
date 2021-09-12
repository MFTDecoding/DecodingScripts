#!/bin/bash
root -b -q readCCDB.C
root -b -q -l 'ChangeName.C("test.root")'
root -b "DiffNoise.C(-1,1625066988580)"

