import FWCore.ParameterSet.Config as cms

L1TkPrimaryVertex = cms.EDProducer('L1TkFastVertexProducer',
    HepMCInputTag = cms.InputTag("generator"),
    GenParticleInputTag = cms.InputTag("genParticles",""),
    L1TrackInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    zMax = cms.double ( 25. ) ,           # in cm
    chi2dofMax = cms.double( 100. ),      # maximum chi2/dof cut allowed
    ptMin = cms.double( 2.),              # PTMIN of L1Tracks, in GeV
    ptMax = cms.double( 50. ),            # in GeV. When PTMAX > 0, tracks with PT above PTMAX are considered as
					                      # mismeasured and are treated according to HighPtTracks below.
					                      # When PTMAX < 0, no special treatment is done for high PT tracks.
					                      # If PTMAX < 0, no saturation or truncation is done
    highPtTracks = cms.int32( 0 ),	      # when = 0 : truncation. Tracks with PT above PTMAX are ignored
					                      # when = 1 : saturation. Tracks with PT above PTMAX are set to PT=PTMAX.
    nStubsMin = cms.int32( 4 ) ,          # minimum number of stubs
    nPSStubsMin = cms.int32( 3 ),         # minimum number of stubs in PS modules
    nBinning = cms.int32( 601 ),          # number of bins for the temp histo (from -30 cm to + 30 cm)
    monteCarloVertex = cms.bool( False ), #  when True: dont run the vxt finding algo but pick up the MC generated vtx
    doPtComp = cms.bool( True ),          # track-stubs PT compatibility cut
    doTightChi2 = cms.bool( False ),      # chi2dof < 5 for tracks with PT > 10
    weight = cms.int32(1)                 # WEIGHT can be set to 0, 1 or 2 for unweighted, pT weighted
                                          # or pT2 weighted tracks respectively.

# Other working point which works better for H -> TauTau,
# cf talk by Moshan Ather, Dec 12, 2014:

#    weight = cms.int32(2),
#    ptMax = cms.double( 25. ),
#    nStubsMin = cms.int32( 5 ),
#    highPtTracks = cms.int32( 1),
#    doPtComp = cms.bool( False ),
#    chi2dofMax = cms.double( 20 )
#
)
