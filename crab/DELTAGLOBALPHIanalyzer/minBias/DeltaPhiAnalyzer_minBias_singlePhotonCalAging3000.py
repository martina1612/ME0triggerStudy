#from CRABClient.UserUtilities import config
#config = config()
from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'DeltaGlobalPhiAnalyzer_B0_minBias_singlePhotonCalAging3000'
config.General.workArea = 'crab_DeltaGlobalPhiAnalyzer__minBias'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/ME0analyzer/DeltaGlobalPhiAnalyzerMinbias/python/ConfFile_cfg.py'
#config.JobType.priority = 1
#config.JobType.allowUndistributedCMSSW = True #AGGIUNTO DA ME

config.section_("Data")
config.Data.inputDataset = '/SinglePhoton_Pt-500/PhaseIITDRSpring17DR-PU200CalAging3000_91X_upgrade2023_realistic_v3-v2/GEN-SIM-RECO'
config.Data.inputDBS = 'global'
#job_control
config.Data.publication = False #whether to publish EDM output files to DBS
config.Data.outputDatasetTag = 'DeltaGlobalPhiAnalyzer_B0_minBias_singlePhotonCalAging3000'
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

