//
// File:      XYInputFilter_t.cc
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
    // Serialize XYInputFilter.
    //
    template <class Archive>
    void 
    XYInputFilter::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<InputFilter>(*this);

      //
      //
      //
      return;
      
    }
  
  }
}
