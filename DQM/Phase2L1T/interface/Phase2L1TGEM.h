#ifndef Phase2L1T_Phase2L1TGEM_h
#define Phase2L1T_Phase2L1TGEM_h

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

#include "DataFormats/L1TMuon/interface/EMTFDaqOut.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"


class DQMStore;

class Phase2L1TGEM : public DQMEDAnalyzer {

public:
  explicit Phase2L1TGEM(const edm::ParameterSet&);
  ~Phase2L1TGEM() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // GEMs
  MonitorElement* GEM_eta = nullptr;
  MonitorElement* GEM_phi = nullptr;
  // MonitorElement* GEM_pattern = nullptr;
  // MonitorElement* GEM_bx = nullptr;
  // MonitorElement* GEM_bend = nullptr;


 private:
  edm::ParameterSet conf_;
  edm::EDGetTokenT<l1t::EMTFHitCollection> EMTFhitToken_;
  std::string topFolderName_;
};
#endif
