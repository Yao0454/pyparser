cmake_minimum_required(VERSION 3.16)
project(pyparser LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/parser.cpp"
)

add_executable(pyparser main.cpp ${SRC_FILES})
target_include_directories(pyparser PRIVATE "${CMAKE_SOURCE_DIR}")
