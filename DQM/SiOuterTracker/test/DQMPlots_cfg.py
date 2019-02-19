#This uses DQMsaver, the output will look like: DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root
#The DQM script requires clusters, stubs, tracks, and all 3 association maps

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("DQMPlots")

#Choose the process
TrackReco = "Tracklet_Emulation"
#TrackReco = "Tracklet"

process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.DQMSaverAtRunEnd_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('DQM.SiOuterTracker.OuterTrackerSourceConfig_cff')
process.load('Validation.SiOuterTrackerV.OuterTrackerSourceConfigV_cff')
process.load('Validation.SiOuterTrackerV.SiOuterTrackerMCHarvesting_cff')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

#Input file
readFiles = cms.untracked.vstring(
#'/store/relval/CMSSW_9_3_7/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_93X_upgrade2023_realistic_v5_2023D17PU200-v1/10000/5A8CFF7F-1E2D-E811-A7B0-0242AC130002.root'
"/store/relval/CMSSW_9_3_7/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/886F551E-7E2C-E811-9D10-0025905A608C.root"
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

#The DQM script requires clusters, stubs, tracks, and all 3 association maps
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')
from L1Trigger.TrackTrigger.TTStubAlgorithmRegister_cfi import *

from SimTracker.TrackTriggerAssociation.TTClusterAssociation_cfi import *
process.TTClusterAssociatorFromPixelDigis.digiSimLinks = cms.InputTag("simSiPixelDigis","Tracker")

process.load('L1Trigger.VertexFinder.VertexProducer_cff')
process.VertexProducer.l1TracksInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks")
process.vertex_step = cms.Path(process.VertexProducer)

if TrackReco == "Tracklet_Emulation":
    print "Using Tracklet emulation track reconstruction"
    process.load("L1Trigger.TrackFindingTracklet.L1TrackletEmulationTracks_cff")
    #process.tracks = cms.Path(process.TrackTriggerClustersStubs * process.TrackTriggerAssociatorClustersStubs * process.L1TrackletEmulationTracksWithAssociators)
    process.tracks = cms.Path(process.L1TrackletEmulationTracksWithAssociators)

    process.DQM_trkParticles = process.OuterTrackerMonitorTrackingParticles.clone(
        TTTracksTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
        MCTruthTrackInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigis", "Level1TTTracks"),

    )
    process.DQM_tracks = process.OuterTrackerMonitorTTTrack.clone(
        TTTracksTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    )
    process.validation_OuterTracker = cms.Path(process.DQM_trkParticles*process.OuterTrackerTkMETV)
    #process.validation_OuterTracker = cms.Path(process.OuterTrackerSourceV)


    process.dqmoffline_OuterTracker = cms.Path(process.OuterTrackerMonitorTTCluster*process.OuterTrackerMonitorTTStub*process.DQM_tracks*process.OuterTrackerTkMET)


elif TrackReco == "Tracklet":
    print "Using Tracklet floating point simulation track reconstruction"
    process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
    process.tracks = cms.Path(process.L1TrackletTracksWithAssociators)

    #if you need to re-run clusters/stubs, or their association maps
    #process.tracks = cms.Path(process.TrackTriggerClustersStubs * process.TrackTriggerAssociatorClustersStubs * process.L1TrackletTracksWithAssociators)
    process.validation_OuterTracker = cms.Path(process.OuterTrackerSourceV)
    process.dqmoffline_OuterTracker = cms.Path(process.OuterTrackerSource)

else:
    print "This is not a track reconstruction!!!"

from Validation.SiOuterTrackerV.SiOuterTrackerMCHarvesting_cff import *
process.postValidationOuterTracker_step = cms.Path(process.OuterTracker_harvestingV)
process.dqmsave_step = cms.Path(process.DQMSaver)

process.schedule = cms.Schedule(process.tracks, process.vertex_step,process.dqmoffline_OuterTracker, process.validation_OuterTracker, process.postValidationOuterTracker_step, process.dqmsave_step)
#process.schedule = cms.Schedule(process.tracks, process.validation_OuterTracker, process.dqmoffline_OuterTracker, process.postValidationOuterTracker_step, process.dqmsave_step)
