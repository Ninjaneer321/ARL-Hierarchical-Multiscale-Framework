//
// File:      Warehouse.h
// Package    warehouse
//
// HMS warehouse.
//
#if !defined(warehouse_Warehouse_h)
#define warehouse_Warehouse_h

#include <comm/Communicator.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Warehouse for storage of objects.
     */
    template <typename Object,
              template<typename, typename> class STLContainer = std::vector>
    class Warehouse {

      //
      // types
      //
    public:

      typedef 
      STLContainer<CommunicatorPointer, 
                   std::allocator<CommunicatorPointer> > CommunicatorContainer;

      //
      // methods
      //
    public:

      /**
       * Constructor.
       *
       * @param communicators Communicators to ship and receive from.
       */
      Warehouse(const CommunicatorContainer & communicators);

      /**
       * Destructor.
       */
      ~Warehouse();
  
      /**
       * Receive objects.
       *
       * @return Vector of objects received from warehouse.
       */
      std::vector<Object> receive();

      /**
       * Ship objects.
       *
       * @param objects Vector of objects to ship.
       */
      void ship(const std::vector<Object> & objects);

      //
      // methods
      // 
    protected:

    private:
      //
      // copy constructor/assignment operator
      //
      Warehouse(const Warehouse &); // not implemented
      Warehouse & operator=(const Warehouse &); // not implemented

      //
      // data
      //
    private:

      Warehouse::CommunicatorContainer d_communicators;

    };

  }
}

#include "Warehouse_t.cc"

#endif // warehouse_Warehouse_h
