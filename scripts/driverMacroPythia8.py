#!/usr/bin/env python

import os
import sys

fdir = "Configuration/GenProduction/python/EightTeV/"
fragment = "TTToHCWB_HToMuMu_M-%i_8TeV_pythia8175"

globaltag = "START53_V27::All"


# ______________________________________________________________________________
def doStep0(mass):
    fragment_m = fragment % mass
    
    cmd = r"cmsDriver.py %s \
    --fileout 'file:%s' \
    --python_filename '%s' \
    --step GEN,SIM \
    --conditions %s\
    --pileup NoPileUp \
    --datamix NODATAMIXER \
    --mc --eventcontent RAWSIM --datatier GEN-SIM \
    --no_exec -n 20"
    
    cmd = cmd % (fdir+fragment_m+".py", fragment_m+"_step0.root", fragment_m+"_step0_cfg.py", globaltag)
    print cmd
    os.system(cmd)
    
    print "\n>>> You also need to add these six lines by hand.<<<"
    print "process.load('NWU.GenFilters.pythia8MultiParticleFilter_cfi')"
    print "process.genfilter = cms.Sequence(process.pythia8MultiParticleFilter)"
    print ""
    print "# filter all path with the production filter sequence"
    print "for path in process.paths:"
    print "    getattr(process,path)._seq = process.generator * process.genfilter * getattr(process,path)._seq"
    
    print "\n>>> You also need to comment out the conflicting lines about production filter sequence.<<<"

def doStep1(mass):
    fragment_m = fragment % mass
    
    cmd = r"cmsDriver.py step1 \
    --filein 'file:%s' \
    --fileout 'file:%s' \
    --python_filename '%s' \
    --step DIGI,L1,DIGI2RAW,HLT:7E33v2 \
    --conditions %s \
    --pileup 2012_Summer_50ns_PoissonOOTPU --pileup_input 'dbs:/MinBias_TuneZ2star_8TeV-pythia6/Summer12-START50_V13-v3/GEN-SIM' \
    --mc --eventcontent RAWSIM --datatier GEN-SIM-RAW \
    --no_exec -n 100"
    
    cmd = cmd % (fragment_m+"_step0.root", fragment_m+"_step1.root", fragment_m+"_step1_cfg.py", globaltag)
    print cmd
    os.system(cmd)
    
    if not os.path.exists("mixFileNames.py"):
        raise Exception('Cannot find mixFileNames.py in the current directory.')
    
    print "\n>>> You also need to add these two lines by hand.<<<"
    print "from mixFileNames import prim"
    print "process.mix.input.fileNames = cms.untracked.vstring(prim)"

def doStep2(mass):
    fragment_m = fragment % mass
    
    cmd = r"cmsDriver.py step2 \
    --filein 'file:%s' \
    --fileout 'file:%s' \
    --python_filename '%s' \
    --step RAW2DIGI,L1Reco,RECO,VALIDATION:validation_prod \
    --conditions %s \
    --mc --eventcontent AODSIM --datatier AODSIM \
    --no_exec -n 100"
    
    cmd = cmd % (fragment_m+"_step1.root", fragment_m+"_step2.root", fragment_m+"_step2_cfg.py", globaltag)
    print cmd
    os.system(cmd)


# ______________________________________________________________________________
if __name__=="__main__":
    
    if len(sys.argv) != 3:
        raise Exception('Need two arguments: step mass')
        
    step = int(sys.argv[1])
    mass = int(sys.argv[2])
    
    print "--- Using fragment: %s" % (fdir+fragment+".py")
    print "--- Using globaltag: %s" % (globaltag)
    print "--- Doing step %i mass %i" % (step, mass)
    
    if step == 0:
        doStep0(mass)
    
    elif step == 1:
        doStep1(mass)
    
    elif step == 2:
        doStep2(mass)
