#from CRABClient.UserUtilities import config
#config = config()
from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'DeltaGlobalPhiAnalyzer_B0_filterME0_padbased__HiStat7'
config.General.workArea = 'crab_DeltaGlobalPhiAnalyzer__HiStat'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/ME0analyzer/DeltaGlobalPhiAnalyzer/python/ConfFile_cfg.py'
#config.JobType.priority = 1
#config.JobType.allowUndistributedCMSSW = True #AGGIUNTO DA ME

config.section_("Data")
config.Data.inputDataset = '/ME0trigger/mressego-B0ToTau_To3Mu_beamspot14TeV_filterME0_padbased-step3_100evt__HiStat7_FEVTDEBUGHLToutput-61cff2b17b7157f560edc7ff8cf1c306/USER'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
#job_control
config.Data.publication = False #whether to publish EDM output files to DBS
config.Data.outputDatasetTag = 'DeltaGlobalPhiAnalyzer_B0_filterME0_padbased__HiStat7'
config.Data.outLFNDirBase = '/store/user/mressego'
#config.Data.ignoreLocality = True 
#PER MC:
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.splitting = 'FileBased'
config.Data.totalUnits = -1
config.Data.unitsPerJob  = 100000

config.section_("Site")
#config.Site.whitelist = ["T2_IT_Bari"]
#config.Site.whitelist = ["T2_IT_*"]
config.Site.storageSite = 'T2_IT_Bari' #'T3_US_FNALLPC'

