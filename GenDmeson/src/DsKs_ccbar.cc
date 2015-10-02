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



class DsKs_ccbar : public edm::EDAnalyzer
{

   public:
   
      //
      explicit DsKs_ccbar( const edm::ParameterSet& ) ;
      virtual ~DsKs_ccbar() {} // no need to delete ROOT stuff
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
     
     TH1D*       dskspt ;
     TTree *     dskstree;
     double      pt_dmeson;
     double      eta_paifromdsks;
//     daughterparticle *paionfromdsks;
     dmeson *    dmeson_dsks;
     daughterparticle * particlefromdmeson;
     daughterparticle * particle1frommiddle;
     daughterparticle * particle2frommiddle;
     motherquark *      dmesonmotherquark;
     middleparticle *   particledecayed;
	 edm::InputTag genParticleSrc_;
     
}; 


DsKs_ccbar::DsKs_ccbar( const ParameterSet& iConfig )
{
	genParticleSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genpSrc",edm::InputTag("hiGenParticles"));
    particlefromdmeson = new daughterparticle();
    particle1frommiddle = new daughterparticle();
    particle2frommiddle = new daughterparticle();
    dmesonmotherquark =  new motherquark();
    particledecayed = new middleparticle();
    dmeson_dsks = new dmeson();
// actually, pset is NOT in use - we keep it here just for illustratory putposes

//  dskstree = new TTree("dskstree", "dskstree");
//  pai = new daughterparticle();
//  dskstree->Branch("pai","daughterparticle",&pai,32000,1);

}

void DsKs_ccbar::beginJob()
{
  Service<TFileService> fs; 
  dskstree = fs->make<TTree>("dskstree","dskstree");
//  dskstree->Branch("pai","daughterparticle",&paionfromdsks,32000,0);
  dskstree->Branch("dsks","dmeson",&dmeson_dsks,32000,1);  
  
  return ;
  
}

void DsKs_ccbar::daughterfill(daughterparticle *daughter, const Candidate * candidate)
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

void DsKs_ccbar::motherfill(motherquark *mother, const Candidate * candidate)
{
       mother->pt = candidate->pt();
       mother->charge = candidate->charge();
       mother->pid = candidate->pdgId();
       mother->phi = candidate->phi();
       mother->eta = candidate->eta();
       mother->mass = candidate->mass();
  return;
}

void DsKs_ccbar::middlefill(middleparticle *middle, const Candidate * candidate)
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

void DsKs_ccbar::dmesonfill(dmeson *d, const Candidate * candidate)
{
       d->pt = candidate->pt();
       d->charge = candidate->charge();
	   if(candidate->mother())
            d->motherid = (candidate->mother())->pdgId();
	   else d->motherid = -999;
       d->pid = candidate->pdgId();
       d->phi = candidate->phi();
       d->eta = candidate->eta();
       d->mass = candidate->mass();
       d->numberofdaughter = candidate->numberOfDaughters();
  return;
}


void DsKs_ccbar::analyze( const Event& e, const EventSetup& )
{
  

  int numberofdsks = 0;
  
  Handle<GenParticleCollection> genParticles;
//  e.getByLabel("genParticles", genParticles);
  e.getByLabel(genParticleSrc_, genParticles);
//  cout << "particle number = " << genParticles->size() << endl;


  for(size_t i = 0; i < genParticles->size(); ++ i) {
     const GenParticle & p = (*genParticles)[i];
     int id = p.pdgId();
     int st = p.status();  
     if (abs(id) != 431 || abs(st) != 2)   continue;
     numberofdsks++;
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
     const Candidate * kstar = NULL;
     const Candidate * kai = NULL;
     const Candidate * paionfromkstar = NULL;
     const Candidate * kaionfromkstar = NULL;
     if (!((abs(p.daughter(0)->pdgId()) == 313 && abs( p.daughter(1)->pdgId()) == 321) ||( abs(p.daughter(0)->pdgId()) == 321 &&  abs(p.daughter(1)->pdgId()) == 313)))   
           continue;
     if ( abs(p.daughter(0)->pdgId()) == 313 &&  abs(p.daughter(1)->pdgId()) == 321 )   { kstar = p.daughter(0); kai = p.daughter(1); }
     if ( abs(p.daughter(0)->pdgId()) == 321 &&  abs(p.daughter(1)->pdgId()) == 313 )   { kstar = p.daughter(1); kai = p.daughter(0); }
  //    cout << "DsKs to pai kstar" << endl;
  //    cout << "D0 daughter number" << kstar->numberOfDaughters()<<endl;
     if (!(kstar->status() == 2 && kstar->numberOfDaughters() == 2)) continue;
     if (!((abs(kstar->daughter(0)->pdgId())==321&&abs(kstar->daughter(1)->pdgId())==211)||(abs(kstar->daughter(0)->pdgId())==211&&abs(kstar->daughter(1)->pdgId())==321)))
           continue;
     if(abs(kstar->daughter(0)->pdgId())==321&&abs(kstar->daughter(1)->pdgId())==211)   {  paionfromkstar = kstar->daughter(1); kaionfromkstar = kstar->daughter(0) ;}
     if(abs(kstar->daughter(0)->pdgId())==211&&abs(kstar->daughter(1)->pdgId())==321)   {  paionfromkstar = kstar->daughter(0); kaionfromkstar = kstar->daughter(1) ;}

     const Candidate * ddsks = &p;
//     cout << "DsKs to kstar pai to pai kai kai" << endl;
     
     daughterfill(particlefromdmeson,kai);
//     motherfill(dmesonmotherquark,mom);
     middlefill(particledecayed,kstar);
     daughterfill(particle1frommiddle,paionfromkstar);
     daughterfill(particle2frommiddle,kaionfromkstar);
     dmesonfill (dmeson_dsks,ddsks);
//     (dmeson_dsks->momquark).push_back(*dmesonmotherquark);
     (dmeson_dsks->middle).push_back(*particledecayed);
     (dmeson_dsks->daughters).push_back(*particlefromdmeson);
     (dmeson_dsks->daughters).push_back(*particle1frommiddle);
     (dmeson_dsks->daughters).push_back(*particle2frommiddle); 

     dskstree->Fill();
     dmeson_dsks->clear();
     particlefromdmeson->clear();
     dmesonmotherquark->clear();
     particledecayed->clear();
     particle1frommiddle->clear();
     particle2frommiddle->clear();
   }
 

//   cout << "number of DsKs =====" << numberofdsks << endl;
  
   
   return ;
   
}

void DsKs_ccbar::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void DsKs_ccbar::endJob()
{
   
   return ;
}
 
typedef DsKs_ccbar DsKs_ccbar_Analyzer;
DEFINE_FWK_MODULE(DsKs_ccbar_Analyzer);
