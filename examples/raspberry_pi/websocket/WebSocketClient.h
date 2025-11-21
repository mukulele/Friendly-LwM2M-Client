#pragma once
#include <string>
#include <functional>

class WebSocketClient {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    WebSocketClient();
    ~WebSocketClient();

    bool connect(const std::string& url, const std::string& token = "");
    void send(const std::string& message);
    void close();
    void setOnMessage(MessageCallback cb);

private:
    struct Impl;
    Impl* impl;
};
