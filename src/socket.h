#pragma once

#include "message_info.h"

#include <pthread.h>

#ifdef WIN32
#include <winsock.h>
#endif

namespace NewsHub
{
  class Socket
  {
  public:
    Socket();
    virtual ~Socket();

    virtual bool Read(std::string* data) = 0;
    virtual bool Write(std::string data) = 0;

    bool IsStopped() const;
    void Stop();

  private:
    bool stopped;
    mutable pthread_mutex_t mutexStop;
  };
}