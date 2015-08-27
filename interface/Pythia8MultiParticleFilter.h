#ifndef Pythia8MultiParticleFilter_h
#define Pythia8MultiParticleFilter_h
// -*- C++ -*-
//
// Package:    Pythia8MultiParticleFilter
// Class:      Pythia8MultiParticleFilter
// 
/* 

 Description: Filter to select events with an arbitrary number of given particle(s).

 Implementation: derived from MCMultiParticleFilter
     
*/
//
// Original Author:  Paul Lujan
//         Created:  Wed Feb 29 04:22:16 CST 2012
// $Id: Pythia8MultiParticleFilter.h,v 1.1 2012/03/18 10:50:20 lenzip Exp $
//
//

// system include files
#include <memory>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//
// class declaration
//

class Pythia8MultiParticleFilter : public edm::EDFilter {
 public:
  explicit Pythia8MultiParticleFilter(const edm::ParameterSet&);
  ~Pythia8MultiParticleFilter();
  
 private:
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  bool is_equal_vint32(const std::vector<int>& v1, const std::vector<int>& v2) const {
    return std::equal(v1.begin(), v1.end(), v2.begin());
  }
  
  // ----------member data ---------------------------
  
  edm::InputTag src_;              // input tag
  bool acceptMore_;                // if true (default), accept numRequired or more.
                                   // if false, accept events with exactly equal to numRequired.
  std::vector<int> particleID_;    // vector of particle IDs to look for
  // the three next variables can either be a vector of length 1 (in which case the same
  // value is used for all particle IDs) or of length equal to the length of ParticleID (in which
  // case the corresponding value is used for each).
  std::vector<double> ptMin_;      // minimum Pt of particles
  std::vector<double> etaMax_;     // maximum fabs(eta) of particles
  std::vector<int> status_;        // status of particles
  std::vector<int> numRequired_;   // number of particles required to pass filter
  int totalEvents_;                // counters
  int passedEvents_;
};
#endif
