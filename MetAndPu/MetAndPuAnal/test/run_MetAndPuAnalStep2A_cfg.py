import FWCore.ParameterSet.Config as cms

process = cms.Process("das2019metandpu")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1


process.TFileService = cms.Service("TFileService", fileName = cms.string("./outputs/metandpu_step2.root") )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring([
            #'file:/eos/uscms/store/user/cmsdas/2019/short_exercises/METAndPU/data/handson1_dy1jet_m50_fall17_part.root',
            #'file:/eos/uscms/store/user/cmsdas/2019/short_exercises/METAndPU/data/handson1_dylljets_amcatnlo.root',
            '/store/user/cmsdas/2019/short_exercises/METAndPU/data/handson1_dylljets_amcatnlo.root',
            ]
                                      )
    )

process.load("MetAndPu.MetAndPuAnal.MetAndPuAnalStep2A_cfi")
process.p = cms.Path(process.metandpuanalstep2a)
