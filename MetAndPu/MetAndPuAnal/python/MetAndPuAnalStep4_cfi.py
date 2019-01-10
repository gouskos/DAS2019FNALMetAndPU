import FWCore.ParameterSet.Config as cms

metandpuanalstep4 = cms.EDAnalyzer('MetAndPuAnalStep4',
                                   doprints = cms.bool(False),
                                   metFilters = cms.InputTag("TriggerResults","","PAT"),
                                   beamHaloFilterSel = cms.string("Flag_globalSuperTightHalo2016Filter"),
                                   hbheFilterSel = cms.string("Flag_HBHENoiseFilter"),
                                   hbheIsoFilterSel = cms.string("Flag_HBHENoiseIsoFilter"),
                                   ecalTPFilterSel = cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter"),
                                   badPFMuonFilterSel = cms.string("Flag_BadPFMuonFilter"),
                                   badChargedCandFilterSel = cms.string("Flag_BadChargedCandidateFilter"),
                                   eeBadScFilterSel = cms.string("Flag_eeBadScFilter"),
                                   )
