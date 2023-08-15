cmake_minimum_required(VERSION 3.0)
project(dynamodbJsonSetup)

add_executable(dynamodbJsonSetup main.cpp)

# Add the path to the rapidjson headers
target_include_directories(dynamodbJsonSetup PRIVATE ${CMAKE_SOURCE_DIR}/include)