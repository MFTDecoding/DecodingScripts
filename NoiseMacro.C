#include <stdio.h>
#include <math.h>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TMath.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace o2::base;
using namespace o2::detectors;
using o2::itsmft::Digit;
using o2::itsmft::ROFRecord;

void NoiseMacro(const Char_t *inFile="/local/home/mc262512/alice/output/data-2021_03_10__16_59_27__-20490-2.raw.root"){

   int nb = 12;
  std::ostringstream os_file;
  string os1_file;

   TChain chain("o2sim");
   os_file<<"/local/home/mc262512/alice/output/data-2021_03_12__16_24_08__-20490-0.raw.root";
   os1_file = os_file.str();
   os_file.str("");
   os_file.clear();
   chain.Add(os1_file.c_str());

  auto nentries=chain.GetEntries();
  cout << "Nb of entries : " << nentries << endl;
  double ROFs = ((double) nentries)*128./(12.*10.);
  cout << "ROFs : " << ROFs << endl;
  UShort_t chipid;
  std::vector<o2::itsmft::Digit>* digArr = nullptr;
  std::vector<o2::itsmft::ROFRecord>* ROFArr = nullptr;
  chain.SetBranchAddress("MFTDigit", &digArr);
  chain.SetBranchAddress("MFTDigitROF", &ROFArr);

  vector<int> vecChipDec;
  vector<string> vecHistoName;

  std::ostringstream os;
  string os1;

 cout << "Entries : " << nentries << endl; 
 for(int i=0;i<nentries;i++){
   chain.GetEvent(i);
   Int_t nd = digArr->size();
   while (nd--) {
     const Digit* d = &(*digArr)[nd];
     Int_t chipID = d->getChipIndex();
     if(find (vecChipDec.begin(), vecChipDec.end(), chipID)==vecChipDec.end()){
	     vecChipDec.push_back(chipID);
     }
   }
 }

  int sizeVecDec = vecChipDec.size();
  vector<string> vecPixel[sizeVecDec];
  vector<int> vecHits[sizeVecDec];
  int vecROF[sizeVecDec];
  for (int i=0; i<sizeVecDec; i++){
	  vecROF[i]=0;
  }

  std::ostringstream os2;
  string os3;



  std::cout<<"How many chips? "<<sizeVecDec<<std::endl;

  int count =0;

  for(int i=0;i<nentries;i++){
    chain.GetEvent(i);
    Int_t nd = digArr->size();
    count +=nd;
    while (nd--) {
      const Digit* d2 = &(*digArr)[nd];
      Int_t chipID = d2->getChipIndex();
      for(int j=0;j<sizeVecDec;j++){
        if (chipID==vecChipDec[j]){
  	     os<< d2->getColumn() <<";"<< d2->getRow();
	     os1 = os.str();
	     os.str("");
	     os.clear();
	     
          auto it = find (vecPixel[j].begin(), vecPixel[j].end(), os1);
          if(it==vecPixel[j].end()){
	     vecPixel[j].push_back(os1);
	     vecHits[j].push_back(1);
	  }else{
	     vecHits[j][it - vecPixel[j].begin()]+=1;
	  }
        }
      }
    }
  }

  std::ofstream myfile;
  myfile.open ("noisy_pixels.csv");
  vector<int> vecNoise;
  vector<int> vecTriggers;
  for(int i=0; i<sizeVecDec;i++){
	  int sizeInput = vecPixel[i].size();
	  for(int j=0; j<sizeInput; j++)
	  {
		 vecNoise.push_back(vecHits[i][j]);
		 myfile << vecChipDec[i]<< "," << vecPixel[i][j] << "," << vecHits[i][j] << "\n";
	  }
  }
  int nb_pixels = vecNoise.size();
//  sort(vecNoise.begin(), vecNoise.end()); 
      myfile.close();
}

