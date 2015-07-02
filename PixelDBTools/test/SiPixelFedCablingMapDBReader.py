import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelCablingReader")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PRE_MC_71_V5::All'

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)


###### OUTPUT HISTOGRAM FILE NAME #######
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(10),
    firstRun = cms.untracked.uint32(1)
)

####process.Timing = cms.Service("Timing")
####process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
####    ignoreTotal = cms.untracked.int32(0)
####)


##### DATABASE CONNECTION INFO ######
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect='sqlite_file:../../../CalibTracker/SiPixelConnectivity/test/cabling_v17.db'
process.CondDBCommon.DBParameters.authenticationPath = '.'
process.CondDBCommon.DBParameters.messageLevel = 1


###### TAGS TO READ ######
process.PoolDBESSourceForReader = cms.ESSource("PoolDBESSource",
    process.CondDBCommon,
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelFedCablingMapRcd'),
            tag = cms.string('SiPixelFedCablingMap_v17')
       ))
)

###### PREFER ABOVE TAGS #######
process.esprefer_DBReaders = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceForReader")


####### CABLING MAP READER ######
process.SiPixelFedCablingMapAnalyzer = cms.EDAnalyzer("SiPixelFedCablingMapDBReader")

process.p = cms.Path(
process.SiPixelFedCablingMapAnalyzer
)


