import FWCore.ParameterSet.Config as cms

process = cms.Process('HiForest')
#process.options = cms.untracked.PSet(
#    # wantSummary = cms.untracked.bool(True)
##    SkipEvent = cms.untracked.vstring('ProductNotFound')
#)
#
#process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
#oncePerEventMode=cms.untracked.bool(True))
#
#process.Timing = cms.Service("Timing")
#

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
#                            "/store/hidata/HIRun2011/HIHighPt/RECO/14Mar2014-v2/00012/0C0FD828-E5BE-E311-90AA-FA163E6AC682.root"
#							"/store/user/mnguyen/HIHighPt/jetRegit_HIHighPt_Jet80_v1/ea21a976f39e8fe4f82ae523253849bf/regionalTracking_10_1_zvD.root"
							"file:/home/sun229/sl6_PbPb/cjetsample/files/04D4F50A-11AE-E311-8B9E-FA163EE7D8E5_rereco_MB.root"
#							"file:/home/sun229/sl6_PbPb/cjetsample/files/1CC46C43-99B9-E311-B9CF-FA163E4A10E1_highpt_rereco_run181913.root"
							)
                            )

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1))


#####################################################################################
# Load Global Tag, Geometry, etc.
#####################################################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')

# PbPb 53X MC
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_53_LV6::All', '')

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideGT_PbPb2760(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowers"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

#####################################################################################
# Define tree output
#####################################################################################

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("HiForestDmeson_PbPbdata.root"))

#####################################################################################
# Additional Reconstruction and Analysis: Main Body
#####################################################################################

###################
## Jets ##
###################

#process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')
#process.load('RecoHI.HiJetAlgos.HiRecoPFJets_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs3PFJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu3PFJetSequence_PbPb_data_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs4PFJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu4PFJetSequence_PbPb_data_cff')

process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5CaloJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akVs5PFJetSequence_PbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.jets.akPu5PFJetSequence_PbPb_data_cff')

process.jetSequences = cms.Sequence(#process.akPu3CaloJetSequence +
                                    #process.akVs3CaloJetSequence +
                                    #process.akVs3PFJetSequence +
                                    process.akPu3PFJetSequence #+

                                    #process.akPu4CaloJetSequence +
                                    #process.akVs4CaloJetSequence +
                                    #process.akVs4PFJetSequence +
                                    #process.akPu4PFJetSequence #+

                                    #process.akPu5CaloJetSequence +
                                    #process.akVs5CaloJetSequence +
                                    #process.akVs5PFJetSequence +
                                    #process.akPu5PFJetSequence

                                    )


#####################################################################################
# Rechits/PFcands
process.load("HeavyIonsAnalysis.JetAnalysis.pfcandAnalyzer_cfi")
process.load('HeavyIonsAnalysis.JetAnalysis.rechitanalyzer_cfi')
process.rechitAna = cms.Sequence(process.rechitanalyzer+process.pfTowers)
process.pfcandAnalyzer.skipCharged = False
process.pfcandAnalyzer.pfPtMin = 0

#####################################################################################

#########################
# Track Analyzer
#########################
process.load('HeavyIonsAnalysis.JetAnalysis.ExtraTrackReco_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.TrkAnalyzers_cff')

process.anaTrack.qualityStrings = cms.untracked.vstring('highPurity','highPuritySetWithPV')
process.pixelTrack.qualityStrings = cms.untracked.vstring('highPurity','highPuritySetWithPV')
process.hiTracks.cut = cms.string('quality("highPurity")')



# set track collection to iterative tracking
process.anaTrack.trackSrc = cms.InputTag("hiGeneralTracks")
process.anaTrack.vertexSrc = cms.vstring("hiSelectedVertex")

# clusters missing in recodebug - to be resolved
process.anaTrack.doPFMatching = False
process.pixelTrack.doPFMatching = False


# clusters missing in recodebug - to be resolved
process.anaTrack.doPFMatching = False

#####################
# photons
process.load('HeavyIonsAnalysis.JetAnalysis.EGammaAnalyzers_cff')
process.photonStep_withReco.remove(process.photonMatch)
process.RandomNumberGeneratorService.multiPhotonAnalyzer = process.RandomNumberGeneratorService.generator.clone()

#####################

######################
# muons
######################
process.load("HeavyIonsAnalysis.MuonAnalysis.hltMuTree_cfi")
process.hltMuTree.doGen = cms.untracked.bool(True)
process.load("RecoHI.HiMuonAlgos.HiRecoMuon_cff")
process.muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("akVs3PFJets")
process.globalMuons.TrackerCollectionLabel = "hiGeneralTracks"
process.muons.TrackExtractorPSet.inputTrackCollection = "hiGeneralTracks"
process.muons.inputCollectionLabels = ["hiGeneralTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx", "tevMuons:firstHit","tevMuons:picky", "tevMuons:dyt"]

process.hltMuTree.doGen = False

######################

process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.EventSelection_cff')
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")


process.pcollisionEventSelection = cms.Path(process.collisionEventSelection)
process.pHBHENoiseFilter = cms.Path( process.HBHENoiseFilter )
process.phfCoincFilter = cms.Path(process.hfCoincFilter )
process.phfCoincFilter3 = cms.Path(process.hfCoincFilter3 )
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter )
process.phltPixelClusterShapeFilter = cms.Path(process.siPixelRecHits*process.hltPixelClusterShapeFilter )
process.phiEcalRecHitSpikeFilter = cms.Path(process.hiEcalRecHitSpikeFilter )


process.ana_step = cms.Path( process.hltanalysis *
                            process.hltobject *
                            process.hiEvtAnalyzer *
#                            process.jetSequences +
#                            process.photonStep_withReco *
#                            process.pfcandAnalyzer +
#                            process.rechitAna +
#temp                            process.hltMuTree +
                            process.HiForest +
                            process.anaTrack 
                            )


#D meson for hi reco
process.HFtree = cms.EDAnalyzer(
        "HFTree",
        verbose      = cms.untracked.int32(1),
        printFrequency = cms.untracked.int32(1000),
        requireCand  =  cms.untracked.bool(False),
        fReducedTree  =  cms.untracked.bool(True),
		tracksLabel = cms.untracked.InputTag("hiGeneralTracks"),
        isMC = cms.untracked.bool(False)
        )

process.load("UserCode.OpenHF.HFRecoStuff_PbPb_cff")
process.load("UserCode.OpenHF.HFCharm_PbPb_cff")


process.d0Dump.trackPt = cms.untracked.double(1.0)

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


process.pAna = cms.EndPath(process.skimanalysis)

#Filtering
#############################################################
# To filter on an HLT trigger path, uncomment the lines below, add the
# HLT path you would like to filter on to 'HLTPaths' and also
# uncomment the snippet at the end of the configuration.
#############################################################
# Minimum bias trigger selection (later runs)
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.skimFilter = process.hltHighLevel.clone()
#process.skimFilter.HLTPaths = ["HLT_HIJet80_v*"]
#process.skimFilter.HLTPaths = ["HLT_HIJet55_v*",
#                               "HLT_HIJet65_v*",
#                               "HLT_HIJet80_v*",
#                               "HLT_HIJet95_v*"
#							   "HLT_HIDiJet55_v*",
#                               "HLT_HIJet65_Jet55_v*"
#							   ]
#process.skimFilter.HLTPaths = ["HLT_HIMinBiasHfOrBSC_*",
#                               "HLT_HIUCC*_*"
#						       ]
process.skimFilter.HLTPaths = ["HLT_HIMinBiasHfOrBSC_v*"]

process.superFilterSequence = cms.Sequence(process.skimFilter)
process.superFilterPath = cms.Path(process.superFilterSequence)
process.skimanalysis.superFilters = cms.vstring("superFilterPath")
################################################################
#Filtering
for path in process.paths:
    getattr(process,path)._seq = process.superFilterSequence*getattr(process,path)._seq
