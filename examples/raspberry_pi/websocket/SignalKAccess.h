#pragma once
#include <string>
#include <optional>

// Result of an access request
struct SignalKAccessResult {
    enum class State { Pending, Approved, Denied, Error } state;
    std::string token;
    std::string href;
    std::string message;
};

// Request access to a Signal K server. Returns href for polling status.
std::optional<std::string> requestSignalKAccess(const std::string& serverUrl, const std::string& clientId, const std::string& description);

// Poll the access request status. Returns token if approved.
SignalKAccessResult pollSignalKAccess(const std::string& serverUrl, const std::string& href);
