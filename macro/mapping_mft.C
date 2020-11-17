#include "MFTBase/GeometryTGeo.h"
#include "ITSMFTReconstruction/ChipMappingMFT.h"
#include <stdio.h>
#include <math.h>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH3.h"
#include "TH3.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <TStyle.h>
#include "TFile.h"
#include "TTree.h"
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

vector <int> getChipDec()
{
  fstream inputDec; //ChipID from decoder
  inputDec.open("/home/alice/alice/output/Chip_ID.txt", ios_base::in);
  vector <int> vecChipDec;
  int ChipDecod;
  int sizeVecDec=vecChipDec.size();
  while(inputDec >> ChipDecod) vecChipDec.push_back(ChipDecod);
  inputDec.close();
  return vecChipDec;
}

int getZone(int layer, int ladderID);

void mapping_mft()
{
  const std::string inputGeom = "/home/alice/alice/O2/o2sim_geometry.root";
  o2::base::GeometryManager::loadGeometry(inputGeom);
  auto gm = o2::mft::GeometryTGeo::Instance(); // geometry manager

  const o2::itsmft::ChipMappingMFT map;
    int half, disk, layer, zone, module, ladder, ladderID;
  int chipIDglo, chipIDlocSW, chipIDlocHW, chipOnModule;
  int ruIDSW, ruIDHW, ruType;
  uint16_t ruOnLayer, chipOnRU, link = 0;
  uint8_t connector, cableHW, cableSW;
  int faceID;
  const int NChips = map.getNChips();

  const o2::itsmft::ChipOnRUInfo* chipOnRUInfo;
  const o2::itsmft::RUInfo* ruInfo;
  o2::itsmft::ChipInfo chipInfo;

  vector<int> vecChipDec=getChipDec();
  int sizeVecDec=vecChipDec.size();
  fstream output_histoname;
  fstream output_trans;
  output_histoname.open("/home/alice/alice/output/histoname.txt",ios::out);
  output_trans.open("/home/alice/alice/output/trans.txt",ios::out);

  printf("ChipIDglo   ChipIDlocSW   ChipIDlocHW   Half   Disk   Layer   RUIDSW   RUIDHW   Type   RUonLayer   Zone   Cnct   CableHW   ChipOnRU \n");
  for (int i = 0; i < sizeVecDec; ++i) {
    chipIDglo = vecChipDec[i];
    gm->getSensorID(chipIDglo, half, disk, ladder, chipIDlocSW);
    module = map.chipID2Module(chipIDglo, chipOnModule);
    layer = gm->getLayer(chipIDglo);
    ladderID = gm->getLadderID(disk, ladder);
    map.getChipInfoSW(chipIDglo, chipInfo);
    ruType = chipInfo.ruType;
    zone = getZone(layer, ladderID);
    ruOnLayer = 4 * half + zone;
    ruIDSW = map.getRUIDSW(layer, ruOnLayer);
    ruIDHW = map.RUSW2FEEId(ruIDSW);
    chipIDlocHW = map.chipModuleIDSW2HW(ruType, chipIDlocSW);

    chipOnRUInfo = chipInfo.chOnRU;
    chipOnRU = chipOnRUInfo->id;
    connector = chipOnRUInfo->moduleHW;
    cableHW = chipOnRUInfo->cableHW;

    if((layer % 2)==0)
      faceID=0;
    else
      faceID=1;

    printf("%-12d%-14d%-14d%-7d%-7d%-8d%-9d0x%-7x%-7d%-12d%-7d%-7d%-10d%-12d\n", chipIDglo, chipIDlocSW, chipIDlocHW, half, disk, \
           layer, ruIDSW, ruIDHW, ruType, faceID, zone, connector, cableHW, chipOnRU);
    int transID = (int)cableHW;

    std::ostringstream os2;
    os2<<"h" << half<<"-f"<<faceID <<"-d"<<disk<<"-z"<<zone<<"-trans"<<transID;
    output_histoname<<os2.str()<<endl;
    cout<<"Chips decoded : "<<os2.str()<<endl;

    std::ostringstream os4;
    os4<<transID;
    output_trans<<os4.str()<<endl;
  }
  output_histoname.close();
  output_trans.close();

}
//__________________________________________________________________________
int getZone(int layer, int ladderID)
{
  int zone = -1;
  if (layer == 0) {
    if (ladderID >=  0 && ladderID <=  2) zone = 0;
    if (ladderID >=  3 && ladderID <=  5) zone = 1;
    if (ladderID >=  6 && ladderID <=  8) zone = 2;
    if (ladderID >=  9 && ladderID <= 11) zone = 3;
  }
  if (layer == 1) {
    if (ladderID >= 12 && ladderID <= 14) zone = 3;
    if (ladderID >= 15 && ladderID <= 17) zone = 2;
    if (ladderID >= 18 && ladderID <= 20) zone = 1;
    if (ladderID >= 21 && ladderID <= 23) zone = 0;
  }
  if (layer == 2) {
    if (ladderID >=  0 && ladderID <=  2) zone = 0;
    if (ladderID >=  3 && ladderID <=  5) zone = 1;
    if (ladderID >=  6 && ladderID <=  8) zone = 2;
    if (ladderID >=  9 && ladderID <= 11) zone = 3;
  }
  if (layer == 3) {
    if (ladderID >= 12 && ladderID <= 14) zone = 3;
    if (ladderID >= 15 && ladderID <= 17) zone = 2;
    if (ladderID >= 18 && ladderID <= 20) zone = 1;
    if (ladderID >= 21 && ladderID <= 23) zone = 0;
  }
  if (layer == 4) {
    if (ladderID >=  0 && ladderID <=  2) zone = 0;
    if (ladderID >=  3 && ladderID <=  5) zone = 1;
    if (ladderID >=  6 && ladderID <=  8) zone = 2;
    if (ladderID >=  9 && ladderID <= 12) zone = 3;
  }
  if (layer == 5) {
    if (ladderID >= 13 && ladderID <= 16) zone = 3;
    if (ladderID >= 17 && ladderID <= 19) zone = 2;
    if (ladderID >= 20 && ladderID <= 22) zone = 1;
    if (ladderID >= 23 && ladderID <= 25) zone = 0;
  }
  if (layer == 6) {
    if (ladderID >=  0 && ladderID <=  3) zone = 0;
    if (ladderID >=  4 && ladderID <=  7) zone = 1;
    if (ladderID >=  8 && ladderID <= 11) zone = 2;
    if (ladderID >= 12 && ladderID <= 15) zone = 3;
  }
  if (layer == 7) {
    if (ladderID >= 16 && ladderID <= 19) zone = 3;
    if (ladderID >= 20 && ladderID <= 23) zone = 2;
    if (ladderID >= 24 && ladderID <= 27) zone = 1;
    if (ladderID >= 28 && ladderID <= 31) zone = 0;
  }
  if (layer == 8) {
    if (ladderID >=  0 && ladderID <=  4) zone = 0;
    if (ladderID >=  5 && ladderID <=  8) zone = 1;
    if (ladderID >=  9 && ladderID <= 12) zone = 2;
    if (ladderID >= 13 && ladderID <= 16) zone = 3;
  }
 if (layer == 9) {
    if (ladderID >= 17 && ladderID <= 20) zone = 3;
    if (ladderID >= 21 && ladderID <= 24) zone = 2;
    if (ladderID >= 25 && ladderID <= 28) zone = 1;
    if (ladderID >= 29 && ladderID <= 33) zone = 0;
  }

  return zone;
}

