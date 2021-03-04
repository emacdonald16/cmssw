import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
Phase2L1TGEMVal = DQMEDAnalyzer('Phase2L1TGEM',

    TopFolderName = cms.string('Phase2L1T'),
    EMTFInputTag = cms.InputTag("emtfStage2Digis"), #assuming GEM will be added to this

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

)
