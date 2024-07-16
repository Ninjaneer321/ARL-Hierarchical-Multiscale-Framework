//
// File:      TimeoutStatusPredicate_t.cc
// Package    base/predicate
//
// Predicates for HMS model evaluations
//

#include <boost/serialization/base_object.hpp>

//
//
//

namespace arl {
  namespace hms {

    //
    // Serialize
    //
    template <class Archive>
    void 
    TimeoutStatusPredicate::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive &
	boost::serialization::base_object<StatusPredicate>(*this);

      //
      // serialize maximum wall clock
      //
      archive & d_maximumWallClockTime;
      
      //
      //
      //
      return;
      
    }
  
  }
}
