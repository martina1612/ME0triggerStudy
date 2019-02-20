from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'B0ToTau_To3Mu_beamspot14TeV_filterME0_digibased-step3_100evt__HiStat5'
config.General.workArea = 'crab_step3'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/cmsswConfig_data_step3/step3_RAW2DIGI_L1Reco_RECO_PAT_VALIDATION_DQM_digibased.py'
#config.JobType.maxJobRuntimeMin = 3000

config.section_("Data")
config.Data.inputDataset = '/ME0trigger/mressego-B0ToTau_To3Mu_beamspot14TeV_filterME0_digibased-step2_100evt__HiStat5-bf5e7ef4a1eb44f2330386d3fc78951f/USER'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = True
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter'
config.Data.outputDatasetTag = 'B0ToTau_To3Mu_beamspot14TeV_filterME0_digibased-step3_100evt__HiStat5'

config.section_("Site")
config.Site.storageSite = 'T2_IT_Bari' #'T3_US_FNALLPC'
