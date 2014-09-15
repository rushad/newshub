#pragma once

#include "thread_loop.h"

#include "news_delegate.h"
#include "socket.h"

namespace NewsHub
{
  class SocketThreadLoop : public ThreadLoop
  {
  public:
    SocketThreadLoop(Socket & _socket, NewsDelegate & _newsDelegate);
    ~SocketThreadLoop();
    virtual bool LoopBody();

  private:
    Socket & socket;
    NewsDelegate & newsDelegate;
  };
}