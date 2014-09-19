#include "udp_client.h"

#include "udp_socket.h"

namespace NewsHub
{
  UdpClient::UdpClient(const std::string & _host, int _port)
    : host(_host),
      port(_port)
  {
  }

  std::string UdpClient::Host() const
  {
    return host;
  }

  int UdpClient::Port() const
  {
    return port;
  }

  Socket* UdpClient::Connect()
  {
    struct hostent *server = gethostbyname(host.c_str());
    if (!server)
      return 0;

    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      throw std::exception("socket() failed");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serverAddr.sin_port = htons(port);

    return new UdpSocket(sock, serverAddr);  
  }

}