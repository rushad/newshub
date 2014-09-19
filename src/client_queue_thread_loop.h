#pragma once

#include "thread_loop.h"

#include "client.h"
#include "delivery_delegate.h"

#include <queue>

#include "config.h"

namespace NewsHub
{
  const unsigned int defTimeout = 1000;

  class ClientQueueThreadLoop : public ThreadLoop
  {
  public:
    ClientQueueThreadLoop(Client & _client, DeliveryDelegate & _deliveryDelegate, unsigned int _timeout = defTimeout);
    ~ClientQueueThreadLoop();
    
    void AddMessage(const unsigned int messageId, const std::string & message);

    virtual bool LoopBody();

  private:
    ClientQueueThreadLoop();
    ClientQueueThreadLoop(const ClientQueueThreadLoop &);

    Client & client;
    DeliveryDelegate & deliveryDelegate;
    unsigned int timeout;

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