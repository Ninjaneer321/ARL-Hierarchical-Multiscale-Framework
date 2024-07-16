//
// File:      Model.cc
// Package    base
//
// Base classes for HMS.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include "Model.h"

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    Model::Model()
    {

      //
      //
      //
      return;

    }

    //
    // Constructor.
    //
    Model::Model(const std::string              & prefix,
                 const std::string              & executable,
                 const std::vector<std::string> & arguments,
                 const ResourceType             & resourceType,
                 int                              resourceAmount) :
      d_prefix(prefix),
      d_executable(executable),
      d_arguments(arguments),
      d_resourceType(resourceType),
      d_resourceAmount(resourceAmount)
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    Model::~Model()
    {

      //
      //
      //
      return;

    }

    //
    // Obtain arguments.
    //
    const std::vector<std::string> &
    Model::getArguments() const
    {

      //
      //
      //
      return d_arguments;

    }

    //
    // Obtain executable.
    //
    const std::string &
    Model::getExecutable() const
    {

      //
      //
      //
      return d_executable;

    }

    //
    // Obtain prefix.
    //
    const std::string &
    Model::getPrefix() const
    {

      //
      //
      //
      return d_prefix;

    }

    //
    // Obtain resource amount.
    //
    const int &
    Model::getResourceAmount() const
    {

      //
      //
      //
      return d_resourceAmount;

    }

    //
    // Obtain resource type.
    //
    const ResourceType &
    Model::getResourceType() const
    {

      //
      //
      //
      return d_resourceType;

    }

    //
    // output operator.
    //
    std::ostream & operator<<(std::ostream & outputStream,
                              const Model  & model)
    {

      //
      // output prefix
      //
      outputStream << "Prefix: " << model.getPrefix() << std::endl;

      //
      // output executable
      //
      outputStream << "Executable: " << model.getExecutable() << std::endl;

      //
      // output arguments
      //
      const std::vector<std::string> & arguments = model.getArguments();
      std::vector<std::string>::const_iterator iter = arguments.begin();
      const std::vector<std::string>::const_iterator iterEnd = arguments.end();
      outputStream << "Arguments: [ ";
      for(; iter != iterEnd; ++iter) {
        outputStream << *iter << " ";
      }
      outputStream << "]" << std::endl;

      //
      // output resource type
      //
      outputStream << "Resource Type: "
                   << resourceTypeToString(model.getResourceType())
                   << std::endl;

      //
      // output resource amount
      //
      outputStream << "Resource Amount: " << model.getResourceAmount();

      //
      //
      //
      return outputStream;

    }

  }
}
