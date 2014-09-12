#pragma once

#include "socket.h"

namespace NewsHub
{
  class Client
  {
  public:
    virtual ~Client()
    {
    }

    virtual Socket* Connect() = 0;
  };
}