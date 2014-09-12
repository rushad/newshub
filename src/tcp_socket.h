#pragma once

#include "socket.h"

#include "protocol.h"

namespace NewsHub
{
  const int defTcpSocketTimeout = 100;

  class TcpSocket : public Socket
  {
  public:
    explicit TcpSocket(SOCKET _socket);
    ~TcpSocket();

    virtual bool Read(std::string* data);
    virtual bool Write(std::string data);

  private:
    TcpSocket();
    TcpSocket(const TcpSocket &);

    bool waitForData(int msec);
    bool readHeader(PacketHeader* header);
    std::string readData(int len);

    SOCKET socket;

  };
}