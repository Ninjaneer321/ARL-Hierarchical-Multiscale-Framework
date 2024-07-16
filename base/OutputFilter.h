//
// File:      OutputFilter.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_OutputFilter_h)
#define base_OutputFilter_h

#include "Argument.h"
#include "Value.h"

#include <utils/HMSMacros.h>

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class implementing OutputFilter - Filters Model output to
     * compute Value.
     */
    class OutputFilter {

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
      virtual ~OutputFilter() = 0;
  
      /**
       * Apply OutputFilter
       *
       * @param directory Directory path where Model was run.
       * @param stdOut Path containing stdout of Model execution.
       * @param argument Argument of the Model.
       */
      virtual ValuePointer 
      apply(const std::string     & directory, 
            const std::string     & stdOut,
	    const ArgumentPointer & argument) const = 0;

      /**
       * Serialize OutputFilter.
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
      OutputFilter();

    private:
      //
      // copy constructor/assignment operator
      //
      OutputFilter(const OutputFilter &); // not implemented
      OutputFilter & operator=(const OutputFilter &); // not implemented

      //
      // data
      //
    private:
      
    };

    HMS_SHARED_PTR(OutputFilter);

  }
}

#include "OutputFilter_t.cc"

#endif // base_OutputFilter_h
