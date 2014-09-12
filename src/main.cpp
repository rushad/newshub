#include <iostream>

#include "message_info.h"
#include "server_thread_loop.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_socket.h"

#ifdef WIN32
#include <winsock.h>
#endif

int main(int argc, char* argv[])
{
#ifdef WIN32
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

  try
  {
    NewsHub::TcpServer server1(12345);
    NewsHub::TcpServer server2(12346);
    NewsHub::TcpServer server3(12347);
    NewsHub::ServerThreadLoop tcpServerLoop1(server1);
    NewsHub::ServerThreadLoop tcpServerLoop2(server2);
    NewsHub::ServerThreadLoop tcpServerLoop3(server3);

    Sleep(1000);

    NewsHub::Socket* client1 = NewsHub::TcpClient("localhost", 12345).Connect();
    NewsHub::Socket* client2 = NewsHub::TcpClient("localhost", 12346).Connect();
    NewsHub::Socket* client3 = NewsHub::TcpClient("localhost", 12347).Connect();

    std::string msg;
    std::cin >> msg;
    client1->Write(msg);
    std::cin >> msg;
    client2->Write(msg);
    std::cin >> msg;
    client3->Write(msg);

    Sleep(10000);
    std::cout << "About to finish app..." << std::endl;
    delete client1;
    delete client2;
    delete client3;
  }
  catch (const std::exception & e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return 0;
}