#pragma once
#include <string>
#include <vector>

struct SignalKService {
    std::string host;
    int port;
    std::string roles;
    std::string self;
    std::string swname;
    std::string swvers;
};

// Discover Signal K servers using mDNS/DNS-SD
std::vector<SignalKService> discoverSignalKServers();
