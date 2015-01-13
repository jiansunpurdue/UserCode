import FWCore.ParameterSet.Config as cms

# ----------------------------------------------------------------------
regitdstarDump = cms.EDAnalyzer(
    "HFDstar",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralAndRegitTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    D0Window           = cms.untracked.double(0.2),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(1), 
    isPbPb             = cms.untracked.bool(True)
    )
#

# ----------------------------------------------------------------------
regitd0Dump = cms.EDAnalyzer(
    "HFD0",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralAndRegitTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    D0Window           = cms.untracked.double(0.2),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(2),
    isPbPb             = cms.untracked.bool(True)
    )

# ----------------------------------------------------------------------
regitdsPhiPiDump = cms.EDAnalyzer(
    "HFDs2PhiPi",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralAndRegitTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    PhiWindow          = cms.untracked.double(0.1),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(3),
    isPbPb             = cms.untracked.bool(True)
    )
#
# ----------------------------------------------------------------------
regitdsKstarKaDump = cms.EDAnalyzer(
    "HFDs2KstarKaon",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralAndRegitTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowKaonPt         = cms.untracked.double(0.5),
    KstarWindow           = cms.untracked.double(0.5),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(4),
    isPbPb             = cms.untracked.bool(True)
    )
#
# ----------------------------------------------------------------------
regitdpmDump = cms.EDAnalyzer(
    "HFDpm2KaPiPi",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralAndRegitTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(5),
    isPbPb             = cms.untracked.bool(True)
    )
#

# ######################################################################
# Sequences
# ######################################################################
regitd0Sequence     = cms.Sequence(regitd0Dump);
regitdstarSequence     = cms.Sequence(regitdstarDump);
regitd0dstarSequence     = cms.Sequence(regitd0Dump*regitdstarDump);
regitdpmSequence     = cms.Sequence(regitdpmDump);
regitdsSequence     = cms.Sequence(regitdsPhiPiDump*regitdsKstarKaDump);
regitdpmdsSequence = cms.Sequence(regitdpmDump*regitdsPhiPiDump*regitdsKstarKaDump);
regitcharmSequence     = cms.Sequence(regitd0Dump*regitdstarDump*regitdpmDump*regitdsPhiPiDump*regitdsKstarKaDump);
