#include "stdafx.h"

#include "serverLoop.h"

ServerLoop::ServerLoop(NewsHub::Server* _server, NewsHub::ServerThreadLoop* _threadLoop)
: server(_server),
  threadLoop(_threadLoop)
{
}

ServerLoop::~ServerLoop()
{
  delete threadLoop;
  delete server;
}
