#pragma once

#include "socket.h"

#include <string>

namespace NewsHub
{
  class NewsDelegate
  {
  public:
    virtual ~NewsDelegate()
    {
    }

    virtual bool Message(const Socket & socket, const unsigned int messageId, const std::string & message) = 0;
  };
}