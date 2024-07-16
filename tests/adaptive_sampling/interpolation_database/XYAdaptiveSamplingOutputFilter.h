//
// File:      XYAdaptiveSamplingOutputFilter.h
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if !defined(adaptive_sampling_XYAdaptiveSamplingOutputFilter_h)
#define adaptive_sampling_XYAdaptiveSamplingOutputFilter_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <adaptive_sampling/InterpolationDatabaseOutputFilter.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Interpolation database output filter constructing Value
     * from single value for z = f(x, y)
     */
    class XYAdaptiveSamplingOutputFilter : public InterpolationDatabaseOutputFilter {

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
      XYAdaptiveSamplingOutputFilter();

      /**
       * Destructor.
       */
      virtual ~XYAdaptiveSamplingOutputFilter();

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
	      const ArgumentPointer & argument) const;

      /**
       * Apply filter inverse to Value, returning vector representation.
       *
       * @param value Value.
       *
       * @return Vector representation corresponding to Value. 
       */
      virtual 
      std::vector<double>
      applyInverse(const ValuePointer & value) const;
       
      /**
       * Get dimension of filtered value.
       */
      virtual int getDimension() const;

    private:
      //
      // copy constructor/assignment operator
      //
      XYAdaptiveSamplingOutputFilter(const XYAdaptiveSamplingOutputFilter &); // not implemented
      XYAdaptiveSamplingOutputFilter & operator=(const XYAdaptiveSamplingOutputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(XYAdaptiveSamplingOutputFilter);

  }
}

#endif // adaptive_sampling_XYAdaptiveSamplingOutputFilter_h
