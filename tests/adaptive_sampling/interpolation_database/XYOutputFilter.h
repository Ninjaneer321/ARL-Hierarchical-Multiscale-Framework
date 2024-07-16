//
// File:      XYOutputFilter.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_XYOutputFilter_h)
#define base_XYOutputFilter_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/OutputFilter.h>
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
     * @brief Output filter for executable accepting XYArgument.
     */
    class XYOutputFilter : public OutputFilter {

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
      XYOutputFilter();
  
      /**
       * Destructor.
       */
      virtual ~XYOutputFilter();
  
      /**
       * Apply XYOutputFilter
       *
       * @param directory Directory path where Model was run.
       * @param stdOut Path containing stdout of Model execution.
       * @param argument Argument of the Model.
       */
      virtual ValuePointer 
      apply(const std::string & directory, 
            const std::string & stdOut,
	    const ArgumentPointer & argument) const;

      /**
       * Serialize XYOutputFilter.
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
      XYOutputFilter(const XYOutputFilter &); // not implemented
      XYOutputFilter & operator=(const XYOutputFilter &); // not implemented

      //
      // data
      //
    private:
      
    };

    HMS_SHARED_PTR(XYOutputFilter);

  }
}

#include "XYOutputFilter_t.cc"

#endif // base_XYOutputFilter_h
