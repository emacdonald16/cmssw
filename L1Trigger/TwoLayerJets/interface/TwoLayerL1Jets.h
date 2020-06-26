#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
using namespace edm;

class TwoLayerClusterData {
public:
  TwoLayerClusterData(vector< Ptr< TTTrack< Ref_Phase2TrackerDigi_ > > > L1TrackPtrs,int netabins, int nphibins, int nzbins);
  void FillCellsinZlice(vector< Ptr< TTTrack< Ref_Phase2TrackerDigi_ > > > L1TrackPtrs,float Zmin, float Zmax);
  void MarkCellUsed(unsigned int cellindex) {AlreadyClustered[cellindex]=true;}
  void MergeCells(unsigned int seed,unsigned int neighbor, std::vector<unsigned int> &TracksInSeed);
  void PhiSlice(float phi,std::vector<float> &EtaCellsPt);

private:
  int NEta_;
  int NPhi_;
  int Nz_;
  float etastep;
  float phistep;
  float zstep;
  vector< Ptr< TTTrack<Ref_Phase2TrackerDigi_> > > L1TrackPtrs_;
  std::vector<std::pair<float,float> > EtaPhiBins_; // bin centers
  std::vector<bool> AlreadyClustered; // clustered bool
  std::vector<float> CellPt;
  std::vector<float> ClusterPt;
  std::vector<int> NTracks;
  std::vector<int> N_ttrk;
  std::vector<int> N_tdtrk;
  std::vector<int> N_ttdtrk;
  std::vector< std::vector<unsigned int> > TracksInCells;
};

TwoLayerClusterData::TwoLayerClusterData(vector< Ptr< TTTrack<Ref_Phase2TrackerDigi_> > > L1TrackPtrs, int netabins, int nphibins, int nzbins):
NEta_(netabins), NPhi_(nphibins), Nz_(nzbins)
{
  L1TrackPtrs_ = L1TrackPtrs;
  float phi = -1.0 * M_PI;
  etastep = 2.0 * maxeta / netabins;
  phistep = 2.0*M_PI / nphibins;
  EtaPhiBins_.clear();
  for (int i = 0; i < nphibins; ++i) {
    float phimin = phi;
    float phimax = phi + phistep;
    float eta = -1.0 * 2.4;
    for (int j = 0; j < netabins; ++j) {
      float etamin = eta;
      eta = eta + etastep;
      float etamax = eta;

      pair<float,float> temp;
      temp.first = (phimin + phimax)/2.;
      temp.second = (etamin + etamax)/2.;
      EtaPhiBins_.push_back(temp);
      AlreadyClustered.push_back(false);
      ClusterPt.push_back(0);
      N_ttrk.push_back(0);
      N_tdtrk.push_back(0);
      N_ttdtrk.push_back(0);
    } // for each phibin
    phi = phi + phistep;
  }
}

// Fill eta-phi grid for a given z-slice
void TwoLayerClusterData::FillCellsinZlice(vector< Ptr< TTTrack<Ref_Phase2TrackerDigi_> > > L1TrackPtrs,float Zmin, float Zmax) {
  TracksInCells.clear();
  CellPt.clear();
  for (unsigned int i = 0; i < EtaPhiBins_.size(); ++i) {
    float etamin = EtaPhiBins_[i].second - etastep/2.0; // ith cell eta bounds
    float phimin = EtaPhiBins_[i].first - phistep/2.0; // ith cell phi bounds
    float etamax = EtaPhiBins_[i].second + etastep/2.0; // ith cell eta bounds
    float phimax = EtaPhiBins_[i].first + phistep/2.0; // ith cell phi bounds
    vector<unsigned int> TrackIndices;

    float totalpt = 0;
    for (unsigned int k = 0; k < L1TrackPtrs.size(); ++k) {
      float trk_z0 = L1TrackPtrs[k]->z0();
      float trketa = L1TrackPtrs[k]->momentum().eta();
      float trkphi = L1TrackPtrs[k]->momentum().phi();
      float trkpt = L1TrackPtrs[k]->momentum().perp();
      if (trk_z0<Zmin || trk_z0>Zmax) continue;
      if (trketa<etamin || trketa>etamax) continue;
      if (trkphi<phimin || trkphi>phimax) continue;
      TrackIndices.push_back(k); // tracks in eta-phi grid cell
      totalpt = totalpt+trkpt;
    }
    CellPt.push_back(totalpt);
    TracksInCells.push_back(TrackIndices); // list of tracks for each eta-phi index
  }
}

void TwoLayerClusterData::MergeCells(unsigned int seed, unsigned int neighbor, std::vector<unsigned int> &TracksInSeed) {
  TracksInSeed = TracksInCells[seed];
  for (unsigned int t=0; t<TracksInCells[seed].size(); ++t)TracksInSeed.push_back(TracksInCells[seed][t]);
  MarkCellUsed(seed);
  MarkCellUsed(neighbor);
  CellPt[seed] = CellPt[seed]+CellPt[neighbor];
  CellPt[neighbor] = 0;
}

//return vector of eta bins for a phi slice for L1 clustering
void TwoLayerClusterData::PhiSlice(float phi,std::vector<float>&EtaCellsPt) {
  for (unsigned int i = 0; i < EtaPhiBins_.size(); ++i) {
    if (fabs(EtaPhiBins_[i].first-phi) > phistep/2.0) continue;
    EtaCellsPt.push_back(CellPt[i]); // will have a vector of size netabins
  }
  if (EtaCellsPt.size()==0) edm::LogWarning("TwoLayerJets")
    <<"Eta clusters not found for"<<phi<<"\n";
}
