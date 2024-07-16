//
// File:      Model.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Model_h)
#define base_Model_h

#include "ResourceType.h"

#include <utils/HMSMacros.h>

#include <string>
#include <vector>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Base class implementing Model - an interface to an executable 
     * that evaluates f(Argument)
     */
    class Model {

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
      Model();
      
      /**
       * Constructor.
       *
       * @param prefix Prefix before executable required to execute model.
       * @param executable Path to executable evaluation function f.
       * @param arguments Vector of command line arguments needed for execution.
       * @param resourceType Type of resource required for execution.
       * @param resouceAmount Amount of resources required for execution.
       */
      Model(const std::string              & prefix,
            const std::string              & executable,
            const std::vector<std::string> & arguments,
            const ResourceType             & resourceType,
            int                              resourceAmount);

      /**
       * Destructor.
       */
      virtual ~Model();

      /**
       * Obtain arguments.
       *
       * @return Vector of command line arguments needed for execution.
       */
      const std::vector<std::string> & getArguments() const;

      /**
       * Obtain executable.
       *
       * @return Path to executable evaluation function f.
       */
      const std::string & getExecutable() const;

      /**
       * Obtain prefix.
       *
       * @return Prefix before executable required to execute model.
       */
      const std::string & getPrefix() const;

      /**
       * Obtain resource amount.
       *
       * @return Amount of resources required for execution.
       */
      const int & getResourceAmount() const;
      
      /**
       * Obtain resource type.
       *
       * @return Type of resource required for execution.
       */
      const ResourceType & getResourceType() const;
      
      /**
       * Serialize model.
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
      Model(const Model &); // not implemented
      Model & operator=(const Model &); // not implemented

      //
      // data
      //
    private:
      
      std::string              d_prefix;
      std::string              d_executable;
      std::vector<std::string> d_arguments;
      ResourceType             d_resourceType;
      int                      d_resourceAmount;

    };

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param model Model object.
     *
     * @return Updated output stream.
     */
    std::ostream & operator<<(std::ostream & outputStream,
                              const Model  & model);

    HMS_SHARED_PTR(Model);

  }
}

#include "Model_t.cc"

#endif // base_Model_h
