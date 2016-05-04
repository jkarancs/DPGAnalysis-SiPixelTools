#include <iostream>
#include <map>
#include <cmath>
#include <vector>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclDetectorStatus.h"

using namespace std;


// ----------------------------------------------------------------------
PixPclDetectorStatus::PixPclDetectorStatus() {

}

// ----------------------------------------------------------------------
PixPclDetectorStatus::~PixPclDetectorStatus() {

}

// ----------------------------------------------------------------------
void PixPclDetectorStatus::addModule(int detid) {
  PixPclModuleStatus a(detid);
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
