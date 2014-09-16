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

    virtual bool Read(unsigned int & messageId, std::string & message);
    virtual bool Write(const unsigned int messageId, const std::string & message);

    void SockAddr(std::string & ip, int & port) const;
    void PeerAddr(std::string & ip, int & port) const;

  private:
    TcpSocket();
    TcpSocket(const TcpSocket &);

    bool waitForData(int msec);
    bool readHeader(PacketHeader & header);
    std::string readData(int len, bool & ok);

    SOCKET socket;

  };
}