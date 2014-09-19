#include "stdafx.h"

#include "utils.h"

ClientLoop::ClientLoop(NewsHub::Client* _client, NewsHub::ClientQueueThreadLoop* _threadLoop)
  : client(_client),
    threadLoop(_threadLoop),
    messageId(0)
{
}

ClientLoop::~ClientLoop()
{
  delete threadLoop;
  delete client;
}

NewsHub::Client* ClientLoop::Client()
{
  return client;
}

NewsHub::ClientQueueThreadLoop* ClientLoop::Queue()
{
  return threadLoop;
}

unsigned int ClientLoop::nextMessageId()
{
  return ++messageId;
}

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
