//
// File:      InterpolationDatabaseInputFilter.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_InterpolationDatabaseInputFilter_h)
#define adaptive_sampling_InterpolationDatabaseInputFilter_h

#include <base/Argument.h>
#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for interpolation database input filter
     * transforming Argument to vector of doubles.
     */
    class InterpolationDatabaseInputFilter {

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
      virtual ~InterpolationDatabaseInputFilter() = 0;

      /**
       * Apply filter to Argument returning vector of doubles.
       *
       * @param argument to filter.
       * 
       * @return vector containing representation of argument.
       */
      virtual std::vector<double>
      apply(const ArgumentPointer & argument) const = 0;

      /**
       * Apply filter inverse to vector of doubles, returning Argument
       * representation.
       *
       * @param value Vector of doubles representing Argument.
       * @param argument Original argument (can be used as a template
       * for creating new argument)
       *
       * @return Argument representation corresponding to value. 
       */
      virtual 
      ArgumentPointer
      applyInverse(const std::vector<double> & value,
                   const ArgumentPointer & argument) const = 0;

      /**
       * Get dimension of filtered argument.
       */
      virtual int
      getDimension() const = 0;

      /**
       * Serialize InterpolationDatabaseInputFilter.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);
      
    protected:

      /**
       * Constructor.
       */
      InterpolationDatabaseInputFilter();

    private:
      //
      // copy constructor/assignment operator
      //
      InterpolationDatabaseInputFilter(const InterpolationDatabaseInputFilter &); // not implemented
      InterpolationDatabaseInputFilter & operator=(const InterpolationDatabaseInputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(InterpolationDatabaseInputFilter);

  }
}


#include "InterpolationDatabaseInputFilter_t.cc"

#endif // adaptive_sampling_InterpolationDatabaseInputFilter_h
