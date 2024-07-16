//
// File:      NumberProcessorsPerNode.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_NumberProcessorsPerNode_h)
#define base_NumberProcessorsPerNode_h

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief function to return number of nodes per processor
     * required to submit to a pbs batch queue
     */
    class NumberProcessorsPerNode {

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
      NumberProcessorsPerNode();
            
      /**
       * Destructor.
       */
      ~NumberProcessorsPerNode();

      /**
       * Obtain Number of Processors Per Node.
       *
       * @return number processors per node.
       */
      int getNumberProcessorsPerNode() const;

    private:

      //
      // copy constructor/assignment operator
      //
      NumberProcessorsPerNode(const NumberProcessorsPerNode &); // not implemented
      NumberProcessorsPerNode & operator=(const NumberProcessorsPerNode); // not implemented

      //
      // data
      //
    private:
      int d_numberCpus;  
   
    };
   
  }
}

#endif // base_NumberProcessorsPerNode_h
