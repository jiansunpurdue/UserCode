#include <iostream>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "DataFormats/TrackReco/interface/Track.h"
// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TMath.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
using namespace edm;
using namespace std;
using namespace reco;



class D0_bkg_costhetastar : public edm::EDAnalyzer
{

   public:
   
      //
      explicit D0_bkg_costhetastar( const edm::ParameterSet& ) ;
      virtual ~D0_bkg_costhetastar() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob() ;
      virtual void endRun( const edm::Run&, const edm::EventSetup& ) ;
      virtual void endJob() ;
	  void Reset_values();

   private:
     
     TTree *     candtree;
     double      pt_dmeson;
     double      eta_paifromd0;
	 float fpt1 ;
	 float fpt2 ;
	 float feta1 ;
	 float feta2 ;
	 float fphi1 ;
	 float fphi2 ;
	 float fmass1;
	 float fmass2 ;
	 float fmass ;
	 float p_D_x ;
	 float p_D_y ;
	 float p_D_z ;
//     daughterparticle *paionfromd0;
     
}; 


D0_bkg_costhetastar::D0_bkg_costhetastar( const ParameterSet& pset )
{
// actually, pset is NOT in use - we keep it here just for illustratory putposes

//  candtree = new TTree("candtree", "candtree");
//  pai = new daughterparticle();
//  candtree->Branch("pai","daughterparticle",&pai,32000,1);

}
void D0_bkg_costhetastar::Reset_values()
{
	fpt1 = -10.;
	fpt2 = -10.;
	fphi1 = -10.;
	fphi2 = -10.;
	feta1 = -10.;
	feta2 = -10.;
	fmass1 = -10.;
	fmass2 = -10.;
	fmass  = -10.;
	p_D_x = -10.;
	p_D_y = -10.;
	p_D_z = -10.;


}

void D0_bkg_costhetastar::beginJob()
{

	fpt1 = -10.;
	fpt2 = -10.;
	fphi1 = -10.;
	fphi2 = -10.;
	feta1 = -10.;
	feta2 = -10.;
	fmass1 = -10.;
	fmass2 = -10.;
	fmass  = -10.;
	p_D_x = -10.;
	p_D_y = -10.;
	p_D_z = -10.;
  Service<TFileService> fs; 
  candtree = fs->make<TTree>("candtree","candtree");
  

	candtree->Branch("fmass",&fmass,"fmass/F");
    candtree->Branch("p_D_x",&p_D_x,"p_D_x/F");
	candtree->Branch("p_D_y",&p_D_y,"p_D_y/F");
	candtree->Branch("p_D_z",&p_D_z,"p_D_z/F");
    candtree->Branch("fmass1",&fmass1,"fmass1/F"); 
    candtree->Branch("fmass2",&fmass2,"fmass2/F");
    candtree->Branch("fpt1",&fpt1,"fpt1/F");
    candtree->Branch("fpt2",&fpt2,"fpt2/F");
    candtree->Branch("fphi1",&fphi1,"fphi1/F");
    candtree->Branch("fphi2",&fphi2,"fphi2/F");
    candtree->Branch("feta1",&feta1,"feta1/F");
    candtree->Branch("feta2",&feta2,"feta2/F");
  

  
  return ;
  
}

void D0_bkg_costhetastar::analyze( const Event& e, const EventSetup& )
{
  
  // here's an example of accessing GenEventInfoProduct
  
  Handle<GenParticleCollection> genParticles;
  e.getByLabel("genParticles", genParticles);


  for(size_t i = 0; i < genParticles->size() - 1 ; i++) {
	for(size_t j = i+1; j < genParticles->size(); j++) {
      const GenParticle & p_1 = (*genParticles)[i];
	  const GenParticle & p_2 = (*genParticles)[j];
	  const Candidate * paionfromd0 = NULL;
	  const Candidate * kaionfromd0 = NULL;

	  int id_1 = p_1.pdgId();
	  int id_2 = p_2.pdgId();
	  int charge_1 = p_1.charge();
	  int charge_2 = p_2.charge();

	  if(!((abs(id_1) == 321 && abs(id_2) == 211) || (abs(id_1) == 211 && abs(id_2) == 321)))    continue;
	  if( charge_1 * charge_2 > 0 || charge_1 * charge_2 == 0)  continue;

	  if ( abs(id_1) == 321 && abs(id_2) == 211 )   { paionfromd0 = &p_2; kaionfromd0 = &p_1; }
	  if ( abs(id_1) == 211 && abs(id_2) == 321 )   { paionfromd0 = &p_1; kaionfromd0 = &p_2; }

	  Reset_values();

	  fpt1 = paionfromd0->pt();
	  fphi1 = paionfromd0->phi();
	  feta1 = paionfromd0->eta();
	  fmass1 = paionfromd0->mass();
	  fpt2 = kaionfromd0->pt();
	  fphi2 = kaionfromd0->phi();
	  feta2 = kaionfromd0->eta();
	  fmass2 = kaionfromd0->mass();

	  p_D_x = paionfromd0->px()+kaionfromd0->px();
	  p_D_y = paionfromd0->py()+kaionfromd0->py();
	  p_D_z = paionfromd0->pz()+kaionfromd0->pz();

	//  cout << "p_D_y : " << p_D_y << " py pion: " << paionfromd0->py() << "*******" << fpt1*TMath::Sin(fphi1) << " py kaon: " << kaionfromd0->py() << "*******" << fpt2*TMath::Sin(fphi2) <<  endl;

	  fmass = TMath::Sqrt((TMath::Sqrt(fmass1 * fmass1 + paionfromd0->p()*paionfromd0->p() )+TMath::Sqrt( fmass2 * fmass2 + kaionfromd0->p()* kaionfromd0->p())) * (TMath::Sqrt(fmass1 * fmass1 + paionfromd0->p()*paionfromd0->p() )+TMath::Sqrt( fmass2 * fmass2 + kaionfromd0->p()* kaionfromd0->p()))  - (p_D_x * p_D_x + p_D_y * p_D_y  + p_D_z * p_D_z));
 

      candtree->Fill();
     
	 
	 }
    

   }
 

   
   return ;
   
}

void D0_bkg_costhetastar::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void D0_bkg_costhetastar::endJob()
{
   
   return ;
}
 
typedef D0_bkg_costhetastar D0_bkg_costhetastar_Analyzer;
DEFINE_FWK_MODULE(D0_bkg_costhetastar_Analyzer);
