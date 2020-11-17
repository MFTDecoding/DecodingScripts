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

void ChipDecod(const Char_t *inFile="/home/alice/alice/output/data-d4-f1-2020_09_03__14_10_16__.raw_200904_183137.root"){

  TFile *inputFile =new TFile(inFile);
TTree *tree=(TTree*)inputFile->Get("o2sim");
auto nentries=tree->GetEntries();
std::vector<o2::itsmft::Digit>* digArr = nullptr;
tree->SetBranchAddress("MFTDigit", &digArr);
 std::vector<int> tab_ID;
 for(int i=0;i<nentries;i++){
   tree->GetEvent(i);
   Int_t nd = digArr->size();
   cout<<"Digit? "<<nd<<endl;
   
   while (nd--) {
     const Digit* d = &(*digArr)[nd];
     Int_t chipID = d->getChipIndex();
     if(find (tab_ID.begin(), tab_ID.end(), chipID)==tab_ID.end())tab_ID.push_back(chipID);
   }
 }
 std::fstream output;
 output.open("/home/alice/alice/output/Chip_ID.txt", std::ios::out);
 cout<<"Chips in Digit ROOT file"<<endl;
 for(int n=0;n<(int)tab_ID.size();n++){
   output << tab_ID[n] << std::endl;
   cout<<tab_ID[n] << std::endl;
 }

 output.close();
}
