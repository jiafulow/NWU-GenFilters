import FWCore.ParameterSet.Config as cms

pythia8MultiParticleFilter = cms.EDFilter("Pythia8MultiParticleFilter",
    src = cms.InputTag("generator"),
    AcceptMore = cms.bool(False),
    ParticleID = cms.vint32(24, 25),
    PtMin = cms.vdouble(0., 0.),
    EtaMax = cms.vdouble(5., 5.),
    Status = cms.vint32(22, 22),
    NumRequired = cms.vint32(1, 1),
)
