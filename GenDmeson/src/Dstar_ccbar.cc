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



class Dstar_ccbar : public edm::EDAnalyzer
{

   public:
   
      //
      explicit Dstar_ccbar( const edm::ParameterSet& ) ;
      virtual ~Dstar_ccbar() {} // no need to delete ROOT stuff
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
     
     TH1D*       dstarpt ;
     TTree *     dstartree;
     double      pt_dmeson;
     double      eta_paifromdstar;
//     daughterparticle *paionfromdstar;
     dmeson *    dmeson_dstar;
     daughterparticle * particlefromdmeson;
     daughterparticle * particle1frommiddle;
     daughterparticle * particle2frommiddle;
     motherquark *      dmesonmotherquark;
     middleparticle *   particledecayed;
	 edm::InputTag genParticleSrc_;
     
}; 


Dstar_ccbar::Dstar_ccbar( const ParameterSet& iConfig )
{
	genParticleSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genpSrc",edm::InputTag("hiGenParticles"));
    particlefromdmeson = new daughterparticle();
    particle1frommiddle = new daughterparticle();
    particle2frommiddle = new daughterparticle();
    dmesonmotherquark =  new motherquark();
    particledecayed = new middleparticle();
    dmeson_dstar = new dmeson();
// actually, pset is NOT in use - we keep it here just for illustratory putposes

//  dstartree = new TTree("dstartree", "dstartree");
//  pai = new daughterparticle();
//  dstartree->Branch("pai","daughterparticle",&pai,32000,1);

}

void Dstar_ccbar::beginJob()
{
  Service<TFileService> fs; 
  dstartree = fs->make<TTree>("dstartree","dstartree");
//  dstartree->Branch("pai","daughterparticle",&paionfromdstar,32000,0);
  dstartree->Branch("dstar","dmeson",&dmeson_dstar,32000,1);  
  
  return ;
  
}

void Dstar_ccbar::daughterfill(daughterparticle *daughter, const Candidate * candidate)
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

void Dstar_ccbar::motherfill(motherquark *mother, const Candidate * candidate)
{
       mother->pt = candidate->pt();
       mother->charge = candidate->charge();
       mother->pid = candidate->pdgId();
       mother->phi = candidate->phi();
       mother->eta = candidate->eta();
       mother->mass = candidate->mass();
  return;
}

void Dstar_ccbar::middlefill(middleparticle *middle, const Candidate * candidate)
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

void Dstar_ccbar::dmesonfill(dmeson *d, const Candidate * candidate)
{
       d->pt = candidate->pt();
       d->charge = candidate->charge();
	   if ( candidate->mother())
           d->motherid = (candidate->mother())->pdgId();
	   else d->motherid = -999;
       d->pid = candidate->pdgId();
       d->phi = candidate->phi();
       d->eta = candidate->eta();
       d->mass = candidate->mass();
       d->numberofdaughter = candidate->numberOfDaughters();
  return;
}


void Dstar_ccbar::analyze( const Event& e, const EventSetup& )
{
  
  int numberofdstar = 0;
 
  
  Handle<GenParticleCollection> genParticles;
//  e.getByLabel("genParticles", genParticles);
  e.getByLabel(genParticleSrc_, genParticles);
//  cout << "particle number = " << genParticles->size() << endl;


  for(size_t i = 0; i < genParticles->size(); ++ i) {
     const GenParticle & p = (*genParticles)[i];
     int id = p.pdgId();
     int st = p.status();  
     if (abs(id) != 413 || abs(st) != 2)   continue;
     numberofdstar++;
//     const Candidate * mom1 = p.mother();
//     const Candidate * mom = mom1->mother();
//     cout << "pid of mother's mother = " << (mom->mother())->pdgId() << endl;
//     const Candidate * ancester = p.();
//     double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
//     pt_dmeson = pt;
//     double vx = p.vx(), vy = p.vy(), vz = p.vz();
//     int charge = p.charge();
     int n = p.numberOfDaughters();
     if ( n != 2) continue;
     const Candidate * d0 = NULL;
     const Candidate * pai = NULL;
     const Candidate * paionfromd0 = NULL;
     const Candidate * kaionfromd0 = NULL;
     if (!((abs(p.daughter(0)->pdgId()) == 421 && abs( p.daughter(1)->pdgId()) == 211) ||( abs(p.daughter(0)->pdgId()) == 211 && abs( p.daughter(1)->pdgId()) == 421)))   
           continue;
     if ( abs(p.daughter(0)->pdgId()) == 421 && abs( p.daughter(1)->pdgId()) == 211 )   { d0 = p.daughter(0); pai = p.daughter(1); }
     if ( abs(p.daughter(0)->pdgId()) == 211 && abs( p.daughter(1)->pdgId()) == 421 )   { d0 = p.daughter(1); pai = p.daughter(0); }
  //    cout << "Dstar to pai d0" << endl;
  //    cout << "D0 daughter number" << d0->numberOfDaughters()<<endl;
     if (!(d0->status() == 2 && d0->numberOfDaughters() == 2)) continue;
     if (!((abs(d0->daughter(0)->pdgId())==321&&abs(d0->daughter(1)->pdgId())==211)||(abs(d0->daughter(0)->pdgId())==211&&abs(d0->daughter(1)->pdgId())==321)))
           continue;
     if(d0->daughter(0)->charge() * d0->daughter(1)->charge() > 0)   continue;
     if(abs(d0->daughter(0)->pdgId())==321&&abs(d0->daughter(1)->pdgId())==211)   {  paionfromd0 = d0->daughter(1); kaionfromd0 = d0->daughter(0) ;}
     if(abs(d0->daughter(0)->pdgId())==211&&abs(d0->daughter(1)->pdgId())==321)   {  paionfromd0 = d0->daughter(0); kaionfromd0 = d0->daughter(1) ;}

     const Candidate * ddstar = &p;
//     cout << "Dstar to D0 pai to pai kai pai" << endl;
     
     daughterfill(particlefromdmeson,pai);
//     motherfill(dmesonmotherquark,mom);
     middlefill(particledecayed,d0);
     daughterfill(particle1frommiddle,paionfromd0);
     daughterfill(particle2frommiddle,kaionfromd0);
     dmesonfill (dmeson_dstar,ddstar);
//     (dmeson_dstar->momquark).push_back(*dmesonmotherquark);
     (dmeson_dstar->middle).push_back(*particledecayed);
     (dmeson_dstar->daughters).push_back(*particlefromdmeson);
     (dmeson_dstar->daughters).push_back(*particle1frommiddle);
     (dmeson_dstar->daughters).push_back(*particle2frommiddle); 

     dstartree->Fill();
     dmeson_dstar->clear();
     particlefromdmeson->clear();
     dmesonmotherquark->clear();
     particledecayed->clear();
     particle1frommiddle->clear();
     particle2frommiddle->clear();
   }
 

//   cout << "number of Dstar =====" << numberofdstar << endl;
  
   
   return ;
   
}

void Dstar_ccbar::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void Dstar_ccbar::endJob()
{
   
   return ;
}
 
typedef Dstar_ccbar Dstar_ccbar_Analyzer;
DEFINE_FWK_MODULE(Dstar_ccbar_Analyzer);
