#pragma once

#include "client.h"

namespace NewsHub
{
  class TcpClient : public Client
  {
  public:
    TcpClient(const std::string & _host, int _port);

    virtual Socket* Connect();

  private:
    TcpClient();
    TcpClient(const Client &);

    std::string host;
    int port;
  };
}