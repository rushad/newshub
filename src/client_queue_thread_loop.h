#pragma once

#include "thread_loop.h"

#include "client.h"

#include <queue>

namespace NewsHub
{
  class ClientQueueThreadLoop : public ThreadLoop
  {
  public:
    ClientQueueThreadLoop(Client & _client);
    ~ClientQueueThreadLoop();
    
    void AddMessage(const unsigned int messageId, const std::string & message);

    virtual bool LoopBody();

  private:
    Client & client;
    Socket* socket;

    struct Message
    {
      unsigned int id;
      std::string message;
    };

    std::queue<Message> queue;
    mutable pthread_mutex_t mutexQueue;

    bool nextMessage(unsigned int & messageId, std::string & message);
    void popMessage();
  };
}