#pragma once

#include "message_info.h"

#ifdef WIN32
#include <winsock.h>
#endif

namespace NewsHub
{
  class Socket
  {
  public:
    virtual ~Socket()
    {
    }

    virtual bool Read(MessageInfo* messageInfo) = 0;
  };
}