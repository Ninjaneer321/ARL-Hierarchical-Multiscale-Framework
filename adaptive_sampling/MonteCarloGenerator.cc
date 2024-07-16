//
// File:      MonteCarloGenerator.cc
// Package    adaptive_sampling
//
// HMS Interpolation Databases.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "MonteCarloGenerator.h"

#if defined(HAVE_CSTDLIB)
#include <cstdlib>
#else
#error cstdlib header file not available
#endif // HAVE_CSTDLIB


//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    MonteCarloGenerator::MonteCarloGenerator(const InterpolationDatabaseInputFilterPointer & inputFilter) :
      d_inputFilter(inputFilter)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    MonteCarloGenerator::~MonteCarloGenerator()
    {

      //
      //
      //
      return;

    }

    //
    // Generate evaluations
    // 
    std::vector<ModelPackagePointer>
    MonteCarloGenerator::generate(const ModelPackagePointer & modelPackageTemplate,
				  unsigned int numberEvaluations)
    {
      
      //
      // get dimension of point to generate
      //
      const int dimension = d_inputFilter->getDimension();

      //
      // instantiate return value
      //
      std::vector<ModelPackagePointer> returnValue;

      //
      // generate new model packages
      //
      for(unsigned int i=0; i<numberEvaluations; ++i) {
	std::vector<double> newArgument(dimension);
	for(int j=0; j<dimension; ++j) {
	  newArgument[j] = 2.0 * ( (double) rand() / RAND_MAX - 0.5 );
	}
	ArgumentPointer generatedArgument = 
	  d_inputFilter->applyInverse(newArgument,
				      modelPackageTemplate->getArgument());
	ModelPackagePointer 
	  newModelPackage(new ModelPackage(modelPackageTemplate->getModel(),
					   modelPackageTemplate->getInputFilter(),
					   modelPackageTemplate->getOutputFilter(),
					   generatedArgument));
	returnValue.push_back(newModelPackage);	
      }

      //
      //
      //
      return returnValue;

    }

    //
    // Update generator with new completed evaluations
    //
    void
    MonteCarloGenerator::update(const ModelPackagePointer & /* modelPackage */)
    {

      //
      //
      //
      return;

    }

  }
}
