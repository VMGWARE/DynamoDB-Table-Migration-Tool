# DynamoDB Table Migration Tool

The **DynamoDB Table Migration Tool** is a utility written in C++ that automates the process of creating DynamoDB tables for local development. It scans a directory of JSON configuration files and uses the AWS CLI to generate tables based on those configurations.

## Overview

- Automatically creates DynamoDB tables from JSON files using the AWS CLI.
- Designed to accelerate local development and testing workflows.
- Extracts the table name from JSON and handles table creation.
- Scans a directory and processes all .json configuration files found.
- Provides options like help text, debug logging, and forced overwrites.
- Cross-platform C++ application using the AWS CLI and rapidjson.

## Prerequisites

Before using this utility, ensure that you have the following prerequisites installed:

- [AWS CLI](https://aws.amazon.com/cli/) with the necessary credentials set up.
- [CMake](https://cmake.org/) for building the C++ application.

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

   - For Linux/MacOS, use the CMake command-line tools:

     ```
     mkdir build
     cd build
     cmake ..
     make
     ```

   - For Windows, use the CMake GUI or CLion to build the project. Alternatively, you can use the CMake command-line tools:

     ```
     mkdir build
     cd build
     cmake ..
     cmake --build .
     ```

4. Run the application with the desired options. Use the `-p` or `--path` option to specify the path to the directory containing your JSON files:

   ```
   ./dynamo-table-migrate -p /path/to/json/files
   ```

   To display available command-line options, you can use the `-h` or `--help` option:

   ```
   ./dynamo-table-migrate -h
   ```

   You can also use the `-f` or `--force` option to force the utility to overwrite existing tables. By default, this option is disabled.

   ```
    ./dynamo-table-migrate -p /path/to/json/files -f
    ```

## JSON Configuration Format

Each JSON file in the specified directory should adhere to the following format. The utility extracts the `TableName` and other configuration details from each JSON file to create the corresponding DynamoDB table. Please make sure to follow the AWS JSON [Syntax](https://docs.aws.amazon.com/cli/latest/reference/dynamodb/create-table.html):

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

This project is licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file.

## Contributions

Contributions are welcome! If you encounter any issues or have improvements to suggest, feel free to open an issue or submit a pull request.

## Contact

For any questions or suggestions, please feel free to [contact the author](mailto:developers@vmgware.dev). You can also find more information about the author below.

---

By [VMG Ware](https://github.com/VMGWARE)
