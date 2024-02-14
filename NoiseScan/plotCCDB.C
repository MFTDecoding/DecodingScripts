void plotCCDB(){
	o2::ccdb::CcdbApi api;
	api.init("http://alice-ccdb.cern.ch");
	map<string, string> headers;
	map<std::string, std::string> filter;
	auto calib = api.retrieveFromTFileAny<o2::itsmft::NoiseMap>("MFT/Calib/NoiseMap/", filter , -1, &headers );
	float thresh = 0;
//	calib->dumpAboveThreshold(thresh);
	vector<int> vecNoise;
	int nb_chips=0;
	for (int id=0; id<936; id++){
		bool empty=true;
		for (int row=0; row<512; row++){
			for (int col=0; col<1024; col++){
				int lvl = calib->getNoiseLevel(id, row, col);
//				if (lvl && id > 467){
				if (lvl && id < 468){
//					cout << "Noise level (id, row col, noise) : " << id << ", " << row << ", " << col << ", " << lvl << endl;
					vecNoise.push_back(lvl);
					empty=false;
				}
			}
		}
		if (!empty){
			nb_chips++;
		}
	}		
	cout << "Number of strobes : " << calib->getNumOfStrobes() << endl;
	  int ROFs = calib->getNumOfStrobes();
	  int nb_pixels = vecNoise.size();
//	  int sizeVecDec = 936;
//	  int sizeVecDec = 468;
	  int sizeVecDec = nb_chips;
	  cout << "Number of noisy pixels : " << nb_pixels << endl;
  double occupancy[nb_pixels];
  double masked_pixels[nb_pixels];
  sort(vecNoise.begin(), vecNoise.end());
  int total = 0;
  int index_99=0;
  int index_10=0;
  int index_01=0;
  int index_101=0;
  int index_3=0;
  int index_2=0;
  cout << "Tot nb pixels : " << nb_pixels << endl;
  for(int i=0; i<nb_pixels; i++){
          total+=vecNoise[i];
          if (vecNoise[i]<2){
                  index_2=nb_pixels-i-1;
          }else if (vecNoise[i]==2){
                  index_3=nb_pixels-i-1;
          }else if (vecNoise[i]>2 && vecNoise[i] < 101){
                  index_101=nb_pixels-i-1;
          }else if (vecNoise[i]>100 && vecNoise[i] < 0.001*ROFs){
                  index_01=nb_pixels-i-1;
          }else if (vecNoise[i]>0.001*ROFs && vecNoise[i] < 0.1*ROFs){
                  index_10=nb_pixels-i-1;
          }else if (vecNoise[i]>0.1*ROFs && vecNoise[i] < 0.99*ROFs){
                  index_99=nb_pixels-i-1;
          }
          double tot_pixels = ROFs*1024.*512.*((double) sizeVecDec);
          occupancy[nb_pixels-i-1]= (double) total / (double) tot_pixels;
	  cout << "Occupancy : " << occupancy[nb_pixels-i-1] << endl;
          masked_pixels[i]=i;
  }
  cout << "index 2 : " << index_2 << ", 3 : " << index_3 << ", 101 : " << index_101 << ", 01 : " << index_01 << ", 10 : " << index_10 << ", 99 : " << index_99 << endl;
//  cout << "Nb pixels w/ 1 hit: " << nb_pixels-index_2 << endl;
//  cout << "Nb pixels w/ 2 hit: " << index_2-index_3 << endl;
//  cout << "Nb pixels w/ 3-100 hit: " << index_3-index_101 << endl;
  cout << "Nb pixels w/ 101-0.1\% hit: " << nb_pixels-index_01 << endl;
  cout << "Nb pixels w/ 0.1\%-10\% hit: " << index_01-index_10 << endl;
  cout << "Nb pixels w/ 10\%-99\% hit: " << index_10-index_99 << endl;
  cout << "Nb pixels w/ >99\% hit: " << index_99 << endl;
     TGraph *grshade_1 = new TGraph(2*index_99);
   grshade_1->SetName("grshade_1");
   for (int i=0;i<index_99;i++) {
           grshade_1->SetPoint(i,masked_pixels[i],occupancy[i]);
           grshade_1->SetPoint(index_99+i,masked_pixels[index_99-i-1],0);
   }
   grshade_1->SetFillStyle(3001);
   grshade_1->SetFillColor(41);
   TGraph *grshade_01 = new TGraph(2*(nb_pixels-index_01));
   grshade_01->SetName("grshade_01");
   for (int i=0;i<(nb_pixels-index_01);i++) {
           grshade_01->SetPoint(i,masked_pixels[i+index_01],occupancy[i+index_01]);
           grshade_01->SetPoint((nb_pixels-index_01)+i,masked_pixels[nb_pixels-i-1],0);
   }
   grshade_01->SetFillStyle(3001);
   grshade_01->SetFillColor(5);
   TGraph *grshade_10 = new TGraph(2*(index_01-index_10));
   grshade_10->SetName("grshade_10");
   for (int i=0;i<(index_01-index_10);i++) {
           grshade_10->SetPoint(i,masked_pixels[i+index_10],occupancy[i+index_10]);
           grshade_10->SetPoint((index_01-index_10)+i,masked_pixels[index_01-i-1],0);
   }
   grshade_10->SetFillStyle(3001);
   grshade_10->SetFillColor(6);
   TGraph *grshade_99 = new TGraph(2*(index_10-index_99));
   grshade_99->SetName("grshade_99");
   for (int i=0;i<(index_10-index_99);i++) {
           grshade_99->SetPoint(i,masked_pixels[i+index_99],occupancy[i+index_99]);
           grshade_99->SetPoint((index_10-index_99)+i,masked_pixels[index_10-i-1],0);
   }
   grshade_99->SetFillStyle(3001);
   grshade_99->SetFillColor(7);

   TGraph *gr  = new TGraph(nb_pixels,masked_pixels,occupancy);
   gr->SetTitle("; # masked pixels; Noise occupancy per pixel per event");
   gr->SetName("gr");
   gr->SetMarkerColor(2);
   gr->SetLineColor(2);
   gr->SetLineWidth(3);
   gr->SetMarkerSize(0.5);
   gr->SetMarkerStyle(7);

   auto c = new TCanvas("c", "c", 750, 500);

   gr->Draw("AC*");
   gr->GetYaxis()->SetRangeUser(1e-13,5e-6);
   gr->GetXaxis()->SetRangeUser(1,1e5);
   grshade_1->Draw("f");
   grshade_01->Draw("f");
   grshade_10->Draw("f");
   grshade_99->Draw("f");

   auto legend = new TLegend(0.6, 0.7, 0.9, 0.9);
   legend->AddEntry("grshade_01", Form("(101 - 0.1 \%) : %d pixels",nb_pixels-index_01), "f");
   legend->AddEntry("grshade_10", Form("(0.1 \% - 10 \%) : %d pixels",index_01-index_10), "f");
   legend->AddEntry("grshade_99", Form("(10 \% - 99 \%) : %d pixels",index_10-index_99), "f");
   legend->AddEntry("grshade_1", Form("(>99 \%) : %d pixels",index_99), "f");
   legend->Draw();




}
