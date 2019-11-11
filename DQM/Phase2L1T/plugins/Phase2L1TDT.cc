// system include files
#include <memory>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"
#include "DQM/Phase2L1T/interface/Phase2L1TDT.h"

Phase2L1TDT::Phase2L1TDT(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
  DTtpsToken_ = consumes< L1Phase2MuDTPhContainer > (conf_.getParameter<edm::InputTag>("DTDigis") );
}

Phase2L1TDT::~Phase2L1TDT()
{
}

void Phase2L1TDT::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  /// Track Trigger Clusters
  edm::Handle< L1Phase2MuDTPhContainer > Phase2DTDigiHandleHandle;
  iEvent.getByToken( DTtpsToken_, Phase2DTDigiHandleHandle );

  // Adding protection
  if ( !Phase2DTDigiHandleHandle.isValid() ) {
    edm::LogWarning("DataNotFound") << "DT Digis for Phase2 L1T cannot be found!\n";
    return;
  }

  int digi_nTPs = 0;

    L1Phase2MuDTPhContainer::Segment_Container * thisContainer =  (L1Phase2MuDTPhContainer::Segment_Container*)Phase2DTDigiHandleHandle->getContainer();

    for (auto Digi = thisContainer->begin(); Digi != thisContainer->end(); ++Digi) {
      digi_nTPs++;
      float digi_wheel = Digi->whNum();
      float digi_station = Digi->stNum();
      float digi_sector = Digi->scNum();
      float digi_bx = Digi->bxNum();
      float digi_t0 = Digi->t0();
      float digi_quality = Digi->quality();

      DT_wheel->Fill(digi_wheel);
      DT_station->Fill(digi_station);
      DT_sector->Fill(digi_sector);
      DT_bx->Fill(digi_bx);
      DT_t0->Fill(digi_t0);
      DT_quality->Fill(digi_quality);
    } //loop over digis
  DT_nTPs->Fill(digi_nTPs);


} // end of analyze

void Phase2L1TDT::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;

  edm::ParameterSet ps_wheel =  conf_.getParameter<edm::ParameterSet>("dt_tp_wheel");
  edm::ParameterSet ps_station =  conf_.getParameter<edm::ParameterSet>("dt_tp_station");
  edm::ParameterSet ps_sector =  conf_.getParameter<edm::ParameterSet>("dt_tp_sector");
  edm::ParameterSet ps_bx =  conf_.getParameter<edm::ParameterSet>("dt_tp_bx");
  edm::ParameterSet ps_t0 =  conf_.getParameter<edm::ParameterSet>("dt_tp_t0");
  edm::ParameterSet ps_quality =  conf_.getParameter<edm::ParameterSet>("dt_tp_quality");
  edm::ParameterSet ps_ntps =  conf_.getParameter<edm::ParameterSet>("dt_tp_n");

  iBooker.setCurrentFolder(topFolderName_+"/DTDigis");

  HistoName = "DT_nTPs";
  DT_nTPs = iBooker.book1D(HistoName, HistoName,
      ps_ntps.getParameter<int32_t>("Nbinsx"),
      ps_ntps.getParameter<double>("xmin"),
      ps_ntps.getParameter<double>("xmax"));
  DT_nTPs->setAxisTitle("# DT TPs", 1);
  DT_nTPs->setAxisTitle("# Events", 2);

  HistoName = "DT_wheel";
  DT_wheel = iBooker.book1D(HistoName, HistoName,
      ps_wheel.getParameter<int32_t>("Nbinsx"),
      ps_wheel.getParameter<double>("xmin"),
      ps_wheel.getParameter<double>("xmax"));
  DT_wheel->setAxisTitle("Wheel", 1);
  DT_wheel->setAxisTitle("# DT TPs", 2);

  HistoName = "DT_station";
  DT_station = iBooker.book1D(HistoName, HistoName,
      ps_station.getParameter<int32_t>("Nbinsx"),
      ps_station.getParameter<double>("xmin"),
      ps_station.getParameter<double>("xmax"));
  DT_station->setAxisTitle("Station", 1);
  DT_station->setAxisTitle("# DT TPs", 2);

  HistoName = "DT_quality";
  DT_quality = iBooker.book1D(HistoName, HistoName,
      ps_quality.getParameter<int32_t>("Nbinsx"),
      ps_quality.getParameter<double>("xmin"),
      ps_quality.getParameter<double>("xmax"));
  DT_quality->setAxisTitle("Quality", 1);
  DT_quality->setAxisTitle("# DT TPs", 2);

  HistoName = "DT_sector";
  DT_sector = iBooker.book1D(HistoName, HistoName,
      ps_sector.getParameter<int32_t>("Nbinsx"),
      ps_sector.getParameter<double>("xmin"),
      ps_sector.getParameter<double>("xmax"));
  DT_sector->setAxisTitle("Sector", 1);
  DT_sector->setAxisTitle("# DT TPs", 2);

  HistoName = "DT_bx";
  DT_bx = iBooker.book1D(HistoName, HistoName,
      ps_bx.getParameter<int32_t>("Nbinsx"),
      ps_bx.getParameter<double>("xmin"),
      ps_bx.getParameter<double>("xmax"));
  DT_bx->setAxisTitle("BX", 1);
  DT_bx->setAxisTitle("# DT TPs", 2);

  HistoName = "DT_t0";
  DT_t0 = iBooker.book1D(HistoName, HistoName,
      ps_t0.getParameter<int32_t>("Nbinsx"),
      ps_t0.getParameter<double>("xmin"),
      ps_t0.getParameter<double>("xmax"));
  DT_t0->setAxisTitle("t0", 1);
  DT_t0->setAxisTitle("# DT TPs", 2);

} //end of method

DEFINE_FWK_MODULE(Phase2L1TDT);
