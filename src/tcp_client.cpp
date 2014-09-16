#include "tcp_client.h"
#include "tcp_socket.h"

namespace NewsHub
{
  TcpClient::TcpClient(const std::string & _host, int _port)
    : host(_host),
      port(_port)
  {
  }

  Socket* TcpClient::Connect()
  {
    struct hostent *server = gethostbyname(host.c_str());
    if (!server)
      return 0;

    SOCKET clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      throw std::exception("socket() failed");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
      closesocket(clientSocket);
      return 0;
    }

    return new TcpSocket(clientSocket);  
  }

}