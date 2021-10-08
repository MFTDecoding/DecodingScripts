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

#include "DataFormatsITSMFT/Digit.h"
#include "DataFormatsITSMFT/ROFRecord.h"
#include "CommonDataFormat/InteractionRecord.h"


using namespace std;
using namespace o2::base;
using namespace o2::detectors;
using o2::itsmft::Digit;
using o2::itsmft::ROFRecord;
using BCData = o2::InteractionRecord;

void tedshots(const int TFStart = 10021){

  TChain o2MFTDigitsChain("o2sim");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_aroundtf10022.root");

  std::vector<o2::itsmft::Digit>* mftdigit  = nullptr;
  std::vector<o2::itsmft::ROFRecord>* mftdigitROFs = nullptr;
  o2MFTDigitsChain.SetBranchAddress("MFTDigit", &mftdigit );
  o2MFTDigitsChain.SetBranchAddress("MFTDigitROF", &mftdigitROFs);

  Int_t numberTF = o2MFTDigitsChain.GetEntries();
  std::cout << "Number of TF = " << numberTF << std::endl;
  
for (int itf = 0; itf < numberTF; itf++)
{
  o2MFTDigitsChain.GetEvent(itf);
  auto nStrobes = mftdigitROFs->size();
  auto nDigits = mftdigit->size();
  std::cout << "TF " << TFStart + itf << std::endl;
  std::cout << "- Number of RO Frames = " << nStrobes << std::endl;
  std::cout << "- Number of MFT Digits = " << nDigits << std::endl;

  int n = 0;
  for ( auto rofRec : *mftdigitROFs ) { // iterate over the ROF records
    auto& bcdata = (*mftdigitROFs)[n].getBCData();
    auto orbit = bcdata.orbit;
    auto bc = bcdata.bc;
    std::cout << "\tstrobe = " << n << "\torbit = "<< orbit << "\tbc = " << bc << "\t\tnDigits = " << rofRec.getNEntries() << std::endl ;
    n++;
  }
}

}

