//
// File:      NullGenerator.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_NullGenerator_h)
#define adaptive_sampling_NullGenerator_h

#include "SpeculativeGenerator.h"

#include <base/ModelPackage.h>

#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Null generator of speculative evaluations (doesn't
     * generate any speculative evaluations)
     */
    class NullGenerator : public SpeculativeGenerator {

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
      NullGenerator();

      /**
       * Destructor.
       */
      virtual ~NullGenerator();


      /**
       * Generate evaluations
       * 
       * @param modelPackageTemplate Model Package used as a basis for
       * new generated model packages
       * @param numberEvaluations Number of evaluations to generate
       * @return Vector of model packages.
       */
      virtual std::vector<ModelPackagePointer>
	generate(const ModelPackagePointer & modelPackageTemplate,
		 const unsigned int numberEvaluations);

      /**
       * Update generator with new completed evaluations
       *
       * @param modelPackage A completed evaluation
       */
      virtual void
	update(const ModelPackagePointer & modelPackage);

    protected:
      
    private:
      //
      // copy constructor/assignment operator
      //
      NullGenerator(const NullGenerator &); // not implemented
      NullGenerator & operator=(const NullGenerator &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(NullGenerator);

  }
}

#endif // adaptive_sampling_NullGenerator_h
