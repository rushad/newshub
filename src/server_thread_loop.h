#pragma once

#include "thread_loop.h"

#include "server.h"

namespace NewsHub
{
  class ServerThreadLoop : public ThreadLoop
  {
  public:
    ServerThreadLoop(Server & _server);
    ~ServerThreadLoop();
    virtual bool LoopBody();

  private:
    Server & server;
  };
}