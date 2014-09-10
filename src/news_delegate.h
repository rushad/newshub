#pragma once

#include "message_info.h"

namespace NewsHub
{
  class NewsDelegate
  {
  public:
    virtual ~NewsDelegate()
    {
    }

    void Message(const MessageInfo & messageInfo) = 0;
  };
}