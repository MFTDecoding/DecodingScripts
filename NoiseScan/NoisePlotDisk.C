


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

int AddressToColumn      (int disk, int tr, int zone, int col, int row)
{
    int connector;
    if (tr==5 || tr==6 || tr==7 || tr==24 || tr==23){
            connector = 0;
    }else if (tr==0 || tr==1 || tr==2 || tr==3 || tr==4){
            connector = 1;
    }else if (tr==17 || tr==16 || tr==15 || tr==14 || tr==13){
            connector = 2;
    }else if (tr==22 || tr==21 || tr==20 || tr==19 || tr==18){
            connector = 3;
    }else if (tr==12 || tr==11 || tr==10 || tr==9 || tr==8){
            connector = 4;
    }
    static constexpr Int_t ZoneRUType[4][5]{
    {1, 1, 1, 7, 11},
    {2, 2, 4, 8, 9},
    {2, 2, 3, 8, 10},
    {0, 0, 5, 6, 12}};
    std::array<int, 13> NChipsOnRUType{3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 4};
    int type;
    int nb_connectors=0;
    for (int z=0; z<zone; z++){
        type = ZoneRUType[z % 4][disk];
        nb_connectors+=(NChipsOnRUType[type]);
    }

//    cout << "Nb connectors : " << nb_connectors << endl;
    int Column    =  (connector+nb_connectors) * 512 + row;

    return Column;
}

int AddressToRow         (int disk, int tr, int zone, int col, int row)
{
    int chipID;
    if (tr==5 || tr==0 || tr==17 || tr==22 || tr==12){
            chipID = 0;
    }else if (tr==6 || tr==1 || tr==16 || tr==21 || tr==11){
            chipID = 1;
    }else if (tr==7 || tr==2 || tr==15 || tr==20 || tr==10){
            chipID = 2;
    }else if (tr==24 || tr==3 || tr==14 || tr==19 || tr==9){
            chipID = 3;
    }else if (tr==23 || tr==4 || tr==13 || tr==18 || tr==8){
            chipID = 4;
    }
    int Row = (5-chipID)*1024-col;
    return Row;
}


void NoisePlotDisk(long timestamp=-1){

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
				     Int_t tr = chipMap[id].cable;
				     Int_t half = chipMap[id].half;
				     Int_t layer = chipMap[id].layer;
				     Int_t face = layer % 2;
				     Int_t disk = chipMap[id].disk;
				     Int_t zone = chipMap[id].zone;

				     vecChip.push_back({half, disk, face, zone, tr, row, col, lvl});

					empty=false;
				}
			}
		}
		if (!empty){
			nb_chips++;
		}
	}		
//	sort(vecChip.begin(), vecChip.end(), compareInterval);
	int nb_pixels = vecChip.size();
	cout << nb_pixels << endl;
	
  TCanvas *c1[20];
  TH2F *hplot[20];
  int countz[20][4];
    for(int i=0;i<20;i++){
	    for (int f=0; f<4; f++){
		    countz[i][f]=0;
	    }
	    string histoname;
	    int f = i%2;
	    if (i < 10){
		    histoname = "h0-d"+to_string((i-f)/2)+"-f"+to_string(f);
	    }else{
		    histoname = "h1-d"+to_string((i-f)/2-5)+"-f"+to_string(f);
	    }
    	hplot[i] = new TH2F(histoname.c_str(),histoname.c_str(),17*512,-0.5,17*512-0.5,5*1024,-0.5,5*1024-0.5);
     }


	for (int i=0; i<nb_pixels; i++){
	        int Column = AddressToColumn(vecChip[i].disk,vecChip[i].tr,vecChip[i].zone,vecChip[i].col,vecChip[i].row);
	        int Row = AddressToRow(vecChip[i].disk,vecChip[i].tr,vecChip[i].zone,vecChip[i].col,vecChip[i].row);
		int index = 10*vecChip[i].half+2*vecChip[i].disk+vecChip[i].face;
		hplot[index]->Fill(Column, Row, vecChip[i].noise);
		countz[index][vecChip[i].zone]++;

	}
	cout << '\n' << "Noisy pixels per zone : " << endl;
	for(int k=0;k<20;k++){   //to have the plot
	    int face = k%2;
		for(int f=0; f<4; f++){
		    if (k < 10){
			    cout << '\t' << "h0-d" << (k-face)/2 << "-f" << face << "-z" << f << " : " << countz[k][f] << endl;
		    }else{
			    cout << '\t' << "h1-d"<< (k-face)/2-5 << "-f" << face << "-z" << f <<" : " << countz[k][f] << endl;;
	    	}
		}
	}

	for(int k=0;k<20;k++){   //to have the plot
	    c1[k]= new TCanvas();
	    c1[k]->cd(k);
	    gStyle->SetOptStat(0);
	    hplot[k]->Draw("colz");
	}



}
