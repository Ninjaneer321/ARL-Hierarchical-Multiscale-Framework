#include "PiValue.h"
#include "PiOutputFilter.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(PiOutputFilter)

PiOutputFilter::PiOutputFilter()
{
}

PiOutputFilter::~PiOutputFilter()
{
}

arl::hms::ValuePointer
PiOutputFilter::apply(const std::string               & /* directory */,
		      const std::string               & stdOut,
		      const arl::hms::ArgumentPointer & /* argument */) const
{

  std::ifstream file(stdOut.c_str());

  int value;
  file >> value;

  PiValuePointer returnValue(new PiValue(value));
  return returnValue;

}
