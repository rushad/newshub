#pragma once

#include <pthread.h>

#include <string>

#include "config.h"

namespace NewsHub
{
  class Socket
  {
  public:
    Socket();
    virtual ~Socket();

    virtual bool Read(unsigned int & messageId, std::string & message, int msec = 0) = 0;
    virtual bool Write(const unsigned int messageId, const std::string & message) = 0;
    virtual std::string Type() const = 0;

	  void Stop();
    bool IsStopped() const;

  private:
    bool stopped;
    mutable pthread_mutex_t mutexStop;
  };
}