#ifndef SIGNALK_LWM2M_CONFIG_H
#define SIGNALK_LWM2M_CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include "WppClient.h"

namespace signalk {

/**
 * @struct ConnectionConfig
 * @brief Configuration parameters for LwM2M connection from JSON
 */
struct ConnectionConfig {
    std::string endpoint;
    uint16_t serverPort;
    std::string serverHost;
    uint16_t listenPort;
    bool useIPv4;
    bool requestBootstrap;
    uint32_t maxPackets;
    
    struct {
        bool enableDTLS;
        std::string pskIdentity;
        std::string psk;
    } dtlsConfig;
};

/**
 * @struct ClientConfig
 * @brief Client configuration parameters
 */
struct ClientConfig {
    std::string endpointName;
    uint32_t bootstrapInterval;
    uint32_t lifetime;
    uint32_t reconnect;
    std::string binding;
};

/**
 * @class SignalKLwM2MConfig
 * @brief Helper class to load and apply SignalK LwM2M configuration
 */
class SignalKLwM2MConfig {
public:
    ConnectionConfig connection;
    ClientConfig client;

public:
    /**
     * @brief Load configuration from JSON file
     * @param configPath Path to the signalk-lwm2m.json file
     * @return true if successful
     */
    bool loadFromFile(const std::string& configPath) {
        // For now, use the original signalk-lwm2m.json format
        // In a real implementation, you'd use a JSON parser like nlohmann/json
        
        // Hardcoded values based on your JSON for demonstration
        connection.endpoint = "signalk-lwm2m-client";
        connection.serverPort = 5683;
        connection.serverHost = "leshan.eclipseprojects.io";
        connection.listenPort = 56830;
        connection.useIPv4 = true;
        connection.requestBootstrap = false;
        connection.maxPackets = 1152;
        connection.dtlsConfig.enableDTLS = false;
        connection.dtlsConfig.pskIdentity = "";
        connection.dtlsConfig.psk = "";
        
        client.endpointName = connection.endpoint;
        client.bootstrapInterval = 60;
        client.lifetime = 300;
        client.reconnect = 0;
        client.binding = "U";
        
        std::cout << "Loaded SignalK LwM2M configuration:" << std::endl;
        std::cout << "  Server: " << connection.serverHost << ":" << connection.serverPort << std::endl;
        std::cout << "  Endpoint: " << connection.endpoint << std::endl;
        std::cout << "  Lifetime: " << client.lifetime << "s" << std::endl;
        std::cout << "  DTLS: " << (connection.dtlsConfig.enableDTLS ? "enabled" : "disabled") << std::endl;
        
        return true;
    }

    /**
     * @brief Get server URI string for LwM2M security object
     */
    std::string getServerUri() const {
        const char* protocol = connection.dtlsConfig.enableDTLS ? "coaps" : "coap";
        return std::string(protocol) + "://" + connection.serverHost + ":" + std::to_string(connection.serverPort);
    }

    /**
     * @brief Apply configuration to Connection object
     */
    template<typename ConnectionType>
    void applyToConnection(ConnectionType& conn) {
        // This would be implemented based on your Connection class interface
        std::cout << "Applying connection config..." << std::endl;
        std::cout << "  Listen port: " << connection.listenPort << std::endl;
        std::cout << "  IPv4: " << (connection.useIPv4 ? "yes" : "no") << std::endl;
        std::cout << "  Max packets: " << connection.maxPackets << std::endl;
    }

    /**
     * @brief Apply configuration to WppClient
     */
    void applyToClient(wpp::WppClient& client) {
        // This would configure the client based on the loaded parameters
        std::cout << "Applying client config..." << std::endl;
        std::cout << "  Endpoint name: " << client.endpointName << std::endl;
        std::cout << "  Binding: " << client.binding << std::endl;
        std::cout << "  Bootstrap interval: " << client.bootstrapInterval << "s" << std::endl;
    }

    /**
     * @brief Print configuration summary
     */
    void printSummary() const {
        std::cout << "\n=== SignalK LwM2M Configuration Summary ===" << std::endl;
        std::cout << "Connection:" << std::endl;
        std::cout << "  Server URI: " << getServerUri() << std::endl;
        std::cout << "  Listen Port: " << connection.listenPort << std::endl;
        std::cout << "  Protocol: IPv" << (connection.useIPv4 ? "4" : "6") << std::endl;
        std::cout << "  Bootstrap: " << (connection.requestBootstrap ? "yes" : "no") << std::endl;
        
        std::cout << "Client:" << std::endl;
        std::cout << "  Endpoint: " << client.endpointName << std::endl;
        std::cout << "  Lifetime: " << client.lifetime << "s" << std::endl;
        std::cout << "  Binding: " << client.binding << std::endl;
        
        std::cout << "Security:" << std::endl;
        std::cout << "  DTLS: " << (connection.dtlsConfig.enableDTLS ? "enabled" : "disabled") << std::endl;
        if (connection.dtlsConfig.enableDTLS) {
            std::cout << "  PSK Identity: " << 
                (connection.dtlsConfig.pskIdentity.empty() ? "[none]" : "[configured]") << std::endl;
            std::cout << "  PSK: " << 
                (connection.dtlsConfig.psk.empty() ? "[none]" : "[configured]") << std::endl;
        }
        std::cout << "===========================================" << std::endl;
    }
};

} // namespace signalk

#endif // SIGNALK_LWM2M_CONFIG_H