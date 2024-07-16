//
// File:      Queue.h
// Package    base
//
// Base classes for HMS.
//
#if !defined(base_Queue_h)
#define base_Queue_h

#include <iostream>
#include <list>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing queue of objects.
     */
    template <typename Object>
    class Queue {
      
      //
      // types
      //
    public:
      
      typedef std::list<Object> QueueType;
      typedef typename QueueType::iterator iterator;
      typedef typename QueueType::const_iterator const_iterator;
      typedef typename QueueType::size_type size_type;
      
      //
      // methods
      //
    public:

      /**
       * Constructor.
       */
      Queue();

      /**
       * Destructor.
       */
      ~Queue();

      /**
       * Add object to end of queue.
       *
       * @param object to add to end of Queue.
       */
      void add(const Object & object);

      /**
       * Get iterator to beginning of queue.
       *
       * @return Iterator to beginning of queue.
       */
      iterator begin();

      /**
       * Get iterator to beginning of queue.
       *
       * @return Iterator to beginning of queue.
       */
      const_iterator begin() const;

      /**
       * Clear queue.
       */
      void clear();

      /**
       * Get iterator to end of queue.
       *
       * @return Iterator to end of queue.
       */
      iterator end();

      /**
       * Get iterator to end of queue.
       *
       * @return Iterator to end of queue.
       */
      const_iterator end() const;

      /**
       * Get whether queue is empty.
       *
       * @return Whether queue is empty.
       */
      bool empty() const;

      /**
       * Get size of the queue.
       *
       * @return Size of the queue.
       */
      size_type size() const;

      /**
       * Sort queue.
       */
      template <class Compare>
      void sort(Compare comp);
      
      /**
       * Pop object from queue.
       *
       * @param iterator Iterator to object to be removed. After
       * object is popped, the iterator is invalid.
       */
      void pop(iterator iterator);
  
      //
      // methods
      // 
    protected:

 
    private:
      //
      // copy constructor/assignment operator
      //
      Queue(const Queue &); // not implemented
      Queue & operator=(const Queue &); // not implemented

      //
      // data
      //
    private:

      QueueType d_queue;

    };

    /**
     * Output operator.
     *
     * @param outputStream Output stream to use for output.
     * @param queue Queue object.
     *
     * @return Updated output stream.
     */
    template <typename Object>
    std::ostream & operator<<(std::ostream        & outputStream,
                              const Queue<Object> & queue);


  }
}

#include "Queue_t.cc"

#endif // base_Queue_h
