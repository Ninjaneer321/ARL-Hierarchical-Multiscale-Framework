//
// Test KDE Speculative Generator
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#if defined(HAVE_MPI)
#include <mpi.h>
#endif // HAVE_MPI

#include <adaptive_sampling/InterpolationDatabaseInputFilter.h>
#include <adaptive_sampling/KDEGenerator.h>
#include <base/Argument.h>

namespace arl {
  namespace hms {

    class DummyArgument : public Argument {
      
    public:
      
      DummyArgument(const std::vector<double> & argument) : 
	d_argument(argument) {};
      ~DummyArgument() {};

      std::vector<double> d_argument;
      
    };
    
    class DummyInputFilter : public InterpolationDatabaseInputFilter {
      
    public:

      DummyInputFilter() {};
      ~DummyInputFilter() {};
      
      std::vector<double> apply(const ArgumentPointer & argument) const
      {
	boost::shared_ptr<DummyArgument> dummyArgument =
	  boost::dynamic_pointer_cast<arl::hms::DummyArgument>(argument);
        return dummyArgument->d_argument;
      }
      
      ArgumentPointer
      applyInverse(const std::vector<double> & value,
                   const ArgumentPointer & /* argument */) const
      {
        return ArgumentPointer(new DummyArgument(value));
      }
      
      int getDimension() const { return 2; }
      
    };
        
  }
}

//
//
//
int
main(int /* ac */,
     char ** /* av */)
{

#if defined(HAVE_MPI)
  
  MPI_Init(&ac,
           &av);
  
#endif // HAVE_MPI

  //sleep(10);

  const int cycleSize = 50;

  arl::hms::InterpolationDatabaseInputFilterPointer 
    inputFilter(new arl::hms::DummyInputFilter);
  arl::hms::KDEGeneratorPointer 
    kdeGenerator(new arl::hms::KDEGenerator(inputFilter,
					    cycleSize));
	
  //
  // repeat this process
  //
  for(int j=0; j<5; ++j) {

    int limit = 1;
    if(j ==0) {
      limit = 2;
    }
						     
    //
    // generate random points & build kde
    //
    std::cout << "Calling KDE Update" << std::endl;
    for(int i=0; i<cycleSize * limit; ++i) {

      std::vector<double> newArgument(2);
      for(int j=0; j<2; ++j) {
	newArgument[j] = 2.0 * ( (double) rand() / RAND_MAX - 0.5 );
      }   
      arl::hms::ArgumentPointer 
	dummyArgument(new arl::hms::DummyArgument(newArgument));
      arl::hms::ModelPackagePointer 
	modelPackage(new arl::hms::ModelPackage(arl::hms::ModelPointer(),
						arl::hms::InputFilterPointer(),
						arl::hms::OutputFilterPointer(),
						dummyArgument));
      
      kdeGenerator->update(modelPackage);   
      
    }
    std::cout << "Done All Updates" << std::endl;

  
    //
    // build speculative points
    //
    int numberPoints = 50;
    arl::hms::ModelPackagePointer 
      modelPackage(new arl::hms::ModelPackage(arl::hms::ModelPointer(),
					      arl::hms::InputFilterPointer(),
					      arl::hms::OutputFilterPointer(),
					      arl::hms::ArgumentPointer()));
    
    int numberGeneratedPoints = 0;
    while(numberGeneratedPoints != numberPoints) {
      std::vector<arl::hms::ModelPackagePointer> modelPackages =
	kdeGenerator->generate(modelPackage,
			       numberPoints);
      std::cout << "Generated " << modelPackages.size() << " points" << std::endl;
      numberGeneratedPoints = modelPackages.size();
      for(std::vector<arl::hms::ModelPackagePointer>::size_type i=0;
	  i<modelPackages.size(); ++i) {
	const arl::hms::ModelPackagePointer & modelPackage = modelPackages[i];
	const arl::hms::ArgumentPointer & argument = modelPackage->getArgument();
	boost::shared_ptr<arl::hms::DummyArgument> dummyArgument =
	  boost::dynamic_pointer_cast<arl::hms::DummyArgument>(argument);
	std::cout << dummyArgument->d_argument[0] << " " 
		  << dummyArgument->d_argument[1] << std::endl;
	
      
      }
      sleep(1);

    }    

  }

}
