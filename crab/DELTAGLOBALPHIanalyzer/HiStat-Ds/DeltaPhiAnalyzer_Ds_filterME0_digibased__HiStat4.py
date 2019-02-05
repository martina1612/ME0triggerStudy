#from CRABClient.UserUtilities import config
#config = config()
from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'DeltaGlobalPhiAnalyzer_Ds_filterME0_digibased__HiStat4'
config.General.workArea = 'crab_DeltaGlobalPhiAnalyzer__HiStat'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/ME0analyzer/DeltaGlobalPhiAnalyzer/python/ConfFile_cfg.py'
#config.JobType.priority = 1
#config.JobType.allowUndistributedCMSSW = True #AGGIUNTO DA ME

config.section_("Data")
config.Data.inputDataset = '/ME0trigger/mressego-DsToTau_To3Mu_beamspot14TeV_filterME0_digibased-step3_100evt__HiStat4_FEVTDEBUGHLToutput-0ca0c7f22478c521b70ed98356042824/USER'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
#job_control
config.Data.publication = False #whether to publish EDM output files to DBS
config.Data.outputDatasetTag = 'DeltaGlobalPhiAnalyzer_Ds_filterME0_digibased__HiStat4'
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

