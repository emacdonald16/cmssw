#ifndef Phase2L1T_Phase2L1THGCal_h
#define Phase2L1T_Phase2L1THGCal_h

// system include files
#include <memory>
#include <iostream>
#include <string>
#include <vector>

// user include files
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalCluster.h"
#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"
#include "DataFormats/L1THGCal/interface/HGCalTower.h"
#include "FWCore/Common/interface/Provenance.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "L1Trigger/L1THGCal/interface/backend/HGCalTriggerClusterIdentificationBase.h"

class DQMStore;

class Phase2L1THGCal : public DQMEDAnalyzer {

public:
  explicit Phase2L1THGCal(const edm::ParameterSet &);
  ~Phase2L1THGCal() override;
  void analyze(const edm::Event &, const edm::EventSetup &) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // Trigger cells
  MonitorElement *h_tc_n_ = nullptr;
  MonitorElement *h_tc_mipPt_ = nullptr;
  MonitorElement *h_tc_pt_ = nullptr;
  MonitorElement *h_tc_energy_ = nullptr;
  MonitorElement *h_tc_eta_ = nullptr;
  MonitorElement *h_tc_phi_ = nullptr;
  MonitorElement *h_tc_x_ = nullptr;
  MonitorElement *h_tc_y_ = nullptr;
  MonitorElement *h_tc_z_ = nullptr;
  MonitorElement *h_tc_layer_ = nullptr;

  // Clusters
  MonitorElement *h_cl_n_ = nullptr;
  MonitorElement *h_cl_mipPt_ = nullptr;
  MonitorElement *h_cl_pt_ = nullptr;
  MonitorElement *h_cl_energy_ = nullptr;
  MonitorElement *h_cl_eta_ = nullptr;
  MonitorElement *h_cl_phi_ = nullptr;
  MonitorElement *h_cl_layer_ = nullptr;
  MonitorElement *h_cl_cells_n_ = nullptr;

  // 3D clusters / multiclusters
  MonitorElement *h_cl3d_n_ = nullptr;
  MonitorElement *h_cl3d_pt_ = nullptr;
  MonitorElement *h_cl3d_energy_ = nullptr;
  MonitorElement *h_cl3d_eta_ = nullptr;
  MonitorElement *h_cl3d_phi_ = nullptr;
  MonitorElement *h_cl3d_clusters_n_ = nullptr;
  // 3D cluster shower shapes
  MonitorElement *h_cl3d_showerlength_ = nullptr;
  MonitorElement *h_cl3d_coreshowerlength_ = nullptr;
  MonitorElement *h_cl3d_firstlayer_ = nullptr;
  MonitorElement *h_cl3d_maxlayer_ = nullptr;
  MonitorElement *h_cl3d_seetot_ = nullptr;
  MonitorElement *h_cl3d_seemax_ = nullptr;
  MonitorElement *h_cl3d_spptot_ = nullptr;
  MonitorElement *h_cl3d_sppmax_ = nullptr;
  MonitorElement *h_cl3d_szz_ = nullptr;
  MonitorElement *h_cl3d_srrtot_ = nullptr;
  MonitorElement *h_cl3d_srrmax_ = nullptr;
  MonitorElement *h_cl3d_srrmean_ = nullptr;
  MonitorElement *h_cl3d_emaxe_ = nullptr;
  MonitorElement *h_cl3d_bdteg_ = nullptr;
  MonitorElement *h_cl3d_quality_ = nullptr;

  // Tower
  MonitorElement *h_tower_n_ = nullptr;
  MonitorElement *h_tower_pt_ = nullptr;
  MonitorElement *h_tower_energy_ = nullptr;
  MonitorElement *h_tower_eta_ = nullptr;
  MonitorElement *h_tower_phi_ = nullptr;
  MonitorElement *h_tower_etEm_ = nullptr;
  MonitorElement *h_tower_etHad_ = nullptr;
  MonitorElement *h_tower_iEta_ = nullptr;
  MonitorElement *h_tower_iPhi_ = nullptr;

private:
  edm::ParameterSet conf_;
  std::string topFolderName_;

  edm::EDGetToken trigger_cells_token_;
  edm::EDGetToken clusters_token_;
  edm::EDGetToken multiclusters_token_;
  edm::EDGetToken towers_token_;

  std::unique_ptr<HGCalTriggerClusterIdentificationBase> id_;
  HGCalTriggerTools triggerTools_;
};
#endif
