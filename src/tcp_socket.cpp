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

  bool TcpSocket::Read(MessageInfo* messageInfo)
  {
    fd_set rfds;
    fd_set efds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);

    FD_ZERO(&efds);
    FD_SET(socket, &efds);

    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    int res = select((int)socket + 1, &rfds, 0, &efds, &tv);
    if (res < 0)
      throw std::exception("select() failed");

    if (!res)
    {
      return true;
    }

    if (FD_ISSET(socket, &efds))
      return false;

    char buf[1024];
    res = recv(socket, buf, sizeof(buf), 0);
    if (res < 0)
      throw std::exception("recv() failed");

    if (!res)
    {
      std::cout << "<EOF>" << std::endl;
      return false;
    }

    messageInfo->message = std::string(buf, res);
    return true;
  }
}