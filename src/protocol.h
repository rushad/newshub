#pragma once

namespace NewsHub
{
  const char PacketSignature[] = "NH";
  const char ConfirmationString[] = "OK";

#pragma pack(push, 1)
  struct PacketHeader
  {
    char sig[2];
    unsigned short len;
    unsigned int id;
    PacketHeader()
    {
    }
    PacketHeader(unsigned __int32 _id, const std::string & data)
    {
      if (data.size() > 0xffff)
        throw std::exception("Too large packet size");

      memcpy(sig, PacketSignature, sizeof(sig));
      len = htons((int)data.size());
      id = htonl(_id);
    }
    int length()
    {
      return ntohs(len);
    }
    unsigned int messageId()
    {
      return ntohl(id);
    }
  };
#pragma pack(pop)

}