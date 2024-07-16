//
// Test Basic Monitor
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/predicate/EndPredicate.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing picalc end predicate.
     */
    class PicalcEndPredicate : public EndPredicate {

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
      PicalcEndPredicate();

      /**
       * Destructor.
       */
      virtual ~PicalcEndPredicate();

      /**
       * Evaluate predicate.
       *
       * @param command The command used to evaluate the predicate.
       *
       * @return True if command completed successfully, False otherwise.
       */
      virtual bool evaluate(const Command & command);
  
      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      PicalcEndPredicate(const PicalcEndPredicate &); // not implemented
      PicalcEndPredicate & operator=(const PicalcEndPredicate &); // not implemented

      //
      // data
      //
    private:

    };

  }
}
