#include "tcp_socket.h"

namespace NewsHub
{
  TcpSocket::TcpSocket(SOCKET _socket)
    : socket(_socket)
  {
  }

  TcpSocket::~TcpSocket()
  {
    closesocket(socket);
  }

  bool TcpSocket::Read(unsigned int & messageId, std::string & message, int msec)
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

  bool TcpSocket::Write(const unsigned int messageId, const std::string & message)
  {
    PacketHeader header(messageId, message);

    if (send(socket, (char*)&header, sizeof(header), 0) < 0)
      return false;
    if (send(socket, message.c_str(), header.length(), 0) < 0)
      return false;

    return true;
  }

  std::string TcpSocket::Type() const
  {
    return "TCP";
  }

  void TcpSocket::SockAddr(std::string & ip, int & port) const
  {
    struct sockaddr_in addr;
    int len = sizeof(addr);
    if (getsockname(socket, (struct sockaddr*)&addr, &len) < 0)
      throw std::exception("getsockname() failed");
    ip = inet_ntoa(addr.sin_addr);
    port = ntohs(addr.sin_port);
  }

  void TcpSocket::PeerAddr(std::string & ip, int & port) const
  {
    struct sockaddr_in addr;
    int len = sizeof(addr);
    if (getpeername(socket, (struct sockaddr*)&addr, &len) < 0)
      throw std::exception("getpeername() failed");
    ip = inet_ntoa(addr.sin_addr);
    port = ntohs(addr.sin_port);
  }

  bool TcpSocket::waitForData(int timeSlice, int msec)
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
        throw std::exception("select() failed");

      if (msec)
      {
        waiting += timeSlice;
        if (waiting >= msec)
          return false;
      }
    } while (!res);

    return true;
  }

  bool TcpSocket::readHeader(PacketHeader & header, int msec)
  {
    if (!waitForData(defTcpSocketSlice, msec))
      return false;

    int res = recv(socket, (char*)&header, sizeof(header), 0);
    if (res < 0)
      throw std::exception("recv() failed");

    if (res != sizeof(PacketHeader))
      return false;

    if (strcmp(PacketSignature, header.sig))
      return false;

    return true;
  }

  std::string TcpSocket::readData(int len, bool & ok)
  {
    char data[0xffff];
    int read = 0;

    ok = false;

    while (read < len)
    {
      if (!waitForData(defTcpSocketSlice, 0))
        return "";

      int res = recv(socket, data + read, len - read, 0);
      if (res < 0)
        throw std::exception("recv() failed");

      read += res;

    }

    if (read != len)
      return "";

    ok = true;
    return std::string(data, read);
  }
}