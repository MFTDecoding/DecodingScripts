void ChangeName(std::string file_name="noise_1631202922451.root"){
    TFile* f = new TFile(file_name.c_str());
    auto pnoise = (o2::itsmft::NoiseMap*)f->Get("ccdb_object");
    TFile * out = new TFile("noise.root", "recreate");
    out->cd();
    gDirectory->WriteObject(pnoise,"Noise");
    out->Close();

}
