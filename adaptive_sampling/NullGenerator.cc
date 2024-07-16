//
// File:      NullGenerator.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "NullGenerator.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    NullGenerator::NullGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    NullGenerator::~NullGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Generate evaluations
    // 
    std::vector<ModelPackagePointer>
    NullGenerator::generate(const ModelPackagePointer & /* modelPackageTemplate */,
			    unsigned int /* numberEvaluations */)
    {
      
      //
      // instantiate return value
      //
      std::vector<ModelPackagePointer> returnValue;

      //
      //
      //
      return returnValue;

    }

    //
    // Update generator with new completed evaluations
    //
    void
    NullGenerator::update(const ModelPackagePointer & /* modelPackage */)
    {

      //
      //
      //
      return;

    }

  }
}
