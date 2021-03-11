#include <stdio.h>
#include <math.h>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TMath.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH2F.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace o2::base;
using namespace o2::detectors;
using o2::itsmft::DigitHW;

void plot(const std::string base_file_name = "data-d0-2021_03_04__17_26_26__-20490-3", 
  const std::string input_path = "/home/flp/data/digits",
  std::string output_path = "/home/flp/plots")
{
  std::cout << "---------------------------------------" << std::endl;
  std::string input_file_name = input_path + "/" + base_file_name + ".root";

  TFile *inputFile =new TFile(input_file_name.c_str());
  if ( inputFile->IsZombie() ) {
		std::cout << "ERROR opening digit file " << input_file_name << " ! exit" << std::endl;
		return;
  }
  std::cout << "digit file " << input_file_name <<" opened" << std::endl;

  TTree *tree=(TTree*)inputFile->Get("o2sim");
  auto nentries=tree->GetEntries();
  std::vector<o2::itsmft::DigitHW>* digArr = nullptr;
  tree->SetBranchAddress("MFTDigit", &digArr);

  vector<int> vecChipDec;
  vector<string> vecHistoName;

  std::ostringstream os;
  string os1;

  
 for(int i=0;i<nentries;i++){
   tree->GetEvent(i);
   Int_t nd = digArr->size();
   while (nd--) {
     const DigitHW* d = &(*digArr)[nd];
     Int_t chipID = d->getChipIndex();
     Int_t cableHW = d->getCableHW();
     Int_t half = d->getHalf();
     Int_t face = d->getPlane();
     Int_t disk = d->getDisk();
     Int_t zone = d->getZone();
     if(find (vecChipDec.begin(), vecChipDec.end(), chipID)==vecChipDec.end()){
  	     os<<"h" << half<<"-d"<<disk<<"-f"<<face<<"-z"<<zone<<"-tr"<<cableHW;
	     os1 = os.str();
	     os.str("");
	     os.clear();
	     vecChipDec.push_back(chipID);
	     vecHistoName.push_back(os1);
     }
   }
 }

  int sizeVecDec = vecChipDec.size();
  std::cout<<"How many chips? "<<sizeVecDec<<std::endl;
  for(int ichip = 0; ichip<sizeVecDec; ichip++){
    std::cout << "\t" << vecHistoName[ichip] << std::endl;
  }

  gStyle->SetPalette(55); // Rainbow
 
  TCanvas *c1[sizeVecDec];
  TH2F *hplot[sizeVecDec];
  char *histoname = new char[sizeVecDec];

  for(int i=0;i<sizeVecDec;i++){
    hplot[i] = new TH2F(histoname,histoname,1024,0,1024,512,0,512);
    TString os1=vecHistoName[i]+";Cols;Rows";
    TString os2=vecHistoName[i];
    hplot[i]->SetName(os2);
    hplot[i]->SetTitle(os1);
     hplot[i]->GetName();
     hplot[i]->GetTitle();
 }

  for(int i=0;i<nentries;i++){
    tree->GetEvent(i);
    Int_t nd = digArr->size();
    if(nd>0) {
      std::cout << ">>>>> Entry " << i+1 << "/" << nentries << " Events ? " << nd << "\r "<< std::flush;
    }
 
    while (nd--) {
      const DigitHW* d2 = &(*digArr)[nd];
      Int_t chipID = d2->getChipIndex();
      for(int j=0;j<sizeVecDec;j++){
        if (chipID==vecChipDec[j]){
          hplot[j]->Fill(d2->getColumn(), d2->getRow());
        }
      }
    }
  }
  std::cout << "" << std::endl;
  output_path += "/" + base_file_name;
  std::string command = "mkdir -p " + output_path;
  std::cout << command << std::endl;
  gSystem->Exec(command.c_str());

  for(int k=0;k<sizeVecDec;k++){   //to have the plot
    TString os2=vecHistoName[k];
    c1[k]= new TCanvas();
    c1[k]->SetName(os2);
    c1[k]->SetTitle(os2);
    c1[k]->GetName();
    c1[k]->GetTitle();
    c1[k]->cd(k);
    gStyle->SetOptStat(0);
    hplot[k]->Draw("colz PMC");
    std::string histnamesave = output_path + "/";
    histnamesave += os2;
    histnamesave += ".pdf";
    const char *finalname =  histnamesave.c_str();
    c1[k]->SaveAs(finalname);
  }
  std::cout << "---------------------------------------" << std::endl;
}

