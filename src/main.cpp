#include <iostream>

#include "message_info.h"

int main(int argc, char* argv[])
{
  NewsHub::MessageInfo mi(NewsHub::TimeStamp(), "hello");
  std::cout << "hello" << std::endl;
  return 0;
}