#include "socket_thread_loop.h"

#include "protocol.h"

namespace NewsHub
{
  SocketThreadLoop::SocketThreadLoop(Socket & _socket, NewsDelegate & _newsDelegate)
    : socket(_socket),
      newsDelegate(_newsDelegate)
  {
  }

  SocketThreadLoop::~SocketThreadLoop()
  {
    socket.Stop();
    Finish();
  }

  bool SocketThreadLoop::LoopBody()
  {
    unsigned int messageId;
    std::string res;

    if (!socket.Read(messageId, res))
      return false;

    if (newsDelegate.Message(socket, messageId, res))
    {
	    if (!socket.Write(messageId, ConfirmationString))
		    return true;
    }

    return true;
  }
}