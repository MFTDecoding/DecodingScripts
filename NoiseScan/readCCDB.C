void readCCDB(long timestamp=-1){
	o2::ccdb::CcdbApi api;
	api.init("ccdb-test.cern.ch:8080");
	map<string, string> headers;
	map<std::string, std::string> filter;
	auto calib = api.retrieveFromTFileAny<o2::itsmft::NoiseMap>("MFT/Calib/NoiseMap/", filter , timestamp, &headers);
	float thresh = 1e-5;
	calib->dumpAboveThreshold(thresh);
	cout << "Number of strobes : " << calib->getNumOfStrobes() << endl;


}
