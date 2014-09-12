#pragma once

namespace NewsHub
{
  const char PacketSignature[] = "NH";

#pragma pack(push, 1)
  struct PacketHeader
  {
    char sig[2];
    unsigned short len;
    PacketHeader()
    {
    }
    PacketHeader(const std::string & data)
    {
      if (data.size() > 0xffff)
        throw std::exception("Too large packet size");

      memcpy(sig, PacketSignature, sizeof(sig));
      len = htons((short)data.size());
    }
    int length()
    {
      return ntohs(len);
    }
  };
#pragma pack(pop)

}