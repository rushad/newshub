#pragma once

#ifdef WIN32

typedef unsigned __int32 UINT32;

#include <exception>
typedef std::exception Error;

#include <winsock2.h>

typedef int socklen_t;

#else

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <string>

typedef unsigned int UINT32;
typedef int SOCKET;

#define closesocket close

class Error : public std::exception
{
  std::string description;
public:
  Error(const std::string & _description) throw()
    : description(_description)
  {
  }
  ~Error() throw()
  {
  }

  char const* what() const throw()
  {
    return description.c_str(); 
  }
};
#endif
