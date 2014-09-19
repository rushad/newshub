#include "tcp_server.h"

#include "tcp_socket.h"

namespace NewsHub
{
  TcpServer::TcpServer(int port, int backLog)
  {
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      throw std::exception("socket() failed");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
      closesocket(serverSocket);
      throw std::exception("bind() failed");
    }

    if (listen(serverSocket, backLog) < 0)
    {
      closesocket(serverSocket);
      throw std::exception("listen() failed");
    }
  }

  TcpServer::~TcpServer()
  {
    closesocket(serverSocket);
  }

  Socket* TcpServer::Listen()
  {
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(serverSocket, &fds);

    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    int res = select((int)serverSocket + 1, &fds, 0, 0, &tv);
    if (res < 0)
      throw std::exception("select() failed");
    if (!res)
      return 0;

    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    SOCKET connSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrLen);

    return new TcpSocket(connSocket);
  }
}