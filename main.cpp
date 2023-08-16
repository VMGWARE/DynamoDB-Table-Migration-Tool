#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h> // For getting the current working directory
#include <getopt.h> // For command-line option parsing
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h> // Use istreamwrapper to read from ifstream

using namespace std;
using namespace rapidjson;

// Global variables
bool force = false; // Force re-creation flag
bool debug = false; // Debug flag

// Debug logging macro
#define DEBUG_LOG(msg)                                   \
    do                                                   \
    {                                                    \
        if (debug)                                       \
            std::cout << "[DEBUG] " << msg << std::endl; \
    } while (0)

// Check if a table exists by attempting to describe it
bool tableExists(const string &tableName)
{
    DEBUG_LOG("Checking if table exists: " << tableName);

    // Use the AWS CLI to describe the table and check if it exists
    string command = "aws dynamodb describe-table --table-name " + tableName + " --output json > NUL 2>&1";
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

int main(int argc, char *argv[])
{

    const char *const short_opts = "hp:fd";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"path", required_argument, nullptr, 'p'},
        {"force", no_argument, nullptr, 'f'}, // Add force option
        {"debug", no_argument, nullptr, 'd'}, // Add debug option
    };

    string jsonDir;

    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'h':
            // Print usage
            cout << "Usage: " << argv[0] << " [OPTIONS]" << endl;
            cout << "Options:" << endl;
            cout << "  -h, --help         Show this help message and exit." << endl;
            cout << "  -p, --path         Specify the path to JSON directory." << endl;
            cout << "  -f, --force        Force re-creation of existing tables." << endl;
            cout << "  -d, --debug        Enable debug logging." << endl;
            return 0;

        case 'p':
            jsonDir = optarg;
            break;

        case 'f':
            force = true;
            break;

        case 'd':
            debug = true;
            break;

        default:
            cerr << "Usage: " << argv[0] << " [OPTIONS]" << endl;
            return 1;
        }
    }

    if (jsonDir.empty())
    {
        cerr << "Error: JSON directory path is required. Please specify with -p or --path." << endl;
        return 1;
    }

    // Handle case where path is "./" or "."
    if (jsonDir == "./" || jsonDir == ".")
    {
        char currentPath[FILENAME_MAX];
        if (getcwd(currentPath, sizeof(currentPath)) != nullptr)
        {
            jsonDir = currentPath;
        }
    }

    string awsCommandBase = "aws dynamodb create-table --cli-input-json file://";

    cout << "Loading JSON files from directory: " << jsonDir << endl;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(jsonDir.c_str())) != nullptr)
    {

        cout << endl
             << "Creating tables..." << endl;

        while ((ent = readdir(dir)) != nullptr)
        {
            string filename = ent->d_name;
            if (filename.size() > 5 && filename.substr(filename.size() - 5) == ".json")
            {
                string jsonFile = jsonDir + "/" + filename;
                DEBUG_LOG("Processing JSON file: " << jsonFile);
                string tableName = getTableNameFromJson(jsonFile);
                if (!tableName.empty())
                {
                    // Check if table already exists, moved here so it only runs once per file
                    bool tableAlreadyExists = tableExists(tableName);

                    if (tableAlreadyExists && !force)
                    {
                        cout << "  - Skipping " << filename << ", table already exists." << endl;
                    }
                    else
                    {
                        // Delete if force flag set and table exists
                        if (force && tableAlreadyExists)
                        {
                            string deleteCommand = "aws dynamodb delete-table --table-name " + tableName + " --endpoint-url http://localhost:8000 > NUL 2>&1";
                            int deleteResult = system(deleteCommand.c_str());
                            if (deleteResult != 0)
                            {
                                cerr << "  - Error deleting table for " << filename << "." << endl;
                            }
                            else
                            {
                                cout << "  -+ Deleted table for " << filename << "." << endl;
                            }
                        }

                        // Create table
                        string command = awsCommandBase + jsonFile + " --endpoint-url http://localhost:8000 > NUL 2>&1";
                        ;
                        int result = system(command.c_str());
                        if (result != 0)
                        {
                            cerr << "  - Error creating table for " << filename << "." << endl;
                        }
                        else
                        {
                            cout << "  + Created table for " << filename << "." << endl;
                        }
                    }
                }
                else
                {
                    cerr << "  - Could not get table name from " << filename << "." << endl;
                }
            }
        }

        closedir(dir);

        cout << endl
             << "Finished creating tables." << endl;
    }
    else
    {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }

    return 0;
}