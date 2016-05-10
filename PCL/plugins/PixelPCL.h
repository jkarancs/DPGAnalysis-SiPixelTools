#ifndef PixelPCL_h
#define PixelPCL_h

/** \class PixelPCL
 * ----------------------------------------------------------------------
 * PixelPCL
 * ---------
 *
 * ED analyzer instantiating pclMon and looping over all hits in detid's.
 *
 * ----------------------------------------------------------------------
 * Send all questions, wishes and complaints to the
 *
 * Author:  Urs Langenegger (PSI)
 * ----------------------------------------------------------------------
 *
 *
 ************************************************************/

#include <map>

#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFrameConverter.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingMap.h"

#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"


#include "DataFormats/Common/interface/ConditionsInEdm.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


#include "DPGAnalysis-SiPixelTools/PCL/interface/PixPclDetectorStatus.h"

struct triplet{
  int roc, col, row;
};

class DetId;

class PixelPCL : public edm::EDAnalyzer {
 public:

  explicit PixelPCL(const edm::ParameterSet& ps);
  virtual ~PixelPCL();
  virtual void beginJob();
  virtual void beginRun(const edm::Run &, const edm::EventSetup &);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void onlineRocColRow(const DetId &pID, int offlineRow, int offlineCol, int &roc, int &col, int &row);

 protected:

 private:
  int                                                     fVerbose;
  bool                                                    fUpdateMaps;
  std::string                                             fFileName;
  int                                                     fInit;
  edm::InputTag                                           fPixelClusterLabel, fPixelRecHitLabel;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>  fSiPixelClusterToken;
  edm::EDGetTokenT<SiPixelRecHitCollection>               fPixelRecHitToken;

  std::map<int, int>                                      fFEDID;
  SiPixelFrameConverter                                  *fPFC[41];
  edm::ESHandle<SiPixelFedCablingMap>                     fCablingMap;
  edm::ESHandle<TrackerGeometry>                          fTG;

  PixPclDetectorStatus                                    fDet;
  // -- map (for each detid) of the map from offline col/row to the online roc/col/row
  std::map<int, std::map<std::pair<int, int>, triplet> >  fIndices;


};

#endif
