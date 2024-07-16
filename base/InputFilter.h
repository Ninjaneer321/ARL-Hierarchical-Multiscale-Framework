//
// File:      InputFilter.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_InputFilter_h)
#define base_InputFilter_h

#include "Argument.h"

#include <utils/HMSMacros.h>

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class implementing InputFilter - Converts Argument
     * to input to Model so that Model can be executed.
     */
    class InputFilter {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:
      
      /**
       * Destructor.
       */
      virtual ~InputFilter() = 0;
  
      /**
       * Apply InputFilter
       *
       * @param argument Argument that InputFilter acts on.
       * @param directory Directory path where Model will be run.
       */
      virtual void 
      apply(const ArgumentPointer & argument, 
            const std::string     & directory) const = 0;

      /**
       * Serialize InputFilter.
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

      /**
       * Constructor.
       */
      InputFilter();

    private:
      //
      // copy constructor/assignment operator
      //
      InputFilter(const InputFilter &); // not implemented
      InputFilter & operator=(const InputFilter &); // not implemented

      //
      // data
      //
    private:
      
    };

    HMS_SHARED_PTR(InputFilter);

  }
}

#include "InputFilter_t.cc"

#endif // base_InputFilter_h
