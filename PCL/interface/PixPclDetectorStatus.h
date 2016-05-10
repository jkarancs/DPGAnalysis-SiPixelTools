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

  /// number of modules in detector
  int nmodules();

  /// get a Module
  PixPclModuleStatus* getModule(int detid);


  /// provide for iterating over the entire detector
  std::map<int, PixPclModuleStatus>::iterator begin();
  std::map<int, PixPclModuleStatus>::iterator next();
  std::map<int, PixPclModuleStatus>::iterator end();

 private:
  std::map<int, PixPclModuleStatus> fModules;
  std::map<int, PixPclModuleStatus>::iterator fNext;
};

#endif
