cmake_minimum_required(VERSION 3.0)
project(DynamoDBAutoTableSetup)

add_executable(DynamoDBAutoTableSetup main.cpp)

# Add the path to the rapidjson headers
target_include_directories(DynamoDBAutoTableSetup PRIVATE ${CMAKE_SOURCE_DIR}/include)