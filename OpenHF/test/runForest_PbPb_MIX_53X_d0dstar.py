#!/usr/bin/env python2
# Run the foresting configuration on PbPb in CMSSW_5_3_X, using the new HF/Voronoi jets
# Author: Alex Barbieri
# Date: 2013-10-15

hiTrackQuality = "highPurity"              # iterative tracks
#hiTrackQuality = "highPuritySetWithPV"    # calo-matched tracks

import FWCore.ParameterSet.Config as cms
process = cms.Process('HiForest')
process.options = cms.untracked.PSet(
    # wantSummary = cms.untracked.bool(True)
    #SkipEvent = cms.untracked.vstring('ProductNotFound')
)

#####################################################################################
# HiForest labelling info
#####################################################################################

process.load("HeavyIonsAnalysis.JetAnalysis.HiForest_cff")
process.HiForest.inputLines = cms.vstring("HiForest V3",)
import subprocess
version = subprocess.Popen(["(cd $CMSSW_BASE/src && git describe --tags)"], stdout=subprocess.PIPE, shell=True).stdout.read()
if version == '':
    version = 'no git info'
process.HiForest.HiForestVersion = cms.untracked.string(version)

#####################################################################################
# Input source
#####################################################################################

process.source = cms.Source("PoolSource",
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            fileNames = cms.untracked.vstring(
#     "file:/home/sun229/sl6_PbPb/cjetsample/files/9683CA1F-10D9-E311-941C-848F69FD4553.root"
	  "file:/home/sun229/store/Hydjet1p8_TuneDrum_Quenched_MinBias_2760GeV/D0embedded_Hydjet1p8_2760GeV_GEN_SIM_rerunhltRECO_D0pt3_pthat0_1217/2c32042f86c50ef41784f9a935177ecd/D0_RECO_rerunhlt_166_1_sBQ.root"
#     "file:/home/sun229/sl6_PbPb/embedding/CMSSW_5_3_20/src/D_embeded_RECO.root"
    ))

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10))


#####################################################################################
# Load Global Tag, Geometry, etc.
#####################################################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

# PbPb 53X MC
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'STARTHI53_LV1::All', '')

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideGT_PbPb2760(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowers"),
    nonDefaultGlauberModel = cms.string("Hydjet_Drum"),
    centralitySrc = cms.InputTag("hiCentrality")
    )

#####################################################################################
# Define tree output
#####################################################################################

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("HiForest_Dmeson_PbPbMC.root"))

#####################################################################################
# Additional Reconstruction and Analysis: Main Body
#####################################################################################

process.load('HeavyIonsAnalysis.JetAnalysis.jets.HiGenJetsCleaned_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3PFJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3PFJetSequence_PbPb_mix_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4PFJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4PFJetSequence_PbPb_mix_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5CaloJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5PFJetSequence_PbPb_mix_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5PFJetSequence_PbPb_mix_cff')

process.jetSequences = cms.Sequence(#process.akPu3CaloJetSequence +
                                    #process.akVs3CaloJetSequence +
                                    process.akVs3PFJetSequence +
                                    process.akPu3PFJetSequence +

                                    #process.akPu4CaloJetSequence +
                                    #process.akVs4CaloJetSequence +
                                    process.akVs4PFJetSequence +
                                    process.akPu4PFJetSequence +

                                    #process.akPu5CaloJetSequence +
                                    #process.akVs5CaloJetSequence +
                                    process.akVs5PFJetSequence +
                                    process.akPu5PFJetSequence

                                    )

process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_mc_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cff')
#process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cff_d0_cleanup')
process.load('HeavyIonsAnalysis.JetAnalysis.HiGenAnalyzer_cfi')

#####################################################################################
# To be cleaned

process.load('HeavyIonsAnalysis.JetAnalysis.ExtraTrackReco_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.TrkAnalyzers_MC_cff')
process.load("HeavyIonsAnalysis.TrackAnalysis.METAnalyzer_cff")
process.load("HeavyIonsAnalysis.JetAnalysis.pfcandAnalyzer_cfi")
process.load('HeavyIonsAnalysis.JetAnalysis.rechitanalyzer_cfi')
process.rechitAna = cms.Sequence(process.rechitanalyzer+process.pfTowers)
process.pfcandAnalyzer.skipCharged = False
process.pfcandAnalyzer.pfPtMin = 0

#####################################################################################

#########################
# Track Analyzer
#########################
process.anaTrack.qualityStrings = cms.untracked.vstring('highPurity','highPuritySetWithPV')
process.pixelTrack.qualityStrings = cms.untracked.vstring('highPurity','highPuritySetWithPV')
process.hiTracks.cut = cms.string('quality("highPurity")')

# set track collection to iterative tracking
process.anaTrack.trackSrc = cms.InputTag("hiGeneralTracks")
process.anaTrack.tpFakeSrc = cms.untracked.InputTag("cWeakDecaySelector")
process.anaTrack.tpEffSrc = cms.untracked.InputTag("cWeakDecaySelector")
process.anaTrack.doSimVertex = True
process.anaTrack.doSimTrack = True
process.anaTrack.fillSimTrack = True
process.anaTrack.associateChi2 = False
process.anaTrack.doDebug = cms.untracked.bool(True)

# clusters missing in recodebug - to be resolved
process.anaTrack.doPFMatching = False
process.pixelTrack.doPFMatching = False

#####################
# photons
process.load('HeavyIonsAnalysis.JetAnalysis.EGammaAnalyzers_cff')
process.multiPhotonAnalyzer.GenEventScale = cms.InputTag("hiSignal")
process.multiPhotonAnalyzer.HepMCProducer = cms.InputTag("hiSignal")
process.RandomNumberGeneratorService.multiPhotonAnalyzer = process.RandomNumberGeneratorService.generator.clone()

#####################
# muons
######################
process.load("HeavyIonsAnalysis.MuonAnalysis.hltMuTree_cfi")
process.hltMuTree.doGen = cms.untracked.bool(True)
process.load("RecoHI.HiMuonAlgos.HiRecoMuon_cff")
process.muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("akVs3PFJets")
process.globalMuons.TrackerCollectionLabel = "hiGeneralTracks"
process.muons.TrackExtractorPSet.inputTrackCollection = "hiGeneralTracks"
process.muons.inputCollectionLabels = ["hiGeneralTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx", "tevMuons:firstHit", "tevMuons:picky", "tevMuons:dyt"]

##################################
# cWeakDecay tracking particle
#################################
process.load("SimTracker.TrackHistory.TrackClassifier_cff")

from SimTracker.TrackHistory.CategorySelectors_cff import *
process.cWeakDecaySelector = TrackingParticleCategorySelector(
     src = cms.InputTag("mergedtruth","MergedTrackTruth"),
     cut = cms.string("is('CWeakDecay')")
)

process.cWeakDecaytp=cms.Path(process.cWeakDecaySelector)

#####################################
#reproduce hi gen particle
#####################################


process.load('Configuration.StandardSequences.Generator_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')

process.hiGenParticles=process.hiGenParticles.clone()
process.hiGenParticles.srcVector = cms.vstring('hiSignal')

process.genStep = cms.Path(process.hiGenParticles)

#D meson for hi reco
process.HFtree = cms.EDAnalyzer(
        "HFTree",
        verbose      = cms.untracked.int32(1),
        printFrequency = cms.untracked.int32(1000),
        requireCand  =  cms.untracked.bool(False),
        fReducedTree  =  cms.untracked.bool(True),
        tracksLabel = cms.untracked.InputTag("hiGeneralTracks"),
        tpFakeLabel = cms.untracked.InputTag("cWeakDecaySelector"),
		cut_alpha0_in = cms.untracked.double(9.0),
        isMC = cms.untracked.bool(True)
        )

process.load("UserCode.OpenHF.HFRecoStuff_PbPb_cff")
process.load("UserCode.OpenHF.HFCharm_PbPb_cff")


process.d0Dump.trackPt = cms.untracked.double(1.5)
process.d0Dump.isMC = cms.untracked.bool(True)

process.dstarDump.trackPt = cms.untracked.double(2.0)
process.dstarDump.slowPionPt= cms.untracked.double(0.5)
process.dpmDump.trackPt = cms.untracked.double(2.0)
process.dpmDump.slowPionPt= cms.untracked.double(0.5)

process.dsPhiPiDump.trackPt = cms.untracked.double(2.0)
process.dsKstarKaDump.trackPt = cms.untracked.double(2.0)

process.OpenHfTree_step = cms.Path(
        process.recoStuffSequence*
#        process.charmSequence*
#        process.dpmSequence*
#        process.d0dstarSequence*
        process.d0Sequence*
        process.HFtree
        )


process.hltobject.triggerNames = cms.vstring("HLT_HIJet80_v7", "HLT_HIJet65_v7", "HLT_HIJet55_v7")


# HYDJET RECO file didn't have ak2GenJets and ak6GenJets as input, so removed them
# and ran our own hiGenJetsCleaned sequence
from HeavyIonsAnalysis.JetAnalysis.jets.HiGenJetsCleaned_cff import *

process.hiSelectGenJets = cms.Sequence(
    ak3HiGenJetsCleaned +
    ak4HiGenJetsCleaned +
    ak5HiGenJetsCleaned +
    ak7HiGenJetsCleaned
)

process.HiGenParticleAna.stableOnly = cms.untracked.bool(False)
process.HiGenParticleAna.etaMax = cms.untracked.double(3.0)
process.HiGenParticleAna.genpSrc = cms.untracked.InputTag('hiGenParticles')

process.ana_step = cms.Path(process.heavyIon*
                            process.hltanalysis *
                            process.hltobject *
                            process.hiEvtAnalyzer*
                            process.HiGenParticleAna*
                            #process.hiGenJetsCleaned*
                            process.hiSelectGenJets +
                            process.jetSequences +
#                            process.photonStep_withReco +
                            process.pfcandAnalyzer +
                            process.rechitAna +
#temp                            process.hltMuTree +
                            process.HiForest +
                            process.cutsTPForFak +
                            process.cutsTPForEff +
                            process.anaTrack #+
#                            process.pixelTrack
                            )

process.load('HeavyIonsAnalysis.JetAnalysis.EventSelection_cff')
process.phltJetHI = cms.Path( process.hltJetHI )
process.pcollisionEventSelection = cms.Path(process.collisionEventSelection)
process.pHBHENoiseFilter = cms.Path( process.HBHENoiseFilter )
process.phfCoincFilter = cms.Path(process.hfCoincFilter )
process.phfCoincFilter3 = cms.Path(process.hfCoincFilter3 )
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter )
process.phltPixelClusterShapeFilter = cms.Path(process.siPixelRecHits*process.hltPixelClusterShapeFilter )
process.phiEcalRecHitSpikeFilter = cms.Path(process.hiEcalRecHitSpikeFilter )

process.pAna = cms.EndPath(process.skimanalysis)

process.schedule = cms.Schedule( process.genStep, process.cWeakDecaytp, process.ana_step, process.OpenHfTree_step, process.phltJetHI, process.pcollisionEventSelection, process.pHBHENoiseFilter, process.phfCoincFilter, process.phfCoincFilter3, process.pprimaryVertexFilter, process.phltPixelClusterShapeFilter, process.phiEcalRecHitSpikeFilter)
