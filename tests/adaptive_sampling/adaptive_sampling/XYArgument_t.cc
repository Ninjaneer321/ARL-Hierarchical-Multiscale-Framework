//
// File:      XYArgument_t.cc
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
    // Serialize XYArgument.
    //
    template <class Archive>
    void 
    XYArgument::serialize(Archive & archive, const unsigned int)
    {

      //
      // serialize base class
      //
      archive & boost::serialization::base_object<Argument>(*this);

      //
      // serialize x & y
      //
      archive & d_x;
      archive & d_y;
      
      //
      //
      //
      return;
      
    }
  
  }
}
