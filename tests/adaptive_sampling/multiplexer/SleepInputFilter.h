//
// File:      SleepInputFilter.h
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if !defined(adaptive_sampling_SleepInputFilter_h)
#define adaptive_sampling_SleepInputFilter_h

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
     * @brief Interpolation database input filter for Sleep argument.
     */
    class SleepInputFilter : public InterpolationDatabaseInputFilter {

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
      SleepInputFilter();

      /**
       * Destructor.
       */
      virtual ~SleepInputFilter();

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
       * Serialize SleepInputFilter.
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
      SleepInputFilter(const SleepInputFilter &); // not implemented
      SleepInputFilter & operator=(const SleepInputFilter &); // not implemented

      //
      // data
      //
    private:
        
    };

    HMS_SHARED_PTR(SleepInputFilter);

  }
}

#include "SleepInputFilter_t.cc"

#endif // adaptive_sampling_SleepInputFilter_h
