#pragma once

#include "socket.h"

namespace NewsHub
{
  class TcpSocket : public Socket
  {
  public:
    explicit TcpSocket(SOCKET _socket);
    ~TcpSocket();

    virtual bool Read(MessageInfo* messageInfo);

  private:
    TcpSocket();
    TcpSocket(const TcpSocket &);

    SOCKET socket;
  };
}