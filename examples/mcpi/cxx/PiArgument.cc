#include "PiArgument.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(PiArgument)

PiArgument::PiArgument()
{
}

PiArgument::PiArgument(double x1, double x2) :
  d_x1(x1), d_x2(x2)
{
}

PiArgument::~PiArgument()
{
}

double 
PiArgument::getX1() const
{
  return d_x1;
}

double
PiArgument::getX2() const
{
  return d_x2;
}


