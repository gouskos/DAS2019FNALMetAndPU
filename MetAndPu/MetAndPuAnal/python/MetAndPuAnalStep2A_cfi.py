import FWCore.ParameterSet.Config as cms

metandpuanalstep2a = cms.EDAnalyzer('MetAndPuAnalStep2A',
                                    doprints = cms.bool(False),      
                                    mets     = cms.InputTag("slimmedMETs")
                                    )