#ifndef TrkAnalyzerUtils_h_
#define TrkAnalyzerUtils_h_

#include <memory>
#include <iostream>
#include <string>
#include <fstream>

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "TMath.h"


using namespace std;
using namespace reco;

bool decideDmesonindaughters( const Candidate * pin )
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
        Dmesonindaughters = decideDmesonindaughters(daughterparticle);
        if( Dmesonindaughters )  return true;
    }
    return Dmesonindaughters;
}



bool decidesaveitornot( const Candidate * pin )
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
        bool Dmesonindaughters = decideDmesonindaughters( pin );
        if( Dmesonindaughters )  tosaveit = true;
    }
    return tosaveit;

}

#endif
