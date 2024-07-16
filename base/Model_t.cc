//
// File:      Model_t.cc
// Package    base
//
// Base classes for HMS.
//

#include <boost/serialization/vector.hpp>

//
//
//

namespace arl {
  namespace hms {

    //
    // Serialize model.
    //
    template <class Archive>
    void 
    Model::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize contents of model
      //
      archive & d_prefix;
      archive & d_executable;
      archive & d_arguments;
      archive & d_resourceType;
      archive & d_resourceAmount;

      //
      //
      //
      return;
      
    }
  
  }
}
