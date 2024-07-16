//
// File:      SleepArgument.h
// Package    tests/adaptive_sampling/multiplexer
//
#if !defined(tests_adaptive_sampling_multiplexer_SleepArgument_h)
#define tests_adaptive_sampling_multiplexer_SleepArgument_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>
#include <utils/HMSMacros.h>

#if defined(HAVE_VECTOR)
#include <vector>
#else
#error vector header file not available
#endif // HAVE_VECTOR

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing strain argument.
     */
    class SleepArgument : public Argument {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor
       */
      SleepArgument();

      /**
       * Constructor.
       *
       * @param sleepTime Time to sleep.
       */
      SleepArgument(int sleepTime);

      /**
       * Destructor.
       */
      virtual ~SleepArgument();

      /**
       * Get sleep time.
       */
      double getSleepTime() const;

      /**
       * Serialize argument.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      SleepArgument(const SleepArgument &); // not implemented
      SleepArgument & operator=(const SleepArgument &); // not implemented

      //
      // data
      //
    private:

      int d_sleepTime;
    
    };

    HMS_SHARED_PTR(SleepArgument);

  }
}

#include "SleepArgument_t.cc"

#endif // tests_adaptive_sampling_multiplexer_SleepArgument_h
