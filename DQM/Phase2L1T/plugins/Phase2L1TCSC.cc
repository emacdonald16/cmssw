#include "DQM/Phase2L1T/interface/Phase2L1TCSC.h"

//Stole from here: https://github.com/cms-sw/cmssw/blob/master/DQM/L1TMonitor/src/L1TStage2EMTF.cc
Phase2L1TCSC::Phase2L1TCSC(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  using namespace std;
  EMTFhitToken_ = consumes<l1t::EMTFHitCollection> (conf_.getParameter<edm::InputTag>("EMTFInputTag") );
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
}

Phase2L1TCSC::~Phase2L1TCSC() {
}

void Phase2L1TCSC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;

  // Hits (CSC LCTs and RPC hits)
  edm::Handle<l1t::EMTFHitCollection> HitCollection;
  iEvent.getByToken(EMTFhitToken_, HitCollection);

  if (!HitCollection.isValid()) {
    edm::LogWarning("DataNotFound") << "EMTF Hit Collection for Phase2 L1T cannot be found!\n";
    return;
  }

  for (auto Hit = HitCollection->begin(); Hit != HitCollection->end(); ++Hit) {
    float hit_eta = Hit->Eta();
    float hit_phi = Hit->Phi_glob();
    int hit_pattern = Hit->Pattern();
    int hit_bx = Hit->BX();
    int hit_bend = Hit->Bend();

    if (Hit->Is_CSC() == true && Hit->Neighbor() == false) {
      CSC_eta->Fill(hit_eta);
      CSC_phi->Fill(hit_phi);
      CSC_pattern->Fill(hit_pattern);
      CSC_bx->Fill(hit_bx);
      CSC_bend->Fill(hit_bend);
    }
  } //loop over hits
} //end analyze

void Phase2L1TCSC::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;
  edm::ParameterSet ps_eta =  conf_.getParameter<edm::ParameterSet>("tp_eta");
  edm::ParameterSet ps_phi =  conf_.getParameter<edm::ParameterSet>("tp_phi");
  edm::ParameterSet ps_pattern =  conf_.getParameter<edm::ParameterSet>("tp_pattern");
  edm::ParameterSet ps_bx =  conf_.getParameter<edm::ParameterSet>("tp_bx");
  edm::ParameterSet ps_bend =  conf_.getParameter<edm::ParameterSet>("tp_bend");

  iBooker.setCurrentFolder(topFolderName_+"/CSC");

  HistoName = "CSC_eta";
  CSC_eta = iBooker.book1D(HistoName, HistoName,
      ps_eta.getParameter<int32_t>("Nbinsx"),
      ps_eta.getParameter<double>("xmin"),
      ps_eta.getParameter<double>("xmax"));
  CSC_eta->setAxisTitle("#eta", 1); //x axis
  CSC_eta->setAxisTitle("# CSC hits", 2); //y axis

  HistoName = "CSC_phi";
  CSC_phi = iBooker.book1D(HistoName, HistoName,
      ps_phi.getParameter<int32_t>("Nbinsx"),
      ps_phi.getParameter<double>("xmin"),
      ps_phi.getParameter<double>("xmax"));
  CSC_phi->setAxisTitle("#phi", 1); //x axis
  CSC_phi->setAxisTitle("# CSC hits", 2); //y axis

  HistoName = "CSC_pattern";
  CSC_pattern = iBooker.book1D(HistoName, HistoName,
      ps_pattern.getParameter<int32_t>("Nbinsx"),
      ps_pattern.getParameter<double>("xmin"),
      ps_pattern.getParameter<double>("xmax"));
  CSC_pattern->setAxisTitle("Pattern", 1); //x axis
  CSC_pattern->setAxisTitle("# CSC hits", 2); //y axis

  HistoName = "CSC_bx";
  CSC_bx = iBooker.book1D(HistoName, HistoName,
      ps_bx.getParameter<int32_t>("Nbinsx"),
      ps_bx.getParameter<double>("xmin"),
      ps_bx.getParameter<double>("xmax"));
  CSC_bx->setAxisTitle("bx", 1); //x axis
  CSC_bx->setAxisTitle("# CSC hits", 2); //y axis

  HistoName = "CSC_bend";
  CSC_bend = iBooker.book1D(HistoName, HistoName,
      ps_bend.getParameter<int32_t>("Nbinsx"),
      ps_bend.getParameter<double>("xmin"),
      ps_bend.getParameter<double>("xmax"));
  CSC_bend->setAxisTitle("bend", 1); //x axis
  CSC_bend->setAxisTitle("# CSC hits", 2); //y axis
} //end of method

DEFINE_FWK_MODULE(Phase2L1TCSC);
