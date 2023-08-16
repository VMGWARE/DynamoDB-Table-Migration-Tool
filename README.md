# DynamoDBAutoTableSetup

**DynamoDBAutoTableSetup** is an automated utility designed to streamline the process of creating DynamoDB tables from JSON configurations using the AWS Command Line Interface (CLI). This tool scans a directory containing JSON files, extracts the table name and configuration details from each JSON file, and automatically creates the corresponding DynamoDB table using the `aws dynamodb create-table` command. It currently is designed only for local development purposes, and is not intended for use in production environments.

## Prerequisites

Before using this utility, ensure that you have the following prerequisites:

- [AWS CLI](https://aws.amazon.com/cli/) installed and configured with the necessary credentials.
- [CMake](https://cmake.org/) (for building the C++ application).

## Usage

1. Clone this repository to your local machine:

   ```
   git clone https://github.com/VMGWARE/DynamoDBAutoTableSetup.git
   ```

2. Navigate to the cloned repository:

   ```
   cd DynamoDBAutoTableSetup
   ```

3. Build the C++ application using CMake:

    - Linux/MacOS:

        ```
        mkdir build
        cd build
        cmake ..
        make
        ```

    - Windows:

        ```
        mkdir build
        cd build
        cmake ..
        cmake --build .
        ```  

1. Run the application with desired options. Use the `-p` or `--path` option to specify the path to the directory containing your JSON files:

   ```
   ./DynamoDBAutoTableSetup -p /path/to/json/files
   ```

   You can also use the `-h` or `--help` option to display available command-line options:

   ```
   ./DynamoDBAutoTableSetup -h
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
