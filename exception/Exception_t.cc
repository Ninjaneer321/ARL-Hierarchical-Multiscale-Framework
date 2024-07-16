//
// File:      InputFilter_t.cc
// Package    base
//
// Base classes for HMS.
//

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_KEY(arl::hms::Exception)

namespace arl {
  namespace hms {

    //
    // Serialize InputFilter.
    //
    template <class Archive>
    void 
    Exception::serialize(Archive & ar, const unsigned int)
    {

      //
      // serialize message
      //
      ar & d_message;

      //
      //
      //
      return;
      
    }
  
  }
}
