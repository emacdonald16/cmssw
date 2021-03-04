#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "L1Trigger/L1THGCal/interface/backend/HGCalTriggerClusterIdentificationBase.h"

#include "DQM/Phase2L1T/interface/Phase2L1THGCal.h"

Phase2L1THGCal::Phase2L1THGCal(const edm::ParameterSet &iConfig)
  : trigger_cells_token_{consumes<l1t::HGCalTriggerCellBxCollection>(iConfig.getParameter<edm::InputTag>("TriggerCells"))},
    clusters_token_{consumes<l1t::HGCalClusterBxCollection>(iConfig.getParameter<edm::InputTag>("Clusters"))},
    multiclusters_token_{consumes<l1t::HGCalMulticlusterBxCollection>(iConfig.getParameter<edm::InputTag>("Multiclusters"))},
    towers_token_{consumes<l1t::HGCalTowerBxCollection>(iConfig.getParameter<edm::InputTag>("Towers"))},
    id_{HGCalTriggerClusterIdentificationFactory::get()->create("HGCalTriggerClusterIdentificationBDT")} {
      id_->initialize(iConfig.getParameter<edm::ParameterSet>("EGIdentification"));
      topFolderName_ = iConfig.getParameter<std::string>("TopFolderName");
    }

Phase2L1THGCal::~Phase2L1THGCal() {}

void Phase2L1THGCal::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
  int tc_n = 0; // number of trigger cells
  int cl_n = 0; // number of clusters
  int cl3d_n = 0; // number of 3D clusters
  int tower_n = 0; // number of towers

  triggerTools_.eventSetup(iSetup);

  // Trigger cells
  edm::Handle<l1t::HGCalTriggerCellBxCollection> trigger_cells_h;
  iEvent.getByToken(trigger_cells_token_, trigger_cells_h);
  const l1t::HGCalTriggerCellBxCollection &trigger_cells = *trigger_cells_h;

  if (trigger_cells_h.isValid()) {
    for (auto tc_itr = trigger_cells.begin(0); tc_itr != trigger_cells.end(0); tc_itr++) {
      tc_n++;
      HGCalDetId id(tc_itr->detId());
      h_tc_pt_->Fill(tc_itr->pt());
      h_tc_mipPt_->Fill(tc_itr->mipPt());
      h_tc_energy_->Fill(tc_itr->energy());
      h_tc_eta_->Fill(tc_itr->eta());
      h_tc_phi_->Fill(tc_itr->phi());
      h_tc_x_->Fill(tc_itr->position().x());
      h_tc_y_->Fill(tc_itr->position().y());
      h_tc_z_->Fill(tc_itr->position().z());
      h_tc_layer_->Fill(triggerTools_.layerWithOffset(id));
    }
  }
  else {
    edm::LogWarning("DataNotFound") << "HGCal Trigger Cells for Phase2 L1T cannot be found!\n";
  }
  h_tc_n_->Fill(tc_n);

  // Clusters
  edm::Handle<l1t::HGCalClusterBxCollection> clusters_h;
  iEvent.getByToken(clusters_token_, clusters_h);
  const l1t::HGCalClusterBxCollection &clusters = *clusters_h;

  if (clusters_h.isValid()) {
    for (auto cl_itr = clusters.begin(0); cl_itr != clusters.end(0); cl_itr++) {
      cl_n++;
      h_cl_mipPt_->Fill(cl_itr->mipPt());
      h_cl_pt_->Fill(cl_itr->pt());
      h_cl_energy_->Fill(cl_itr->energy());
      h_cl_eta_->Fill(cl_itr->eta());
      h_cl_phi_->Fill(cl_itr->phi());
      h_cl_layer_->Fill(triggerTools_.layerWithOffset(cl_itr->detId()));
      h_cl_cells_n_->Fill(cl_itr->constituents().size());
    }
  }
  else {
    edm::LogWarning("DataNotFound") << "HGCal 2D Clusters collection for Phase2 L1T cannot be found!\n";
  }
  h_cl_n_->Fill(cl_n);

  // 3D clusters
  edm::Handle<l1t::HGCalMulticlusterBxCollection> multiclusters_h;
  iEvent.getByToken(multiclusters_token_, multiclusters_h);
  const l1t::HGCalMulticlusterBxCollection &multiclusters = *multiclusters_h;

  if (multiclusters_h.isValid()) {
    for (auto cl3d_itr = multiclusters.begin(0); cl3d_itr != multiclusters.end(0); cl3d_itr++) {
      cl3d_n++;
      h_cl3d_pt_->Fill(cl3d_itr->pt());
      h_cl3d_energy_->Fill(cl3d_itr->energy());
      h_cl3d_eta_->Fill(cl3d_itr->eta());
      h_cl3d_phi_->Fill(cl3d_itr->phi());
      h_cl3d_clusters_n_->Fill(cl3d_itr->constituents().size());
      // cluster shower shapes
      h_cl3d_showerlength_->Fill(cl3d_itr->showerLength());
      h_cl3d_coreshowerlength_->Fill(cl3d_itr->coreShowerLength());
      h_cl3d_firstlayer_->Fill(cl3d_itr->firstLayer());
      h_cl3d_maxlayer_->Fill(cl3d_itr->maxLayer());
      h_cl3d_seetot_->Fill(cl3d_itr->sigmaEtaEtaTot());
      h_cl3d_seemax_->Fill(cl3d_itr->sigmaEtaEtaMax());
      h_cl3d_spptot_->Fill(cl3d_itr->sigmaPhiPhiTot());
      h_cl3d_sppmax_->Fill(cl3d_itr->sigmaPhiPhiMax());
      h_cl3d_szz_->Fill(cl3d_itr->sigmaZZ());
      h_cl3d_srrtot_->Fill(cl3d_itr->sigmaRRTot());
      h_cl3d_srrmax_->Fill(cl3d_itr->sigmaRRMax());
      h_cl3d_srrmean_->Fill(cl3d_itr->sigmaRRMean());
      h_cl3d_emaxe_->Fill(cl3d_itr->eMax() / cl3d_itr->energy());
      h_cl3d_bdteg_->Fill(id_->value(*cl3d_itr));
      h_cl3d_quality_->Fill(cl3d_itr->hwQual());
    }
  }
  else {
    edm::LogWarning("DataNotFound") << "HGCal 3D Clusters collection for Phase2 L1T cannot be found!\n";
  }
  h_cl3d_n_->Fill(cl3d_n);

  // Towers
  edm::Handle<l1t::HGCalTowerBxCollection> towers_h;
  iEvent.getByToken(towers_token_, towers_h);
  const l1t::HGCalTowerBxCollection &towers = *towers_h;

  if (towers_h.isValid()) {
    for (auto tower_itr = towers.begin(0); tower_itr != towers.end(0); tower_itr++) {
      tower_n++;
      h_tower_pt_->Fill(tower_itr->pt());
      h_tower_energy_->Fill(tower_itr->energy());
      h_tower_eta_->Fill(tower_itr->eta());
      h_tower_phi_->Fill(tower_itr->phi());
      h_tower_etEm_->Fill(tower_itr->etEm());
      h_tower_etHad_->Fill(tower_itr->etHad());
      h_tower_iEta_->Fill(tower_itr->id().iEta());
      h_tower_iPhi_->Fill(tower_itr->id().iPhi());
    }
  }
  else {
    edm::LogWarning("DataNotFound") << "HGCal Towers collection for Phase2 L1T cannot be found!\n";
  }
  h_tower_n_->Fill(tower_n);
}

void Phase2L1THGCal::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const &, edm::EventSetup const &) {

  // trigger cells
  iBooker.setCurrentFolder(topFolderName_+"/HGCAL/TriggerCells");
  h_tc_n_ = iBooker.book1D("tc_n", ";Trigger cell number;# Trigger cells", 400, 0, 400);
  h_tc_mipPt_ = iBooker.book1D("tc_mipPt", ";Trigger cell mipPt;# Trigger cells", 400, 0, 400);
  h_tc_pt_ = iBooker.book1D("tc_pt", ";Trigger cell p_{T} [GeV];# Trigger cells", 15, 0, 15);
  h_tc_energy_ = iBooker.book1D("tc_energy", ";Trigger cell energy [GeV];# Trigger cells", 70, 0, 70);
  h_tc_eta_ = iBooker.book1D("tc_eta", ";Trigger cell #eta;# Trigger cells", 60, -3.14, 3.14);
  h_tc_phi_ = iBooker.book1D("tc_phi", ";Trigger cell #phi;# Trigger cells", 60, -3.14, 3.14);
  h_tc_x_ = iBooker.book1D("tc_x", ";Trigger cell x [cm];# Trigger cells", 500, -250, 250);
  h_tc_y_ = iBooker.book1D("tc_y", ";Trigger cell y [cm];# Trigger cells", 500, -250, 250);
  h_tc_z_ = iBooker.book1D("tc_z", ";Trigger cell z [cm];# Trigger cells", 1100, -550, 550);
  h_tc_layer_ = iBooker.book1D("tc_layer", ";Trigger cell layer", 50, 0, 50);

  // cluster 2D histograms
  iBooker.setCurrentFolder(topFolderName_+"/HGCAL/2DClusters");
  h_cl_n_ = iBooker.book1D("cl_n", "; 2D cluster number;# 2D clusters", 80, 0, 80);
  h_cl_mipPt_ = iBooker.book1D("cl_mipPt", "; 2D cluster mipPt;# 2D clusters", 600, 0, 600);
  h_cl_pt_ = iBooker.book1D("cl_pt", "; 2D cluster p_{T} [GeV];# 2D clusters", 20, 0, 20);
  h_cl_energy_ = iBooker.book1D("cl_energy", "; 2D cluster energy [GeV];# 2D clusters", 80, 0, 80);
  h_cl_eta_ = iBooker.book1D("cl_eta", "; 2D cluster #eta;# 2D clusters", 60, -3.14, 3.14);
  h_cl_phi_ = iBooker.book1D("cl_phi", "; 2D cluster #phi;# 2D clusters", 60, -3.14, 3.14);
  h_cl_cells_n_ = iBooker.book1D("cl_cells_n", "; 2D cluster cells_n;# 2D clusters", 16, 0, 16);
  h_cl_layer_ = iBooker.book1D("cl_layer", "; 2D cluster layer;# 2D clusters", 50, 0, 50);

  // multiclusters
  iBooker.setCurrentFolder(topFolderName_+"/HGCAL/3DClusters");
  h_cl3d_n_ = iBooker.book1D("cl3d_n", "; 3D cluster number;# 3D clusters", 12, 0, 12);
  h_cl3d_pt_ = iBooker.book1D("cl3d_pt", "; 3D cluster p_{T} [GeV];# 3D clusters", 50, 0, 50);
  h_cl3d_energy_ = iBooker.book1D("cl3d_energy", "; 3D cluster energy [GeV];# 3D clusters", 80, 0, 80);
  h_cl3d_eta_ = iBooker.book1D("cl3d_eta", "; 3D cluster #eta;# 3D clusters", 60, -3.14, 3.14);
  h_cl3d_phi_ = iBooker.book1D("cl3d_phi", "; 3D cluster #phi;# 3D clusters", 60, -3.14, 3.14);
  h_cl3d_clusters_n_ = iBooker.book1D("cl3d_clusters_n", "; 3D cluster clusters_n;# 3D clusters", 30, 0, 30);

  // cluster shower shapes
  iBooker.setCurrentFolder(topFolderName_+"/HGCAL/3DClusters/ShowerShape");
  h_cl3d_showerlength_ = iBooker.book1D("cl3d_showerlength", "; 3D cluster showerlength;# 3D clusters", 50, 0, 50);
  h_cl3d_coreshowerlength_ = iBooker.book1D("cl3d_coreshowerlength", "; 3D cluster coreshowerlength;# 3D clusters", 16, 0, 16);
  h_cl3d_firstlayer_ = iBooker.book1D("cl3d_firstlayer", "; 3D cluster firstlayer;# 3D clusters", 50, 0, 50);
  h_cl3d_maxlayer_ = iBooker.book1D("cl3d_maxlayer", "; 3D cluster maxlayer;# 3D clusters", 50, 0, 50);
  h_cl3d_seetot_ = iBooker.book1D("cl3d_seetot", "; 3D cluster seetot;# 3D clusters", 50, 0, 0.05);
  h_cl3d_seemax_ = iBooker.book1D("cl3d_seemax", "; 3D cluster seemax;# 3D clusters", 40, 0, 0.04);
  h_cl3d_spptot_ = iBooker.book1D("cl3d_spptot", "; 3D cluster spptot;# 3D clusters", 800, 0, 0.08);
  h_cl3d_sppmax_ = iBooker.book1D("cl3d_sppmax", "; 3D cluster sppmax;# 3D clusters", 800, 0, 0.08);
  h_cl3d_szz_ = iBooker.book1D("cl3d_szz", "; 3D cluster szz;# 3D clusters", 50, 0, 50);
  h_cl3d_srrtot_ = iBooker.book1D("cl3d_srrtot", "; 3D cluster srrtot;# 3D clusters", 800, 0, 0.008);
  h_cl3d_srrmax_ = iBooker.book1D("cl3d_srrmax", "; 3D cluster srrmax;# 3D clusters", 900, 0, 0.009);
  h_cl3d_srrmean_ = iBooker.book1D("cl3d_srrmean", "; 3D cluster srrmean;# 3D clusters", 800, 0, 0.008);
  h_cl3d_emaxe_ = iBooker.book1D("cl3d_emaxe", "; 3D cluster emaxe;# 3D clusters", 15, 0, 1.5);
  h_cl3d_bdteg_ = iBooker.book1D("cl3d_bdteg", "; 3D cluster bdteg;# 3D clusters", 30, -0.7, 0.4);
  h_cl3d_quality_ = iBooker.book1D("cl3d_quality", "; 3D cluster quality;# 3D clusters", 20, 0, 2);

  // towers
  iBooker.setCurrentFolder(topFolderName_+"/HGCAL/Towers");
  h_tower_n_ = iBooker.book1D("tower_n", "; Tower Number; # towers", 400, 1200, 1600);
  h_tower_pt_ = iBooker.book1D("tower_pt", ";Tower p_{T} [GeV]; # towers", 50, 0, 50);
  h_tower_energy_ = iBooker.book1D("tower_energy", ";Tower energy[GeV]; # towers", 200, 0, 200);
  h_tower_eta_ = iBooker.book1D("tower_eta", ";Tower #eta; # towers", 60, -3.14, 3.14);
  h_tower_phi_ = iBooker.book1D("tower_phi", ";Tower #phi; # towers", 60, -3.14, 3.14);
  h_tower_etEm_ = iBooker.book1D("tower_etEm", ";Tower etEm; # towers", 50, 0, 50);
  h_tower_etHad_ = iBooker.book1D("tower_etHad", ";Tower etHad; # towers", 30, 0, 0.3);
  h_tower_iEta_ = iBooker.book1D("tower_iEta", ";Tower iEta; # towers", 20, 0, 20);
  h_tower_iPhi_ = iBooker.book1D("tower_iPhi", ";Tower iPhi; # towers", 80, 0, 80);
}

DEFINE_FWK_MODULE(Phase2L1THGCal);
