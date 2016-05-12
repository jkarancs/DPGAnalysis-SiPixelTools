#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

#include "TROOT.h"
#include "TRint.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TRandom.h"
#include "TSystem.h"

#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclDetectorStatus.h"

using namespace std;

// ----------------------------------------------------------------------
vector<string> glob(string basedir, string basename) {
  // cout << "Looking in " << basedir << " for " << basename << endl;
  vector<string> lof;
  TString fname;
  const char *file;
  TSystem *lunix = gSystem; //new TUnixSystem();
  void *pDir = lunix->OpenDirectory(basedir.c_str());
  while ((file = lunix->GetDirEntry(pDir))) {
    fname = file;
    // cout << "glob checking " << fname;
    if (fname.Contains(basename.c_str())) {
      lof.push_back(string(basedir+"/"+fname));
      // cout << ", adding " << string(basedir+"/"+fname) << endl;
    } else {
      // cout << ", skipping" << endl;
    }
  }
  return lof;
}



// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {
  int processID = gSystem->GetPid();
  string progName  = argv[0];
  string runName("nada"), fileName("test/PixelPclDcCounts.txt"), outfileName("test/PixelPclDcCounts2.txt");
  int verbose(0);
  // -- command line arguments
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i],"-f"))  {fileName    = string(argv[++i]); }     // input file
    if (!strcmp(argv[i],"-o"))  {outfileName = string(argv[++i]); }     // output file
    if (!strcmp(argv[i],"-r"))  {runName     = string(argv[++i]); }     // run to combine/process
    if (!strcmp(argv[i],"-v"))  {verbose     = atoi(argv[++i]); }       // set verbosity level
  }


  PixPclDetectorStatus a;
  // -- combine output from a run
  if (runName != "nada") {
    outfileName = Form("combined-%s.txt", runName.c_str());
    vector<string> tmpdirs = glob(Form("test/run-%s", runName.c_str()), "tmp-pcl-");
    sort(tmpdirs.begin(), tmpdirs.end());
    for (unsigned int i = 0; i < tmpdirs.size(); ++i) {
      fileName = Form("%s/pcl-%s-%04d.sge.log", tmpdirs[i].c_str(), runName.c_str(), i);
      cout << "tmpdirs[" << i << "] = " << tmpdirs[i] << ": " << fileName << endl;
      a.readFromFile(fileName);
    }
    a.dumpToFile(outfileName);
  } else {
    a.readFromFile(fileName);
    a.analysis();
  }
  return 0;
}
