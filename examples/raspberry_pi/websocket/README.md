# WebSocket Signal K Client C++ Interface Documentation

This module provides a minimal, robust C++ interface for connecting to Signal K servers using WebSockets, including access request and token-based authentication. It is built on [ixwebsocket](https://github.com/machinezone/IXWebSocket) and [nlohmann_json](https://github.com/nlohmann/json).

## Main Classes and Functions

### 1. `WebSocketClient`
A simple, reusable C++ WebSocket client abstraction.

**Header:** `websocket/WebSocketClient.h`

#### Methods
- `WebSocketClient()` — Constructor
- `~WebSocketClient()` — Destructor
- `bool connect(const std::string& url, const std::string& token = "")`
  - Connect to a WebSocket server. Optionally provide a Bearer token for authentication.
  - Returns `true` if connection is initiated.
- `void send(const std::string& message)`
  - Send a message to the server.
- `void close()`
  - Close the WebSocket connection.
- `void setOnMessage(MessageCallback cb)`
  - Set a callback to handle incoming messages. `MessageCallback` is `std::function<void(const std::string&)>`.

#### Example
```cpp
WebSocketClient ws;
ws.setOnMessage([](const std::string& msg) {
    std::cout << msg << std::endl;
});
ws.connect("ws://localhost:3000/signalk/v1/stream", token);
```

---

### 2. Signal K Access Request

**Header:** `websocket/SignalKAccess.h`

#### Types
- `struct SignalKAccessResult`
  - `enum class State { Pending, Approved, Denied, Error } state;`
  - `std::string token;` — Access token if approved
  - `std::string href;` — Polling URL
  - `std::string message;` — Error or status message

#### Functions
- `std::optional<std::string> requestSignalKAccess(const std::string& serverUrl, const std::string& clientId, const std::string& description);`
  - Initiate an access request. Returns a polling `href` if successful.
- `SignalKAccessResult pollSignalKAccess(const std::string& serverUrl, const std::string& href);`
  - Poll the access request status. Returns a `SignalKAccessResult` with state and token if approved.

---

### 3. Signal K Server Discovery (mDNS/DNS-SD)

**Header:** `websocket/discovery/Discovery.h`

#### Types
- `struct SignalKService`
  - `std::string host;`
  - `int port;`
  - `std::string roles;`
  - `std::string self;`
  - `std::string swname;`
  - `std::string swvers;`

#### Functions
- `std::vector<SignalKService> discoverSignalKServers();`
  - Discover available Signal K servers on the local network (currently returns localhost:3000 as a placeholder).

---

## Example Usage
See `examples/websocket_stream_test.cpp` for a full example of requesting access, polling for approval, and streaming data from a Signal K server.

---

## Dependencies
- [ixwebsocket](https://github.com/machinezone/IXWebSocket)
- [nlohmann_json](https://github.com/nlohmann/json)

---

## To Do
- Implement real mDNS/DNS-SD discovery in `discoverSignalKServers()`
- Add reconnect and error handling logic to `WebSocketClient`
