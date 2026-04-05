cmake_minimum_required(VERSION 4.2.3)

project(unid VERSION 0.1.0 LANGUAGES C CXX)

# This project requires C++20
# This project uses gcc14-gcc and gcc14-g++
# See environment variables CPP and CXX in .bashr
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# This project results in two executables: unid_server and unid_client
# Common project libraries are maintained and compiled in common
# Project unit tests are maintained and compiled in test
add_subdirectory(server)
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(test)
