//
// File:      CommandFactory.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "CommandFactory.h"

#include "MPICommand.h"
#include "MPIMachinefileGenerator.h"
#include "PBSCommand.h"

#include <exception/IOError.h>

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    CommandFactory::CommandFactory(const CommandType & commandType,
				   const std::vector<StatusPredicatePointer> & statusPredicates,
				   const std::vector<EndPredicatePointer> & endPredicates) :
      d_commandType(commandType),
      d_statusPredicates(statusPredicates),
      d_endPredicates(endPredicates)
    {
      
      //
      //
      //
      return;

    }
 
    //
    // Destructor.
    //
    CommandFactory::~CommandFactory()
    {
      
      //
      //
      //
      return;

    }

    //
    // Build a command given a model.
    //
    CommandPointer 
    CommandFactory::build(const ModelPackagePointer & modelPackage,
                          const std::string         & directory)
    {

      //
      // build status and end predicates for the command by
      // concatenating the default predicates with the specific ones
      // added to this model package
      //
      std::vector<StatusPredicatePointer> statusPredicates(d_statusPredicates);
      std::vector<EndPredicatePointer> endPredicates(d_endPredicates);
      statusPredicates.insert(statusPredicates.end(),
			      modelPackage->getStatusPredicates().begin(),
			      modelPackage->getStatusPredicates().end());
      endPredicates.insert(endPredicates.end(),
			   modelPackage->getEndPredicates().begin(),
			   modelPackage->getEndPredicates().end());
      
      CommandPointer returnValue;

      if(d_commandType == MPI) {

	//
	// build appropriate machinefile generator
	//
	const MachinefileGeneratorPointer 
	  machinefileGenerator(new MPIMachinefileGenerator());
	
	//
	// build a command
	//
	returnValue = 
	  CommandPointer(new MPICommand(modelPackage,
					directory,
					statusPredicates,
					endPredicates,
					machinefileGenerator));

      }
      else if(d_commandType == PBS) {
	
	//
	// build a command
	//
	returnValue = 
	  CommandPointer(new PBSCommand(modelPackage,
					directory,
					statusPredicates,
					endPredicates));
	
      }
      else {
	const std::string message("Unknown command type specified");
	throw IOError(message);
      }

      //
      //
      //
      return returnValue;

    }

  }
}
