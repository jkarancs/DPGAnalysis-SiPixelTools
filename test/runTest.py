import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("ClusTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Geometry.CMSCommonData.cmsExtendedGeometry2015PilotXML_cfi")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.trackerGeometry.applyAlignment = cms.bool(False)

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# for raw
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(500)
  )

process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
    )
                                    )

# get the files from DBS:
process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring('file:./digisclusters_100kpilot.root')
                            fileNames = cms.untracked.vstring('file:./digisclusters_minb_gensim_pilotblade.root')
                            )

# a service to use root histos
process.TFileService = cms.Service("TFileService", fileName = cms.string('histo.root')
                                   )

# Choose the global tag here:
process.GlobalTag.globaltag = "MC_72_V1::All"
process.GlobalTag.globaltag = "START72_V1::All"

process.CablingReader = cms.ESSource("PoolDBESSource",
                                     BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
                                     DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0),
    authenticationPath = cms.untracked.string('')
    ),
                                     toGet = cms.VPSet(cms.PSet(
      record = cms.string('SiPixelFedCablingMapRcd'),
      tag = cms.string('SiPixelFedCablingMap_v20')
      )),
                                     connect = cms.string('sqlite_file:cabling.db')
                                     )
process.es_prefer_CablingReader = cms.ESPrefer("PoolDBESSource","CablingReader")

process.siPixelClusters.MissCalibrate = cms.untracked.bool(False)

WHAT = os.environ.get('WHAT')
if WHAT.find('POST') != -1:
  process.siPixelRawData.InputLabel = 'mix'
  process.siPixelDigis.InputLabel = 'siPixelRawData'
  process.siPixelClusters.src = 'siPixelDigis'
  process.anapost = cms.EDAnalyzer("PixDigisTestUL",
                                   Verbosity = cms.untracked.bool(True),
                                   src = cms.InputTag("siPixelDigis"),
                                   srcCluster = cms.InputTag("siPixelClusters"),
                                   )
  process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis*process.siPixelClusters*process.anapost)
else:
  process.siPixelClusters.src = 'mix'
  process.anapre = cms.EDAnalyzer("PixDigisTestUL",
                                  Verbosity = cms.untracked.bool(True),
                                  src = cms.InputTag("mix"),
                                  srcCluster = cms.InputTag("siPixelClusters")
                                  )
  process.p1 = cms.Path(process.siPixelClusters*process.anapre) 

