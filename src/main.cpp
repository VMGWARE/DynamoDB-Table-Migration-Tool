
/*!
 * DynamoDB Table Migration Tool
 * https://vmgware.dev/
 *
 * Copyright (c) 2023 VMG Ware
 * MIT Licensed
 */

#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h> // For getting the current working directory
#include <getopt.h> // For command-line option parsing
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h> // Use istreamwrapper to read from ifstream
// #include "spdlog/spdlog.h" // For logging
#include "utils/TableMigrationTool.h" // Include the TableMigrationTool header

// Namespaces
using namespace std;
using namespace rapidjson;


int main(int argc, char *argv[])
{
    // Parse command-line options
    const char *const short_opts = "hp:fd";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"path", required_argument, nullptr, 'p'},
        {"force", no_argument, nullptr, 'f'}, // Add force option
        {"debug", no_argument, nullptr, 'd'}, // Add debug option
    };

    string jsonDir;

    // Print banner
    printBanner();

// Set application directory to AppData on Windows, home directory on Linux
#ifdef _WIN32
    appDir = getenv("APPDATA") + string("\\DynamoDB-Table-Migration-Tool");

    // Create app directory if it doesn't exist
    if (system(("if not exist \"" + appDir + "\" mkdir \"" + appDir + "\" > NUL 2>&1").c_str()) != 0)
    {
        cerr << "Error: Unable to create application directory." << endl;
        return 1;
    }

    tempDir = appDir + string("\\temp");

    // Create temp directory if it doesn't exist
    if (system(("if not exist \"" + tempDir + "\" mkdir \"" + tempDir + "\" > NUL 2>&1").c_str()) != 0)
    {
        cerr << "Error: Unable to create temporary directory." << endl;
        return 1;
    }
#else
    appDir = getenv("HOME") + string("/.DynamoDB-Table-Migration-Tool");
    tempDir = appDir + string("/temp");
#endif

    // Parse command-line options
    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'h':
            // Print usage
            cout << "Usage: " << argv[0] << " [OPTIONS]" << endl;
            cout << "Description: This program reads JSON files from a directory, interacts with AWS DynamoDB using the AWS CLI,"
                 << endl;
            cout << "             and creates tables based on the table definitions in the JSON files." << endl;
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

    DEBUG_LOG("Starting program.");

    // Check if path is empty
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

    // Check if path is valid
    if (!canAccessDynamoDB())
    {
        cerr << "Error: Unable to access DynamoDB." << endl;
        return 1;
    }

    string awsCommandBase = "aws dynamodb create-table --cli-input-json file://";

    cout << "Loading JSON files from directory: " << jsonDir << endl;
    string tmpErrorFile = tempDir + "\\error.log"; // Temporary file to capture error messages
    cout << "Temporary error file: " << tmpErrorFile << endl;

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

                cout << "  Processing " << tableName << " table..." << endl;

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
                            string deleteCommand = "aws dynamodb delete-table --table-name " + tableName + " > NUL 2>&1";
                            int deleteResult = system(deleteCommand.c_str());
                            if (deleteResult != 0)
                            {
                                cerr << "  - Error deleting table for " << filename << "." << endl;
                            }
                            else
                            {
                                cout << "  + Deleted table for " << filename << "." << endl;
                            }
                        }

                        // Create table
                        string command = awsCommandBase + jsonFile + " > NUL 2>" + tmpErrorFile;
                        int result = system(command.c_str());
                        if (result != 0)
                        {
                            ifstream errorStream(tmpErrorFile);
                            if (errorStream.is_open())
                            {
                                string line;
                                cerr << "  - Error creating table for " << filename << ":\n";
                                while (getline(errorStream, line))
                                {
                                    cerr << "    " << line << "\n";
                                }
                                errorStream.close();
                            }
                            else
                            {
                                cerr << "  - Error creating table for " << filename << ", and couldn't read the error log." << endl;
                            }
                            remove(tmpErrorFile.c_str()); // Delete the temporary error file
                        }
                        else
                        {
                            cout << "  + Created table for " << filename << "." << endl;
                            remove(tmpErrorFile.c_str()); // Delete the temporary error file if it exists
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