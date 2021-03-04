import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
Phase2L1TCSCVal = DQMEDAnalyzer('Phase2L1TCSC',

    TopFolderName = cms.string('Phase2L1T'),
    EMTFInputTag = cms.InputTag("emtfStage2Digis"), #maybe simEmtfDigis?

    tp_eta = cms.PSet(
        Nbinsx = cms.int32(45),
        xmin = cms.double(-3.0),
        xmax = cms.double(3.0)
    ),

    tp_phi = cms.PSet(
        Nbinsx = cms.int32(60),
        xmin = cms.double(-180),
        xmax = cms.double(180)
    ),

    tp_pattern = cms.PSet(
        Nbinsx = cms.int32(11),
        xmin = cms.double(0),
        xmax = cms.double(11)
    ),

    tp_bx = cms.PSet(
        Nbinsx = cms.int32(7),
        xmin = cms.double(-3),
        xmax = cms.double(4)
    ),

    tp_bend = cms.PSet(
        Nbinsx = cms.int32(2),
        xmin = cms.double(0),
        xmax = cms.double(2)
    ),
)
