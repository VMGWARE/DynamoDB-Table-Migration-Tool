#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h>  // For getting the current working directory
#include <getopt.h>  // For command-line option parsing

bool tableExists(const std::string& tableName) {
    std::string command = "aws dynamodb describe-table --table-name " + tableName + " --output json > NUL 2>&1";
    int result = std::system(command.c_str());
    return result == 0;
}

int main(int argc, char* argv[]) {
    const char* const short_opts = "hp:";
    const option long_opts[] = {
            {"help", no_argument, nullptr, 'h'},
            {"path", required_argument, nullptr, 'p'},
            {nullptr, 0, nullptr, 0}
    };

    std::string jsonDir;

    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Usage: " << argv[0] << " [OPTIONS]" << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "  -h, --help         Show this help message and exit." << std::endl;
                std::cout << "  -p, --path PATH    Specify the path to JSON directory." << std::endl;
                return 0;
            case 'p':
                jsonDir = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [OPTIONS]" << std::endl;
                return 1;
        }
    }

    if (jsonDir.empty()) {
        std::cerr << "Error: You must provide a JSON directory using -p or --path." << std::endl;
        return 1;
    }

    // Handle the case where the provided path is "./" or similar
    if (jsonDir == "./" || jsonDir == ".") {
        char currentPath[FILENAME_MAX];
        if (getcwd(currentPath, sizeof(currentPath)) != nullptr) {
            jsonDir = currentPath;
        }
    }

    std::string awsCommandBase = "aws dynamodb create-table --cli-input-json file://";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(jsonDir.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string filename = ent->d_name;
            if (filename.size() > 5 && filename.substr(filename.size() - 5) == ".json") {
                std::string jsonFile = jsonDir + "/" + filename;
                std::string tableName = filename.substr(0, filename.size() - 5);  // Remove ".json" extension
                std::string command = awsCommandBase + jsonFile + " --endpoint-url http://localhost:8000 > NUL 2>&1";
                if (tableExists(tableName)) {
                    std::cout << "Warning: Table already exists for " << filename << std::endl;
                } else {
                    int result = std::system(command.c_str());
                    if (result != 0) {
                        std::cerr << "Warning: Table not created for " << filename << std::endl;
                    } else {
                        std::cout << "Table created successfully for " << filename << std::endl;
                    }
                }
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }

    return 0;
}
