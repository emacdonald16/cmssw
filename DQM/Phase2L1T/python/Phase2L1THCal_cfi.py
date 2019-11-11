import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
Phase2L1THCalVal = DQMEDAnalyzer('Phase2L1THCal',
    TopFolderName = cms.string('Phase2L1T'),
    digiTag	= cms.InputTag("simHcalDigis"),
    dataTPs = cms.InputTag("simHcalTriggerPrimitiveDigis"), #this is included in all EDM files

    tp_hl_et = cms.PSet(
        Nbinsx = cms.int32(260),
        xmin = cms.double(-10.0),
        xmax = cms.double(250.0)
        ),

    tp_hl_ntp = cms.PSet(
        Nbinsx = cms.int32(640),
        xmin = cms.double(-20.0),
        xmax = cms.double(3180.0)
        ),

    tp_hl_ntp_sub = cms.PSet(
        Nbinsx = cms.int32(404),
        xmin = cms.double(-20.0),
        xmax = cms.double(2000.0)
        ),

    tp_hl_ieta = cms.PSet(
        Nbinsx = cms.int32(85),
        xmin = cms.double(-42.5),
        xmax = cms.double(42.5)
        ),

    tp_hl_iphi = cms.PSet(
        Nbinsx = cms.int32(74),
        xmin = cms.double(-0.5),
        xmax = cms.double(73.5)
        ),

    tp_hl_ieta_et = cms.PSet( #ieta is x, et is y
        Nbinsx = cms.int32(85),
        xmin = cms.double(-42.5),
        xmax = cms.double(42.5),
        Nbinsy = cms.int32(260),
        ymin = cms.double(-10.0),
        ymax = cms.double(250.0)
        ),

    tp_hl_ieta_iphi = cms.PSet( #ieta is x, iphi is y
        Nbinsx = cms.int32(85),
        xmin = cms.double(-42.5),
        xmax = cms.double(42.5),
        Nbinsy = cms.int32(74),
        ymin = cms.double(-0.5),
        ymax = cms.double(73.5)
        ),
)
