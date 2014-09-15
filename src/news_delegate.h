#pragma once

#include <string>

namespace NewsHub
{
  class NewsDelegate
  {
  public:
    virtual ~NewsDelegate()
    {
    }

    virtual void Message(const std::string & message) = 0;
  };
}