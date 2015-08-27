# Taken from Configuration/PyReleaseValidation/python/ConfigBuilder.py

import os
import FWCore.ParameterSet.Config as cms
fdir = os.environ["CMSSW_BASE"] + "/src/NWU/GenFilters/scripts/"
fname = fdir + "mixFileNames.txt"
prim=[]
sec=[]
with open(fname, "r") as f:
    for line in f.readlines():
        if line.count(".root")>=2:
                #two files solution...
                entries=line.replace("\n","").split()
                if not entries[0] in prim:
                        prim.append(entries[0])
                if not entries[1] in sec:
                        sec.append(entries[1])
        elif (line.find(".root")!=-1):
                entry=line.replace("\n","")
                if not entry in prim:
                        prim.append(entry)

#print len(prim), prim
#print len(sec), sec
