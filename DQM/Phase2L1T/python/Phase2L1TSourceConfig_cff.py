import FWCore.ParameterSet.Config as cms

from DQM.Phase2L1T.Phase2L1THCal_cfi import *
from DQM.Phase2L1T.Phase2L1THGCal_cfi import *
from DQM.Phase2L1T.Phase2L1TCSC_cfi import *
from DQM.Phase2L1T.Phase2L1TDT_cfi import *
from DQM.Phase2L1T.Phase2L1TECal_cfi import *
from DQM.Phase2L1T.Phase2L1TGEM_cfi import *
from DQM.Phase2L1T.Phase2L1TRPC_cfi import *

Phase2L1TDQMSource = cms.Sequence(
                                    # Phase2L1THCalVal *
                                    # Phase2L1THGCalVal *
                                    # Phase2L1TCSCVal *
                                    # Phase2L1TDTVal
                                    # Phase2L1TGEMVal *
                                    # Phase2L1TRPCVal
                                    Phase2L1TECalVal
                                 )
