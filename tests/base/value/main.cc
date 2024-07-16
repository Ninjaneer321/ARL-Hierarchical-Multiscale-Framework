//
// Test value
//
#if defined(HAVE_CONFIG_H)
#include "arlhms_config.h"
#endif // HAVE_CONFIG_H 

#include <base/Value.h>

#include "ScalarValue.h"

#if defined(HAVE_BOOST_SERIALIZATION)
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#else
#error boost serialization not available
#endif // HAVE_BOOST_SERIALIZATION

#if defined(HAVE_IOSTREAM)
#include <iostream>
#else
#error iostream header file not available.
#endif // HAVE_IOSTREAM

#if defined(HAVE_SSTREAM)
#include <sstream>
#else
#error sstream header file not available.
#endif // HAVE_SSTREAM

//
//
//
int
main(int    ac,
     char * av[])
{

  //
  // instantiate ScalarValue of value 2.0
  //
  arl::hms::Value * value = new arl::hms::ScalarValue(2.0);

  //
  // serialize value
  //
  std::stringstream serializationStream;
  boost::archive::text_oarchive outputArchive(serializationStream);
  outputArchive << value;

  //
  // deserialize value
  //
  arl::hms::Value * newValue;
  boost::archive::text_iarchive inputArchive(serializationStream);
  inputArchive >> newValue;

  //
  // cast value to derived type
  //
  arl::hms::ScalarValue * scalarValue;
  scalarValue = dynamic_cast<arl::hms::ScalarValue *>(newValue);

  //
  // output value
  //
  std::cout << scalarValue->getValue() << std::endl;

  //
  // cleanup
  //
  delete value;
  delete newValue;

  //
  //
  //
  return 0;

}
