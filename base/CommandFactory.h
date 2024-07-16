//
// File:      CommandFactory.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_CommandFactory_h)
#define base_CommandFactory_h


#include "Command.h"
#include "CommandType.h"
#include "ModelPackage.h"

#include <utils/HMSMacros.h>

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing command factory - builds a particular
     * command from a model.
     */
    class CommandFactory {

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
       *
       * @param commandType Type of command this factory constructs.
       * @param statusPredicates Status predicates applied to all
       * created commands.  
       * @param endPredicates End predicates applied to all created
       * commands.
       */
      CommandFactory(const CommandType & commandType,
		     const std::vector<StatusPredicatePointer> & statusPredicates,
		     const std::vector<EndPredicatePointer> & endPredicates);
 
      /**
       * Destructor.
       */
      ~CommandFactory();

      /**
       * Build a command given a model.
       *
       * @param modelPackage The model package associated with the command.
       * @param directory Directory where command is executed.
       */
      CommandPointer build(const ModelPackagePointer & modelPackage,
			   const std::string         & directory);

    private:
      //
      // copy constructor/assignment operator
      //
      CommandFactory(const CommandFactory &); // not implemented
      CommandFactory & operator=(const CommandFactory &); // not implemented

      //
      // data
      //
    private: 
      CommandType                         d_commandType;
      std::vector<StatusPredicatePointer> d_statusPredicates;
      std::vector<EndPredicatePointer>    d_endPredicates;
      
    };

    //
    // 
    //
    HMS_SHARED_PTR(CommandFactory);

  }
}

#endif // base_CommandFactory_h
