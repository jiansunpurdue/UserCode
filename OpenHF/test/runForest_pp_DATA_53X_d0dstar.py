#!/usr/bin/env python2
# Run the foresting configuration on PbPb in CMSSW_5_3_X, using the new HF/Voronoi jets
# Author: Alex Barbieri
# Date: 2013-10-15

import FWCore.ParameterSet.Config as cms
process = cms.Process('HiForest')
process.options = cms.untracked.PSet(
    # wantSummary = cms.untracked.bool(True)
    #SkipEvent = cms.untracked.vstring('ProductNotFound')
)

#process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
#oncePerEventMode=cms.untracked.bool(True))

#process.Timing = cms.Service("Timing")

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
#"root://xrootd.unl.edu//store/data/Run2013A/PPJet/RECO/PromptReco-v1/000/211/693/00000/E217D7AB-9775-E211-BCE7-003048F1110E.root"
#"root://xrootd.unl.edu//store/data/Run2013A/PPMinBias/RECO/PromptReco-v1/000/211/693/00000/04787D89-9675-E211-92EF-003048D37456.root"
 "root://xrootd.unl.edu//store/data/Run2013A/PPJet/RECO/PromptReco-v1/000/211/797/00000/001BCA99-B677-E211-B53F-002481E0D7EC.root"
#                                "file:/home/sun229/sl6_PbPb/cjetsample/files/8484C153-3276-E211-9476-0025B32036E2_2013PPMinBiasRECO.root"
    ))

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10))

#import FWCore.PythonUtilities.LumiList as LumiList
#process.source.lumisToProcess = LumiList.LumiList(filename = 'Cert_211739-211831_2760GeV_PromptReco_Collisions13_JSON.txt').getVLuminosityBlockRange()

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
process.load('RecoHI.HiCentralityAlgos.CentralityBin_cfi')

process.load('FWCore.MessageService.MessageLogger_cfi')

# PbPb 53X MC
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V43D::All', '')

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideGT_pp2760(process)
overrideJEC_pp2760(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowersTrunc"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

#####################################################################################
# Define tree output
#####################################################################################

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("HiForest.root"))

#####################################################################################
# Additional Reconstruction and Analysis: Main Body
#####################################################################################

#process.load('Configuration.StandardSequences.Generator_cff')
#process.load('RecoJets.Configuration.GenJetParticles_cff')
#process.load('RecoHI.HiJetAlgos.HiGenJets_cff')
#process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')
#process.load('RecoHI.HiJetAlgos.HiRecoPFJets_cff')

#process.hiGenParticles.srcVector = cms.vstring('generator')

process.hiCentrality.producePixelhits = False
process.hiCentrality.producePixelTracks = False
process.hiCentrality.srcTracks = cms.InputTag("generalTracks")
process.hiCentrality.srcVertex = cms.InputTag("offlinePrimaryVerticesWithBS")
process.hiEvtPlane.vtxCollection_ = cms.InputTag("offlinePrimaryVerticesWithBS")
process.hiEvtPlane.trackCollection_ = cms.InputTag("generalTracks")

process.load('HeavyIonsAnalysis.JetAnalysis.jets.HiGenJetsCleaned_JEC_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu2CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs2CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs2PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu2PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak2PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak2CaloJetSequence_pp_data_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak3PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak3CaloJetSequence_pp_data_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak4PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak4CaloJetSequence_pp_data_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5CaloJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak5PFJetSequence_pp_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.ak5CaloJetSequence_pp_data_cff')


process.load('HeavyIonsAnalysis.JetAnalysis.jets.HiReRecoJets_pp_cff')
process.voronoiBackgroundPF.src = cms.InputTag("particleFlow")
process.PFTowers.src = cms.InputTag("particleFlow")

process.jetSequences = cms.Sequence(process.voronoiBackgroundCalo +
                                    process.voronoiBackgroundPF +
                                    process.PFTowers +
                                    process.hiReRecoCaloJets +
                                    process.hiReRecoPFJets +

#                                    process.akPu2CaloJetSequence +
#                                    process.akVs2CaloJetSequence +
                                    process.akVs2PFJetSequence +
                                    process.akPu2PFJetSequence +
#                                    process.ak2PFJetSequence +
#                                    process.ak2CaloJetSequence +

#                                    process.akPu3CaloJetSequence +
#                                    process.akVs3CaloJetSequence +
                                    process.akVs3PFJetSequence +
                                    process.akPu3PFJetSequence +
#                                    process.ak3PFJetSequence +
#                                    process.ak3CaloJetSequence +

#                                    process.akPu4CaloJetSequence +
#                                    process.akVs4CaloJetSequence +
                                    process.akVs4PFJetSequence +
                                    process.akPu4PFJetSequence #+
#                                    process.ak4PFJetSequence +
#                                    process.ak4CaloJetSequence +

#                                    process.akPu5CaloJetSequence +
#                                    process.akVs5CaloJetSequence +
#                                    process.akVs5PFJetSequence +
#                                    process.akPu5PFJetSequence #+
#                                    process.ak5PFJetSequence +
#                                    process.ak5CaloJetSequence

                                    )

process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
#process.load('HeavyIonsAnalysis.JetAnalysis.HiGenAnalyzer_cfi')
process.hiEvtAnalyzer.Vertex = cms.InputTag("offlinePrimaryVerticesWithBS")

#####################################################################################
# To be cleaned

process.load('HeavyIonsAnalysis.JetAnalysis.ExtraTrackReco_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.TrkAnalyzers_cff')
process.load("HeavyIonsAnalysis.TrackAnalysis.METAnalyzer_cff")
process.load("HeavyIonsAnalysis.JetAnalysis.pfcandAnalyzer_pp_cfi")
process.load('HeavyIonsAnalysis.JetAnalysis.rechitanalyzer_pp_cfi')
process.rechitAna = cms.Sequence(process.rechitanalyzer+process.pfTowers)
process.pfcandAnalyzer.skipCharged = False
process.pfcandAnalyzer.pfPtMin = 0

#####################################################################################

#########################
# Track Analyzer
#########################
process.hiTracks.cut = cms.string('quality("highPurity")')

# clusters missing in recodebug - to be resolved
process.anaTrack.doPFMatching = False
process.ppTrack.doPFMatching = False
process.ppTrack.doSimTrack = False

#####################
# photons
process.load('HeavyIonsAnalysis.JetAnalysis.EGammaAnalyzers_cff')
process.photonStep.remove(process.photonMatch)
process.hiGoodTracks.src = cms.InputTag("generalTracks")
process.hiGoodTracks.vertices = cms.InputTag("offlinePrimaryVerticesWithBS")
process.RandomNumberGeneratorService.multiPhotonAnalyzer = process.RandomNumberGeneratorService.generator.clone()

#####################
# muons
######################
process.load("HeavyIonsAnalysis.MuonAnalysis.hltMuTree_cfi")
process.hltMuTree.doGen = cms.untracked.bool(True)
process.load("RecoHI.HiMuonAlgos.HiRecoMuon_cff")
process.muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("akVs3PFJets")
process.globalMuons.TrackerCollectionLabel = "generalTracks"
process.muons.TrackExtractorPSet.inputTrackCollection = "generalTracks"
process.muons.inputCollectionLabels = ["generalTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx", "tevMuons:firstHit", "tevMuons:picky", "tevMuons:dyt"]


process.ana_step = cms.Path(process.hiCentrality +
                            process.centralityBin +
                            process.hiEvtPlane +
                            process.hiEvtAnalyzer*
                            process.jetSequences +
                            #process.photonStep +
                            process.pfcandAnalyzer +
#                            process.rechitAna +
#temp                            process.hltMuTree +
                            process.HiForest +
                            process.ppTrack)



#D meson for pp reco
process.HFtree = cms.EDAnalyzer(
        "HFTree",
        verbose      = cms.untracked.int32(1),
        printFrequency = cms.untracked.int32(1000),
        requireCand  =  cms.untracked.bool(False),
        fReducedTree  =  cms.untracked.bool(True),
        tracksLabel = cms.untracked.InputTag("generalTracks"),
        isMC = cms.untracked.bool(False)
        )

process.load("UserCode.OpenHF.HFRecoStuff_pppPb_cff")
process.load("UserCode.OpenHF.HFCharm_pppPb_cff")


process.d0Dump.trackPt = cms.untracked.double(0.5)
process.dstarDump.trackPt = cms.untracked.double(0.5)
process.dstarDump.slowPionPt= cms.untracked.double(0.5)
process.dpmDump.trackPt = cms.untracked.double(2.0)
process.dpmDump.slowPionPt= cms.untracked.double(0.5)

process.dsPhiPiDump.trackPt = cms.untracked.double(2.0)
process.dsKstarKaDump.trackPt = cms.untracked.double(2.0)

process.OpenHfTree_step = cms.Path(
        process.recoStuffSequence*
#        process.charmSequence*
#        process.dpmSequence*
        process.d0dstarSequence*
#        process.d0Sequence*
        process.HFtree
        )


process.load('HeavyIonsAnalysis.JetAnalysis.EventSelection_cff')
process.phltJetHI = cms.Path( process.hltJetHI )
process.pPAcollisionEventSelectionPA = cms.Path(process.PAcollisionEventSelection)
process.pHBHENoiseFilter = cms.Path( process.HBHENoiseFilter )
process.phfCoincFilter = cms.Path(process.hfCoincFilter )
process.phfCoincFilter3 = cms.Path(process.hfCoincFilter3 )
#process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter )
process.phltPixelClusterShapeFilter = cms.Path(process.siPixelRecHits*process.hltPixelClusterShapeFilter )
process.phiEcalRecHitSpikeFilter = cms.Path(process.hiEcalRecHitSpikeFilter )

# Customization
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cff')
process.hltobject.triggerNames = cms.vstring("HLT_PAJet20_NoJetID_v1", "HLT_PAJet40_NoJetID_v1", "HLT_PAJet60_NoJetID_v1", "HLT_PAJet80_NoJetID_v1", "HLT_PAJet100_NoJetID_v1", "HLT_PAJet120_NoJetID_v1")

process.hltAna = cms.Path(process.hltanalysis + process.hltobject)
process.pAna = cms.EndPath(process.skimanalysis)

##Filtering
##############################################################
## To filter on an HLT trigger path, uncomment the lines below, add the
## HLT path you would like to filter on to 'HLTPaths' and also
## uncomment the snippet at the end of the configuration.
##############################################################
## Minimum bias trigger selection (later runs)
#process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
#process.skimFilter = process.hltHighLevel.clone()
##process.skimFilter.HLTPaths = ["HLT_PAZeroBiasPixel_SingleTrack_v*"]
#process.skimFilter.HLTPaths = ["HLT_PAJet20_NoJetID_v*",
#                               "HLT_PAJet40_NoJetID_v*",
#							   "HLT_PAJet60_NoJetID_v*",
#							   "HLT_PAJet80_NoJetID_v*",
#							   "HLT_PAJet100_NoJetID_v*",
#							   "HLT_PAJet120_NoJetID_v*"]
#
#process.superFilterSequence = cms.Sequence(process.skimFilter)
#process.superFilterPath = cms.Path(process.superFilterSequence)
#process.skimanalysis.superFilters = cms.vstring("superFilterPath")
#################################################################
##Filtering
#for path in process.paths:
#    getattr(process,path)._seq = process.superFilterSequence*getattr(process,path)._seq
