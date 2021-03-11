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
using o2::itsmft::DigitHW;


void PlotMacro(){

  TFile *inputFile =new TFile("../output/data-d2-2020_12_08__16_03_04__-1.raw.root");
  TTree *tree=(TTree*)inputFile->Get("o2sim");
  UShort_t half, plane, zone, disk, chipindex, row, col, charge, counter, ninj, chargeinj, cableHW;

auto nentries=tree->GetEntries();
vector<o2::itsmft::DigitHW>* digArr = nullptr;
tree->SetBranchAddress("MFTDigit", &digArr);

vector<int> vecChipDec;
vector <string> vecHistoName;
ostringstream os2;
string str1;

Int_t nd=0;


for(int i=0;i<nentries;i++){
   tree->GetEvent(0);
   nd = digArr->size();

   while (nd--) {
     const DigitHW* d = &(*digArr)[nd];
      chipindex = d->getChipIndex();
      cableHW = d->getCableHW();
      half = d->getHalf();
      disk = d->getDisk();
      plane = d->getPlane();
      zone = d->getZone();
     if(find (vecChipDec.begin(), vecChipDec.end(), chipindex)==vecChipDec.end()){
             vecChipDec.push_back(chipindex);
             cout << chipindex << endl;
             os2<<"h" <<half<<"-f"<<plane<<"-d"<<disk<<"-z"<<zone<<"-trans"<<cableHW;
             str1 = os2.str();
             os2.str("");
             os2.clear();
             cout << str1 << endl;
             vecHistoName.push_back(str1);
     }
   }
}

  int sizeVecDec=vecChipDec.size();
  int sizeVecHisto=vecHistoName.size();
  if (sizeVecDec != sizeVecHisto) {
          cout << "Error size" <<endl;
          return 1;
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
    nd = digArr->size();
    cout<<"Events? "<<nd<<endl;

    while (nd--) {
      const DigitHW* d2 = &(*digArr)[nd];
      chipindex = d2->getChipIndex();
//      cout << "Transiever : " << cableHW << ", Half : " << half << ", Disk : " << disk << ", Plane : "<< plane << ", Zone : " << zone << endl;
      for(int j=0;j<sizeVecDec;j++){
        if (chipindex==vecChipDec[j]){
          hplot[j]->Fill(d2->getColumn(), d2->getRow());
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
    string histnamesave = "hist_";
    histnamesave +=os2;
    histnamesave += ".pdf";
    const char *finalname =  histnamesave.c_str();
    c1[k]->SaveAs(finalname);
    }
}
