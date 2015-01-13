#include "HFDstar.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

#include "UserCode/OpenHF/interface/HFMasses.hh"
#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

using std::cout;
using std::endl;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
HFDstar::HFDstar(const edm::ParameterSet& iConfig) :
    HFVirtualDecay(iConfig),
    fSlowPionPt(iConfig.getUntrackedParameter<double>("slowPionPt", 0.4)),
    fD0Window(iConfig.getUntrackedParameter<double>("D0Window", 0.1)),
    fDeltaM(iConfig.getUntrackedParameter<double>("deltaM", 0.03))
{
    dumpConfiguration();
} // HFDstar()

void HFDstar::dumpConfiguration()
{
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFDstar constructor" << endl;
    HFVirtualDecay::dumpConfiguration();
    cout << "---  slowPionPt:               " << fSlowPionPt << endl;
    cout << "---  D0Window:                 " << fD0Window << endl;
    cout << "---  deltaM:                   " << fDeltaM << endl;
    cout << "----------------------------------------------------------------------" << endl;
} // dumpConfiguration()

// ----------------------------------------------------------------------
void HFDstar::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    typedef HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsSet;

    try {
        HFVirtualDecay::analyze(iEvent,iSetup);
    }
    catch (HFSetupException e) {
        cout << "==>HFDstar> " << e.fMsg << endl;
        return; // problem with setup
    }

    std::vector<int> trkFastList = fListBuilder->getTrackList();
    fListBuilder->setMinPt(fSlowPionPt);
    fListBuilder->setMaxDocaToTracks(fMaxDoca);
    fListBuilder->setCloseTracks(&trkFastList);
    std::vector<int> trkSlowList = fListBuilder->getTrackList();

    HFTwoParticleCombinatoricsNew a(fTracksHandle,fVerbose);
    HFTwoParticleCombinatoricsSet kapiSet = a.combine(trkFastList, MKAON, trkFastList, MPION, MD0-fD0Window,MD0+fD0Window, 0);

    TLorentzVector ka,pi,d0,pis,dstar;
    for (HFTwoParticleCombinatoricsNew::iterator d0It = kapiSet.begin(); d0It != kapiSet.end(); ++d0It) {

        unsigned int iKaon = d0It->first;
        unsigned int iPion = d0It->second;

        reco::TrackBaseRef kaonTrackView(fTracksHandle, iKaon);
        reco::Track tKaon(*kaonTrackView);
        ka.SetPtEtaPhiM(tKaon.pt(), tKaon.eta(), tKaon.phi(), MKAON);

        reco::TrackBaseRef pionTrackView(fTracksHandle, iPion);
        reco::Track tPion(*pionTrackView);
        pi.SetPtEtaPhiM(tPion.pt(), tPion.eta(), tPion.phi(), MPION);

        if( tKaon.charge()*tPion.charge() > 0 ) continue;

        d0 = ka + pi;

        for (unsigned iTrack = 0; iTrack < trkSlowList.size(); iTrack++) {

            unsigned int sPion = trkSlowList[iTrack];

            if (sPion == iKaon || sPion == iPion) continue;

            reco::TrackBaseRef rTrackView(fTracksHandle, sPion);
            reco::Track tSlowPion(*rTrackView);

            // -- the slow pion has the same charge like the fast pion
            if (tSlowPion.charge()*tPion.charge() < 0) continue;

            pis.SetXYZM(tSlowPion.px(), tSlowPion.py(), tSlowPion.pz(), MPION);
            if (d0.DeltaR(pis) > fDeltaR) continue;

            dstar = d0 + pis;

            if (TMath::Abs(dstar.M() - d0.M()) > 0.16) continue;
			if ( dstar.Pt() < 7.0 )  continue;


            HFDecayTree theTree(fType, true, MDSTARPLUS, false, -1.0, true);
            HFDecayTreeIterator iterator = theTree.addDecayTree(421, true, MD0, false);
            iterator->addTrack(iKaon,321);
            iterator->addTrack(iPion,211);
            iterator->setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));
            theTree.addTrack(sPion,211);
            theTree.setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));

            fSequentialFitter->doFit(&theTree);
        }
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDstar);
