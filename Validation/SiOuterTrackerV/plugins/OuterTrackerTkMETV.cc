// Package:    SiOuterTrackerV
// Class:      SiOuterTrackerV


// Original Author:  Emily MacDonald
//

// system include files
#include <memory>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "Validation/SiOuterTrackerV/interface/OuterTrackerTkMETV.h"
#include "DataFormats/L1TVertex/interface/Vertex.h"



//
// constructors and destructor
//
OuterTrackerTkMETV::OuterTrackerTkMETV(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
  trackingParticleToken_ = consumes< std::vector < TrackingParticle > >(conf_.getParameter<edm::InputTag>("trackingParticleToken"));
  ttStubMCTruthToken_ = consumes < TTStubAssociationMap < Ref_Phase2TrackerDigi_ > > (conf_.getParameter<edm::InputTag>("MCTruthStubInputTag"));
  ttClusterMCTruthToken_ = consumes< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > >(conf_.getParameter<edm::InputTag>("MCTruthClusterInputTag"));
  ttTrackMCTruthToken_ = consumes< TTTrackAssociationMap< Ref_Phase2TrackerDigi_ > > (conf_.getParameter<edm::InputTag>("MCTruthTrackInputTag"));
  ttStubToken_ = consumes < edmNew::DetSetVector < TTStub < Ref_Phase2TrackerDigi_ > > > (conf_.getParameter<edm::InputTag>("StubInputTag"));
  ttTrackToken_ = consumes< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > (conf_.getParameter<edm::InputTag>("TTTracksTag"));
  pvToken = consumes< l1t::VertexCollection > (conf_.getParameter<edm::InputTag>("L1VertexInputTag"));

  L1Tk_nPar        = conf_.getParameter< int >("L1Tk_nPar"); //4 or 5(d0) track parameters
  L1Tk_minPt    = conf_.getParameter< double >("L1Tk_minPt"); //min pT
  L1Tk_maxPt    = conf_.getParameter< double >("L1Tk_minPt"); //max pT
  L1Tk_maxEta   = conf_.getParameter< double >("L1Tk_maxEta"); //max eta
  L1Tk_minNStub    = conf_.getParameter< int >("L1Tk_minNStub"); //min number of stubs in the track
  L1Tk_minNPSstub    = conf_.getParameter< int >("L1Tk_minNPSstub"); //min number of stubs in the track
  L1Tk_maxVtxZ    = conf_.getParameter< double >("L1Tk_maxVtxZ"); //min number of stubs in the track
  L1Tk_maxChi2dof  = conf_.getParameter< double >("L1Tk_maxChi2dof"); //maximum chi2/dof of the track
  L1Tk_maxBendChi2  = conf_.getParameter< double >("L1Tk_maxBendChi2"); //maximum chi2/dof of the track

  TP_minPt         = conf_.getParameter< double >("TP_minPt"); //min pT
  TP_maxEta        = conf_.getParameter< double >("TP_maxEta"); //max eta
  TP_maxVtxZ         = conf_.getParameter< double >("TP_maxVtxZ"); //max vertZ (or z0)
  TP_maxDxy         = conf_.getParameter< double >("TP_maxDxy"); //max vertR (or dxy)
  TP_minNStub      = conf_.getParameter< int >("TP_minNStub"); //min number of stubs in the tracking particle
  TP_minNLayers      = conf_.getParameter< int >("TP_minNLayers"); //min number of layers with stubs in the tracking particle
}
// OuterTrackerTkMETV::~OuterTrackerTkMETV()
// {
//   // do anything here that needs to be done at desctruction time
//   // (e.g. close files, deallocate resources etc.)
// }

// member functions

// ------------ method called for each event  ------------
void OuterTrackerTkMETV::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // Tracking Particles
  edm::Handle< std::vector < TrackingParticle > > trackingParticleHandle;
  iEvent.getByToken(trackingParticleToken_, trackingParticleHandle);

  // L1 Primaries
  edm::Handle< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > > > TTStubHandle;
  iEvent.getByToken(ttStubToken_, TTStubHandle);
  edm::Handle< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > TTTrackHandle;
  iEvent.getByToken(ttTrackToken_, TTTrackHandle);
  edm::Handle< l1t::VertexCollection > L1VertexHandle;
  iEvent.getByToken(pvToken,L1VertexHandle);

  // Truth Association Maps
  edm::Handle< TTTrackAssociationMap< Ref_Phase2TrackerDigi_ > > MCTruthTTTrackHandle;
  iEvent.getByToken(ttTrackMCTruthToken_, MCTruthTTTrackHandle);
  edm::Handle< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > > MCTruthTTClusterHandle;
  iEvent.getByToken(ttClusterMCTruthToken_, MCTruthTTClusterHandle);
  edm::Handle< TTStubAssociationMap< Ref_Phase2TrackerDigi_ > > MCTruthTTStubHandle;
  iEvent.getByToken(ttStubMCTruthToken_, MCTruthTTStubHandle);

  // Geometries
  edm::ESHandle< TrackerGeometry > tGeometryHandle;
  const TrackerGeometry* theTrackerGeometry;
  iSetup.get< TrackerDigiGeometryRecord >().get(tGeometryHandle);
  theTrackerGeometry = tGeometryHandle.product();

  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);

  const TrackerTopology* const tTopo = tTopoHandle.product();


  //Fill vectors for tracking particles
  float trueTkMETx = 0;
  float trueTkMETy = 0;
  float bins[]={0,20,30,40,55,75,120,150,250};


  if ( !trackingParticleHandle.isValid()) {
    edm::LogWarning("DataNotFound") << "Tracking particle handle invalid!\n";
    return;
  }

  int this_tp = 0;
  std::vector< TrackingParticle >::const_iterator iterTP;
  for (auto iterTP: *trackingParticleHandle) {
    edm::Ptr< TrackingParticle > tp_ptr(trackingParticleHandle, this_tp);
    this_tp++;

    bool tmp_tp_signal = (tp_ptr->eventId().event() == 0);
    if (tmp_tp_signal!=1) continue;

    float tmp_tp_pt  = tp_ptr->pt();
    float tmp_tp_eta = tp_ptr->eta();
    float tmp_tp_phi = tp_ptr->phi();
    float tmp_tp_vz  = tp_ptr->vz();
    float tmp_tp_vx  = tp_ptr->vx();
    float tmp_tp_vy  = tp_ptr->vy();

    if (tmp_tp_pt < TP_minPt) continue;
    if (fabs(tmp_tp_eta) > TP_maxEta) continue;

    float tmp_tp_t = tan(2.0*atan(1.0)-2.0*atan(exp(-tmp_tp_eta)));
    float delx = -tmp_tp_vx;
    float dely = -tmp_tp_vy;
    float A = 0.01*0.5696;
    float Kmagnitude = A / tmp_tp_pt;
    float tmp_tp_charge = tp_ptr->charge();

    if (tmp_tp_charge==0) continue;

    float K = Kmagnitude * tmp_tp_charge;
    float tmp_tp_x0p = delx - (1./(2. * K)*sin(tmp_tp_phi));
    float tmp_tp_y0p = dely + (1./(2. * K)*cos(tmp_tp_phi));
    float tmp_tp_rp = sqrt(tmp_tp_x0p*tmp_tp_x0p + tmp_tp_y0p*tmp_tp_y0p);
    float tmp_tp_d0 = tmp_tp_charge*tmp_tp_rp - (1. / (2. * K));
    tmp_tp_d0 = tmp_tp_d0*(-1); //fix d0 sign

    static double pi = 4.0*atan(1.0);
    float delphi = tmp_tp_phi-atan2(-K*tmp_tp_x0p,K*tmp_tp_y0p);
    if (delphi<-pi) delphi+=2.0*pi;
    if (delphi>pi) delphi-=2.0*pi;
    float tmp_tp_z0 = tmp_tp_vz+tmp_tp_t*delphi/(2.0*K);

    //if (fabs(tmp_tp_z0) > TP_maxZ0) continue;

    float dxy = sqrt(tmp_tp_vx*tmp_tp_vx + tmp_tp_vy*tmp_tp_vy);
    if (dxy > 1.0) continue;

    // only consider TPs associated with >= 1 cluster, or >= X stubs, or have stubs in >= X layers (configurable options)
    if (MCTruthTTClusterHandle.isValid()){
      if (MCTruthTTClusterHandle->findTTClusterRefs(tp_ptr).size() < 1) continue;
    }

    std::vector< edm::Ref< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > >, TTStub< Ref_Phase2TrackerDigi_ > > > theStubRefs = MCTruthTTStubHandle->findTTStubRefs(tp_ptr);
    int nStubTP = (int) theStubRefs.size();

    // how many layers/disks have stubs?
    int hasStubInLayer[11] = {0};
    for (unsigned int is=0; is<theStubRefs.size(); is++) {
      DetId detid( theStubRefs.at(is)->getDetId() );
      int layer = -1;
      if ( detid.subdetId()==StripSubdetector::TOB ) {
        layer = static_cast<int>(tTopo->layer(detid)) - 1; //fill in array as entries 0-5
      }
      else if ( detid.subdetId()==StripSubdetector::TID ) {
        layer = static_cast<int>(tTopo->layer(detid)) + 5; //fill in array as entries 6-10
      }

      //treat genuine stubs separately (==2 is genuine, ==1 is not)
      if (MCTruthTTStubHandle->findTrackingParticlePtr(theStubRefs.at(is)).isNull() && hasStubInLayer[layer]<2) hasStubInLayer[layer] = 1;
      else hasStubInLayer[layer] = 2;
    }

    int nStubLayerTP = 0;
    for (int isum=0; isum<11; isum++) {
      if ( hasStubInLayer[isum] >= 1) nStubLayerTP   += 1;
    }

    if (TP_minNStub>0 && nStubTP<TP_minNStub) continue;
    if (TP_minNLayers>0 && nStubLayerTP<TP_minNLayers) continue;

    trueTkMETx += tmp_tp_pt*cos(tmp_tp_phi);
    trueTkMETy += tmp_tp_pt*sin(tmp_tp_phi);

    trackParts_Eta->Fill(tmp_tp_eta);
    trackParts_Pt->Fill(tmp_tp_pt);
    trackParts_NStubs->Fill(nStubTP);
    trackParts_NLayers->Fill(nStubLayerTP);
    trackParts_VtxZ->Fill(tmp_tp_z0);
    trackParts_Dxy->Fill(dxy);
  } //end loop tracking particles

  float trueTkMET = sqrt(trueTkMETx*trueTkMETx + trueTkMETy*trueTkMETy);


  //Fill all vectors for the tracks (NOT matched tracks!)
  if ( !L1VertexHandle.isValid()) {
    edm::LogWarning("DataNotFound") << "L1T Vertex handle invalid!\n";
    return;
  }
  float zVTX = L1VertexHandle->begin()->z0();

  float recoTkMETx = 0;
  float recoTkMETy = 0;
  float recoTkMET = 0;

  if ( !TTTrackHandle.isValid()) {
    edm::LogWarning("DataNotFound") << "TTTrack handle invalid!\n";
    return;
  }
  int this_l1track = 0;
  std::vector< TTTrack< Ref_Phase2TrackerDigi_ > >::const_iterator iterL1Track;
  for ( auto iterL1Track: *TTTrackHandle) {
    edm::Ptr< TTTrack< Ref_Phase2TrackerDigi_ > > l1track_ptr(TTTrackHandle, this_l1track);
    this_l1track++;

    float tmp_trk_pt   = l1track_ptr->getMomentum(L1Tk_nPar).perp();
    float tmp_trk_eta  = l1track_ptr->getMomentum(L1Tk_nPar).eta();
    float tmp_trk_phi  = l1track_ptr->getMomentum(L1Tk_nPar).phi();
    float tmp_trk_z0   = l1track_ptr->getPOCA(L1Tk_nPar).z(); //cm
    float tmp_trk_bendchi2 = l1track_ptr->getStubPtConsistency(L1Tk_nPar);
    float tmp_trk_deltaz = tmp_trk_z0-zVTX;

    if (fabs(tmp_trk_eta) > TP_maxEta) continue;
    if (tmp_trk_pt<TP_minPt) continue;
    if (fabs(tmp_trk_z0)>L1Tk_maxVtxZ) continue;

    if (tmp_trk_pt>200) tmp_trk_pt = 200; //Saturate high-pt tracks to 200 GeV


    std::vector< edm::Ref< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > >, TTStub< Ref_Phase2TrackerDigi_ > > > stubRefs = l1track_ptr->getStubRefs();
    int tmp_trk_nstub  = (int) stubRefs.size();
    int tmp_trk_nPSstub = 0;
    for (int is=0; is<tmp_trk_nstub; is++) {
      DetId detIdStub = theTrackerGeometry->idToDet( (stubRefs.at(is)->getClusterRef(0))->getDetId() )->geographicalId();
      DetId stackDetid = tTopo->stack(detIdStub);
      if (theTrackerGeometry->getDetectorType(stackDetid)==TrackerGeometry::ModuleType::Ph2PSP) tmp_trk_nPSstub++;
    }
    if (tmp_trk_nstub<L1Tk_minNStub) continue;
    if (tmp_trk_nPSstub<L1Tk_minNPSstub) continue;
    float tmp_trk_chi2dof = (l1track_ptr->getChi2(L1Tk_nPar) / (2*tmp_trk_nstub - L1Tk_nPar));
    if (tmp_trk_chi2dof>L1Tk_maxChi2dof || tmp_trk_bendchi2>L1Tk_maxBendChi2) continue;


    // In an attempt to remove PU, apply a deltaZ cut based on track eta
    float deltaZ_cut = 3.0;
    if ( fabs(tmp_trk_eta)>=0 &&  fabs(tmp_trk_eta)<0.7) deltaZ_cut = 0.4;
    else if ( fabs(tmp_trk_eta)>=0.7 &&  fabs(tmp_trk_eta)<1.0) deltaZ_cut = 0.6;
    else if ( fabs(tmp_trk_eta)>=1.0 &&  fabs(tmp_trk_eta)<1.2) deltaZ_cut = 0.76;
    else if ( fabs(tmp_trk_eta)>=1.2 &&  fabs(tmp_trk_eta)<1.6) deltaZ_cut = 1.0;
    else if ( fabs(tmp_trk_eta)>=1.6 &&  fabs(tmp_trk_eta)<2.0) deltaZ_cut = 1.7;
    else if ( fabs(tmp_trk_eta)>=2.0 &&  fabs(tmp_trk_eta)<=2.4) deltaZ_cut = 2.20;

    if (fabs(tmp_trk_deltaz) > deltaZ_cut) continue;

    recoTkMETx += tmp_trk_pt*cos(tmp_trk_phi);
    recoTkMETy += tmp_trk_pt*sin(tmp_trk_phi);

  } //end track loop
  recoTkMET = sqrt(recoTkMETx*recoTkMETx + recoTkMETy*recoTkMETy);


  //Fill all plots (bin resolution in trueTkMET)
  float METdiff = recoTkMET-trueTkMET;

  if (trueTkMET>=bins[0] && trueTkMET<bins[1]) {
    tkMETDiff_bin1->Fill(METdiff);
  }
  else if (trueTkMET>=bins[1] && trueTkMET<bins[2]) {
    tkMETDiff_bin2->Fill(METdiff);
  }
  else if (trueTkMET>=bins[2] && trueTkMET<bins[3]) {
    tkMETDiff_bin3->Fill(METdiff);
  }
  else if (trueTkMET>=bins[3] && trueTkMET<bins[4]) {
    tkMETDiff_bin4->Fill(METdiff);
  }
  else if (trueTkMET>=bins[4] && trueTkMET<bins[5]) {
    tkMETDiff_bin5->Fill(METdiff);
  }
  else if (trueTkMET>=bins[5] && trueTkMET<bins[6]) {
    tkMETDiff_bin6->Fill(METdiff);
  }
  else if (trueTkMET>=bins[6] && trueTkMET<bins[7]) {
    tkMETDiff_bin7->Fill(METdiff);
  }
  else if (trueTkMET>=bins[7]) {
    tkMETDiff_bin8->Fill(METdiff);
  }


} // end of method

// ------------ method called once each job just before starting event loop  ------------
void OuterTrackerTkMETV::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  // Histogram setup and definitions
  std::string HistoName;
  iBooker.setCurrentFolder(topFolderName_+"/tkMET/trackParticles");

  // 1D: pT
  edm::ParameterSet psTrackParts_Pt =  conf_.getParameter<edm::ParameterSet>("TH1TrackParts_Pt");
  HistoName = "trackParts_Pt";
  trackParts_Pt = iBooker.book1D(HistoName, HistoName, psTrackParts_Pt.getParameter<int32_t>("Nbinsx"), psTrackParts_Pt.getParameter<double>("xmin"),psTrackParts_Pt.getParameter<double>("xmax"));
  trackParts_Pt->setAxisTitle("p_{T} [GeV]", 1);
  trackParts_Pt->setAxisTitle("# tracking particles", 2);

  // 1D: eta
  edm::ParameterSet psTrackParts_Eta =  conf_.getParameter<edm::ParameterSet>("TH1TrackParts_Eta");
  HistoName = "trackParts_Eta";
  trackParts_Eta = iBooker.book1D(HistoName, HistoName,psTrackParts_Eta.getParameter<int32_t>("Nbinsx"),psTrackParts_Eta.getParameter<double>("xmin"),psTrackParts_Eta.getParameter<double>("xmax"));
  trackParts_Eta->setAxisTitle("#eta", 1);
  trackParts_Eta->setAxisTitle("# tracking particles", 2);

  // 1D: NStubs
  edm::ParameterSet psTrackParts_NStubs =  conf_.getParameter<edm::ParameterSet>("TH1_Track_NStubs");
  HistoName = "trackParts_NStubs";
  trackParts_NStubs = iBooker.book1D(HistoName, HistoName,psTrackParts_NStubs.getParameter<int32_t>("Nbinsx"),psTrackParts_NStubs.getParameter<double>("xmin"),psTrackParts_NStubs.getParameter<double>("xmax"));
  trackParts_NStubs->setAxisTitle("# L1 Stubs", 1);
  trackParts_NStubs->setAxisTitle("# tracking particles", 2);

  // 1D: NLayers hit
  HistoName = "trackParts_NLayers";
  trackParts_NLayers = iBooker.book1D(HistoName, HistoName,psTrackParts_NStubs.getParameter<int32_t>("Nbinsx"),psTrackParts_NStubs.getParameter<double>("xmin"),psTrackParts_NStubs.getParameter<double>("xmax"));
  trackParts_NLayers->setAxisTitle("# Layers w stubs", 1);
  trackParts_NLayers->setAxisTitle("# tracking particles", 2);

  // 1D: VtxZ
  edm::ParameterSet psTrackParts_VtxZ =  conf_.getParameter<edm::ParameterSet>("TH1_Track_VertZ");
  HistoName = "trackParts_VtxZ";
  trackParts_VtxZ = iBooker.book1D(HistoName, HistoName,psTrackParts_VtxZ.getParameter<int32_t>("Nbinsx"),psTrackParts_VtxZ.getParameter<double>("xmin"),psTrackParts_VtxZ.getParameter<double>("xmax"));
  trackParts_VtxZ->setAxisTitle("VtxZ [cm]", 1);
  trackParts_VtxZ->setAxisTitle("# tracking particles", 2);

  // 1D: Dxy
  edm::ParameterSet psTrackParts_Dxy =  conf_.getParameter<edm::ParameterSet>("TH1_Track_Dxy");
  HistoName = "trackParts_Dxy";
  trackParts_Dxy = iBooker.book1D(HistoName, HistoName,psTrackParts_Dxy.getParameter<int32_t>("Nbinsx"),psTrackParts_Dxy.getParameter<double>("xmin"),psTrackParts_Dxy.getParameter<double>("xmax"));
  trackParts_Dxy->setAxisTitle("d_{xy} [cm]", 1);
  trackParts_Dxy->setAxisTitle("# tracking particles", 2);

  // 1D plots for tkMET resolution
  iBooker.setCurrentFolder(topFolderName_+"/tkMET/tkMETResolution");

  //pT
  edm::ParameterSet psRes_tkMET =  conf_.getParameter<edm::ParameterSet>("TH1Res_TkMET");
  HistoName = "tkMETDiff_bin1";
  tkMETDiff_bin1 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin1->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin1->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin2";
  tkMETDiff_bin2 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin2->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin2->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin3";
  tkMETDiff_bin3 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin3->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin3->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin4";
  tkMETDiff_bin4 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin4->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin4->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin5";
  tkMETDiff_bin5 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin5->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin5->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin6";
  tkMETDiff_bin6 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin6->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin6->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin7";
  tkMETDiff_bin7 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin7->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin7->setAxisTitle("# Events", 2);

  HistoName = "tkMETDiff_bin8";
  tkMETDiff_bin8 = iBooker.book1D(HistoName, HistoName, psRes_tkMET.getParameter<int32_t>("Nbinsx"), psRes_tkMET.getParameter<double>("xmin"), psRes_tkMET.getParameter<double>("xmax"));
  tkMETDiff_bin8->setAxisTitle("trueTkMET - recoTkMET [GeV]", 1);
  tkMETDiff_bin8->setAxisTitle("# Events", 2);
} //end of method


DEFINE_FWK_MODULE(OuterTrackerTkMETV);
