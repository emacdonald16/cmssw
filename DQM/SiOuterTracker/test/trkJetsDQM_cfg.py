#This uses DQMsaver, the output will look like: DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root
#The DQM script requires track Jets

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("TkJetsDQMPlots")


process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.DQMSaverAtRunEnd_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )


#Input file
readFiles = cms.untracked.vstring(
#"/store/relval/CMSSW_9_3_7/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/886F551E-7E2C-E811-9D10-0025905A608C.root"
"file:/mnt/hadoop/store/user/rish/MinBiasPU200/reprocessMinBias10.root"

)
secFiles = cms.untracked.vstring()

process.source = cms.Source ("PoolSource",
                            fileNames = readFiles,
                            secondaryFileNames = secFiles,
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            # Following needed to read CMSSW 9 datasets with CMSSW 10
                            inputCommands = cms.untracked.vstring(
                              'keep *_*_*_*',
                              'drop l1tEMTFHit2016*_*_*_*',
                              'drop l1tEMTFTrack2016*_*_*_*')
                            )

process.load('DQM.SiOuterTracker.TestSourceConfig_cff')
#from DQM.SiOuterTracker.OTTrackJetsandMHT_cfi import *

process.DQM_trkJets = cms.Path(process.TestTkJetsDQM)

process.dqmsave_step = cms.Path(process.DQMSaver)

process.schedule = cms.Schedule(process.DQM_trkJets,process.dqmsave_step)
#process.schedule = cms.Schedule(process.tracks, process.validation_OuterTracker, process.dqmoffline_OuterTracker, process.postValidationOuterTracker_step, process.dqmsave_step)
