//
// File:      DPDInputFilter.h
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if !defined(adaptive_sampling_DPDInputFilter_h)
#define adaptive_sampling_DPDInputFilter_h

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
     * @brief Interpolation database input filter for DPD argument.
     */
    class DPDInputFilter : public InterpolationDatabaseInputFilter {

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
      DPDInputFilter();

      /**
       * Destructor.
       */
      virtual ~DPDInputFilter();

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

      /**
       * Serialize DPDInputFilter.
       *
       * @param archive Archive to store serialization.
       * @param version Version of serialization.
       */
      template <class Archive>
      void serialize(Archive & archive, const unsigned int version);
         
    private:
      //
      // copy constructor/assignment operator
      //
      DPDInputFilter(const DPDInputFilter &); // not implemented
      DPDInputFilter & operator=(const DPDInputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(DPDInputFilter);

  }
}

#include "DPDInputFilter_t.cc"

#endif // adaptive_sampling_DPDInputFilter_h
