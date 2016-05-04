#ifndef PIXPCLMODULESTATUS_h
#define PIXPCLMODULESTATUS_h

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclRocStatus.h"

#include <vector>


// ----------------------------------------------------------------------
class PixPclModuleStatus {
public:
  PixPclModuleStatus(int det = 0, int nrocs = 16);
  ~PixPclModuleStatus();

  /// fill with online coordinates
  void fill(int iroc, int idc);

  /// return DC status of a ROC (=hits on DC idc on ROC iroc)
  int status(int iroc, int idc);

  /// return ROC status (= hits on ROC iroc)
  int status(int iroc);

  /// return module status (= hits on module)
  int status();

  /// get a ROC
  PixPclRocStatus* getRoc(int i);

  /// accessors
  int    detid();
  int    nrocs();
  double average();
  double sigma();

  /// calculate (averaged over this module's ROCs) mean hit number and its sigma
  void occupancy();

private:
  int fDetid, fNrocs;
  double fAverage, fSigma;
  std::vector<PixPclRocStatus> fRocs;
};

#endif
