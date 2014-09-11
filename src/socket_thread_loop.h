#pragma once

#include "thread_loop.h"

#include "socket.h"

namespace NewsHub
{
  class SocketThreadLoop : public ThreadLoop
  {
  public:
    SocketThreadLoop(Socket & _socket);
    ~SocketThreadLoop();
    virtual bool LoopBody();

  private:
    Socket & socket;
  };
}