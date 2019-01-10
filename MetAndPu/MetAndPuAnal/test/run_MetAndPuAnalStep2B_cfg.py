import FWCore.ParameterSet.Config as cms

process = cms.Process("das2019metandpu")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring([
            'file:/eos/uscms/store/user/cmsdas/2019/short_exercises/METAndPU/data/handson1_dy1jet_m50_fall17_part.root',
            ]
                                      )
    )


## e.g. calculate MET using PF candidates with pT>10 GeV only
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

## create the new PFCandidate collection
#process.PFCands = cms.EDFilter("CandPtrSelector",
#                                      src=cms.InputTag("packedPFCandidates"),
#                                      cut=cms.string("pt>10.0")
#                                      )

## use the "MET tool" to re-cluster MET and then recorrect and get the proper uncertainties
runMetCorAndUncFromMiniAOD(process,
                           isData=False,
                           )

#runMetCorAndUncFromMiniAOD(process,
#                           isData=False,
#                           pfCandColl=cms.InputTag("PFCands"),
#                           recoMetFromPFCs=True,
#                           CHS = True, 
#                           reclusterJets=True,
#                           postfix="DAS2019"
#                           )



process.load("MetAndPu.MetAndPuAnal.MetAndPuAnalStep2B_cfi")
process.p = cms.Path(
    process.fullPatMetSequence * ## add this line including the POSTFIX name given above 
    process.metandpuanalstep2b
    )
