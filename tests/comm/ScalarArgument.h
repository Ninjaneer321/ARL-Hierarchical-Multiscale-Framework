//
// File:      ScalarArgument.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_ScalarArgument_h)
#define base_ScalarArgument_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing scalar argument.
     */
    class ScalarArgument : public Argument {

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
      ScalarArgument();

      /**
       * Constructor.
       *
       * @param value Argument to store
       */
      ScalarArgument(double argument);

      /**
       * Destructor.
       */
      virtual ~ScalarArgument();

      /**
       * Get argument.
       */
      double getArgument() const;

      /**
       * Serialize value.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);

      //
      // data
      //
    public:
  
      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      ScalarArgument(const ScalarArgument &); // not implemented
      ScalarArgument & operator=(const ScalarArgument &); // not implemented

      //
      // data
      //
    private:

      double d_argument;

    };

  }
}

#include "ScalarArgument_t.cc"

#endif // base_ScalarArgument_h
