#include "client_queue_thread_loop.h"

#include <iostream>

namespace NewsHub
{
  ClientQueueThreadLoop::ClientQueueThreadLoop(Client & _client)
    : client(_client),
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

    std::cout << "Added " << messageId << ": " << message << std::endl;
  }

  bool ClientQueueThreadLoop::LoopBody()
  {
    unsigned int messageId;
    std::string message;

    if (!nextMessage(messageId, message))
      return true;

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
	if (!socket->Read(answerMessageId, answerMessage))
		return true;
	if (answerMessageId != messageId)
		return true;
	if (answerMessage != "OK")
		return true;

    popMessage();

    std::cout << "Sent " << messageId << ": " << message << std::endl;
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

    std::cout << "Fetched " << messageId << ": " << message << std::endl;
    return true;
  }

  void ClientQueueThreadLoop::popMessage()
  {
    pthread_mutex_lock(&mutexQueue);
    queue.pop();
    pthread_mutex_unlock(&mutexQueue);
  }

}