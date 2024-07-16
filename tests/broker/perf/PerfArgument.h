//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>

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
     * @brief Class implementing perf argument.
     */
    class PerfArgument : public Argument {

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
      PerfArgument();

      /**
       * Constructor.
       *
       * @param argument Argument to store
       */
      PerfArgument(const std::vector<double> & argument);

      /**
       * Destructor.
       */
      virtual ~PerfArgument();

      /**
       * Get argument.
       */
      const std::vector<double> & getArgument() const;
  
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
      PerfArgument(const PerfArgument &); // not implemented
      PerfArgument & operator=(const PerfArgument &); // not implemented

      //
      // data
      //
    private:

      std::vector<double> d_argument;

    };

    HMS_SHARED_PTR(PerfArgument);

  }
}

#include "PerfArgument_t.cc"

