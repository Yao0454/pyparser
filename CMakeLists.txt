cmake_minimum_required(VERSION 3.16)
project(pyparser LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

add_executable(pyparser
    main.cpp
    source/lexer.cpp
    source/parser.cpp
    source/interpreter.cpp
)
target_include_directories(pyparser PRIVATE "${CMAKE_SOURCE_DIR}/include")
