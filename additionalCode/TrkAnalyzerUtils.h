#ifndef TrkAnalyzerUtils_h_
#define TrkAnalyzerUtils_h_

#include <vector>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

//#include "HeavyIonsAnalysis/TrackAnalysis/interface/GenParticlefilter.h"

using namespace std;
using namespace reco;
using namespace edm;

//bool decideDmesonindaughters2( const Candidate * pin );
//bool decidesaveitornot2( const Candidate * pin );


bool decideDmesonindaughters2( const Candidate * pin )
{
    bool Dmesonindaughters = false;
    if( pin->numberOfDaughters() == 0 ) return false;
    const Candidate * daughterparticle = NULL;
    for( unsigned int idau = 0; idau < pin->numberOfDaughters(); idau ++ )
    {
        daughterparticle = pin->daughter(idau);
        if( TMath::Abs( daughterparticle->pdgId() ) > 400 && TMath::Abs( daughterparticle->pdgId() ) < 500 )
        {
            return true;
        }
		if( daughterparticle == pin )   continue;
        Dmesonindaughters = decideDmesonindaughters2(daughterparticle);
        if( Dmesonindaughters )  return true;
    }
    return Dmesonindaughters;
}


bool decidesaveitornot2( const Candidate * pin )
{
    bool tosaveit = false;

    //save if if it is a D meson
    if( TMath::Abs( pin->pdgId() ) > 400 && TMath::Abs( pin->pdgId() ) < 500 ) return true;

    if( pin->numberOfMothers() != 1 )  return false;

    //save it if there is D mesons in mother chain (tracing back)
    const Candidate * mom = pin->mother();
    if( TMath::Abs( mom->pdgId() ) > 400 && TMath::Abs( mom->pdgId() ) < 500 ) tosaveit = true;
    while( !tosaveit && mom->numberOfMothers() == 1 )
    {
        if( TMath::Abs( mom->pdgId() ) > 400 && TMath::Abs( mom->pdgId() ) < 500 )
          tosaveit = true;
		if( mom == mom->mother() )  break;
        mom = mom->mother();
    }

    //save it if there is D mesons in daughter chain (tracing forward )
    if( !tosaveit )
    {
        bool Dmesonindaughters = decideDmesonindaughters2( pin );
        if( Dmesonindaughters )  tosaveit = true;
    }
    return tosaveit;

}


const TrackingParticle* doRecoToTpMatch(reco::RecoToSimCollection recSimColl, const reco::TrackRef &in){

    //if(in.status()!=1) return NULL;
    reco::RecoToSimCollection::const_iterator matchedSim = recSimColl.find(edm::RefToBase<reco::Track>(in));
    if(matchedSim==recSimColl.end()) return NULL;
    else{
        const TrackingParticle *tparticle = matchedSim->val[0].first.get();
        return tparticle;
    }
}

//-------------------------------------------------------------------------------------------------

vector<int> matchTpToGen(const edm::Event& iEvent, const TrackingParticle* tparticle, std::string genCollection="hiGenParticles"){

    vector<int> retArr;
    if(!tparticle){ retArr.push_back(-999); return retArr; }
    vector<HepMC::GenParticle> tempStore;
    for(TrackingParticle::genp_iterator igen=tparticle->genParticle_begin(); igen!=tparticle->genParticle_end(); igen++){
        const HepMC::GenParticle* temp = igen->get();
        tempStore.push_back(*temp); //temp->momentum().px()*temp->momentum().py()*temp->momentum().pz(); //store HepMC barcode for unique id
    }

    //now figure out the array number of the associated genParticles
    bool *tripwire = new bool[tempStore.size()];
    for(unsigned int ii=0; ii<tempStore.size(); ii++){ tripwire[ii]=0; } //reset tripwires

    edm::Handle<reco::GenParticleCollection> parts;
    iEvent.getByLabel((edm::InputTag)genCollection,parts);
    for(unsigned int igenCand=0; igenCand<tempStore.size(); igenCand++){
		UInt_t savedidx = -1;
        for(UInt_t igenP = 0; igenP < parts->size(); ++igenP){
            const reco::GenParticle& p = (*parts)[igenP];
            ////////////////////added on 8/27/2015
            bool tosaveit = false;
            const Candidate * pcand = &p;
            tosaveit = decidesaveitornot2( pcand );
            if( !tosaveit ) continue;
            savedidx++;
            ////////////////////end added on 8/27/2015
            if(tripwire[igenCand]) continue;
            //there's no equivalence operator for genParticle!! This is a workaround since the stupid things are floats
            //if(p.px()*p.py()*p.pz() == tempStore.at(igenCand).momentum().px()*tempStore.at(igenCand).momentum().py()*tempStore.at(igenCand).momentum().pz()){
//            if(fabs((p.px()+p.py()+p.pz()) - (tempStore.at(igenCand).momentum().px()+tempStore.at(igenCand).momentum().py()+tempStore.at(igenCand).momentum().pz())) < 0.001)
             if(fabs(p.pt()-tempStore.at(igenCand).momentum().perp()) < 0.001 && fabs ( p.eta() - tempStore.at(igenCand).momentum().eta() ) < 0.001 && fabs ( p.phi() - tempStore.at(igenCand).momentum().phi() ) < 0.001)
			{
                retArr.push_back(savedidx);  //genParticle instance number;
                tripwire[igenCand]=1;
            }
        }
        if(!tripwire[igenCand]) retArr.push_back(-999);
    }
    delete[] tripwire;

    return retArr;
}

#endif
