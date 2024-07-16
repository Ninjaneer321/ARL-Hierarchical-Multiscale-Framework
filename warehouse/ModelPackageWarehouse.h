//
// File:      ModelPackageWarehouse.h
// Package    warehouse
//
// HMS warehouse.
//
#if !defined(warehouse_ModelPackageWarehouse_h)
#define warehouse_ModelPackageWarehouse_h

#include "Warehouse.h"

#include <base/ModelPackage.h>
#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    typedef Warehouse<ModelPackagePointer> ModelPackageWarehouse;

    HMS_SHARED_PTR(ModelPackageWarehouse);

  }
}

#endif // warehouse_ModelPackageWarehouse_h
