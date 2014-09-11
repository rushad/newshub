#include <iostream>

#include "message_info.h"
#include "server_thread_loop.h"
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
    Sleep(100000);
    std::cout << "About to finish app..." << std::endl;
  }
  catch (const std::exception & e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return 0;
}