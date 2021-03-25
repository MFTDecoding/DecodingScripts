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

void NewDiskPlot(){

  std::ostringstream os_file;
  string os1_file;

   TChain chain("o2sim");
   for (int i=0; i<12; i++){ 
  	     os_file<<"/local/home/mc262512/alice/output/data-2021_03_12__19_29_35__-20561-"<< i <<".root";
	     os1_file = os_file.str();
	     os_file.str("");
	     os_file.clear();
	   chain.Add(os1_file.c_str());
   }



  auto nentries=chain.GetEntries();
  std::vector<o2::itsmft::Digit>* digArr = nullptr;
  chain.SetBranchAddress("MFTDigit", &digArr);

  vector<int> vecChipDec;
  vector<string> vecHistoName;

  std::ostringstream os;
  string os1;

  
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
  std::cout<<"How many chips? "<<sizeVecDec<<std::endl;

  gStyle->SetPalette(55); // Rainbow
 
TH2F * hThreshold = new TH2F("h0-d4-f1","h0-d4-f1",17*512,-0.5,17*512-0.5,5*1024,-0.5,5*1024-0.5);

  for(int i=0;i<nentries;i++){
    chain.GetEvent(i);
    Int_t nd = digArr->size();
 
    while (nd--) {
      const Digit* d2 = &(*digArr)[nd];
      Int_t chipID = d2->getChipIndex();
      Int_t cableHW = chipMap[d2->getChipIndex()].cable;
      Int_t half = chipMap[d2->getChipIndex()].disk;
      Int_t layer = chipMap[d2->getChipIndex()].layer;
      Int_t face = layer % 2;
      Int_t disk = chipMap[d2->getChipIndex()].disk;
      Int_t zone = chipMap[d2->getChipIndex()].zone;

      for(int j=0;j<sizeVecDec;j++){
        if (chipID==vecChipDec[j]){
	  int col = d2->getColumn();
	  int row = d2->getRow();
          int Column = AddressToColumn(disk,cableHW,zone,col,row);
          int Row    = AddressToRow(disk,cableHW,zone,col,row);
          hThreshold->Fill(Column, Row);
        }
      }
    }
  }
    auto c1 = new TCanvas();
    gStyle->SetOptStat(0);
    hThreshold->Draw("colz PMC");
    std::string histnamesave = "hist_h0-d4-f1.pdf";
    const char *finalname =  histnamesave.c_str();
    c1->SaveAs(finalname);

}

