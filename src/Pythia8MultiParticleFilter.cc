#include "NWU/GenFilters/interface/Pythia8MultiParticleFilter.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

Pythia8MultiParticleFilter::Pythia8MultiParticleFilter(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src")),
  acceptMore_(iConfig.getParameter<bool>("AcceptMore")),
  particleID_(iConfig.getParameter< std::vector<int> >("ParticleID")),
  ptMin_(iConfig.getParameter< std::vector<double> >("PtMin")),
  etaMax_(iConfig.getParameter< std::vector<double> >("EtaMax")),
  status_(iConfig.getParameter< std::vector<int> >("Status")),
  numRequired_(iConfig.getParameter< std::vector<int> >("NumRequired")),
  totalEvents_(0), passedEvents_(0)
{
  //here do whatever other initialization is needed

  // default pt, eta, status cuts to "don't care"
  std::vector<double> defptmin(1, 0);
  std::vector<double> defetamax(1, 999.0);
  std::vector<int> defstat(1, 0);
  std::vector<int> defnum(1, 1);

  // check for same size
  if ( (ptMin_.size() > 1 &&  particleID_.size() != ptMin_.size()) 
       ||  (etaMax_.size() > 1 && particleID_.size() != etaMax_.size()) 
       ||  (status_.size() > 1 && particleID_.size() != status_.size()) 
       ||  (numRequired_.size() > 1 && particleID_.size() != numRequired_.size()) ) {
    edm::LogWarning("Pythia8MultiParticleFilter") << "WARNING: Pythia8MultiParticleFilter: size of PtMin, EtaMax, Status, and/or NumRequired does not match ParticleID size!" << std::endl;   
  }
  
  // Fill arrays with defaults if necessary
  while (ptMin_.size() < particleID_.size())
    ptMin_.push_back(defptmin[0]);
  while (etaMax_.size() < particleID_.size())
    etaMax_.push_back(defetamax[0]);
  while (status_.size() < particleID_.size())
    status_.push_back(defstat[0]);
  while (numRequired_.size() < particleID_.size())
    numRequired_.push_back(defnum[0]);
}

Pythia8MultiParticleFilter::~Pythia8MultiParticleFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


// ------------ method called to skim the data  ------------
bool Pythia8MultiParticleFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<edm::HepMCProduct> evt;
  iEvent.getByLabel(src_, evt);
  
  totalEvents_++;
  std::vector<int> nFound(particleID_.size(), 0);
  
  const HepMC::GenEvent * myGenEvent = evt->GetEvent();
  
  for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();
	p != myGenEvent->particles_end(); ++p ) {
    
    for (unsigned int i = 0; i < particleID_.size(); ++i) {
      if ((particleID_[i] == 0 || abs(particleID_[i]) == abs((*p)->pdg_id())) &&
	  (*p)->momentum().perp() > ptMin_[i] &&
	  fabs((*p)->momentum().eta()) < etaMax_[i] &&
	  (status_[i] == 0 || (*p)->status() == status_[i])) {
	//edm::LogInfo("Pythia8MultiParticleFilter") << "Particle: " << (*p)->pdg_id() << " status: " << (*p)->status();
	nFound[i] ++;
	break; // only match a given particle once!
      }
    } // loop over targets
    
    if (acceptMore_ && is_equal_vint32(nFound, numRequired_)) break; // stop looking if we don't mind having more
  } // loop over particles
  
  if (is_equal_vint32(nFound, numRequired_)) {
    passedEvents_++;
    return true;
  } else {
    return false;
  }
  
}

// ------------ method called once each job just after ending the event loop  ------------
void Pythia8MultiParticleFilter::endJob() {
  edm::LogInfo("Pythia8MultiParticleFilter") << "=== Results of Pythia8MultiParticleFilter: passed "
                                        << passedEvents_ << "/" << totalEvents_ << " events" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(Pythia8MultiParticleFilter);

