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


using namespace o2::base;
using namespace o2::detectors;
using o2::itsmft::Digit;

vector <int> getChipDec()
{
  fstream inputDec; //read chip_ID.txt in order to have the number of chip in raw data file
  inputDec.open("/home/alice/alice/output/Chip_ID.txt", ios_base::in);
  vector <int> vecChipDec;
  int ChipDecod;
  int sizeVecDec=vecChipDec.size();
  while(inputDec >> ChipDecod)
    {
      vecChipDec.push_back(ChipDecod);
    }
  for(int n=0;n<sizeVecDec;n++)
    cout << vecChipDec[n] << endl;
  inputDec.close();
  return vecChipDec;
}

vector<int> getTrans()
{
  fstream inputTrans; //read chip_ID.txt in order to have the number of chip in raw data file
  inputTrans.open("/home/alice/alice/output/trans.txt", ios_base::in);
  vector <int> vecChipTrans;
  int ChipTransod;
  int sizeVecTrans=vecChipTrans.size();
  while(inputTrans >> ChipTransod)
    {
      vecChipTrans.push_back(ChipTransod);
    }
  for(int n=0;n<sizeVecTrans;n++)
    cout << vecChipTrans[n] << endl;
  inputTrans.close();
  return vecChipTrans;
}

void RawDec(const Char_t *inFile="/home/alice/alice/output/data-charge29.root",const Char_t *vpulselow="50"){

  TFile *inputFile =new TFile(inFile);
  vector<int> vecChipDec=getChipDec();
  int sizeVecDec=vecChipDec.size();
  std::cout<<"How many chips? "<<sizeVecDec<<std::endl;
  Int_t vpulsel=std::stoi(vpulselow);
  Int_t vpulseh=170;
  Int_t chargeinj=vpulseh-vpulsel;
  vector<int> vecTrans=getTrans();
  vector<vector<vector<short int>>> vecColRow(sizeVecDec);
  for(int i=0;i<sizeVecDec;i++){
    for(int col=0;col<1024;col++){
      std::vector<short int> vector1(512,0);
      vecColRow[i].push_back(vector1);
   }
  }
  TTree *tree=(TTree*)inputFile->Get("o2sim");
  auto nentries=tree->GetEntries();
  std::vector<o2::itsmft::Digit>* digArr = nullptr;
  tree->SetBranchAddress("MFTDigit", &digArr);
  for(int i=0;i<nentries;i++){
    tree->GetEvent(i);
    Int_t nd = digArr->size();
    cout<<"Number of digits: "<<nd<<endl;
    while (nd--) {
    Int_t binx,biny, bin, binmax;
      const Digit* d = &(*digArr)[nd];
      Int_t chipID = d->getChipIndex();
      for(int j=0;j<=sizeVecDec;j++){
        if (chipID==vecChipDec[j])
          vecColRow[j][d->getColumn()][d->getRow()]++;
      }
    }
  }
  ofstream output;
  output.open ("digit_coordinate.txt");
  Int_t bin;
  for(int k=0;k<sizeVecDec;k++){ // to store the coordinate for the thr scan
    int kk = vecTrans[k];
        for(int binx=0;binx<1024;binx++){
          for(int biny=0;biny<512;biny++){
            bin=vecColRow[k][binx][biny];
            if(bin!=0)output<<kk<<" "<<binx-1<<" "<<biny-1<<" "<<bin-1<<" "<<vpulsel<<"\n";
          }
        }
      }
      output.close();
}
