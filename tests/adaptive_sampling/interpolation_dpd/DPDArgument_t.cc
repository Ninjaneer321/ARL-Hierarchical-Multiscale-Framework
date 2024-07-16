//
// File:      DPDArgument_t.cc
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
    // Serialize argument.
    //
    template <class Archive>
    void
    DPDArgument::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<Argument>(*this);

      //
      // serialize densities
      //
      archive & d_energyDensity;
      archive & d_massDensity;

      //
      //
      //
      return;

    }

  }
}

