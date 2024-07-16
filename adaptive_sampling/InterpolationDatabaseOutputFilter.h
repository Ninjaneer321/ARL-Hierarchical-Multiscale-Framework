//
// File:      InterpolationDatabaseOutputFilter.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_InterpolationDatabaseOutputFilter_h)
#define adaptive_sampling_InterpolationDatabaseOutputFilter_h

#include <base/Argument.h>
#include <base/Value.h>
#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for interpolation database output filter
     * constructing Value from vector of doubles.
     */
    class InterpolationDatabaseOutputFilter {

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
      virtual ~InterpolationDatabaseOutputFilter() = 0;

      /**
       * Apply filter to vector of doubles returning Value.
       *
       * @param value Vector representation of Value.
       * @param argument Argument corresponding to the Value.
       * 
       * @return Value corresponding to vector respresentation.
       */
      virtual 
      ValuePointer
      apply(const std::vector<double> & value,
            const ArgumentPointer & argument) const = 0;

      /**
       * Apply filter inverse to Value, returning vector representation.
       *
       * @param value Value.
       *
       * @return Vector representation corresponding to Value. 
       */
      virtual 
      std::vector<double>
      applyInverse(const ValuePointer & value) const = 0;
       
      /**
       * Get dimension of filtered value.
       */
      virtual int getDimension() const = 0;

      /**
       * Serialize InterpolationDatabaseOutputFilter.
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
      InterpolationDatabaseOutputFilter();

    private:
      //
      // copy constructor/assignment operator
      //
      InterpolationDatabaseOutputFilter(const InterpolationDatabaseOutputFilter &); // not implemented
      InterpolationDatabaseOutputFilter & operator=(const InterpolationDatabaseOutputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(InterpolationDatabaseOutputFilter);

  }
}

#include "InterpolationDatabaseOutputFilter_t.cc"

#endif // adaptive_sampling_InterpolationDatabaseOutputFilter_h
