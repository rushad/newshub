#include "socket.h"

#include <iostream>

namespace NewsHub
{
  Socket::Socket()
    : stopped(false)
  {
//    std::cout << "Socket()" << std::endl;
    pthread_mutex_init(&mutexStop, 0);
  }

  Socket::~Socket()
  {
//    std::cout << "~Socket()" << std::endl;
    pthread_mutex_destroy(&mutexStop);
  }

  bool Socket::IsStopped() const
  {
    bool res;
    pthread_mutex_lock(&mutexStop);
    res = stopped;
    pthread_mutex_unlock(&mutexStop);
    return res;
  }

  void Socket::Stop()
  {
    pthread_mutex_lock(&mutexStop);
    stopped = true;
    pthread_mutex_unlock(&mutexStop);
  }
}