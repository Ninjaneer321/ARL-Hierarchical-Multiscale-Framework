//
// File:      ScalarValue_t.cc
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
    // Serialize value.
    //
    template <class Archive>
    void 
    ScalarValue::serialize(Archive & archive, const unsigned int version)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<Value>(*this);

      //
      // serialize value
      //
      archive & d_value;
      
      //
      //
      //
      return;
      
    }
  
  }
}
