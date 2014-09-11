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
    MessageInfo mi;
    if (!socket.Read(&mi))
      return false;
    std::cout << mi.message;
    return true;
  }
}