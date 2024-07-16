//
// File:      XYValue.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_XYValue_h)
#define base_XYValue_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Value.h>
#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing scalar value.
     */
    class XYValue : public Value {

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
      XYValue();

      /**
       * Constructor.
       *
       * @param value Value to store
       */
      XYValue(double value);

      /**
       * Destructor.
       */
      virtual ~XYValue();

      /**
       * Get value.
       */
      double getValue() const;
  
      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      XYValue(const XYValue &); // not implemented
      XYValue & operator=(const XYValue &); // not implemented

      //
      // data
      //
    private:

      double d_value;

    };

    HMS_SHARED_PTR(XYValue);

  }
}

#endif // base_XYValue_h
