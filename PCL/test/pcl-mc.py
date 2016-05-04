# ----------------------------------------------------------------------
# -- RECO py template file for running pixel PCL
# ----------------------------------------------------------------------

import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("PCL")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)

# -- Database configuration
process.load("CondCore.CondDB.CondDB_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# -- Input files
# POOLSOURCE

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	"/store/user/ursl/files/pcl/relval-803/4C998900-B9EF-E511-9CF0-0CC47A745298.root"
    )
)


# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(300)
)

process.pcl = cms.EDAnalyzer(
    "PixelPCL",
    verbose            = cms.untracked.int32(0),
    updateMaps         = cms.untracked.bool(False),
    pixelClusterLabel  = cms.untracked.InputTag('siPixelClusters::RECO')
)

# -- Path
process.p = cms.Path(
    process.pcl
)
