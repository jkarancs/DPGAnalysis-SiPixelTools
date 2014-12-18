##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2015_cff')
process.load("Geometry.CMSCommonData.cmsExtendedGeometry2015PilotXML_cfi")
#process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Geometry.TrackerGeometryBuilder.trackerGeometry_cfi')
process.trackerGeometry.applyAlignment = cms.bool(False)

# from v7
#process.load("SimGeneral.MixingModule.pixelDigitizer_cfi")
# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")

#process.load("SimGeneral.MixingModule.mixNoPU_cfi")
from SimGeneral.MixingModule.aliases_cfi import * 
from SimGeneral.MixingModule.mixObjects_cfi import *
# from SimGeneral.MixingModule.digitizers_cfi import *
from SimGeneral.MixingModule.pixelDigitizer_cfi import *
from SimGeneral.MixingModule.stripDigitizer_cfi import *
from SimGeneral.MixingModule.trackingTruthProducer_cfi import *

process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
process.siPixelClusters.src = cms.InputTag("mix")
process.siPixelClusters.MissCalibrate = cms.untracked.bool(False)

process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
process.PixelCPEGenericESProducer.UseErrorsFromTemplates = cms.bool(False)
process.PixelCPEGenericESProducer.TruncatePixelCharge = cms.bool(False)
process.PixelCPEGenericESProducer.IrradiationBiasCorrection = cms.bool(False)
process.PixelCPEGenericESProducer.LoadTemplatesFromDB = cms.bool(False)
process.PixelCPEGenericESProducer.DoCosmics = cms.bool(False)
process.PixelCPEGenericESProducer.lAOffset = cms.double(1.0)
process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(1.0)
process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(1.0)
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

## process.MessageLogger = cms.Service("MessageLogger",
##        destinations   = cms.untracked.vstring(
##                                              'debugInfo'
## ),

##        debugInfo   = cms.untracked.PSet(
##                       threshold  = cms.untracked.string('DEBUG'),
##                       default = cms.untracked.PSet(limit = cms.untracked.int32(-1))
##        ),
##    debugModules = cms.untracked.vstring(
##                                        'mix'
                                    
##   )
## )

process.mix = cms.EDProducer("MixingModule",
#    digitizers = cms.PSet(theDigitizers),
#    digitizers = cms.PSet(
#      mergedtruth = cms.PSet(
#            trackingParticles
#      )
#    ),

  digitizers = cms.PSet(
   pixel = cms.PSet(
    pixelDigitizer 
   ),
#  strip = cms.PSet(
#    stripDigitizer
#  ),
  ),

#theDigitizersValid = cms.PSet(
#  pixel = cms.PSet(
#    pixelDigitizer
#  ),
#  strip = cms.PSet(
#    stripDigitizer
#  ),
#  ecal = cms.PSet(
#    ecalDigitizer
#  ),
#  hcal = cms.PSet(
#    hcalDigitizer
#  ),
#  castor = cms.PSet(
#    castorDigitizer
#  ),
#  mergedtruth = cms.PSet(
#    trackingParticles
#  )
#),

    LabelPlayback = cms.string(' '),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-5), ## in terms of 25 ns

    bunchspace = cms.int32(25),
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),
    mixObjects = cms.PSet(
        mixTracks = cms.PSet(
            mixSimTracks
        ),
        mixVertices = cms.PSet(
            mixSimVertices
        ),
        mixSH = cms.PSet(
#            mixPixSimHits
# mixPixSimHits = cms.PSet(
    input = cms.VInputTag(cms.InputTag("g4SimHits","TrackerHitsPixelBarrelHighTof"), 
                          cms.InputTag("g4SimHits","TrackerHitsPixelBarrelLowTof"),
                          cms.InputTag("g4SimHits","TrackerHitsPixelEndcapHighTof"), 
                          cms.InputTag("g4SimHits","TrackerHitsPixelEndcapLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTECHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTECLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIBHighTof"),
#                          cms.InputTag("g4SimHits","TrackerHitsTIBLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIDHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIDLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTOBHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTOBLowTof")
    ),
    type = cms.string('PSimHit'),
    subdets = cms.vstring(
        'TrackerHitsPixelBarrelHighTof',
        'TrackerHitsPixelBarrelLowTof',
        'TrackerHitsPixelEndcapHighTof',
        'TrackerHitsPixelEndcapLowTof',
#        'TrackerHitsTECHighTof',
#        'TrackerHitsTECLowTof',
#        'TrackerHitsTIBHighTof',
#        'TrackerHitsTIBLowTof',
#        'TrackerHitsTIDHighTof',
#        'TrackerHitsTIDLowTof',
#        'TrackerHitsTOBHighTof',
#        'TrackerHitsTOBLowTof'
    ),
    crossingFrames = cms.untracked.vstring(),
#        'MuonCSCHits',
#        'MuonDTHits',
#        'MuonRPCHits'),
#)   
        ),
        mixHepMC = cms.PSet(
            mixHepMCProducts
        )
    )
)

#process.mix.digitizers.pixel.GeometryType = cms.string('idealForDigi')
process.mix.digitizers.pixel.useDB = cms.bool(False)
process.mix.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
process.mix.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
process.mix.digitizers.pixel.DeadModules_DB = cms.bool(False)
process.mix.digitizers.pixel.enablePilotBlades = cms.bool(True)
#ROC killing
## process.mix.digitizers.pixel.DeadModules = cms.VPSet(
##                                                cms.PSet(Dead_detID = cms.int32(302055952), Module = cms.string("tbmA"), Dead_RocID = 
##                                                cms.vint32(1,4)) #lay1_lad4_mod-1;multiple test!z<0
##                                                ,cms.PSet(Dead_detID = cms.int32(302055956), Module = cms.string("none"),Dead_RocID = cms.vint32(6,13,10))
## #lay1_lad4_mod1;multiple test!z>0
##                                                ,cms.PSet(Dead_detID = cms.int32(302056720), Module = cms.string("none"),Dead_RocID = cms.vint32(0,7)) #half test lay1_lad1_mod1
##                                                #,cms.PSet(Dead_detID = cms.int32(302057744), Module = cms.string("none"), Dead_RocID = 
##                                                #cms.int32(14)) #lay1_lad-4_mod-1;multiple test
                                              
##                                                ,cms.PSet(Dead_detID = cms.int32(344019464), Module = cms.string("none"), Dead_RocID = 
##                                                cms.vint32(0,1,2)) #disk -1 Blade 11, panel 2, Module 2 multiple test
##                                                ,cms.PSet(Dead_detID = cms.int32(302056220), Module = cms.string("none"), Dead_RocID = 
##                                                cms.vint32(-1)) #lay1 lad6 mod-2 multiple test
##                                                 )


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#     simMuonCSCDigis = cms.PSet(
#        initialSeed = cms.untracked.uint32(1234567),
#        engineName = cms.untracked.string('TRandom3')
#    ),
     mix = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
   )
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
    #'file:/data/hazia/digitizer/CMSSW_7_0_0_pre9/src/simHitsD.root'
    #'file:/data/store/relval/CMSSW_6_2_0_pre2-START61_V11_g496p1/RelValMinBias/GEN-SIM/v2/00000/9E4FF022-177C-E211-BAD6-003048D37580.root'
#    'file:./minb_gensim_psimhittestpilotMu100k.root'
#    'file:./SingleMuPt100_cfi_py_GEN_SIM100k.root'
    'file:./minb_gensim_pilotblade.root'
  )
)

# Choose the global tag here:
# for v7.0
#process.GlobalTag.globaltag = 'MC_70_V1::All'
#process.GlobalTag.globaltag = "PRE_STA71_V2::All"
#process.GlobalTag.globaltag = 'START62_V1::All'
process.GlobalTag.globaltag = 'START71_V1::All'

#change to local tag
## import CalibTracker.Configuration.Common.PoolDBESSource_cfi

## process.myLocalDB = CalibTracker.Configuration.Common.PoolDBESSource_cfi.poolDBESSource.clone(
##        connect = cms.string (
##         'sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERCALIB/Pixels/PixelDB2013/SiPixelQuality/SiPixelQuality_620p5/builder/deadModule_v20.db'
##         ),
##         toGet = cms.VPSet(cms.PSet(
##                 record = cms.string('SiPixelQualityFromDbRcd'),
##                 tag = cms.string('SiPixelQuality_v20')
##                                   )
##                          )
##         )
## process.es_prefer_Quality = cms.ESPrefer("PoolDBESSource","myLocalDB")


process.o1 = cms.OutputModule("PoolOutputModule",
      #      outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Test'),
      fileName = cms.untracked.string('file:digisclusters_minb_gensim_pilotblade.root')
#      fileName = cms.untracked.string('file:dummy.root')
)

process.g4SimHits.Generator.HepMCProductLabel = 'source'

# modify digitizer parameters
#process.simSiPixelDigis.ThresholdInElectrons_BPix = 3500.0 
#process.mix.digitizers.pixel.ThresholdInElectrons_BPix = 3500.0 

#This process is to run the digitizer, pixel gitizer is now clled by the mix module


#######
#######
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digicluster_histos.root')
)
process.analyzeDigis = cms.EDAnalyzer("PixelDigisTest",
    Verbosity = cms.untracked.bool(False),
# sim in V7
    src = cms.InputTag("mix"),
# old default
#    src = cms.InputTag("siPixelDigis"),
)

process.analyzeClusters = cms.EDAnalyzer("ReadPixClusters",
    Verbosity = cms.untracked.bool(True),
    src = cms.InputTag("siPixelClusters"),
)

process.analyzeRecHits = cms.EDAnalyzer("ReadPixelRecHit",
    Verbosity = cms.untracked.bool(True),
    src = cms.InputTag("siPixelRecHits"),
)

#ul process.p = cms.Path(process.mix*process.siPixelClusters*process.siPixelRecHits*process.analyzeDigis*process.analyzeClusters*process.analyzeRecHits)

process.p = cms.Path(process.mix*process.analyzeDigis)

#########
#########

#process.p1 = cms.Path(process.mix*process.siPixelClusters*process.siPixelRecHits)

process.outpath = cms.EndPath(process.o1)


