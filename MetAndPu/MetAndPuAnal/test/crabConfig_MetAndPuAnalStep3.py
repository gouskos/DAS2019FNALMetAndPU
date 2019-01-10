from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'my_CRAB_project_directory'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_MetAndPuAnalStep3_cfg.py'
config.JobType.outputFiles = ['metandpu_step3.root']

config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
#config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 20
config.Data.outLFNDirBase = '/store/user/%s/DASTEST/' % (getUsernameFromSiteDB())
#config.Data.outLFNDirBase = '/store/user/loukas/DASTEST/'
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_LGDAS2019_TRY'

#config.Site.whitelist = ['T3_US_FNALLPC']
config.Site.storageSite = 'T3_US_FNALLPC'
