cmake_minimum_required(VERSION 3.0)
project(DynamoDB-Table-Migration-Tool)

add_executable(dynamo-table-migrate main.cpp)

# Add the path to the rapidjson headers
target_include_directories(dynamo-table-migrate PRIVATE ${CMAKE_SOURCE_DIR}/include)