#pragma once

#include "server.h"
#include "socket.h"

#include "config.h"

namespace NewsHub
{
  const int defBackLog = 100;

  class TcpServer : public Server
  {
  public:
    explicit TcpServer(int _port, int backLog = defBackLog);
    ~TcpServer();

    virtual Socket* Listen();

  private:
    TcpServer();

    SOCKET serverSocket;
  };
}