//
// File:      BatchQueueSettings.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_BatchQueueSettings_h)
#define base_BatchQueueSettings_h

#include <utils/HMSMacros.h>

#include <string>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Storage for settings required to submit to a batch queue
     * (queue, walltime, etc.)
     */
    class BatchQueueSettings {

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
      BatchQueueSettings();
      
      /**
       * Constructor.
       *
       * @param queue Queue to use.
       * @param walltime Walltime.
       * @param numberNodes Number of Nodes.
       * @param numberCpus Number of Cpus.
       */
      BatchQueueSettings(const std::string & queue,
			 const std::string & walltime,
			 const int & numberNodes,
			 const int & numberCpus);

      /**
       * Destructor.
       */
      virtual ~BatchQueueSettings();

      /**
       * Obtain queue.
       *
       * @return queue.
       */
      const std::string & getQueue() const;

      /**
       * Obtain Walltime.
       *
       * @return walltime.
       */
      const std::string & getWalltime() const;

      /**
       * Obtain Number of Nodes.
       *
       * @return numberNodes.
       */
      const int & getNumberNodes() const;

      /**
       * Obtain Number of Cpus.
       *
       * @return numberCpus.
       */
      const int & getNumberCpus() const;
      
      /**
       * Serialize.
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
      BatchQueueSettings(const BatchQueueSettings &); // not implemented
      BatchQueueSettings & operator=(const BatchQueueSettings &); // not implemented

      //
      // data
      //
    private:
      
      std::string d_queue;
      std::string d_walltime;
      int d_numberNodes;
      int d_numberCpus;

    };

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param model BatchQueueSettings object.
     *
     * @return Updated output stream.
     */
    std::ostream & operator<<(std::ostream & outputStream,
                              const BatchQueueSettings & batchQueueSettings);

    HMS_SHARED_PTR(BatchQueueSettings);

  }
}

#include "BatchQueueSettings_t.cc"

#endif // base_BatchQueueSettings_h
