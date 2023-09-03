
/*!
 * DynamoDB Table Migration Tool
 * https://vmgware.dev/
 *
 * Copyright (c) 2023 VMG Ware
 * MIT Licensed
 */

#ifndef TABLE_MIGRATION_TOOL_H
#define TABLE_MIGRATION_TOOL_H

#include <string>
#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace std;
using namespace rapidjson;

// Global variables
extern bool force;
extern bool debug;
extern string appDir;
extern string tempDir;

// Debug logging macro
#define DEBUG_LOG(msg)                                   \
    do                                                   \
    {                                                    \
        if (debug)                                       \
            std::cout << "[DEBUG] " << msg << std::endl; \
    } while (0)

bool tableExists(const string &tableName);
bool canAccessDynamoDB();
string getTableNameFromJson(const string &jsonFilePath);
void printBanner();

#endif
