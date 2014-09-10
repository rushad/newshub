#pragma once

#include <string>

#include "time_stamp.h"

namespace NewsHub
{
  class MessageInfo
  {
  public:
    MessageInfo(const TimeStamp & _publicationTime, const std::string & _message);
    MessageInfo(const MessageInfo & messageInfo);

    const TimeStamp & PublicationTime() const;
    const std::string & Message() const;

  private:
    MessageInfo();
    TimeStamp publicationTime;
    std::string message;

  };
}