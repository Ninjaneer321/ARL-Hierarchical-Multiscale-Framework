//
// File:      SpeculativeGenerator.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_SpeculativeGenerator_h)
#define adaptive_sampling_SpeculativeGenerator_h

#include <base/ModelPackage.h>
#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class for HMS Speculative Generation of Evaluations.
     */
    class SpeculativeGenerator {

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
      virtual ~SpeculativeGenerator() = 0;

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
		 const unsigned int numberEvaluations) = 0;

      /**
       * Update generator with new completed evaluations
       *
       * @param modelPackage A completed evaluation
       */
      virtual void
	update(const ModelPackagePointer & modelPackage) = 0;

    protected:
      
      /**
       * Constructor.
       */
      SpeculativeGenerator();

    private:
      //
      // copy constructor/assignment operator
      //
      SpeculativeGenerator(const SpeculativeGenerator &); // not implemented
      SpeculativeGenerator & operator=(const SpeculativeGenerator &); // not implemented

      //
      // data
      //
    private:

    };

    HMS_SHARED_PTR(SpeculativeGenerator);

  }
}

#endif // adaptive_sampling_SpeculativeGenerator_h
