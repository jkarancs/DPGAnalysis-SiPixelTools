#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <vector>

#include <TString.h>
#include <TMath.h>
#include <TH1.h>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclDetectorStatus.h"

using namespace std;


// ----------------------------------------------------------------------
PixPclDetectorStatus::PixPclDetectorStatus(): fLS0(9999999), fLS1(0), fRun0(9999999), fRun1(0) {

}

// ----------------------------------------------------------------------
PixPclDetectorStatus::~PixPclDetectorStatus() {

}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::analysis() {
  occupancy();

  if (fDetAverage < 1.) {
    cout << "detector likely switched off, stop" << endl;
    return;
  }
  int cnt(0);
  map<int, PixPclModuleStatus>::iterator itEnd = end();
  for (map<int, PixPclModuleStatus>::iterator it = begin(); it != itEnd; ++it) {
    it->second.analysis(fDetAverage, fDetSigma);
    ++cnt;
  }
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::readFromFile(std::string filename) {
  ifstream INS;
  string sline;
  INS.open(filename.c_str());
  int oldDetId(-1);
  int detid(0), roc(0), dc(0), hits(0), nroc(0);
  PixPclModuleStatus *pMod(0);
  bool readOK(false);
  while (getline(INS, sline)) {
    if (string::npos != sline.find("# PixPclDetectorStatus START")) {
      // cout << "found start marker" << endl;
      readOK = true;
      continue;
    }
    if (!readOK) continue;

    if (string::npos != sline.find("# PixPclDetectorStatus END")) {
      pMod->setNrocs(nroc+1);
      // cout << "set nrocs = " << nroc+1 << " for detid = " << oldDetId << " pMod->detid = " << pMod->detid() << endl;
      // cout << "found end marker" << endl;
      break;
    }

    if (sline.find("# PixPclDetectorStatus for LS") != string::npos) {
      sscanf(sline.c_str(), "# PixPclDetectorStatus for LS  %d .. %d", &fLS0, &fLS1);
      continue;
    }
    if (sline.find("# PixPclDetectorStatus for run") != string::npos) {
      sscanf(sline.c_str(), "# PixPclDetectorStatus for run %d .. %d", &fRun0, &fRun1);
      continue;
    }

    sscanf(sline.c_str(), "%d %d %d %d", &detid, &roc, &dc, &hits);
    if (roc > nroc) nroc = roc;
    if (detid != oldDetId) {
      if (pMod) {
	pMod->setNrocs(nroc+1);
	// cout << "set nrocs = " << nroc+1 << " for detid = " << oldDetId << " pMod->detid = " << pMod->detid() << endl;
      }

      oldDetId = detid;
      if (0 == getModule(detid)) {
	addModule(detid);
	// cout << "adding " << detid << endl;
      } else {
	// cout << "retrieving " << detid << endl;
      }
      pMod = getModule(detid);
      nroc = 0;
    }
    if (pMod) pMod->fill(roc, dc, hits);
  }
  INS.close();
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::dumpToFile(std::string filename) {
  ofstream OD(filename.c_str());
  OD << "# PixPclDetectorStatus START" << endl;
  OD << "# PixPclDetectorStatus for LS  " << fLS0 << " .. " << fLS1 << endl;
  OD << "# PixPclDetectorStatus for run " << fRun0 << " .. " << fRun1 << endl;
  map<int, PixPclModuleStatus>::iterator itEnd = end();
  for (map<int, PixPclModuleStatus>::iterator it = begin(); it != itEnd; ++it) {
    for (int iroc = 0; iroc < it->second.nrocs(); ++iroc) {
      for (int idc = 0; idc < 26; ++idc) {
	OD << Form("%10d %2d %2d %3d", it->first, iroc, idc, it->second.getRoc(iroc)->status(idc)) << endl;
      }
    }
  }
  OD << "# PixPclDetectorStatus END" << endl;
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
void PixPclDetectorStatus::ls(int ls) {
  if (ls < fLS0) fLS0 = ls;
  if (ls > fLS1) fLS1 = ls;
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::run(int run) {
  if (run < fRun0) fRun0 = run;
  if (run > fRun1) fRun1 = run;
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
  if (fModules.find(detid) == fModules.end()) {
    return 0;
  }
  return &(fModules[detid]);
}


// ----------------------------------------------------------------------
void PixPclDetectorStatus::occupancy() {
  fDetAverage = fDetSigma = 0.;
  double ave(0.), sig(0.);
  int nrocs(0);
  map<int, PixPclModuleStatus>::iterator itEnd = end();
  for (map<int, PixPclModuleStatus>::iterator it = begin(); it != itEnd; ++it) {
    int inc = it->second.status();
    ave   += inc;
    nrocs += it->second.nrocs();
  }
  fDetAverage = ave/nrocs;

  for (map<int, PixPclModuleStatus>::iterator it = begin(); it != itEnd; ++it) {
    int inc = it->second.status();
    sig += (fDetAverage - inc) * (fDetAverage - inc);
  }

  fDetSigma   = sig/(nrocs - 1);
  fDetSigma   = TMath::Sqrt(fDetSigma);
}
