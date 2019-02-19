import FWCore.ParameterSet.Config as cms

from Validation.SiOuterTrackerV.OuterTrackerMonitorTrackingParticles_cfi import *
from Validation.SiOuterTrackerV.OuterTrackerTkMETV_cfi import *
OuterTrackerSourceV = cms.Sequence( OuterTrackerMonitorTrackingParticles
                                    * OuterTrackerTkMETV
)
