#pragma once

#include "socket.h"

#include <string>

namespace NewsHub
{
  class ClientQueueThreadLoop;
  class DeliveryDelegate
  {
  public:
    virtual ~DeliveryDelegate()
    {
    }

    virtual void MessageDelivered(ClientQueueThreadLoop* queue, const Socket & socket, const unsigned int messageId, const std::string & message) = 0;
  };
}