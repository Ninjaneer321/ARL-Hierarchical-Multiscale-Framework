//
// File:      XYInputFilter.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_XYInputFilter_h)
#define base_XYInputFilter_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/InputFilter.h>

#include <utils/HMSMacros.h>

#if defined(HAVE_STRING)
#include <string>
#else
#error string header file not available
#endif // HAVE_STRING

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Input filter for executable accepting XYArgument.
     */
    class XYInputFilter : public InputFilter {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      XYInputFilter();
      
      /**
       * Destructor.
       */
      virtual ~XYInputFilter();
  
      /**
       * Apply XYInputFilter
       *
       * @param argument Argument that XYInputFilter acts on.
       * @param directory Directory path where Model will be run.
       */
      virtual void 
      apply(const ArgumentPointer & argument, 
            const std::string     & directory) const;

      /**
       * Serialize XYInputFilter.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);
  
    private:
      //
      // copy constructor/assignment operator
      //
      XYInputFilter(const XYInputFilter &); // not implemented
      XYInputFilter & operator=(const XYInputFilter &); // not implemented

      //
      // data
      //
    private:
      
    };

    HMS_SHARED_PTR(XYInputFilter);

  }
}

#include "XYInputFilter_t.cc"

#endif // base_XYInputFilter_h
