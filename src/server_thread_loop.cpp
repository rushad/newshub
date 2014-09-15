#include "server_thread_loop.h"
#include "socket_thread_loop.h"

#include <iostream>

namespace NewsHub
{
  ServerThreadLoop::ServerThreadLoop(Server & _server, NewsDelegate & _newsDelegate)
    : server(_server),
      newsDelegate(_newsDelegate)
  {
  }

  ServerThreadLoop::~ServerThreadLoop()
  {
    while (!socketThreadLoops.empty())
    {
      delete socketThreadLoops.front();
      socketThreadLoops.pop_front();

      delete sockets.front();
      sockets.pop_front();
    }
  }

  bool ServerThreadLoop::LoopBody()
  {
    Socket* socket = server.Listen();

    if (socket)
    {
      SocketThreadLoop* socketThreadLoop = new SocketThreadLoop(*socket, newsDelegate);

      sockets.push_back(socket);
      socketThreadLoops.push_back(socketThreadLoop);
    }

    return true;
  }
}