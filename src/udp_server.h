#pragma once

#include "server.h"
#include "socket.h"

#include "config.h"

namespace NewsHub
{
  class UdpServer : public Server
  {
  public:
    explicit UdpServer(int _port);

    virtual Socket* Listen();

  private:
    UdpServer();

    int port;

    SOCKET sock;
    struct sockaddr_in serverAddr;
  };
}