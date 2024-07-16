//
// File:      Queue_t.cc
// Package    base
//
// Base classes for HMS.
//

//
//
//

namespace arl {
  namespace hms {

    //
    // Constructor.
    //
    template <typename Object>
    Queue<Object>::Queue()
    {

      //
      //
      //
      return;

    }

    //
    // Destructor.
    //
    template <typename Object>
    Queue<Object>::~Queue()
    {

      //
      //
      //
      return;

    }

    //
    // Add object to end of queue.
    //
    template <typename Object>
    void 
    Queue<Object>::add(const Object & object)
    {

      //
      // add object to end of queue
      //
      d_queue.push_back(object);

      //
      //
      //
      return;

    }

    //
    // Get iterator to beginning of queue.
    //
    template <typename Object>
    typename Queue<Object>::iterator 
    Queue<Object>::begin()
    {

      //
      //
      //
      return d_queue.begin();

    }

    //
    // Get iterator to beginning of queue.
    //
    template <typename Object>
    typename Queue<Object>::const_iterator 
    Queue<Object>::begin() const
    {
      
      //
      //
      //
      return d_queue.begin();

    }

    //
    // Clear queue.
    //
    template <typename Object>
    void
    Queue<Object>::clear()
    {

      //
      // clear queue
      //
      d_queue.clear();
 
      //
      //
      //
      return;

    }

    //
    // Get whether queue is empty.
    //
    template <typename Object>
    bool 
    Queue<Object>::empty() const
    {

      //
      //
      //
      return d_queue.empty();

    }

    //
    // Get iterator to end of queue.
    //
    template <typename Object>
    typename Queue<Object>::iterator 
    Queue<Object>::end()
    {

      //
      //
      //
      return d_queue.end();

    }

    //
    // Get iterator to end of queue.
    //
    template <typename Object>   
    typename Queue<Object>::const_iterator 
    Queue<Object>::end() const
    {

      //
      //
      //
      return d_queue.end();

    }

    //
    // Pop object from queue.
    //
    template <typename Object>   
    void 
    Queue<Object>::pop(iterator iterator)
    {
 
      //
      // erase object at iterator position from queue
      //
      d_queue.erase(iterator);
      
      //
      //
      //
      return;

    }

    //
    // Get size of the queue.
    //
    template <typename Object>
    typename Queue<Object>::size_type 
    Queue<Object>::size() const
    {

      //
      //
      //
      return d_queue.size();

    }

    //
    // Sort queue.
    //
    template <typename Object>
    template <class Compare>
    void 
    Queue<Object>::sort(Compare comp)
    {
      
      //
      // sort queue
      //
      d_queue.sort(comp);

      //
      //
      //
      return;

    }     

    //
    // Output operator.
    //
    template <typename Object>
    std::ostream & operator<<(std::ostream        & outputStream,
                              const Queue<Object> & queue)
    {

      //
      // iterate over queue
      //
      typename Queue<Object>::const_iterator iter = queue.begin();
      const typename Queue<Object>::const_iterator iterEnd = queue.end();
      
      for( ; iter != iterEnd; ++iter) {
        
        outputStream << *iter << " ";

      }

      //
      //
      //
      return outputStream;
     
    }

  }
}
