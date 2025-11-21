#include "../websocket/JsonConfig.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include "wpp/client/WppClient.h"
#include "wpp/platform/connection/WppConnection.h"
#include "wpp/registry/objects/m_0_lwm2m_security/Lwm2mSecurity.h"
#include "wpp/registry/data_model/types/WppTypes.h"

using namespace wpp;


#include "../websocket/JsonConfig.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include "wpp/client/WppClient.h"
#include "wpp/platform/connection/WppConnection.h"
#include "wpp/registry/objects/m_0_lwm2m_security/Lwm2mSecurity.h"
#include "wpp/registry/data_model/types/WppTypes.h"

// Helper: convert string to OPAQUE_T (vector<uint8_t>)

static wpp::OPAQUE_T toOpaque(const std::string& s) {
    return wpp::OPAQUE_T(s.begin(), s.end());
}


int main(int argc, char* argv[]) {
    // Load configuration at the very start

    JsonConfig config;
    if (!config.load("../src/signalk-lwm2m.json")) {
        std::cerr << "Failed to load configuration file." << std::endl;
        return 1;
    }

    // Extract connectionParams from the loaded JSON
    nlohmann::json root;
    {
        std::ifstream in("../src/signalk-lwm2m.json");
        in >> root;
    }
    auto connectionParams = root["configuration"]["connectionParams"];
    if (connectionParams.is_null()) {
        std::cerr << "connectionParams missing in config." << std::endl;
        return 1;
    }

    std::string endpoint = connectionParams.value("endpoint", "");
    std::string serverHost = connectionParams.value("serverHost", "");
    int serverPort = connectionParams.value("serverPort", 5683);
    bool request_bootstrap = connectionParams.value("request_bootstrap", false);
    auto dtlsConfig = connectionParams["dtlsConfig"];
    bool enableDTLS = dtlsConfig.value("enableDTLS", false);
    std::string pskIdentity = dtlsConfig.value("PSK_Identity", "");
    std::string pskKey = dtlsConfig.value("PSK", "");

    // Print config for debug
    std::cout << "endpoint: " << endpoint << std::endl;
    std::cout << "serverHost: " << serverHost << std::endl;
    std::cout << "serverPort: " << serverPort << std::endl;
    std::cout << "DTLS: " << (enableDTLS ? "enabled" : "disabled") << std::endl;

    // --- LwM2M connection setup ---
    // 1. Create WppClient and WppConnection (pseudo, replace with your actual connection impl)
    // You may need to implement a concrete WppConnection subclass if not present

    // DummyConnection for demonstration (replace with your real connection class)

    struct DummyConnection : public wpp::WppConnection {
        wpp::WppConnection::SESSION_T connect(wpp::Lwm2mSecurity& security) override {
            std::cout << "[DummyConnection] Simulating connection..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return reinterpret_cast<wpp::WppConnection::SESSION_T>(this); // Dummy session
        }
        void disconnect(wpp::WppConnection::SESSION_T session) override {
            std::cout << "[DummyConnection] Disconnected." << std::endl;
        }
    };

    DummyConnection connection;
    wpp::WppClient wppClient(connection, nullptr);

    // 2. Create Lwm2mSecurity instance
    auto* sec = wpp::Lwm2mSecurity::createInst(wppClient, 0);
    if (!sec) {
        std::cerr << "Failed to create Lwm2mSecurity instance." << std::endl;
        return 1;
    }

    // 3. Set security resources from config
    std::string serverUri = "coap";
    serverUri += enableDTLS ? "s" : "";
    serverUri += "://" + serverHost + ":" + std::to_string(serverPort);
    sec->resource(wpp::Lwm2mSecurity::LWM2M_SERVER_URI_0)->set<wpp::STRING_T>(serverUri);
    sec->resource(wpp::Lwm2mSecurity::BOOTSTRAP_SERVER_1)->set<wpp::BOOL_T>(request_bootstrap);
    sec->resource(wpp::Lwm2mSecurity::SECURITY_MODE_2)->set<wpp::INT_T>(enableDTLS ? 0 : 3); // 0=PSK, 3=None
    sec->resource(wpp::Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3)->set<wpp::OPAQUE_T>(toOpaque(pskIdentity));
    sec->resource(wpp::Lwm2mSecurity::SECRET_KEY_5)->set<wpp::OPAQUE_T>(toOpaque(pskKey));
    sec->resource(wpp::Lwm2mSecurity::SHORT_SERVER_ID_10)->set<wpp::INT_T>(1); // Example server ID

    // 4. Establish connection and wait until ready
    std::cout << "Establishing LwM2M connection..." << std::endl;
    auto session = connection.connect(*sec);
    if (!session) {
        std::cerr << "Failed to establish LwM2M connection." << std::endl;
        return 1;
    }
    std::cout << "LwM2M connection established!" << std::endl;

    return 0;
}
} // namespace wpp

    // ...continue with application logic...
    return 0;
}
