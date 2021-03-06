/*
 *  HFVirtualDecay.cc
 *
 *  Created by Christoph Nägeli <christoph.naegeli@psi.ch> on 31.01.13.
 *
 */

#include "HFVirtualDecay.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/Common/interface/Handle.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include <TString.h>

HFVirtualDecay::HFVirtualDecay(const edm::ParameterSet& iConfig) :
	fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
	fTracksLabel(iConfig.getUntrackedParameter<edm::InputTag>("tracksLabel", edm::InputTag("generalTracks"))),
	fTrackQualityString(iConfig.getUntrackedParameter<std::string>("trackQualityString",std::string("highPurity"))),
	fPrimaryVertexLabel(iConfig.getUntrackedParameter<edm::InputTag>("PrimaryVertexLabel", edm::InputTag("offlinePrimaryVertices"))),
	fBeamSpotLabel(iConfig.getUntrackedParameter<edm::InputTag>("BeamSpotLabel", edm::InputTag("offlineBeamSpot"))),
	fTrackPt(iConfig.getUntrackedParameter<double>("trackPt", 3.0)),
	fDeltaR(iConfig.getUntrackedParameter<double>("deltaR", 99.0)),
	fMaxDoca(iConfig.getUntrackedParameter<double>("maxDoca", 0.2)),
	fMaxD0(iConfig.getUntrackedParameter<double>("maxD0", 999.)),
	fMaxDz(iConfig.getUntrackedParameter<double>("maxDz", 999.)),
	fPvWeight(iConfig.getUntrackedParameter<double>("pvWeight", 0.6)),
	fType(iConfig.getUntrackedParameter<int>("type")), 
    isPbPb(iConfig.getUntrackedParameter<bool>("isPbPb", false)),
	isMC(iConfig.getUntrackedParameter<bool>("isMC", false))    //just used for pbpb, not used for pp
{
} // HFVirtualDecay()

void HFVirtualDecay::dumpConfiguration()
{
	using namespace std;
	cout << "---  verbose                     " << fVerbose << endl;
	cout << "---  tracksLabel                 " << fTracksLabel << endl;
	cout << "---  trackQualityString          " << fTrackQualityString << endl;
	cout << "---  PrimaryVertexLabel          " << fPrimaryVertexLabel << endl;
	cout << "---  BeamSpotLabel               " << fBeamSpotLabel << endl;
	
	cout << "---  trackPt                     " << fTrackPt << endl;
	cout << "---  deltaR                      " << fDeltaR << endl;
	cout << "---  maxDoca                     " << fMaxDoca << endl;
	cout << "---  maxD0                       " << fMaxD0 << endl;
	cout << "---  maxDz                       " << fMaxDz << endl;
	cout << "---  pvWeight:                   " << fPvWeight << endl;
	
	cout << "---  type                        " <<  fType << endl;
	cout << "---  isPbPb                      " <<  isPbPb << endl;
	cout << "---  isMC (just for PbPb)        " <<  isMC << endl;
} // dumpConfiguration()

void HFVirtualDecay::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
	using std::cout; using std::endl;
	
	// -- load the magnetic field
	edm::ESHandle<MagneticField> fieldHandle;
	iSetup.get<IdealMagneticFieldRecord>().get(fieldHandle);
	fMagneticField = fieldHandle.product();
	
	// -- load the primary vertices
	edm::Handle<reco::VertexCollection> vertexHandle;
	iEvent.getByLabel(fPrimaryVertexLabel, vertexHandle);
	if (!vertexHandle.isValid()) throw HFSetupException("No primary vertex collection found, skipping");
	fVertexCollection = *vertexHandle;
	if (fVertexCollection.size() == 0) throw HFSetupException("Primary vertex collection is empty, skipping");
	
	// -- load the beam spot
	edm::Handle<reco::BeamSpot> bspotHandle;
	iEvent.getByLabel(fBeamSpotLabel, bspotHandle);
	if (!bspotHandle.isValid()) throw HFSetupException("No beamspot found, skipping");
	fBeamSpot = *bspotHandle;
	
	// -- load the tracks
	iEvent.getByLabel(fTracksLabel, fTracksHandle);
	if (!fTracksHandle.isValid()) throw HFSetupException(Form("No valid TrackCollection with label '%s' found, skipping",fTracksLabel.encode().c_str()));
	
	// -- load the transient track builder
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", fTTB);
	if (!fTTB.isValid()) throw HFSetupException("Error: no TransientTrackBuilder found");
	
	// -- construct a list builder
	fListBuilder.reset(new  HFTrackListBuilder(fTracksHandle, fTTB.product(), fVerbose, isPbPb));
	fListBuilder->setMaxD0(fMaxD0);
	fListBuilder->setMaxDz(fMaxDz);
	fListBuilder->setMinPt(fTrackPt);
	fListBuilder->setTrackQuality(fTrackQualityString);

	// -- construct the sequential vertex fitter
	fSequentialFitter.reset(new HFSequentialVertexFit(fTracksHandle, fTTB.product(), vertexHandle, fMagneticField, fBeamSpot, fVerbose, true, isPbPb, isMC));
} // analyze()
