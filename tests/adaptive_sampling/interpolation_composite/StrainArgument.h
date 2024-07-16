//
// File:      StrainArgument.h
// Package    tests/adaptive_sampling/interpolation_database
//
#if !defined(tests_adaptive_sampling_interpolation_database_StrainArgument_h)
#define tests_adaptive_sampling_interpolation_database_StrainArgument_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>
#include <utils/HMSMacros.h>

#if defined(HAVE_VECTOR)
#include <vector>
#else
#error vector header file not available
#endif // HAVE_VECTOR

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing strain argument.
     */
    class StrainArgument : public Argument {

      //
      // types
      //
    public:

      //
      // methods
      //
    public:

      /**
       * Constructor
       */
      StrainArgument();

      /**
       * Constructor.
       *
       * @param strain Strain value to store.
       */
      StrainArgument(const std::vector<double> & strain);

      /**
       * Destructor.
       */
      virtual ~StrainArgument();

      /**
       * Get strain.
       */
      const std::vector<double> & getStrain() const;

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
      StrainArgument(const StrainArgument &); // not implemented
      StrainArgument & operator=(const StrainArgument &); // not implemented

      //
      // data
      //
    private:

      std::vector<double> d_strain;
    
    };

    HMS_SHARED_PTR(StrainArgument);

  }
}

#include "StrainArgument_t.cc"

#endif // tests_adaptive_sampling_interpolation_database_StrainArgument_h
