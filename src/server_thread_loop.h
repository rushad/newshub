#pragma once

#include "thread_loop.h"

#include "server.h"
#include "socket.h"
#include "socket_thread_loop.h"

#include <list>

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
    std::list<Socket*> sockets;
    std::list<SocketThreadLoop*> socketThreadLoops;
  };
}