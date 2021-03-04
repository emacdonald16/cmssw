import FWCore.ParameterSet.Config as cms
import math

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
OuterTrackerTkMET = DQMEDAnalyzer('OuterTrackerTkMET',
    TopFolderName  = cms.string('Phase2L1T'),
    TTTracksTag    = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    # L1VertexInputTag = cms.InputTag("VertexProducer", "l1vertextdr"),
    L1VertexInputTag = cms.InputTag("L1TkPrimaryVertex"),
    maxZ0 = cms.double ( 15. ) ,    # in cm
    maxEta = cms.double ( 2.4 ) ,
    chi2dofMax = cms.double( 10. ),
    bendchi2Max = cms.double( 2.2 ),
    minPt = cms.double( 2. ),       # in GeV
    DeltaZ = cms.double( 3. ),      # in cm
    nStubsmin = cms.int32( 4 ),     # min number of stubs for the tracks to enter in TrkMET calculation
    nStubsPSmin = cms.int32( -1 ),   # min number of stubs in the PS Modules
    maxPt = cms.double( -10. ),	    # in GeV. When maxPt > 0, tracks with PT above maxPt are considered as
                                    # mismeasured and are treated according to HighPtTracks below.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.
    HighPtTracks = cms.int32( 1 ),  # when = 0 : truncation. Tracks with PT above maxPt are ignored
                                    # when = 1 : saturation. Tracks with PT above maxPt are set to PT=maxPt.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.

# Number of TTTracks
    TH1_NTracks = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(99.5),
        xmin = cms.double(-0.5)
        ),

#Pt of the track
    TH1_Track_Pt = cms.PSet(
        Nbinsx = cms.int32(50),
        xmax = cms.double(100),
        xmin = cms.double(0)
        ),

#Eta of the track
    TH1_Track_Eta = cms.PSet(
        Nbinsx = cms.int32(45),
        xmax = cms.double(3.0),
        xmin = cms.double(-3.0)
        ),

#VtxZ of the track
    TH1_Track_VtxZ = cms.PSet(
        Nbinsx = cms.int32(51),
        xmax = cms.double(25),
        xmin = cms.double(-25)
        ),

#Nstubs of the track
    TH1_Track_NStubs = cms.PSet(
        Nbinsx = cms.int32(6),
        xmax = cms.double(9),
        xmin = cms.double(3)
        ),

#N PS stubs of the track
    TH1_Track_NPSstubs = cms.PSet(
        Nbinsx = cms.int32(6),
        xmax = cms.double(7),
        xmin = cms.double(1)
        ),

#Chi2dof of the track
    TH1_Track_Chi2Dof = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(50),
        xmin = cms.double(0)
        ),

#Bendchi2 of the track
    TH1_Track_BendChi2 = cms.PSet(
        Nbinsx = cms.int32(30),
        xmax = cms.double(50),
        xmin = cms.double(0)
        ),

#tkMET distribution
    TH1_Track_TkMET = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(500),
        xmin = cms.double(0)
        ),

)
