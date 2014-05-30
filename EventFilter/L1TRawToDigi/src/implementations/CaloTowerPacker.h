#include "EventFilter/L1TRawToDigi/interface/PackerFactory.h"

namespace l1t {
   class CaloTowerPackerFactory : public PackerFactory {
      public:
         virtual PackerList create(const edm::ParameterSet& cfg, const unsigned& fw, const int fedid) override;
   };
}
