#ifndef Phase2L1T_Phase2L1TDT_h
#define Phase2L1T_Phase2L1TDT_h

#include <vector>
#include <memory>
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

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"

class DQMStore;

class Phase2L1TDT : public DQMEDAnalyzer {

public:
  explicit Phase2L1TDT(const edm::ParameterSet&);
  ~Phase2L1TDT() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // DT elements
  MonitorElement* DT_nTPs = nullptr;
  MonitorElement* DT_wheel = nullptr;
  MonitorElement* DT_station = nullptr;
  MonitorElement* DT_sector = nullptr;
  MonitorElement* DT_bx = nullptr;
  MonitorElement* DT_t0 = nullptr;
  MonitorElement* DT_quality = nullptr;


 private:
  edm::ParameterSet conf_;
  edm::EDGetTokenT< L1Phase2MuDTPhContainer >  DTtpsToken_;
  std::string topFolderName_;
};
#endif
