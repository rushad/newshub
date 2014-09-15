#pragma once

#include "server.h"
#include "server_thread_loop.h"

class ServerLoop
{
public:
  ServerLoop(NewsHub::Server* _server, NewsHub::ServerThreadLoop* _threadLoop);
  ~ServerLoop();

private:
  NewsHub::Server* server;
  NewsHub::ServerThreadLoop* threadLoop;
};