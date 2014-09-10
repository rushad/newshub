#pragma once

#include "server.h"

namespace NewsHub
{
  class UdpServer : public Server
  {
  public:
    UdpServer(const NewsDelegate & newsDelegate, int port);

  private:
    UdpServer();
    UdpServer(const UdpServer&);
  };
}