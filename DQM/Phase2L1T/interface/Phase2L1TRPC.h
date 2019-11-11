#ifndef Phase2L1T_Phase2L1TRPC_h
#define Phase2L1T_Phase2L1TRPC_h

#include <vector>
#include <memory>
#include <map>
#include <utility>
#include <ostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>


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

#include "DataFormats/RPCRecHit/interface/RPCRecHit.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

class DQMStore;

class Phase2L1TRPC : public DQMEDAnalyzer {

public:
  explicit Phase2L1TRPC(const edm::ParameterSet&);
  ~Phase2L1TRPC() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // RPC Digis
  MonitorElement* RPCDigi_strip = nullptr;
  MonitorElement* RPCDigi_bx = nullptr;
  MonitorElement* RPCDigi_time = nullptr;

  // MonitorElement* RPCDigi_n = nullptr;
  // MonitorElement* RPCDigi_Barrel_Wheel = nullptr;
  // MonitorElement* RPCDigi_Barrel_Station = nullptr;
  // MonitorElement* RPCDigi_Barrel_Sector = nullptr;
  // MonitorElement* RPCDigi_EC_Ring = nullptr;
  // MonitorElement* RPCDigi_EC_Station = nullptr;
  // MonitorElement* RPCDigi_EC_Sector = nullptr;

  // RPC Rec Hits
  MonitorElement* RPCRecHit_firstClusterStrip = nullptr;
  MonitorElement* RPCRecHit_clusterSize = nullptr;
  MonitorElement* RPCRecHit_bx = nullptr;
  MonitorElement* RPCRecHit_time = nullptr;

  MonitorElement* RPCRecHit_Barrel_Wheel = nullptr;
  MonitorElement* RPCRecHit_Barrel_Station = nullptr;
  MonitorElement* RPCRecHit_Barrel_Sector = nullptr;
  MonitorElement* RPCRecHit_EC_Ring = nullptr;
  MonitorElement* RPCRecHit_EC_Station = nullptr;
  MonitorElement* RPCRecHit_EC_Sector = nullptr;

  // iRPC Digis
  MonitorElement* iRPCDigi_strip = nullptr;
  MonitorElement* iRPCDigi_bx = nullptr;
  MonitorElement* iRPCDigi_time = nullptr;
  MonitorElement* iRPCDigi_y = nullptr;

  // MonitorElement* iRPCDigi_n = nullptr;
  // MonitorElement* iRPCDigi_Barrel_Wheel = nullptr;
  // MonitorElement* iRPCDigi_Barrel_Station = nullptr;
  // MonitorElement* iRPCDigi_Barrel_Sector = nullptr;
  // MonitorElement* iRPCDigi_EC_Ring = nullptr;
  // MonitorElement* iRPCDigi_EC_Station = nullptr;
  // MonitorElement* iRPCDigi_EC_Sector = nullptr;

  MonitorElement* iRPCRecHit_firstClusterStrip = nullptr;
  MonitorElement* iRPCRecHit_clusterSize = nullptr;
  MonitorElement* iRPCRecHit_bx = nullptr;
  MonitorElement* iRPCRecHit_time = nullptr;
  MonitorElement* iRPCRecHit_y = nullptr;



 private:
  edm::ParameterSet conf_;
  std::string topFolderName_;

  edm::EDGetTokenT<RPCDigiCollection> RPCDigiToken_;
  edm::EDGetTokenT<RPCRecHitCollection> RPCRecHitToken_;
};
#endif
