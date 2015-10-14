import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("ClusTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('HLTrigReport')

#process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("Geometry.CMSCommonData.cmsExtendedGeometry2015PilotXML_cfi")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# requires >= CMSSW_7_4_10
process.GlobalTag.globaltag = "74X_dataRun2_Prompt_v2"

process.options   = cms.untracked.PSet( 
  wantSummary = cms.untracked.bool(True)
  , SkipEvent = cms.untracked.vstring('ProductNotFound')
  )


# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# for raw
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")


process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
  )

process.MessageLogger = cms.Service(
  "MessageLogger",
  debugModules = cms.untracked.vstring('SiPixelClusterizer'),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string(
      'WARNING')
    )
  )

# get the files from DBS:
process.source = cms.Source(
  "PoolSource",
  fileNames = cms.untracked.vstring(
    '/store/user/ursl/files/258415/Cosmics/2073D30A-546C-E511-96A5-02163E014268.root'
    , '/store/user/ursl/files/258415/Cosmics/7A07636E-546C-E511-B7A0-02163E0125F8.root' 
    )
  )

# a service to use root histos
process.TFileService = cms.Service(
  "TFileService", fileName = cms.string('histo.root')
  )

process.CablingReader = cms.ESSource(
  "PoolDBESSource",
  BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
  DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0),
    authenticationPath = cms.untracked.string('')
    ),
  toGet = cms.VPSet(
    cms.PSet(
      record = cms.string('SiPixelFedCablingMapRcd'),
      tag = cms.string('SiPixelFedCablingMap_v21')
      )
    ),
  connect = cms.string('sqlite_file:cabling.db')
  )


# ---------------------- pilotBlade Geometry -------------------
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.Geometry.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Geometry.CMSCommonData.cmsExtendedGeometry2015PilotXML_cfi")
process.trackerGeometryDB.applyAlignment = cms.bool(False)
process.XMLFromDBSource.label=''
process.PoolDBESSourceGeometry = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.string('runnumber'),
        toGet = cms.VPSet(
                cms.PSet(
                        record = cms.string('GeometryFileRcd'),
                        tag = cms.string('XMLFILE_Geometry_74YV2_Extended2015_mc')
                ),
                cms.PSet(
                        record = cms.string('IdealGeometryRecord'),
                        tag = cms.string('TKRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PGeometricDetExtraRcd'),
                        tag = cms.string('TKExtra_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PTrackerParametersRcd'),
                        tag = cms.string('TKParameters_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PEcalBarrelRcd'),
                        tag = cms.string('EBRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PEcalEndcapRcd'),
                        tag = cms.string('EERECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PEcalPreshowerRcd'),
                        tag = cms.string('EPRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PHcalRcd'),
                        tag = cms.string('HCALRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PCaloTowerRcd'),
                        tag = cms.string('CTRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PZdcRcd'),
                        tag = cms.string('ZDCRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('PCastorRcd'),
                        tag = cms.string('CASTORRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('CSCRecoGeometryRcd'),
                        tag = cms.string('CSCRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('CSCRecoDigiParametersRcd'),
                        tag = cms.string('CSCRECODIGI_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('DTRecoGeometryRcd'),
                        tag = cms.string('DTRECO_Geometry_74YV2')
                ),
                cms.PSet(
                        record = cms.string('RPCRecoGeometryRcd'),
                        tag = cms.string('RPCRECO_Geometry_74YV2')
                )
        ),
        connect = cms.string('sqlite_file:./PilotGeometry.db') 
)
process.es_prefer_geometry = cms.ESPrefer( "PoolDBESSource", "PoolDBESSourceGeometry" )
#-------------------------------------------------------


process.es_prefer_CablingReader = cms.ESPrefer("PoolDBESSource","CablingReader")

process.siPixelDigis.UsePilotBlade = cms.bool(True)
process.siPixelDigis.InputLabel = 'rawDataCollector'

process.siStripDigis.ProductLabel = 'rawDataCollector'

process.siPixelClusters.MissCalibrate = cms.untracked.bool(False)
process.siPixelClusters.src = cms.InputTag("siPixelDigis")

process.ana = cms.EDAnalyzer(
  "PixDigisTestUL",
  Verbosity = cms.untracked.bool(True),
  src = cms.InputTag("siPixelDigis"),
  srcCluster = cms.InputTag("siPixelClusters"),
  )

process.p1 = cms.Path(
  process.siPixelDigis
#  *process.siStripDigis
#  *process.trackerlocalreco
  *process.ana
  )

