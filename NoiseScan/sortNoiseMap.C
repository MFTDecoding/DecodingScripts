


struct Chip {
    int half, disk, face, zone, tr, row, col, noise;
};

bool compareInterval(Chip i1, Chip i2)
{
	if (i1.half < i2.half){
		return true;
	}else if (i1.half == i2.half){
	if (i1.disk < i2.disk){
		return true;
	}else if (i1.disk == i2.disk){
		if (i1.face < i2.face){
			return true;
		}else if (i1.face == i2.face){
			if (i1.zone < i2.zone){
				return true;
			}else if (i1.zone == i2.zone){
				if (i1.tr < i2.tr){
					return true;
				}else{
					return false;
				}
			}else{
				return false;
			}
		}else{
			return false;
		}
	
	}else{
		return false;
	}
	}else{
		return false;
	}
}

void sortNoiseMap(long timestamp=-1){

	const o2::itsmft::ChipMappingMFT maping;
	auto chipMap = maping.getChipMappingData();

	o2::ccdb::CcdbApi api;
	api.init("ccdb-test.cern.ch:8080");
	map<string, string> headers;
	map<std::string, std::string> filter;
	auto calib = api.retrieveFromTFileAny<o2::itsmft::NoiseMap>("MFT/Calib/NoiseMap/", filter , timestamp, &headers );
	float thresh = 0;
//	calib->dumpAboveThreshold(thresh);
	vector<int> vecNoise;
	vector<Chip> vecChip;
	int nb_chips=0;
	for (int id=0; id<936; id++){
		bool empty=true;
		for (int row=0; row<512; row++){
			for (int col=0; col<1024; col++){
				int lvl = calib->getNoiseLevel(id, row, col);
				if (lvl){
				     Int_t cableHW = chipMap[id].cable;
				     Int_t half = chipMap[id].half;
				     Int_t layer = chipMap[id].layer;
				     Int_t face = layer % 2;
				     Int_t disk = chipMap[id].disk;
				     Int_t zone = chipMap[id].zone;

				     vecChip.push_back({half, disk, face, zone, cableHW, row, col, lvl});

					empty=false;
				}
			}
		}
		if (!empty){
			nb_chips++;
		}
	}		
	sort(vecChip.begin(), vecChip.end(), compareInterval);
	int nb_pixels = vecChip.size();
	cout << nb_pixels << endl;

	std::ofstream OutStream; 
	OutStream.open("noiseMap_sorted.dat");
	OutStream << "half \t disk \t face \t zone \t tr \t row \t col "<< endl; 
	
  for (int i=0; i<nb_pixels; i++){
	  OutStream << vecChip[i].half << "\t" << vecChip[i].disk << "\t" << vecChip[i].face << "\t" << vecChip[i].zone << "\t" << vecChip[i].tr << "\t" << vecChip[i].row << "\t" << vecChip[i].col << "\t" << vecChip[i].noise << endl; 
  }
	OutStream.close();

}
