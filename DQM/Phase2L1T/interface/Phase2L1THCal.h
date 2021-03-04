#ifndef Phase2L1T_Phase2L1THCal_h
#define Phase2L1T_Phase2L1THCal_h

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

//Specifically added from https://github.com/cms-sw/cmssw/blob/a2816f87856da3054918a05646e4bb97067b5520/Validation/HcalDigis/interface/HcalDigisValidation.h
#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"
#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/HcalGeometryRecord.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

class DQMStore;

class Phase2L1THCal : public DQMEDAnalyzer {

public:
  explicit Phase2L1THCal(const edm::ParameterSet&);
  ~Phase2L1THCal() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // DQM plots
  MonitorElement* HCal_tp_et = nullptr;
  MonitorElement* HCal_tp_et_HB = nullptr;
  MonitorElement* HCal_tp_et_HE = nullptr;
  MonitorElement* HCal_tp_et_HF = nullptr;
  MonitorElement* HCal_tp_ntp = nullptr;
  MonitorElement* HCal_tp_ntp_10_ieta = nullptr;
  MonitorElement* HCal_tp_ntp_HB = nullptr;
  MonitorElement* HCal_tp_ntp_HE = nullptr;
  MonitorElement* HCal_tp_ntp_HF = nullptr;
  MonitorElement* HCal_tp_ntp_ieta = nullptr;
  MonitorElement* HCal_tp_ntp_iphi = nullptr;
  MonitorElement* HCal_tp_ieta_et = nullptr;
  MonitorElement* HCal_tp_ieta_iphi = nullptr;

 private:
  edm::ParameterSet conf_;
  std::string topFolderName_;
  edm::InputTag inputTag_;
  edm::InputTag dataTPsTag_;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> tok_dataTPs_;
  edm::ESHandle<CaloGeometry> geometry;
  edm::ESHandle<HcalDbService> conditions;
};
#endif
