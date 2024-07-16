//
// File:      XYOutputFilter_t.cc
// Package    base
//
// Base classes for HMS.
//

//
//
//

namespace arl {
  namespace hms {

    //
    // Serialize XYOutputFilter.
    //
    template <class Archive>
    void 
    XYOutputFilter::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<OutputFilter>(*this);

      //
      //
      //
      return;
      
    }
  
  }
}
