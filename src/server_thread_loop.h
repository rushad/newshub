#pragma once

#include "thread_loop.h"

#include "news_delegate.h"
#include "server.h"
#include "socket.h"
#include "socket_thread_loop.h"

#include <list>

#include "config.h"

namespace NewsHub
{
  class ServerThreadLoop : public ThreadLoop
  {
  public:
    ServerThreadLoop(Server & _server, NewsDelegate & _newsDelegate, bool _multipleConnections = true);
    ~ServerThreadLoop();
    virtual bool LoopBody();

  private:
    ServerThreadLoop();
    ServerThreadLoop(const ServerThreadLoop &);

    Server & server;
    NewsDelegate & newsDelegate;
    bool multipleConnections;

    std::list<Socket*> sockets;
    std::list<SocketThreadLoop*> socketThreadLoops;
  };
}