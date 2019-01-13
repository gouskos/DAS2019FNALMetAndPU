import FWCore.ParameterSet.Config as cms

metandpuanalstep2a = cms.EDAnalyzer('MetAndPuAnalStep2A',
                                    doprints = cms.bool(True),      
                                    mets     = cms.InputTag("slimmedMETs")
                                    )
