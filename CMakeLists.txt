cmake_minimum_required(VERSION 3.5)
project(DynamoDB-Table-Migration-Tool)

# Collect all source files in src and src/utils
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/utils/*.cpp")

add_executable(dynamo-table-migrate ${SOURCES})

# Add the path to the rapidjson headers
target_include_directories(dynamo-table-migrate PRIVATE ${CMAKE_SOURCE_DIR}/include)
