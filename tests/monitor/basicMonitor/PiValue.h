//
// Basic Monitor Test.
//
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
