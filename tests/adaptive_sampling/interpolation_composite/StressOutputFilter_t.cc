//
// File:      StressOutputFilter_t.cc
// Package    base
//
// Base classes for HMS.
//

#if defined(HAVE_BOOST_SERIALIZATION)
#include <boost/serialization/base_object.hpp>
#else
#error boost serialization not available
#endif // HAVE_BOOST_SERIALIZATION

//
//
//

namespace arl {
  namespace hms {

    //
    // Serialize StressOutputFilter.
    //
    template <class Archive>
    void 
    StressOutputFilter::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<InterpolationDatabaseOutputFilter>(*this);

      //
      // serialize scale
      //
      archive & d_scale;

      //
      //
      //
      return;
      
    }
  
  }
}
