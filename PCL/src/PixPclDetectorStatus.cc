#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <vector>

#include <TString.h>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclDetectorStatus.h"

using namespace std;


// ----------------------------------------------------------------------
PixPclDetectorStatus::PixPclDetectorStatus() {

}

// ----------------------------------------------------------------------
PixPclDetectorStatus::~PixPclDetectorStatus() {

}

// ----------------------------------------------------------------------
void PixPclDetectorStatus::readFromFile(std::string filename) {
  ifstream INS;
  string sline;
  INS.open(filename.c_str());
  int oldDetId(-1);
  int detid, roc, dc, hits;
  PixPclModuleStatus *pMod(0);
  while (getline(INS, sline)) {
    sscanf(sline.c_str(), "%d %d %d %d", &detid, &roc, &dc, &hits);
    if (detid != oldDetId) {
      oldDetId = detid;
      addModule(detid);
      pMod = getModule(detid);
      cout << "adding " << detid << endl;
    }
    if (pMod) pMod->fill(roc, dc, hits);
  }
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::dumpToFile(std::string filename) {
  ofstream OD(filename.c_str());
  map<int, PixPclModuleStatus>::iterator itEnd = end();
  for (map<int, PixPclModuleStatus>::iterator it = begin(); it != itEnd; ++it) {
    for (int iroc = 0; iroc < it->second.nrocs(); ++iroc) {
      for (int idc = 0; idc < 26; ++idc) {
	OD << Form("%10d %2d %2d %3d", it->first, iroc, idc, it->second.getRoc(iroc)->status(idc)) << endl;
      }
    }
  }
  OD.close();
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::addModule(int detid) {
  PixPclModuleStatus a(detid);
  fModules.insert(make_pair(detid, a));
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::addModule(int detid, int nrocs) {
  PixPclModuleStatus a(detid, nrocs);
  fModules.insert(make_pair(detid, a));
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::fill(int detid, int roc, int idc) {
  fModules[detid].fill(roc, idc);
}


// ----------------------------------------------------------------------
map<int, PixPclModuleStatus>::iterator PixPclDetectorStatus::begin() {
  return fModules.begin();
}

// ----------------------------------------------------------------------
map<int, PixPclModuleStatus>::iterator PixPclDetectorStatus::next() {
  return fNext++;
}

// ----------------------------------------------------------------------
map<int, PixPclModuleStatus>::iterator PixPclDetectorStatus::end() {
  return fModules.end();
}

// ----------------------------------------------------------------------
int PixPclDetectorStatus::nmodules() {
  return static_cast<int>(fModules.size());
}

// ----------------------------------------------------------------------
PixPclModuleStatus* PixPclDetectorStatus::getModule(int detid) {
  return &(fModules[detid]);
}
