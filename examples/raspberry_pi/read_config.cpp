std::string getClientNameFromConfig() {
	std::ifstream file("signalk-lwm2m.json");
	if (!file) {
		throw std::runtime_error("Could not open signalk-lwm2m.json");
	}
	nlohmann::json j;
	file >> j;
	auto& params = j["configuration"]["connectionParams"];
	return params["endpoint"];
}
#include "read_config.h"
#include <fstream>
#include <string>
#include <memory>
#include "rpi_libs/includes/nlohmann/json.hpp"
#include <netinet/in.h>

// For DTLS, include the DTLS Connection header if needed
// #include "../shared/platform/connection_dtls/Connection.h"
#include "../shared/platform/connection/Connection.h"

std::unique_ptr<Connection> createConnectionFromConfig() {
    std::ifstream file("signalk-lwm2m.json");
    if (!file) {
        throw std::runtime_error("Could not open signalk-lwm2m.json");
    }
    nlohmann::json j;
    file >> j;
    auto& params = j["configuration"]["connectionParams"];
    std::string port = std::to_string(params["listenPort"]);
    bool useIPv4 = params["useIPv4"];
    bool enableDTLS = params["dtlsConfig"]["enableDTLS"];

    int addressFamily = useIPv4 ? AF_INET : AF_INET6;

    // If you want to support DTLS, you can add logic here to instantiate the DTLS Connection class
    // For now, we instantiate the UDP Connection
    // If you have a DTLS Connection class, you can do:
    // if (enableDTLS) return std::make_unique<ConnectionDtls>(port, addressFamily);
    // else
    return std::make_unique<Connection>(port, addressFamily);
}
/*
int main() {
	std::ifstream file("signalk-lwm2m.json");
	if (!file) {
		std::cerr << "Could not open signalk-lwm2m.json" << std::endl;
		return 1;
	}
	nlohmann::json j;
	file >> j;

	auto& params = j["configuration"]["connectionParams"];
	std::string endpoint = params["endpoint"];      // LwM2M client endpoint name (Client ID)
	std::string host = params["serverHost"];        // LwM2M server address (host part of URI)
	int port = params["serverPort"];                // LwM2M server port (port part of URI)
	int listenPort = params["listenPort"];          // Local UDP port to bind (if your connection class supports it)
	bool useIPv4 = params["useIPv4"];               // Use IPv4 for socket (if your connection class supports it)
	bool bootstrap = params["request_bootstrap"];   // If true, use LwM2M bootstrap mode
	bool enableDTLS = params["dtlsConfig"]["enableDTLS"];       // If true, use DTLS (WppDtlsConnection); else, use UDP (WppUdpConnection)
	std::string pskId = params["dtlsConfig"]["PSK_Identity"];   // DTLS PSK identity (for Lwm2mSecurity, if DTLS is enabled)
	std::string psk = params["dtlsConfig"]["PSK"];              // DTLS PSK key (for Lwm2mSecurity, if DTLS is enabled)

	std::string uri = (enableDTLS ? "coaps://" : "coap://") + host + ":" + std::to_string(port);

	std::cout << "Endpoint: " << endpoint << std::endl;
	std::cout << "Server URI: " << uri << std::endl;
	std::cout << "Listen Port: " << listenPort << std::endl;
	std::cout << "Use IPv4: " << (useIPv4 ? "true" : "false") << std::endl;
	std::cout << "Bootstrap: " << (bootstrap ? "true" : "false") << std::endl;
	std::cout << "DTLS Enabled: " << (enableDTLS ? "true" : "false") << std::endl;
	std::cout << "PSK Identity: " << pskId << std::endl;
	std::cout << "PSK: " << psk << std::endl;

	// Here you would use these values to configure your WppConnection and Lwm2mSecurity
	return 0;
}
*/