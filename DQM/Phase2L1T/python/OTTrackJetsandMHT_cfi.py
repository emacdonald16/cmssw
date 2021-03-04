import FWCore.ParameterSet.Config as cms
import math

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
OTTrackJetsandMHT = DQMEDAnalyzer('OTTrackJetsandMHT',
     TopFolderName  = cms.string('Phase2L1T'),
     L1TkJetInputTag = cms.InputTag("L1TrackJets","L1TrackJets"),
     #L1VertexInputTag = cms.InputTag("L1TkPrimaryVertex"),
     jet_maxEta = cms.double(2.4),          # maximum eta of jets for HT
     jet_minPt = cms.double(5.0),          # minimum pt of jets for HT [GeV]
     jet_minNtracksLowPt=cms.int32(2),        #Add track jet quality criteria pT>50
     jet_minNtracksHighPt=cms.int32(3),       #Add track jet quality criteria pT>100


# Number of tracks in jet
    TH1_NTracks = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(99.5),
        xmin = cms.double(-0.5)
        ),

#Pt of the jet
    TH1_Jet_Pt = cms.PSet(
        Nbinsx = cms.int32(50),
        xmax = cms.double(300),
        xmin = cms.double(0)
        ),

#Eta of the jet
    TH1_Jet_Eta = cms.PSet(
        Nbinsx = cms.int32(45),
        xmax = cms.double(3.0),
        xmin = cms.double(-3.0)
        ),

#Phi of the jet
    TH1_Jet_Phi = cms.PSet(
        Nbinsx = cms.int32(45),
        xmax = cms.double(3.0),
        xmin = cms.double(-3.0)
        ),

#Vtx of the track
    TH1_Jet_Vtx = cms.PSet(
        Nbinsx = cms.int32(51),
        xmax = cms.double(25),
        xmin = cms.double(-25)
        ),

#HT distribution
    TH1_HT = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(500),
        xmin = cms.double(0)
        ),

#MHT distribution
    TH1_MHT = cms.PSet(
        Nbinsx = cms.int32(100),
        xmax = cms.double(500),
        xmin = cms.double(0)
        ),
)
