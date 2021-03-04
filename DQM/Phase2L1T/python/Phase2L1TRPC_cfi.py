import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

Phase2L1TRPCVal = DQMEDAnalyzer('Phase2L1TRPC',
    TopFolderName = cms.string('Phase2L1T'),
    RPCDigis = cms.InputTag("simMuonRPCDigis"),
    RPCRecHits = cms.InputTag("rpcRecHits"), #This is in Vladimir's sequence

    rpc_digi_strip = cms.PSet(
        Nbinsx = cms.int32(100),
        xmin = cms.double(0.0),
        xmax = cms.double(101),
        ),

    rpc_rechit_strip = cms.PSet(
        Nbinsx = cms.int32(100),
        xmin = cms.double(0.0),
        xmax = cms.double(101.0),
        ),

    rpc_rechit_size = cms.PSet(
        Nbinsx = cms.int32(20),
        xmin = cms.double(0.0),
        xmax = cms.double(20.0),
        ),

    rpc_bx = cms.PSet(
        Nbinsx = cms.int32(7),
        xmin = cms.double(-3),
        xmax = cms.double(4),
        ),

    rpc_time = cms.PSet(
        Nbinsx = cms.int32(200),
        xmin = cms.double(-100.0),
        xmax = cms.double(100.0),
        ),

    rpc_y = cms.PSet(
        Nbinsx = cms.int32(100),
        xmin = cms.double(-100.0),
        xmax = cms.double(100.0),
        ),

    rpc_ring = cms.PSet(
        Nbinsx = cms.int32(8),
        xmin = cms.double(-3),
        xmax = cms.double(5),
        ),

    rpc_station = cms.PSet(
        Nbinsx = cms.int32(8),
        xmin = cms.double(-1),
        xmax = cms.double(6),
        ),
    rpc_sector = cms.PSet(
        Nbinsx = cms.int32(15),
        xmin = cms.double(-1),
        xmax = cms.double(14),
        ),
)
