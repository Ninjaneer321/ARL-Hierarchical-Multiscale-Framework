//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/InputFilter.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing perf output filter.
     */
    class PerfInputFilter : public InputFilter {

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
      PerfInputFilter();

      /**
       * Destructor.
       */
      virtual ~PerfInputFilter();

      /**
       * Apply PerfInputFilter
       *
       * @param directory Directory path where Model was run.
       * @param stdOut Path containing stdout of Model execution.
       */
      virtual void
      apply(const ArgumentPointer & argument,
            const std::string     & directory) const;

      /**
       * Serialize value.
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
      PerfInputFilter(const PerfInputFilter &); // not implemented
      PerfInputFilter & operator=(const PerfInputFilter &); // not implemented

      //
      // data
      //
    private:

    };

  }
}

#include "PerfInputFilter_t.cc"
