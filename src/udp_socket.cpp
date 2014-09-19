#include "udp_socket.h"

namespace NewsHub
{
  UdpSocket::UdpSocket(SOCKET _socket, const struct sockaddr_in & _peerAddr)
    : socket(_socket),
      peerAddr(_peerAddr)
  {
  }

  UdpSocket::~UdpSocket()
  {
    closesocket(socket);
  }

  bool UdpSocket::Read(unsigned int & messageId, std::string & message, int msec)
  {
    PacketHeader header;
    if (!readHeader(header, msec))
      return false;

    bool ok;
    std::string res = readData(header.length(), ok);
    if (!ok)
      return false;

    messageId = header.messageId();
    message = res;
    return true;
  }

  bool UdpSocket::Write(const unsigned int messageId, const std::string & message)
  {
    PacketHeader header(messageId, message);

    if (sendto(socket, (char*)&header, sizeof(header), 0, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) < 0)
      return false;
    if (sendto(socket, message.c_str(), header.length(), 0, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) < 0)
      return false;

    return true;
  }

  std::string UdpSocket::Type() const
  {
    return "UDP";
  }

  bool UdpSocket::waitForData(int timeSlice, int msec)
  {
    fd_set rfds;
    struct timeval tv;

    int waiting = 0;
    int res;
    do
    {
      if (IsStopped())
        return false;

      FD_ZERO(&rfds);
      FD_SET(socket, &rfds);

      tv.tv_sec = timeSlice / 1000;
      tv.tv_usec = (timeSlice % 1000) * 1000;

      if ((res = select((int)socket + 1, &rfds, 0, 0, &tv)) < 0)
        throw Error("select() failed");

      if (msec)
      {
        waiting += timeSlice;
        if (waiting >= msec)
          return false;
      }
    } while (!res);

    return true;
  }

  bool UdpSocket::readHeader(PacketHeader & header, int msec)
  {
    if (!waitForData(defUdpSocketSlice, msec))
      return false;

    socklen_t fromLen = sizeof(peerAddr);
    int res = recvfrom(socket, (char*)&header, sizeof(header), 0, (struct sockaddr*)&peerAddr, &fromLen);
    if (res < 0)
      throw Error("recvfrom() failed");

    if (res != sizeof(PacketHeader))
      return false;

    if (strcmp(PacketSignature, header.sig))
      return false;

    return true;
  }

  std::string UdpSocket::readData(int len, bool & ok)
  {
    char data[0xffff];
    int read = 0;

    ok = false;

    while (read < len)
    {
      if (!waitForData(defUdpSocketSlice, 0))
        return "";

      int res = recvfrom(socket, data + read, len - read, 0, 0, 0);
      if (res < 0)
        throw Error("recvfrom() failed");

      read += res;

    }

    if (read != len)
      return "";

    ok = true;
    return std::string(data, read);
  }

}