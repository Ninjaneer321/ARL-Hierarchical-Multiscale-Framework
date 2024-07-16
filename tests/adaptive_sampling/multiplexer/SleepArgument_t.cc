//
// File:      SleepArgument_t.cc
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
    // Serialize argument.
    //
    template <class Archive>
    void
    SleepArgument::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<Argument>(*this);

      //
      // serialize sleepTime
      //
      archive & d_sleepTime;

      //
      //
      //
      return;

    }

  }
}

