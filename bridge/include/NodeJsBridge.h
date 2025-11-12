#ifndef NODE_JS_BRIDGE_H
#define NODE_JS_BRIDGE_H

#include <string>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include "WppClient.h"
#include "WppTaskQueue.h"

namespace wpp {

/**
 * @class NodeJsBridge
 * @brief Bridge interface between Node.js and C++ LwM2M client
 * 
 * This class handles JSON commands from Node.js and executes corresponding
 * operations on the LwM2M client using the WppTaskQueue system.
 */
class NodeJsBridge {
public:
    struct ResourceUpdate {
        std::string objectId;
        std::string instanceId;
        std::string resourceId;
        std::string value;
        std::string dataType;
        time_t timestamp;
    };

    struct JsonCommand {
        std::string action;
        std::string objectId;
        std::string instanceId;
        std::string resourceId;
        std::string value;
        std::string dataType;
        time_t timestamp;
    };

    using CommandHandler = std::function<bool(const JsonCommand&, WppClient&)>;

private:
    std::map<std::string, CommandHandler> _commandHandlers;
    std::queue<JsonCommand> _commandQueue;
    std::mutex _queueMutex;
    bool _running;
    std::thread _processingThread;

public:
    NodeJsBridge();
    ~NodeJsBridge();

    /**
     * @brief Start the bridge processing
     */
    void start();

    /**
     * @brief Stop the bridge processing
     */
    void stop();

    /**
     * @brief Process a JSON command string from Node.js
     * @param jsonCommand JSON string containing the command
     * @return true if command was queued successfully
     */
    bool processJsonCommand(const std::string& jsonCommand);

    /**
     * @brief Send response back to Node.js
     * @param response JSON response string
     */
    void sendResponse(const std::string& response);

private:
    /**
     * @brief Initialize command handlers
     */
    void initializeHandlers();

    /**
     * @brief Parse JSON command string
     * @param jsonString JSON command string
     * @return Parsed JsonCommand structure
     */
    JsonCommand parseJsonCommand(const std::string& jsonString);

    /**
     * @brief Process commands from the queue
     */
    void processCommandQueue();

    /**
     * @brief Convert string value to appropriate LwM2M type and set resource
     */
    bool setResourceValue(WppClient& client, const std::string& objectId, 
                         const std::string& instanceId, const std::string& resourceId,
                         const std::string& value, const std::string& dataType);

    /**
     * @brief Get resource value and convert to string
     */
    std::string getResourceValue(WppClient& client, const std::string& objectId,
                                const std::string& instanceId, const std::string& resourceId);

    // Command handlers
    bool handleWriteResource(const JsonCommand& cmd, WppClient& client);
    bool handleReadResource(const JsonCommand& cmd, WppClient& client);
    bool handleNotifyResource(const JsonCommand& cmd, WppClient& client);
    bool handleUpdateResource(const JsonCommand& cmd, WppClient& client);
};

} // namespace wpp

#endif // NODE_JS_BRIDGE_H