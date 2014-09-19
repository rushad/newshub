#pragma once

#include "client.h"
#include "socket.h"

#include <string>

#include "config.h"

namespace NewsHub
{
  class UdpClient : public Client
  {
  public:
    UdpClient(const std::string & _host, int _port);

    std::string Host() const;
    int Port() const;

    virtual Socket* Connect();

  private:
    UdpClient();

    std::string host;
    int port;
  };
}