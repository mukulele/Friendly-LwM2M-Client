
#include "SignalKAccess.h"
#include <ixwebsocket/IXHttpClient.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <thread>
#include <chrono>

using json = nlohmann::json;


// Helper: POST access request
std::optional<std::string> requestSignalKAccess(const std::string& serverUrl, const std::string& clientId, const std::string& description) {
    ix::HttpClient httpClient;
    std::string url = serverUrl + "/signalk/v1/access/requests";
    json req = { {"clientId", clientId}, {"description", description} };
    auto args = std::make_shared<ix::HttpRequestArgs>();
    args->extraHeaders["Content-Type"] = "application/json";
    auto res = httpClient.post(url, req.dump(), args);
    if (res && res->statusCode == 202) {
        auto j = json::parse(res->body);
        if (j.contains("href")) return j["href"].get<std::string>();
    }
    return std::nullopt;
}


SignalKAccessResult pollSignalKAccess(const std::string& serverUrl, const std::string& href) {
    ix::HttpClient httpClient;
    std::string url = serverUrl + href;
    auto args = std::make_shared<ix::HttpRequestArgs>();
    auto res = httpClient.get(url, args);
    SignalKAccessResult result;
    if (res && res->statusCode == 200) {
        auto j = json::parse(res->body);
        if (j.contains("state")) {
            std::string state = j["state"];
            if (state == "PENDING") result.state = SignalKAccessResult::State::Pending;
            else if (state == "COMPLETED") {
                if (j.contains("accessRequest") && j["accessRequest"].contains("permission")) {
                    std::string perm = j["accessRequest"]["permission"];
                    if (perm == "APPROVED") {
                        result.state = SignalKAccessResult::State::Approved;
                        if (j["accessRequest"].contains("token"))
                            result.token = j["accessRequest"]["token"];
                    } else {
                        result.state = SignalKAccessResult::State::Denied;
                    }
                }
            }
        }
    } else {
        result.state = SignalKAccessResult::State::Error;
        result.message = res ? res->body : "No response";
    }
    return result;
}
