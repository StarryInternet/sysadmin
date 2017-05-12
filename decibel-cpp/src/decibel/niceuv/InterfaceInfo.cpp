#include "decibel/niceuv/InterfaceInfo.h"
#include "decibel/ToString.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <uv.h>

namespace decibel
{
namespace niceuv
{
InterfaceInfo::InterfaceInfo(const std::string& ipaddr, uint16_t port) : mInfo()
{
    memset(&mInfo, 0, sizeof(mInfo));
    mInfo.sin_family = AF_INET;
    mInfo.sin_port = htons(port);
    in_addr ip;
    int retval = inet_aton(ipaddr.c_str(), &ip);
    if (retval == 0)
    {
        std::cerr << "Could not parse IP Address: " << ipaddr << std::endl;
    }
    mInfo.sin_addr = ip;
}

InterfaceInfo::InterfaceInfo(const InterfaceInfo& info) : mInfo(info.mInfo)
{
}

std::string InterfaceInfo::GetIpPort() const
{
    return ToString(inet_ntoa(mInfo.sin_addr)) + ":" +
        ToString(ntohs(mInfo.sin_port));
}

const sockaddr_in& InterfaceInfo::GetSockAddr_In() const
{
    return mInfo;
}

std::string GetInterfaceIp(const std::string& interface_name)
{
    uv_interface_address_t* info;
    int num_if = 0;
    // libuv allocates the space for these vars, and they're freed at the end
    uv_interface_addresses(&info, &num_if);

    std::string foundIP("");
    for (int i = 0; i < num_if; i++)
    {
        uv_interface_address_t interface = info[i];

        char* potentialIP = inet_ntoa(interface.address.address4.sin_addr);
        if (strcmp(interface.name, interface_name.c_str()) == 0 &&
            strcmp(potentialIP, "0.0.0.0") != 0)
        {
            foundIP = potentialIP;
            std::cout << "Interface " << interface_name << " has local IP "
                      << foundIP << std::endl;
        }
    }

    if (foundIP == "")
    {
        std::cerr << "Could not find Interface with name: " << interface_name
                  << std::endl;
    }

    uv_free_interface_addresses(info, num_if);

    return foundIP;
}
}
}
