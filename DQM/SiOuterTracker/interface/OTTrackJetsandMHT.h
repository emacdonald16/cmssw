#ifndef SiOuterTracker_OTTrackJetsandMHT_h
#define SiOuterTracker_OTTrackJetsandMHT_h

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
#include "DataFormats/L1TVertex/interface/Vertex.h"

#include "DataFormats/L1TrackTrigger/interface/L1TkJetParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkJetParticleFwd.h"



class DQMStore;

class OTTrackJetsandMHT : public DQMEDAnalyzer {

public:
  explicit OTTrackJetsandMHT(const edm::ParameterSet&);
  ~OTTrackJetsandMHT() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  //All track jets
  MonitorElement* allJets_pt = nullptr; // pt of jet
  MonitorElement* allJets_eta = nullptr; // eta of jet
  MonitorElement* allJets_phi = nullptr; // phi of jet
  MonitorElement* allJets_vtx = nullptr; // vtx of jet
  MonitorElement* allJets_nTracks = nullptr; // num of tracks that went into jet
  MonitorElement* allJets_nTightTracks = nullptr; // num of tight tracks that went into jet
  MonitorElement* allJets_nDisplacedTracks = nullptr; // num of displaced tracks that went into jet
  MonitorElement* allJets_nTightDispTracks = nullptr; // num of tight displaced tracks that went into jet


  //Jets used in HT and MHT (pT, eta, and NTracks cuts)
  MonitorElement* HTJets_pt = nullptr; // pt of jet
  MonitorElement* HTJets_eta = nullptr; // eta of jet
  MonitorElement* HTJets_phi = nullptr; // phi of jet
  MonitorElement* HTJets_vtx = nullptr; // vtx of jet
  MonitorElement* HTJets_nTracks = nullptr; // num of tracks that went into jet
  MonitorElement* HTJets_nTightTracks = nullptr; // num of tight tracks that went into jet
  MonitorElement* HTJets_nDisplacedTracks = nullptr; // num of displaced tracks that went into jet
  MonitorElement* HTJets_nTightDispTracks = nullptr; // num of tight displaced tracks that went into jet

  //MHT
  MonitorElement* h_MHT = nullptr; // MHT of event from track jets

  //HT
  MonitorElement* h_HT = nullptr; // HT of event from track jets


 private:
  edm::ParameterSet conf_;

  float jet_minPt;                // [GeV]
  float jet_maxEta;               // [rad]
  //const edm::EDGetTokenT< L1TkPrimaryVertexCollection > pvToken;
  edm::EDGetTokenT< l1t::L1TkJetParticleCollection > jetToken;

  //const edm::EDGetTokenT< std::vector< l1t::L1TkJetParticle > > jetToken;


  unsigned int minNtracksHighPt;
  unsigned int minNtracksLowPt;


  std::string topFolderName_;
};
#endif
