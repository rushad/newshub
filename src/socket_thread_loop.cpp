#include "socket_thread_loop.h"

#include <iostream>

namespace NewsHub
{
  SocketThreadLoop::SocketThreadLoop(Socket & _socket)
    : socket(_socket)
  {
  }

  SocketThreadLoop::~SocketThreadLoop()
  {
  }

  bool SocketThreadLoop::LoopBody()
  {
    std::string res;
    if (!socket.Read(&res))
      return false;
    std::cout << res;
    return true;
  }
}