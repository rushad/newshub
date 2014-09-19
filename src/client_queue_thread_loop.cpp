#include "client_queue_thread_loop.h"

#include "protocol.h"

#ifndef WIN32
#include <unistd.h>
#endif

namespace NewsHub
{
  ClientQueueThreadLoop::ClientQueueThreadLoop(Client & _client, DeliveryDelegate & _deliveryDelegate, unsigned int _timeout)
    : client(_client),
      deliveryDelegate(_deliveryDelegate),
      timeout(_timeout),
      socket(0)
  {
    pthread_mutex_init(&mutexQueue, 0);
  }

  ClientQueueThreadLoop::~ClientQueueThreadLoop()
  {
    if (socket)
      socket->Stop();
    Finish();
    pthread_mutex_destroy(&mutexQueue);
  }

  void ClientQueueThreadLoop::AddMessage(const unsigned int messageId, const std::string & message)
  {
    Message msgStruct = {messageId, message};

    pthread_mutex_lock(&mutexQueue);
    queue.push(msgStruct);
    pthread_mutex_unlock(&mutexQueue);
  }

  bool ClientQueueThreadLoop::LoopBody()
  {
    unsigned int messageId;
    std::string message;

    if (!nextMessage(messageId, message))
    {
#ifdef WIN32
      Sleep(1);
#else
      sleep(1);
#endif
      return true;
    }

    if (!socket && !(socket = client.Connect()))
      return true;

    if (!socket->Write(messageId, message))
    {
      delete socket;
      socket = 0;
	    return true;
    }

	  unsigned int answerMessageId;
	  std::string answerMessage;
	  if (!socket->Read(answerMessageId, answerMessage, timeout))
		  return true;
	  if (answerMessageId != messageId)
		  return true;
	  if (answerMessage != ConfirmationString)
		  return true;

    popMessage();

    deliveryDelegate.MessageDelivered(this, *socket, messageId, message);
    return true;
  }

  bool ClientQueueThreadLoop::nextMessage(unsigned int & messageId, std::string & message)
  {
    Message msgStruct;

    pthread_mutex_lock(&mutexQueue);
    if (queue.empty())
    {
      pthread_mutex_unlock(&mutexQueue);
      return false;
    }
    msgStruct = queue.front();
    pthread_mutex_unlock(&mutexQueue);

    messageId = msgStruct.id;
    message = msgStruct.message;

    return true;
  }

  void ClientQueueThreadLoop::popMessage()
  {
    pthread_mutex_lock(&mutexQueue);
    queue.pop();
    pthread_mutex_unlock(&mutexQueue);
  }
}