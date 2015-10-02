#include <iostream>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "UserCode/GenDmeson/container/daughterparticle.h"
#include "UserCode/GenDmeson/container/motherquark.h"
#include "UserCode/GenDmeson/container/middleparticle.h"
#include "UserCode/GenDmeson/container/dmeson.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

using namespace edm;
using namespace std;
using namespace reco;



class Dcharged_ccbar : public edm::EDAnalyzer
{

   public:
   
      //
      explicit Dcharged_ccbar( const edm::ParameterSet& ) ;
      virtual ~Dcharged_ccbar() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob() ;
      virtual void endRun( const edm::Run&, const edm::EventSetup& ) ;
      virtual void endJob() ;
      void daughterfill(daughterparticle * daughter,const Candidate * candidate);
      void motherfill(motherquark * mother, const Candidate * candidate);
      void middlefill(middleparticle * middle, const Candidate * candidate);
      void dmesonfill(dmeson * d, const Candidate * candidate);    

   private:
     
     TH1D*       dchargedpt ;
     TTree *     dchargedtree;
     double      pt_dmeson;
     double      eta_paifromdcharged;
//     daughterparticle *paionfromdcharged;
     dmeson *    dmeson_dcharged;
     daughterparticle * particle1fromdmeson;
     daughterparticle * particle2fromdmeson;
     daughterparticle * particle3fromdmeson;
     motherquark *      dmesonmotherquark;
	 edm::InputTag genParticleSrc_;
//     middleparticle *   particledecayed;
     
}; 


Dcharged_ccbar::Dcharged_ccbar( const ParameterSet& iConfig )
{
	genParticleSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genpSrc",edm::InputTag("hiGenParticles"));
    particle1fromdmeson = new daughterparticle();
    particle2fromdmeson = new daughterparticle();
    particle3fromdmeson = new daughterparticle();
    dmesonmotherquark =  new motherquark();
    dmeson_dcharged = new dmeson();
// actually, pset is NOT in use - we keep it here just for illustratory putposes

//  dchargedtree = new TTree("dchargedtree", "dchargedtree");
//  pai = new daughterparticle();
//  dchargedtree->Branch("pai","daughterparticle",&pai,32000,1);

}

void Dcharged_ccbar::beginJob()
{
  Service<TFileService> fs; 
  dchargedtree = fs->make<TTree>("dchargedtree","dchargedtree");
//  dchargedtree->Branch("pai","daughterparticle",&paionfromdcharged,32000,0);
  dchargedtree->Branch("dcharged","dmeson",&dmeson_dcharged,32000,1);  
  
  return ;
  
}

void Dcharged_ccbar::daughterfill(daughterparticle *daughter, const Candidate * candidate)
{  
       daughter->pt = candidate->pt();
       daughter->charge = candidate->charge(); 
       daughter->motherid = (candidate->mother())->pdgId(); 
       daughter->pid = candidate->pdgId(); 
       daughter->phi = candidate->phi(); 
       daughter->eta = candidate->eta(); 
       daughter->mass = candidate->mass();
  return;
}

void Dcharged_ccbar::motherfill(motherquark *mother, const Candidate * candidate)
{
       mother->pt = candidate->pt();
       mother->charge = candidate->charge();
       mother->pid = candidate->pdgId();
       mother->phi = candidate->phi();
       mother->eta = candidate->eta();
       mother->mass = candidate->mass();
  return;
}

void Dcharged_ccbar::middlefill(middleparticle *middle, const Candidate * candidate)
{
       middle->pt = candidate->pt();
       middle->charge = candidate->charge();
       middle->motherid = (candidate->mother())->pdgId();
       middle->pid = candidate->pdgId();
       middle->phi = candidate->phi();
       middle->eta = candidate->eta();
       middle->mass = candidate->mass();
       middle->numberofdaughter = candidate->numberOfDaughters();
  return;
}

void Dcharged_ccbar::dmesonfill(dmeson *d, const Candidate * candidate)
{
       d->pt = candidate->pt();
       d->charge = candidate->charge();
	   if(candidate->mother())
         d->motherid = (candidate->mother())->pdgId();
	   else
         d->motherid = -999;
       d->pid = candidate->pdgId();
       d->phi = candidate->phi();
       d->eta = candidate->eta();
       d->mass = candidate->mass();
       d->numberofdaughter = candidate->numberOfDaughters();
  return;
}


void Dcharged_ccbar::analyze( const Event& e, const EventSetup& )
{
  
  
//  cout << " Dcharged_ccbar " << endl;
  int numberofdcharged = 0;
 
  
  Handle<GenParticleCollection> genParticles;
//  e.getByLabel("genParticles", genParticles);
  e.getByLabel(genParticleSrc_, genParticles);
//  cout << "particle number = " << genParticles->size() << endl;

  for(size_t i = 0; i < genParticles->size(); ++ i) {

     const GenParticle & p = (*genParticles)[i];
     int id = p.pdgId();
     int st = p.status();  
     if (abs(id) != 411 || abs(st) != 2)   continue;
     numberofdcharged++;

//	 const Candidate * mom = p.mother();
//     while( mom->mother() )  
//	 {
//      mom=mom->mother();
//      cout << "pid of mother's mother = " << mom->pdgId() << endl;
//	  if(abs(mom->pdgId()) == 4 )   break;
//	 }
//     const Candidate * ancester = p.();
//     double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
//     pt_dmeson = pt;
//     double vx = p.vx(), vy = p.vy(), vz = p.vz();
//     int charge = p.charge();
     int n = p.numberOfDaughters();
     if ( n != 3) continue;
     const Candidate * paion1 = NULL;
     const Candidate * paion2 = NULL;
     const Candidate * kaion = NULL;
    
//     cout << " 3 daughters" << endl;

     for(int j = 0; j < n; ++ j) {
       const Candidate * dau = p.daughter( j );
       int dauId = abs(dau->pdgId());
       if(dauId == 321)     kaion = p.daughter(j);
       if(dauId == 211  && paion1 == NULL)     paion1 = p.daughter(j);
       if(dauId == 211 && abs(paion1->pdgId()) == 211 && paion1 != p.daughter(j) )   paion2 = p.daughter(j);
     }
   
     if (!(paion1!=NULL && paion2!=NULL && kaion != NULL))    continue;
     if (!(abs(paion1->pdgId())==211 && abs(paion2->pdgId())==211 && abs(kaion->pdgId())==321))    continue;

     const Candidate * ddcharged = &p;
//     cout << "Dcharged to pai kai pai" << endl;
     
     daughterfill(particle1fromdmeson,paion1);
//     motherfill(dmesonmotherquark,mom);
     daughterfill(particle2fromdmeson,paion2);
     daughterfill(particle3fromdmeson,kaion);
     dmesonfill (dmeson_dcharged,ddcharged);
//     (dmeson_dcharged->momquark).push_back(*dmesonmotherquark);
     (dmeson_dcharged->daughters).push_back(*particle1fromdmeson);
     (dmeson_dcharged->daughters).push_back(*particle2fromdmeson);
     (dmeson_dcharged->daughters).push_back(*particle3fromdmeson); 

     dchargedtree->Fill();
     dmeson_dcharged->clear();
     particle1fromdmeson->clear();
     particle2fromdmeson->clear();
     particle3fromdmeson->clear();
     dmesonmotherquark->clear();
   }
 

//   cout << "number of Dcharged =====" << numberofdcharged << endl;
  
   
   return ;
   
}

void Dcharged_ccbar::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void Dcharged_ccbar::endJob()
{
   
   return ;
}
 
typedef Dcharged_ccbar Dcharged_ccbar_Analyzer;
DEFINE_FWK_MODULE(Dcharged_ccbar_Analyzer);
