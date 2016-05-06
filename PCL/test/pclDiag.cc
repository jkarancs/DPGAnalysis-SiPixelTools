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
int main(int argc, char *argv[]) {
  int processID = gSystem->GetPid();
  cout << "Running under process ID  " << processID << endl;
  string progName  = argv[0];
  string fileName("test/PixelPclDcCounts.txt"), outfileName("test/PixelPclDcCounts2.txt");
  int verbose(0);
  // -- command line arguments
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i],"-f"))  {fileName   = string(argv[++i]); }     // single file instead of chain
    if (!strcmp(argv[i],"-o"))  {fileName   = string(argv[++i]); }     // single file instead of chain
    if (!strcmp(argv[i],"-v"))  {verbose    = atoi(argv[++i]); }       // set verbosity level
  }


  PixPclDetectorStatus a;
  a.readFromFile(fileName);
  a.dumpToFile(outfileName);

  return 0;
}
