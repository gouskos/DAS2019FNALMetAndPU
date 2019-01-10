import FWCore.ParameterSet.Config as cms

metandpuanalstep3 = cms.EDAnalyzer('MetAndPuAnalStep3',
                                   mets       = cms.InputTag("slimmedMETs"),
                                   metFilters = cms.InputTag("TriggerResults"),
                                   )
