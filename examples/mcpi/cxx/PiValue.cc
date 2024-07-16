#include "PiValue.h"

#include <utils/HMSMacros.h>

HMS_SERIALIZATION_EXPORT(PiValue)

PiValue::PiValue()
{
}

PiValue::PiValue(int value) :
  d_value(value)
{
}

PiValue::~PiValue()
{
}

int
PiValue::getValue() const
{
  return d_value;
}


