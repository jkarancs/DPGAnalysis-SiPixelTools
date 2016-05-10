// ----------------------------------------------------------------------
// PixelPCL
// ---------
// see header file for description
// ----------------------------------------------------------------------
// Send all questions, wishes and complaints to the
//
// Author:  Urs Langenegger (PSI)
// ----------------------------------------------------------------------

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "PixelPCL.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

using namespace std;
using namespace edm;
using namespace edmNew;
using namespace reco;


// ----------------------------------------------------------------------
PixelPCL::PixelPCL(edm::ParameterSet const& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fUpdateMaps(iConfig.getUntrackedParameter<bool>("updateMaps", false)),
  fFileName(iConfig.getUntrackedParameter<string>("fileName", string("PixelPclDcCounts.txt"))),
  fInit(0),
  fPixelClusterLabel(iConfig.getUntrackedParameter<InputTag>("pixelClusterLabel", edm::InputTag("siPixelClusters"))),
  fPixelRecHitLabel(iConfig.getUntrackedParameter<InputTag>("pixelRecHitLabel", InputTag("siPixelRecHits"))),
  fSiPixelClusterToken(consumes<edmNew::DetSetVector<SiPixelCluster>>(fPixelClusterLabel)),
  fPixelRecHitToken(consumes<SiPixelRecHitCollection>(fPixelRecHitLabel)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- PixelPCL constructor" << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  pixelClusterLabel                " << fPixelClusterLabel << endl;
  cout << "---  pixelRecHitLabel:                " << fPixelRecHitLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
PixelPCL::~PixelPCL() {

}


// ----------------------------------------------------------------------
void PixelPCL::endJob() {

  for (int i = 0; i < 40; ++i) {
    delete fPFC[i];
  }

  // -- determine number of dead modules
  int dead(0);
  map<int, PixPclModuleStatus>::iterator itEnd = fDet.end();
  for (map<int, PixPclModuleStatus>::iterator it = fDet.begin(); it != itEnd; ++it) {
    (*it).second.occupancy();
    if ((*it).second.status() < 1) {
      ++dead;
    }
  }

  // -- summary printout
  if (0) {
  if (dead == fDet.nmodules()) {
    cout << "==> the pixel detector was likely off" << endl;
  } else {
    for (map<int, PixPclModuleStatus>::iterator it = fDet.begin(); it != itEnd; ++it) {
      (*it).second.occupancy();
      // -- print dead modules
      if (it->second.status() < 1) {
	cout << "module " << it->second.detid()
	     << " is dead"
	     << endl;
      } else if (it->second.average() < 1) {
	cout << "module " << it->second.detid()
	     << " has virtually no hits: " << it->second.status()
	     << endl;
      }

      // -- print bad ROCs and  DC: NSIG sigma away from mean DC occupancy
      double NSIG(20.);
      for (int iroc = 0; iroc < it->second.nrocs(); ++iroc) {
	if (it->second.status(iroc) < it->second.average() - 2.*it->second.sigma()) {
	  cout << "module " << (*it).second.detid()
	       << " bad ROC " << iroc
	       << " with nhits = " << it->second.status(iroc)
	       << " (module ROC average +/- " << " 1 sigma = " << it->second.average()
	       << " +/- " << it->second.sigma()
	       << endl;
	} else if (it->second.status(iroc) > it->second.average() + 2.*it->second.sigma()) {
	  cout << "module " << (*it).second.detid()
	       << " hot ROC " << iroc
	       << " with nhits = " << it->second.status(iroc)
	       << " (module ROC average +/- " << " 1 sigma = " << it->second.average()
	       << " +/- " << it->second.sigma()
	       << endl;
	} else {
	  for (int idc = 0; idc < 26; ++idc) {
	    if (TMath::Abs(it->second.getRoc(iroc)->status(idc) - it->second.average()/26) > NSIG*it->second.sigma()/26) {
	      cout << "module " << (*it).second.detid()
		   << " ROC " << iroc << " bad DC " << idc
		   << " with nhits = " << it->second.getRoc(iroc)->status(idc)
		   << " (module DC average +/- " << NSIG << " sigma = " << it->second.average()/(26)
		   << " +/- " << NSIG*it->second.sigma()/26
		   << endl;
	    }
	  }
	}
      }
    }
  }
  }

  // -- write out complete DC counts (for validation and analysis)
  fDet.dumpToFile(fFileName.c_str());

  // -- write maps from offline detid/col/row to online roc/col/row
  if (fUpdateMaps) {
    for (map<int, map<pair<int, int>, triplet> >::iterator iIdx = fIndices.begin();
	 iIdx != fIndices.end();
	 ++iIdx) {

      ofstream OS(Form("maps/map%d.txt", iIdx->first));
      for (map<pair<int, int>, triplet>::iterator iPix = iIdx->second.begin(); iPix != iIdx->second.end(); ++iPix) {
	OS << iPix->first.first << " " << iPix->first.second << " "
	   << iPix->second.roc << " " << iPix->second.col << " " << iPix->second.row
	   << endl;
      }
      OS.close();
    }
  }
}


// ----------------------------------------------------------------------
void PixelPCL::beginJob() {

}


// ----------------------------------------------------------------------
void  PixelPCL::beginRun(const Run &run, const EventSetup &iSetup) {
}


// ----------------------------------------------------------------------
void PixelPCL::endRun(Run const&run, EventSetup const&iSetup) {

}


// ----------------------------------------------------------------------
void PixelPCL::analyze(const edm::Event& iEvent,
			const edm::EventSetup& iSetup)  {

  if (0 == fInit) {
    fInit = 1;
    // -- Setup cabling map and its map to detIDs
    cout << "--> setup cabling map and frame converters" << endl;
    iSetup.get<SiPixelFedCablingMapRcd>().get(fCablingMap);
    for (int i = 0; i < 41; ++i) {
      fPFC[i] = new SiPixelFrameConverter(fCablingMap.product(), i);
    }

    // -- initialization of fFEDID
    cout << "--> initialize FEDs for offline/online conversion" << endl;
    edm::ESHandle<TrackerGeometry> pDD;
    iSetup.get<TrackerDigiGeometryRecord>().get(pDD);

    for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
      if (dynamic_cast<const PixelGeomDetUnit*>((*it)) != 0) {
	DetId detId = (*it)->geographicalId();
	uint32_t newDetId = detId;

	for (int fedid = 0; fedid < 41; ++fedid) {
	  if (fPFC[fedid]->hasDetUnit(newDetId)) {
	    fFEDID.insert(make_pair(newDetId, fedid));
	    break;
	  }
	}
      }
    }

    // -- Initialization of fIndices
    cout << "--> initialize fIndices" << endl;
    iSetup.get<TrackerDigiGeometryRecord>().get(fTG);
    int nmod(0);
    for (TrackerGeometry::DetContainer::const_iterator it = fTG->dets().begin(); it != fTG->dets().end(); it++){
      const PixelGeomDetUnit *pgdu = dynamic_cast<const PixelGeomDetUnit*>((*it));
      if (0 == pgdu) continue;
      DetId detId = (*it)->geographicalId();
      int nrocs = pgdu->specificTopology().ncolumns()/52*pgdu->specificTopology().nrows()/80;
      cout << "detid = " << detId.rawId()
	   << " columns =  " << pgdu->specificTopology().ncolumns()
	   << " rows = " << pgdu->specificTopology().nrows()
	   << " nrocs = " << nrocs
	   << endl;
      fDet.addModule(detId, nrocs);
      map<pair<int, int>, triplet> a;
      fIndices.insert(make_pair(detId, a));
      ++nmod;
    }
    cout << "--> initialized fDet with " << nmod << " modules" << endl;

    cout << "--> read in offline/online mapping from maps/*.txt" << endl;
    int mr0(-1), mc0(-1);
    triplet a;
    for (map<int, map<pair<int, int>, triplet> >::iterator iIdx = fIndices.begin();
	 iIdx != fIndices.end();
	 ++iIdx) {
      ifstream INS(Form("maps/map%d.txt", iIdx->first));
      string sline;
      while (getline(INS, sline)) {
	sscanf(sline.c_str(), "%d %d %d %d %d", &mc0, &mr0, &a.roc, &a.col, &a.row);
	fIndices[iIdx->first].insert(make_pair(make_pair(mc0, mr0), a));
      }
      INS.close();
    }
  }


  int run   = iEvent.id().run();
  int event = iEvent.id().event();
  int ls    = iEvent.luminosityBlock();
  fDet.run(run);
  fDet.ls(ls);


  // ----------------------------------------------------------------------
  // -- Pixel cluster analysis
  // ----------------------------------------------------------------------
  const edmNew::DetSetVector<SiPixelCluster> *clustColl(0);
  Handle<edmNew::DetSetVector<SiPixelCluster> > hClusterColl;
  iEvent.getByToken(fSiPixelClusterToken, hClusterColl);
  clustColl = hClusterColl.product();

  for (TrackerGeometry::DetContainer::const_iterator it = fTG->dets().begin(); it != fTG->dets().end(); it++) {
    if (0 == dynamic_cast<const PixelGeomDetUnit*>((*it))) continue;
    DetId detId = (*it)->geographicalId();
    int detid = detId.rawId();
    // -- clusters on this det
    edmNew::DetSetVector<SiPixelCluster>::const_iterator isearch = clustColl->find(detId);
    if (isearch == clustColl->end()) continue;
    int roc(0), rocC(0), rocR(0);
    int mr0(-1), mc0(-1);
    // -- it is faster to use fIndices[detid] repeatedly instead of the next line?!
    //    map<int, map<pair<int, int>, triplet> >::iterator iMap = fIndices.find(detid);
    for (edmNew::DetSet<SiPixelCluster>::const_iterator  di = isearch->begin(); di != isearch->end(); ++di) {
      // -- loop over digis of cluster
      const vector<SiPixelCluster::Pixel>& pixvector = di->pixels();
      for (unsigned int i = 0; i < pixvector.size(); ++i) {
	// -- map all rows into just two
	mr0 = pixvector[i].x;
	if (mr0 < 80) {
	  mr0 = 0;
	} else {
	  mr0 = 159;
	}
	//	mr0 = (pixvector[i].x < 80:0:159);
	mc0 = pixvector[i].y;
	pair<int, int> offline(mc0, mr0);
	map<pair<int, int>, triplet>::iterator pI = fIndices[detid].find(offline);
	// -- it is faster to use end() in the next line than to store it as an iterator outside the loop?!
	if (pI != fIndices[detid].end()) {
	  roc  = pI->second.roc;
	  rocC = pI->second.col;
	  rocR = pI->second.row;
	} else {
	  onlineRocColRow(detId, mr0, mc0, roc, rocC, rocR);
	  triplet a;
	  a.roc = roc;
	  a.col = rocC;
	  a.row = rocR;
	  fIndices[detid].insert(make_pair(offline, a));
	}
	fDet.fill(detId, roc, rocC/2);
      }
    }
  }
}

// ----------------------------------------------------------------------
// copied from DQM/SiPixelMonitorClient/src/SiPixelInformationExtractor.cc
// this function is quite slow!
void PixelPCL::onlineRocColRow(const DetId &pID, int offlineRow, int offlineCol, int &roc, int &col, int &row) {
  int realfedID = -1;
  uint32_t newDetId = pID;

  sipixelobjects::ElectronicIndex cabling;
  sipixelobjects::DetectorIndex detector = {pID, offlineRow, offlineCol};

  realfedID = fFEDID[newDetId];
  fPFC[realfedID]->toCabling(cabling, detector);

  sipixelobjects::LocalPixel::DcolPxid loc;
  loc.dcol = cabling.dcol;
  loc.pxid = cabling.pxid;

  sipixelobjects::LocalPixel locpixel(loc);
  sipixelobjects::CablingPathToDetUnit path = {(unsigned int) realfedID, (unsigned int)cabling.link, (unsigned int)cabling.roc};
  const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);

  roc = theRoc->idInDetUnit();
  uint32_t detSubId = pID.subdetId();
  if (detSubId == 1) {
    PixelBarrelName nameworker(pID);
    std::string outputname = nameworker.name();
    bool HalfModule = nameworker.isHalfModule();
    if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
      roc = theRoc->idInDetUnit() + 8;
    }
  }
  col = locpixel.rocCol();
  row = locpixel.rocRow();
}



// define this as a plug-in
DEFINE_FWK_MODULE(PixelPCL);
