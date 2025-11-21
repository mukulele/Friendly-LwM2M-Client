#include "../websocket/SignalKAccess.h"
#include "../websocket/WebSocketClient.h"
#include "../websocket/JsonConfig.h"
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <chrono>

std::string random_uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::stringstream ss;
    for (int i = 0; i < 32; ++i) {
        ss << std::hex << dis(gen);
        if (i == 7 || i == 11 || i == 15 || i == 19) ss << "-";
    }
    return ss.str();
}


int main() {
    JsonConfig config;
    config.load("../examples/test_config.json");
    std::string serverUrl = config.get<std::string>("serverUrl").value_or("http://localhost:3000");
    std::string wsUrl = config.get<std::string>("wsUrl").value_or("ws://localhost:3000/signalk/v1/stream");
    std::string clientId = config.get<std::string>("clientId").value_or(random_uuid());
    std::string description = config.get<std::string>("description").value_or("C++ WebSocket Stream Test");
    std::string token;

    // Request access and poll for approval
    auto hrefOpt = requestSignalKAccess(serverUrl, clientId, description);
    if (!hrefOpt) {
        std::cerr << "Access request failed or not supported by server." << std::endl;
        return 1;
    }
    std::string href = *hrefOpt;
    while (true) {
        auto result = pollSignalKAccess(serverUrl, href);
        if (result.state == SignalKAccessResult::State::Pending) {
            std::cout << "Access request pending. Waiting for approval..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        } else if (result.state == SignalKAccessResult::State::Approved) {
            std::cout << "Access approved! Token: " << result.token << std::endl;
            token = result.token;
            break;
        } else if (result.state == SignalKAccessResult::State::Denied) {
            std::cout << "Access denied by server." << std::endl;
            return 1;
        } else {
            std::cerr << "Error: " << result.message << std::endl;
            return 1;
        }
    }

    // Connect to WebSocket with token
    WebSocketClient wsClient;
    wsClient.setOnMessage([](const std::string& msg) {
        std::cout << "[WS] " << msg << std::endl;
    });
    std::string wsUrlWithToken = wsUrl + "?token=" + token;
    std::cout << "Connecting to " << wsUrlWithToken << std::endl;
    if (!wsClient.connect(wsUrlWithToken)) {
        std::cerr << "WebSocket connection failed." << std::endl;
        return 1;
    }
    std::cout << "WebSocket connected. Listening for messages... (Press Ctrl+C to exit)" << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
