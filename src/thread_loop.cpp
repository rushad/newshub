#include "thread_loop.h"

#include <exception>
#include <iostream>

namespace NewsHub
{
  ThreadLoop::ThreadLoop()
    : stopped(false)
  {
    pthread_mutex_init(&mutexStop, 0);

    if(pthread_create(&threadId, 0, run, this))
      throw std::exception("pthread_create() failed");
  }

  ThreadLoop::~ThreadLoop()
  {
    stop();
    pthread_join(threadId, 0);
    pthread_mutex_destroy(&mutexStop);
  }

  bool ThreadLoop::isStopped() const
  {
    bool res;
    pthread_mutex_lock(&mutexStop);
    res = stopped;
    pthread_mutex_unlock(&mutexStop);
    return res;
  }

  void ThreadLoop::stop()
  {
    pthread_mutex_lock(&mutexStop);
    stopped = true;
    pthread_mutex_unlock(&mutexStop);
  }

  void* ThreadLoop::run(void* data)
  {
    ThreadLoop* loop = (ThreadLoop*)data;

    while(!loop->isStopped())
    {
      if(!loop->LoopBody())
      {
        break;
      }
    }

    return 0;
  }
}