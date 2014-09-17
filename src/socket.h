#pragma once

#include <pthread.h>

#ifdef WIN32
#include <winsock.h>
#endif

#include <string>

namespace NewsHub
{
  class Socket
  {
  public:
    Socket();
    virtual ~Socket();

    virtual bool Read(unsigned int & messageId, std::string & message) = 0;
    virtual bool Write(const unsigned int messageId, const std::string & message) = 0;

	void Stop();

  protected:
    bool IsStopped() const;

  private:
    bool stopped;
    mutable pthread_mutex_t mutexStop;
  };
}