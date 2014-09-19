#pragma once

#include "client.h"
#include "client_queue_thread_loop.h"
#include "server.h"
#include "server_thread_loop.h"

class ClientLoop
{
public:
  ClientLoop(NewsHub::Client* _client, NewsHub::ClientQueueThreadLoop* _threadLoop);
  ~ClientLoop();

  NewsHub::Client* Client();
  NewsHub::ClientQueueThreadLoop* Queue();
  unsigned int nextMessageId();

private:
  NewsHub::Client* client;
  NewsHub::ClientQueueThreadLoop* threadLoop;

  unsigned int messageId;
};

class ServerLoop
{
public:
  ServerLoop(NewsHub::Server* _server, NewsHub::ServerThreadLoop* _threadLoop);
  ~ServerLoop();

private:
  NewsHub::Server* server;
  NewsHub::ServerThreadLoop* threadLoop;
};

struct MessageInfo
{
  NewsHub::ClientQueueThreadLoop* queue;
  unsigned int messageId;
};
