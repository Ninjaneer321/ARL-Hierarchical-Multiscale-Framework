#include <base/OutputFilter.h>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

class PiOutputFilter : public arl::hms::OutputFilter {

public:

  PiOutputFilter();
  virtual ~PiOutputFilter();

  virtual arl::hms::ValuePointer
  apply(const std::string               & directory,
	const std::string               & stdOut,
	const arl::hms::ArgumentPointer & argument) const;
  
  template <class Archive>
  void serialize(Archive & archive, const unsigned int version) {
    archive & boost::serialization::base_object<OutputFilter>(*this);
  }
  
private:
  
  PiOutputFilter(const PiOutputFilter &); // not implemented
  PiOutputFilter & operator=(const PiOutputFilter &); // not implemented

};

HMS_SHARED_PTR(PiOutputFilter);
