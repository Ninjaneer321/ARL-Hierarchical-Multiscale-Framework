//
// Basic Monitor Test.
//
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
     * @brief Class implementing pi argument.
     */
    class PiArgument : public Argument {

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
      PiArgument();

      /**
       * Constructor.
       *
       * @param argument Argument to store
       */
      PiArgument(double argument);

      /**
       * Destructor.
       */
      virtual ~PiArgument();

      /**
       * Get argument.
       */
      double getArgument() const;
  
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
      PiArgument(const PiArgument &); // not implemented
      PiArgument & operator=(const PiArgument &); // not implemented

      //
      // data
      //
    private:

      double d_argument;

    };

    HMS_SHARED_PTR(PiArgument);

  }
}

#include "PiArgument_t.cc"

