#include "message_info.h"

namespace NewsHub
{
  MessageInfo::MessageInfo(const TimeStamp & _publicationTime, const std::string & _message)
    : publicationTime(_publicationTime),
      message(_message)
  {
  }

  MessageInfo::MessageInfo(const MessageInfo & messageInfo)
    : publicationTime(messageInfo.publicationTime),
    message(messageInfo.message)
  {
  }

  const TimeStamp & MessageInfo::PublicationTime() const
  {
    return publicationTime;
  }

  const std::string & MessageInfo::Message() const
  {
    return message;
  }
}