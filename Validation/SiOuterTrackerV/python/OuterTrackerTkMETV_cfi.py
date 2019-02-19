import FWCore.ParameterSet.Config as cms
import math

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
OuterTrackerTkMETV = DQMEDAnalyzer('OuterTrackerTkMETV',
    TopFolderName = cms.string('SiOuterTrackerV'),
    trackingParticleToken = cms.InputTag("mix","MergedTrackTruth"), #tracking particles
    StubInputTag = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"), #stubs
    TTTracksTag       = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"), #tracks (currently from tracklet)
    MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"), #truth stub associator
    MCTruthTrackInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigis", "Level1TTTracks"), #truth track associator
    MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"), #truth cluster associator
    L1VertexInputTag = cms.InputTag("VertexProducer", "l1vertextdr"),

    L1Tk_nPar = cms.int32(4),           # use 4 or 5-parameter L1 track fit ??
    L1Tk_minPt = cms.double(2.0),       # L1 tracks, pT >= X GeV
    L1Tk_maxPt = cms.double(200.0),       # L1 tracks, pT saturated upper limit
    L1Tk_maxEta = cms.double(2.4),       # L1 tracks, |eta| < X
    L1Tk_minNStub = cms.int32(4),       # L1 tracks with >= 4 stubs
    L1Tk_minNPSstub = cms.int32(2),       # L1 tracks with >= 2 PS stubs
    L1Tk_maxVtxZ = cms.double(30.0),   # L1 tracks with |z0| <= X
    L1Tk_maxChi2dof = cms.double(50.0),# L1 tracks with Chi2/dof <= X
    L1Tk_maxBendChi2 = cms.double(1.75),# L1 tracks with Chi2/dof <= X

    TP_minPt = cms.double(2.0),      # only save TPs with pt > X GeV
    TP_maxEta = cms.double(2.4),     # only save TPs with |eta| < X
    TP_maxVtxZ = cms.double(30.0),     # only save TPs with |z0| < X cm
    TP_maxDxy = cms.double(1.0),     # only save TPs with |dxy| < X cm (also known as VertR)
    TP_minNStub = cms.int32(4),      # require TP to have >= X number of stubs associated with it
    TP_minNLayers = cms.int32(4),      # require TP to have >= X number of layers with stubs

# tracking particles vs eta
    TH1TrackParts_Eta = cms.PSet(
        Nbinsx = cms.int32(45),
        xmax = cms.double(3),
        xmin = cms.double(-3)
        ),

# tracking particles vs pT
    TH1TrackParts_Pt = cms.PSet(
        Nbinsx = cms.int32(45),
        xmax = cms.double(100),
        xmin = cms.double(0)
        ),


#NStubs of the track
    TH1_Track_NStubs = cms.PSet(
        Nbinsx = cms.int32(10),
        xmax = cms.double(10),
        xmin = cms.double(0)
        ),

#dxy (also VertR) of the track
    TH1_Track_Dxy = cms.PSet(
        Nbinsx = cms.int32(20),
        xmax = cms.double(1.5),
        xmin = cms.double(0)
        ),

#VertZ (also z0) of the track
    TH1_Track_VertZ = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(30),
        xmin = cms.double(0)
        ),

# trueTkMET - recoTkMET (for TkMET resolution plots)
    TH1Res_TkMET = cms.PSet(
        Nbinsx = cms.int32(200),
        xmax = cms.double(500),
        xmin = cms.double(-500)
        ),
)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(OuterTrackerTkMETV, trackingParticleToken = "mixData:MergedTrackTruth")
