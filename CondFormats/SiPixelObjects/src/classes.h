#include "CondFormats/SiPixelObjects/src/headers.h"

template class PixelDCSObject<bool>;
template class PixelDCSObject<float>;
template class PixelDCSObject<CaenChannel>;

namespace {
  struct dictionary {
    std::map<SiPixelFedCablingMap::Key, sipixelobjects::PixelROC> theMap; 
    std::pair<const SiPixelFedCablingMap::Key, sipixelobjects::PixelROC> theMapValueT; 
 
    std::map< unsigned int, SiPixelPedestals::SiPixelPedestalsVector> sipixped;
 
    std::vector<char>::iterator p1;
    std::vector<char>::const_iterator p2;
    std::vector< SiPixelGainCalibration::DetRegistry >::iterator p3;
    std::vector< SiPixelGainCalibration::DetRegistry >::const_iterator p4;
 
    std::vector< SiPixelGainCalibrationForHLT::DetRegistry >::iterator p5;
    std::vector< SiPixelGainCalibrationForHLT::DetRegistry >::const_iterator p6;
 
    std::vector< SiPixelGainCalibrationOffline::DetRegistry >::iterator p7;
    std::vector< SiPixelGainCalibrationOffline::DetRegistry >::const_iterator p8;
 
    std::vector<SiPixelPerformanceSummary::DetSummary>::iterator spps1;
    std::vector<SiPixelPerformanceSummary::DetSummary>::const_iterator spps2;
 
    std::vector<SiPixelQuality::disabledModuleType>::iterator p9;
    std::vector<SiPixelQuality::disabledModuleType>::const_iterator p10;
    std::vector<SiPixelDbItem> p11;
  };
}

