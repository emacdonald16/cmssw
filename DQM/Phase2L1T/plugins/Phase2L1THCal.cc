#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/Records/interface/HcalRecNumberingRecord.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"

#include "DQM/Phase2L1T/interface/Phase2L1THCal.h"

//Stole from here: https://github.com/cms-sw/cmssw/blob/a2816f87856da3054918a05646e4bb97067b5520/Validation/HcalDigis/src/HcalDigisValidation.cc
Phase2L1THCal::Phase2L1THCal(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  using namespace std;
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
  tok_dataTPs_ = consumes<HcalTrigPrimDigiCollection>( conf_.getParameter<edm::InputTag>("dataTPs") );
}

Phase2L1THCal::~Phase2L1THCal() {
}


void Phase2L1THCal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;


  //TP Code
  ESHandle<CaloTPGTranscoder> decoder;
  iSetup.get<CaloTPGRecord>().get(decoder);

  // Geometry
  ESHandle<HcalTrigTowerGeometry> tp_geometry;
  iSetup.get<CaloGeometryRecord>().get(tp_geometry);

  //Trigger primitives
  edm::Handle<HcalTrigPrimDigiCollection> dataTPs;
  iEvent.getByToken(tok_dataTPs_, dataTPs);

  if (!dataTPs.isValid()) {
    edm::LogWarning("DataNotFound") << "HCal Trigger Primitives for Phase2 L1T cannot be found!\n";
    return;
  }

  int c = 0, chb = 0, che = 0, chf = 0; //counters for plots: all, barrel, EC, forward
  for (HcalTrigPrimDigiCollection::const_iterator itr = dataTPs->begin(); itr != dataTPs->end(); ++itr) {
    int ieta = itr->id().ieta();
    int iphi = itr->id().iphi();

    HcalSubdetector subdet = (HcalSubdetector)0;

    if (abs(ieta) <= 16) subdet = HcalSubdetector::HcalBarrel;
    else if (abs(ieta) < tp_geometry->firstHFTower(itr->id().version())) subdet = HcalSubdetector::HcalEndcap;
    else if (abs(ieta) <= 42) subdet = HcalSubdetector::HcalForward;

    float en = decoder->hcaletValue(itr->id(), itr->t0());

    if (en < 0.00001) continue;

    //Plot the variables
    HCal_tp_et->Fill(en);
    HCal_tp_ntp_ieta->Fill(ieta);
    HCal_tp_ntp_iphi->Fill(iphi);
    HCal_tp_ieta_et->Fill(ieta, en);
    HCal_tp_ieta_iphi->Fill(ieta, iphi);
    if (en > 10.) HCal_tp_ntp_10_ieta->Fill(ieta);
    ++c;
    if (subdet == HcalSubdetector::HcalBarrel) {
      HCal_tp_et_HB->Fill(en);
      ++chb;
    }
    if (subdet == HcalSubdetector::HcalEndcap) {
      HCal_tp_et_HE->Fill(en);
      ++che;
    }
    if (subdet == HcalSubdetector::HcalForward) {
      HCal_tp_et_HF->Fill(en);
      ++chf;
    }

  }  //end data TP collection
  HCal_tp_ntp->Fill(c);
  HCal_tp_ntp_HB->Fill(chb);
  HCal_tp_ntp_HE->Fill(che);
  HCal_tp_ntp_HF->Fill(chf);
} //end analyze

void Phase2L1THCal::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;
  edm::ParameterSet ps_et =  conf_.getParameter<edm::ParameterSet>("tp_hl_et");
  edm::ParameterSet ps_ntp =  conf_.getParameter<edm::ParameterSet>("tp_hl_ntp");
  edm::ParameterSet ps_ntp_sub =  conf_.getParameter<edm::ParameterSet>("tp_hl_ntp_sub");
  edm::ParameterSet ps_ieta =  conf_.getParameter<edm::ParameterSet>("tp_hl_ieta");
  edm::ParameterSet ps_iphi =  conf_.getParameter<edm::ParameterSet>("tp_hl_iphi");
  edm::ParameterSet ps_ieta_et =  conf_.getParameter<edm::ParameterSet>("tp_hl_ieta_et");
  edm::ParameterSet ps_ieta_iphi =  conf_.getParameter<edm::ParameterSet>("tp_hl_ieta_iphi");

  iBooker.setCurrentFolder(topFolderName_+"/HCal");

  HistoName = "HCal_tp_et";
  HCal_tp_et = iBooker.book1D(HistoName, HistoName,
      ps_et.getParameter<int32_t>("Nbinsx"),
      ps_et.getParameter<double>("xmin"),
      ps_et.getParameter<double>("xmax"));
  HCal_tp_et->setAxisTitle("E_{T}", 1); //x axis
  HCal_tp_et->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_et_HB";
  HCal_tp_et_HB = iBooker.book1D(HistoName, HistoName,
      ps_et.getParameter<int32_t>("Nbinsx"),
      ps_et.getParameter<double>("xmin"),
      ps_et.getParameter<double>("xmax"));
  HCal_tp_et_HB->setAxisTitle("E_{T}, HB", 1); //x axis
  HCal_tp_et_HB->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_et_HE";
  HCal_tp_et_HE = iBooker.book1D(HistoName, HistoName,
      ps_et.getParameter<int32_t>("Nbinsx"),
      ps_et.getParameter<double>("xmin"),
      ps_et.getParameter<double>("xmax"));
  HCal_tp_et_HE->setAxisTitle("E_{T}, HE", 1); //x axis
  HCal_tp_et_HE->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_et_HF";
  HCal_tp_et_HF = iBooker.book1D(HistoName, HistoName,
      ps_et.getParameter<int32_t>("Nbinsx"),
      ps_et.getParameter<double>("xmin"),
      ps_et.getParameter<double>("xmax"));
  HCal_tp_et_HF->setAxisTitle("E_{T}, HF", 1); //x axis
  HCal_tp_et_HF->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_ntp";
  HCal_tp_ntp = iBooker.book1D(HistoName, HistoName,
      ps_ntp.getParameter<int32_t>("Nbinsx"),
      ps_ntp.getParameter<double>("xmin"),
      ps_ntp.getParameter<double>("xmax"));
  HCal_tp_ntp->setAxisTitle("# TPs", 1); //x axis
  HCal_tp_ntp->setAxisTitle("# Events", 2); //y axis

  HistoName = "HCal_tp_ntp_HB";
  HCal_tp_ntp_HB = iBooker.book1D(HistoName, HistoName,
      ps_ntp_sub.getParameter<int32_t>("Nbinsx"),
      ps_ntp_sub.getParameter<double>("xmin"),
      ps_ntp_sub.getParameter<double>("xmax"));
  HCal_tp_ntp_HB->setAxisTitle("# TPs, HB", 1); //x axis
  HCal_tp_ntp_HB->setAxisTitle("# Events", 2); //y axis

  HistoName = "HCal_tp_ntp_HE";
  HCal_tp_ntp_HE = iBooker.book1D(HistoName, HistoName,
      ps_ntp_sub.getParameter<int32_t>("Nbinsx"),
      ps_ntp_sub.getParameter<double>("xmin"),
      ps_ntp_sub.getParameter<double>("xmax"));
  HCal_tp_ntp_HE->setAxisTitle("# TPs, HE", 1); //x axis
  HCal_tp_ntp_HE->setAxisTitle("# Events", 2); //y axis

  HistoName = "HCal_tp_ntp_HF";
  HCal_tp_ntp_HF = iBooker.book1D(HistoName, HistoName,
      ps_ntp_sub.getParameter<int32_t>("Nbinsx"),
      ps_ntp_sub.getParameter<double>("xmin"),
      ps_ntp_sub.getParameter<double>("xmax"));
  HCal_tp_ntp_HF->setAxisTitle("# TPs, HF", 1); //x axis
  HCal_tp_ntp_HF->setAxisTitle("# Events", 2); //y axis

  HistoName = "HCal_tp_ntp_ieta";
  HCal_tp_ntp_ieta = iBooker.book1D(HistoName, HistoName,
      ps_ieta.getParameter<int32_t>("Nbinsx"),
      ps_ieta.getParameter<double>("xmin"),
      ps_ieta.getParameter<double>("xmax"));
  HCal_tp_ntp_ieta->setAxisTitle("iEta", 1); //x axis
  HCal_tp_ntp_ieta->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_ntp_iphi";
  HCal_tp_ntp_iphi = iBooker.book1D(HistoName, HistoName,
      ps_iphi.getParameter<int32_t>("Nbinsx"),
      ps_iphi.getParameter<double>("xmin"),
      ps_iphi.getParameter<double>("xmax"));
  HCal_tp_ntp_iphi->setAxisTitle("iPhi", 1); //x axis
  HCal_tp_ntp_iphi->setAxisTitle("# Digis", 2); //y axis

  HistoName = "HCal_tp_ntp_10_ieta";
  HCal_tp_ntp_10_ieta = iBooker.book1D(HistoName, HistoName,
      ps_ieta.getParameter<int32_t>("Nbinsx"),
      ps_ieta.getParameter<double>("xmin"),
      ps_ieta.getParameter<double>("xmax"));
  HCal_tp_ntp_10_ieta->setAxisTitle("iEta", 1); //x axis
  HCal_tp_ntp_10_ieta->setAxisTitle("# Digis, E_{T} > 10 GeV", 2); //y axis

  HistoName = "HCal_tp_ieta_et";
  HCal_tp_ieta_et = iBooker.book2D(HistoName, HistoName,
      ps_ieta_et.getParameter<int32_t>("Nbinsx"),
      ps_ieta_et.getParameter<double>("xmin"),
      ps_ieta_et.getParameter<double>("xmax"),
      ps_ieta_et.getParameter<int32_t>("Nbinsy"),
      ps_ieta_et.getParameter<double>("ymin"),
      ps_ieta_et.getParameter<double>("ymax"));
  HCal_tp_ieta_et->setAxisTitle("iEta", 1);
  HCal_tp_ieta_et->setAxisTitle("E_{T} [GeV]", 2);

  HistoName = "HCal_tp_ieta_iphi";
  HCal_tp_ieta_iphi = iBooker.book2D(HistoName, HistoName,
      ps_ieta_iphi.getParameter<int32_t>("Nbinsx"),
      ps_ieta_iphi.getParameter<double>("xmin"),
      ps_ieta_iphi.getParameter<double>("xmax"),
      ps_ieta_iphi.getParameter<int32_t>("Nbinsy"),
      ps_ieta_iphi.getParameter<double>("ymin"),
      ps_ieta_iphi.getParameter<double>("ymax"));
  HCal_tp_ieta_iphi->setAxisTitle("iEta", 1);
  HCal_tp_ieta_iphi->setAxisTitle("iPhi", 2);

} //end of method

DEFINE_FWK_MODULE(Phase2L1THCal);
