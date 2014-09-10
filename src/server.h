#pragma once

#include "hub_delegate.h"

namespace NewsHub
{
  class Server
  {
  public:
    virtual ~Server()
    {
    }

    ConnectedServer* Listen(const ServerParameters & params);
  };
}