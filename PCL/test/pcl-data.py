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
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Express_v3', '')

# -- Input files
# POOLSOURCE

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
#	"/store/express/Run2016A/ExpressPhysics/FEVT/Express-v1/000/271/024/00000/069C34AF-C008-E611-951D-02163E014601.root"
#dbx        "/store/express/Run2016A/ExpressPhysics/FEVT/Express-v2/000/271/170/00000/00D3C1BC-9409-E611-A2C6-02163E014408.root"
#pix bad    "/store/express/Run2016A/ExpressPhysics/FEVT/Express-v2/000/271/192/00000/18307499-C709-E611-9885-02163E0145AC.root"
        "/store/user/ursl/files/pcl/272022/06D145C6-BD0D-E611-939F-02163E0146B0.root",
        "/store/user/ursl/files/pcl/272022/12513599-BF0D-E611-B019-02163E0138C1.root",
        "/store/user/ursl/files/pcl/272022/18955DD3-BD0D-E611-8EE0-02163E0143BF.root",
        "/store/user/ursl/files/pcl/272022/200534D5-BD0D-E611-BCCD-02163E012434.root",
        "/store/user/ursl/files/pcl/272022/24890197-BF0D-E611-B1F4-02163E011F69.root",
        "/store/user/ursl/files/pcl/272022/26241FD9-BD0D-E611-96DA-02163E0146B0.root",
        "/store/user/ursl/files/pcl/272022/2A329B71-BF0D-E611-B2BB-02163E01191A.root",
        "/store/user/ursl/files/pcl/272022/44EC696F-BF0D-E611-B6F2-02163E011B3F.root",
        "/store/user/ursl/files/pcl/272022/4695FEAB-C00D-E611-96C8-02163E01210E.root",
        "/store/user/ursl/files/pcl/272022/487A94E1-BD0D-E611-A28C-02163E0139C2.root",
        "/store/user/ursl/files/pcl/272022/4CB0D3EA-BD0D-E611-AA8C-02163E014196.root",
        "/store/user/ursl/files/pcl/272022/54F4383B-BC0D-E611-ADFC-02163E01433F.root",
        "/store/user/ursl/files/pcl/272022/5870DAEA-BD0D-E611-931D-02163E0129C8.root",
        "/store/user/ursl/files/pcl/272022/5E3954F2-BD0D-E611-9165-02163E0146B0.root",
        "/store/user/ursl/files/pcl/272022/66998C69-BF0D-E611-9601-02163E01373A.root",
        "/store/user/ursl/files/pcl/272022/70C74FE3-BD0D-E611-AF69-02163E014196.root",
        "/store/user/ursl/files/pcl/272022/74AB9CBD-BD0D-E611-928F-02163E011870.root",
        "/store/user/ursl/files/pcl/272022/7A0D3D68-BF0D-E611-8A39-02163E01210E.root",
        "/store/user/ursl/files/pcl/272022/84A3D447-BC0D-E611-A3B2-02163E012984.root",
        "/store/user/ursl/files/pcl/272022/8882F6AD-BF0D-E611-B6F7-02163E012511.root",
        "/store/user/ursl/files/pcl/272022/94341D74-BF0D-E611-B301-02163E011B57.root",
        "/store/user/ursl/files/pcl/272022/948A2271-BC0D-E611-AF65-02163E01210E.root",
        "/store/user/ursl/files/pcl/272022/94A8A0BF-BD0D-E611-B128-02163E011870.root",
        "/store/user/ursl/files/pcl/272022/9632D942-BC0D-E611-AD24-02163E01430F.root",
        "/store/user/ursl/files/pcl/272022/B042D742-BC0D-E611-A7D4-02163E01430F.root",
        "/store/user/ursl/files/pcl/272022/B48EFB21-BE0D-E611-B9A3-02163E0129C8.root",
        "/store/user/ursl/files/pcl/272022/B4DD676F-BF0D-E611-983D-02163E013466.root",
        "/store/user/ursl/files/pcl/272022/C0C35864-BF0D-E611-8102-02163E011AE4.root",
        "/store/user/ursl/files/pcl/272022/C0F9EF6E-BF0D-E611-96A6-02163E013967.root",
        "/store/user/ursl/files/pcl/272022/CEA54350-BC0D-E611-9D6D-02163E01210E.root",
        "/store/user/ursl/files/pcl/272022/CEE296EB-BD0D-E611-9B59-02163E0143BF.root",
        "/store/user/ursl/files/pcl/272022/D68D1057-BC0D-E611-AD0D-02163E01433F.root",
        "/store/user/ursl/files/pcl/272022/E0F9728A-BF0D-E611-9AA3-02163E014468.root",
        "/store/user/ursl/files/pcl/272022/EC55C451-BC0D-E611-99BB-02163E012984.root",
        "/store/user/ursl/files/pcl/272022/F25D5596-BF0D-E611-8504-02163E013921.root",
        "/store/user/ursl/files/pcl/272022/F4B82B7A-BC0D-E611-B40F-02163E01475E.root"
    )
)


# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
