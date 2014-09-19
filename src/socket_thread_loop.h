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
    SocketThreadLoop();
    SocketThreadLoop(const SocketThreadLoop &);

    Socket & socket;
    NewsDelegate & newsDelegate;
  };
}