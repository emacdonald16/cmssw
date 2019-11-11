#include "DQM/Phase2L1T/interface/Phase2L1TGEM.h"

//Currently, there is no phase 2 data format for GEM, or way to access it from RelVals
//I am assuming the EMTF hit collection will update to include isGEM
Phase2L1TGEM::Phase2L1TGEM(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  using namespace std;
  EMTFhitToken_ = consumes<l1t::EMTFHitCollection> (conf_.getParameter<edm::InputTag>("EMTFInputTag") );
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
}

Phase2L1TGEM::~Phase2L1TGEM() {
}

void Phase2L1TGEM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;

  // Assuming GEM hits will be stored
  edm::Handle<l1t::EMTFHitCollection> HitCollection;
  iEvent.getByToken(EMTFhitToken_, HitCollection);

  if (!HitCollection.isValid()) {
    edm::LogWarning("DataNotFound") << "GEM hit collection for Phase2 L1T cannot be found!\n";
    return;
  }

  for (auto Hit = HitCollection->begin(); Hit != HitCollection->end(); ++Hit) {
    float hit_eta = Hit->Eta();
    float hit_phi = Hit->Phi_glob();

    // if (Hit->Is_GEM() == true && Hit->Neighbor() == false) {
    //   GEM_eta->Fill(hit_eta);
    //   GEM_phi->Fill(hit_phi);
    // }

  } //loop over hits
} //end analyze

void Phase2L1TGEM::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;
  edm::ParameterSet ps_eta =  conf_.getParameter<edm::ParameterSet>("tp_eta");
  edm::ParameterSet ps_phi =  conf_.getParameter<edm::ParameterSet>("tp_phi");

  iBooker.setCurrentFolder(topFolderName_+"/GEM");

  HistoName = "GEM_eta";
  GEM_eta = iBooker.book1D(HistoName, HistoName,
      ps_eta.getParameter<int32_t>("Nbinsx"),
      ps_eta.getParameter<double>("xmin"),
      ps_eta.getParameter<double>("xmax"));
  GEM_eta->setAxisTitle("#eta", 1); //x axis
  GEM_eta->setAxisTitle("# GEM hits", 2); //y axis

  HistoName = "GEM_phi";
  GEM_phi = iBooker.book1D(HistoName, HistoName,
      ps_phi.getParameter<int32_t>("Nbinsx"),
      ps_phi.getParameter<double>("xmin"),
      ps_phi.getParameter<double>("xmax"));
  GEM_phi->setAxisTitle("#phi", 1); //x axis
  GEM_phi->setAxisTitle("# GEM hits", 2); //y axis

} //end of method

DEFINE_FWK_MODULE(Phase2L1TGEM);
