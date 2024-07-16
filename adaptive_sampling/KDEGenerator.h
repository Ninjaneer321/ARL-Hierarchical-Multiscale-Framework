//
// File:      KDEGenerator.h
// Package    adaptive_sampling
//
// Adaptive Sampling Algorithm.
//
#if !defined(adaptive_sampling_KDEGenerator_h)
#define adaptive_sampling_KDEGenerator_h

#include "SpeculativeGenerator.h"

#include "InterpolationDatabaseInputFilter.h"

#include <base/ModelPackage.h>

#include <utils/HMSMacros.h>

#include <pthread.h>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Generator of speculative evaluations using kernel
     * density estimator.
     */
    class KDEGenerator : public SpeculativeGenerator {

      //
      // types
      //
    public:

      class GenerateThreadData;
      class KDEData;

      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      KDEGenerator(const InterpolationDatabaseInputFilterPointer & inputFilter,
		   int cycleSize);

      /**
       * Destructor.
       */
      virtual ~KDEGenerator();

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
      KDEGenerator(const KDEGenerator &); // not implemented
      KDEGenerator & operator=(const KDEGenerator &); // not implemented

      //
      // data
      //
    private:

      int                                       d_cycleSize;
      pthread_cond_t                            d_generateThreadCondition;
      GenerateThreadData                      * d_generateThreadData;
      bool                                      d_generatingKDE;
      InterpolationDatabaseInputFilterPointer   d_inputFilter;
      pthread_mutex_t                           d_kdeLock;
      pthread_t                                 d_threadId;

      //
      // PIMPL
      //
      KDEData * d_kdeData;   
      
    };

    HMS_SHARED_PTR(KDEGenerator);

  }
}

#endif // adaptive_sampling_KDEGenerator_h
