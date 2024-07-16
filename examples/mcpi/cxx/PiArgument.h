#include <base/Argument.h>

#include <boost/serialization/base_object.hpp>

class PiArgument : public arl::hms::Argument {
  
public:
  
  PiArgument();
  PiArgument(double x1, double x2);
  virtual ~PiArgument();
  
  template <class Archive>
  void serialize(Archive & archive, const unsigned int version) {
    archive & boost::serialization::base_object<Argument>(*this);
    archive & d_x1;
    archive & d_x2;
  };
  
  double getX1() const;
  double getX2() const;
  
private:
  
  PiArgument(const PiArgument &); // not implemented
  PiArgument & operator=(const PiArgument &); // not implemented
  
  double d_x1, d_x2;
  
};

HMS_SHARED_PTR(PiArgument);
