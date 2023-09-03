
/*!
 * DynamoDB Table Migration Tool
 * https://vmgware.dev/
 *
 * Copyright (c) 2023 VMG Ware
 * MIT Licensed
 */

#include "TableMigrationTool.h"

bool force = false;
bool debug = false;
string appDir;
string tempDir;

// Check if a table exists by attempting to describe it
bool tableExists(const string &tableName)
{
    DEBUG_LOG("Checking if table exists: " << tableName);

    // Use the AWS CLI to describe the table and check if it exists
    string command = "aws dynamodb describe-table --table-name " + tableName + " --output json > NUL 2>&1";
    int result = system(command.c_str());
    return result == 0;
}

// Check if DynamoDB can be accessed
bool canAccessDynamoDB()
{
    DEBUG_LOG("Checking if DynamoDB can be accessed.");

    // Use the AWS CLI to describe the table and check if it exists
    string command = "aws dynamodb list-tables --output json > NUL 2>&1";
    int result = system(command.c_str());
    return result == 0;
}

// Get the table name from a JSON file
string getTableNameFromJson(const string &jsonFilePath)
{
    DEBUG_LOG("Getting table name from JSON: " << jsonFilePath);

    ifstream file(jsonFilePath);
    if (file.is_open())
    {
        // Parse JSON using rapidjson
        IStreamWrapper inputStream(file);

        Document root;
        root.ParseStream(inputStream);

        if (!root.HasParseError() && root.HasMember("TableName") && root["TableName"].IsString())
        {
            string tableName = root["TableName"].GetString();
            DEBUG_LOG("Extracted table name: " << tableName);
            return tableName;
        }
    }

    DEBUG_LOG("Table name extraction failed.");
    return "";
}

// Print banner
void printBanner()
{
    cout << "#######################################\n";
    cout << "#                                     #\n";
    cout << "#    DynamoDB Table Migration Tool    #\n";
    cout << "#                                     #\n";
    cout << "#######################################\n\n";
    cout << "This utility reads JSON files from a directory and creates\n";
    cout << "DynamoDB tables based on the table definitions in the JSON files.\n";
    cout << "Please ensure you have the AWS CLI configured and accessible.\n";
    cout << "---------------------------------------\n\n";
}