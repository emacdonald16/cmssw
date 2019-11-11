
#include "DQM/EcalMonitorTasks/interface/TimingTask.h"
#include "DQM/EcalCommon/interface/EcalDQMCommonUtils.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlock.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/Phase2L1T/interface/Phase2L1TRPC.h"

Phase2L1TRPC::Phase2L1TRPC(const edm::ParameterSet& iConfig)
: conf_(iConfig)
{
  using namespace std;
  topFolderName_ = conf_.getParameter<std::string>("TopFolderName");
  RPCDigiToken_ = consumes <RPCDigiCollection> (conf_.getParameter<edm::InputTag>("RPCDigis") );
  RPCRecHitToken_ = consumes <RPCRecHitCollection> (conf_.getParameter<edm::InputTag>("RPCRecHits") );
}

Phase2L1TRPC::~Phase2L1TRPC() {}

void Phase2L1TRPC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;

  edm::Handle<RPCDigiCollection> RPCDigiHandle;
  iEvent.getByToken(RPCDigiToken_, RPCDigiHandle);

  edm::Handle<RPCRecHitCollection> RPCRecHitHandle;
  iEvent.getByToken(RPCRecHitToken_, RPCRecHitHandle);

  // const RPCDigiCollection* digis = nullptr;
  // const FWGeometry* geom = iItem->getGeom();

  if (RPCDigiHandle.isValid()){
    const RPCDigiCollection *RPCdigis = RPCDigiHandle.product();
    RPCDigiCollection::DigiRangeIterator RLayerIt;

    for (RLayerIt = RPCdigis->begin(); RLayerIt != RPCdigis->end(); ++RLayerIt) {
      const RPCDigiCollection::Range& range = (*RLayerIt).second;
      for (RPCDigiCollection::const_iterator dit = range.first; dit != range.second; ++dit) {
        int rpc_digi_strip = (*dit).strip();
        int rpc_digi_bx = (*dit).bx();
        int rpc_digi_time = (*dit).time();
        bool is_iRPC = (*dit).hasY();

        if ( is_iRPC ){
          iRPCDigi_strip->Fill(rpc_digi_strip);
          iRPCDigi_bx->Fill(rpc_digi_bx);
          iRPCDigi_time->Fill(rpc_digi_time);
          iRPCDigi_y->Fill( (*dit).coordinateY() );
        }
        else { //Regular RPC
          RPCDigi_strip->Fill(rpc_digi_strip);
          RPCDigi_bx->Fill(rpc_digi_bx);
          RPCDigi_time->Fill(rpc_digi_time);
        }

      }
    } // end loop over RPC digis
  }
  else {
    edm::LogWarning("DataNotFound") << "RPC digis for Phase2 L1T cannot be found!\n";
  }

  if (RPCRecHitHandle.isValid()){
    //Loop over RPC rec htis
    for (RPCRecHitCollection::const_iterator tpItr = RPCRecHitHandle->begin(); tpItr != RPCRecHitHandle->end(); ++tpItr) {
      int rpc_rechit_strip = tpItr->firstClusterStrip();
      int rpc_rechit_clustersize = tpItr->clusterSize();
      int rpc_rechit_bx = tpItr->BunchX();
      int rpc_rechit_time = tpItr->time();

      RPCDetId detId = tpItr->rpcId();
      int region = detId.region(); //0 for barrel, +/- for ECs
      int ring = detId.ring(); //Ring id: Wheel number in Barrel (from -2 to +2) Ring Number in Endcap (from 1 to 3)
      int station = detId.station(); //For Barrel: the four groups of chambers at same r (distance from beam axis) and increasing phi, For Endcap: the three groups of chambers at same z (distance from interaction point), i.e. the disk
      int sector = detId.sector(); //the group of chambers at same phi (and increasing r)

      bool isiRPC_RecHit = (region==1 && (station==3||station==4) && ring==1);
      if (isiRPC_RecHit) {
        iRPCRecHit_firstClusterStrip->Fill(rpc_rechit_strip);
        iRPCRecHit_clusterSize->Fill(rpc_rechit_clustersize);
        iRPCRecHit_bx->Fill(rpc_rechit_bx);
        iRPCRecHit_time->Fill(rpc_rechit_time);
        int rpc_rechit_y = tpItr->localPosition().y();
        iRPCRecHit_y->Fill(rpc_rechit_y);
      }
      else {
        RPCRecHit_firstClusterStrip->Fill(rpc_rechit_strip);
        RPCRecHit_clusterSize->Fill(rpc_rechit_clustersize);
        RPCRecHit_bx->Fill(rpc_rechit_bx);
        RPCRecHit_time->Fill(rpc_rechit_time);
      }

      //Number of rec hits
      if (region==0) {
        RPCRecHit_Barrel_Wheel->Fill(ring);
        RPCRecHit_Barrel_Station->Fill(station);
        RPCRecHit_Barrel_Sector->Fill(sector);
      }
      else {
        RPCRecHit_EC_Ring->Fill(ring);
        RPCRecHit_EC_Station->Fill(station);
        RPCRecHit_EC_Sector->Fill(sector);
      }
    } //end loop over RPC rec hits
  }
  else {
    edm::LogWarning("DataNotFound") << "RPC RecHits for Phase2 L1T cannot be found!\n";
  }
} //end analyze

void Phase2L1TRPC::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const & run, edm::EventSetup const & es) {
  std::string HistoName;

  edm::ParameterSet ps_digi_strip =  conf_.getParameter<edm::ParameterSet>("rpc_digi_strip");
  edm::ParameterSet ps_rechit_strip =  conf_.getParameter<edm::ParameterSet>("rpc_rechit_strip");
  edm::ParameterSet ps_rechit_size =  conf_.getParameter<edm::ParameterSet>("rpc_rechit_size");
  edm::ParameterSet ps_bx =  conf_.getParameter<edm::ParameterSet>("rpc_bx");
  edm::ParameterSet ps_time =  conf_.getParameter<edm::ParameterSet>("rpc_time");
  edm::ParameterSet ps_y =  conf_.getParameter<edm::ParameterSet>("rpc_y");
  edm::ParameterSet ps_nRing =  conf_.getParameter<edm::ParameterSet>("rpc_ring");
  edm::ParameterSet ps_nStation =  conf_.getParameter<edm::ParameterSet>("rpc_station");
  edm::ParameterSet ps_nSector =  conf_.getParameter<edm::ParameterSet>("rpc_sector");

  //RPC Digis
  iBooker.setCurrentFolder(topFolderName_+"/RPC/Digis");
  HistoName = "RPCDigi_strip";
  RPCDigi_strip = iBooker.book1D(HistoName, HistoName,
    ps_digi_strip.getParameter<int32_t>("Nbinsx"),
    ps_digi_strip.getParameter<double>("xmin"),
    ps_digi_strip.getParameter<double>("xmax")
  );
  RPCDigi_strip->setAxisTitle("Strip index", 1); //x axis
  RPCDigi_strip->setAxisTitle("# RPC Digis", 2); //y axis

  HistoName = "RPCDigi_bx";
  RPCDigi_bx = iBooker.book1D(HistoName, HistoName,
    ps_bx.getParameter<int32_t>("Nbinsx"),
    ps_bx.getParameter<double>("xmin"),
    ps_bx.getParameter<double>("xmax")
  );
  RPCDigi_bx->setAxisTitle("Bunch crossing", 1); //x axis
  RPCDigi_bx->setAxisTitle("# RPC Digis", 2); //y axis

  HistoName = "RPCDigi_time";
  RPCDigi_time = iBooker.book1D(HistoName, HistoName,
    ps_time.getParameter<int32_t>("Nbinsx"),
    ps_time.getParameter<double>("xmin"),
    ps_time.getParameter<double>("xmax")
  );
  RPCDigi_time->setAxisTitle("Sub-bx timing", 1); //x axis
  RPCDigi_time->setAxisTitle("# RPC Digis", 2); //y axis

  //iRPC Digis
  iBooker.setCurrentFolder(topFolderName_+"/iRPC/Digis");
  HistoName = "iRPCDigi_strip";
  iRPCDigi_strip = iBooker.book1D(HistoName, HistoName,
    ps_digi_strip.getParameter<int32_t>("Nbinsx"),
    ps_digi_strip.getParameter<double>("xmin"),
    ps_digi_strip.getParameter<double>("xmax")
  );
  iRPCDigi_strip->setAxisTitle("Strip index", 1); //x axis
  iRPCDigi_strip->setAxisTitle("# iRPC Digis", 2); //y axis

  HistoName = "iRPCDigi_bx";
  iRPCDigi_bx = iBooker.book1D(HistoName, HistoName,
    ps_bx.getParameter<int32_t>("Nbinsx"),
    ps_bx.getParameter<double>("xmin"),
    ps_bx.getParameter<double>("xmax")
  );
  iRPCDigi_bx->setAxisTitle("Bunch crossing", 1); //x axis
  iRPCDigi_bx->setAxisTitle("# iRPC Digis", 2); //y axis

  HistoName = "iRPCDigi_time";
  iRPCDigi_time = iBooker.book1D(HistoName, HistoName,
    ps_time.getParameter<int32_t>("Nbinsx"),
    ps_time.getParameter<double>("xmin"),
    ps_time.getParameter<double>("xmax")
  );
  iRPCDigi_time->setAxisTitle("Sub-bx timing", 1); //x axis
  iRPCDigi_time->setAxisTitle("# iRPC Digis", 2); //y axis

  HistoName = "iRPCDigi_y";
  iRPCDigi_y = iBooker.book1D(HistoName, HistoName,
    ps_y.getParameter<int32_t>("Nbinsx"),
    ps_y.getParameter<double>("xmin"),
    ps_y.getParameter<double>("xmax")
  );
  iRPCDigi_y->setAxisTitle("Y-coordinate", 1); //x axis
  iRPCDigi_y->setAxisTitle("# iRPC Digis", 2); //y axis



  //Rec Hits - number
  iBooker.setCurrentFolder(topFolderName_+"/RPC/RecHits/nRecHits");


  HistoName = "RPCRecHit_Barrel_Wheel";
  RPCRecHit_Barrel_Wheel = iBooker.book1D(HistoName, HistoName,
    ps_nRing.getParameter<int32_t>("Nbinsx"),
    ps_nRing.getParameter<double>("xmin"),
    ps_nRing.getParameter<double>("xmax")
  );
  RPCRecHit_Barrel_Wheel->setAxisTitle("Barrel Wheel", 1); //x axis
  RPCRecHit_Barrel_Wheel->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_Barrel_Station";
  RPCRecHit_Barrel_Station = iBooker.book1D(HistoName, HistoName,
    ps_nStation.getParameter<int32_t>("Nbinsx"),
    ps_nStation.getParameter<double>("xmin"),
    ps_nStation.getParameter<double>("xmax")
  );
  RPCRecHit_Barrel_Station->setAxisTitle("Barrel Station", 1); //x axis
  RPCRecHit_Barrel_Station->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_Barrel_Sector";
  RPCRecHit_Barrel_Sector = iBooker.book1D(HistoName, HistoName,
    ps_nSector.getParameter<int32_t>("Nbinsx"),
    ps_nSector.getParameter<double>("xmin"),
    ps_nSector.getParameter<double>("xmax")
  );
  RPCRecHit_Barrel_Sector->setAxisTitle("Barrel Sector", 1); //x axis
  RPCRecHit_Barrel_Sector->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_EC_Ring";
  RPCRecHit_EC_Ring = iBooker.book1D(HistoName, HistoName,
    ps_nRing.getParameter<int32_t>("Nbinsx"),
    ps_nRing.getParameter<double>("xmin"),
    ps_nRing.getParameter<double>("xmax")
  );
  RPCRecHit_EC_Ring->setAxisTitle("Endcap Ring", 1); //x axis
  RPCRecHit_EC_Ring->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_EC_Station";
  RPCRecHit_EC_Station = iBooker.book1D(HistoName, HistoName,
    ps_nStation.getParameter<int32_t>("Nbinsx"),
    ps_nStation.getParameter<double>("xmin"),
    ps_nStation.getParameter<double>("xmax")
  );
  RPCRecHit_EC_Station->setAxisTitle("Endcap Station", 1); //x axis
  RPCRecHit_EC_Station->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_EC_Sector";
  RPCRecHit_EC_Sector = iBooker.book1D(HistoName, HistoName,
    ps_nSector.getParameter<int32_t>("Nbinsx"),
    ps_nSector.getParameter<double>("xmin"),
    ps_nSector.getParameter<double>("xmax")
  );
  RPCRecHit_EC_Sector->setAxisTitle("Endcap Sector", 1); //x axis
  RPCRecHit_EC_Sector->setAxisTitle("# RPC RecHits", 2); //y axis


  //Rec Hits - general quantities
  iBooker.setCurrentFolder(topFolderName_+"/RPC/RecHits");

  HistoName = "RPCRecHit_firstClusterStrip";
  RPCRecHit_firstClusterStrip = iBooker.book1D(HistoName, HistoName,
    ps_rechit_strip.getParameter<int32_t>("Nbinsx"),
    ps_rechit_strip.getParameter<double>("xmin"),
    ps_rechit_strip.getParameter<double>("xmax")
  );
  RPCRecHit_firstClusterStrip->setAxisTitle("First cluster strip", 1); //x axis
  RPCRecHit_firstClusterStrip->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_clusterSize";
  RPCRecHit_clusterSize = iBooker.book1D(HistoName, HistoName,
    ps_rechit_size.getParameter<int32_t>("Nbinsx"),
    ps_rechit_size.getParameter<double>("xmin"),
    ps_rechit_size.getParameter<double>("xmax")
  );
  RPCRecHit_clusterSize->setAxisTitle("Cluster size", 1); //x axis
  RPCRecHit_clusterSize->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_bx";
  RPCRecHit_bx = iBooker.book1D(HistoName, HistoName,
    ps_bx.getParameter<int32_t>("Nbinsx"),
    ps_bx.getParameter<double>("xmin"),
    ps_bx.getParameter<double>("xmax")
  );
  RPCRecHit_bx->setAxisTitle("Bunch crossing", 1); //x axis
  RPCRecHit_bx->setAxisTitle("# RPC RecHits", 2); //y axis

  HistoName = "RPCRecHit_time";
  RPCRecHit_time = iBooker.book1D(HistoName, HistoName,
    ps_time.getParameter<int32_t>("Nbinsx"),
    ps_time.getParameter<double>("xmin"),
    ps_time.getParameter<double>("xmax")
  );
  RPCRecHit_time->setAxisTitle("Sub-bx timing", 1); //x axis
  RPCRecHit_time->setAxisTitle("# RPC RecHits", 2); //y axis


  //iRPC Rec Hits - general quantities
  iBooker.setCurrentFolder(topFolderName_+"/iRPC/RecHits");

  HistoName = "iRPCRecHit_firstClusterStrip";
  iRPCRecHit_firstClusterStrip = iBooker.book1D(HistoName, HistoName,
    ps_rechit_strip.getParameter<int32_t>("Nbinsx"),
    ps_rechit_strip.getParameter<double>("xmin"),
    ps_rechit_strip.getParameter<double>("xmax")
  );
  iRPCRecHit_firstClusterStrip->setAxisTitle("First cluster strip", 1); //x axis
  iRPCRecHit_firstClusterStrip->setAxisTitle("# iRPC RecHits", 2); //y axis

  HistoName = "iRPCRecHit_clusterSize";
  iRPCRecHit_clusterSize = iBooker.book1D(HistoName, HistoName,
    ps_rechit_size.getParameter<int32_t>("Nbinsx"),
    ps_rechit_size.getParameter<double>("xmin"),
    ps_rechit_size.getParameter<double>("xmax")
  );
  iRPCRecHit_clusterSize->setAxisTitle("Cluster size", 1); //x axis
  iRPCRecHit_clusterSize->setAxisTitle("# iRPC RecHits", 2); //y axis

  HistoName = "iRPCRecHit_bx";
  iRPCRecHit_bx = iBooker.book1D(HistoName, HistoName,
    ps_bx.getParameter<int32_t>("Nbinsx"),
    ps_bx.getParameter<double>("xmin"),
    ps_bx.getParameter<double>("xmax")
  );
  iRPCRecHit_bx->setAxisTitle("Bunch crossing", 1); //x axis
  iRPCRecHit_bx->setAxisTitle("# iRPC RecHits", 2); //y axis

  HistoName = "iRPCRecHit_time";
  iRPCRecHit_time = iBooker.book1D(HistoName, HistoName,
    ps_time.getParameter<int32_t>("Nbinsx"),
    ps_time.getParameter<double>("xmin"),
    ps_time.getParameter<double>("xmax")
  );
  iRPCRecHit_time->setAxisTitle("Sub-bx timing", 1); //x axis
  iRPCRecHit_time->setAxisTitle("# iRPC RecHits", 2); //y axis

  HistoName = "iRPCRecHit_y";
  iRPCRecHit_y = iBooker.book1D(HistoName, HistoName,
    ps_y.getParameter<int32_t>("Nbinsx"),
    ps_y.getParameter<double>("xmin"),
    ps_y.getParameter<double>("xmax")
  );
  iRPCRecHit_y->setAxisTitle("Y-coordinate", 1); //x axis
  iRPCRecHit_y->setAxisTitle("# iRPC ReCHits", 2); //y axis

} //end of method

DEFINE_FWK_MODULE(Phase2L1TRPC);
