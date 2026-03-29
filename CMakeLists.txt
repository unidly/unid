cmake_minimum_required(VERSION 4.2.3)

project(unid VERSION 0.1.0 LANGUAGES C CXX)

# This project requires C++20
# Update CPP and CXX in .bashrc to use gcc14-gcc, gcc14-g++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(server)
add_subdirectory(client)
#add_subdirectory(test)
