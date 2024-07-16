#include "PiArgument.h"
#include "PiInputFilter.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(PiInputFilter)

PiInputFilter::PiInputFilter()
{
}

PiInputFilter::PiInputFilter(const std::string & pointFileName) :
  d_pointFileName(pointFileName)
{
}

PiInputFilter::~PiInputFilter()
{
}

void
PiInputFilter::apply(const arl::hms::ArgumentPointer & argument,
		     const std::string               & directory) const
{
  PiArgumentPointer piArgument =
    boost::dynamic_pointer_cast<PiArgument>(argument);
  const double x1 = piArgument->getX1();
  const double x2 = piArgument->getX2();

  std::stringstream filePath;
  filePath << directory << "/" << d_pointFileName;

  std::ofstream file(filePath.str().c_str());
  file << std::setprecision(16);
  file << x1 << std::endl;
  file << x2 << std::endl;
}
