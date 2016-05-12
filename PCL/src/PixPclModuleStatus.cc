#include <iostream>
#include <map>
#include <cmath>
#include <vector>

#include <TMath.h>
#include <TH1.h>

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
  if (iroc < fNrocs) fRocs[iroc].fill(idc);
}

// ----------------------------------------------------------------------
void PixPclModuleStatus::fill(int iroc, int idc, int nhit) {
  if (iroc < fNrocs) fRocs[iroc].fill(idc, nhit);
}


// ----------------------------------------------------------------------
void PixPclModuleStatus::analysis(double rocThr, double rocRms) {
  double NSIG(10);
  occupancy();
  cout << "detid: " << detid() << " ";
    // << Form("<hits>/ROC: %7.1f +/- %7.1f: ", average(), sigma());
  int OK(true);

  for (int i = 0; i < fNrocs; ++i) {
    // -- check for dead modules
    if (average() < 1.e-4*rocThr) {
      cout << " module dead " << endl;
      OK = false;
      return;
    }
    // -- check for dead ROCs
    if (fRocs[i].status() < 1.e-3*rocThr) {
      cout << " ROC " << i << "(dead) ";
      OK = false;
    }
    // -- check for hot  ROCs
    if (0 && (fRocs[i].status() - average()) > NSIG*fSigma) {
      cout << " ROC " << i << "(+) ";
      OK = false;
    }
    // -- check for inefficient ROCs
    if (0 && (fRocs[i].status() - average()) < NSIG*fSigma) {
      cout << " ROC " << i << "(-) ";
      OK = false;
    }
  }
  if (OK) {
    cout << "all ROCs OK";
  }
  if (average() < 1.e-2*rocThr) {
    cout << ", module with low occupancy";
  }
  if (average() > 1.e2*rocThr) {
    cout << ", module with high occupancy";
  }
  cout << endl;
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
void PixPclModuleStatus::setNrocs(int iroc) {
  fNrocs = iroc;
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
  double ave(0.), sig(0.);
  for (int iroc = 0; iroc < fNrocs; ++iroc) {
    int inc = status(iroc);
    ave += inc;
  }
  fAverage = ave/fNrocs;

  for (int iroc = 0; iroc < fNrocs; ++iroc) {
    int inc = status(iroc);
    sig += (fAverage-inc)*(fAverage-inc);
  }

  fSigma   = sig/(fNrocs-1);
  fSigma   = TMath::Sqrt(fSigma);
}

// ----------------------------------------------------------------------
PixPclRocStatus* PixPclModuleStatus::getRoc(int i) {
  return &fRocs[i];
}
