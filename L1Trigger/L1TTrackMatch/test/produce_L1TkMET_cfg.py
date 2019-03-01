############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TrackMET")
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff') ## this needs to match the geometry you are running on
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')     ## this needs to match the geometry you are running on

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')



from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

############################################################
# input and output
############################################################


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5))
Source_Files = cms.untracked.vstring(
"/store/relval/CMSSW_9_3_7/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_93X_upgrade2023_realistic_v5_2023D17PU200-v1/10000/FCC765BD-332D-E811-A1F2-0242AC130002.root"
)
process.load("L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff")
process.TTTracks = cms.Path(process.L1TrackletTracks)                         #run only the tracking (no MC truth associators)
process.TTTracksWithTruth = cms.Path(process.L1TrackletTracksWithAssociators)


process.source = cms.Source("PoolSource", fileNames = Source_Files,
                            inputCommands = cms.untracked.vstring(
                              'keep *_*_*_*',
                              'drop l1tEMTFHit2016*_*_*_*',
                              'drop l1tEMTFTrack2016*_*_*_*')
                            )

process.load("L1Trigger.VertexFinder.VertexProducer_cff")
process.pPrimaryVertex = cms.Path( process.VertexProducer )

############################################################
# Tk MET
############################################################
process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissProducer_cfi")
process.L1TrackerEtMiss.L1VertexInputTag = cms.InputTag("VertexProducer","l1vertextdr")
process.pMETProducer = cms.Path(process.L1TrackerEtMiss)

process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string("test.root" )
		               )
process.FEVToutput_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.TTTracks,process.TTTracksWithTruth,process.pPrimaryVertex,process.pMETProducer,process.FEVToutput_step)
