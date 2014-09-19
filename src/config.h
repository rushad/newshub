#pragma once

#ifdef WIN32

typedef unsigned __int32 UINT 32;
typedef std::exception Error;

#else

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

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
