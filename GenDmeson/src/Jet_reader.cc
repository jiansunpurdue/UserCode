#include <iostream>
#include <typeinfo>
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
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "RecoJets/JetAnalyzers/interface/JetPlotsExample.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JPTJetCollection.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <Math/DistFunc.h>
#include "TMath.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTrigger.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"



using namespace edm;
using namespace std;
using namespace reco;



class Jet_reader : public edm::EDAnalyzer
{

   public:
   
      //
      explicit Jet_reader( const edm::ParameterSet& ) ;
      virtual ~Jet_reader() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob() ;
      virtual void endRun( const edm::Run&, const edm::EventSetup& ) ;
      virtual void endJob() ;

   private:
     
     TTree *     regitjetstree;
     
}; 


Jet_reader::Jet_reader( const ParameterSet& pset )
{

}

void Jet_reader::beginJob()
{
  Service<TFileService> fs; 
  regitjetstree = fs->make<TTree>("regitjetstree","regitjetstree");
//  regitjetstree->Branch("pai","daughterparticle",&paionfromregitjets,32000,0);
  
  return ;
  
}



void Jet_reader::analyze( const Event& e, const EventSetup& )
{
  
  Handle<edm::View<reco::Candidate>> RegitJets;
  e.getByLabel("akVs3PFSelectedJets", RegitJets);
  cout << " size of reigt jets: " << RegitJets->size() << endl;
  const reco::Candidate & myJet = (*RegitJets)[0];

  cout << " x momentum: " << myJet.momentum().x() << endl;
//  cout << " pt of first jet: " << (*(RegitJets->begin()))->pt() << endl;
//  cout << " pt of first jet: " << ((*RegitJets)[0]).pt() << endl;
  
//  for ( edm::RefToBaseVector<reco::Candidate>::const_iterator jet = RegitJets->begin() ; jet != RegitJets-> end(); jet++)
//  {
//	  if(!((*(*jet)).pt())) cout << "no objects inside!" << endl;
//	  cout << " jet pt: " << (*(*jet)).pt() << endl;
//	  if(!(*jet) )   cout << " no jet " << endl;
   //   cout << " jet type : " << typeid(*(*jet)).name() << endl;
//  }


   return ;
   
}

void Jet_reader::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void Jet_reader::endJob()
{
   
   return ;
}
 
typedef Jet_reader Jet_reader_Analyzer;
DEFINE_FWK_MODULE(Jet_reader_Analyzer);
