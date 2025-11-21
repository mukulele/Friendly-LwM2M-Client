#include "../websocket/SignalKAccess.h"
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <chrono>

// Helper to generate a random UUID (v4, not RFC-compliant, but good enough for test)
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
    std::string serverUrl = "http://localhost:3000";
    std::string clientId = random_uuid();
    std::string description = "Test C++ WebSocket Client";

    std::cout << "Requesting access from " << serverUrl << " with clientId: " << clientId << std::endl;
    auto hrefOpt = requestSignalKAccess(serverUrl, clientId, description);
    if (!hrefOpt) {
        std::cerr << "Access request failed or not supported by server." << std::endl;
        return 1;
    }
    std::string href = *hrefOpt;
    std::cout << "Access request submitted. Polling status at: " << href << std::endl;

    while (true) {
        auto result = pollSignalKAccess(serverUrl, href);
        if (result.state == SignalKAccessResult::State::Pending) {
            std::cout << "Access request pending. Waiting for approval..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        } else if (result.state == SignalKAccessResult::State::Approved) {
            std::cout << "Access approved! Token: " << result.token << std::endl;
            break;
        } else if (result.state == SignalKAccessResult::State::Denied) {
            std::cout << "Access denied by server." << std::endl;
            break;
        } else {
            std::cerr << "Error: " << result.message << std::endl;
            break;
        }
    }
    return 0;
}
