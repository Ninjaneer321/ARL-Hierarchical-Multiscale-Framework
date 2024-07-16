//
// File:      StressArgument_t.cc
// Package    base
//
// Base classes for HMS.
//

#if defined(HAVE_BOOST_SERIALIZATION)
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
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
    StressValue::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<Value>(*this);

      //
      // serialize stress
      //
      archive & d_stress;

      //
      //
      //
      return;

    }

  }
}

