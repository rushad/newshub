#pragma once

#include "socket.h"

#include "protocol.h"

#include <string>

#include "config.h"

namespace NewsHub
{
  const int defTcpSocketSlice = 100;

  class TcpSocket : public Socket
  {
  public:
    explicit TcpSocket(SOCKET _socket);
    ~TcpSocket();

    virtual bool Read(unsigned int & messageId, std::string & message, int msec);
    virtual bool Write(const unsigned int messageId, const std::string & message);
    virtual std::string Type() const;

    void SockAddr(std::string & ip, int & port) const;
    void PeerAddr(std::string & ip, int & port) const;

  private:
    TcpSocket();

    bool waitForData(int slice, int msec);
    bool readHeader(PacketHeader & header, int msec);
    std::string readData(int len, bool & ok);

    SOCKET socket;
  };
}