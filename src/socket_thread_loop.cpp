#include "socket_thread_loop.h"

#include <iostream>

namespace NewsHub
{
  SocketThreadLoop::SocketThreadLoop(Socket & _socket, NewsDelegate & _newsDelegate)
    : socket(_socket),
      newsDelegate(_newsDelegate)
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
    if (!res.empty())
    {
      newsDelegate.Message(res);
    }
    return true;
  }
}