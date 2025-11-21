#include "WebSocketClient.h"
#include <ixwebsocket/IXWebSocket.h>
#include <memory>

struct WebSocketClient::Impl {
    ix::WebSocket ws;
    WebSocketClient::MessageCallback onMessage;
};

WebSocketClient::WebSocketClient() : impl(new Impl) {}
WebSocketClient::~WebSocketClient() { close(); delete impl; }

bool WebSocketClient::connect(const std::string& url, const std::string& token) {
    impl->ws.setUrl(url);
    if (!token.empty()) {
        impl->ws.setExtraHeaders({{"Authorization", "Bearer " + token}});
    }
    impl->ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message && impl->onMessage) {
            impl->onMessage(msg->str);
        }
    });
    impl->ws.start();
    return true; // Assume start() always succeeds; add error handling if needed
}

void WebSocketClient::send(const std::string& message) {
    impl->ws.send(message);
}

void WebSocketClient::close() {
    impl->ws.stop();
}

void WebSocketClient::setOnMessage(MessageCallback cb) {
    impl->onMessage = std::move(cb);
}
