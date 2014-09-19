#pragma once

#include "socket.h"

#include "protocol.h"

#include <string>

#include "config.h"

namespace NewsHub
{
  const int defUdpSocketSlice = 100;

  class UdpSocket : public Socket
  {
  public:
    explicit UdpSocket(SOCKET _socket, const struct sockaddr_in & _peerAddr);
    ~UdpSocket();

    virtual bool Read(unsigned int & messageId, std::string & message, int msec);
    virtual bool Write(const unsigned int messageId, const std::string & message);
    virtual std::string Type() const;

  private:
    UdpSocket();

    bool waitForData(int slice, int msec);
    bool readHeader(PacketHeader & header, int msec);
    std::string readData(int len, bool & ok);

    SOCKET socket;
    struct sockaddr_in peerAddr;
  };
}