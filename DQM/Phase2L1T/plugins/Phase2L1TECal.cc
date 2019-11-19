
#include "DQM/EcalMonitorTasks/interface/TimingTask.h"
#include "DQM/EcalCommon/interface/EcalDQMCommonUtils.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlock.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DQM/Phase2L1T/interface/Phase2L1TECal.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"   //     ----> CLHEP/Geometry/Point3D.h issue
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"

//List of all ECAL collection tags: https://github.com/cms-sw/cmssw/blob/b13393b0be265d88391e2ceaf4de9324c784a223/DQM/EcalMonitorTasks/python/CollectionTags_cfi.py

Phase2L1TECal::Phase2L1TECal(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  using namespace std;
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
  // ECalTPsToken_ = consumes <EcalTrigPrimDigiCollection> (conf_.getParameter<edm::InputTag>("ECalTPs") );
  ECalTPsToken_ = consumes <EcalEBTrigPrimDigiCollection> (conf_.getParameter<edm::InputTag>("ECalTPs") );
  ECalRecHitToken_ = consumes <EcalRecHitCollection> (conf_.getParameter<edm::InputTag>("ECalRecHit") );
}

Phase2L1TECal::~Phase2L1TECal() {
}


void Phase2L1TECal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;


  // edm::Handle<EcalTrigPrimDigiCollection> ecalTPs;
  edm::Handle<EcalEBTrigPrimDigiCollection> ecalTPs;
  iEvent.getByToken(ECalTPsToken_, ecalTPs);

  edm::Handle<EcalRecHitCollection> ecalRecHit;
  iEvent.getByToken(ECalRecHitToken_, ecalRecHit);

  // calo geometry
  edm::ESHandle<CaloGeometry> pGeometry;
  iSetup.get<CaloGeometryRecord>().get(pGeometry);
  const CaloGeometry *geometry = pGeometry.product();


  std::array<int, 16> bxBinEdges_;
  int* pBin = std::upper_bound(bxBinEdges_.begin(), bxBinEdges_.end(), iEvent.bunchCrossing());
  double bxBin_ = static_cast<int>(pBin - bxBinEdges_.begin()) - 0.5;

  int nTP = 0;

  //https://github.com/cms-sw/cmssw/blob/master/DataFormats/EcalDetId/interface/EBDetId.h //This is for things to access with the detId for something in the barrel
  //Includes tower info and crystal info (not sure if this will change with upgrade)

  float tp_et=0.0; //This is encoded
  float rh_et=0.0; //RecHit Et, in GeV

  if (!ecalTPs.isValid()) {
    edm::LogWarning("DataNotFound") << "Ecal EB Trigger Primitives for Phase2 L1T cannot be found!\n";
    return;
  }

  if (!ecalRecHit.isValid()) {
    edm::LogWarning("DataNotFound") << "Ecal RecHits for Phase2 L1T cannot be found!\n";
  }

  // for (EcalTrigPrimDigiCollection::const_iterator tpItr = ecalTPs->begin(); tpItr != ecalTPs->end(); ++tpItr) {
  for (EcalEBTrigPrimDigiCollection::const_iterator tpItr = ecalTPs->begin(); tpItr != ecalTPs->end(); ++tpItr) {

    EBDetId ttid(tpItr->id());

    int ieta = ttid.ieta();
    int iphi = ttid.iphi();
    ECAL_iEta->Fill(ieta);
    ECAL_iPhi->Fill(iphi);
    ECAL_iEta_iPhi->Fill(iphi, ieta);


    tp_et = tpItr->encodedEt(); // Need to multiply the TP ADC values
    //by a scaling factor of 0.125 to get the Et in GeV
    ECAL_et->Fill(tp_et);

    if (tp_et > 0.) nTP++;

    if (!ecalRecHit.isValid()) continue;

    for (EcalRecHitCollection::const_iterator rhItr = ecalRecHit->begin(); rhItr != ecalRecHit->end(); ++rhItr) {
      EBDetId recHitid(rhItr->id());
      DetId rhDetID(rhItr->id());

      int rh_ieta = recHitid.ieta();
      int rh_iphi = recHitid.iphi();
      float dphi = abs(rh_iphi-iphi);
      float deta = abs(rh_ieta-ieta);

      if (dphi<0.005 && deta<0.005) {
        rh_et =  rhItr->energy();
        GlobalPoint rechitcell = geometry -> getPosition( rhDetID );
        rh_et=rh_et* rechitcell.perp()/rechitcell.mag();

        if (rh_et>0 && tp_et>0){
          tp_et*=0.125;
          ECAL_TP_offline_recHitEt->Fill(rh_et, tp_et);
          if (tp_et>0.5) ECAL_Ratio_TP_offline_recHitEt->Fill(tp_et/rh_et);
        }
        break; //if found matching rec hit, then no longer care about this tp, so moving on
      }
    } //end loop over RecHits


  }
  ECAL_occupancy->Fill(nTP);

} //end analyze

void Phase2L1TECal::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;
  edm::ParameterSet ps_et =  conf_.getParameter<edm::ParameterSet>("tp_Et");
  edm::ParameterSet ps_timingvbx =  conf_.getParameter<edm::ParameterSet>("tp_TimingvsBx");
  edm::ParameterSet ps_occupvbx =  conf_.getParameter<edm::ParameterSet>("tp_OccupancyvsBx");
  edm::ParameterSet ps_ieta =  conf_.getParameter<edm::ParameterSet>("tp_iEta");
  edm::ParameterSet ps_iphi =  conf_.getParameter<edm::ParameterSet>("tp_iPhi");
  edm::ParameterSet ps_ieta_iphi =  conf_.getParameter<edm::ParameterSet>("tp_iEta_iPhi");
  edm::ParameterSet ps_et_ratio =  conf_.getParameter<edm::ParameterSet>("tp_EtRatio");
  edm::ParameterSet ps_et_tpVrh =  conf_.getParameter<edm::ParameterSet>("tp_Et_TPvRecHit");


  iBooker.setCurrentFolder(topFolderName_+"/ECAL");



  HistoName = "ECAL_occupancy";
  ECAL_occupancy = iBooker.book2D(HistoName, HistoName,
    ps_occup.getParameter<int32_t>("Nbinsx"),
    ps_occup.getParameter<double>("xmin"),
    ps_occup.getParameter<double>("xmax"),
    ps_occup.getParameter<int32_t>("Nbinsy"),
    ps_occup.getParameter<double>("ymin"),
    ps_occup.getParameter<double>("ymax")
  );
  ECAL_occupancy->setAxisTitle("# ECal TPs", 1); //x axis
  ECAL_occupancy->setAxisTitle("Events", 2); //y axis

  HistoName = "ECAL_et";
  ECAL_et = iBooker.book1D(HistoName, HistoName,
    ps_et.getParameter<int32_t>("Nbinsx"),
    ps_et.getParameter<double>("xmin"),
    ps_et.getParameter<double>("xmax")
  );
  ECAL_et->setAxisTitle("Et", 1); //x axis
  ECAL_et->setAxisTitle("# ECal TPs", 2); //y axis

  HistoName = "ECAL_iEta";
  ECAL_iEta = iBooker.book1D(HistoName, HistoName,
    ps_ieta.getParameter<int32_t>("Nbinsx"),
    ps_ieta.getParameter<double>("xmin"),
    ps_ieta.getParameter<double>("xmax")
  );
  ECAL_iEta->setAxisTitle("iEta", 1); //x axis
  ECAL_iEta->setAxisTitle("# ECal TPs", 2); //y axis

  HistoName = "ECAL_iPhi";
  ECAL_iPhi = iBooker.book1D(HistoName, HistoName,
    ps_iphi.getParameter<int32_t>("Nbinsx"),
    ps_iphi.getParameter<double>("xmin"),
    ps_iphi.getParameter<double>("xmax")
  );
  ECAL_iPhi->setAxisTitle("iPhi", 1); //x axis
  ECAL_iPhi->setAxisTitle("# ECal TPs", 2); //y axis

  HistoName = "ECAL_iEta_iPhi";
  ECAL_iEta_iPhi = iBooker.book2D(HistoName, HistoName,
    ps_ieta_iphi.getParameter<int32_t>("Nbinsx"),
    ps_ieta_iphi.getParameter<double>("xmin"),
    ps_ieta_iphi.getParameter<double>("xmax"),
    ps_ieta_iphi.getParameter<int32_t>("Nbinsy"),
    ps_ieta_iphi.getParameter<double>("ymin"),
    ps_ieta_iphi.getParameter<double>("ymax")
  );
  ECAL_iEta_iPhi->setAxisTitle("iPhi", 1); //x axis
  ECAL_iEta_iPhi->setAxisTitle("iEta", 2); //y axis

  HistoName = "ECAL_Ratio_TP_offline_recHitEt";
  ECAL_Ratio_TP_offline_recHitEt = iBooker.book1D(HistoName, HistoName,
    ps_et_ratio.getParameter<int32_t>("Nbinsx"),
    ps_et_ratio.getParameter<double>("xmin"),
    ps_et_ratio.getParameter<double>("xmax")
  );
  ECAL_Ratio_TP_offline_recHitEt->setAxisTitle("Et TP/offline", 1); //x axis
  ECAL_Ratio_TP_offline_recHitEt->setAxisTitle("# ECal TPs", 2); //y axis

  HistoName = "ECAL_TP_offline_recHitEt";
  ECAL_TP_offline_recHitEt = iBooker.book2D(HistoName, HistoName,
    ps_et_tpVrh.getParameter<int32_t>("Nbinsx"),
    ps_et_tpVrh.getParameter<double>("xmin"),
    ps_et_tpVrh.getParameter<double>("xmax"),
    ps_et_tpVrh.getParameter<int32_t>("Nbinsy"),
    ps_et_tpVrh.getParameter<double>("ymin"),
    ps_et_tpVrh.getParameter<double>("ymax")
  );
  ECAL_TP_offline_recHitEt->setAxisTitle("RecHit Et", 1); //x axis
  ECAL_TP_offline_recHitEt->setAxisTitle("TP Et", 2); //y axis

} //end of method

DEFINE_FWK_MODULE(Phase2L1TECal);
