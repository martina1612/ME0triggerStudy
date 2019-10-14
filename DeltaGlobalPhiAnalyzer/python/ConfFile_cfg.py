import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.Geometry.GeometryExtended2023D20Reco_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/RecoLocalMuon/GEMRecHit/test/TEST_recoSegment_Digi.root'
        #'file:/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/localSamples/promptMu/recosegment_Digi_3.root'
	#'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_50GeV-RECOSEGMENTpad/180322_095158/0000/recoSegment_PadDigi_110.root'
	#'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_50GeV-RECOSEGMENTpad/180322_095158/0000/recoSegment_PadDigi_50.root'
	#'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_5GeV-RECOSEGMENTpad/180322_095009/0000/recoSegment_PadDigi_10.root'
	#'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_3GeV-RECOSEGMENTdigi/180313_091618/0000/recoSegment_Digi_1.root'
	#'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_50GeV-RECOSEGMENTdigi/180307_094343/0000/recoSegment_Digi_110.root'
	'root://xrootd-cms.infn.it:1194//store/user/mressego/ME0trigger/promptMu_3GeV-RECOSEGMENTdigi/180313_091618/0000/recoSegment_Digi_1.root'
	#'file:/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/minBias/cmsswConfig_step2/recoSegment_PadDigi.root'
    )
)

process.demo = cms.EDAnalyzer('ME0SegmentAnalyzer',
    verbose = cms.bool(True),
    me0DigiToken = cms.InputTag("simMuonME0Digis"),
    me0PadDigiToken = cms.InputTag("simMuonME0PadDigis"),
    me0PadDigiClusterToken = cms.InputTag("simMuonME0PadDigiClusters"),
    genParticles = cms.InputTag("genParticles"),
    me0RecHits = cms.InputTag("me0RecHits","","GEMLocalRECOSEGMENT"),
    me0SimHits = cms.InputTag("g4SimHits","MuonME0Hits","SIM"),
    me0Segments = cms.InputTag("me0Segments","","GEMLocalRECOSEGMENT")
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('out_Digi_ME0SegmentAnalyzer_3.root'))
    #fileName = cms.string('outPadDigi_ME0SegmentAnalyzer.root'))

process.p = cms.Path(process.demo)
