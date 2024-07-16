//
// File:      XYValue_t.cc
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
    // Serialize XYValue.
    //
    template <class Archive>
    void 
    XYValue::serialize(Archive & archive, const unsigned int)
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
