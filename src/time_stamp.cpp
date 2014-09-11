#include "time_stamp.h"

namespace NewsHub
{
  TimeStamp::TimeStamp(const TimeStamp & timeStamp)
    : timeValue(timeStamp.timeValue)
  {
  }

  TimeStamp::TimeStamp()
    : timeValue(time(0))
  {
  }
}