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

bool tableExists(const string &tableName)
{
    string command = "aws dynamodb describe-table --table-name " + tableName + " --output json > NUL 2>&1";
    int result = system(command.c_str());
    return result == 0;
}

string getTableNameFromJson(const string &jsonFilePath)
{
    ifstream file(jsonFilePath);
    if (file.is_open())
    {
        IStreamWrapper inputStream(file);

        Document root;
        root.ParseStream(inputStream);

        if (!root.HasParseError() && root.HasMember("TableName") && root["TableName"].IsString())
        {
            return root["TableName"].GetString();
        }
    }
    return "";
}

int main(int argc, char *argv[])
{
    const char *const short_opts = "hp:";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"path", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, 0}};

    string jsonDir;

    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'h':
            cout << "Usage: " << argv[0] << " [OPTIONS]" << endl;
            cout << "Options:" << endl;
            cout << "  -h, --help         Show this help message and exit." << endl;
            cout << "  -p, --path PATH    Specify the path to JSON directory." << endl;
            return 0;
        case 'p':
            jsonDir = optarg;
            break;
        default:
            cerr << "Usage: " << argv[0] << " [OPTIONS]" << endl;
            return 1;
        }
    }

    if (jsonDir.empty())
    {
        cerr << "Error: You must provide a JSON directory using -p or --path." << endl;
        return 1;
    }

    // Handle the case where the provided path is "./" or similar
    if (jsonDir == "./" || jsonDir == ".")
    {
        char currentPath[FILENAME_MAX];
        if (getcwd(currentPath, sizeof(currentPath)) != nullptr)
        {
            jsonDir = currentPath;
        }
    }

    string awsCommandBase = "aws dynamodb create-table --cli-input-json file://";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(jsonDir.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            string filename = ent->d_name;
            if (filename.size() > 5 && filename.substr(filename.size() - 5) == ".json")
            {
                string jsonFile = jsonDir + "/" + filename;
                string tableName = getTableNameFromJson(jsonFile);
                if (!tableName.empty())
                {
                    string command = awsCommandBase + jsonFile + " --endpoint-url http://localhost:8000 > NUL 2>&1";
                    if (tableExists(tableName))
                    {
                        cout << "Warning: Table already exists for " << filename << endl;
                    }
                    else
                    {
                        int result = system(command.c_str());
                        if (result != 0)
                        {
                            cerr << "Warning: Table not created for " << filename << endl;
                        }
                        else
                        {
                            cout << "Table created successfully for " << filename << endl;
                        }
                    }
                }
                else
                {
                    cerr << "Error: TableName not found in " << filename << endl;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }

    return 0;
}
