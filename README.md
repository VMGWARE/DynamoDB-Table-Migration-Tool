# DynamoDB JSON Setup

**dynamodbJsonSetup** is a C++ utility that automates the process of creating DynamoDB tables from JSON files using the AWS Command Line Interface (CLI). This tool scans a directory containing JSON files, extracts the table name from each JSON file, and creates the corresponding DynamoDB table using the `aws dynamodb create-table` command.

## Prerequisites

Before using this utility, make sure you have the following prerequisites:

- [AWS CLI](https://aws.amazon.com/cli/) installed and configured with the necessary credentials.
- [CMake](https://cmake.org/) (for building the C++ application).

## Usage

1. Clone this repository to your local machine:

   ```
   git clone https://github.com/Codycody31/dynamodbJsonSetup.git
   ```

2. Navigate to the cloned repository:

   ```
   cd dynamodbJsonSetup
   ```

3. Build the C++ application using CMake:

   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the application with the desired options. Use the `-p` or `--path` option to specify the path to the directory containing your JSON files:

   ```
   ./dynamodbJsonSetup -p /path/to/json/files
   ```

   You can also use the `-h` or `--help` option to display the available command-line options:

   ```
   ./dynamodbJsonSetup -h
   ```

## JSON File Format

Each JSON file in the specified directory should follow a format similar to the example below. The utility extracts the `TableName` from each JSON file to create the corresponding DynamoDB table.

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

Contributions are welcome! If you find any issues or have improvements to suggest, feel free to open an issue or submit a pull request.

## Contact

For any questions or suggestions, please feel free to [contact the author](mailto:me@vahngomes.dev).

---

By [Vahn Gomes](https://github.com/Codycody31)
