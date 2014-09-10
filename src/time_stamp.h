#pragma once

#include <time.h>

namespace NewsHub
{
  class TimeStamp
  {
  public:
    TimeStamp(const TimeStamp & timeStamp);
    TimeStamp();

  private:
    time_t timeValue;
  };
}