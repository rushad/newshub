#pragma once

#include "socket.h"

namespace NewsHub
{
  class Server
  {
  public:
    virtual ~Server()
    {
    }

    virtual Socket* Listen() = 0;
  };
}