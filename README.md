# DynamoDB Table Migration Tool

**DynamoDB Table Migration Tool** is a C++ utility that automates the process of creating DynamoDB tables for local development. It scans a directory of JSON configuration files and uses the AWS CLI to generate tables based on those configs.

## Overview

- Automatically creates DynamoDB tables from JSON files using the AWS CLI
- Designed to accelerate local dev and testing workflows
- Extracts the table name from JSON and handles table creation
- Scans a directory and processes all .json config files found
- Provides options like help text, debug logging, and forced overwrites
- Cross-platform C++ app using the AWS CLI and rapidjson

## Prerequisites

Before using this utility, ensure that you have the following prerequisites:

- [AWS CLI](https://aws.amazon.com/cli/) installed and configured with the necessary credentials.
- [CMake](https://cmake.org/) (for building the C++ application).

## Usage

1. Clone this repository to your local machine:

   ```
   git clone https://github.com/VMGWARE/DynamoDB-Table-Migration-Tool.git
   ```

2. Navigate to the cloned repository:

   ```
   cd DynamoDB-Table-Migration-Tool
   ```

3. Build the C++ application using CMake:

    - Linux/MacOS: Use the CMake command-line tools:

        ```
        mkdir build
        cd build
        cmake ..
        make
        ```

    - Windows: Use the CMake GUI to generate it, or use CLion to build the project. Alternatively, you can use the CMake command-line tools, something like this:

        ```
        mkdir build
        cd build
        cmake ..
        cmake --build .
        ```  

4. Run the application with desired options. Use the `-p` or `--path` option to specify the path to the directory containing your JSON files:

   ```
   ./dynamo-table-migrate -p /path/to/json/files
   ```

   You can also use the `-h` or `--help` option to display available command-line options:

   ```
   ./dynamo-table-migrate -h
   ```

   You also can use the `-f` or `--force` option to force the utility to overwrite existing tables. This option is disabled by default.

   ```
    ./dynamo-table-migrate -p /path/to/json/files -f
    ```

## JSON Configuration Format

Each JSON file in the specified directory should adhere to the format below. The utility extracts the `TableName` and other configuration details from each JSON file to create the corresponding DynamoDB table.

```json
{
    "TableName": "YourTableName",
    "KeySchema": [
        {
            "AttributeName": "PrimaryKey",
            "KeyType": "HASH"
        }
    ],
    "AttributeDefinitions": [
        {
            "AttributeName": "PrimaryKey",
            "AttributeType": "S"
        }
    ],
    "ProvisionedThroughput": {
        "ReadCapacityUnits": 1,
        "WriteCapacityUnits": 1
    }
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributions

Contributions are welcome! If you encounter issues or have improvements to suggest, feel free to open an issue or submit a pull request.

## Contact

For any questions or suggestions, please feel free to [contact the author](mailto:developers@vmgware.dev). Otherwise, you can find more information about the author below.

---

By [VMG Ware](https://github.com/VMGWARE)
