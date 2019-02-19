#ifndef SiOuterTrackerV_OuterTrackerTkMETV_h
#define SiOuterTrackerV_OuterTrackerTkMETV_h

#include <vector>
#include <memory>
#include <string>
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/L1TVertex/interface/Vertex.h"



class DQMStore;

class OuterTrackerTkMETV : public DQMEDAnalyzer {

public:
  explicit OuterTrackerTkMETV(const edm::ParameterSet&);
  //~OuterTrackerTkMETV() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;


  // Tracking particle distributions
  MonitorElement* trackParts_Eta = nullptr;
  MonitorElement* trackParts_Pt = nullptr;
  MonitorElement* trackParts_NStubs = nullptr;
  MonitorElement* trackParts_NLayers = nullptr;
  MonitorElement* trackParts_VtxZ = nullptr;
  MonitorElement* trackParts_Dxy = nullptr;


  // 1D intermediate trkMET resolution plots
  MonitorElement* tkMETDiff_bin1 = nullptr;
  MonitorElement* tkMETDiff_bin2 = nullptr;
  MonitorElement* tkMETDiff_bin3 = nullptr;
  MonitorElement* tkMETDiff_bin4 = nullptr;
  MonitorElement* tkMETDiff_bin5 = nullptr;
  MonitorElement* tkMETDiff_bin6 = nullptr;
  MonitorElement* tkMETDiff_bin7 = nullptr;
  MonitorElement* tkMETDiff_bin8 = nullptr;



 private:
   edm::ParameterSet conf_;
   edm::EDGetTokenT< std::vector <TrackingParticle> > trackingParticleToken_;
   edm::EDGetTokenT< edmNew::DetSetVector < TTStub < Ref_Phase2TrackerDigi_ > > > ttStubToken_;
   edm::EDGetTokenT<std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > >  ttTrackToken_;
   edm::EDGetTokenT< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > > ttClusterMCTruthToken_;//MC truth association map for clusters
   edm::EDGetTokenT< TTStubAssociationMap < Ref_Phase2TrackerDigi_ > > ttStubMCTruthToken_; //MC truth association map for stubs
   edm::EDGetTokenT< TTTrackAssociationMap< Ref_Phase2TrackerDigi_ > > ttTrackMCTruthToken_;//MC truth association map for tracks
   edm::EDGetTokenT< l1t::VertexCollection > pvToken;

   int L1Tk_nPar;
   double L1Tk_minPt;
   double L1Tk_maxPt;
   double L1Tk_maxEta;
   int L1Tk_minNStub;
   int L1Tk_minNPSstub;
   double L1Tk_maxVtxZ;
   double L1Tk_maxChi2dof;
   double L1Tk_maxBendChi2;

   double TP_minPt;
   double TP_maxEta;
   double TP_maxVtxZ;
   double TP_maxDxy;
   int TP_minNStub;
   int TP_minNLayers;
   std::string topFolderName_;

};
#endif
