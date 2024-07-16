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
    class PiValue : public Value {

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
      PiValue();

      /**
       * Constructor.
       *
       * @param value Value to store
       */
      PiValue(double value);

      /**
       * Destructor.
       */
      virtual ~PiValue();

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
      PiValue(const PiValue &); // not implemented
      PiValue & operator=(const PiValue &); // not implemented

      //
      // data
      //
    private:

      double d_value;

    };

    HMS_SHARED_PTR(PiValue);

  }
}

#include "PiValue_t.cc"

