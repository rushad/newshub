#pragma once

#include "client.h"
#include "socket.h"

#include <string>

#include "config.h"

namespace NewsHub
{
  class TcpClient : public Client
  {
  public:
    TcpClient(const std::string & _host, int _port);

    std::string Host() const;
    int Port() const;

    virtual Socket* Connect();

  private:
    TcpClient();

    std::string host;
    int port;
  };
}