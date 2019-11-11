import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

Phase2L1TECalVal = DQMEDAnalyzer('Phase2L1TECal',
    TopFolderName = cms.string('Phase2L1T'),
    # ECalTPs = cms.InputTag("simEcalTriggerPrimitiveDigis"),
    ECalTPs = cms.InputTag("simEcalEBTriggerPrimitiveDigis"),
    ECalRecHit = cms.InputTag("ecalRecHit","EcalRecHitsEB"),

    tp_TimingvsBx = cms.PSet(
        Nbinsx = cms.int32(7),
        xmin = cms.double(-3),
        xmax = cms.double(4),
        Nbinsy = cms.int32(100),
        ymin = cms.double(0.0),
        ymax = cms.double(100.0),
    ),

    tp_OccupancyvsBx = cms.PSet(
        Nbinsx = cms.int32(7),
        xmin = cms.double(-3),
        xmax = cms.double(4),
        Nbinsy = cms.int32(200),
        ymin = cms.double(0.0),
        ymax = cms.double(100.0),
    ),

    tp_Et = cms.PSet(
        Nbinsx = cms.int32(1023),
        xmin = cms.double(0.0),
        xmax = cms.double(1023.0),
        ),

    tp_Occupancy = cms.PSet(
        Nbinsx = cms.int32(100),
        xmin = cms.double(0.0),
        xmax = cms.double(100.0),
        ),

    tp_iEta = cms.PSet(
        Nbinsx = cms.int32(171),
        xmin = cms.double(-85.0),
        xmax = cms.double(85.0)
        ),

    tp_iPhi = cms.PSet(
        Nbinsx = cms.int32(360),
        xmin = cms.double(1.0),
        xmax = cms.double(360.0)
        ),

    tp_iEta_iPhi = cms.PSet(
        Nbinsx = cms.int32(360),
        xmin = cms.double(1.0),
        xmax = cms.double(360.0),
        Nbinsy = cms.int32(171),
        ymin = cms.double(-85.0),
        ymax = cms.double(85.0)
        ),

    tp_EtRatio = cms.PSet(
        Nbinsx = cms.int32(50),
        xmin = cms.double(-4.0),
        xmax = cms.double(4.0)
        ),

    tp_Et_TPvRecHit = cms.PSet(
        Nbinsx = cms.int32(400),
        xmin = cms.double(0.0),
        xmax = cms.double(100.0),
        Nbinsy = cms.int32(400),
        ymin = cms.double(0.0),
        ymax = cms.double(100.0)
        ),
)
