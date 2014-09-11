#include "server_thread_loop.h"
#include "socket_thread_loop.h"

#include <iostream>

namespace NewsHub
{
  ServerThreadLoop::ServerThreadLoop(Server & _server)
    : server(_server)
  {
  }

  ServerThreadLoop::~ServerThreadLoop()
  {
    std::cout << "~ServerThreadLoop()" << std::endl;
  }

  bool ServerThreadLoop::LoopBody()
  {
    Socket* socket = server.Listen();

    if (socket)
      SocketThreadLoop* socketThreadLoop = new SocketThreadLoop(*socket);

    return true;
  }
}