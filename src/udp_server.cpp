#include "udp_server.h"

#include "udp_socket.h"

namespace NewsHub
{
  UdpServer::UdpServer(int _port)
    : port(_port)
  {
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      throw std::exception("socket() failed");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
      closesocket(sock);
      throw std::exception("bind() failed");
    }
  }

  Socket* UdpServer::Listen()
  {
    return new UdpSocket(sock, serverAddr);
  }
}