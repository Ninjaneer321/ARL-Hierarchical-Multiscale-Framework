//
// File:      XYAdaptiveSamplingInputFilter.h
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if !defined(adaptive_sampling_XYAdaptiveSamplingInputFilter_h)
#define adaptive_sampling_XYAdaptiveSamplingInputFilter_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <adaptive_sampling/InterpolationDatabaseInputFilter.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Interpolation database input filter for XY argument.
     */
    class XYAdaptiveSamplingInputFilter : public InterpolationDatabaseInputFilter {

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
      XYAdaptiveSamplingInputFilter();

      /**
       * Destructor.
       */
      virtual ~XYAdaptiveSamplingInputFilter();

      /**
       * Apply filter to Argument returning vector of doubles.
       *
       * @param argument to filter.
       * 
       * @return vector containing representation of argument.
       */
      virtual std::vector<double>
      apply(const ArgumentPointer & argument) const;

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
		     const ArgumentPointer & argument) const;

      /**
       * Get dimension of filtered argument.
       */
      virtual int
      getDimension() const;
      
    private:
      //
      // copy constructor/assignment operator
      //
      XYAdaptiveSamplingInputFilter(const XYAdaptiveSamplingInputFilter &); // not implemented
      XYAdaptiveSamplingInputFilter & operator=(const XYAdaptiveSamplingInputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(XYAdaptiveSamplingInputFilter);

  }
}

#endif // adaptive_sampling_XYAdaptiveSamplingInputFilter_h
