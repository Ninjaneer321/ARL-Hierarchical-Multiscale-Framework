//
// File:      DPDArgument.h
// Package    tests/adaptive_sampling/interpolation_database
//
#if !defined(tests_adaptive_sampling_interpolation_dpd_DPDArgument_h)
#define tests_adaptive_sampling_interpolation_dpd_DPDArgument_h

#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/Argument.h>
#include <utils/HMSMacros.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing dpd argument.
     */
    class DPDArgument : public Argument {

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
      DPDArgument();

      /**
       * Constructor.
       *
       * @param strain DPD value to store.
       */
      DPDArgument(double energyDensity,
                  double massDensity);

      /**
       * Destructor.
       */
      virtual ~DPDArgument();

      /**
       * Get energy density.
       */
      double getEnergyDensity() const;

      /**
       * Get mass density.
       */
      double getMassDensity() const;

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
      DPDArgument(const DPDArgument &); // not implemented
      DPDArgument & operator=(const DPDArgument &); // not implemented

      //
      // data
      //
    private:

      double d_energyDensity;
      double d_massDensity;
    
    };

    HMS_SHARED_PTR(DPDArgument);

  }
}

#include "DPDArgument_t.cc"

#endif // tests_adaptive_sampling_interpolation_dpd_DPDArgument_h
