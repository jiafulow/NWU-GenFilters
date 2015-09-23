import FWCore.ParameterSet.Config as cms

# Z2 star tune with pT-ordered showers
#from Configuration.Generator.PythiaUEZ2starSettings_cfi import *
#from GeneratorInterface.ExternalDecays.TauolaSettings_cff import *

generator = cms.EDFilter("Pythia8175GeneratorFilter",
    maxEventsToPrint = cms.untracked.int32(5),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    comEnergy = cms.double(8000.),
    PythiaParameters = cms.PSet(
        py8HDecaySettings = cms.vstring(  
            'Tune:pp = 5',                     # UE tune
            'PDF:pSet = 7',                    # PDF set
            'Top:qq2tq = on',                  # q q' -> t q" via t-channel
            'HiggsSM:all = off',               # turn OFF all H production mode
            '25:isResonance = on',             # Resonance
            '25:m0 = 30',                      # Higgs mass
            #'25:onMode = off',                 # Decay modes for Higgs
            #'25:onIfMatch = 13 -13',           # Turn on h -> mu+ mu-
            '25:oneChannel = 1 1.00 0 13 -13', # Turn on h -> mu+ mu-
            '6:addChannel = 1 0.93 100 25 4',  # New top decay mode t -> h c
            '6:onMode = off',                  # Decay modes for top
            '6:onIfAny = 25',                  # Decay modes for top
        ),
        parameterSets = cms.vstring('py8HDecaySettings')
    )
)


