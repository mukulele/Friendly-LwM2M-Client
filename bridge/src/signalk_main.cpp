#include <iostream>
#include <thread>
#include <chrono>

#include "Connection.h"
#include "objects.h"
#include "SignalKLwM2MConfig.h"

using namespace std;
using namespace wpp;

void socketPolling(Connection *connection) {
    while (!isDeviceShouldBeRebooted()) {
        connection->loop();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void wppErrorHandler(WppClient &client, int errCode) {
    cout << "wppErrorHandler(): Error: " << errCode << endl;
    if (client.getState() == STATE_BOOTSTRAPPING || client.getState() == STATE_BOOTSTRAP_REQUIRED) {
        cout << "Trying to restore security and server objects" << endl;
        Object &securityObj = Lwm2mSecurity::object(client);
        Object &serverObj = Lwm2mServer::object(client);

        #if OBJ_O_2_LWM2M_ACCESS_CONTROL
        for (auto *inst : securityObj.instances()) Lwm2mAccessControl::remove(*inst);
        for (auto *inst : serverObj.instances()) Lwm2mAccessControl::remove(*inst);
        #endif

        securityObj.clear();
        serverObj.clear();

        securityInit(client);
        serverInit(client);
    }
}

int main() {
    cout << endl << "=== SignalK LwM2M Client Starting ===" << endl;
    
    // Load SignalK configuration
    signalk::SignalKLwM2MConfig config;
    if (!config.loadFromFile("./signalk-lwm2m.json")) {
        cout << "Failed to load configuration!" << endl;
        return 1;
    }
    
    config.printSummary();
    
    cout << endl << "---- Creating required components ----" << endl;
    
    // Create connection with configuration
    Connection connection(to_string(config.connection.listenPort), 
                         config.connection.useIPv4 ? AF_INET : AF_INET6);
    
    // Apply connection configuration
    config.applyToConnection(connection);

    // Client initialization with configured endpoint name
    cout << endl << "---- Creating WppClient ----" << endl;
    string clientName = config.client.endpointName;
    
    #if DTLS_WITH_PSK
    clientName += "PSK";
    #elif DTLS_WITH_RPK  
    clientName += "RPK";
    #endif
    
    cout << "WppClient name: " << clientName << endl;
    
    WppClient::create({clientName, "", ""}, connection, wppErrorHandler);
    WppClient *client = WppClient::takeOwnershipBlocking();

    // Apply client configuration
    config.applyToClient(*client);

    // Initialize wpp objects with configuration-aware initialization
    #ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
    acInit(*client);
    #endif
    
    cout << endl << "---- Initialization wpp Server ----" << endl;
    serverInit(*client);
    
    cout << endl << "---- Initialization wpp Security ----" << endl;
    securityInit(*client);
    
    cout << endl << "---- Initialization wpp Device ----" << endl;
    deviceInit(*client);
    
    #ifdef OBJ_O_5_FIRMWARE_UPDATE
    cout << endl << "---- Initialization wpp FirmwareUpdate ----" << endl;
    fwUpdaterInit(*client);
    #endif
    
    #ifdef OBJ_O_4_CONNECTIVITY_MONITORING
    cout << endl << "---- Initialization wpp ConnectivityMonitoring ----" << endl;
    connMonitoringInit(*client);
    #endif
    
    #ifdef OBJ_O_3339_AUDIO_CLIP
    cout << endl << "---- Initialization wpp AudioClip ----" << endl;
    audioClipInit(*client);
    #endif

    // Configure security object with server URI from configuration
    cout << endl << "---- Configuring LwM2M Security Object ----" << endl;
    Object& securityObj = Lwm2mSecurity::object(*client);
    if (!securityObj.instances().empty()) {
        Instance* securityInst = securityObj.instances().front();
        string serverUri = config.getServerUri();
        cout << "Setting server URI: " << serverUri << endl;
        securityInst->set<STRING_T>(Lwm2mSecurity::LWM2M_SERVER_URI_0, serverUri);
        
        // Configure security mode based on DTLS settings
        if (config.connection.dtlsConfig.enableDTLS) {
            securityInst->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, 0); // PSK mode
            if (!config.connection.dtlsConfig.pskIdentity.empty()) {
                securityInst->set<OPAQUE_T>(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, 
                    config.connection.dtlsConfig.pskIdentity);
            }
            if (!config.connection.dtlsConfig.psk.empty()) {
                securityInst->set<OPAQUE_T>(Lwm2mSecurity::SECRET_KEY_5,
                    config.connection.dtlsConfig.psk);
            }
        } else {
            securityInst->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, 3); // NoSec mode
        }
    }

    // Configure server object with lifetime from configuration
    cout << endl << "---- Configuring LwM2M Server Object ----" << endl;
    Object& serverObj = Lwm2mServer::object(*client);
    if (!serverObj.instances().empty()) {
        Instance* serverInst = serverObj.instances().front();
        cout << "Setting lifetime: " << config.client.lifetime << "s" << endl;
        serverInst->set<TIME_T>(Lwm2mServer::LIFETIME_1, config.client.lifetime);
        serverInst->set<STRING_T>(Lwm2mServer::BINDING_7, config.client.binding);
    }
    
    // Giving ownership to registry
    client->giveOwnership();

    // Add SignalK-specific periodic tasks
    WppTaskQueue::addTask(10, [](WppClient &client, void *ctx) {
        cout << "SignalK periodic task: Updating navigation data" << endl;
        // Here you would update navigation-related resources
        // from SignalK data via the Node.js bridge
        return true; // Repeat task
    });

    cout << endl << "---- Starting Connection thread ----" << endl;
    thread my_thread(socketPolling, &connection);

    cout << endl << "---- SignalK LwM2M Client Ready ----" << endl;
    cout << "Connect to: " << config.getServerUri() << endl;
    cout << "Endpoint: " << config.client.endpointName << endl;
    cout << "Node.js Bridge: http://localhost:3000" << endl;

    time_t callTime = 0;
    for (int iterationCnt = 0; !isDeviceShouldBeRebooted(); iterationCnt++) {
        time_t currTime = time(NULL);

        if (iterationCnt % 10 == 0) {
            cout << "---- iteration:" << iterationCnt << ", time: " << time(NULL) << " ----" << endl;
        }
        
        if (currTime >= callTime || connection.getPacketQueueSize()) {
            // Handle client state and process packets from the server
            client = WppClient::takeOwnership();
            if (client) { 
                callTime = currTime + client->loop();
                client->giveOwnership();
                
                if (iterationCnt % 10 == 0) {
                    cout << "Sleep time: " << callTime - time(NULL) << "s" << endl;
                    cout << "Client state: " << client->getState() << endl;
                }
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << endl << "---- Closing SignalK LwM2M Client ----" << endl;
    my_thread.join();
    WppClient::remove();
    
    return 0;
}