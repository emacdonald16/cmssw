# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: repr --processName=REPR --python_filename=reprocess_test_10_5_0_pre1.py --no_exec -s L1 --datatier GEN-SIM-DIGI-RAW -n 10 --era eras.Phase2C8_trigger --eventcontent FEVTDEBUGHLT --filein root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_0_pre3/RelValMuGunPt2To100/GEN-SIM-DIGI-RAW/105X_upgrade2023_realistic_v5_2023D41noPU-v2/10000/602E0B41-B698-6340-AC68-517578FEC457.root --conditions 103X_upgrade2023_realistic_v2 --beamspot HLLHC14TeV --geometry Extended2023D41 --fileout file:step2_2ev_reprocess_slim.root
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Phase2C9_cff import Phase2C9
process = cms.Process('DQMPlotsStep3',Phase2C9)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('DQM.Phase2L1T.Phase2L1TSourceConfig_cff')

#For ECAL offline rec hits
#process.load('Configuration.StandardSequences.RawToDigi_cff')
#process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('Configuration.StandardSequences.DQMSaverAtRunEnd_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T15', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #'file:root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/230000/FA9113E3-4095-0442-A45A-D8A6F8C190ED.root'
    # 'file:/uscms_data/d3/emacdona/WorkingArea/Phase2L1TriggerDQM/ForPR/CMSSW_10_6_1_patch2/src/DTHough_Test_ForDQM.root'
    # 'file:/uscms_data/d3/emacdona/WorkingArea/Phase2L1TriggerDQM/ForPR/CMSSW_10_6_1_patch2/src/EDM_forECALTest.root'
        'file:/uscms_data/d3/emacdona/WorkingArea/L1TinCMSSW/CMSSW_11_1_7/src/L1Trigger/L1TTrackMatch/test/output_dataset.root'
),
    secondaryFileNames = cms.untracked.vstring(),
    # inputCommands = cms.untracked.vstring (
    #                                        "keep *",
    #                                        "drop l1tEMTFHitExtras_simEmtfDigis_CSC_HLT",
    #                                        "drop l1tEMTFHitExtras_simEmtfDigis_RPC_HLT",
    #                                        "drop l1tEMTFTrackExtras_simEmtfDigis__HLT",
    #                                        )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('repr nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:test.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition


# Other statements
#process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
#process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

#process.load("RecoLocalMuon.RPCRecHit.rpcRecHits_cfi")
#process.rpcRecHits.rpcDigiLabel = cms.InputTag('simMuonRPCDigis')

#process.load("L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff")
#process.tracks_step = cms.Path(process.TTTracksFromTracklet)


process.testDQM = cms.Path(process.Phase2L1TDQMSource)

process.load("DQMServices.Components.DQMEnvironment_cfi")
from DQMServices.Components.DQMEnvironment_cfi import *
dqmSaver.saveByRun = 1 
dqmSaver.saveAtJobEnd = False 
process.dqmSaver.workflow =  '/DQM/' + 'Check' + '/GTT'
process.dqmsave_step = cms.Path(process.dqmSaver)

# Path and EndPath definitions
#process.L1simulation_step = cms.Path(process.SimL1Emulator)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)


#For ECAL offline RecHits
#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.reconstruction_step = cms.Path(process.reconstruction)
#
#process.ecalDigis_step = cms.Path(process.ecalDigis)
#
#process.ecalTriggerPrimitiveDigis = cms.EDProducer("EcalTrigPrimProducer",
#    InstanceEB = cms.string('ebDigis'),
#    InstanceEE = cms.string('eeDigis'),
#    Label = cms.string('ecalDigis'),
#    BarrelOnly = cms.bool(True),
#    binOfMaximum = cms.int32(6),
#)

#process.ecalTriggerPrimitiveDigis_step = cms.Path(process.ecalTriggerPrimitiveDigis)

#process.ecalEBTriggerPrimitiveDigis = cms.EDProducer("EcalEBTrigPrimProducer",
#    BarrelOnly = cms.bool(True),
#    barrelEcalDigis = cms.InputTag('ebDigis'),
#    binOfMaximum = cms.int32(6),
#    TcpOutput = cms.bool(False),
#    Debug = cms.bool(False),
#    Famos = cms.bool(False),
#    nOfSamples = cms.int32(1),
#)

#process.ecalEBTriggerPrimitiveDigis_step = cms.Path(process.ecalEBTriggerPrimitiveDigis)

# Schedule definition
# process.schedule = cms.Schedule(process.L1simulation_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)
# process.schedule = cms.Schedule(process.L1simulation_step,process.testHCal,process.dqmsave_step)

# process.schedule = cms.Schedule(process.ecalDigis_step,process.testHCal,process.dqmsave_step)
process.schedule = cms.Schedule(
          # process.raw2digi_step,
          # process.reconstruction_step,
          # process.ecalDigis_step,
          # process.ecalTriggerPrimitiveDigis_step,
          # process.ecalEBTriggerPrimitiveDigis_step,
          # process.tracks_step,
          # process.L1simulation_step,
          process.testDQM,
          process.dqmsave_step
          # process.FEVTDEBUGHLToutput_step
          )

# process.schedule = cms.Schedule(process.raw2digi_step, process.reconstruction_step, process.ecalDigis_step,process.testHCal,process.dqmsave_step)
# process.schedule = cms.Schedule(process.raw2digi_step,process.testHCal,process.dqmsave_step)
# from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
# associatePatAlgosToolsTask(process)
#
#
# # Customisation from command line
#
# from L1Trigger.Configuration.customiseUtils import L1TrackTriggerTracklet
# process = L1TrackTriggerTracklet(process)
#
# # Add early deletion of temporary data products to reduce peak memory need
# from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
# process = customiseEarlyDelete(process)
# # End adding early deletion
