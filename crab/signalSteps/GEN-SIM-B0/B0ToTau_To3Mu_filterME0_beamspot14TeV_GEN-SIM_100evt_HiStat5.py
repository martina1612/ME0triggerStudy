from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'B0ToTau_To3Mu_beamspot14TeV_filterME0-GEN-SIM_100evt__HiStat5'
config.General.workArea = 'crab_GEN'

config.section_("JobType")
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = '/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/cmsswConfig_data_step1/B0Tau3Mu_beamspot14TeV_filterME0_GEN-SIM_forHiStat5.py'
#config.JobType.maxJobRuntimeMin = 3000

config.section_("Data")
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 100
config.Data.totalUnits = 1000000 #66000000
config.Data.publication = True
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter'
config.Data.outputDatasetTag = 'B0ToTau_To3Mu_beamspot14TeV_filterME0-GEN-SIM_100evt__HiStat5'
config.Data.outputPrimaryDataset = 'ME0trigger'

config.section_("Site")
config.Site.storageSite = 'T2_IT_Bari'#'T3_US_FNALLPC' 
