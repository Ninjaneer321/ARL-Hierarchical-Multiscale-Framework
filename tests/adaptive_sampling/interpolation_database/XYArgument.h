//
// File:      XYArgument.h
// Package    tests/adaptive_sampling/interpolation_database
//
#if !defined(tests_adaptive_sampling_interpolation_database_XYArgument_h)
#define tests_adaptive_sampling_interpolation_database_XYArgument_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>
#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing two-dimensional argument.
     */
    class XYArgument : public Argument {

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
       *
       * @param x value.
       * @param y value.
       */
      XYArgument(double x,
                 double y);

      /**
       * Destructor.
       */
      virtual ~XYArgument();

      /**
       * Get X.
       */
      double getX() const;
  
      /**
       * Get Y.
       */
      double getY() const;

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      XYArgument(const XYArgument &); // not implemented
      XYArgument & operator=(const XYArgument &); // not implemented

      //
      // data
      //
    private:

      const double d_x;
      const double d_y;

    };

    HMS_SHARED_PTR(XYArgument);

  }
}

#endif // tests_adaptive_sampling_interpolation_database_XYArgument_h
