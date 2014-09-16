#pragma once

#include <pthread.h>

namespace NewsHub
{
  class ThreadLoop
  {
  public:
    ThreadLoop();
    virtual ~ThreadLoop()
    {
    }

  protected:
    virtual bool LoopBody() = 0;
    void Finish();

  private:
    pthread_t threadId;

    bool stopped;
    mutable pthread_mutex_t mutexStop;

    bool isStopped() const;
    void stop();

    static void* run(void* data);

  };
}