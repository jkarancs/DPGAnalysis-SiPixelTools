#include <iostream>
#include <map>
#include <cmath>
#include <vector>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclModuleStatus.h"

using namespace std;

// ----------------------------------------------------------------------
PixPclModuleStatus::PixPclModuleStatus(int det, int nrocs): fDetid(det), fNrocs(nrocs) {
  for (int i = 0; i < fNrocs; ++i) {
    PixPclRocStatus a;
    fRocs.push_back(a);
  }
};


// ----------------------------------------------------------------------
PixPclModuleStatus::~PixPclModuleStatus() {};


// ----------------------------------------------------------------------
void PixPclModuleStatus::fill(int iroc, int idc) {
  if (iroc < static_cast<int>(fRocs.size())) fRocs[iroc].fill(idc);
}

// ----------------------------------------------------------------------
void PixPclModuleStatus::fill(int iroc, int idc, int nhit) {
  if (iroc < static_cast<int>(fRocs.size())) fRocs[iroc].fill(idc, nhit);
}


// ----------------------------------------------------------------------
int PixPclModuleStatus::status(int iroc, int idc) {
  return (iroc < fNrocs ? fRocs[iroc].status(idc) : -1);
}


// ----------------------------------------------------------------------
int PixPclModuleStatus::status(int iroc) {
  return (iroc < fNrocs ? fRocs[iroc].status() : -1);
}


// ----------------------------------------------------------------------
int PixPclModuleStatus::status() {
  int count(0);
  for (int iroc = 0; iroc < fNrocs; ++iroc) {
    count += status(iroc);
  }
  return count;
}


// ----------------------------------------------------------------------
int PixPclModuleStatus::detid() {
  return fDetid;
}


// ----------------------------------------------------------------------
int PixPclModuleStatus::nrocs() {
  return fNrocs;
}


// ----------------------------------------------------------------------
double PixPclModuleStatus::average() {
  return fAverage;
}


// ----------------------------------------------------------------------
double PixPclModuleStatus::sigma() {
  return fSigma;
}


// ----------------------------------------------------------------------
void PixPclModuleStatus::occupancy() {
  fAverage = fSigma = 0.;
  for (int iroc = 0; iroc < fNrocs; ++iroc) {
    int inc = status(iroc);
    fAverage += inc;
    fSigma += inc*inc;
  }
  fAverage /= fNrocs;
  fSigma    = sqrt((fSigma/fNrocs - fAverage*fAverage));
}

// ----------------------------------------------------------------------
PixPclRocStatus* PixPclModuleStatus::getRoc(int i) {
  return &fRocs[i];
}
