#include <base/Value.h>

#include <boost/serialization/base_object.hpp>

class PiValue : public arl::hms::Value {

public:
  
  PiValue();
  PiValue(int value);
  virtual ~PiValue();
  
  template <class Archive>
  void serialize(Archive & archive, const unsigned int version) {
    archive & boost::serialization::base_object<Value>(*this);
    archive & d_value;
  };
  
  int getValue() const;
  
private:
  
  PiValue(const PiValue &); // not implemented
  PiValue & operator=(const PiValue &); // not implemented
  
  int d_value;
  
};

HMS_SHARED_PTR(PiValue);
