#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#include <memory>
#include "Connection.h"

#include <string>
// Factory function to create a Connection object from JSON config
std::unique_ptr<Connection> createConnectionFromConfig();

// Function to get client name (endpoint) from config
std::string getClientNameFromConfig();

#endif // READ_CONFIG_H
