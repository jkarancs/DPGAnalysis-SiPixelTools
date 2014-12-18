#include <memory>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" 

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"

#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"

// For L1
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

// For HLT
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"

// -- residuals
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"

#include "Alignment/OfflineValidation/interface/TrackerValidationVariables.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"



// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


// For ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TVector3.h>

#define HISTOS
#define L1
#define HLT


using namespace std;

// ----------------------------------------------------------------------
class PixDigisTestUL : public edm::EDAnalyzer {

public:

  explicit PixDigisTestUL(const edm::ParameterSet&);
  ~PixDigisTestUL();
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(); 

private:
  map<string, TH1*> fHistMap;
  vector<unsigned int> fNewList; 
  bool PRINT;
  edm::EDGetTokenT<edm::DetSetVector<PixelDigi>> tPixelDigi;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>> tPixelCluster;
  edm::EDGetTokenT<TrajTrackAssociationCollection> TTAToken;

  edm::InputTag src_;  
  edm::InputTag srcCluster_;  
};

// ----------------------------------------------------------------------
PixDigisTestUL::PixDigisTestUL(const edm::ParameterSet& iConfig) {
  PRINT = iConfig.getUntrackedParameter<bool>("Verbosity",false);
  src_ =  iConfig.getParameter<edm::InputTag>( "src" );
  srcCluster_ =  iConfig.getParameter<edm::InputTag>( "srcCluster" );
  tPixelDigi = consumes <edm::DetSetVector<PixelDigi> > (src_);
  tPixelCluster = consumes <edmNew::DetSetVector<SiPixelCluster> > (srcCluster_);
  TTAToken =consumes  <TrajTrackAssociationCollection> (edm::InputTag("ctfRefitter"));
}

// ----------------------------------------------------------------------
PixDigisTestUL::~PixDigisTestUL() {
  cout<<" Destroy PixDigisTestUL "<<endl;
}

// ----------------------------------------------------------------------
void PixDigisTestUL::beginJob() {
  using namespace edm;
  PRINT = true; 
  cout << "Initialize PixDigisTestUL," 
       << " src = " << src_.label() 
       << " srcCluster = " << srcCluster_.label() 
       << " PRINT = " << PRINT
       << endl;

  edm::Service<TFileService> fs;
  fHistMap.insert(make_pair("gall1", fs->make<TH2F>("gall1", "gall1", 1500, -15., 15., 1500, -15., 15.)));
  fHistMap.insert(make_pair("gall2", fs->make<TH2F>("gall2", "gall2", 1500, -15., 15., 1500, -15., 15.)));

  fNewList.push_back(344130820);
  fNewList.push_back(344131076);
  fNewList.push_back(344131844);
  fNewList.push_back(344132100);
  fNewList.push_back(344132868);
  fNewList.push_back(344133124);
  fNewList.push_back(344133892);
  fNewList.push_back(344134148);

  cout << "HELLO WORLD: ";
  for (unsigned int i = 0; i < fNewList.size(); ++i) cout << fNewList[i] << " "; 
  cout << endl;


  // Histos go to a subdirectory "PixRecHits")
  //TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
  //TFileDirectory subSubDir = subDir.mkdir( "mySubSubDirectory" );

   // put here whatever you want to do at the beginning of the job
  //hFile = new TFile ( "histo.root", "RECREATE" );
     
}

// ----------------------------------------------------------------------
void PixDigisTestUL::analyze(const edm::Event& iEvent, 
			   const edm::EventSetup& iSetup) {

  edm::Service<TFileService> fs;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<IdealGeometryRecord>().get(tTopo);

  using namespace edm;


  //  int run       = iEvent.id().run();
  int event     = iEvent.id().event();

  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get(geom);
  const TrackerGeometry& theTracker(*geom);

  cout << "------- event " << event << "---------------------------------------------------------" << endl;
  // -- digis
  edm::Handle< edm::DetSetVector<PixelDigi> > pixelDigis;
  iEvent.getByToken(tPixelDigi, pixelDigis);
  cout << "--- digis: " << pixelDigis->size() << endl;

  bool found(false);

  // Iterate on detector units
  {
    edm::DetSetVector<PixelDigi>::const_iterator DSViter;
    for (DSViter = pixelDigis->begin(); DSViter != pixelDigis->end(); DSViter++) {
      unsigned int detid = DSViter->id; // = rawid
      DetId detId(detid);
      unsigned int detType=detId.det(); // det type, tracker=1
      unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
      
      //      cout << "detType = " << detType << endl;
     
      if(detType!=1) continue; // look only at tracker

      // -- only look at pilot blade modules
      found = false;
      for (unsigned int i = 0; i < fNewList.size(); ++i) {
	if (detid == fNewList[i]) {
	  found = true; 
	  break;
	}
      }
      //      if (!found) continue; 
      if (found) {

      }

      // Get the geom-detector 
      const PixelGeomDetUnit * theGeomDet =   dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) );
      LocalPoint lp(0., 0., 0.); 
      GlobalPoint gp0 = theGeomDet->surface().toGlobal(lp);
      TVector3 t3(gp0.x(), gp0.y(), gp0.z()); 
      if(PRINT) 
	cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid
	    << "x/y/z = " << gp0.x() << "/" << gp0.y() << "/" << gp0.z() 
	    << " eta = " << t3.Eta()
	    <<endl;
      
      
      const PixelTopology &topology = theGeomDet->specificTopology(); 
      int cols = topology.ncolumns();
      int rows = topology.nrows();
      float pitchX = topology.pitch().first;
      float pitchY = topology.pitch().second;
      
      unsigned int layerC=0;
      unsigned int ladderC=0;
      unsigned int zindex=0;
      
      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      int shell  = 0; // shell id 
      int sector = 0; // 1-8
      int ladder = 0; // 1-22
      int layer  = 0; // 1-3
      int module = 0; // 1-4
      bool half  = false; // 
      
      unsigned int disk=0; //1,2,3
      unsigned int blade=0; //1-24
      unsigned int side=0; //size=1 for -z, 2 for +z
      unsigned int panel=0; //panel=1
      
      // Subdet it, pix barrel=1, forward=2
      if(subid==2) {   // forward
	
	disk=tTopo->pxfDisk(detid); //1,2,3
	blade=tTopo->pxfBlade(detid); //1-24
	zindex=tTopo->pxfModule(detid); //
	side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
	panel=tTopo->pxfPanel(detid); //panel=1
	
	if(PRINT) {
	  cout<<"Forward det "<<subid<<", disk "<<disk<<", blade "
	      <<blade<<", module "<<zindex<<", side "<<side<<", panel "
	      <<panel<<endl;
	  cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	      <<pitchX<<" "<<pitchY<<endl;
	}
	
      } else if(subid == 1) { // Barrel 
	
	// Barell layer = 1,2,3
	layerC=tTopo->pxbLayer(detid);
	// Barrel ladder id 1-20,32,44.
	ladderC=tTopo->pxbLadder(detid);
	// Barrel Z-index=1,8
	zindex=tTopo->pxbModule(detid);
	// Convert to online 
	PixelBarrelName pbn(detid);
	// Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
	PixelBarrelName::Shell sh = pbn.shell(); //enum
	sector = pbn.sectorName();
	ladder = pbn.ladderName();
	layer  = pbn.layerName();
	module = pbn.moduleName();
	half  = pbn.isHalfModule();
	shell = int(sh);
	// change the module sign for z<0
	if(shell==1 || shell==2) module = -module;
	// change ladeer sign for Outer )x<0)
	if(shell==1 || shell==3) ladder = -ladder;
	
	
	if(PRINT) { 
	  cout<<" Barrel layer, ladder, module "
	      <<layerC<<" "<<ladderC<<" "<<zindex<<" "
	      <<sh<<"("<<shell<<") "<<sector<<" "<<layer<<" "<<ladder<<" "
	      <<module<<" "<<half<< endl;
	  //cout<<" Barrel det, thick "<<detThick<<" "
	  //  <<" layer, ladder, module "
	  //  <<layer<<" "<<ladder<<" "<<zindex<<endl;
	  //cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	  //  <<pitchX<<" "<<pitchY<<endl;
	}      
	
      } // end fb-bar
      
      unsigned int numberOfDigis = 0;
      
      // -- Look at digis now
      edm::DetSet<PixelDigi>::const_iterator  di;
      for(di = DSViter->data.begin(); di != DSViter->data.end(); di++) {
	numberOfDigis++;
	int adc = di->adc();    // charge, modifued to unsiged short 
	int col = di->column(); // column 
	int row = di->row();    // row
	int channel = PixelChannelIdentifier::pixelToChannel(row,col);
	if(PRINT) cout <<numberOfDigis<< " Col: " << col << " Row: " << row 
		       << " ADC: " << adc <<" channel = "<<channel<<endl;
      } // end for digis in detunit
    } // end for det-units
  }


  // Get Cluster Collection from InputTag
  {
    edmNew::DetSet<SiPixelCluster>::const_iterator clustIt;
    edm::Handle< edmNew::DetSetVector<SiPixelCluster> > clusters;
    iEvent.getByToken(tPixelCluster, clusters);
    const edmNew::DetSetVector<SiPixelCluster>& input = *clusters;     
    cout << "--- clusters: " << input.size() << endl;
    edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=input.begin();
    for ( ; DSViter != input.end() ; DSViter++) {

      unsigned int detid = DSViter->detId();

      unsigned int blade = tTopo->pxfBlade(detid); 
      unsigned int panel = tTopo->pxfPanel(detid);
      
      // -- only look at pilot blade modules
      found = false;
      for (unsigned int i = 0; i < fNewList.size(); ++i) {
	if (detid == fNewList[i]) {
	  found = true; 
	  break;
	}
      }
      //      if (!found) continue; 

      if (0 == fHistMap.count(Form("H%d", detid))) {
	cout << "creating hitmap for detid = " << detid << endl;
	fHistMap.insert(make_pair(Form("H%d", detid), fs->make<TH2F>(Form("H%d", detid), Form("H%d", detid), 160, 0., 160., 416, 0., 416.))); 
      }

      if (0 == fHistMap.count(Form("G%d", detid))) {
	cout << "creating hitmap for detid = " << detid << endl;
	fHistMap.insert(make_pair(Form("G%d", detid), fs->make<TH2F>(Form("G%d", detid), Form("G%d", detid), 1500, -15., 15., 1500, -15., 15.))); 
      }

      if (0 == fHistMap.count(Form("z%d", detid))) {
	cout << "creating z hist for detid = " << detid << endl;
	fHistMap.insert(make_pair(Form("z%d", detid), fs->make<TH1F>(Form("z%d", detid), Form("z%d", detid), 1000, 0., 100.))); 
      }

      DetId detId = DetId(detid);       // Get the Detid object
      unsigned int detType=detId.det(); // det type, pixel=1
      unsigned int subid=detId.subdetId(); //subdetector type, barrel=1

      const PixelGeomDetUnit *pgdu = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
      double detZ = pgdu->surface().position().z();
      double detR = pgdu->surface().position().perp();


      for (clustIt = DSViter->begin(); clustIt != DSViter->end(); clustIt++) {
	double clX = clustIt->x();
	double clY = clustIt->y();

	Surface::GlobalPoint gp = pgdu->surface().toGlobal(pgdu->specificTopology().localPosition(
				  MeasurementPoint(clX, clY)));
	double gX = gp.x();
	double gY = gp.y();
	double gZ = gp.z();


	cout << "  new cluster: " << clX << ", " << clY << " global = " << gX << ", " << gY << ", " << gZ << endl;

	const vector<SiPixelCluster::Pixel>& pixelsVec = clustIt->pixels();
	for (unsigned int i = 0;  i < pixelsVec.size(); ++i) { // loop over pixels
	  float pixx = pixelsVec[i].x; // index as float=iteger, row index, 0-159
	  float pixy = pixelsVec[i].y; // same, col index, 0-415
	  //	  float adc = (float(pixelsVec[i].adc)/1000.);

	  Surface::GlobalPoint gpp = pgdu->surface().toGlobal(pgdu->specificTopology().localPosition(
				    MeasurementPoint(pixx, pixy)));

	  double gpX = gpp.x();
	  double gpY = gpp.y();
	  double gpZ = gpp.z();
	  int channel = PixelChannelIdentifier::pixelToChannel(pixx, pixy);

	  cout << "  detid = " << detid
	       << " panel = " << panel << " blade = " << blade 
	       << " cluster global x/y/z = " << gX << "/" << gY << "/" << gZ << "(" << gpZ << ")"
	       << " pixel global x/y = " << gpX << "/" << gpY 
	       << " local x/y = " << pixx << "/" << pixy 
	       << " channel = " << channel
	       << endl;

	  continue;

	  TH2F *h2 = (TH2F*)fHistMap[Form("H%d", detid)]; 
	  int ibin = h2->FindBin(pixx, pixy); 
	  h2->SetBinContent(ibin, channel);

	  h2 = (TH2F*)fHistMap[Form("G%d", detid)]; 
	  ibin = h2->FindBin(gpX, gpY); 
	  h2->SetBinContent(ibin, channel);

	  fHistMap[Form("z%d", detid)]->Fill(TMath::Abs(gpZ));


	  h2 = (TH2F*)fHistMap[Form("gall%d", panel)]; 
	  cout << "h2 = " << h2 << " panel = " << panel << " gpX = " << gpX << " gpY = " << gpY << endl;
	  ibin = h2->FindBin(gpX, gpY); 
	  cout << "ibin = " << ibin << endl;
	  if (ibin > 0) h2->SetBinContent(ibin, channel);
	  
	}
	//      const PixelTopology *topol = &(theGeomDet->specificTopology());
	
	
	cout << "Det: "<<detId.rawId() << " " << detType << " " << subid 
	     << " detZ = " << detZ  << " detR = " << detR
	     << " npixel = " << pixelsVec.size()
	     << endl;    
      }
      
    }
  }



  
  if (0) {
  // -- Track trajectory association map
  edm::Handle<TrajTrackAssociationCollection> hTTAC;
  iEvent.getByToken(TTAToken, hTTAC);
  cout << "===========> trajectory collection size: " << hTTAC->size() << endl;
  
  TrajectoryStateCombiner tsoscomb;
  if (hTTAC.isValid()) {
    const TrajTrackAssociationCollection ttac = *(hTTAC.product());
    cout << "   hTTAC.isValid()" << endl;
    for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
      
      cout << "      TracjTrackAssociationCollection iterating" << endl;
      const edm::Ref<std::vector<Trajectory> > refTraj = it->key;  
      reco::TrackRef trackref = it->val;
      
      // -- Clusters associated with a track
      std::vector<TrajectoryMeasurement> tmeasColl = refTraj->measurements();
      for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = tmeasColl.begin(); tmeasIt!=tmeasColl.end(); tmeasIt++){   
	if (!tmeasIt->updatedState().isValid()) continue; 
	TrajectoryStateOnSurface tsos = tsoscomb(tmeasIt->forwardPredictedState(), tmeasIt->backwardPredictedState());
      }
    }
  }
  }

}

// ----------------------------------------------------------------------
void PixDigisTestUL::endJob(){
  cout << " End PixDigisTestUL " << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(PixDigisTestUL);
