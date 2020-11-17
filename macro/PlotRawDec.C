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
  while(inputDec >> ChipDecod)vecChipDec.push_back(ChipDecod);
  inputDec.close();
  return vecChipDec;
}

vector <string> getHistoName()
{
  fstream inputHistoName; //read file with namehisto
  inputHistoName.open("/home/alice/alice/output/histoname.txt",ios_base::in);
  vector <string> vecHistoName;
  string word2;
  while(inputHistoName >> word2) vecHistoName.push_back(word2);
  inputHistoName.close();
  return vecHistoName;
}

void PlotRawDec(const Char_t *inFile="/home/alice/alice/output/data-d4-f1-2020_09_03__14_10_16__.raw_200904_183137.root",const char *dataraw=" ",const char *datatime=" "){

  TFile *inputFile =new TFile(inFile);
  vector<int> vecChipDec=getChipDec();
  int sizeVecDec=vecChipDec.size();
  std::cout<<"How many chips? "<<sizeVecDec<<std::endl;
  vector<string> vecHistoName=getHistoName();

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
 
  TTree *tree=(TTree*)inputFile->Get("o2sim");
  auto nentries=tree->GetEntries();
  std::vector<o2::itsmft::Digit>* digArr = nullptr;
  tree->SetBranchAddress("MFTDigit", &digArr);
  for(int i=0;i<nentries;i++){
    tree->GetEvent(i);
    Int_t nd = digArr->size();
    cout<<"Events? "<<nd<<endl;
 
    while (nd--) {
    Int_t binx,biny, bin, binmax;
      const Digit* d = &(*digArr)[nd];
      Int_t chipID = d->getChipIndex();
      for(int j=0;j<sizeVecDec;j++){
        if (chipID==vecChipDec[j]){
          hplot[j]->Fill(d->getColumn(), d->getRow());
        }
      }
    }
  }
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
    std::string histnamesave = "Plots/hist_";
    histnamesave +=os2;
    histnamesave += "_raw_";
    histnamesave += dataraw;
    histnamesave += "_created_";
    histnamesave += datatime;
    histnamesave += ".pdf";
    const char *finalname =  histnamesave.c_str();
    c1[k]->SaveAs(finalname);

  }
}

