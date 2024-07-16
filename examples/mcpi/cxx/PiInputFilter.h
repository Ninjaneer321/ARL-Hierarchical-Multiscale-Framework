#include <base/InputFilter.h>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

class PiInputFilter : public arl::hms::InputFilter {

public:

  PiInputFilter();
  PiInputFilter(const std::string & pointFileName);
  virtual ~PiInputFilter();

  virtual void
  apply(const arl::hms::ArgumentPointer & argument,
	const std::string               & directory) const;
  
  template <class Archive>
  void serialize(Archive & archive, const unsigned int version) {
    archive & boost::serialization::base_object<InputFilter>(*this);
    archive & d_pointFileName;
  }
  
private:
  
  PiInputFilter(const PiInputFilter &); // not implemented
  PiInputFilter & operator=(const PiInputFilter &); // not implemented

  std::string d_pointFileName;

};

HMS_SHARED_PTR(PiInputFilter);
