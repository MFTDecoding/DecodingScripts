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

void tedshots(const UInt_t tfStart = 1, UInt_t runNumber = 503581){

  TChain o2MFTDigitsChain("o2sim");

  //o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_aroundtf10022.root"); // tfStart = 10021

  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_00.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_01.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_02.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_03.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_04.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_05.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_06.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_07.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_08.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_09.root");

  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_10.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_11.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_12.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_13.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_14.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_15.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_16.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_17.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_18.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_19.root");

  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_20.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_21.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_22.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_23.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_24.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_25.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_26.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_27.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_28.root");
  o2MFTDigitsChain.Add("/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_group_tflist_29.root");  

  std::vector<o2::itsmft::Digit>* mftdigit  = nullptr;
  std::vector<o2::itsmft::ROFRecord>* mftdigitROFs = nullptr;
  o2MFTDigitsChain.SetBranchAddress("MFTDigit", &mftdigit );
  o2MFTDigitsChain.SetBranchAddress("MFTDigitROF", &mftdigitROFs);

  UInt_t numberTF = o2MFTDigitsChain.GetEntries();
  std::cout << "Number of TF = " << numberTF << std::endl;

  std::stringstream runNumberName; runNumberName << runNumber;
  std::string outputFileName = "/Users/andry/alice/commissioning/tedshots/2021-10-08/run503581/mftdigits_summary_run";
  outputFileName += runNumberName.str() + ".root";

  TFile *outputFile = new TFile(outputFileName.c_str(), "recreate");
  
  TTree* summaryTree = new TTree("summary","the summary tree");
  UInt_t tfId = tfStart;
  UInt_t orbit = 0;
  UInt_t bc = 0;
  UInt_t strobe = 0;
  UInt_t nDigits = 0;
	summaryTree->Branch("tfId",    &tfId,    "tfId/i");
	summaryTree->Branch("orbit",   &orbit,   "orbit/i");
	summaryTree->Branch("bc",      &bc,      "bc/i");
	summaryTree->Branch("strobe",  &strobe,  "strobe/i");
	summaryTree->Branch("nDigits", &nDigits, "nDigits/i");	

  for (UInt_t itf = 0; itf < numberTF; itf++)
  {
    tfId = tfStart + itf;
    o2MFTDigitsChain.GetEvent(itf);
    auto nStrobesPerTF = mftdigitROFs->size();
    auto nDigitsPerTF = mftdigit->size();
    std::cout << "TF " << tfId << std::endl;
    std::cout << "- Number of RO Frames = " << nStrobesPerTF << std::endl;
    std::cout << "- Number of MFT Digits = " << nDigitsPerTF << std::endl;

    strobe = 0;
    for ( auto rofRec : *mftdigitROFs ) { // iterate over the ROF records
      auto& bcdata = (*mftdigitROFs)[strobe].getBCData();
      orbit = bcdata.orbit; // uint32_t
      bc = bcdata.bc; // uint16_t
      nDigits = rofRec.getNEntries();
      if (false) {
        std::cout << "\tstrobe = " << strobe << "\torbit = "<< orbit << "\tbc = " << bc << "\t\tnDigits = " << nDigits << std::endl;
      }
      summaryTree->Fill();
      strobe++;
    }
  }
  summaryTree->Write();

  TTree* inputParamTree = new TTree( "inputParam", "the input parameters" );
  inputParamTree->Branch( "runNumber", &runNumber, "runNumber/i" );
  inputParamTree->Fill();
  inputParamTree->Write();
  
  outputFile->Close();
}

