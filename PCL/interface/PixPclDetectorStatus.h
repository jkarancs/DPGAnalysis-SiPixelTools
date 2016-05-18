#ifndef PIXPCLDETECTORSTATUS_h
#define PIXPCLDETECTORSTATUS_h

#include <map>
#include <string>

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclModuleStatus.h"

// ----------------------------------------------------------------------
class PixPclDetectorStatus {
public:
  PixPclDetectorStatus();
  ~PixPclDetectorStatus();

  /// file I/O
  void readFromFile(std::string filename);
  void dumpToFile(std::string filename);

  /// add PixPclModuleStatus for detID
  void addModule(int detid);

  /// add PixPclModuleStatus for detID, specifying nrocs
  void addModule(int detid, int nrocs);

  /// fill hit in double idc in ROC roc into module detid
  void fill(int detid, int roc, int idc);

  /// fill hit in OFFLINE col/row into module detid
  void fillOffline(int detid, int col, int row);


  /// determine detector average nhits and RMS
  void occupancy();

  /// analysis of detector performance
  void analysis();

  /// number of modules in detector
  int nmodules();

  /// pass lumisection
  void ls(int ls);

  /// pass run
  void run(int run);

  /// get a Module
  PixPclModuleStatus* getModule(int detid);


  /// provide for iterating over the entire detector
  std::map<int, PixPclModuleStatus>::iterator begin();
  std::map<int, PixPclModuleStatus>::iterator next();
  std::map<int, PixPclModuleStatus>::iterator end();

 private:
  std::map<int, PixPclModuleStatus> fModules;
  std::map<int, PixPclModuleStatus>::iterator fNext;

  /// first and last lumisection seen in this instance
  int fLS0, fLS1;

  /// first and last run seen in this instance (likely to be the same number!)
  int fRun0, fRun1;

  /// average (per module) number of hits over entire detector
  int fDetAverage, fDetSigma;

  /// total hits in detector
  unsigned long int fDetHits;
};

#endif
