//
// File:      MonteCarloGenerator.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_MonteCarloGenerator_h)
#define adaptive_sampling_MonteCarloGenerator_h

#include "SpeculativeGenerator.h"

#include "InterpolationDatabaseInputFilter.h"

#include <base/ModelPackage.h>

#include <utils/HMSMacros.h>

#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Random generator of speculative evaluations.
     */
    class MonteCarloGenerator : public SpeculativeGenerator {

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
      MonteCarloGenerator(const InterpolationDatabaseInputFilterPointer & inputFilter);

      /**
       * Destructor.
       */
      virtual ~MonteCarloGenerator();


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
      MonteCarloGenerator(const MonteCarloGenerator &); // not implemented
      MonteCarloGenerator & operator=(const MonteCarloGenerator &); // not implemented

      //
      // data
      //
    private:
      
      InterpolationDatabaseInputFilterPointer d_inputFilter;
      

    };

    HMS_SHARED_PTR(MonteCarloGenerator);

  }
}

#endif // adaptive_sampling_MonteCarloGenerator_h
