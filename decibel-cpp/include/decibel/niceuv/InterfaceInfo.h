
#pragma once

#include <netinet/in.h>
#include <string>

namespace decibel
{
namespace niceuv
{
class InterfaceInfo
{
public:
    InterfaceInfo(const std::string& ipaddr, uint16_t port);
    InterfaceInfo(const InterfaceInfo& info);

    std::string GetIpPort() const;

    const sockaddr_in& GetSockAddr_In() const;

private:
    struct sockaddr_in mInfo;
};

std::string GetInterfaceIp(const std::string& interface_name);
}
}
