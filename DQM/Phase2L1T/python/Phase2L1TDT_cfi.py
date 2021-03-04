import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
Phase2L1TDTVal = DQMEDAnalyzer('Phase2L1TDT',
  TopFolderName = cms.string('Phase2L1T'),
  DTDigis    = cms.InputTag("DTTPG","MMTCHT"), #L1Phase2MuDTPhContainer  "DTTPG" "MMTCHT" "HLT"

  dt_tp_wheel = cms.PSet(
    Nbinsx = cms.int32(7),
    xmin = cms.double(-3),
    xmax = cms.double(4)
  ),

  dt_tp_station = cms.PSet(
    Nbinsx = cms.int32(6),
    xmin = cms.double(0),
    xmax = cms.double(6)
  ),

  dt_tp_sector = cms.PSet(
    Nbinsx = cms.int32(15),
    xmin = cms.double(0),
    xmax = cms.double(15),
  ),

  # dt_tp_bx = cms.PSet(
  #   Nbinsx = cms.int32(7),
  #   xmax = cms.double(4),
  #   xmin = cms.double(-3)
  # ),

   dt_tp_bx = cms.PSet(
     Nbinsx = cms.int32(20),
     xmax = cms.double(30),
     xmin = cms.double(10)
   ),

  dt_tp_t0 = cms.PSet(
    Nbinsx = cms.int32(15),
    xmax = cms.double(850),
    xmin = cms.double(400)
  ),

  dt_tp_quality = cms.PSet(
    Nbinsx = cms.int32(10),
    xmax = cms.double(12.0),
    xmin = cms.double(2.0)
  ),

  dt_tp_n = cms.PSet(
    Nbinsx = cms.int32(25),
    xmax = cms.double(25),
    xmin = cms.double(0)
  ),
)
