#include <iostream>
#include <map>
#include <cmath>
#include <vector>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclRocStatus.h"

using namespace std;

// ----------------------------------------------------------------------
PixPclRocStatus::PixPclRocStatus() {
  for (int i = 0; i < 26; ++i) {
    fDC[i]        = 0;
  }
}


// ----------------------------------------------------------------------
PixPclRocStatus::~PixPclRocStatus() {

}

// ----------------------------------------------------------------------
void PixPclRocStatus::fill(int idc) {
  if (idc<26) fDC[idc]++;
}

// ----------------------------------------------------------------------
void PixPclRocStatus::fillOffline(int col, int row) {

}

// ----------------------------------------------------------------------
int PixPclRocStatus::status(int idc) {
  return (idc<26?fDC[idc]:-1);
}

// ----------------------------------------------------------------------
int PixPclRocStatus::status() {
  int count(0) ;
  for (int idc = 0; idc < 26; ++idc) {
    count += fDC[idc];
  }
  return count;
}
