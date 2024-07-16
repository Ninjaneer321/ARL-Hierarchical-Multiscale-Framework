//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Value.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing pi value.
     */
    class PerfValue : public Value {

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
      PerfValue();

      /**
       * Constructor.
       *
       * @param value Value to store
       */
      PerfValue(double value);

      /**
       * Destructor.
       */
      virtual ~PerfValue();

      /**
       * Get value.
       */
      double getValue() const;
  
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
      PerfValue(const PerfValue &); // not implemented
      PerfValue & operator=(const PerfValue &); // not implemented

      //
      // data
      //
    private:

      double d_value;

    };

    HMS_SHARED_PTR(PerfValue);

  }
}

#include "PerfValue_t.cc"

