import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDHarvester import DQMEDHarvester
OuterTrackerHarvesterV = DQMEDHarvester("OuterTrackerMCHarvester",)
OuterTrackertkMETHarvesterV = DQMEDHarvester("OuterTrackerTkMETHarvester",)

OuterTracker_harvestingV = cms.Sequence(OuterTrackerHarvesterV*OuterTrackertkMETHarvesterV)
