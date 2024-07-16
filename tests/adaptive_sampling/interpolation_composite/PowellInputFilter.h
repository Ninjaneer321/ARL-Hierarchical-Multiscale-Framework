//
// Basic Monitor Test.
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H

#include <base/InputFilter.h>

//
//
//

namespace arl {
  namespace hms {

    /**
     * @brief Class implementing input filter for powell.
     */
    class PowellInputFilter : public InputFilter {

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
      PowellInputFilter();

      /**
       * Constructor.
       *
       * @param meshFileDirectory Directory containing mesh file for
       * powell model.
       * @param meshFileName Name of mesh file.
       */
      PowellInputFilter(const std::string & meshFileDirectory,
                        const std::string & meshFileName);

      /**
       * Destructor.
       */
      virtual ~PowellInputFilter();

      /**
       * Apply PowellInputFilter
       *
       * @param directory Directory path where Model was run.
       * @param stdOut Path containing stdout of Model execution.
       */
      virtual void
      apply(const ArgumentPointer & argument,
            const std::string     & directory) const;

      /**
       * Serialize input filter
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
      PowellInputFilter(const PowellInputFilter &); // not implemented
      PowellInputFilter & operator=(const PowellInputFilter &); // not implemented

      //
      // data
      //
    private:

      std::string d_meshFileDirectory;
      std::string d_meshFileName;

    };

  }
}

#include "PowellInputFilter_t.cc"
