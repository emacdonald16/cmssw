#ifndef Phase2L1T_Phase2L1TECal_h
#define Phase2L1T_Phase2L1TECal_h

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

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveSample.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"

class DQMStore;

class Phase2L1TECal : public DQMEDAnalyzer {

public:
  explicit Phase2L1TECal(const edm::ParameterSet&);
  ~Phase2L1TECal() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // ECal DQM plots
  MonitorElement* ECAL_occupancy = nullptr;
  MonitorElement* ECAL_et = nullptr;
  MonitorElement* ECAL_iEta = nullptr;
  MonitorElement* ECAL_iPhi = nullptr;
  MonitorElement* ECAL_iEta_iPhi = nullptr;

  MonitorElement* ECAL_TP_offline_recHitEt = nullptr;
  MonitorElement* ECAL_Ratio_TP_offline_recHitEt = nullptr;

 private:
  edm::ParameterSet conf_;
  std::string topFolderName_;

  edm::EDGetTokenT<EcalRecHitCollection> ECalRecHitToken_;
  edm::EDGetTokenT<EcalEBTrigPrimDigiCollection> ECalTPsToken_;
  // edm::EDGetTokenT<EcalTrigPrimDigiCollection> ECalTPsToken_;

};
#endif
