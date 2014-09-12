#include "tcp_socket.h"

#include <io.h>
#include <iostream>

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

  bool TcpSocket::Read(std::string* data)
  {
    if (!waitForData(defTcpSocketTimeout))
      return true;

    PacketHeader header;
    if (!readHeader(&header))
      return false;

    std::string res = readData(header.length());
    if (res == "")
      return false;

    *data = res;
    return true;
  }

  bool TcpSocket::Write(std::string data)
  {
    PacketHeader header(data);

    send(socket, (char*)&header, sizeof(header), 0);
    send(socket, data.c_str(), (int)data.size(), 0);

    return true;
  }

  bool TcpSocket::waitForData(int msec)
  {
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);

    tv.tv_sec = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;

    int res;
    if ((res = select((int)socket + 1, &rfds, 0, 0, &tv)) < 0)
      throw std::exception("select() failed");

    return res ? true : false;
  }

  bool TcpSocket::readHeader(PacketHeader* header)
  {
    int res = recv(socket, (char*)header, sizeof(*header), 0);
    if (res < 0)
      throw std::exception("recv() failed");

    if (!res)
      return false;

    if (res != sizeof(PacketHeader))
      return false;

    if (strcmp(PacketSignature, header->sig))
      return false;

    return true;
  }

  std::string TcpSocket::readData(int len)
  {
    char data[0xffff];
    int read = 0;

    while (read < len)
    {
      if (IsStopped())
        return "";

      if (waitForData(defTcpSocketTimeout))
      {
        int res = recv(socket, data + read, len - read, 0);
        if (res < 0)
          throw std::exception("recv() failed");
        read += res;
      }
    }

    if (read != len)
      return "";

    return std::string(data, read);
  }

}